#ifndef RESTAPI_HEADER_FILE
#define RESTAPI_HEADER_FILE

#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#ifdef __linux__ 
    #include <sys/socket.h> /* socket, connect */
    #include <netdb.h> /* struct hostent, gethostbyname */
    #include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#elif _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #pragma comment(lib,"ws2_32.lib") //Winsock Library

#else

#endif

extern void error(const char *msg);

extern int RestAPI(int argc,char *argv[]);

#endif
