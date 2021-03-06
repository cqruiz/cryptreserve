/**
 * 
 * CryptReserve Rest API
 * 
 * Public API's
 *  - CreateLogin
 *
 * Copyright 2020 BlockchainBPI Ed Vergara <vergara_ed@yahoo.com>
 * 
 *
 * WebbService API
 * 
 * Server key and certificate
 * openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout server.key -out server.crt
 * 
 * Certificate authority
 * openssl genrsa -out ca.key 4096
 * openssl req -new -x509 -days 365 -key ca.key -out ca.crt
 * 
 * Run auth_server with the following command
 * $ ./auth_server server.key server.crt ca.crt
 * 
 * Client Key and CSR
 * openssl genrsa -out client.key 4096
 * openssl req -new -key client.key -out client.csr
 * openssl x509 -req -days 365 -in client.csr -CA ca.crt -CAkey ca.key -set_serial 01 -out client.crt
 * 
 * Run auth_client with the following command
 * ./auth_client client.crt client.key <password>
 */

#include <stdio.h>
#include <string.h>
#include <ulfius.h>

#include "../../include/restserver.h"
#include "../../include/jwthelper.h"
#include "../../include/jsonparser.h"
#include "../../include/curlipfsclient.h"
#include "../../include/jwthelper.h"
#include "../../include/cryptreservesecureapi.h"
#include "../../include/queue.h"
#include <sys/stat.h>

#define PORT 2884
#define PREFIX "/auth"
#define PSSPRTAPI "/api"
#define DISCOVER "/discovery"
#define ACCESS_CTRL_MAX_AGE 1800
#define FILE_PREFIX "/upload"
#define FILECACHEDIR "/tmp/crcache"

#define USER "test"
#define PASSWORD "testpassword"
#define ERRLOGFILE "/var/log/cr/cr_error.log"
#define LOGFILE "/var/log/cr/cr_log.log"

    //////////////////////////////////////////////////////////////////////////////
   //                                                                          //
  //    comm/resetserver.c                                                    //
 //                                                                          //
//////////////////////////////////////////////////////////////////////////////


const char * get_filename_ext(const char*);

char * read_file(const char * filename) {
	char * buffer = NULL;
	long length;
	FILE * f = fopen (filename, "rb");
	if (f != NULL) {
		fseek (f, 0, SEEK_END);
		length = ftell (f);
		fseek (f, 0, SEEK_SET);
		buffer = o_malloc (length + 1);
		if (buffer != NULL) {
			fread (buffer, 1, length, f);
			buffer[length] = '\0';
		}
		fclose (f);
	}
	return buffer;
}

//User API

/**
 * Callback function for creating a user account (username/password)
 */
