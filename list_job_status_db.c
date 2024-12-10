/*
 * Usage: listdb [file] | sort  +2 -3 +3 -4 +5 -6 +7 -8r 
 */

#include <stdio.h>
#include <gdbm.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
int main (int ac, char **av)
{
  char *infile = "/tmp/monitor.db";
  GDBM_FILE gf;
  datum key, content, next_key;
  /* The job status fields.*/
  char *jobnum, *jtime, *tape, *dev, *stat, *file, *prod;

  if (ac == 2) infile = av[1];
  else if (ac > 2) {
	fprintf(stderr, "Usage: %s [gdbm_file]\n", av[0]);
    fprintf(stderr, "Default gdbm_file = /tmp/monitor.db\n");
	exit(-1);
  }

OPEN:
  gf = gdbm_open(infile, 512, GDBM_READER, 0, 0);
  if (gf == NULL) {
	if (gdbm_errno == GDBM_CANT_BE_READER) {
	  sleep(1); /* Someone is writing. Eventually, we'll get it.*/
	  goto OPEN;
	}
	perror(gdbm_strerror(gdbm_errno));
	perror(infile);
	exit(-1);
  }
  for(key = gdbm_firstkey(gf); key.dptr; key = next_key) {
	next_key = gdbm_nextkey(gf, key);
	content = gdbm_fetch(gf, key);
	if (strstr(key.dptr, " ")) {
	  jtime  = strtok(content.dptr, " "); sscanf(jtime, "%s", jtime);
	  dev    = strtok(NULL, " "); sscanf(dev,   "%s", dev);
	  file   = strtok(NULL, " "); sscanf(file,  "%s", file);
	  stat   = strtok(NULL, "");  sscanf(stat,  "%s", stat);
	  
	  tape   = strtok(key.dptr, " "); sscanf(tape,   "%s", tape);
	  jobnum = strtok(NULL, " ");     sscanf(jobnum, "%s", jobnum);
	  prod   = strtok(NULL, "");      sscanf(prod,   "%s", prod);
	  
	  printf("status: %s %s %s %s %s %s %s\n",
			 jobnum, jtime, tape, dev, prod, file, stat);
	}
	free(key.dptr);
	free(content.dptr);
  }

  gdbm_close(gf);
  exit(0);
}
