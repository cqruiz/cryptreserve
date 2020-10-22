#include "jwthelper.h"
#include <string.h>
#include <time.h>

void GetIatExp(char* iat, char* exp, int time_size);

char * CreateJWT(char *json, char *key)
{
	//const char *json = "{\"id\":\"FVvGYTr3FhiURCFebsBOpBqTbzHdX/DvImiA2yheXr8=\","
      //  "\"iss\":\"localhost\"}";

    // jwt pointer 
    jwt_t *jwt = NULL;

    // Key 
    //const  char key[] = " My Passphrase " ;

    // Generate jwt object 
    int ret = jwt_new (& jwt);
    //assert (ret == 0 );
    //assert(jwt != NULL);
    printf( " empty jwt object\n%s\n " , jwt_dump_str(jwt, 1 ));

    // Add string content 
    ret = jwt_add_grant(jwt, " iss " , " files.cyphre.com " );
    //assert (ret == 0 );
    printf( " Add string content \n%s\n " , jwt_dump_str(jwt, 1 ));
  	
	char iat_time[sizeof(time_t) * 3 + 2];
  	char exp_time[sizeof(time_t) * 3 + 2];

    // Add integer content 
    ret = jwt_add_grant_int(jwt, " iat " , ( long )time(NULL));
    //assert (ret == 0 );
    printf( " Add integer content \n%s\n " , jwt_dump_str(jwt, 1 ));
	  
	ret = jwt_add_grant(jwt, "exp", exp_time);
  	if (ret) {
    	printf("Error setting expiration: %d\n", ret);
  	}

    printf( " Add expiry time \n%s\n " , jwt_dump_str(jwt, 1 ));
    
	// Add json content
    //The above iat, iss will be added to json
    // The above iss will be covered by the iss in json 
    ret = jwt_add_grants_json(jwt, json);
    //assert (ret == 0 );
    printf( " Add json content\n%s\n " , jwt_dump_str(jwt, 1 ));

    // Set the algorithm and key
    // will automatically generate the header part 
    ret = jwt_set_alg (jwt, JWT_ALG_HS256, (unsigned char * ) key, strlen (key));
    //assert (ret == 0 );
    printf( " Set algorithm and key \n%s\n " , jwt_dump_str(jwt, 1 ));
    
    // Output the final jwt token 
    char * jwt_str = jwt_encode_str (jwt);
    printf( " output the final jwt\n%s\n " , jwt_str);

    // used to store the jwt object after 
    //decoding 
	jwt_t * jwt2 = NULL;

    // decode
    ret = jwt_decode(&jwt2, jwt_str, (const char unsigned *)key, strlen(key));
    //assert (ret == 0 );

    // decoded json content 
    printf ( " json content \n%s\n after \nDecode " , jwt_dump_str(jwt2, 1 ));

    jwt_free(jwt);
    jwt_free(jwt2);

    return jwt_str;
}

void GetIatExp(char* iat, char* exp, int time_size) {
  // TODO: Use time.google.com for iat
  time_t now_seconds = time(NULL);
  snprintf(iat, time_size, "%lu", now_seconds);
  snprintf(exp, time_size, "%lu", now_seconds + 3600);
  //if (TRACE) 
  {
    printf("IAT: %s\n", iat);
    printf("EXP: %s\n", exp);
  }
}

