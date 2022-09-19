#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>


int cgiMain()
{
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<TITLE>MY TEST</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY><H1>111111111111111</H1>\n");
	fprintf(cgiOut, "</BODY></HTML>\n");

	return 0;
}


