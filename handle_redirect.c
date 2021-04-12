#include "h_files.h"
void handle_redirect(char **ecmd_brk,int tot,char act_dir[],char home_dir[],int rd_end,int wr_end,char curdir[],int *curdirsz,char lastdir[],char prev_act[],int *bgproct,int pidbg[],char **bgcmds[])
{
	int j,inp=0,k=0,first,O=0,A=0,op=0,cpid,resinfd,resopfd,ofd,fd,ref,topassod=1,topassid=0,red;
	char **argv=(char **)malloc(tot*sizeof(char*));
	if(argv==NULL)
		error_handle("Malloc failed");
	char *opfile,*inpfile;
	resinfd=dup(STDIN_FILENO);
	resopfd=dup(STDOUT_FILENO);
	if(resinfd==-1 || resopfd==-1)
		error_handle("dup system call failed");
	if(rd_end!=0)
	{
		red=dup2(rd_end,STDIN_FILENO);
		if(red==-1)
			error_handle("dup2 failed");
	}
	if(wr_end!= 1)
	{
		red=dup2(wr_end,STDOUT_FILENO);
		if(red==-1)
                        error_handle("dup2 failed");
	}
	red=chdir(act_dir);
	if(red==-1)
		error_handle("Internal Error Occured");
	fd=-2;
	for(j=0;j<tot;j++)
	{
		if(ecmd_brk[j][0]=='<')
		{
			inp=1;
			break;
		}
		else
		{
			argv[k]=(char *)malloc((strlen(ecmd_brk[j])+2)*sizeof(char));
			if(argv[k]==NULL)
				error_handle("Malloc failed");
			argv[k]=strcpy(argv[k],ecmd_brk[j]);
			k++;
		}
	}
	argv[k]=NULL;
	if(inp==1)
	{
		first=1;
		for(j=j+1;j<tot;j++)
		{
			if(ecmd_brk[j][0]=='>')
			{
				if(strlen(ecmd_brk[j])==2)
					A=1;
				else
					O=1;
				op=1;
				break;
			}
			else
			{
				if(first==0)
				{
					inpfile=(char *)realloc(inpfile,(strlen(ecmd_brk[j])+strlen(inpfile)+3)*sizeof(char));
					if(inpfile==NULL)
						error_handle("Realloc failed");
					inpfile=strcat(inpfile," ");
					inpfile=strcat(inpfile,ecmd_brk[j]);
				}
				else
				{
					inpfile=(char *)malloc((strlen(ecmd_brk[j])+3)*sizeof(char));
					if(inpfile==NULL)
                                                error_handle("Malloc failed");
					inpfile=strcpy(inpfile,ecmd_brk[j]);
					first=0;
				}
			}
		}
		if(op==1)
		{
			first=1;
			for(j=j+1;j<tot;j++)
			{	
				if(first==0)
				{
					opfile=(char *)realloc(opfile,(strlen(ecmd_brk[j])+strlen(opfile)+3)*sizeof(char));
					if(opfile==NULL)
                                                error_handle("Realloc failed");
					opfile=strcat(opfile," ");
					opfile=strcat(opfile,ecmd_brk[j]);
				}
				else
				{
					opfile=(char *)malloc((strlen(ecmd_brk[k])+2)*sizeof(char));
					if(opfile==NULL)
                                                error_handle("Malloc failed");
					opfile=strcpy(opfile,ecmd_brk[j]);
					first=0;
				}
			}
		}
		fd=open(inpfile,O_RDONLY);
		if(fd==-1)
		{
			printf("krikash : redirection : ERROR: No file exists at the given location\n");
			if(rd_end!=STDIN_FILENO)
			{
				red=dup2(resinfd,STDIN_FILENO);
				if(red==-1)
					error_handle("dup2 failed");
			}
			if(wr_end!=STDOUT_FILENO)
			{
				red=dup2(resopfd,STDOUT_FILENO);
				if(red==-1)
                                        error_handle("dup2 failed");
			}
			red=chdir(home_dir);
			red=close(resinfd);
			red=close(resopfd);
			if(red==-1)
				error_handle("Internal Error Occured");
			return;
		}
		red=dup2(fd,STDIN_FILENO);
		if(red==-1)
			error_handle("dup2 failed");
		topassid=fd;
	}
	else
	{
		k=0;
		for(j=0;j<tot;j++)
		{
			if(ecmd_brk[j][0]=='>')
			{
				if(strlen(ecmd_brk[j])==2)
					A=1;
				else
					O=1;
				op=1;
				break;
			}
			else
				k++;
		}
		argv[k]=NULL;
		if(op==1)
		{
			first=1;
			for(j=j+1;j<tot;j++)
			{
				if(first==0)
				{
					opfile=(char *)realloc(opfile,(strlen(ecmd_brk[j])+strlen(opfile)+3)*sizeof(char));
					if(opfile==NULL)
						error_handle("Realloc failed");
					opfile=strcat(opfile," ");
					opfile=strcat(opfile,ecmd_brk[j]);
				}
				else
				{
					opfile=(char *)malloc((strlen(ecmd_brk[k])+2)*sizeof(char));
					if(opfile==NULL)
                                                error_handle("Malloc failed");
					opfile=strcpy(opfile,ecmd_brk[j]);
					first=0;
				}
			}
		}
	}
	umask(0000);
	ofd=-2;
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
		error_handle("open failed");
	if(ofd!=-2)
	{
		red=dup2(ofd,STDOUT_FILENO);
		if(red==-1)
			error_handle("dup2 system call failed");
		topassod=ofd;
	}
	if(strlen(argv[0])==2 && argv[0][0]=='c' && argv[0][1]=='d')
		manage_cd(argv,act_dir,curdir,curdirsz,home_dir,topassid,topassod,lastdir,prev_act);
	else if(strlen(argv[0])==3 && argv[0][0]=='p' && argv[0][1]=='w' && argv[0][2]=='d')
		manage_pwd(act_dir,topassid,topassod);
	else if(strlen(argv[0])==4 && argv[0][0]=='e' && argv[0][1]=='c' && argv[0][2]=='h' && argv[0][3]=='o')
		manage_echo(argv+1,topassid,topassod);
	else if(strlen(argv[0])==2 && argv[0][0]=='l' && argv[0][1]=='s')
		manage_ls(argv+1,act_dir,home_dir,tot+512,topassid,topassod);
	else if(strlen(argv[0])==5 && argv[0][0]=='p' && argv[0][1]=='i' && argv[0][2]=='n' && argv[0][3]=='f'&& argv[0][4]=='o' )
		manage_pinfo(argv,home_dir,topassid,topassod);
	else if(strcmp(argv[0],"history")==0)
		manage_history(argv,topassid,topassod);
	else if(strcmp(argv[0],"setenv")==0)
		handle_setenv(argv,topassid,topassod);
	else if(strcmp(argv[0],"unsetenv")==0)
		handle_unsetenv(argv,topassid,topassod);
	else if(strcmp(argv[0],"kjob")==0)
		handle_kjob(argv,pidbg,bgcmds,STDIN_FILENO,STDOUT_FILENO);
	else if(strcmp(argv[0],"overkill")==0)
		handle_overkill(pidbg,bgproct,bgcmds,STDIN_FILENO,STDOUT_FILENO);
	else if(strcmp(argv[0],"jobs")==0)
		handle_jobs(bgcmds,pidbg,bgproct,STDIN_FILENO,STDOUT_FILENO);
	else if(strcmp(argv[0],"bg")==0)
		handle_bag(argv,pidbg,bgproct,STDIN_FILENO,STDOUT_FILENO);
	else if(strcmp(argv[0],"fg")==0)
		handle_fag(argv,pidbg,bgcmds,STDIN_FILENO,STDOUT_FILENO);
	else
		manage_fg(argv,act_dir,home_dir,topassid,topassod,bgproct,pidbg,bgcmds,k);
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
	red=chdir(home_dir);
	if(red==-1)
		error_handle("Internal Error Occured");
	if(fd!=-2)
	{
		red=dup2(resinfd,STDIN_FILENO);
		if(red==-1)
                        error_handle("dup2 system call failed");
		red=close(fd);
		if(red==-1)
                	error_handle("Internal Error Occured");
	}
	if(ofd!=-2)
	{
		red=dup2(resopfd,STDOUT_FILENO);
		if(red==-1)
                        error_handle("dup2 system call failed");
		red=close(ofd);
		if(red==-1)
                        error_handle("Internal Error Occured");
	}
	red=close(resinfd);
	red=close(resopfd);
	if(red==-1)
        	error_handle("Internal Error Occured");
	return;
}
