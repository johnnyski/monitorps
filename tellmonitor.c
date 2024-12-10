#include "comm.h"
#include <string.h>

int main(int argc, char **argv)
{
  char line[100];
  char cmd[500]; /* Prepend gui: to all lines. */
  char *call_host;
  int s;
  int port;

  if (argc != 3) {
	printf("Usage: %s host {5000}  -- 5000 to monitorps.\n", argv[0]);
	exit(1);
  }
  if (strcmp(argv[2], "5000") != 0 &&
	  strcmp(argv[2], "5010") != 0) {
	printf("Usage: %s host {5000 | 5010}  -- 5000 or 5010 to monitorps.\n", argv[0]);
	exit(1);
  }

  call_host = strdup(argv[1]);
  sscanf(argv[2], "%d", &port);
  printf("Tell %s using port %d\n", call_host, port);

  line[0] = '\0';
  while(line[0] != 'q') {
	printf("Enter command: ");
	gets(line);
	sprintf(cmd, "%s", line);
	s = call_socket(call_host, port);
	if (s < 0) {
	  perror(call_host);
	  exit(-1);
	}
	write_data(s, cmd, strlen(cmd));
	close(s);
  }

  return 0;
}
