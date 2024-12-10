#include <sys/param.h>
#include <errno.h>
#include "comm.h"
#include <gdbm.h>

int verbose = 0;

/*
 * This deamon does the following:
 *   1. Sleep until I/O interrupt on the GUI port.
 *   2. Process the following commands from the GUI:
 *      a. add neighbor x
 *      b. del neighbor x
 *      c. add gui      x.n
 *           The only thing that we do is to report the time to
 *           the GUI.  In order for the GUI to recieve the job
 *           status database, it has to issue a 'reset' command.
 *      
 *      d. del job trmm time1 time2 J0002 /dev/8mm2 cdrom
 *           (Only time2 is important).
 *      e. add psgui x.n
 *           (Note: send to NEIGHOBOR_PORT instead of
 *            FROM_GUI_PORT.  This will limit getting ps information
 *            from only one computer.)
 *      f. reset gui x.n [host]
 *              Where [host] is a specific host to request a reset from.
 *
 *   3. Wake up every 10 seconds and send to all GUI's  (psgui's) the
 *      status of all programs, if they have requested it.
 *   4. When communicating with our neighbors, simply fake
 *      the command as if it was initiated from a GUI.
 */

/*
 * How to use this deamon.
 *
 * The syntax of each command is listed above and here I'll show
 * the order you should issue commands to this deamon.
 *
 * The GUI should:
 *   1. send 'add gui guihost.port#'
 *      Ex., 'add gui raindrop2.1433'
 *
 *      This registers the GUI to the deamon so that updates
 *      to the job database will be automatically sent to the
 *      GUI.  The deamon receives job update via 'send_status' commands.
 *    
 *      After receiving the 'add gui', the time is sent to the GUI so that
 *      elapsed times can be correctly displayed even when the two
 *      computers have different dates; this is a time correction.
 *
 *      The 'add gui' is echoed to all neighbors.
 *
 *   2. send 'reset gui guihost.port# server_host'
 *      Ex., 'reset gui trmm.2333 raindrop1'
 *
 *      The GUI should send this request after it has received the
 *      time information as a result of the 'add gui' above.
 *
 *   After 1 and 2 are done, you can send 'reset gui' at any time.  It is
 *   important to perform 2 only after the time has been received by the
 *   GUI.
 *
 *   3. 'add neighbor x' and 'del neighbor x' can be sent in any order
 *      and at any time.
 *
 *   4. 'del job ...' can be sent at any time.
 *
 *   5. 'add psgui x.n' can be sent at any time.
 *
 */

#include "ports.h"
#include "monitorps.h"
#ifdef __linux
#define LOCK_IT(fp) flock(fileno(fp), LOCK_EX)
#define UNLOCK_IT(fp) flock(fileno(fp), LOCK_UN)
#else
#define LOCK_IT(fp) lockf(fileno(fp), F_LOCK, 0)
#define UNLOCK_IT(fp) lockf(fileno(fp), F_ULOCK, 0)
#endif

char monitorps_buffer[STRLEN];
char *monitorps_bufptr;
int monitorps_bufsiz;

int monitorps_socket;
int pipefd[2]; /* Pipe data between child and parent. The data transmitted
                *  will be the 'Lists' structure. 
				*/


Lists lists;  /* This structure is sent from children when add's and del's
			   * happen.  This is a bit slicker than using shared memory
			   * in that I don't have to worry about race conditions.
			   */

int signal_mask;  /* Used to block child processes; one child at a time. */
sigset_t lsigset, old_lsigset;
sigset_t zeromask;
int need_to_send_sigio = 0;

#include <sys/wait.h>
/***********************************************************************/
/*                                                                     */
/*                    fireman                                          */
/*                                                                     */
/***********************************************************************/
void fireman(void)
{ int pid;

  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);
  while ((pid =waitpid(-1, NULL, WNOHANG)) > 0) {
	/* Ok, let another child be born. */
	continue;
  }
  signal(SIGCHLD, (void(*)(int))fireman);
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
}

