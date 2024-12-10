#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <gdbm.h>


void usage(char **av)
{
  fprintf(stderr, "Usage: %s [-f gdb_file] [msg]\n", av[0]);
  fprintf(stderr, "\nWhere: msg = status: 241 time_in_sec N00469 /dev/8mm1 1C-51 N00469.240 successful\n");
  fprintf(stderr, "If msg is not present, then all lines are from stdin.\n");
  exit(-1);
}

void process_args(int ac, char **av, char **file, int *qstdin)
{
  int c;
  
  while ((c = getopt(ac, av, "f:")) != -1)
	switch (c) {
	case 'f': *file = strdup(optarg);  break;
	case '?': usage(av); break;
	default:  break;
	}

/* Could be reading from stdin. */
  if (ac - optind == 0) *qstdin = 1;
  else {
/* must have 8 fields listed */
	if (ac - optind != 8) {
	  fprintf(stderr, "ac = %d,  optind=%d\n", ac, optind);
	  usage(av);
	}
	*qstdin = 0;
  }
  return;
}

int get_fields(int ac, char **av, int qstdin, char *fields[])
{
  char line[500];
  int i;
  static int been_here = 0;
  if (qstdin) {
	if (gets(line) == NULL) return 0;
	fields[0] = strtok(line, " "); sscanf(fields[0], "%s", fields[0]);
	fields[1] = strtok(NULL, " "); sscanf(fields[1], "%s", fields[1]);
	fields[2] = strtok(NULL, " "); sscanf(fields[2], "%s", fields[2]);
	fields[3] = strtok(NULL, " "); sscanf(fields[3], "%s", fields[3]);
	fields[4] = strtok(NULL, " "); sscanf(fields[4], "%s", fields[4]);
	fields[5] = strtok(NULL, " "); sscanf(fields[5], "%s", fields[5]);
	fields[6] = strtok(NULL, " "); sscanf(fields[6], "%s", fields[6]);
	fields[7] = strtok(NULL, ""); sscanf(fields[7], "%s\n", fields[7]);
  } else {
	if (been_here) return 0;
	for(i=0; i<8; i++)
	  fields[i] = av[optind+i];
	been_here = 1;
  }
  return 1;
}

char *fetch_time(GDBM_FILE gf, datum key)
{
  datum content;
  char *time_str = (char *)calloc(20, sizeof(char));
  char *time;

  content = gdbm_fetch(gf, key);
  time = strtok(content.dptr, " ");
  sprintf(time_str, "%s", time);
  return time_str;
}
  

int main(int ac, char **av)
{
  GDBM_FILE gf;
  char *dbm_file = "/tmp/monitor.db";
  char *field[8]; /* Nine fields. */
  int qstdin;
  datum key, content;
  char *ckey = (char *)calloc(50, sizeof(char));
  char *cdata = (char *)calloc(500, sizeof(char));
  char *time_str = (char *)calloc(20, sizeof(char));
  char *time_str_in_db; /* gdbm_fetch allocates memory. */

  /*
   * Usage: $0 -f gdbm_file job_status_params
   *
   * Ex. CMD -f /tmp/monitor.db status: 241 22043292 N00469 /dev/8mm1 1C-51 N00469.240 successful
   */
  process_args(ac, av, &dbm_file, &qstdin);

  gf = gdbm_open (dbm_file, 512, GDBM_WRCREAT, 0664, 0);
  if (gf == NULL) {
	perror(gdbm_strerror(gdbm_errno));
	exit(-1);
  }

  /*
   * The key is: 'TAPE JOB# PRODUCT'.  Ex.  N00469 241 1C-51
   */
  
  while (get_fields(ac, av, qstdin, field)) {
	sprintf(ckey, "%s %s %s", field[3], field[1], field[5]);
	key.dptr = ckey;
	key.dsize = strlen(ckey)+1;
	
	sprintf(cdata, "%s %s %s %s",
			field[2], /* time */
			field[4], field[6], field[7]  /* dev, file, status */
			);
	content.dptr = cdata;
	content.dsize = strlen(cdata)+1;
	
	/* Determine if existing db entry has a younger time stamp.
     * if such is the case we ignore this attempt to replace it.
     */
	time_str_in_db = fetch_time(gf, key);
	if (time_str_in_db != NULL) {
	  sprintf(time_str, "%s", field[2]);
	  if (strcmp(time_str, time_str_in_db) <= 0) {
		fprintf(stderr, "time in db is <%s>\n", time_str_in_db);
		fprintf(stderr, "time to insrt <%s>\n", time_str);
		continue;
	  }
	}
	if (gdbm_store(gf, key, content, GDBM_REPLACE)!=0) {
	  perror(gdbm_strerror(gdbm_errno));
	}
  }

  gdbm_close(gf);
  exit(0);
}
