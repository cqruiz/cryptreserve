#ifndef DBCACHE_H
#define DBCACHE_H
#include <stdio.h>
#include <sqlite3.h>

#define ID_SIZE 64
#define NAME_SIZE 64
#define PASSWORD_SIZE 128
#define DBNAME "cryptreserve.db"
#define ID_COL_NAME "Id"
#define NAME_COL_NAME "Name"
#define PWD_COL_NAME "Password"
#define EMAIL_COL_NAME "Password"
#define TOK_COL_NAME "Token"
#define USER_TABLE_NAME "users"
#define ISSUER_TABLE_NAME "issuers"
#define TOKEN_TABLE_NAME "tokens"

struct User{
	char *name;
	int id;
	char *password;
	char *email;
};
 
typedef struct User* pUser;
typedef struct User User;

int AddUser(pUser args);
int GetUser(int, pUser);
int GetUserByName(char *, pUser);
#endif