int callback_create_user_account (const struct _u_request * request, struct _u_response * response, void * user_data) {
	
	LogMsg("callback_create_user_account.");
	
	printf("RestServerAPI::callback_create_user_account len=%d, strlen=%d\n", request->binary_body_length, strlen(request->binary_body));
	
	// Create a wallet address from a managed ethereum node
	struct Data_t *dt = calloc(1, sizeof(struct Data_t));
	// create a smart contract for managing this users session token and documents.
	char *body;
	//char *loginReqData;
	char *reqData = calloc(request->binary_body_length*sizeof(char)+1, sizeof(char));
	strncpy(reqData, (char *)request->binary_body, request->binary_body_length*sizeof(char));

//	 loginReqData = malloc(strlen(request->binary_body)*sizeof(char)+1);
//	strncpy(loginReqData,(char *)request->binary_body, strlen(request->binary_body)*sizeof(char));

	printf("Raw body: %s\n", (char *)request->binary_body);
	printf("Creating user %s Len:%u\n", reqData, (unsigned int) strlen(reqData));
	y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_create_user_account %s", reqData);

	struct User *pusr = calloc(1, sizeof(struct User));
	struct User *pGetUser = calloc(1, sizeof(UserPtr));

	if ( json_to_user(reqData, &pusr) ==0)
	{
		printf("json_to_user \n\tname: %s\n\tid: %d\n\tpassword: %s\n\temail: %s\n", 
			pusr->name,pusr->id, pusr->password, pusr->email); 
		
		char *response_value = msprintf("CRYPTRESERVE_USER%s-%s", pusr->name, pusr->password);
    	u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
		o_free(response_value);
		LogMsg("GetUser");
		if(GetUser(pusr->id, &pGetUser)==0) 
		{
			sprintf(dt->name, "%s", pusr->name);
			sprintf(dt->addr, "%s", pusr->email);
			dt->number=pGetUser->id;
			dt->cmd = createprofile;

			////////////////////////////////////////////////
			// Create the Profile File to be sent to IPFS //
			////////////////////////////////////////////////
			snprintf(dt->file,strlen(pusr->email)+1, pusr->email);
			sprintf(dt->path, FILECACHEDIR);
			sprintf(dt->pathfile, "%s/%s",  dt->path, dt->file);
			
			printf("Cache File: %s\n", dt->pathfile);
			FILE *fileProfile = fopen(dt->pathfile,"w+");
			fprintf(fileProfile, "%s\n%s\n%s\n", pusr->name, pusr->email, pusr->password);
			fflush(fileProfile);

			struct stat file_info;
			if(fstat(fileno(fileProfile), &file_info) != 0)
				return 1; 
			__off_t fsize = file_info.st_size;  
			printf("Local file size: %d bytes.\n", fsize);
			dt->filesize = fsize;
			fclose(fileProfile);
			
			////////////////////////////////
			//	Create InterPlanetery ID  //
			////////////////////////////////
			LogMsg("Creating Interpalenary Identification...");
			SendIPFSData(&dt);
			LogMsg("----------------------------------------------------------");
			printf("Interplantery Identification Created.\n\t***\tCID: %s\n\n", dt->CID);
			pusr->cid = calloc(strlen(dt->CID)+1,1);
			strncpy(pusr->cid, dt->CID, strlen(dt->CID)+1);

			////////////////////
			// Add User to DB //
			////////////////////
			LogMsg("AddUser To Database");
			AddUser(pusr);

			u_map_put(response->map_header, "Name", pusr->name);

			char key[128];
			char val[256];
			char expires[16] = "10";
			sprintf(key, "CryptReserve_%s", pusr->name);

			pusr->cid = calloc(strlen(dt->CID)+1, 1);
			sprintf(pusr->cid, "%s", dt->CID);

			sprintf(val, "%s", pusr->password);
        	y_log_message(Y_LOG_LEVEL_INFO, "CreateUserLogin - \n\tk=%s \n\tvalue=%s \n\tCID=%s", key, val, pusr->cid);
			
			ulfius_add_cookie_to_response(response, key, val, NULL, 0, NULL, NULL, 0, 0);

			LogMsg("Add Cookie to Response");
		  	if (ulfius_add_cookie_to_response(response, key, val, expires, 0, NULL, NULL, 0, 0) != U_OK) {
           		y_log_message(Y_LOG_LEVEL_ERROR, "CreateUserLogin - Error adding cookie %s/%s to response", key, val);
           	}
			   //o_free(path);
               //o_free(expires);

			body = msprintf(APP_NAME " User succesfully Created for User: %s!,  CID: %s", reqData, pusr->cid);
			printf("Body=%s/n", body);
			LogMsg("body");
			LogMsg("_set_string_body_response"); 
			ulfius_set_string_body_response(response, 200, body);

			LogMsg("Done..."); 

		}
		else 
		{
			LogMsg("msprintf pGetUser Null?");
			body = msprintf( APP_NAME " creation failed, User already exists, try a different id + usr combo %s.", pGetUser->name);
		}
		
	}
	else
	{
		body= msprintf("Error Parsing JSON.");

		printf("json_to_user error\n");
	       printf("user: 	\n\tname: %s\n\tid: %d\n\tpassword: %s\n\temail: %s\nLen:%u\n",pusr->name,pusr->id,pusr->password,pusr->email, (unsigned int) strlen(reqData));
	}
	
	free((char*)pusr->name);
	free((char*)pusr->password);
	free((char*)pusr->email);
	free((char*)pusr->cid);
	free(pusr);
	free((char*)pGetUser->name);
	free((char*)pGetUser->password);
	//free(pGetUser->email);
	free(pGetUser);
	free(dt);
	free((char*)body);
	free((char*)reqData);

  	return U_CALLBACK_COMPLETE;
}
    
