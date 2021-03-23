#ifndef JSONPARSER_HEADER
#define JSONPARSER_HEADER

#include <stdio.h>
#include "dbcache.h"
#include <jansson.h>

const char *to_json_string(void *obj, int flags);
int ParseJsonVal(const char *Key, char **RetVal, const char *jsonSrc);
int json_to_user(char *Json, struct User ** pusr);
int user_to_json(const UserPtr pusr, char *);
int json_to_user();
int user_to_json();
#endif