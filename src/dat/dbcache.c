#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include "../../include/dbcache.h"
//#include "dbcache.h"
int AddUser(const UserPtr args)
{
	return insertDB(args, USER_TABLE_NAME);
}

int AddClient(const ClientPtr clientArgs)
{
	return insertDB(clientArgs, CLIENT_TABLE_NAME);
}
char *AddToken(pUsr)
{
	return addToken(pUsr, TOKEN_TABLE_NAME);
}
int GetUser(const int id, UserPtr *out)
{
	return getData(id, USER_TABLE_NAME, out);
}
int GetUserByName(const char *name, UserPtr *out)
{
	return getDataByName(name, USER_TABLE_NAME, out);
}
int GetClientByName(const char *name, ClientPtr *out)
{
	return getDataByName(name, CLIENT_TABLE_NAME, out);
}
int GetClient(const int id, ClientPtr *out)
{
	return getData(id, CLIENT_TABLE_NAME, out);
}

void initDB()
{
	sqlite3 *db;
    char *err_msg = 0;
    
	int rc = sqlite3_open(DBNAME, &db);
    
    if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database(" DBNAME "): %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }

	//User Table
    char sql[512] = "CREATE TABLE " USER_TABLE_NAME "(" ID_COL_NAME " INTEGER AUTO_INCREMENT PRIMARY KEY, " NAME_COL_NAME " TEXT, " PWD_COL_NAME " TEXT);";
	if(setjmp(s_jumpBuffer))
	{
		printf("Exception creating table %s\n", USER_TABLE_NAME);

	} else
	{
    	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    	if (rc != SQLITE_OK ) {
    		fprintf(stderr, "Failed to create table " USER_TABLE_NAME "\n");
       		fprintf(stderr, "SQL error: %s\n", err_msg);
        	sqlite3_free(err_msg);
    	} else {
        	fprintf(stdout, "Table " USER_TABLE_NAME " users created successfully\n");
   	 	}
	}

    sprintf(sql, "INSERT INTO " USER_TABLE_NAME "( "NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES ('Tom','1234','tom@mailinator.com') ;"
    	"INSERT INTO " USER_TABLE_NAME "( " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES ('Rebecca','1234','rebecca@mailinator.com');"
    	"INSERT INTO " USER_TABLE_NAME "( " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES ('Jim','1234','jim@mailinator.com');"
    	"INSERT INTO " USER_TABLE_NAME "( " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES ('Roger','1234','roger@mailinator.com');"
    	"INSERT INTO " USER_TABLE_NAME "( " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES ('Robert','1234','robert@mailinator.com');");
        
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
    	fprintf(stderr, "Failed to add users to %s\n", USER_TABLE_NAME);
       	fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Users succesfylly added to the " USER_TABLE_NAME " table.\n");
		}
    
    int last_id = sqlite3_last_insert_rowid(db);
    printf("The last Id of the inserted user row is %d\n", last_id);

	//Issuer Table
	sprintf(sql, "CREATE TABLE " CLIENT_TABLE_NAME " (" ID_COL_NAME " INTEGER AUTO_INCREMENT PRIMARY KEY, " NAME_COL_NAME " TEXT, " PWD_COL_NAME " TEXT, " EMAIL_COL_NAME " TEXT); INSERT INTO " CLIENT_TABLE_NAME " ( " ID_COL_NAME ", " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ") VALUES (1,'MGM','1234','MGM@mailinator.com');");
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
    	fprintf(stderr, "Failed to create table " CLIENT_TABLE_NAME "\n");
       	fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Table " CLIENT_TABLE_NAME " issuers created successfully\n");
    	last_id = sqlite3_last_insert_rowid(db);
    	printf("The last Id of the issuer inserted row is %d\n", last_id);
    }
    
	
	sqlite3_close(db);
    
    return 0;
}


int callback(void *, int, char **, char **);


