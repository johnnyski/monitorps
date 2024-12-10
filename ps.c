#include <stdio.h>
#include <string.h>

void load_psinfo(char *buffer)
{
  FILE *fp_ps;
  char ps_line[256];
  typedef struct {
	char *user;
	int pid;
	int ppid;
	char *cmd;
  } Processes;
  Processes p[1000];
  int nproc;
  int i;
  char *token;

#ifdef __linux
#include <pwd.h>
#include <sys/types.h>
  char *ps_cmd = "ps -ajxw";
  struct passwd *passwd_entry;
  int uid;
#else
  int ntoken;
  char *cmd;
  char *ps_cmd = "ps -ef";
#endif

  if((fp_ps = popen(ps_cmd, "r")) == NULL) {
	perror(ps_cmd);
	exit(-1);
  }

  nproc = -2;
  while (fgets(ps_line, sizeof(ps_line), fp_ps) != NULL) {
	nproc++;
	if (nproc == -1) continue; /* Skip header line. */
#ifdef __linux
	token = strstr(ps_line, "\n");
	*token = '\0';
	p[nproc].cmd = strdup(&ps_line[53]);
	sscanf(&ps_line[39], "%d", &uid);
	/*
	p[nproc].cmd = strdup(&ps_line[54]);
	sscanf(&ps_line[40], "%d", &uid);
	*/
	passwd_entry = getpwuid(uid);
	p[nproc].user = strdup(passwd_entry->pw_name);
	sscanf(ps_line, "%d%d", &p[nproc].ppid, &p[nproc].pid);
#else
	cmd = strstr(&ps_line[44], " ");
	token = strstr(cmd,"\n");
	*token = '\0';
	p[nproc].cmd = strdup(cmd);
	/* Tokenize the line */
	token = strtok(ps_line, " ");
	ntoken = 0;
	p[nproc].user = strdup(token);
	while ((token = strtok(NULL, " ")) != NULL) {
	  ntoken++;
	  if      (ntoken == 1) p[nproc].pid  = atol(token);
	  else if (ntoken == 2) p[nproc].ppid = atol(token);
	  else if (ntoken >= 3) break;
	}
#endif
  }
  nproc++;
  pclose(fp_ps);

  strcpy(buffer, "ps:");
  for (i=0; i<nproc; i++) {
	sprintf(ps_line,"%8s %7d %7d %s\n",
			p[i].user, p[i].pid, p[i].ppid, p[i].cmd);
	strcat(buffer, ps_line);
  }
}
