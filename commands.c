#include "h_files.h"
void commands(char act_curdir[], char curdir[], int *curdirsz, char home_dir[],int *bgproct,int *exc,int pidbg[],int *szbg,char **bgcmds[],char lastdir[],char prev_act[])
{
	int ctr=0,no_of_cmds=1000,j,redirect,pip;
	long unsigned int sz=100000;
	char s_n;
	char *g_comm;
	char **cmd_brk;
	g_comm=(char *)malloc(100000*sizeof(char));
	if(g_comm==NULL)
		error_handle("Error occured in malloc");
	getline(&g_comm,&sz,stdin);
	if(feof(stdin))
	{
		*exc=0;
		return;
	}
	g_comm=(char *)realloc(g_comm,(strlen(g_comm)+2)*sizeof(char));
	if(g_comm==NULL)
		error_handle("Error occured in realloc");

//***************************************************************************************
/* get individual commands in case multiple commands entered using ; */

	cmd_brk=(char **)malloc(1000*sizeof(char *));
	if(cmd_brk==NULL)
		error_handle("Error occured in malloc");
	cmd_brk[ctr]=strtok(g_comm,";");
	while(cmd_brk[ctr++]!=NULL)
	{
		if(ctr==no_of_cmds)
		{
			no_of_cmds=((ctr/1000)+1)*1000;
			cmd_brk=(char **)realloc(cmd_brk,no_of_cmds*sizeof(char *));
			if(cmd_brk==NULL)
				error_handle("Error occured in realloc");
		}
		cmd_brk[ctr]=strtok(NULL,";");
	}
	cmd_brk[ctr-2][strlen(cmd_brk[ctr-2])-1]='\0';
	cmd_brk=(char **)realloc(cmd_brk,(ctr+1)*sizeof(char *));
	if(cmd_brk==NULL)
		error_handle("Error occured in realloc");

//***************************************************************************************

	int i=0,num,val;
	while(cmd_brk[i]!=NULL)
	{
		signal(SIGINT,SIG_IGN);
		signal(SIGTSTP,SIG_IGN);
		char *tempr;
		char **ecmd_brk;
		ecmd_brk=(char **)malloc(1000*sizeof(char *));
		if(ecmd_brk==NULL)
			error_handle("Error occured in malloc");
		num=0;
		val=1000;
		tempr=(char *)malloc((strlen(cmd_brk[i])+1)*sizeof(char));
		if(tempr==NULL)
			error_handle("Malloc failed..");
		tempr=strcpy(tempr,cmd_brk[i]);
		ecmd_brk[num]=strtok(tempr," \t");	//to remove extra space and tabs
		while(ecmd_brk[num++]!=NULL)
		{
			if(num==val)
			{
				val=((num/1000)+1)*1000;
				ecmd_brk=(char **)realloc(ecmd_brk,val*sizeof(char *));
				if(ecmd_brk==NULL)
					error_handle("Error occured in realloc");
			}
			ecmd_brk[num]=strtok(NULL," \t");
		}
		num--;
		if(num>0)
		{
			pip=0;
			redirect=0;
			add_in_history(ecmd_brk);
			for(j=0;j<num;j++)
			{
				if(ecmd_brk[j][0]=='|')
				{
					handle_pipe(ecmd_brk,j,num,act_curdir,home_dir,STDIN_FILENO,STDOUT_FILENO,curdir,curdirsz,lastdir,prev_act,bgproct,pidbg,bgcmds);
					pip=1;
					break;
				}
				else if(ecmd_brk[j][0]=='>' || ecmd_brk[j][0]=='<')
					redirect=1;
			}
			if(pip==0)
			{	
				if(strlen(ecmd_brk[num-1])==1 && ecmd_brk[num-1][0]=='&')
				{
					ecmd_brk[num-1]=NULL;
					manage_bg(ecmd_brk,act_curdir,home_dir,bgproct,pidbg,bgcmds,num);
				}
				else if(redirect == 1)
                                        handle_redirect(ecmd_brk,num,act_curdir,home_dir,STDIN_FILENO,STDOUT_FILENO,curdir,curdirsz,lastdir,prev_act,bgproct,pidbg,bgcmds);
				else if(strlen(ecmd_brk[0])==2 && ecmd_brk[0][0]=='c' && ecmd_brk[0][1]=='d')
					manage_cd(ecmd_brk,act_curdir,curdir,curdirsz,home_dir,STDIN_FILENO,STDOUT_FILENO,lastdir,prev_act);
				else if(strlen(ecmd_brk[0])==3 && ecmd_brk[0][0]=='p' && ecmd_brk[0][1]=='w' && ecmd_brk[0][2]=='d')
					manage_pwd(act_curdir,STDIN_FILENO,STDOUT_FILENO);
				else if(strlen(ecmd_brk[0])==4 && ecmd_brk[0][0]=='e' && ecmd_brk[0][1]=='c' && ecmd_brk[0][2]=='h' && ecmd_brk[0][3]=='o')
					manage_echo(ecmd_brk+1,STDIN_FILENO,STDOUT_FILENO);
				else if(strlen(ecmd_brk[0])==2 && ecmd_brk[0][0]=='l' && ecmd_brk[0][1]=='s')
					manage_ls(ecmd_brk+1,act_curdir,home_dir,val,STDIN_FILENO,STDOUT_FILENO);
				else if(strlen(ecmd_brk[0])==4 && ecmd_brk[0][0]=='q' && ecmd_brk[0][1]=='u' && ecmd_brk[0][2]=='i' && ecmd_brk[0][3]=='t')
				{
					*exc=0;
					break;
				}
				else if(strlen(ecmd_brk[0])==5 && ecmd_brk[0][0]=='p' && ecmd_brk[0][1]=='i' && ecmd_brk[0][2]=='n' && ecmd_brk[0][3]=='f'&& ecmd_brk[0][4]=='o' )
					manage_pinfo(ecmd_brk,home_dir,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"history")==0)
					manage_history(ecmd_brk,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"setenv")==0)
					handle_setenv(ecmd_brk,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"unsetenv")==0)
					handle_unsetenv(ecmd_brk,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"kjob")==0)
					handle_kjob(ecmd_brk,pidbg,bgcmds,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"overkill")==0)
					handle_overkill(pidbg,bgproct,bgcmds,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"jobs")==0)
					handle_jobs(bgcmds,pidbg,bgproct,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"bg")==0)
					handle_bag(ecmd_brk,pidbg,bgproct,STDIN_FILENO,STDOUT_FILENO);
				else if(strcmp(ecmd_brk[0],"fg")==0)
					handle_fag(ecmd_brk,pidbg,bgcmds,STDIN_FILENO,STDOUT_FILENO);
				else
					manage_fg(ecmd_brk,act_curdir,home_dir,STDIN_FILENO,STDOUT_FILENO,bgproct,pidbg,bgcmds,num);
			}
			if(*bgproct==*szbg)
                        {
                        	*szbg=(((*bgproct)/1000)+1)*1000;
                                pidbg=(int *)realloc(pidbg,(*szbg)*sizeof(int));
                                if(pidbg==NULL)
                                	error_handle("Realloc failed..");
                                bgcmds=(char ***)realloc(bgcmds,(*szbg)*sizeof(char **));
				if(bgcmds==NULL)
					error_handle("Realloc failed");
                        }
		}
		i++;
		free(ecmd_brk);
	}
	free(cmd_brk);
	return;
}
