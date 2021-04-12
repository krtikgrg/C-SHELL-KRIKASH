#include "h_files.h"
void manage_bg(char **ecmd_brk,char act_dir[], char home_dir[],int *bgproct,int pidbg[],char **bgcmds[],int num)
{
	int resinfd,resopfd,red;
	resinfd=dup(STDIN_FILENO);
	resopfd=dup(STDOUT_FILENO);
	if(resinfd==-1 || resopfd==-1)
		error_handle("dup failed");
	int rv=chdir(act_dir),cpid;
	if(rv==-1)
		error_handle("Some Internal Error occured");
	int pid=getpid();
	int pgid=getpgid(pid);
	if(pgid==-1)
		error_handle("Internal Error occured");

	//***************************************************************************************	

	int i=0,inp=0,op=0,inpstrt=-1,opstrt=-1,O=0,A=0,first,end,rfd,ofd;
	char *inpfile,*opfile;
	umask(0000);
	while(ecmd_brk[i]!=NULL)
	{
		if(ecmd_brk[i][0]=='<')
		{
			inp=1;
			inpstrt=i+1;
		}
		else if(ecmd_brk[i][0] == '>')
		{
			op=1;
			opstrt=i+1;
			if(strlen(ecmd_brk[i])==2)
				A=1;
			else
				O=1;
		}
		i++;
	}
	if(inp==1)
	{
		if(op==1)
		{
			end=opstrt-1;
			first=1;
			for(int ctr=opstrt;ctr<i;ctr++)
			{
				if(first==0)
				{
					opfile=(char *)realloc(opfile,(strlen(opfile)+strlen(ecmd_brk[ctr])+3)*sizeof(char));
					if(opfile==NULL)
						error_handle("Realloc failed");
					opfile=strcat(opfile," ");
					opfile=strcat(opfile,ecmd_brk[ctr]);
				}
				else
				{
					opfile=(char *)malloc((strlen(ecmd_brk[ctr])+3)*sizeof(char));
					if(opfile==NULL)
                                                error_handle("Malloc failed");
					opfile=strcpy(opfile,ecmd_brk[ctr]);
					first=0;
				}
			}
		}
		else
			end=i;
		first=1;
		for(int ctr=inpstrt;ctr<end;ctr++)
		{
			if(first==0)
			{
				inpfile=(char *)realloc(inpfile,(strlen(inpfile)+strlen(ecmd_brk[ctr])+3)*sizeof(char));
				if(inpfile==NULL)
                                	error_handle("Realloc failed");
				inpfile=strcat(inpfile," ");
				inpfile=strcat(inpfile,ecmd_brk[ctr]);
			}
			else
			{
				inpfile=(char *)malloc((strlen(ecmd_brk[ctr])+3)*sizeof(char));
				if(inpfile==NULL)
                                	error_handle("Malloc failed");
				inpfile=strcpy(inpfile,ecmd_brk[ctr]);
				first=0;
			}
		}
	}
	if(inp==1)
	{
		rfd=open(inpfile,O_RDONLY);
		if(rfd==-1)
			error_handle("open system call failed");
		red=dup2(rfd,STDIN_FILENO);
		if(red==-1)
			error_handle("dup2 system call failed");
	}
	if(op==1)
	{
		if(O==1)
			ofd=open(opfile,O_WRONLY | O_CREAT | O_TRUNC,0644);
		if(A==1)
		{
			struct stat tri;
                	int exi=stat(opfile,&tri);
                	if(exi==-1)
                        	ofd=open(opfile,O_WRONLY | O_CREAT | O_TRUNC,0644);
               	 	else
                        	ofd=open(opfile,O_WRONLY | O_APPEND);
		}
		if(ofd==-1)
			error_handle("open system call failed");
		red=dup2(ofd,STDOUT_FILENO);
		if(red==-1)
			error_handle("dup2 system call failed");
	}
	if(inp==1)
		ecmd_brk[inpstrt-1]=NULL;

	//***************************************************************************************	
	
	int ref=fork();
	if(ref==0)
	{
		rv=setpgid(ref,ref);
		if(rv==-1)
			error_handle("Some error occured..");
		signal(SIGINT,SIG_DFL);
		signal(SIGTSTP,SIG_DFL);
		rv=execvp(ecmd_brk[0],ecmd_brk);
		if(rv==-1)
		{
			printf("Couldn't execute the process in background");
			exit(1);
		}
	}
	else if(ref>0)
	{
		rv=setpgid(ref,ref);
                if(rv==-1)
                        error_handle("Some error occured");
		if(inp==1)
		{
			red=dup2(resinfd,STDIN_FILENO);
			if(red==-1)
				error_handle("dup2 system call failed");
		}
		if(op==1)
		{
			red=dup2(resopfd,STDOUT_FILENO);
			if(red==-1)
				error_handle("dup2 system call failed");
		}
		red=close(resinfd);
		red=close(resopfd);
		if(red==-1)
			error_handle("Internal Error Occured");
		printf("[%d] %d\n",((*bgproct)+1),ref);
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
		rv=chdir(home_dir);
		if(rv==-1)
			error_handle("Some Internal Eroor occured");
	}
	else
		error_handle("Some error occured while forking..");
	return;
}
