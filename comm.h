#ifndef _comm_h_
#define _comm_h_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#include <netinet/in.h>
#ifdef __linux
#include <asm/byteorder.h>
#endif
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* Prototypes */

unsigned short portno(int s);
int make_socket_async(int s);
int establish(unsigned short portnum);
int establish_async(unsigned short portnum);
int establish_unix(char *socket_file);
int get_connection(int s, struct sockaddr_in *from, int *from_len);
int call_socket(char *hostname, unsigned short portnum);
int call_socket_unix(char *filename);
int read_data(int s,     /* connected socket */
              char *buf, /* pointer to the buffer */
              int n      /* number of characters (bytes) we want */
             );
int write_data(int s,     /* connected socket */
              char *buf, /* pointer to the buffer */
              int n      /* number of characters (bytes) we want */
             );
int listen_async(unsigned short s);

#endif
