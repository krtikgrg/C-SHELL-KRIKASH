#include "h_files.h"
void handle_fag(char **ecmd_brk,int pidbg[], char **bgcmds[],int rd_end,int wr_end)
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
		printf("krikash : fg : ERROR: No arguments provided\n");
	else
	{
		if(ecmd_brk[2]!=NULL)
			printf("krikash : fg : ERROR: flooding of arguments\n");
		else
		{
			char *nmbrr=(char *)malloc((strlen(ecmd_brk[1])+4)*sizeof(char));
			if(nmbrr==NULL)
				error_handle("Malloc failed");
			nmbrr=strcpy(nmbrr,ecmd_brk[1]);
			int kk=1,wo,jnum=0;
			for(wo=strlen(nmbrr)-1;wo>=0;wo--)
			{
				jnum=jnum+(((int)(nmbrr[wo]-'0'))*kk);
				kk=kk*10;
			}
			jnum--;
			if(pidbg[jnum]==0)
				printf("krikash : fg : ERROR: No job with given reference number exists\n");
			else
			{
				int wstat,bgadd;
				signal(SIGTTOU,SIG_IGN);
				tcsetpgrp(STDIN_FILENO,pidbg[jnum]);
				signal(SIGTTOU,SIG_DFL);
				signal(SIGINT,SIG_DFL);
				signal(SIGTSTP,SIG_DFL);
				signal(SIGSTOP,SIG_DFL);
				red=kill(pidbg[jnum],SIGCONT);
				if(red==-1)
					error_handle("kill error");
				red=waitpid(pidbg[jnum],&wstat,WUNTRACED);
				if(red==-1)
					error_handle("waitpid failed");
				signal(SIGTTOU,SIG_IGN);
				tcsetpgrp(STDIN_FILENO,getpgid(getpid()));
				signal(SIGTTOU,SIG_DFL);
				signal(SIGINT,SIG_IGN);
				signal(SIGTSTP,SIG_IGN);
				if(WIFSTOPPED(wstat))
					bgadd=1;
				else
					bgadd=0;
				if(bgadd==0)
				{
					pidbg[jnum]=0;
					free(bgcmds[jnum]);
				}
				else
					printf("\n[%d] %d\n",jnum+1,pidbg[jnum]);
			}
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
