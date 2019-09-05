#ifndef SOCKET_HEADER_FILE
#define SOCKET_HEADER_FILE
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT    5555
#define MAXMSG  512

int read_from_client (int filedes);
int ListenSocket();
#endif