/**
 * Callback function for creating a user login (username/password)
 */
	int callback_user_login(const struct _u_request * request, struct _u_response * response, void * user_data) {
		printf("callback_user_login \n");
		y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_login");
		if (request->auth_basic_user != NULL && request->auth_basic_password != NULL)
		{
			UserPtr pUsr = (UserPtr)malloc(sizeof(UserPtr));
			pUsr->email =  malloc(1);
			sprintf(pUsr->email,"%s","");
			if( GetUserByName(request->auth_basic_user, pUsr) == 0)
			{
				int cmpusr = o_strcmp(request->auth_basic_user, pUsr->name);

				int cmppwd = o_strcmp(request->auth_basic_password, pUsr->password);
				if (cmpusr!=0)
					y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logini Incorrect Username/Password.");
				else if (cmppwd!=0)
					y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_login Incorrect Username/Passsword.");
				else
				{
					char *body;
					char usrjson[512];
					//user_to_json(pUsr, usrjson);
					//int x = user_to_json();
					//Add a token to the db associarted to this user
					char *token = CreateJWT( usrjson, pUsr->password);
					char *response_value = msprintf(CRYPTRESERVE_USER ":%s", token);
        				
					if(pUsr==NULL)
				        {
				                AddUser(pUsr);

						//ulfius_add_cookie_to_response(response, 
						//	"CryptReserve", lang, NULL, 0, NULL, NULL, 0, 0);

                				body = msprintf("Passport User succesfully Created for User : %s!", 
								request->binary_body);
				        }
				        else
				                body = msprintf("Passport creation failed, " 
								"try a different id + usr combo %s.", pUsr->name);

				        ulfius_set_string_body_response(response, 200, body);
 
    					u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
					o_free(response_value);
		
					y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon Successfull Logon: %s!", pUsr->name);
				
					return U_CALLBACK_CONTINUE;
				}
			}
		}

		y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon Failed!");
		return U_CALLBACK_UNAUTHORIZED;
	}
        //u_map_get(response->map_cookie, response_value);
			
/*	
		char *body;
		char reqData[128];
		//strcpy(reqData, (char *)request->binary_body);

		//printf("User %s Logging on.\n", reqData);
		y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_create_user %s", reqData);

		pUser pusr = malloc(sizeof(User));
		if ( json_to_user(reqData, pusr) ==0)
		{
			char *response_value = msprintf(CRYPTRESERVE_USER "%s", pusr->password);
        
    		u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
			o_free(response_value);

			pUser pGetUser = (pUser)malloc(sizeof(pUser));
   	
			if(GetUser(pusr->id, pGetUser)==0) 
				return U_CALLBACK_UNAUTHORIZED;
			else
				return U_CALLBACK_CONTINUE;
				
		}
	}
*/

    //Client API
