#include <stdio.h>
#include <jwt.h>

extern void GetIatExp(char* iat, char* exp, int time_size);

extern char * CreateJWT(char *json, char *key);
