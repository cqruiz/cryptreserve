#include <stdio.h>
#include <stdlib.h>
#include "../include/restserver.h"
#include "../include/curlipfsclient.h"
#include <unistd.h> //sleep
#include <pthread.h>
#include "../include/queue.h"

    /////////////////////////////////////////////////////////////////////////////////
   //										  //
  //	cryptreserve.c								 //
 //										//
/////////////////////////////////////////////////////////////////////////////////


void jsontest();

//Curl IPFS Client Interface thread
void CurlIPFSClient(void *args )
{
	printf("Starting Crypt Reserve Curl IPFS Client Interface.");
	//Queue *pQueue;
	CurlThreadData* ctd = (CurlThreadData*)malloc(sizeof(CurlThreadData));
	
	StartCurlServer(ctd);
	printf("Stopped Curl Server.");
}


//Rest API Server thread
void RestAPIServer(void *args )
{
	printf("Starting Rest Server.");
    	char *arrArgs[] = {"localhost","5001","peers","GET","/api/v0/bitswap/stat"};
	//Initialize the database if non exists.
	initDB();
	StartRestServer(1,arrArgs);
	printf("Stopped Rest Server.");
}

int main(int argc, const char* argv[] )
{
    printf( "\nStarting the Crypt Reserve Node \n\n" );

    //Start the Curl IPFS Client and Rest API Server
    pthread_t curlIPFSClient;;
    pthread_t restAPIServer;

//    Queue *pQ =  ConstructQueue(100);

 //   / create a Message Processor thread /
    if(pthread_create(&curlIPFSClient, NULL, (void*)CurlIPFSClient, NULL)) {
	    fprintf(stderr, "Error creating Curl IPFS CLient thread\n");
	    return 1;
    }
    printf("Running Curl IPFS Client Thread\n");

    /* create a Request Listener thread */
    if(pthread_create(&restAPIServer, NULL, (void*)RestAPIServer, NULL)) {
	    fprintf(stderr, "Error creating Request Listener thread\n");
	    return 1;
    }
    printf("Running Request Listener Thread\n");

    pthread_join(restAPIServer, NULL);
    pthread_join(curlIPFSClient, NULL);

    printf("Exiting CryptReserve...");

}

/*
//Json Test
void jsontest()
{
	struct json_object *jobj;
	char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
		\"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
		\"msg-to\": { \"class\": \"supreme-commander\", \"name\": \"[Redacted]\" }, \
		\"msg-log\": [ \
			\"soldier: Boss there is a slight problem with the piece offering to humans\", \
			\"supreme-commander: Explain yourself soldier!\", \
			\"soldier: Well they don't seem to move anymore...\", \
			\"supreme-commander: Oh snap, I came here to see them twerk!\" \
			] \
		}";

	printf("str:\n---\n%s\n---\n\n", str);

	jobj = json_tokener_parse(str);
	printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

}
*/