/**
 * Callback function for creating a client account
 */
    int callback_create_client_account(const struct _u_request * request, struct _u_response * response, void * user_data) {
		
		printf("callback_create_client_account \n");
		y_log_message(Y_LOG_LEVEL_DEBUG, "callback_create_issuer_login");

    	char reqData[128];
    	strcpy(reqData, (char *)request->binary_body);

    	printf("Creating issuer %s\n", reqData);
    	y_log_message(Y_LOG_LEVEL_DEBUG, "callback_create_issueruser %s", reqData);

    	UserPtr pusr = malloc(sizeof(UserPtr));
    	//int z= json_to_user(reqData, pusr);

    	char *response_value = msprintf(HEADER_PREFIX_BEARER " <%s>", pusr->password);

    	u_map_put(response->map_header, HEADER_RESPONSE, response_value);
    	o_free(response_value);

    	char *body;
    	UserPtr pGetUser = (UserPtr)malloc(sizeof(UserPtr)); 
		GetUser(pusr->id, pGetUser);
    	if(pGetUser==NULL)
    	{
        	AddUser(pusr);
        	//ulfius_add_cookie_to_response(response, "CryptReserve", lang, NULL, 0, NULL, NULL, 0, 0);

        	body = msprintf("Passport User succesfully Created for User : %s!", reqData);
    	}
    	else
        	{
				body = msprintf("Passport creation failed, try a different id + usr combo %s.", pGetUser->name);
			}

    	ulfius_set_string_body_response(response, 200, body);

    	free(pusr);
    	free(pGetUser);
    	//u_map_put(response->map_header, "newusr", "1234");
    	return U_CALLBACK_COMPLETE;
    }  

	int callback_user_logon(const struct _u_request * request, struct _u_response * response, void * user_data) {
		return callback_client_logon(request, response, user_data);
	}
/**
 * Callback function for a client logon
 */
	int callback_client_logon(const struct _u_request * request, struct _u_response * response, void * user_data) {

		printf("callback_client_logon \n");
		LogMsg("RestServerAPI::callback_client_logon");
		//y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_client_logon");
		if (request->auth_basic_user != NULL && request->auth_basic_password != NULL)
		{
			UserPtr pUsr = (UserPtr)malloc(sizeof(UserPtr));
			if( GetUserByName(request->auth_basic_user, pUsr) == 0)
			{
				//int cmpusr = o_strcmp(request->auth_basic_user, pUsr->name);
				o_strcmp(request->auth_basic_user, pUsr->name);

				o_strcmp(request->auth_basic_password, pUsr->password);
				//int cmppwd = o_strcmp(request->auth_basic_password, pUsr->password);

				char usrjson[512];
				//user_to_json(pUsr, usrjson);
				//int y = user_to_json();
				//Add a token to the db associarted to this user
				char *token = CreateJWT( usrjson, pUsr->password);
				char *response_value = msprintf(CRYPTRESERVE_USER ":%s", token);
        
    			u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
				o_free(response_value);
		
				LogMsg( "RestServerAPI::callback_user_logon Successfull Logon!");
				LogMsg(pUsr->name);
				y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon Successfull Logon: %s!", pUsr->name);
				
				return U_CALLBACK_CONTINUE;
			}
		}
	return 0;
}


/* OPTIONS callback */
int callback_options (const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  
  //void(request);
  
  //void(user_data);
  
  //Access-Control-Allow-Methods
  u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  
  //Access-Control-Allow-Headers
  u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
  
  //Access-Control-Max-Age
  // This value should be configurable, client should send this value as part of user configuration screen api
  char maxAge[5];

  // convert 123 to string [buf]
  //itoa(ACCESS_CTRL_MAX_AGE, maxAge, 10);
  snprintf (maxAge, sizeof(maxAge), "%d", ACCESS_CTRL_MAX_AGE);
  u_map_put(response->map_header, "Access-Control-Max-Age", maxAge);
  
  return U_CALLBACK_COMPLETE;
}


/**
 * upload a file
 */
int callback_upload_file (const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * url_params = print_map(request->map_url), * headers = print_map(request->map_header), * cookies = print_map(request->map_cookie), 
        * post_params = print_map(request->map_post_body);

  char * string_body = msprintf("Upload file\n\n  method is %s\n  url is %s\n\n  parameters from the url are \n%s\n\n  cookies are \n%s\n\n  headers are \n%s\n\n  post parameters are \n%s\n\n",
                                  request->http_verb, request->http_url, url_params, cookies, headers, post_params);
  ulfius_set_string_body_response(response, 200, string_body);
  
  //Send string_body to IPFS and return a CID
  //response.CID == CID = AddToIPFS.Queue();

  o_free(url_params);
  o_free(headers);
  o_free(cookies);
  o_free(post_params);
  o_free(string_body);
  return U_CALLBACK_CONTINUE;
}

