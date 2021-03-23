#include <curl/curl.h>
#include <stdio.h>
#include <string.h>


#define PATH "/api/v1"
#define PORT 5000
#define URL "localhost"
#define PROTOCOL "https"

#define MAX(a,b) ((a>b)?a:b)
#define QUOTE(a) ("\"" #a "\"")

void   curl_init          (void);
void   curl_clean         (void);
char*  getBlock           (int blockNum);
char*  getTransaction     (const char* hash);
char*  getReceipt         (const char* hash);
char*  getTransactionTrace(const char* hash);
CURL*  curlPtr            (void);
CURL*  curlPtrClean       (void);
size_t write_callback     (char *ptr, size_t size, size_t nmemb, void *userdata );
char*  callRPC            (const char* method, const char* params, const char* id);
