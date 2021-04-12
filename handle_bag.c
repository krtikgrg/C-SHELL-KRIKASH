#include "h_files.h"
void handle_bag(char **ecmd_brk,int pidbg[],int *bgproct,int rd_end,int wr_end)
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
		printf("krikash : bg : ERROR: No arguments provided\n");
	else 
	{
		if(ecmd_brk[2]!=NULL)
			printf("krikash : bg : ERROR: Too many arguments provided\n");
		else
		{
			int refno=0,k=1,i;
			char *cpy=(char *)malloc(2000*sizeof(char));
			if(cpy==NULL)
				error_handle("Malloc failed");
			cpy=strcpy(cpy,ecmd_brk[1]);
			for(i=strlen(cpy)-1;i>=0;i--)
			{
				refno=refno+((int)(cpy[i]-'0'))*k;
				k=k*10;
			}
			refno--;
			if(refno>*bgproct || pidbg[refno]==0)
				printf("krikash : bg : ERROR: No job with the given reference number exits\n");
			else
			{
				red=kill(pidbg[refno],SIGCONT);
				if(red==-1)
					error_handle("Kill error");
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
