#include "h_files.h"
void handle_pipe(char ** ecmd_brk,int ind,int tot,char act_dir[],char home_dir[],int rd_end,int wr_end,char curdir[],int *curdirsz,char lastdir[],char prev_act[],int *bgproct,int pidbg[],char **bgcmds[])
{
	int grestrd,grestwr,red;
	grestrd=dup(STDIN_FILENO);
	grestwr=dup(STDOUT_FILENO);
	if(grestrd==-1 || grestwr==-1)
		error_handle("dup system call malfunctioned");
	if(rd_end!=0)
	{
		red=dup2(rd_end,STDIN_FILENO);
		if(red==-1)
			error_handle("dup system call malfunctioned");
	}
	if(wr_end!=1)
	{
		red=dup2(wr_end,STDOUT_FILENO);
		if(red==-1)
                        error_handle("dup system call malfunctioned");
	}
	int fildes[2],i,ref,reswrfd,resedfd,pip;
	red=chdir(act_dir);
	if(red==-1)
		error_handle("Error occured");
	char **curcpy=(char **) malloc((ind+1)*sizeof(char*));
	if(curcpy==NULL)
		error_handle("Malloc failed");
	for(i=0;i<ind;i++)
	{
		curcpy[i]=(char *)malloc((strlen(ecmd_brk[i])+2)*sizeof(char));
		if(curcpy[i]==NULL)
			error_handle("Malloc failed");
		strcpy(curcpy[i],ecmd_brk[i]);
	}
	curcpy[ind]=NULL;
	red=pipe(fildes);
	if(red==-1)
		error_handle("pipe system call failed");
	ref=fork();
	if(ref==0)
	{
		red=close(fildes[0]);
		if(red==-1)
			error_handle("Internal Error Occured");
		handle_redirect(curcpy,ind,act_dir,home_dir,rd_end,fildes[1],curdir,curdirsz,lastdir,prev_act,bgproct,pidbg,bgcmds);
		red=close(fildes[1]);
		if(red==-1)
                        error_handle("Internal Error Occured");
		if(rd_end!=0)
		{
			red=dup2(grestrd,STDIN_FILENO);
			if(red==-1)
				error_handle("dup system call misbehaved");
		}
		if(wr_end!=1)
		{
			red=dup2(grestwr,STDOUT_FILENO);
			if(red==-1)
                                error_handle("dup system call misbehaved");
		}
		red=close(grestrd);
		red=close(grestwr);
		if(red==-1)
			error_handle("Internal Error Occured");
		exit(0);
	}
	else if(ref>0)
	{
		red=close(fildes[1]);
		if(red==-1)
			error_handle("Internal Error Occured");
		red=waitpid(ref,NULL,WUNTRACED);
		if(red==-1)
			error_handle("waitpid falied in piping");
		pip=0;
		for(i=ind+1;i<tot;i++)
		{
			if(ecmd_brk[i][0]=='|')
			{
				handle_pipe(ecmd_brk+ind+1,i-ind-1,tot-ind-1,act_dir,home_dir,fildes[0],wr_end,curdir,curdirsz,lastdir,prev_act,bgproct,pidbg,bgcmds);
				pip=1;
				break;
			}
		}
		if(pip==0)
			handle_redirect(ecmd_brk+ind+1,tot-ind-1,act_dir,home_dir,fildes[0],wr_end,curdir,curdirsz,lastdir,prev_act,bgproct,pidbg,bgcmds);
		if(rd_end!=0)
		{
			red=dup2(grestrd,STDIN_FILENO);
			if(red==-1)
                                error_handle("dup system call misbehaved");
		}
		if(wr_end!=1)
		{
                	red=dup2(grestwr,STDOUT_FILENO);
			if(red==-1)
                                error_handle("dup system call misbehaved");
		}
                red=close(grestrd);
                red=close(grestwr);
		red=close(fildes[0]);
		red=chdir(home_dir);
		if(red==-1)
			error_handle("Internal Error Occured");
		return;
	}
	else
		error_handle("Error : Fork misbehaved");
	return;
}
