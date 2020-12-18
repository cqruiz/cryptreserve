#include <string.h>
#include <stdio.h>
#include <jansson.h>
#include "../../include/jsonparser.h"
#include <stdlib.h>

/*
const char *to_json_string(void *obj, int flags)
{
	int length;
	char *copy;
	const char *result;

	result = json_object_to_json_string_length(obj, flags, &length);
	copy = strdup(result);
	if (copy == NULL)
		printf("to_json_string: Allocation failed!\n");
	else {
		//result = json_object_to_json_string_ext(obj, flags);
		if (length != strlen(result))
			printf("to_json_string: Length mismatch!\n");
		if (strcmp(copy, result) != 0)
			printf("to_json_string: Comparison Failed!\n");
		free(copy);
	}
	return result;
}
*/
int json_to_user(char *Json, struct User **pusr)
{
	printf("json_to_user: %s\n", Json);

	json_t *json;

	json_error_t error;
	json = json_loads(Json, 0, &error);

	if(json==NULL)	
	{
		printf("Loading json string failed: %s - %s; pos=%u\n",
	    	error.text, error.source, error.position);
	  	return 1;
	}
	//json_incref(json);

	//Name
	const char *Key = "Name\0";
	LogMsg("Parse Json Name");
	//ParseJsonVal(Key, &(*pusr)->name, Json);
	json_t *jsoname, *jsopassword, *jsoemail;
	jsoname = json_object_get(json, Key);
	LogMsg("Name json_object_get");

	char *strnmeval = json_string_value(jsoname);
	(*pusr)->name = calloc(strlen(strnmeval)+1, sizeof(char));
	sprintf((*pusr)->name,"%s", strnmeval);

	/*
	if (!jsoname || strlen((*pusr)->name) == 0 )
	{
		printf("Parse Json Name Error  !\n");
		json_decref(json);
		return 1;
	}*/
	printf("Parsed Json Name %s\n", (*pusr)->name);
	
		
	//Password
	LogMsg("Parse Json Password");
	const char* KeyPwd = "Password\0";
	//ParseJsonVal(KeyPwd, &(*pusr)->password, Json);
	jsopassword = json_object_get(json, KeyPwd);
	LogMsg("Password json_object_get");

	char *strpwd = json_string_value(jsopassword);
	(*pusr)->password = calloc(strlen(strpwd)+1, sizeof(char));
	sprintf((*pusr)->password, "%s", strpwd);
	printf("PWD json_string_value = %s\n", (*pusr)->password);

	/*if (!jsopassword || strlen((*pusr)->password) == 0 )
	{
	  printf("Parse Json Password Error  !\n");
	  json_decref(json);
	  return 1;
	}
	*/
	printf("Parsed Json Password %s\n", (*pusr)->password);

	// Email
	LogMsg("Parse Json Email");
	const char *KeyEmail="Email\0";
	//ParseJsonVal(KeyEmail, &(*pusr)->email, Json);
	jsoemail = json_object_get(json, KeyEmail);
	//LogMsg("Email json_object_get");
	char *stremail = json_string_value(jsoemail);
	(*pusr)->email = calloc(strlen(stremail)+1, sizeof(char));
	sprintf((*pusr)->email, "%s", stremail);
	//(*pusr)->email = json_string_value(jsoemail);
	printf("Email json_string_value = %s\n", (*pusr)->email);

	json_decref(json);
	return 0;

	
/*
	if (!jsoemail || strlen((*pusr)->email) == 0 )
	{
	  printf("Parse Json Email Error  !\n");
	  json_decref(json);
	  return 1;
	}
*/
	printf("Parsed Json Email %s\n", (*pusr)->email);

	json_decref(json);
	return 0;
}

	// ID - Wallet Address 
	// prove you own it or 
	// we will create one for you (not ideal/safe)
	/*json_t *id = 0;
	id = json_object_get(json, "Id");
	if (!id  || !json_is_integer(id))
	{
		//status = ERROR;
		return 4;
	}
	// id now equal to json_t object of type int.
	pusr->id = json_integer_value(id);
*/
	//decrement loads
//	json_decref(json);

/*	if (pusr->name!=0)
		printf("Name: %s\n", pusr->name);
	//printf("Id: %d\n", pusr->id);
	if (pusr->password!=0)
		printf("Password: %s\n", pusr->password);
	if (pusr->email!=0) 
		printf("Email: %s\n", pusr->email);
*/


	//return 0;
//}
	
int ParseJsonVal(const char *Key, char **RetVal, const char *jsonSrc)
{
	LogMsg("ParseJsonVal!");
	printf("Key:%s JsonSrc:%s RetVal:%ul\n", Key, jsonSrc, *RetVal);

	json_t *json;
	json_error_t error;
	json = json_loads(jsonSrc, 0, &error);

	if (json == NULL)
	{
		printf("Loading json string failed: %s - %s; pos=%u\n",
	    		error.text, error.source, error.position);
	  return 1;
	}
	
	json_t *jsonobj;
	jsonobj = json_object_get(json, Key);
	LogMsg("json_object_get");

	char *strval = json_string_value(jsonobj);
	printf("json_string_value = %s\n", strval);

	if (!jsonobj || strlen(strval) == 0 )
	{
	  printf("Parse Json Name Error  !\n");
	  return 1;
	}
	
	strcpy(*RetVal, strval);
	
	printf("RetVal = %s\n", RetVal);
	//json_decref(json);
	json_decref(jsonobj);

	return 0;
}

int user_to_json(const UserPtr pUsr, char *json)
{
	char *s = NULL;

	json_t *root = json_object();

	if (pUsr->name)
		json_object_set_new( root, "name", json_string( pUsr->name ));
	if (pUsr->password)
		json_object_set_new( root, "password", json_string( pUsr->password ));
  	if (pUsr->id)
	  	json_object_set_new( root, "id", json_integer( pUsr->id ));
	if (pUsr->email)
		json_object_set_new( root, "email", json_string( pUsr->email ));
		
  	s = json_dumps(root, NULL);
	printf("user_to_json = %s",s);
  	puts(s);
	json = malloc ( strlen(s)  +1);
	sprintf(json, "%s", s);
	free(s);
	printf("user_to_json json = %s", json);
	return 0;
}

static void *secure_malloc(size_t size)
{
    /* Store the memory area size in the beginning of the block */
    void *ptr = malloc(size + 8);
    *((size_t *)ptr) = size;
    return ptr + 8;
}

static void secure_free(void *ptr)
{
    size_t size;

    ptr -= 8;
    size = *((size_t *)ptr);

//    guaranteed_memset(ptr, 0, size + 8);
    free(ptr);
}

