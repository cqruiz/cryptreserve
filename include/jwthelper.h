#include <stdio.h>
#include <jwt.h>

void GetIatExp(char* iat, char* exp, int time_size);

char * CreateJWT(char *json, char *key);
