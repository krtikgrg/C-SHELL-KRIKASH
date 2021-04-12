#include "h_files.h"
int main()
{
	signal(SIGINT,SIG_IGN);
	interface();
	return 0;
}
