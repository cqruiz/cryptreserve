#include <stdio.h>


int main(void)
{
     // browser gets the output
     // rather then <stdout>(command line # on the server.)
	printf("Content-type: text/html\n\n");
     // print your basic html tags and the content of them.
	printf("<html><head><title>Hello World!! </title></head>\n");
	printf("<body><h1>Hello world</h1></body></html>\n");
}