/***********************************************************************/
/*                                                                     */
/*                    add_member                                       */
/*                                                                     */
/***********************************************************************/
int add_member(char *item, char array[][STRLEN], int *n_array)
{
  /* Check that the item is not in the array, then add it. */
  /* One day we'll use gdbm. */
  int i;

  if (verbose) printf("ROUTINE add_member... n_array is %d\n", *n_array);
  for (i=0; i<*n_array; i++) {
	if (strcmp(item, array[i]) == 0) /* Oops, found it. */
	  return 0;
  }
  strcpy(&array[(*n_array)++][0], item);
  if (verbose) printf("ADDED %s at index %d\n", item, *n_array);
  return 1;
}

/***********************************************************************/
/*                                                                     */
/*                    del_member                                       */
/*                                                                     */
/***********************************************************************/
void del_member(char *item, char array[][STRLEN], int *n_array)
{
  /* Find the item and remove it by colapsing the list. */
  /* One day we'll use gdbm. */
  int i, ii, j;
  for (i=0; i<*n_array; i++) {
	if (strcmp(item, array[i]) == 0) /* Found it. */ {
	  for (ii = i, j=i+1; j<*n_array; j++, ii++)
		strcpy(array[ii], array[j]);
	  (*n_array)--;
	  return;
	}
  }
/* Not found. */
  return;
}

/***********************************************************************/
/*                                                                     */
/*                    print_list                                       */
/*                                                                     */
/***********************************************************************/
void print_list(char *title, char array[][STRLEN], int n_array)
{
  /* Check that the item is not in the array, then add it. */
  int i;
  for (i=0; i<n_array; i++) {
	if (verbose) printf("%s, item %d: %s\n", title, i, array[i]);
  }
  return;
}



#define STATLEN 20000
typedef char Prog_stat[STATLEN];

/***********************************************************************/
/*                                                                     */
/*                    open_gui_entry                                   */
/*                                                                     */
/***********************************************************************/
int open_gui_entry(char *gui_entry)
{
  int s;
  unsigned short to_gui_port;
  char cmd[STRLEN];
  char *gui;
  char gui_save[STRLEN];
  int ntry;

  ntry = 0;
  strcpy(gui_save, gui_entry); /* Work w/ a safe copy. */ 
  gui = strtok(gui_save,".");
  sscanf(gui,"%s",gui); /* Remove leading spaces!!! */ 
  to_gui_port = (unsigned short)atoi(strtok(NULL," "));

  if (verbose) printf("OPEN_GUI_ENTRY: call_socket(%s, %d);\n", gui, to_gui_port);
  s = call_socket(gui, to_gui_port);
  if (s < 0) {
	if (errno == ETIMEDOUT) {
	  if (verbose) printf("============================> ETIMEDOUT to %s\n", gui);
	  return -1 ; /* Give up. */
	}
	if (verbose) perror(gui);
	sprintf(cmd, "from_neighbor: del gui %s.%d\n", gui, to_gui_port);
	process_command(cmd);
	sprintf(cmd, "from_neighbor: del product_gui %s.%d\n", gui, to_gui_port);
	process_command(cmd);
	return s;
  }
  return s;
}

/***********************************************************************/
/*                                                                     */
/*                          send_stat                                  */
/*                                                                     */
/***********************************************************************/
void send_stat(char *prog_stat, int nlen, char *gui)
{
  int s;
  int n;
  s = open_gui_entry(gui);
  if (s < 0) {
	if (verbose) perror("send_stat");
	return;
  }

  n = write_data(s, prog_stat, nlen);
  if (verbose) fprintf(stderr, "send_stat wrote %d bytes.\n", n);
  close(s);
  return;
}


/***********************************************************************/
/*                                                                     */
/*                    report_job_status_to_gui                         */
/*                                                                     */
/***********************************************************************/
void report_job_status_to_gui(char *gui, char *status_file)
{
  FILE *fp;
  char line[1000];
  int n;
  int s;
  
  if (verbose) fprintf(stderr, "report_job_status_to_gui %s\n", gui);
  sprintf(line, "list_job_status_db %s | sort +2 -4 +4 -5 +6 -7 +8 -9r",
		  JOB_STATUS_FILE);
  fp = popen(line, "r");

  if (fp == NULL) {
	if (verbose) perror(JOB_STATUS_FILE);
	return;
  }
  s = open_gui_entry(gui);
  if (s<=0) {
	if (verbose) perror("report_job_status_to_gui");
	pclose(fp);
	return; /* No connection. */
  }

/* Write the entire file to the open socket. */
  while ((n = fread(line, sizeof(char), sizeof(line), fp)) > 0) {
	/*	fprintf(stderr,"%s\n", line); */
	write_data(s, line, n);
  }
  close(s);
  pclose(fp);
}

