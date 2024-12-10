#include "comm.h"
#include <errno.h>
/*
 * This is a test deamon that does the following:
 *   1. Sleep until I/O interrupt on the GUI port.
 *   2. Echo the message.
 */

#include "ports.h"

int neighbor_socket, gui_socket;

#include <sys/wait.h>
/***********************************************************************/
/*                                                                     */
/*                    fireman                                          */
/*                                                                     */
/***********************************************************************/
void fireman(void)
{
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
  signal(SIGCHLD, (void(*)(int))fireman);
}

  typedef struct {
	int pid;
	int ppid;
	char user[9];
	char cmd[200];
  } Process;


void pretty_print_list(Process p[], int n, int ppid, int i_start, int depth, char *from_who)
{
  int i, j;

  for (i=i_start; i<n; i++) {
	if (ppid == p[i].pid) continue;
	  
	if (ppid == p[i].ppid) {
	  printf("%s -->", from_who);
	  for (j=0; j<=depth; j++) printf(".");
	  printf("%d %d %s\n", p[i].ppid, p[i].pid, p[i].cmd);
	  pretty_print_list(p, n, p[i].pid, 0, depth+1, from_who);
	}
  }
}

/***********************************************************************/
/*                                                                     */
/*                    print_ps                                         */
/*                                                                     */
/***********************************************************************/
void print_ps(char *buf, char *from_who)
{
  Process ps_list[1000];
  int  n;
  char *token;
  char *bufcpy;
  char *p_bufcpy;

  n = 0;
  p_bufcpy = bufcpy = strdup(buf);
  token = strtok(bufcpy, " ");
  if (strcmp(token, "status:") == 0) {
	printf("%s: %s\n", from_who, buf);
	return;
  }
  token = strtok(NULL, " ");
  strncat(ps_list[n].user, token, 9);
  while (token) {
	
	token = strtok(NULL, " "); if (token == NULL) break;
	ps_list[n].pid = atol(token);

	token = strtok(NULL, " "); if (token == NULL) break;
	ps_list[n].ppid = atol(token);

	token = strtok(NULL, "\n"); if (token == NULL) break;
	strncat(ps_list[n].cmd, token, 200);

	n++; /* Another line? */
	token = strtok(NULL, " "); if (token == NULL) break;
	strncat(ps_list[n].user, token, 9);

  }
  free(p_bufcpy);
  pretty_print_list(ps_list, n, 0, 0, 0, from_who);
/*
  printf("\n====================================\n");
  for (i=0; i<n; i++) {
	printf("PS: %d ... %s  %d  %d  %s\n",
		   i,
		   ps_list[i].user,
		   ps_list[i].pid,
		   ps_list[i].ppid,
		   ps_list[i].cmd);
  }
*/
}

/***********************************************************************/
/*                                                                     */
/*                    do_something                                     */
/*                                                                     */
/***********************************************************************/
void do_something(int s, char *from_who)
{
  char buf[20000];
  int n;
  int i;

  while ((n = read_data(s, buf, 20000)) > 0) {
	printf("do_something read %d bytes.\n", n);
	for (i=0; i<n; i++)	printf("%c", buf[i]);
	printf("\n");
	printf("The above message was from: %s\n", from_who);
	/*  print_ps(buf, from_who); */
  }
}

/***********************************************************************/
/*                                                                     */
/*                    read_socket                                      */
/*                                                                     */
/***********************************************************************/
void read_socket(int s, char *from_who)
{
  int t;
  struct sockaddr_in from;
  char *peername;
  int from_len;
  if ((t= get_connection(s, &from, &from_len)) < 0) { /* get a connection */
	if (errno == EINTR) return;   /* EINTR might happen on accept(), */
	if (errno == EWOULDBLOCK) return;  /* Nothing right now. */
	perror("read_socket, accept");               /* bad */
	exit(1);
  }
  peername = inet_ntoa(from.sin_addr);
  do_something(t, peername);
  close(t);
}
/***********************************************************************/
/*                                                                     */
/*                    io_handler                                       */
/*                                                                     */
/***********************************************************************/
void io_handler(void)
{
  fd_set read_template;
  struct timeval wait;
  int rc;
  long oldmask;

  oldmask = sigblock(sigmask(SIGIO));
  wait.tv_sec = 0;
  wait.tv_usec = 0; /* Don't wait, there *is* something pending. */
  FD_ZERO(&read_template);
  FD_SET(neighbor_socket, &read_template);
  FD_SET(gui_socket, &read_template);
#ifdef __hpux
#define selCAST int *
#else
#define selCAST fd_set *
#endif

  rc = select(FD_SETSIZE, (selCAST) &read_template, (selCAST) 0, (selCAST) 0, &wait);

  /* Who caused this interrupt? */
  if (rc <= 0)
	/* Possible error or timeout. */
	;
  
  if (FD_ISSET(gui_socket, &read_template)) {
	/* Big brother is watching. */
	read_socket(gui_socket, "gui");
  }
  
  signal(SIGIO, (void(*)(int))io_handler);
  sigsetmask(oldmask);
}
int my_sendto(int s, char *cmd, char *host, int port)
{
  struct sockaddr_in serv_addr;
  struct hostent     *hp;
  int n;

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  if ((hp= gethostbyname(host)) == NULL) { /* do we know the host's */
    perror(host);
    return -1;                                /* no */
  }
  memcpy((char *)&serv_addr.sin_addr,hp->h_addr,hp->h_length);
  serv_addr.sin_port = htons(port);
  
  n = strlen(cmd);
  if ((n = sendto(s, cmd, n, 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr))) != n) {
	perror("sendto");
	printf("sendto sent %d bytes.\n", n);
  }
  return n;
}

/***********************************************************************/
/*                                                                     */
/*                    initialize_communication                         */
/*                                                                     */
/***********************************************************************/
void initialize_communication(void)
{
  int s;
  char myhostname[100];
  char cmd[100];
  unsigned short port;
  int signal_mask;

  signal_mask = sigsetmask(sigmask(SIGIO)); /* Block this, don't miss any. */

  port = 0;
  if ((gui_socket= establish_async(port)) < 0) {
	perror("initialize_communication");
    exit(1);
  }
  /* Ok, we're listening for connections on portno(gui_socket). */
  signal(SIGIO, (void(*)(int))io_handler);

  port = MONITORPS_PORT;  /* Tell monitorps what our port # is by connecting
						  * to the well known port that monitorps listens on.
						  */
  gethostname(myhostname, sizeof(myhostname));
  s = call_socket(myhostname, port);
  if (s < 0) {
	perror(myhostname);
	exit(-1);
  }
  s = make_socket_async(s);

  port = portno(gui_socket); /* Use this port for future 'from' server connects. */
  sprintf(cmd, "from_gui: add gui %s.%d\n", myhostname, port);
  fprintf(stderr,"CMD sent to monitorps: %s\n", cmd);
  write_data(s, cmd, strlen(cmd));  /* Now, tell monitorps. */
  close(s);

  sigsetmask(signal_mask);

}


/***********************************************************************/
/*                                                                     */
/*                    main                                             */
/*                                                                     */
/***********************************************************************/
int main()
{  /* Each socket will be interrupt driven. */
 
  initialize_communication();

  while(1) {
	sigpause(0L);
  }
  return 0;
}