int isValidLogin(const char *loginId, const char *tableName) {

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DBNAME, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char sqlb[64];;
    char sql[64] = "0";
   
   	sprintf(sql, "SELECT * FROM %s where " ID_COL_NAME " = %s", tableName, loginId);
//    snprintf(sql, sizeof(sqlb), "%s", sqlb);

    printf("isValid LoginID sql: %s", sql);

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    free(sql);

    if (rc != SQLITE_OK ) {

        fprintf(stderr, "Failed to fine user\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);

    return 0;
}

int callback(void *NotUsed, int argc, char **argv,
                    char **azColName) {

    NotUsed = 0;

    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}


int getData(const int Id, const char *tableName, UserPtr *pUsrOut)
{
	sqlite3 *db;
    char *err_msg = 0;
	sqlite3_stmt *res;

    int rc = sqlite3_open(DBNAME, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char sql[256];
		   
	sprintf(sql, "SELECT " ID_COL_NAME ", " NAME_COL_NAME ", " PWD_COL_NAME ", "  EMAIL_COL_NAME " FROM %s WHERE " ID_COL_NAME " = @id", tableName);

	printf("%s\n", sql);

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int idx = sqlite3_bind_parameter_index(res, "@id");
        rc = sqlite3_bind_int(res, idx, Id);

    } else {

        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return rc;
    }

    int step = sqlite3_step(res);

	if (rc == SQLITE_ROW) {

       		//printf("%s: ", sqlite3_column_text(res, 0));
        	//printf("%d\n", sqlite3_column_int(res, 0));

		pUsrOut = malloc(sizeof(UserPtr));

		(*pUsrOut)->id = sqlite3_column_int(res, 0);
		const char *value = (const char*) sqlite3_column_text(res, 1);
		if(value != NULL) {
			(*pUsrOut)->name = malloc(strlen(value)+1);
			strcpy((*pUsrOut)->name, value); 
		}
	       //free value;
           free(value);


		value = (const char*) sqlite3_column_text(res, 2);
		if(value != NULL) {
			(*pUsrOut)->password = malloc(strlen(value)+1);
			strcpy((*pUsrOut)->password, value);
		}
		// free value again
        free(value);
	}
	else
		return rc;
	
	sqlite3_finalize(res);
	sqlite3_close(db);

	return 0;
}

int getDataByName(const char *name,const  char *tableName, UserPtr *pUsrOut)
{
	sqlite3 *db;
    char *err_msg = 0;
	sqlite3_stmt *res;

    int rc = sqlite3_open(DBNAME, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    char sql[256];
    sprintf(sql, "SELECT " ID_COL_NAME ", " NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME " FROM %s WHERE " NAME_COL_NAME " = @name", tableName);

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) != SQLITE_OK) {
    char *err = sqlite3_errmsg(db); 
    
    printf("SQL prepare failure: %s", sqlite3_errmsg(db));
    sqlite3_finalize(res);
    	return;
	}

	if (sqlite3_bind_text(res, 1, name, -1, NULL) != SQLITE_OK) {
    	printf("Bind 1 failure: %s", sqlite3_errmsg(db));
    	sqlite3_finalize(res);
    	return;
	}

	if(sqlite3_step(res) != SQLITE_DONE) 
    {
        //ID
		(*pUsrOut)->id = sqlite3_column_int(res, 0);
        //Name
    	const char *value = (const char*) sqlite3_column_text(res, 1);
		if(value != NULL) {
			(*pUsrOut)->name = (char *)malloc(strlen(value)+1);
			strcpy((*pUsrOut)->name, value); 
		}
		//Password
		value = (const char*) sqlite3_column_text(res, 2);
		if(value != NULL) {
			(*pUsrOut)->password = malloc(strlen(value)+1);
			strcpy((*pUsrOut)->password, value);
		}
        //Email
        value = (const char*) sqlite3_column_text(res, 3);
		if(value != NULL) {
			(*pUsrOut)->email = malloc(strlen(value)+1);
			strcpy((*pUsrOut)->email, value);
		}
	}
		// free value again
	sqlite3_finalize(res);
	sqlite3_close(db);

	return 0;
}


int insertDB(const UserPtr usr, char *tableName)
{
 	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DBNAME, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database(" DBNAME "): %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char sql[128];
	sprintf(sql, "INSERT INTO %s (" NAME_COL_NAME ", " PWD_COL_NAME ", " EMAIL_COL_NAME ")  VALUES (", tableName);
 
    /*//id
    char sId[1024];
    sprintf(sId,"%d", usr->id);
    strcat(sql, sId);
    strcat(sql,",'");*/

    //name
    strcat(sql, "'");
    strcat(sql, usr->name);
    strcat(sql,"','");

    //password
    strcat(sql, usr->password);
    strcat(sql,"','");

    //email
    strcat(sql, usr->email);
    strcat(sql,"');");

    printf("Creating New User (SQL): %s\n", sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
		int retErr = 1;
        fprintf(stderr, "Failed to create %s \n", tableName);
		
		if (err_msg == SQLITE_CONSTRAINT)
			retErr = SQLITE_CONSTRAINT;
        
		fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);

		return retErr;

    } else
        fprintf(stdout, "Table %s Friends created successfully\n", tableName);

    int last_id = sqlite3_last_insert_rowid(db);
    printf("The last Id of the inserted row is %d\n", last_id);

   // sqlite3_close(db);

    return SQLITE_OK;
}

int addToken(const UserPtr pUsr, char * tablename)
{
 	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DBNAME, &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database(" DBNAME "): %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char sql[128];
	sprintf(sql, "INSERT INTO %s (" TOK_COL_NAME ", " NAME_COL_NAME ")  VALUES (", tablename);
 
    //token
//    char sId[1024];
  //  sprintf(sId,"%d", pUsr->id);
    //strcat(sql, sId);
    //strcat(sql,",'");

    //name
    strcat(sql, "'");
    strcat(sql, pUsr->name);
    strcat(sql,"','");

    //password
    strcat(sql, pUsr->password);
    strcat(sql,"','");

    //email
    strcat(sql, pUsr->email);
    strcat(sql,"');");

    printf("Creating New User (SQL): %s\n", sql);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
		int retErr = 1;
        fprintf(stderr, "Failed to create %s \n", tablename);
		
		if (err_msg == SQLITE_CONSTRAINT)
			retErr = SQLITE_CONSTRAINT;
        
		fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);

		return retErr;

    } else
        fprintf(stdout, "Table %s Friends created successfully\n", tablename);

    int last_id = sqlite3_last_insert_rowid(db);
    printf("The last Id of the inserted row is %d\n", last_id);

    sqlite3_close(db);

    return SQLITE_OK;
}

int gertErrCode()
{

}