#include <sys/time.h>
#include <unistd.h>
/***********************************************************************/
/*                                                                     */
/*                    report_time_to_gui                               */
/*                                                                     */
/***********************************************************************/
void report_time_to_gui(char *gui)
{
  struct timeval tv;
  char line[1000];
  int n;
  int s;
  
  if (verbose) fprintf(stderr, "report_time_to_gui %s\n", gui);
  gettimeofday(&tv, NULL);

  sprintf(line, "time: %d", (int)tv.tv_sec);
  n = strlen(line);

  s = open_gui_entry(gui);
  if (s<=0) {
	if (verbose) perror("report_time_to_gui");
	return; /* No connection. */
  }

  write_data(s, line, n);

  close(s);
}

/***********************************************************************/
/*                                                                     */
/*                    tell_neighbor                                    */
/*                                                                     */
/***********************************************************************/
void tell_neighbor(char *neighbor, char *cmd)
{
  int s;
  int ntry;
  int n;

  if (fork() != 0) return;
  ntry = 0;

  if (verbose) printf("tell_neighbor: %s %s\n", neighbor, cmd);
  s = call_socket(neighbor, MONITORPS_PORT);
  if (s < 0) {
	if (errno == ETIMEDOUT) {
	  if (verbose) printf("============================> ETIMEDOUT to %s\n", neighbor);
	  exit(-1); /* Give up. */
	}
	if (verbose) perror(neighbor);
	exit(-1);  /* Cannot connect to this neighbor. */
  }
  n =  write_data(s, cmd, strlen(cmd)+1);
  if (verbose) printf("Wrote %d bytes.\n", n);
  if (verbose) printf("tell_neighbor: Just told %s the command %s, socket =%d\n", neighbor, cmd, s);
  if(close(s) != 0) if (verbose) perror(neighbor);

  if (verbose) printf("Leaving tell_neighbor\n");
  exit(0);
}

/***********************************************************************/
/*                                                                     */
/*                    tell_neighbors                                   */
/*                                                                     */
/***********************************************************************/
void tell_neighbors(char *cmd)
{
  int i;
  char myhostname[STRLEN];

  gethostname(myhostname, sizeof(myhostname));
  for (i=0; i<lists.n_neighbors; i++) {
	/* Don't send the command to ourself again or we'll never shut up. */
	if (! strcasecmp(lists.neighbor[i], myhostname)) continue; /* Skip sending to
																* ourself.
																*/
	tell_neighbor(lists.neighbor[i], cmd);
  }
}

void tell_neighbor_the_guis(char *neighbor)
{
  int i;
  char cmd[STRLEN];

  for (i=0; i<lists.n_guis; i++) {
	sprintf(cmd, "from_neighbor: add gui %s\n", lists.gui[i]);
	tell_neighbor(neighbor, cmd);
  }
}

void tell_neighbor_the_neighbors(char *neighbor)
{
  int i;
  char cmd[STRLEN];

  if (verbose) printf("tell_neighbor_the_neighbors(%s)\n", neighbor);
  for (i=0; i<lists.n_neighbors; i++) {
	sprintf(cmd, "from_neighbor: add neighbor %s\n", lists.neighbor[i]);
	if (verbose) printf(">>> %s\n", cmd);
	tell_neighbor(neighbor, cmd);
  }
}

void tell_neighbors_the_guis(void)
{
  int i;
  char cmd[STRLEN];
  for (i=0; i<lists.n_guis; i++) {
	sprintf(cmd,"from_neighbor: add gui %s\n", lists.gui[i]);
	tell_neighbors(cmd);
  }
}
void tell_neighbors_the_neighbors(void)
{
  int i;
  char cmd[STRLEN];
  for (i=0; i<lists.n_neighbors; i++) {
	sprintf(cmd,"from_neighbor: add neighbor %s\n", lists.neighbor[i]);
	tell_neighbors(cmd);
  }
}

