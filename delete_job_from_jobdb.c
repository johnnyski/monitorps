#include <stdio.h>
#include <gdbm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>
#include <math.h>
#include <errno.h>

#ifdef __linux
#define LOCK_IT(fp) flock(fileno(fp), LOCK_EX)
#define UNLOCK_IT(fp) flock(fileno(fp), LOCK_UN)
#else
#define LOCK_IT(fp) lockf(fileno(fp), F_LOCK, 0)
#define UNLOCK_IT(fp) lockf(fileno(fp), F_ULOCK, 0)
#endif

void usage(char *cmd)
{
  fprintf(stderr, "Usage: cmd [-f gdbm_file] host time1 time2 tapeid[*] device[*] product[*]\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "   The '*' wild card may be present at the end of the\n");
  fprintf(stderr, "   'tapeid', 'device', or 'product' specifications.  If\n");
  fprintf(stderr, "   present, perform a wild card match in the database.\n");
  exit(-1);
}


void process_args(int argc, char **argv,
			 char **dbfile, char **host,
			 int *stime,
			 int *etime,
			 char **tape, char **device,
			 char **prod)
{
  int c;
  
  while ((c = getopt(argc, argv, "f:")) != -1)
        switch (c) {
        case 'f': *dbfile = strdup(optarg);  break;
        case '?': usage(argv[0]); break;
        default:  break;
        }

/* must have 8 files listed */
  if (argc - optind != 6) usage(argv[0]);

  *host = strdup(argv[optind]);
  *stime = strtol(argv[optind+1], NULL, 0);
  if (*stime == 0) usage(argv[0]);
  *etime = strtol(argv[optind+2], NULL, 0);
  if (*etime == 0) usage(argv[0]);
  *tape  = strdup(argv[optind+3]);
  *device = strdup(argv[optind+4]);
  *prod  = strdup(argv[optind+5]);
}

#define STRLEN 512

int main(int ac, char **av)
{
/*
 * Usage: cmd [-f file.pag] trmm time1 time2 J0002 /dev/8mm2 cdrom
 */

  char *dbfile;
  char myhostname[STRLEN];
  char *host, *tape, *device, *prod;
  int stime, etime;
  char *jobnum, *tape1,*dev1,   *prod1, *files, *stats;
  char *jtime;
  time_t start_t, end_t, job_t;

  char *tape_star, *dev_star, *prod_star;
  int n_cmp_tape, n_cmp_dev, n_cmp_prod;

  GDBM_FILE gf;
  datum key, content, next_key;
  int verbose;
  char *key_str;
  char *content_str;

  int eagain_count;

  typedef struct _key_list{
	datum key;
	struct _key_list *next;
  } Key_list;

  Key_list *head, *tail;
  FILE *fp_lock;
  
  dbfile = "/tmp/monitor.db";

  process_args(ac, av, &dbfile,
			   &host, &stime, &etime, &tape, &device, &prod);

  verbose = 0;

  if (verbose) printf("ARGS: %s %s %d %d %s %s %s\n",  dbfile,
			   host, stime, etime, tape, device, prod);

  /* Typical content for entry is:
   * trmm time1 time2 J0002 /dev/8mm2 cdrom
   */
  /* The time entries are seconds from Jan 1, 1970. */

  /* This algorithm is simple.  Delete all entries that match
   * the 'computer', 'tapeid', 'device', 'product' for all times
   * between Stime and Etime.
   */

  /* Are we the computer? */
  gethostname(myhostname, sizeof(myhostname));
  if (strcmp(host, myhostname) != 0) exit(0); /* Nope. */
  /* Open the 'file' and compare each line. */
  /* Write the new file and rename it at the end. */

  eagain_count = 0;
OPEN:
  gf = gdbm_open(dbfile, 512, GDBM_READER, 0664, 0);
  if(gf==NULL) {
	/*	fprintf(stderr,"ERROR for read: %s   eagain_count = %d\n", dbfile, eagain_count); */
	if (errno == EAGAIN && eagain_count++ < 1000000) goto OPEN;
	perror(dbfile);
	exit(0);
  }

  /* Keep a linked list of keys to delete.  Then, traverse and delete.
   * Deleting gdbm on the fly reorganizes the hash table, and, I want
   * to do the deletes as fast as possible.
   *
   * Pass1: get the keys to delete.
   * Pass2: Open WRITER and delete.
   */
  head = NULL;
  tail = NULL;

  for(key = gdbm_firstkey(gf); key.dptr; key=next_key) {
	next_key = gdbm_nextkey(gf, key);
	key_str = strdup(key.dptr);
	content = gdbm_fetch(gf, key);
	/* BTW, I expect that the key.dptr and content.dptr are NULL terminated.
     * That's how they were stored by 'update_job...'
	 */

	/* Tokenize the line */
	content_str = strdup(content.dptr);
	jtime = strtok(content_str, " "); sscanf(jtime, "%s", jtime);
	dev1  = strtok(NULL, " "); sscanf(dev1,  "%s", dev1);
	files = strtok(NULL, " "); sscanf(files, "%s", files);
	stats = strtok(NULL, "");  sscanf(stats, "%s", stats);

	tape1  = strtok(key_str, " "); sscanf(tape1,  "%s", tape1);
	jobnum = strtok(NULL, " ");    sscanf(jobnum, "%s", jobnum);
	prod1  = strtok(NULL, "");     sscanf(prod1,  "%s", prod1);
/*
	if (verbose) printf("DELETE: <%s> <%s> <%s> <%s> <%s> <%s>\n",
		   host, stime, etime, tape, device, prod);
*/

	if (verbose) {
	  printf("COMPARE: <%s> to <%s>, ", tape1, tape);
	  printf("COMPARE: <%s> to <%s>, ", dev1, device);
	  printf("COMPARE: <%s> to <%s>\n", prod1, prod);
	}
	tape_star = strstr(tape, "*");
	if (tape_star == NULL) n_cmp_tape = strlen(tape1);
	else n_cmp_tape = tape_star - tape;

	dev_star = strstr(device, "*");
	if (dev_star == NULL) n_cmp_dev = strlen(dev1);
	else n_cmp_dev = dev_star - device;

	prod_star = strstr(prod, "*");
	if (prod_star == NULL) n_cmp_prod = strlen(prod1);
	else n_cmp_prod = prod_star - prod;

	if (verbose) {
	  printf("n_cmp_tape = %d,n_cmp_dev = %d,n_cmp_prod = %d\n",
			 n_cmp_tape, n_cmp_dev, n_cmp_prod);
	}
	if (strncmp(tape1, tape, n_cmp_tape) == 0
		&& strncmp(dev1, device, n_cmp_dev) == 0
		&& strncmp(prod1, prod, n_cmp_prod) == 0) {

	  if (verbose) printf("LOGENT: <%s> <%s> <%s> <%s> <%s> <%s> <%s>\n",
						  jobnum,
						  jtime, tape1, dev1, prod1,
						  files, stats);


	  if (verbose) printf("Match ... check time ...");
	  /*
		if (verbose) printf("LOGENT: %s %s %s %s %s %s %s\n",
		jobnum,
		jtime, tape1, dev1, prod1,
		files, stats);
		*/
	  /* Now check the time information.  If the time is between (inclusivly)
       * the start and stop time
       * than what is to be deleted, then delete it.
       *
       * These tests are not sufficient to catch all date/time situations.
       */
	  
	  /* if jdate is earlier than edate, OR
		 the dates match AND
		 stime <= jtime <= etime
		 */
	  
	  start_t = stime;
	  end_t   = etime;
	  job_t   = atoi(jtime);
	  if (start_t <= job_t && job_t <= end_t) {

		if (verbose) printf("MATCH MATCH ... DELETE.\n");
		if (verbose) printf("Start: <%d> End: <%d> Job: <%s>\n",
							stime, etime, jtime);

		if (head == NULL) {
		  head = (Key_list *)calloc(1, sizeof(Key_list));
		  tail = head;
		} else {
		  tail->next = (Key_list *)calloc(1, sizeof(Key_list));
		  tail = tail->next;
		}
		tail->key = key;
	  } else /* Time doesn't match. */ {
		if (verbose) printf("TIME  DOES'T MATCH.\n");
	  }
	  
	} else { /* Not a matching entry. */
	}
	free(content.dptr);
	free(key_str);
	free(content_str);
  }
  gdbm_close(gf);
  
  /* Open a lock file.  When the lock file is free, given up
   * by another delete_job_from_jobdb command, then we can proceed 
   * with the deletes.
   */
#define LOCKFILE "/tmp/delete_job_from_jobdb.lock"
  fp_lock = fopen(LOCKFILE, "w");
  if (fp_lock == NULL) {
	fprintf(stderr, "Couldn't create lock file <%s>\n", LOCKFILE);
	exit(0);
  }
  LOCK_IT(fp_lock);
  /* Pass2 */
  eagain_count = 0;
E_AGAIN:
  gf = gdbm_open(dbfile, 512, GDBM_WRITER, 0664, 0);
  if(gf==NULL) {
	/* Try again, but, at most 1000 times! */
	/*	fprintf(stderr,"ERROR: %s   eagain_count = %d\n", dbfile, eagain_count); */
	if (errno == EAGAIN && eagain_count++ < 1000000) goto E_AGAIN;
	perror(dbfile);
	exit(0);
  }
  for(tail=head; tail; tail=tail->next) {
	if (verbose) printf("DELETE <%s>\n", tail->key.dptr);
	if (gdbm_delete(gf, tail->key) != 0) {
	  perror(gdbm_strerror(gdbm_errno));
	}
  }
  gdbm_reorganize(gf);
  gdbm_close(gf);
  UNLOCK_IT(fp_lock);
  fclose(fp_lock);
  unlink(LOCKFILE);

  exit (0);
}
