#include <stdio.h>
#include "dbcache.h"
//#include "json-c/json.h"
#include <jansson.h>

const char *to_json_string(json_t *obj, int flags);

int json_to_user(char *Json, pUser pusr);
int user_to_json(pUser, const char *);