void write_new_neighbor_file(char list[][STRLEN], int n, char *file)
{
  FILE *fp;
  int i;
  fp = fopen(file, "w");
  LOCK_IT(fp);
  for (i=0; i<n; i++)
	fprintf(fp, "%s\n", list[i]);
  UNLOCK_IT(fp);
  fclose(fp);
}


#include <gdbm.h>
/***********************************************************************/
/*                                                                     */
/*                    del_job                                          */
/*                                                                     */
/***********************************************************************/
void del_job(char *entry, char *file)
{
  char cmd[500];

  /* Typical content for entry is:
   * trmm time1 time2 J0002 /dev/8mm2 cdrom
   */
  /* The time entries are seconds from jan 1, 1970 (Unix time()) */

  /* This algorithm is simple.  Delete all entries that match
   * the 'computer', 'tapeid', 'device', 'product' for all times
   * between Stime and Etime.
   */

  sprintf(cmd, "delete_job_from_jobdb -f %s %s", file, entry);
  if(verbose)printf("%s\n", cmd);
  system(cmd);
}

/***********************************************************************/
/*                                                                     */
/*                       send_command                                  */
/*                                                                     */
/***********************************************************************/
void send_command(char *tohostport, char *msg_arg)
{
  /* 'tohostport' == "host.port", ex. "trmm.2342" */
  char tohostport_save[STRLEN];
  char *hoststr, *portstr;
  int portno;
  int ntry;
  int s;
  char msg[STRLEN];

  strcpy(tohostport_save, tohostport);
  hoststr = strtok(tohostport_save, ".");
  portstr = strtok(NULL, "");
  sscanf(portstr, "%d", &portno);
  sprintf(msg,"command: %s", msg_arg);

  s = call_socket(hoststr, (unsigned short)portno);
  ntry = 0;
  if (s < 0) {
	if (errno == ETIMEDOUT) {
	  if (verbose) printf("============================> ETIMEDOUT to %s\n", hoststr);
	  return; /* Give up. */
	}
	if (verbose) perror(hoststr);
	return;  /* Cannot connect. */
  }
  if (verbose) printf("SEND_COMMAND: %s\n", msg);
  ntry = write_data(s, msg, strlen(msg)+1);
  if (verbose) printf("WROTE %d bytes to %s at port %d\n", ntry, hoststr, portno);
  if(close(s) != 0) if (verbose) perror(hoststr);
}



/***********************************************************************/
/*                                                                     */
/*                    tell_guis_a_command                              */
/*                                                                     */
/***********************************************************************/
void tell_guis_a_command(char cmd[])
{
  int i;
  for(i=0; i<lists.n_guis; i++) { /* Don't worry about sending to
								   * the local machine gui, they'll ignore
								   * it.
								   */
	send_command(lists.gui[i], cmd);
  }
}



