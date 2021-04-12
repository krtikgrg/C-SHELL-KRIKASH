#include "h_files.h"
void manage_cd(char **ecmd_brk,char act_dir[],char curdir[],int *curdirsz,char home_dir[],int rd_end,int wr_end,char lastdir[],char prev_act[])
{
	int red;
	char *tempo=(char *)malloc((strlen(curdir)+5)*sizeof(char));
        char *tempoact=(char *)malloc((strlen(act_dir)+5)*sizeof(char));
	tempo=strcpy(tempo,curdir);
	tempoact=strcpy(tempoact,act_dir);
	int resinfd,resopfd;
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
	int i,cnd=0;
	char *to_dir;
	if(ecmd_brk[1]==NULL)
		cnd=1;
	if(ecmd_brk[1]!=NULL && ecmd_brk[2]!=NULL)
	{
		printf("krikash : cd : Too many arguments supplied\n");
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
	if(cnd==0)
	{
		to_dir=malloc((strlen(ecmd_brk[1])+1)*sizeof(char));
		if(to_dir==NULL)
			error_handle("Malloc failed");
		to_dir=strcpy(to_dir,ecmd_brk[1]);
	}
	else
	{
		to_dir=(char *)malloc(4*sizeof(char));
		if(to_dir==NULL)
			error_handle("Malloc failed");
		to_dir[0]='~';
		to_dir[1]='\0';
	}
	if(to_dir[strlen(to_dir)-1]=='/' && strlen(to_dir)>1)
		to_dir[strlen(to_dir)-1]='\0';
	if(strlen(to_dir)==1 && to_dir[0]=='-')
	{
		if(strlen(lastdir)==0)
			printf("%s\n",act_dir);
		else
		{
			curdir=strcpy(curdir,lastdir);
			lastdir=strcpy(lastdir,tempo);
			*curdirsz=strlen(curdir);
			act_dir=strcpy(act_dir,prev_act);
			prev_act=strcpy(prev_act,tempoact);
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
			error_handle("Internal Erro occured");
                return;
	}		
	if(strlen(to_dir)==1 && to_dir[0]=='.') //if curr then return
	{
		lastdir=strcpy(lastdir,tempo);
        	prev_act=strcpy(prev_act,tempoact);
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
	if(strlen(to_dir)==1 && to_dir[0]=='~') //if home then go home
	{
		act_dir=strcpy(act_dir,home_dir);
		curdir[0]='~';
		curdir[1]='\0';
		*curdirsz=1;
		lastdir=strcpy(lastdir,tempo);
        	prev_act=strcpy(prev_act,tempoact);
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
	if(strlen(to_dir)==2 && to_dir[0]=='.' && to_dir[1]=='.')  //case of prev dir
	{
		if(curdir[0]=='~' && *curdirsz==1)   //if cwd was home
		{
			i=strlen(act_dir)-2;
			while(i>=0 && act_dir[i]!='/')
				i--;
			if(i<0)
			{
				lastdir=strcpy(lastdir,tempo);
        			prev_act=strcpy(prev_act,tempoact);
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
					error_handle("Internal Erro Occured");
				return;
			}
			act_dir[i+1]='\0';
			curdir=strcpy(curdir,act_dir);
			if(strlen(act_dir)>1)
			curdir[i]='\0';
			*curdirsz=strlen(curdir);
			lastdir=strcpy(lastdir,tempo);
        		prev_act=strcpy(prev_act,tempoact);
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

		//if cwd is not home
		
		i=strlen(act_dir)-2;
                while(i>=0 && act_dir[i]!='/')
                	i--;
                if(i<0)
                {
                	printf("ERROR:Can't go more back\n");
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
		act_dir[i+1]='\0';
		i=*curdirsz-1;
                while(i>=0 && curdir[i]!='/')
                        i--;
                if(i<0)
                {
                        printf("ERROR:Can't go more back\n");
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
		if(i!=0)
			curdir[i]='\0';
		else
			curdir[1]='\0';
		*curdirsz=strlen(curdir);
		lastdir=strcpy(lastdir,tempo);
        	prev_act=strcpy(prev_act,tempoact);
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

	//implementation for actual paths and relative paths (to this home)
	
	int k=0;
	char *cpypath;
	cpypath=(char *)malloc(100000*sizeof(char));
	if(cpypath==NULL)
		error_handle("Malloc failed..");
	if(to_dir[0]=='~')	
	{
		cpypath=strcpy(cpypath,home_dir);
		k=strlen(home_dir);
		for(i=2;i<strlen(to_dir);i++)
			cpypath[k++]=to_dir[i];
		cpypath[k]='\0';
	}
	else if(to_dir[0]!='/')
	{
		cpypath=strcpy(cpypath,act_dir);
		k=strlen(act_dir);
		i=0;
		if(to_dir[0]=='.' && to_dir[1]=='/')
			i=2;
		while(i<strlen(to_dir))
			cpypath[k++]=to_dir[i++];
		cpypath[k]='\0';
	}
	else
	{
		cpypath=strcpy(cpypath,to_dir);
		if(strlen(cpypath)==1)
		{
			act_dir=strcpy(act_dir,cpypath);
			curdir=strcpy(curdir,cpypath);
			*curdirsz=1;
			lastdir=strcpy(lastdir,tempo);
        		prev_act=strcpy(prev_act,tempoact);
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
	}

//***************************************************************************************	
	
	struct stat testf;
	i=stat(cpypath,&testf);
	if(i!=0)
	{
		printf("krikash: cd: %s: No such directory\n",cpypath);
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
	if(testf.st_mode & S_IFDIR)
	{
		act_dir=strcpy(act_dir,cpypath);
		char **dirtok;
		dirtok=(char **)malloc(10000*sizeof(char*));
		if(dirtok==NULL)
			error_handle("Malloc failed..");
		dirtok[0]=strtok(cpypath,"/");
		k=0;
		while(dirtok[k]!=NULL)
		{
			if(strlen(dirtok[k])==1 && dirtok[k][0]=='.')
				k--;
			else if(strlen(dirtok[k])==2 && dirtok[k][0]=='.' && dirtok[k][1]=='.')
			{
				if(k!=0)
					k=k-2;
			}
			dirtok[++k]=strtok(NULL,"/");
		}
		i=0;
		k=0;
		if(act_dir[0]=='/')
                        k=1;
		while(dirtok[i]!=NULL)
		{
			for(int j=0;j<strlen(dirtok[i]);j++)
				act_dir[k++]=dirtok[i][j];
			act_dir[k++]='/';
			i++;
		}
		act_dir[k]='\0';
		int done=0;
		for(i=0;i<strlen(home_dir);i++)
		{
			if(home_dir[i]!=act_dir[i])
				done=1;
		}
		if(done==0)
		{
			curdir[0]='~';
			k=1;
			curdir[k++]='/';
			while(i<strlen(act_dir))
				curdir[k++]=act_dir[i++];
			curdir[k-1]='\0';
			*curdirsz=strlen(curdir);
			lastdir=strcpy(lastdir,tempo);
        		prev_act=strcpy(prev_act,tempoact);
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
		else
		{
			curdir=strcpy(curdir,act_dir);
			if(!(strlen(act_dir)==1 && act_dir[0]=='/'))
			curdir[strlen(curdir)-1]='\0';
			*curdirsz=strlen(curdir);
			lastdir=strcpy(lastdir,tempo);
        		prev_act=strcpy(prev_act,tempoact);
		}	
	}
	else
		printf("krikash: cd: %s: No such directory\n",cpypath);
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
