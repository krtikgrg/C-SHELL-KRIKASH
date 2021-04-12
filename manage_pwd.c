#include "h_files.h"
void manage_pwd(char act_curdir[],int rd_end,int wr_end)
{
	int resinfd,resopfd,red;
	resinfd=dup(STDIN_FILENO);
	resopfd=dup(STDOUT_FILENO);
	if(resinfd==-1 || resopfd==-1)
		error_handle("dup system call failed");
	if(rd_end!=STDIN_FILENO)
	{
        	red=dup2(rd_end,STDIN_FILENO);
		if(red==-1)
                	error_handle("dup2 system call failed");
	}
        if(wr_end!=STDOUT_FILENO)
	{
               	red=dup2(wr_end,STDOUT_FILENO);
		if(red==-1)
                	error_handle("dup2 system call failed");
	}
	char *printdir;
	printdir=(char *)malloc((strlen(act_curdir)+4)*sizeof(char));
	if(printdir==NULL)
		error_handle("Malloc failed..");
	printdir=strcpy(printdir,act_curdir);
	if(!(strlen(printdir)==1 && printdir[0]=='/'))
		printdir[strlen(printdir)-1]='\0';
	printf("%s\n",printdir);
	free(printdir);
	if(rd_end!=STDIN_FILENO)
	{
        	red=dup2(resinfd,STDIN_FILENO);
		if(red==-1)
                        error_handle("dup2 system call failed");
	}
        if(wr_end!=STDOUT_FILENO)
	{
                red=dup2(resopfd,STDOUT_FILENO);
		if(red==-1)
                        error_handle("dup2 system call failed");
	}
	red=close(resinfd);
	red=close(resopfd);
	if(red==-1)
		error_handle("Internal Error Occured");
	return;
}
