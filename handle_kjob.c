#include "h_files.h"
void handle_kjob(char **ecmd_brk,int pidbg[],char **bgcmds[],int rd_end,int wr_end)
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
	int i,k;
	if(ecmd_brk[1]==NULL)
		printf("krikash : kjob : ERROR: Invalid arguments provided\n");
	else
	{
		if(ecmd_brk[2]==NULL)
			printf("krikash : kjob : ERROR: Signal not provided\n");
		else
		{
			if(ecmd_brk[3]!=NULL)
				printf("krikash : kjob : ERROR: Flooding of arguments\n");
			else
			{
				int refno=0,signo=0;
				char *cpy;
				cpy=(char *)malloc(2000*sizeof(char));
				if(cpy==NULL)
					error_handle("Malloc failed");
				k=1;
				cpy=strcpy(cpy,ecmd_brk[1]);
				for(i=strlen(cpy)-1;i>=0;i--)
				{
					refno=refno+((int)(cpy[i]-'0'))*k;
					k=k*10;
				}
				k=1;
				cpy=strcpy(cpy,ecmd_brk[2]);
				for(i=strlen(cpy)-1;i>=0;i--)
				{
					signo=signo+((int)(cpy[i]-'0'))*k;
					k=k*10;
				}
				if(pidbg[refno-1]!=0)
				{
					int a;
					a=kill(pidbg[refno-1],signo);
					if(a==-1)
						error_handle("Error in kill");
					/*else
					{
						int retval,status,exst;
						retval=waitpid(pidbg[refno-1],&status,WNOHANG);
                                                if(retval!=-1 && WIFEXITED(status))
						{
							exst=WEXITSTATUS(status);
							pidbg[refno-1]=0;
							free(bgcmds[refno-1]);
						}
					}*/
				}
				else
					printf("krikash : kjob : ERROR:No job with given reference number exists\n");
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
