#include "h_files.h"
void handle_setenv(char **ecmd_brk,int rd_end,int wr_end)
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
		printf("krikash : setenv : ERROR: Please provide an appropriate variable\n");
	else
	{
		char *varnam=(char *)malloc((strlen(ecmd_brk[1])+1)*sizeof(char));
		if(varnam==NULL)
			error_handle("Malloc failed");
		varnam=strcpy(varnam,ecmd_brk[1]);
		if(ecmd_brk[2]==NULL)
		{
			red=setenv(varnam,"",1);
			if(red==-1)
				error_handle("Internal Error Occured");
		}
		else
		{
			char *val=(char *)malloc((strlen(ecmd_brk[2])+1)*sizeof(char));
			if(val==NULL)
				error_handle("Malloc failed");
			val=strcpy(val,ecmd_brk[2]);
			if(ecmd_brk[3]==NULL)
			{
				red=setenv(varnam,val,1);
				if(red==-1)
					error_handle("Internal Error Occured");
			}
			else
				printf("krikash : setenv : ERROR: Please provide appropriate number of arguments\n");
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
