/**
 *
 * CryptReserve Secured API OAuth2 Authorization token check
 *
 * Copyright 2019 BlockchainBPI Ed Vergara <blockchainbpi@gmail.com>
 *
 * Version 1
 *
 */
#include <jwt.h>

#define G_OK                       0
#define G_ERROR                    1
#define G_ERROR_INTERNAL           2
#define G_ERROR_INVALID_REQUEST    3
#define G_ERROR_INVALID_TOKEN      4
#define G_ERROR_INSUFFICIENT_SCOPE 5

#define G_METHOD_HEADER 0
#define G_METHOD_BODY   1
#define G_METHOD_URL    2

#define HEADER_PREFIX_BEARER 	"Bearer "
#define HEADER_RESPONSE     	"WWW-Authenticate"
#define HEADER_AUTHORIZATION 	"Authorization"
#define BODY_URL_PARAMETER   	"access_token"
#define BODY_URL_CALLBACK   	"callback_url"
#define CRYPTRESERVE_USER   	"access_token"
#define CRYPTRESERVE_PASSWORD   "callback_url"

#define APP_NAME		"CryptReserve"

struct _cryptreserve_resource_config {
  int       method;
  char *    oauth_scope;
  char *    jwt_decode_key;
  jwt_alg_t jwt_alg;
  char *    realm;
  int accept_access_token;
  int accept_client_token;
};

int callback_check_cryptreserve_access_token (const struct _u_request * request, struct _u_response * response, void * user_data);
json_t * access_token_check_signature(struct _cryptreserve_resource_config * config, const char * token_value);
json_t * access_token_get_payload(const char * token_value);
int access_token_check_validity(struct _cryptreserve_resource_config * config, json_t * j_access_token);
json_t * access_token_check_scope(struct _cryptreserve_resource_config * config, json_t * j_access_token);

