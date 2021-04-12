#include "h_files.h"
void handle_unsetenv(char ** ecmd_brk,int rd_end,int wr_end)
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
	if(ecmd_brk[1]==NULL)
		printf("krikash: unsetenv : ERROR: Please provide an appropriate variable\n");
	else
	{
		if(ecmd_brk[2]==NULL)
		{
			red=unsetenv(ecmd_brk[1]);
			if(red==-1)
				error_handle("Internal Error Occured");
		}
		else
			printf("krikash: unsetenv : ERROR: Please provide appropriate number of arguments\n");
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