/***********************************************************************/
/*                                                                     */
/*                    send_image_to_gui                                */
/*                                                                     */
/***********************************************************************/
void send_image_to_gui(char *key_str, char gui_list[][STRLEN], int nguis)
{
  GDBM_FILE gf;
  datum key, key2, date_key,  content, date_content;
  char *bigbuf;
  int j, n;
  
  if (verbose) printf("Routine: send_image_to_gui\n");
  /* First find it in the data base. */
  gf = gdbm_open(PRODUCT_DB_FILE, 512, GDBM_READER, 0, 0);
  if (gf == NULL) {
	fprintf(stderr, "Failed to open gdbm.  File: %s.\n", PRODUCT_DB_FILE);
  }
  key.dptr = key_str;
  key.dsize = strlen(key.dptr)+1;
  content = gdbm_fetch(gf, key);
  /* This 'content' is a key to the real image. */
  if (content.dptr == NULL) {
	if (verbose) printf("Entry <%s> not found in %s\n",
						key_str, PRODUCT_DB_FILE);
    gdbm_close(gf);
	return;
  }
  key2.dptr = (char *)calloc(4+content.dsize, sizeof(char));
  sprintf(key2.dptr,"GIF %s", content.dptr);
  key2.dsize = strlen(key2.dptr) + 1;

  /* Get the date entry. */
  date_key.dptr = (char *)calloc(5+content.dsize, sizeof(char));
  sprintf(date_key.dptr,"DATE %s", content.dptr);
  date_key.dsize = strlen(date_key.dptr) + 1;

  date_content = gdbm_fetch(gf, date_key);
  if (verbose) printf("Date lookup: KEY<<%s>>, Content<<%s>>\n",
					  date_key.dptr, date_content.dptr);

  content = gdbm_fetch(gf, key2);  /* I use 'key2' because 'key' is
									* to be the filename when sent 
									* to the GUI.
									*/


  if (content.dptr != NULL) {
	/* Remember that key.dsize counts \0, but it gets squeezed out. */
	bigbuf = (char *)calloc(key.dsize+content.dsize, sizeof(char));
	memcpy(bigbuf, key.dptr, key.dsize);
	bigbuf[key.dsize-1] = ':';
	memcpy(&bigbuf[key.dsize], content.dptr, content.dsize);
	
	for (j=0; j < nguis; j++) {
	  if (verbose) printf("Sending <%s> to <%s>\n", key.dptr, gui_list[j]);
	  send_stat(bigbuf, content.dsize+key.dsize, (char *)gui_list[j]);
	}
	free(bigbuf);
	
  } else {
	if (verbose) printf("Entry <%s> not found in %s\n",
						key_str, PRODUCT_DB_FILE);
  }

  /* Now send the date/time. */
  /* This will have the form: DATE <prod> time_in_sec */
  
  if (date_content.dptr != NULL) {
	char *prod, *prod_str;
	if (verbose) printf("Now, send DATE\n");
	prod_str = strdup(key.dptr);
	prod = strtok(prod_str," "); /* Discard "LAST" */
	prod = strtok(NULL,"");  /* The product. */
	if (verbose) printf("Product is <%s>\n", prod);
	n = strlen(prod)+date_content.dsize+5+1;
	bigbuf = (char *)calloc(n, sizeof(char));
	sprintf(bigbuf, "DATE %s %s", prod, date_content.dptr);

	for (j=0; j < nguis; j++) {
	  if (verbose) printf("Sending <%s> to <%s>\n", bigbuf, gui_list[j]);
	  send_stat(bigbuf, n, (char *)gui_list[j]);
	}
	free(bigbuf);
	
  } else {
	if (verbose) printf("Entry <%s> not found in %s\n",
						date_key.dptr, PRODUCT_DB_FILE);
  }
    
  gdbm_close(gf);
}

/***********************************************************************/
/*                                                                     */
/*                    process_command                                  */
/*                                                                     */
/***********************************************************************/
void process_command(char cmd[])
{
  if (verbose) printf("Call yyparse()\n");
  if (verbose) printf("cmd = <%s>\n", cmd);
  /* monitorps_buffer is the input buffer for the parser. */
  strcpy(monitorps_buffer, cmd);
  monitorps_bufptr = monitorps_buffer;
  monitorps_bufsiz = strlen(monitorps_buffer);
  yyparse(); /* All routines are in protocol.y, including forking. */

  if (verbose) printf("n_guis %d, n_neighbors %d\n",
					  lists.n_guis, lists.n_neighbors);
  if (verbose) print_list("GUI",     lists.gui,      lists.n_guis);
  if (verbose) print_list("NEIGHBOR",lists.neighbor, lists.n_neighbors);
  
}
  
void log_job_status(char *cmd)
{
  /* Append this to a well known file. */
  char sys_cmd[500];

  sprintf(sys_cmd, "update_job_status_gdbm -f %s %s", JOB_STATUS_FILE, cmd);
  system(sys_cmd);
}


