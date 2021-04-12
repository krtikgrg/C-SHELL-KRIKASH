#include "h_files.h"
void add_in_history(char **ecmd_brk)
{
	int i=0,sz=1000,x=0,fd,ctr,j,numt;
	char *cmd,*buff,*temp;
	temp=(char *)malloc(10000);
	if(temp==NULL)
		error_handle("Malloc failed..");
	cmd=(char *)malloc(1000*sizeof(char));
	if(cmd==NULL)
		error_handle("Malloc failed..");
	while(ecmd_brk[i]!=NULL)
	{
		x=x+strlen(ecmd_brk[i])+1;
		temp=strcpy(temp,ecmd_brk[i]);
		temp[strlen(ecmd_brk[i])]='\0';
		if(x>=sz)
		{
			sz=((x/1000)+1)*1000;
			cmd=(char *)realloc(cmd,sz*sizeof(char));
			if(cmd==NULL)
				error_handle("Realloc failed..");
		}
		if(i!=0)
			cmd=strcat(cmd,temp);
		else
			cmd=strcpy(cmd,temp);
		i++;
		if(ecmd_brk[i]!=NULL)
			cmd=strcat(cmd," ");
		else
			break;
	}
	umask(0000);
	fd=open(".cmd_kk_st_gr_60(history_krikash).txt",O_RDONLY | O_CREAT,0664);
	if(fd==-1)
                error_handle("Error occured..");
        ctr=lseek(fd,0,SEEK_END);
        if(ctr==-1)
                error_handle("Error occured..");
	if(ctr==0)
	{
		close(fd);
                fd=open(".cmd_kk_st_gr_60(history_krikash).txt",O_WRONLY | O_CREAT | O_TRUNC,0664);
                if(fd==-1)
                        error_handle("Error occured..");
		cmd=strcat(cmd,"\n");
		numt=strlen(cmd);
		write(fd,cmd,numt);
                close(fd);
	}
	else
	{
		if(lseek(fd,0,SEEK_SET)==-1)
			error_handle("Error occured..");
		buff=(char *)malloc((ctr+5)*sizeof(char));
                if(buff==NULL)
                        error_handle("Malloc_failed");
                read(fd,buff,ctr);
		buff[ctr]='\0';
                close(fd);
                char **prev_cmd;
                prev_cmd=(char **)malloc(25*sizeof(char *));
                i=0;
                prev_cmd[i]=strtok(buff,"\n");
                while(prev_cmd[i]!=NULL)
		{
			i++;
                        prev_cmd[i]=strtok(NULL,"\n");
		}
                i=i-1;
		if(strcmp(prev_cmd[i],cmd)==0)
			return;
		else
		{
			fd=open(".cmd_kk_st_gr_60(history_krikash).txt",O_WRONLY | O_TRUNC);
			j=0;
                        if(i==19)
                                j=1;
			char *str;
			str=(char *)malloc((strlen(prev_cmd[j])+5)*sizeof(char));
			if(str==NULL)
                                        error_handle("Malloc_failed");
			str=strcpy(str,prev_cmd[j]);
			str=strcat(str,"\n");
			j++;
                        for(j;j<=i;j++)
                        {
				str=(char *)realloc(str,(strlen(str)+strlen(prev_cmd[j])+5)*sizeof(char));
				if(str==NULL)
					error_handle("Realloc_failed");
				str=strcat(str,prev_cmd[j]);
                                str=strcat(str,"\n");
                        }
			cmd=strcat(cmd,"\n");
			str=(char *)realloc(str,(strlen(str)+strlen(cmd)+5)*sizeof(char));
                        if(str==NULL)
                        	error_handle("Realloc_failed");
			str=strcat(str,cmd);
			numt=strlen(str);
                	write(fd,str,numt);
                	close(fd);
		}
	}
	return ;
}