/**
 * File upload callback function
 */
int file_upload_callback (const struct _u_request * request, 
                          const char * key, 
                          const char * filename, 
                          const char * content_type, 
                          const char * transfer_encoding, 
                          const char * data, 
                          uint64_t off, 
                          size_t size, 
                          void * cls) {
  y_log_message(Y_LOG_LEVEL_DEBUG, "Got from file '%s' of the key '%s', offset %llu, size %zu, cls is '%s'", filename, key, off, size, cls);

  //incoming
  // 1. get the data and allocate *pData and cache to a file
  // 2. Create and Queue up the New Node
  //struct QueuePtr* pQ=NULL;
  //CurlThreadDataPtr CTDPtr;
  struct Node_t* NodePtr;
  //struct Queue_t* QueuePtr;
	static int iPR=0;
  CurlThreadDataPtr = (CurlThreadData_t*) cls;
    //QueuePtr = (Queue_t*)(CurlThreadDataPtr->queue);

    NodePtr = (Node_t*) malloc(sizeof (NodePtr));
	//NodePtr->data->number = 100 + iPR++;
    //Write to file
    //
    printf("\nProcessRequest-Call Enqueue(pQ,pN)\n");
//    NODE *tmp = pN->
 //   Enqueue(pQ, pN);   


  //Write to the Queue for cross thread processing.
  return U_OK;
}

/**
 * serve a static file to the client as a very simple http server
 */
int callback_static_file (const struct _u_request * request, struct _u_response * response, void * user_data) {
  void * buffer = NULL;
  long length;
//  char  * file_path = msprintf("%s%s", STATIC_FOLDER, request->http_url);
// Add CID IPFS Stream Pointer to buffer
  const char * content_type;
  

    if (buffer) {
      content_type = u_map_get((struct _u_map *)user_data, get_filename_ext(request->http_url));
      response->binary_body = buffer;
      response->binary_body_length = length;
      u_map_put(response->map_header, "Content-Type", content_type);
      response->status = 200;
    } else {
      response->status = 404;
    }
  return U_CALLBACK_CONTINUE;
}
/**
 * decode a u_map into a string
 */
char * print_map(const struct _u_map * map) {
  char * line, * to_return = NULL;
  const char **keys, * value;
  int len, i;
  if (map != NULL) {
    keys = u_map_enum_keys(map);
    for (i=0; keys[i] != NULL; i++) {
      value = u_map_get(map, keys[i]);
      len = snprintf(NULL, 0, "key is %s, length is %zu, value is %.*s", keys[i], u_map_get_length(map, keys[i]), (int)u_map_get_length(map, keys[i]), value);
      line = o_malloc((len+1)*sizeof(char));
      snprintf(line, (len+1), "key is %s, length is %zu, value is %.*s", keys[i], u_map_get_length(map, keys[i]), (int)u_map_get_length(map, keys[i]), value);
      if (to_return != NULL) {
        len = o_strlen(to_return) + o_strlen(line) + 1;
        to_return = o_realloc(to_return, (len+1)*sizeof(char));
        if (o_strlen(to_return) > 0) {
          strcat(to_return, "\n");
        }
      } else {
        to_return = o_malloc((o_strlen(line) + 1)*sizeof(char));
        to_return[0] = 0;
      }
      strcat(to_return, line);
      o_free(line);
    }
    return to_return;
  } else {
    return NULL;
  }
}

#ifndef U_DISABLE_GNUTLS
/**
 * Callback function on client certificate authentication
 */
