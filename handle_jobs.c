#include "h_files.h"
void handle_jobs(char **bgcmds[],int pidbg[],int *bgproct,int rd_end,int wr_end)
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
	int i,fd;
	char *path=(char *)malloc(1000*sizeof(char));
	if(path==NULL)
		error_handle("Malloc failed");
	for(i=0;i<=*bgproct;i++)
	{
		if(pidbg[i]!=0)
		{
			char *path=(char *)malloc(1000*sizeof(char));
			if(path==NULL)
				error_handle("Malloc failed");
			sprintf(path,"/proc/%d/stat",pidbg[i]);
			fd=open(path,O_RDONLY);
			if(fd==-1)
				error_handle("open failed");
			char data[2048],status;
			read(fd,data,2048);
			int num=0,val=1000;
			char **brk_up;
			brk_up=(char **)malloc(1000*sizeof(char *));
			if(brk_up==NULL)
				error_handle("Malloc failed");
			brk_up[num]=strtok(data,"()");
			while(brk_up[num++]!=NULL)
			{
				if(num==val)
				{
					val=((num/1000)+1)*1000;
					brk_up=(char **)realloc(brk_up,val*sizeof(char *));
					if(brk_up==NULL)
						error_handle("Error occured in realloc");
				}
				brk_up[num]=strtok(NULL,"()");
			}
			num--;
			char **fbrk;
			fbrk=(char **)malloc(100*sizeof(char *));
			if(fbrk==NULL)
				error_handle("Malloc failed");
			int ctr=0;
			fbrk[ctr]=strtok(brk_up[num-1]," ");
			while(fbrk[ctr++]!=NULL)
			{
				if(ctr==1)
					status=fbrk[0][0];
				fbrk[ctr]=strtok(NULL," ");
			}
			red=close(fd);
			if(red==-1)
				error_handle("Internal Error Occured");
			if(status!='Z')
			{
			printf("[%d] ",i+1);
			if(status=='R' || status=='S')
				printf("Running ");
			else
				printf("Stopped ");
			int j=0;
			while(bgcmds[i][j]!=NULL)
				printf("%s ",bgcmds[i][j++]);
			printf("[%d]\n",pidbg[i]);
			}
			free(path);
			free(brk_up);
			free(fbrk);
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