/***********************************************************************/
/*                                                                     */
/*                    do_job_status_commands                           */
/*                                                                     */
/***********************************************************************/
void do_job_status_commands(char *cmd)
{
  /* Basically, echo the 'cmd' to all GUI's. */
  int j;

/*
 * Call process_command to record this status message somewhere.
 */
  if (strncmp("product", cmd, 7) == 0) {
	/*
	 * product <prod name> <tapeid> <job#> <giffile>
	 *
	 * Where: <prod name> is: 1C-51, 2A-52i, ..., 2A-55.
     *        <file>      is: a GIF image.
	 */
	char *cmd_cpy;
	char *cmd_str, *prod_str, *tape_str, *jobnum_str, *file_str;
	char *date_str, *time_str;
	char sys_cmd[500], str[500];

	cmd_cpy = strdup(cmd); /* Work w/ safe copy. */
	cmd_str = strtok(cmd_cpy, " ");
	prod_str = strtok(NULL, " ");
	tape_str = strtok(NULL, " ");
	jobnum_str = strtok(NULL, " ");
	date_str = strtok(NULL, " ");
	time_str = strtok(NULL, " ");
	file_str = strtok(NULL, "");
	/* Store <GIF prod tape job#> [gif image] */
	sprintf(sys_cmd, "gdbm_store_from_stdin -f %s 'GIF %s %s %s' < %s",
			PRODUCT_DB_FILE, prod_str,  tape_str, jobnum_str, file_str);
	if (verbose) fprintf(stderr, "%s\n", sys_cmd);
	system(sys_cmd);
	
	/* Store <DATE prod tape job#> [time_in_sec] */
	sprintf(sys_cmd, "gdbm_store -f %s 'DATE %s %s %s' '%s %s'",
			PRODUCT_DB_FILE, prod_str,  tape_str, jobnum_str, date_str, time_str );
	if (verbose) fprintf(stderr, "%s\n", sys_cmd);
	system(sys_cmd);

	/* Store <LAST prod> [<GIF prod tape job#>] */
	sprintf(sys_cmd, "gdbm_store -f %s 'LAST %s' '%s %s %s'",
			PRODUCT_DB_FILE, prod_str,
			prod_str, tape_str, jobnum_str);
	if (verbose) fprintf(stderr, "%s\n", sys_cmd);
	system(sys_cmd);

	/* Tell all the GUIs */
	/* We send the IMAGE!  Sending a GIF image. */
	sprintf(str, "LAST %s", prod_str);
	if (verbose) printf("Calling send_image_to_gui\n");
	send_image_to_gui(str, lists.product_gui, lists.n_product_guis);
  } else if (strncmp("status", cmd, 6) == 0) {
	
  }

  return;
}

/***********************************************************************/
/*                                                                     */
/*                    do_something                                     */
/*                                                                     */
/***********************************************************************/
void do_something(int s)
{
  char buf[STRLEN];
  int n;

  if (verbose) printf("do_something:\n"); 
  n = read_data(s, buf, sizeof(buf));
  if (n <= 0) {
	if (verbose) fprintf(stderr, "do_something: read_data %d bytes.\n", n);
	if (verbose) perror("do_something");
	return;
  }
  buf[n] = '\0';
  process_command(buf);
}

/***********************************************************************/
/*                                                                     */
/*                    read_socket                                      */
/*                                                                     */
/***********************************************************************/
void read_socket(int s)
{
  int t;
  struct sockaddr_in from;
  int from_len;

/* 't' is a complete 5-tuple address specification.
 * 's' is only a 3-tuple address specification; ie. it is unknown
 *     who is connecting to us.
 *
 * We can comunicate on 't' and listen on 's' simultaneously.
 */
  while((t= get_connection(s, &from, &from_len)) < 0) { /* get a connection */
	if (errno == EINTR) {
	  continue;   /* EINTR might happen on accept(), */
	}
	if (errno == EWOULDBLOCK) {
	  if (verbose) perror("read_socket");
	  return;
	}
	if (verbose) perror("accept: read_socket");               /* bad */
	return;
  }

  if (verbose) {
	char *peername;
	peername = inet_ntoa(from.sin_addr);
	printf("READ_SOCKET: peer=%s  using port %d.\n", peername, ntohs(from.sin_port));
  }

  do_something(t);
  close(t);
  return;
}
/***********************************************************************/
/*                                                                     */
/*                    io_handler                                       */
/*                                                                     */
/***********************************************************************/
void io_handler(int n)
{
  fd_set read_template;
  struct timeval wait;
  int rc;

  /* This section is critical. */

  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);

  if (verbose) printf("\n... In io_handler. pid=%d\n", getpid());

  wait.tv_sec = 1;
  wait.tv_usec = 0; /* Don't wait, there *is* something pending. */
  FD_ZERO(&read_template);
  FD_SET(monitorps_socket, &read_template);

