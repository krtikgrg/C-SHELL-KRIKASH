#include "h_files.h"
void error_handle(char str[])
{
	perror(str);
	exit(1);
}
