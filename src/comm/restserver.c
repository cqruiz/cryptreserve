/**
 * 
 * CryptReserve Rest API
 * 
 * Public API's
 *  - CreateLogin
 *
 * Copyright 2019 CryptReserve Ed Vergara <vergara_ed@yahoo.com>
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

#include "../../include/restserver.h"
#include "../../include/jwthelper.h"
#include <stdio.h>
#include <string.h>

#define PORT 2884
#define PREFIX "/auth"
#define PSSPRTAPI "/api"
#define DISCOVER "/discovery"
#define ACCESS_CTRL_MAX_AGE 1800

#define USER "test"
#define PASSWORD "testpassword"

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
	y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_create_user_account");

	// Create a wallet address from a managed ethereum node
	// create a smart contract for managing this users session token and documents.
	char *body;
	char reqData[128];
	strcpy(reqData, (char *)request->binary_body);

	printf("Creating user %s Len:%d\n", reqData, strlen(reqData));
	y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_create_user_account %s", reqData);

	pUser pusr = malloc(sizeof(User));
	if ( json_to_user(reqData, pusr) ==0)
	{
		char *response_value = msprintf(CRYPTRESERVE_USER "%s", pusr->password);
        
    	u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
		o_free(response_value);

//	initDB();
//	printf("Json Parsed User struct for Name: %s", usr->name);
		

		pUser pGetUser = (pUser)malloc(sizeof(pUser));
   	
		if(GetUser(pusr->id, pGetUser)==0) 
		{
		//pGetUser==NULL)
		//	createsecretapi(pusr->password, response);
			AddUser(pusr);
	
			char key[128];
			char val[256];
			char expires[16] = "10";
			sprintf(key, "CryptReserve_%s", pusr->name);
			sprintf(val, "%s", pusr->password);
			ulfius_add_cookie_to_response(response, key, val, NULL, 0, NULL, NULL, 0, 0);

		  	if (ulfius_add_cookie_to_response(response, key, val, expires, 0, NULL, NULL, 0, 0) != U_OK) {
           		y_log_message(Y_LOG_LEVEL_ERROR, "CreateUserLogin - Error adding cookie %s/%s to response", key, val);
           	}
                  //o_free(path);
                    //o_free(expires);
			body = msprintf(APP_NAME " User succesfully Created for User : %s!", reqData);
		}
		else 
		{
	
			body = msprintf( APP_NAME " creation failed, User already exists, try a different id + usr combo %s.", pGetUser->name);
		}
	}
	else
	{
		body= msprintf("Error Parsing JSON.");
	}

	ulfius_set_string_body_response(response, 200, body);

  	free(pusr);

  	//u_map_put(response->map_header, "newusr", "1234");
  	return U_CALLBACK_CONTINUE;
}
    
/**
 * Callback function for creating a user login (username/password)
 */
	int callback_user_login(const struct _u_request * request, struct _u_response * response, void * user_data) {

		y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_login");
		if (request->auth_basic_user != NULL && request->auth_basic_password != NULL)
		{
			pUser pUsr = (pUser)malloc(sizeof(User));
			pUsr->email =  malloc(1);
			sprintf(pUsr->email,"%s","");
			if( GetUserByName(request->auth_basic_user, pUsr) == 0)
			{
				int cmpusr = o_strcmp(request->auth_basic_user, pUsr->name);

				int cmppwd = o_strcmp(request->auth_basic_password, pUsr->password);
				char usrjson[512];
				user_to_json(pUsr, usrjson);
				//Add a token to the db associarted to this user
				char *token = CreateJWT( usrjson, pUsr->password);
				char *response_value = msprintf(CRYPTRESERVE_USER ":%s", token);
        
    			u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
				o_free(response_value);
		
				y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon Successfull Logon: %s!", pUsr->name);
				
				return U_CALLBACK_CONTINUE;
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
		
		y_log_message(Y_LOG_LEVEL_DEBUG, "callback_create_issuer_login");

    	char reqData[128];
    	strcpy(reqData, (char *)request->binary_body);

    	printf("Creating issuer %s\n", reqData);
    	y_log_message(Y_LOG_LEVEL_DEBUG, "callback_create_issueruser %s", reqData);

    	pUser pusr = malloc(sizeof(User));
    	json_to_user(reqData, pusr);

    	char *response_value = msprintf(HEADER_PREFIX_BEARER " <%s>", pusr->password);

    	u_map_put(response->map_header, HEADER_RESPONSE, response_value);
    	o_free(response_value);

    	char *body;

    	User *pGetUser = (pUser)malloc(sizeof(pUser)); 
		GetUser(pusr->id, pGetUser);
    	if(pGetUser==NULL)
    	{
        	AddUser(pusr);
        	//ulfius_add_cookie_to_response(response, "CryptReserve", lang, NULL, 0, NULL, NULL, 0, 0);

        	body = msprintf("Passport User succesfully Created for User : %s!", reqData);
    	}
    	else
        	body = msprintf("Passport creation failed, try a different id + usr combo %s.", pGetUser->name);

    	ulfius_set_string_body_response(response, 200, body);

    	free(pusr);
    	free(pGetUser);
    	//u_map_put(response->map_header, "newusr", "1234");
    	return U_CALLBACK_CONTINUE;
    }  

/**
 * Callback function for a client logon
 */
	int callback_client_logon(const struct _u_request * request, struct _u_response * response, void * user_data) {

		y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon");
		if (request->auth_basic_user != NULL && request->auth_basic_password != NULL)
		{
			pUser pUsr = (pUser)malloc(sizeof(User));
			if( GetUserByName(request->auth_basic_user, pUsr) == 0)
			{
				int cmpusr = o_strcmp(request->auth_basic_user, pUsr->name);

				int cmppwd = o_strcmp(request->auth_basic_password, pUsr->password);
				char usrjson[512];
				user_to_json(pUsr, usrjson);
				//Add a token to the db associarted to this user
				char *token = CreateJWT( usrjson, pUsr->password);
				char *response_value = msprintf(CRYPTRESERVE_USER ":%s", token);
        
    			u_map_put(response->map_header, HEADER_RESPONSE, response_value);    
				o_free(response_value);
		
				y_log_message(Y_LOG_LEVEL_DEBUG, "RestServerAPI::callback_user_logon Successfull Logon: %s!", pUsr->name);
				
				return U_CALLBACK_CONTINUE;
			}
		}
	return 0;
}


/* OPTIONS callback */
int callback_options (const struct _u_request * request, struct _u_response * response, void * user_data) 
{
  
  void(request);
  
  void(user_data);
  
  //Access-Control-Allow-Methods
  u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  
  //Access-Control-Allow-Headers
  u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
  
  //Access-Control-Max-Age
  // This value should be configurable, client should send this value as part of user configuration screen api
  u_map_put(response->map_header, "Access-Control-Max-Age", ACCESS_CTRL_MAX_AGE);
  
  return U_CALLBACK_COMPLETE;
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

void createsecretapi(char *cid, struct _u_response * response)
{
	// Initialize the instance
  	struct _u_instance instance;

  	printf("Generate CryptReserve  callback API!\n");

  	y_init_logs("auth_server", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "logs start");

  	if (ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default") != U_OK) {
    		
		printf("Error ulfius_init_instance, abort\n");
		return(1);
	}

	//generate token;
	char Tok[1024];
	sprintf(Tok, "CryptReserve_%s", cid);

	// OAuth2 Glewd Structure
  	struct _gameon_resource_config g_config;
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

	// OAuth2 authentication callback callback_check_gameon_access_token for the endpoint GET "/api/resource/*"
	//ulfius_add_endpoint_by_val(&instance, "GET", PSSPRTAPI, secretresource, 0, &callback_check_cryptreserve_token, (void *)&g_config);
}

#endif

int StartRestServer(int argc, char **argv) {
  	// Initialize the instance
  	struct _u_instance instance;
    
  	printf("Start  Node!\n");
  
  	y_init_logs("auth_server", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "***************************");
  	y_init_logs("auth_server", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "* Start CryptReserve Node *");
  	y_init_logs("auth_server", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "***************************");
 
 	int retUlf = ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default");

  	//if (ulfius_init_instance(&instance, PORT, NULL, "auth_basic_default") != U_OK) {
	if (retUlf != U_OK) {
    		printf("Error CryptReserve ulfius_init_instance, abort\n");
    		return(1);
  	}
  
	//User API
	ulfius_add_endpoint_by_val(&instance, "PUT", PREFIX, "/createuseraccount", 0, &callback_create_user_account, NULL);
  	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/userlogin", 0, &callback_user_login, NULL);

	//Client API
	ulfius_add_endpoint_by_val(&instance, "PUT", PREFIX, "/createclientaccount", 0, &callback_create_client_account, NULL);
  	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/clientlogin", 0, &callback_user_login, NULL);
  	
	//Create New Endpoint:
	/*
	  OPTIONS
	*/
	ulfius_add_endpoint_by_val(&instance, "OPTIONS", DISCOVER, "*", 0, &callback_options, NULL);

	//u_map_init();

#ifndef U_DISABLE_GNUTLS
  	if (argc > 3) {
    	ulfius_add_endpoint_by_val(&instance, "GET", PREFIX, "/client_cert", 0, &callback_auth_client_cert, NULL);
  	}
#endif
  
#ifndef U_DISABLE_GNUTLS
  // Start the framework
  	if (argc > 3) {
    	char * server_key = read_file(argv[1]), * server_pem = read_file(argv[2]), * root_ca_pem = read_file(argv[3]);
    	if (ulfius_start_secure_ca_trust_framework(&instance, server_key, server_pem, root_ca_pem) == U_OK) {
      		printf("Start secure CryptReserve Node on port %u\n", instance.port);
    
      		// Wait for the user to press <enter> on the console to quit the application
      		printf("Press <enter> to quit server\n");
      		getchar();
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
#ifndef U_DISABLE_GNUTLS
  	} else {
    	printf("Error starting framework\n");
  	}
#endif

  	printf("End framework\n");
  	ulfius_stop_framework(&instance);
  	ulfius_clean_instance(&instance);
  	y_close_logs();
  
  	return 0;
}
