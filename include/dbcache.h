#ifndef DBCACHE_H
#define DBCACHE_H
#include <stdio.h>
#include <sqlite3.h>
#include <setjmp.h>
#include <stdlib.h>
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
#define CLIENT_TABLE_NAME "clients"
#define TOKEN_TABLE_NAME "tokens"

struct User{
	char *name;
	int id;
	char *password;
	char *email;
};
 
typedef struct User *UserPtr;
//extern struct User* UserPtr;


struct Client{
	char *name;
	int id;
	char *password;
	char *email;
};
 
typedef struct Client* ClientPtr;
//extern struct Client* ClientPtr; 

extern int AddUser(const UserPtr args);
extern int GetUser(const int, UserPtr*);
extern int GetUserByName(const char *, UserPtr*);
extern int AddClient(const ClientPtr args);
extern int GetClient(const int, ClientPtr*);
extern int GetClientByName(const char *, ClientPtr*);
extern void initDB();

/*
extern int getData(int, char *, UserPtr);
extern int getDataByName(char *, char *, UserPtr);
extern int insertDB(UserPtr, char *);
extern int addToken(UserPtr, char*);
*/

static jmp_buf s_jumpBuffer;

#endif