#ifdef __hpux
#define SELTYP int *
#else
#define SELTYP fd_set *
#endif
  rc = select(FD_SETSIZE, (SELTYP) &read_template, (SELTYP) 0, (SELTYP) 0, &wait);
  
  signal(SIGIO, (void(*)(int))io_handler);
  /* Who caused this interrupt? */
  if (rc <= 0) {
	if (verbose) printf("\n... Leaving io_handler rc=%d.\n", rc);
	sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
	return;
  }
  
  if (FD_ISSET(monitorps_socket, &read_template)) {
	/* Howdy */
	read_socket(monitorps_socket);
  }

  need_to_send_sigio = 1;
  if (verbose) printf("\n... Leaving io_handler. PID=%d\n", getpid());
  return;
}
/***********************************************************************/
/*                                                                     */
/*                    initialize_communication                         */
/*                                                                     */
/***********************************************************************/
void initialize_communication(void)
{
  if ((monitorps_socket= establish_async(MONITORPS_PORT)) < 0) {
    if (verbose) perror("establish monitorps_socket");
    exit(1);
  }

  signal(SIGIO, (void(*)(int))io_handler);
  signal(SIGCHLD, (void(*)(int))fireman); /* For the time consuming
										   * things, we fork.
										   */

}

/***********************************************************************/
/*                                                                     */
/*                    tell_progs_to_neighbors                          */
/*                                                                     */
/***********************************************************************/
void tell_progs_to_neighbors(char progs[][STRLEN], int n_progs,
							  char neighbor[][STRLEN], int n_neighbors)
{
  int i, j;
  for (i=0; i < n_progs; i++)
	for (j=0; j < n_neighbors; j++)
	  if (verbose) printf("PROGRAM: %s is told to neighbor %s.\n", progs[i], neighbor[j]);
}
  
/***********************************************************************/
/*                                                                     */
/*                    initialize_list                                  */
/*                                                                     */
/***********************************************************************/
void initialize_list(char list[][STRLEN], int *n, char *infile)
{
  FILE *fp;
  char s[STRLEN];

  *n = 0;
  if ((fp = fopen(infile, "r")) == NULL) {
	if (verbose) perror(infile);
	return;
  }
  while (fscanf(fp, "%s\n", s) == 1) {
	add_member(s, lists.neighbor, &lists.n_neighbors);
  }

  fclose(fp);
  return;
}

Prog_stat prog_stat;
void load_psinfo(char *buffer);

/***********************************************************************/
/*                                                                     */
/*                    remove_dead_guis                                 */
/*                                                                     */
/***********************************************************************/
void remove_dead_guis(char gui_list[][STRLEN], int n_guis, char *type)
{
/*
 * 'type' can be 'gui' or 'product_gui'
 */
  int s,i;
  char *gui;
  char gui_save[20];
  unsigned short to_gui_port;
  char cmd[STRLEN];

  if (n_guis > 0) { /* Check that we can connect.  If not, delete. */
	for (i=0; i<n_guis; i++) {
	  strcpy(gui_save, gui_list[i]); /* Work w/ a safe copy. */
	  gui = strtok(gui_save,".");
	  sscanf(gui,"%s",gui); /* Remove leading spaces!!! */
	  to_gui_port = (unsigned short)atoi(strtok(NULL," "));

	  s = call_socket(gui, to_gui_port);
	  if (s < 0) {
		if (errno == ETIMEDOUT) {
		  if (verbose) printf("============================> ETIMEDOUT to %s\n", gui);
		  return;
		}
		sprintf(cmd, "from_neighbor: del %s %s.%d", type, gui, to_gui_port);
		process_command(cmd);
	  }
	  close(s);
	}
  }
  return;
}
	

