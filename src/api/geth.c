#include "curlipfsclient.h"
#include <pthread.h>
#include "../../include/geth.h"


void ConnectToGeth(unsigned long start, unsigned long stop)
{
    curl_init();
    
	if (stop-start) printf("[");
        
    for (int i=start;i<=stop;i++)
		printf("%s (%c)\n", getBlock(i), (i!=stop)?",":"");
	if (stop-start) printf("]");

	/* - see below
	cout << "\n";
	cout << "getTransaction:\n"        << getTransaction     ("0xb229fbd196624dc38a213b62a32974651d826c7aae4646bea1130b7d9adcf560") << "\n\n";
	cout << "getTransactionReceipt:\n" << getReceipt         ("0xb229fbd196624dc38a213b62a32974651d826c7aae4646bea1130b7d9adcf560") << "\n\n";
	cout << "getTransactionTrace:\n"   << getTransactionTrace("0xb229fbd196624dc38a213b62a32974651d826c7aae4646bea1130b7d9adcf560") << "\n\n";
	*/

	curl_clean();

	return 0;
}

char* CreateWalletAddress()
{

//establish geth connection
}

char* getBlock(int num)
{
    char *rpccmd = msprintf("[%s,true]",QUOTE(to_string(num)));
    char *strnum = msprintf("%d",num);
	return callRPC("eth_getBlockByNumber", rpccmd, strnum);
}

/*
//-------------------------------------------------------------------------
// These functions are not use here, but could easily be used to get
// more information about transcations, etc.
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void padLeft(string& str, const size_t num, const char paddingChar=' ')
{
    if (num > str.size())
        str.insert(0,num-str.size(),paddingChar);
}
string getTransaction(const string& hash)
{
    // assumes the hash starts with '0x'
    string h = hash.substr(2,hash.length());padLeft(h,64,'0');
    return callRPC("eth_getTransactionByHash", "[" + quote(h) +"]");
}
string getReceipt(const string& hash)
{
    string h = hash.substr(2,hash.length());padLeft(h,64,'0');
    return callRPC("eth_getTransactionReceipt", "[" + quote(h) +"]");
}
string getTransactionTrace(const string& hash)
{
    return "traceTransaction not supported unless you start geth with the 'debug' api interface";
//	string h = hash.substr(2,hash.length());padLeft(h,64,'0');
//	return callRPC("debug_traceTransaction", "[" + quote(h) +"]");
}
*/

//-------------------------------------------------------------------------
char* callRPC(const char* method, const char* params, const char* id)
{
	char* thePost = msprintf("{%s:%s,%s:%s,%s:%s,%s:%s}", 
        QUOTE("jsonrpc"), QUOTE("2.0"), QUOTE("method"), 
        QUOTE(method), QUOTE("params"), params, 
        QUOTE("id"), QUOTE(id));

	curl_easy_setopt(curlPtr(), CURLOPT_POSTFIELDS,    thePost);
	curl_easy_setopt(curlPtr(), CURLOPT_POSTFIELDSIZE, strlen(thePost));

	char* result;
	curl_easy_setopt(curlPtr(), CURLOPT_WRITEDATA,     &result);
	curl_easy_setopt(curlPtr(), CURLOPT_WRITEFUNCTION, write_callback);

	CURLcode res = curl_easy_perform(curlPtr());
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		exit(0);
	}
    free(params);
    free(thePost);
    free(id);
	// Remove the json result wrapper so we get a clean block
	//result.erase(0,1);                // skip over first bracket
	//result.erase(0,result.find("{")); // clear to the next bracket
	//result.erase(result.end()-1);     // erase the last bracket
	return result;
}

//-------------------------------------------------------------------------
// 'curl' calls this function as each portion of the request is processed.
// As we accumulate the string, we strip newlines (\n) and return (\r)
//-------------------------------------------------------------------------
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	char* tmpStr;
	int cnt=0;
	for (int i=0;i<nmemb;i++)
	{
		if (ptr[i]!='\n' && ptr[i]!='\r')
		{
			tmpStr += ptr[i];
			cnt++;
		}
	}
	tmpStr[cnt] = '\0';

	// store it away in the caller's string
//	(*(char*)userdata) += tmpStr;

	// we handeled everything, tell curl to send more if there is any
	return size*nmemb;
}

//-------------------------------------------------------------------------

CURL* curlPtrClean()
{
    static CURL *curl = NULL;
	static struct curl_slist *headers = NULL;
	if (!curl)
	{
		curl = curl_easy_init();
		if (!curl)
		{
			fprintf(stderr, "Curl failed to initialize. Quitting...\n");
			exit(0);
		}
		headers = curl_slist_append(headers,       "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL,        "http:/""/localhost:8545"); // 'geth' or 'parity' must be serving RPC here

	} else //if (cleanup)
	{
		if ( headers ) curl_slist_free_all(headers);
		if ( curl    ) curl_easy_cleanup  (curl);
		curl = NULL;
	}

	return curl;
}

CURL *curlPtr()
{
	static CURL *curl = NULL;
	static struct curl_slist *headers = NULL;
	if (!curl)
	{
		curl = curl_easy_init();
		if (!curl)
		{
			fprintf(stderr, "Curl failed to initialize. Quitting...\n");
			exit(0);
		}
		headers = curl_slist_append(headers,       "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL,        "http:/""/localhost:8545"); // 'geth' or 'parity' must be serving RPC here
	}

	return curl;
}

//-------------------------------------------------------------------------
void curl_init(void)
{
	curlPtr();
}

//-------------------------------------------------------------------------
void curl_clean(void)
{
	curlPtrClean();
}