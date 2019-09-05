#include "../../include/jsonparser.h"
#include <string.h>
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
//	struct json_object *parsed_json;
	struct json_object *name;
	struct json_object *id;
	struct json_object *password;
	struct json_object *email;
	struct json_object *jobj;

	//char buf[1024];
	//sprintf(buf, "%s", Json);                                               
    	printf("Input JSON : %s\n", Json);
    	char const *val;
    	jobj = json_tokener_parse(Json);
	printf("Received JSON in String format : %s\n", json_object_to_json_string(jobj));

	//parsed_json = json_tokener_parse(buf);

	json_object_object_get_ex(jobj, "name", &name);
	json_object_object_get_ex(jobj, "id", &id);
	json_object_object_get_ex(jobj, "password", &password);
	json_object_object_get_ex(jobj, "email", &email);

	pusr->name=(char *)malloc((strlen(json_object_get_string(name)) + 1) * sizeof(char));
	strcpy(pusr->name, json_object_get_string(name));

	pusr->id = json_object_get_int(id);

	pusr->password=(char *)malloc((strlen(json_object_get_string(password))+1) * sizeof(char));
	strcpy(pusr->password, json_object_get_string(password));
	
	pusr->email=(char *)malloc((strlen(json_object_get_string(email))+1) * sizeof(char));
	strcpy(pusr->email, json_object_get_string(email));
	
	printf("Name: %s\n", pusr->name);
	printf("Id: %d\n", pusr->id);
	printf("Password: %s\n", pusr->password);
	printf("Email: %s\n", pusr->email);

	return 0;
}

int user_to_json(pUser pUsr, const char *json)
{
 	/*Creating a json object*/
  	json_object * jobj = json_object_new_object();

  	/*Creating a json string*/
  	json_object *jname = json_object_new_string(pUsr->name);

  	/*Creating a json integer*/
  	json_object *jid = json_object_new_int(pUsr->id);

  	/*Creating a json string*/
  	json_object *jpassword = json_object_new_string(pUsr->password);

  	/*Creating a json string*/
  	json_object *jemail = json_object_new_string(pUsr->email);

  	/*Form the json object*/
  	/*Each of these is like a key value pair*/
  	json_object_object_add(jobj,"User Name", jname);
  	json_object_object_add(jobj,"Identifier", jid);
  	json_object_object_add(jobj,"Password", jpassword);
  	json_object_object_add(jobj,"Email", jemail);


  	/*Now printing the json object*/
  	printf ("The json object created: %sn",json_object_to_json_string(jobj));
//	json = (char*)malloc(strlen(json_object_to_json_string(jobj)));
	sprintf(json, json_object_to_json_string(jobj));

	return 0;
}