int callback_auth_client_cert (const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * dn = NULL, * issuer_dn = NULL, * response_message;
  size_t lbuf = 0, libuf = 0;

  if (request->client_cert != NULL) {
    gnutls_x509_crt_get_dn(request->client_cert, NULL, &lbuf);
    gnutls_x509_crt_get_issuer_dn(request->client_cert, NULL, &libuf);
    dn = o_malloc(lbuf + 1);
    issuer_dn = o_malloc(libuf + 1);
    if (dn != NULL && issuer_dn != NULL) {
      gnutls_x509_crt_get_dn(request->client_cert, dn, &lbuf);
      gnutls_x509_crt_get_issuer_dn(request->client_cert, issuer_dn, &libuf);
      dn[lbuf] = '\0';
      issuer_dn[libuf] = '\0';
      y_log_message(Y_LOG_LEVEL_DEBUG, "dn of the client: %s", dn);
      y_log_message(Y_LOG_LEVEL_DEBUG, "dn of the issuer: %s", issuer_dn);
      response_message = msprintf("client dn: '%s', ussued by: '%s'", dn, issuer_dn);
      ulfius_set_string_body_response(response, 200, response_message);
      o_free(response_message);
    }
    o_free(dn);
    o_free(issuer_dn);
  } else {
    ulfius_set_string_body_response(response, 400, "Invalid client certificate");
  }
  return U_CALLBACK_CONTINUE;
}
/*
void createsecretapi(char *cid, struct _u_response * response)
{
	// Initialize the instance
  	struct _u_instance instance;

  	printf("Generate CryptReserve  callback API!\n");

  	y_init_logs("auth_server", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "logs start");

  	if (ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default") != U_OK) {
    		
		printf("Error ulfius_init_instance, abort\n");
	}

	//generate token;
	char Tok[1024];
	sprintf(Tok, "CryptReserve_%s", cid);

	// OAuth2 Glewd Structure
  	struct _crytpreserve_resource_config g_config;
  	g_config.method = G_METHOD_HEADER;
  	g_config.oauth_scope = "scope1";
	strcpy(g_config.jwt_decode_key, Tok);
	g_config.jwt_alg = JWT_ALG_HS512;
	g_config.realm = "example";
	g_config.accept_access_token = 1;
	g_config.accept_client_token = 0;

	char secretresource[1024];
	sprintf(secretresource, "/resource%s", Tok);

	u_map_put(response->map_header, BODY_URL_CALLBACK, secretresource);

	// OAuth2 authentication callback callback_check_crytpreserve_access_token for the endpoint GET "/api/resource/*"
	//ulfius_add_endpoint_by_val(&instance, "GET", PSSPRTAPI, secretresource, 0, &callback_check_cryptreserve_token, (void *)&g_config);
}
*/
#endif

void LogMsg(char *msg){
	y_log_message(Y_LOG_LEVEL_INFO, msg);
	printf("%s\n",msg);
}
void ErrMsg(char * msg){
	y_log_message(Y_LOG_LEVEL_ERROR, "s", msg);
}

