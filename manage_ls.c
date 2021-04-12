#include "h_files.h"
void calc(char **namesofd,int ind,char cdnme[],int file)
{
	char mnths[][5]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	struct stat cong[ind+1];
	int i,j,ctr=0,mon_size=0,mgn_size=0,max_fsize=0,max_date=0,max_hlsize=0;
	for(i=0;i<ind;i++)
	{
		char *temp;
		temp=(char *)malloc((strlen(cdnme)+strlen(namesofd[i])+14)*sizeof(char));
		if(temp==NULL)
			error_handle("Malloc failed..");
		temp=strcpy(temp,cdnme);
		if(file==0)
		{
			if(cdnme[strlen(cdnme)-1]!='/')
			{
				cdnme[strlen(cdnme)+1]='\0';
				cdnme[strlen(cdnme)]='/';
			}
			temp=strcpy(temp,cdnme);
			temp=strcat(temp,namesofd[i]);
		}
		j=stat(temp,&cong[i]);
		if(j!=0)
			error_handle("Some internal error occured");
		ctr=ctr+cong[i].st_blocks;
		struct passwd *pws = getpwuid(cong[i].st_uid);
		if(mon_size<strlen(pws->pw_name))
			mon_size=strlen(pws->pw_name);
		struct group *gws = getgrgid(cong[i].st_gid);
		if(mgn_size<strlen(gws->gr_name))
			mgn_size=strlen(gws->gr_name);
		if(max_fsize<cong[i].st_size)
			max_fsize=cong[i].st_size;
		struct tm* foo=gmtime(&(cong[i].st_mtime));
		if(max_date<(foo->tm_mday))
		{
			max_date=foo->tm_mday;
			if(foo->tm_hour>=19)
				max_date++;
			if(foo->tm_hour==18 && foo->tm_min>=30)
				max_date++;
		}
		if(max_hlsize<cong[i].st_nlink)
			max_hlsize=cong[i].st_nlink;
	}
	ctr=ctr/2;
	if(file==0)
		printf("total %d\n",ctr);
	int a;
	for(i=0;i<ind;i++)
	{
		char permission[10];
		if(cong[i].st_mode & S_IFDIR)
			permission[0]='d';
		else
			permission[0]='-';
		if(cong[i].st_mode & S_IRUSR)
			permission[1]='r';
		else
			permission[1]='-';
		if(cong[i].st_mode & S_IWUSR)
			permission[2]='w';
		else
			permission[2]='-';
		if(cong[i].st_mode & S_IXUSR)
                        permission[3]='x';
                else
                        permission[3]='-';
		if(cong[i].st_mode & S_IRGRP)
                        permission[4]='r';
                else
                        permission[4]='-';
		if(cong[i].st_mode & S_IWGRP)
                        permission[5]='w';
                else
                        permission[5]='-';
		if(cong[i].st_mode & S_IXGRP)
                        permission[6]='x';
                else
                        permission[6]='-';
		if(cong[i].st_mode & S_IROTH)
                        permission[7]='r';
                else
                        permission[7]='-';
		if(cong[i].st_mode & S_IWOTH)
                        permission[8]='w';
                else
                        permission[8]='-';
		if(cong[i].st_mode & S_IXOTH)
                        permission[9]='x';
                else
                        permission[9]='-';
		printf("%s ",permission);
		a=log_function(max_hlsize);
		int b=log_function(cong[i].st_nlink);
		for(int t=0;t<(a-b);t++)
			printf(" ");
		printf("%ld ",cong[i].st_nlink);
		struct passwd *pws = getpwuid(cong[i].st_uid);
		a=mon_size;
		b=strlen(pws->pw_name);
		printf("%s ",pws->pw_name);
		for(int t=0;t<(a-b);t++)
                        printf(" ");
		struct group *gws=getgrgid(cong[i].st_gid);
		a=mgn_size;
		b=strlen(gws->gr_name);
		printf("%s ",gws->gr_name);
		for(int t=0;t<(a-b);t++)
                        printf(" ");
		a= log_function(max_fsize);
		b=log_function(cong[i].st_size);
		for(int t=0;t<(a-b);t++)
                        printf(" ");
		printf("%ld ",cong[i].st_size);
		struct tm *foo=gmtime(&(cong[i].st_mtime));
		printf("%s ",mnths[foo->tm_mon]);
		a= log_function(max_date);
		int curday=foo->tm_mday,hour=foo->tm_hour,min=foo->tm_min;
		if(hour>=19)
			curday++;
		if(hour==18 && min>=30)
			curday++;
		b=log_function(curday);
		for(int t=0;t<(a-b);t++)
                        printf(" ");
		printf("%d ",curday);
		min=min+30;
		if(min>=60)
		{
			hour++;
			min=min%60;
		}
		hour=hour+5;
		if(hour>=24)
			hour=hour%24;
		printf("%02d:%02d ",hour,min);
		printf("%s\n",namesofd[i]);
	}			
	return ;
}	
void manage_ls(char **flad,char act_dir[],char home_dir[],int num,int rd_end,int wr_end)
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
	int l=0,a=0,i=0,k=0,j,arr[num+2],t,mascnd=0;
	char **dirs;
	struct stat tri;
        struct dirent *dent;
	dirs=(char **)malloc((num+4)*sizeof(char *));
	if(dirs==NULL)
		error_handle("Malloc failed..");
	for(int i=0;i<num+2;i++)
		arr[i]=-1;
	while(flad[i]!=NULL)
	{
		if(flad[i][0]=='-')
		{
			if(l!=1 || a!=1)
			{
				for(j=1;j<strlen(flad[i]);j++)
				{
					if(flad[i][j]!='-')
					{
						if(l!=1 || a!=1)
						{
							if(flad[i][j]=='l')
								l=1;
							if(flad[i][j]=='a')
								a=1;
						}
					}
				}
			}
		}
		else
		{
			arr[k]=i;
			if(flad[i][0]=='~')
			{
				dirs[k]=(char *)malloc((strlen(home_dir)+strlen(flad[i])+4)*sizeof(char));
				if(dirs[k]==NULL)
					error_handle("Malloc failed..");
				dirs[k]=strcpy(dirs[k],home_dir);
				j=strlen(home_dir);
				for(t=2;t<strlen(flad[i]);t++)
					dirs[k][j++]=flad[i][t];
				dirs[k][j]='\0';
			}
			else if(flad[i][0]=='/')
			{
				dirs[k]=(char *)malloc((strlen(flad[i])+2)*sizeof(char));
				if(dirs[k]==NULL)
					error_handle("Malloc failed..");
				dirs[k]=strcpy(dirs[k],flad[i]);
			}
			else if(strlen(flad[i])==1 && flad[i][0]=='.')
			{
				dirs[k]=(char *)malloc((strlen(act_dir)+strlen(flad[i])+4)*sizeof(char));
				if(dirs[k]==NULL)
					error_handle("Malloc failed..");
                                dirs[k]=strcpy(dirs[k],act_dir);
			}
			else
			{
				dirs[k]=(char *)malloc((strlen(act_dir)+strlen(flad[i])+4)*sizeof(char));
				if(dirs[k]==NULL)
					error_handle("Malloc failed..");
                                dirs[k]=strcpy(dirs[k],act_dir);
                                j=strlen(act_dir);
                                for(t=0;t<strlen(flad[i]);t++)
                                        dirs[k][j++]=flad[i][t];
                                dirs[k][j]='\0';
                        }
			k++;
		}
		i++;
	}
	if(k==0)
	{
		flad[i]=".";
		flad[i+1]=NULL;
		dirs[k]=(char *)malloc((strlen(act_dir)+strlen(flad[i])+4)*sizeof(char));
		if(dirs[k]==NULL)
			error_handle("Malloc failed..");
                dirs[k]=strcpy(dirs[k],act_dir);
		arr[k]=i;
		k++;
		i++;
	}
	for(j=0;j<k;j++)
	{
		i=stat(dirs[j],&tri);
		if(i!=0)
		{
			printf("ls: cannot access '%s': No such file or directory\n",flad[arr[j]]);
			continue;
		}
		if(mascnd==1)
			printf("\n");
		else
			mascnd=1;
		int file=0;
		if(l==0)
		{
			if(tri.st_mode & S_IFDIR)
			{
				if(k>1)
                        		printf("%s:\n",flad[arr[j]]);
				DIR *dr=opendir(dirs[j]);
				if(dr==NULL)
					error_handle("Couldn't open current directory");
				while((dent=readdir(dr))!=NULL)
				{
					char *nme=(char *)malloc((strlen(dent->d_name)+2)*sizeof(char));
					if(nme==NULL)
						error_handle("Malloc failed..");
					nme=strcpy(nme,dent->d_name);
					if(nme[0]=='.')
					{
						if(a==1)
							printf("%s\n",nme);
					}
					else
						printf("%s\n",nme);
				}
			}
			else
			{
				printf("%s\n",flad[arr[j]]);
				if(k>1)
					printf("\n");
			}
		}
		else
		{
			int ind=0;
			char **namesofd;
                        namesofd=(char **)malloc(100000*sizeof(char *));
			if(namesofd==NULL)
				error_handle("Malloc failed..");
			if(tri.st_mode & S_IFDIR)
			{
				if(k>1)
					printf("%s:\n",flad[arr[j]]);
				DIR *dr=opendir(dirs[j]);
                                if(dr==NULL)
                                        error_handle("Couldn't open current directory");
				int tctr=0;
                                while((dent=readdir(dr))!=NULL)
                                {
                                        char *nme=(char *)malloc((strlen(dent->d_name)+2)*sizeof(char));
                                        if(nme==NULL)
						error_handle("Malloc failed..");
					nme=strcpy(nme,dent->d_name);
                                        if(nme[0]=='.')
                                        {
                                                if(a==1)
						{
							namesofd[ind]=(char *)malloc((strlen(nme)+2)*sizeof(char));
							if(namesofd[ind]==NULL)
								error_handle("Malloc failed..");
							namesofd[ind]=strcpy(namesofd[ind],nme);
							ind++;
						}
					}
					else
					{
						namesofd[ind]=(char *)malloc((strlen(nme)+2)*sizeof(char));
						if(namesofd[ind]==NULL)
							error_handle("Malloc failed..");
						namesofd[ind]=strcpy(namesofd[ind],nme);
                                                ind++;
                                        }
				}
				file=0;
			}
			else
			{
				namesofd[ind]=(char *)malloc((strlen(flad[arr[j]])+2)*sizeof(char));
				if(namesofd[ind]==NULL)
					error_handle("Malloc failed..");
				namesofd[ind]=strcpy(namesofd[ind],flad[arr[j]]);
				ind++;
				file=1;
			}
			if(ind==0)
				printf("total 0\n");
			else
				calc(namesofd,ind,dirs[j],file);
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
