#include "h_files.h"
void manage_pinfo(char **ecmd_brk,char home_dir[],int rd_end,int wr_end)
{
	int resinfd,resopfd,red;
        resinfd=dup(STDIN_FILENO);
        resopfd=dup(STDOUT_FILENO);
	if(resinfd==-1 || resopfd==-1)
		error_handle("dup system called failed");
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
	if(ecmd_brk[1]!=NULL && ecmd_brk[2]!=NULL)
	{
		printf("kirkash : pinfo : Too many arguments supplied\n");
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
	int pid;
	char *path=(char *)malloc(1000*sizeof(char));
	if(path==NULL)
		error_handle("Malloc failed");
	char *pathl=(char *)malloc(1000*sizeof(char));
	if(pathl==NULL)
                error_handle("Malloc failed");
	if(ecmd_brk[1]==NULL)
	{
		pid=getpid();
		sprintf(path,"/proc/%d/stat",pid);
		sprintf(pathl,"/proc/%d/exe",pid);
	}
	else
	{
		sprintf(path,"/proc/%s/stat",ecmd_brk[1]);
		sprintf(pathl,"/proc/%s/exe",ecmd_brk[1]);
	}
	int fd=open(path,O_RDONLY);
	if(fd==-1)
	{
		printf("No process with the given PID exist\n");
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
	int che;
	char data[2048];
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
	printf("pid -- %s\n",brk_up[0]);
	char **fbrk;
	fbrk=(char **)malloc(100*sizeof(char *));
	if(fbrk==NULL)
		error_handle("Malloc failed");
	int ctr=0;
	fbrk[ctr]=strtok(brk_up[num-1]," ");
	while(fbrk[ctr++]!=NULL)
	{
		if(ctr==1)
			printf("Process Status -- %s\n",fbrk[0]);
		else if(ctr==21)
			printf("memory --	%s\n",fbrk[20]);
		fbrk[ctr]=strtok(NULL," ");
	}
	close(fd);
	che=readlink(pathl,data,2048);
	data[che]='\0';
	if(strcmp(data+(che-9),"(deleted)")==0)
		data[che-10]='\0';	
	char newdat[2048];
	if(strlen(data)>=strlen(home_dir))
	{
		int i,done=0;
		for(i=0;i<strlen(home_dir);i++)
		{
			if(home_dir[i]!=data[i])
				done=1;
		}
		if(done==0)
		{
			newdat[0]='~';
			newdat[1]='/';
			strcpy(newdat+2,data+i);
			strcpy(data,newdat);
		}
	}
	printf("Executable Path -- %s\n",data);	
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
	return ;
}

