#include "h_files.h"
void handle_overkill(int pidbg[],int *bgproct,char **bgcmds[],int rd_end,int wr_end)
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
	for(int it=0;it<=*bgproct;it++)
	{
		if(pidbg[it]!=0)
		{
			int status,retval,exst;
			red=kill(pidbg[it],SIGKILL);
			if(red==-1)
				error_handle("kill error");
			retval=waitpid(pidbg[it],&status,0);
			if(retval==-1)
				error_handle("Error in waitpid");
			if(retval==pidbg[it])
				exst=WEXITSTATUS(status);
			pidbg[it]=0;
			free(bgcmds[it]);
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
