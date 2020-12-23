#include "../../include/jsonparser.h"
#include <string.h>
#include <stdio.h>
#include <jansson.h>

/*
const char *to_json_string(json_object *obj, int flags)
{
	int length;
	char *copy;
	const char *result;

	result = json_object_to_json_string_length(obj, flags, &length);
	copy = strdup(result);
	if (copy == NULL)
		printf("to_json_string: Allocation failed!\n");
	else {
		result = json_object_to_json_string_ext(obj, flags);
		if (length != strlen(result))
			printf("to_json_string: Length mismatch!\n");
		if (strcmp(copy, result) != 0)
			printf("to_json_string: Comparison Failed!\n");
		free(copy);
	}
	return result;
}
*/

int json_to_user(char *Json, pUser pusr)
{
	json_t *json;
	json_error_t error;

	json = json_loads(Json, 0, NULL);

//	json = json_load_file(*Json, JSON_DISABLE_EOF_CHECK, &error);
	if (json == NULL)
	{
	  //LOG_PRINT(LOGLEVEL_ERROR, "Loading json string failed: %s - %s; pos=%u\n",
	                             //error.text, error.source, error.position);
	  //status = ERROR;
	  //goto _exit;
	  return 1;
	}

	//Name
	json_t *name = NULL;
	name = json_object_get(json, "user");
	if (!name || !json_is_string(name))
	{
	  //status = ERROR;
	  return 1;
	}
	//pusr->name=(char *)malloc((strlen(json_object_get_string(name)) + 1) * sizeof(char));
	pusr->name=(char *)malloc((strlen(json_string_value(name)) + 1) * sizeof(char));
	strcpy(pusr->name, json_string_value(name));

	//Password
        json_t *password = NULL;
        password = json_object_get(json, "password");
        if (!password || !json_is_string(password))
        {
          //status = ERROR;
          return 2;
        }
	//pusr->password=(char *)malloc((strlen(json_object_get_string(password)) + 1) * sizeof(char));
	pusr->password=(char *)malloc((strlen(json_string_value(password)) + 1) * sizeof(char));
        strcpy(pusr->password, json_string_value(password));

	// Email
        json_t *email = NULL;
        email = json_object_get(json, "id");
        if (!email || !json_is_string(email))
        {
          //status = ERROR;
          return 3;
        }
        strcpy(pusr->email, json_string_value(email));

	// ID
        json_t *id = 0;
	id = json_object_get(json, "id");
        if (!id || !json_is_string(id))
        {
          //status = ERROR;
          return 4;
        }
        // id now equal to json_t object of type int.
        //pusr->id = json_int_value(id);

	printf("Name: %s\n", pusr->name);
//	printf("Id: %d\n", pusr->id);
	printf("Password: %s\n", pusr->password);
	if (email!=0) printf("Email: %s\n", pusr->email);

	return 0;
}

int user_to_json(pUser pUsr, const char *json)
{
	char *s = NULL;
	json_t *root = json_object();
  	json_t *json_arr = json_array();
  
	if (pUsr->name)
  		json_object_set_new( root, "name", json_string( pUsr->name ) );
	if (pUsr->password)
  		json_object_set_new( root, "password", json_string( pUsr->password ) );
  	json_object_set_new( root, "id", json_integer( pUsr->id ));
	if (pUsr->email)
  		json_object_set_new( root, "email", json_string( pUsr->email ));

  	s = json_dumps(root, NULL);
  	puts(s);
	json = malloc ( strlen(s)  +1);
	sprintf(json, "%s", s);
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

    guaranteed_memset(ptr, 0, size + 8);
    free(ptr);
}

