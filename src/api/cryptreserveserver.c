#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //sleep
#include <pthread.h>
#include "dbcache.h"
#include "restserver.h"
#include "curlipfsclient.h"
#include "cryptreserveserver.h"

    //////////////////////////////////////////////////////////////////////////////
   //										  								   //
  //	cryptreserveserver.c								 				  //
 //																			 //
//////////////////////////////////////////////////////////////////////////////

struct thread_info {    /* Used as argument to thread_start() */
           pthread_t thread_id;        /* ID returned by pthread_create() */
           int       thread_num;       /* Application-defined thread # */
           char     *argv_string;      /* From command-line argument */
       };

const int num_threads=2;

void jsontest();

//Curl IPFS Client Interface thread
void CurlIPFSClient(void *args )
{
	printf("Starting Crypt Reserve Curl IPFS Client Interface.\n");
    	
	pQueue =  ConstructQueue(100);
	CurlThreadData* ctd = (CurlThreadData*)malloc(sizeof(CurlThreadData));
	ctd->queue = pQueue;
	ctd->queue->head = malloc(sizeof(NODE));
	ctd->queue->tail = malloc(sizeof(NODE));

	printf("StartCurslServer\n");
	StartCurlServer(ctd);
	printf("Stopped Curl Server.\n");
}


//Rest API Server thread
void RestAPIServer(void *pQueue )
{
	printf("Starting Rest Server.\n");
    char *arrArgs[] = {"localhost","5001","peers","GET","/api/v0/bitswap/stat", pQueue};
	//Initialize the database if non exists.
	initDB();
	StartRestServer(1,arrArgs);
	printf("Stopped Rest Server.\n");
}

int main(int argc, const char* argv[] )
{
    printf("Starting the Crypt Reserve Node\n" );

    //Start the Curl IPFS Client and Rest API Server
    pthread_t curlIPFSClient;
    pthread_t restAPIServer;
	pthread_attr_t attr;

	int s = pthread_attr_init(&attr);
    if (s != 0)
    {
		fprintf(stderr, "Error Initializing thread attrib \n");
		return 1;
	}

	struct thread_info *tinfo;

 	tinfo = calloc(num_threads, sizeof(struct thread_info));
	if (tinfo == NULL)
	{
		printf("Error allocating threads.\n");
		fprintf(stderr, "Error allocating thread info\n");
		return 1;
	}

	/* The pthread_create() call stores the thread ID into
		corresponding element of tinfo[] */

	printf("Starting Curl IPFS Client Thread\n");
	s = pthread_create(&tinfo[0].thread_id, &attr,
						(void*)CurlIPFSClient, &tinfo[0]);
	
	if (s != 0)
	{
		fprintf(stderr, "Error creating Curl IPFS CLient thread\n");
	    return 1;
	}
	printf("Running Curl IPFS Client ThreadId: %lu\n", tinfo[0].thread_id);

 	//create a Message Processor thread /
 	/* create a Request Listener thread */
 	printf("Starting Rest API Server Thread\n");
	s = pthread_create(&tinfo[1].thread_id, &attr,
						(void*)RestAPIServer, &tinfo[1]);
	if (s != 0)
	{
		fprintf(stderr, "Error creating Request Listener thread\n");
	    return 1;
	}
	printf("Running Rest API ThreadId: %lu\n", &tinfo[1].thread_id);


/*    if(pthread_create(&curlIPFSClient, NULL, (void*)CurlIPFSClient, NULL)) {
	    fprintf(stderr, "Error creating Curl IPFS CLient thread\n");
	    return 1;
    }
    printf("Running Curl IPFS Client Thread\n");

   
    if(pthread_create(&restAPIServer, NULL, (void*)RestAPIServer, (void*) pQueue )) 
    {
	    fprintf(stderr, "Error creating Request Listener thread\n");
	    return 1;
    }*/
    printf("Running Request Listener Thread\n");

    //pthread_join7(tinfo[0], NULL);
    //pthread_join(&tinfo[1], NULL);
	void *res;

	for (int tnum=0; tnum<=num_threads; tnum++)
	{
		s = pthread_join(tinfo[tnum].thread_id, &res);
		if (s != 0)
		{
			printf("Error joining thread.\n");
			fprintf(stderr, "Error joining thread id:%lu\n", tinfo[tnum].thread_id);
	    	return 1;
		}
		
		printf("Joined with thread \n\t\tthread id:%lu\n", tinfo[tnum].thread_id);

		if(res)
		{
			printf("Freeing res.\n");
			//free(res);      /* Free memory allocated by thread */
			free(tinfo[tnum].argv_string);
		}
	}
	
	if(tinfo)
	{
		printf("Joining thread, freeing tinfo and quiting.\n");
		free(tinfo);
	}

    printf("Exiting CryptReserve...\n");
	//exit(EXIT_SUCCESS);

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
