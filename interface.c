#include "h_files.h"
void interface()
{
	int cc,curdirsz,bgproct=0,szbg=1000,i;
	int *pidbg,*exc;
	char *hs_name,*home_dir;
	char ***bgcmds;
	char curdir[100000],act_curdir[100000],lastdir[100000],prev_act[100000];
	lastdir[0]='\0';
	prev_act[0]='\0';

//***************************************************************************************
	
	struct passwd *gu_name=getpwuid(geteuid());		//get_user_name
	if(gu_name==NULL)
                error_handle("Internal error occured while working with user id's");
	home_dir=(char *)malloc(100000*sizeof(char));
	if(home_dir==NULL)
		error_handle("Malloc failed");
	hs_name=(char *)malloc(100000*sizeof(char));
	if(hs_name==NULL)
		error_handle("Malloc failed");
	cc=gethostname(hs_name,100000);				//get_host_name
	if(cc==-1)
		error_handle("Couldn't get host name");
	hs_name=(char *) realloc(hs_name,sizeof(char)*(strlen(hs_name)+2));
	if(hs_name==NULL)
		error_handle("Realloc failed");
	printf("\033[0;36m");
	printf("<%s@%s:~>",gu_name->pw_name,hs_name);
	printf("\033[0m");
	curdir[0]='~';
	curdirsz=1;
	if(getcwd(act_curdir,100000)==NULL)
		error_handle("Error occured while getting actual current directory");
	if(!(strlen(act_curdir)==1 && act_curdir[0]=='/'))
	{
		act_curdir[strlen(act_curdir)+1]='\0';
		act_curdir[strlen(act_curdir)]='/';
	}
	home_dir=strcpy(home_dir,act_curdir);

//***************************************************************************************

	pidbg=(int *)malloc(1000*sizeof(int));
	if(pidbg==NULL)
		error_handle("Malloc failed..");
	bgcmds=(char ***)malloc(1000*sizeof(char **));
	if(bgcmds==NULL)
		error_handle("Malloc failed..");
	for(i=0;i<1000;i++)
		pidbg[i]=0;
	exc=(int *)malloc(sizeof(int));
	if(exc==NULL)
		error_handle("Malloc failed..");
	*exc=1;

//***************************************************************************************
	
	while(*exc)
	{
		signal(SIGINT,SIG_IGN);
		signal(SIGTSTP,SIG_IGN);
		commands(act_curdir,curdir,&curdirsz,home_dir,&bgproct,exc,pidbg,&szbg,bgcmds,lastdir,prev_act);
		fflush(stdout);
                fflush(stdin);
		if(*exc==1)
		{
			check_end_bg(pidbg,&bgproct,bgcmds);
			curdir[curdirsz]='\0';
			fflush(stdout);
			fflush(stdin);
			printf("\033[0;36m");
			printf("<%s@%s:%s>",gu_name->pw_name,hs_name,curdir);
			printf("\033[0m");
		}
		else
			printf("\n\n Exiting krikash: ....\n\n Bubie!!! Have a nice day :)\n\n");
	}

//***************************************************************************************
	handle_overkill(pidbg,&bgproct,bgcmds,STDIN_FILENO,STDOUT_FILENO);	
	free(exc);
	free(pidbg);
	free(hs_name);
	free(home_dir);
	free(bgcmds);	
	return;
}