/***********************************************************************/
/*                                                                     */
/*                    timer                                            */
/*                                                                     */
/***********************************************************************/
void timer(int n)
{
  int j;

  /* block signals */
  sigprocmask(SIG_SETMASK, &lsigset, &old_lsigset);

  if (verbose) printf("\n... In timer ... sending status to all gui's.\n");
  remove_dead_guis(lists.gui, lists.n_guis, "gui");
  remove_dead_guis(lists.product_gui, lists.n_product_guis, "product_gui");
  signal(SIGALRM, timer);
  if (lists.n_ps_guis > 0) {
	load_psinfo(prog_stat);
	
	for (j=0; j < lists.n_ps_guis; j++)
	send_stat(prog_stat, strlen(prog_stat), (char *)lists.ps_gui[j]);
	
  }
  if (need_to_send_sigio) {
	kill(getpid(), SIGIO);
	need_to_send_sigio = 0;
  }
  if (verbose) printf("\n... Leaving timer.\n");
  sigprocmask(SIG_SETMASK, &old_lsigset, NULL);
  return;

}
  


/***********************************************************************/
/*                                                                     */
/*                    make_me_a_deamon                                 */
/*                                                                     */
/***********************************************************************/
void make_me_a_deamon(void)
{
  int fd;
  if (fork() != 0) exit(0); /* Parent exits. */
  setpgrp(); /* Dissociate from controlling tty. */
  signal(SIGHUP, SIG_IGN);
  if(fork() != 0) exit(0); /* First child dies. */
/* Second child continues as the deamon. */
  for (fd=0; fd<NOFILE; fd++) close(fd);
  chdir("/");
}

/***********************************************************************/
/*                                                                     */
/*                    main                                             */
/*                                                                     */
/***********************************************************************/
int main(int argc, char **argv)
{  /* Each socket will be interrupt driven. */
  char myhostname[STRLEN];
  
  if (argc > 1)
	if (strcmp(argv[1], "-v") == 0) verbose = 1;
  /* if (verbose == 0)  make_me_a_deamon(); */

  /* INIT starts this program.  We need a bit more than /bin:/usr/bin in
   * our path.
   */
  putenv("PATH=/bin:/usr/bin:/usr/local/trmm/GVBOX/bin");

  /* set signals to block */
  sigemptyset(&lsigset);
  sigemptyset(&zeromask);
  sigaddset(&lsigset, SIGIO); 
  sigaddset(&lsigset, SIGALRM); 
  sigaddset(&lsigset, SIGINT); 
  sigaddset(&lsigset, SIGCHLD); 

  signal(SIGPIPE, SIG_IGN); /* Ignore broken pipe problems when connecting
							 * to gui's
							 */
  initialize_list(lists.neighbor, &lists.n_neighbors, NEIGHBOR_FILE);
/* Initialize the neighbors list to include ourself. */
  gethostname(myhostname, sizeof(myhostname));
  add_member(myhostname, lists.neighbor, &lists.n_neighbors);
  write_new_neighbor_file(lists.neighbor, lists.n_neighbors, NEIGHBOR_FILE);
  initialize_communication();

  lists.n_guis = 0;  /* Force the GUI to report his presense. */
  lists.n_ps_guis = 0;  /* Force the GUI to report his presense. */
  lists.n_product_guis = 0;  /* Force the GUI to report his presense. */

  timer(0); /* Init the timer to wake up periodically. */
  while(1) {
	sigsuspend(&zeromask);

	if (need_to_send_sigio) {
	  alarm(1);
	} else {
	  if (lists.n_ps_guis == 0) alarm(100000);
	  else alarm(10);
	}
	if (verbose) printf("----MAIN begin----\n");
	if (verbose) printf("n_guis         %d\n", lists.n_guis);
	if (verbose) printf("n_neighbors    %d\n", lists.n_neighbors);
	if (verbose) printf("n_ps_guis      %d\n", lists.n_ps_guis);
	if (verbose) printf("n_product_guis %d\n", lists.n_product_guis);
	if (verbose) print_list("GUI         ",lists.gui,      lists.n_guis);
	if (verbose) printf("\n");
	if (verbose) print_list("NEIGHBOR    ",lists.neighbor, lists.n_neighbors);
	if (verbose) printf("\n");
	if (verbose) print_list("PS_GUIS     ",lists.ps_gui, lists.n_ps_guis);
	if (verbose) printf("\n");
	if (verbose) print_list("PRODUCT_GUIS",lists.product_gui, lists.n_product_guis);
	if (verbose) printf("\n");
	if (verbose) printf("----MAIN end  ----\n");

  }
  return 0;
}
