#ifndef RESTSRV_HEADER_FILE
#define RESTSRV_HEADER_FILE

#include <stdio.h>
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

#include <ulfius.h>
#include "dbcache.h"
//#include "cryptreservesecureapi.h"

#define PORT 2884
#define PREFIX "/auth"

#define USER "test"
#define PASSWORD "testpassword"

struct CurlThreadData {
  
};

int i;

char * read_file(const char * filename);
int callback_auth_basic_body (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_auth_basic (const struct _u_request * request, struct _u_response * response, void * user_data);

//Users
int callback_create_user (const struct _u_request * request, struct _u_response * response, void * user_data);
//int callback_create_user_login (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_user_logon(const struct _u_request * request, struct _u_response * response, void * user_data);


//Issuers
int callback_issuer_logon(const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_create_issuer_account(const struct _u_request * request, struct _u_response * response, void * user_data);

//File Upload/Download
// Callback function used to download IPFS files
int callback_static_file (const struct _u_request * request, struct _u_response * response, void * user_data);

// Callback function used to upload file
int callback_upload_file (const struct _u_request * request, struct _u_response * response, void * user_data);

// File upload callback function
int file_upload_callback (const struct _u_request * request, 
                          const char * key, 
                          const char * filename, 
                          const char * content_type, 
                          const char * transfer_encoding, 
                          const char * data, 
                          uint64_t off, 
                          size_t size, 
                          void * user_data);

char * print_map(const struct _u_map * map);

#ifndef U_DISABLE_GNUTLS
/**
 * Callback function on client certificate authentication
 */
int callback_auth_client_cert (const struct _u_request * request, struct _u_response * response, void * user_data);
#endif

int StartRestServer(int argc, char **argv);


#endif
