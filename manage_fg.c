#include "h_files.h"
int ref=-1,bgadd;
/*void handle_sigtstp(int sig)
{
	if(ref==-1)
		return;
	kill(ref,SIGSTOP);
	setpgid(ref,ref);
	bgadd=1;
	return;
}
void handle_child_sigstop(int sig)
{
	printf("Wow one step closer\n");
	signal(SIGINT,SIG_IGN);
	signal(SIGSTOP,SIG_DFL);
	raise(SIGSTOP);
	return;
}*/
void manage_fg(char **ecmd_brk,char act_dir[],char home_dir[],int rd_end,int wr_end,int *bgproct,int pidbg[],char **bgcmds[],int num)
{
	//signal(SIGTSTP,handle_sigtstp);
	signal(SIGTSTP,SIG_IGN);
	int stat;
	bgadd=0;
	ref=-1;
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
	int rv=chdir(act_dir),cpid;
	if(rv==-1)
		error_handle("Some Internal Error occured");
	ref=fork();
	if(ref==0)
	{
		red=setpgid(0,0);
		if(red==-1)
			error_handle("Internal Error Occured");
		signal(SIGINT,SIG_DFL);
		signal(SIGTSTP,SIG_DFL);
		rv=execvp(ecmd_brk[0],ecmd_brk);
		if(rv==-1)
		{
			printf("Couldn't find the command %s\n",ecmd_brk[0]);
			exit(1);
		}
	}
	else
	{
		int wstat;
		red=setpgid(ref,ref);
		if(red==-1)
			error_handle("Internal Error Occured");
		signal(SIGTTOU,SIG_IGN);
		tcsetpgrp(STDIN_FILENO,ref);
		signal(SIGTTOU,SIG_DFL);
		kill(ref,SIGCONT);
		cpid=waitpid(ref,&wstat,WUNTRACED);
		if(cpid==-1)
			error_handle("waitpid failed");
		signal(SIGTTOU,SIG_IGN);
		tcsetpgrp(STDIN_FILENO,getpgid(getpid()));
		signal(SIGTTOU,SIG_DFL);
		if(WIFSTOPPED(wstat))
			bgadd=1;
		else
			bgadd=0;
		rv=chdir(home_dir);
		if(rv==-1)
			error_handle("Some Internal Error occured");
		//signal(SIGTSTP,SIG_IGN);
		if(bgadd==1)
		{
			printf("\n[%d] %d\n",((*bgproct)+1),ref);
			int i=*bgproct;
			pidbg[i]=ref;
			bgcmds[i]=(char **)malloc((num+4)*sizeof(char *));
			if(bgcmds[i]==NULL)
				error_handle("Malloc failed");
			int ctrtri=0;
			while(ecmd_brk[ctrtri]!=NULL)
			{
				bgcmds[i][ctrtri]=(char *)malloc((strlen(ecmd_brk[ctrtri])+4)*sizeof(char));
				if(bgcmds[i][ctrtri]==NULL)
					error_handle("Malloc failed");
				bgcmds[i][ctrtri]=strcpy(bgcmds[i][ctrtri],ecmd_brk[ctrtri]);
				ctrtri++;
			}
			bgcmds[i][ctrtri]=NULL;
			*bgproct=*bgproct+1;
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
	return ;
}
