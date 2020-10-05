#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *data;
	long m,n;
	printf("%s%c%c\n",
		"Content-Type:text/html;charset=iso-8859-1",13,10);
	printf("<TITLE>Multiplication results</TITLE>\n");
	printf("<body><h1>File Uploaded to IPFS</h1>\n");
	printf("<H3>Upload File</H3></body></html>\n");
	data = getenv("QUERY_STRING");
	if(data == NULL)
	  printf("<P>Error! Error in passing data from form to script.");
	else if(sscanf(data,"m=%ld&n=%ld",&m,&n)!=2)
	  printf("<P>The FILENAME IS  of %ld and %ld is %ld.",m,n,m*n);

     // browser gets the output
     // rather then <stdout>(command line # on the server.)
//	printf("Content-type: text/html\n\n");
     // print your basic html tags and the content of them.
//	printf("<html><head><title>File Upload!! </title></head>\n");
//	printf("<body><h1>File Uploaded to IPFS</h1></body></html>\n");

	 //Curl calls to CryptReserve API or directly to IPFS

}

