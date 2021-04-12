#include "h_files.h"
void check_end_bg(int pidbg[],int *bgproct,char **bgcmds[])
{
	int i,retv,stat;
	char *str=(char *)malloc(100*sizeof(char));
	if(str==NULL)
		error_handle("Malloc failed..");
	for(i=0;i<=(*bgproct);i++)
	{
		if(pidbg[i]!=0)
		{
			retv=waitpid(pidbg[i],&stat,WNOHANG);
			if(retv==pidbg[i] || retv==-1)
			{
				int exst=WEXITSTATUS(stat);
				if(retv==-1 || WIFEXITED(stat))
					str=strcpy(str,"normally");
				else
					str=strcpy(str,"abnormally");
				printf("%s with pid %d exited %s\n",bgcmds[i][0],pidbg[i],str);
				pidbg[i]=0;
				free(bgcmds[i]);
			}
		}
	}
	free(str);
	return ;
}
