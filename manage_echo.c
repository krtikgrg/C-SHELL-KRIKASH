#include "h_files.h"
void manage_echo(char **to_op,int rd_end,int wr_end)
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
	int i=0;
	if(to_op[0]==NULL)
		printf("\n");
	else
	{
		while(to_op[i]!=NULL)
		{
			printf("%s",to_op[i++]);
			if(to_op[i]!=NULL)
				printf(" ");
			else
				printf("\n");
		}
	}
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