int StartRestServer() {
  	// Initialize the instance
  	struct _u_instance instance;
    
	y_init_logs("CryptReserve", Y_LOG_MODE_FILE, Y_LOG_LEVEL_INFO, LOGFILE, "Initializing CryptReserve Log File.");
  	LogMsg("***************************");
  	LogMsg("* Start CryptReserve Node *");
  	LogMsg("***************************");
 
 	int retUlf = ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default");

  	//if (ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default") != U_OK) {
	if (retUlf != U_OK) {
    		printf("Error CryptReserve ulfius_init_instance, abort\n");
			LogMsg("Error CryptReserve ulfius_init_instance, abort\n");
    		return(1);
  	}
  
	//User API
	//ulfius_add_endpoint_by_val(&instance, "PUT", PREFIX, "/createuseraccount", 0, &callback_create_user_account, argv);
	ulfius_add_endpoint_by_val(&instance, "POST", PREFIX, "/createuseraccount", 0, &callback_create_user_account, NULL);
	LogMsg("*Endpoint: Added POST createuseraccount with callback_create_user_account \n");


  	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/userlogin", 0, &callback_user_login, NULL);
	LogMsg("*Endpoint: Added GET userlogin with callback_user_login \n");

	//Client API
	ulfius_add_endpoint_by_val(&instance, "PUT", PREFIX, "/createclientaccount", 0, &callback_create_client_account, NULL);
	LogMsg("*Endpoint: Added GET createclientaccount with callback_create_client_account \n");

  	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/clientlogin", 0, &callback_client_logon, NULL);
	//printf("*Endpoint: Added GET clientlogin with callback_user_login \n");
	LogMsg("*Endpoint: Added GET clientlogin with callback_user_login \n");

  	
	//Create New Endpoint:
	/*
	  OPTIONS
	*/
	ulfius_add_endpoint_by_val(&instance, "OPTIONS", DISCOVER, "*", 0, &callback_options, NULL);

	//File Upload
	// Max post param size is 16 kb, which means an uploaded file is no more than 16 kb
  	instance.max_post_param_size = 16*1024;
  
  	if (ulfius_set_upload_file_callback_function(&instance, &file_upload_callback, "my cls") != U_OK) {
    		y_log_message(Y_LOG_LEVEL_ERROR, "Error ulfius_set_upload_file_callback_function");
  	}
  
  	// MIME types that will define the static files
  	struct _u_map mime_types;
  	u_map_init(&mime_types);
  	u_map_put(&mime_types, ".html", "text/html");
  	u_map_put(&mime_types, ".css", "text/css");
  	u_map_put(&mime_types, ".js", "application/javascript");
  	u_map_put(&mime_types, ".png", "image/png");
  	u_map_put(&mime_types, ".jpeg", "image/jpeg");
  	u_map_put(&mime_types, ".jpg", "image/jpeg");
  	u_map_put(&mime_types, "*", "application/octet-stream");
  
  	// Endpoint list declaration
  	// The first 3 are webservices with a specific url
  	// The last endpoint will be called for every GET call and will serve the static files
  	ulfius_add_endpoint_by_val(&instance, "*", FILE_PREFIX, NULL, 1, &callback_upload_file, NULL);
  	ulfius_add_endpoint_by_val(&instance, "GET", "*", NULL, 1, &callback_static_file, &mime_types);

	//u_map_init();

// #ifndef U_DISABLE_GNUTLS
//   	if (argc > 3) {
//     	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/client_cert", 0, &callback_auth_client_cert, argv);
//   	}
// #endif
  
#ifndef U_DISABLE_GNUTLS
  // Start the framework
  	if (false) {
		char sshfile[32] = "/tmp/crcache/ssh.cert";
		char sshfilepem[32] = "/tmp/crcache/ssh.cert.pem";
		char sshrootpem[32] = "/tmp/crcache/ssh.root.pem";
    	char * server_key = read_file(sshfile), * server_pem = read_file(sshfilepem), * root_ca_pem = read_file(sshrootpem);
    	if (ulfius_start_secure_ca_trust_framework(&instance, server_key, server_pem, root_ca_pem) == U_OK) {
      		printf("Start secure CryptReserve Node on port %u\n", instance.port);
    
      		// Wait for the user to press <enter> on the console to quit the application
      		printf("Press <enter> to quit server\n");
      		getchar();
			  CurlThreadDataPtr->running=false;
    	} else {
      		printf("Error starting secure framework\n");
    	}
    	o_free(server_key);
    	o_free(server_pem);
    	o_free(root_ca_pem);
  	} else if (ulfius_start_framework(&instance) == U_OK) {
#endif
    	printf("Start framework on port %u\n", instance.port);
    
    	// Wait for the user to press <enter> on the console to quit the application
    	printf("Press <enter> to quit server\n");
    	getchar();
		printf("Stopping the CryptReserve Server...\n");
		CurlThreadDataPtr->running=false;
#ifndef U_DISABLE_GNUTLS
  	} else {
    	printf("Error starting framework\n");
  	}
#endif

	u_map_clean(&mime_types);
  	LogMsg("End framework\n");
  	ulfius_stop_framework(&instance);
  	ulfius_clean_instance(&instance);
  	y_close_logs();
  
  	return 0;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

