#include "h_files.h"
void manage_history(char **ecmd_brk,int rd_end,int wr_end)
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
	if(ecmd_brk[1]!=NULL && ecmd_brk[2]!=NULL)
	{
		printf("krikash : history : Too many arguments supplied\n");
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
			error_handle("Internal Error Ocuured");
		return;
	}
	char *buff;
	int num,j,i,k,ctr,fd=open(".cmd_kk_st_gr_60(history_krikash).txt",O_RDONLY);
	if(fd==-1)
		error_handle("Error occured..");
	if(ecmd_brk[1]==NULL)
                num=9;
        else
        {
		char *numbr=(char *)malloc((strlen(ecmd_brk[1])+2)*sizeof(char));
		numbr=strcpy(numbr,ecmd_brk[1]);
                num=0;
                k=1;
                for(i=strlen(numbr)-1;i>=0;i--)
                {
                        num=num+(((int)(numbr[i]-'0'))*k);
                        k=k*10;
                }
                num=num-1;
        }
	ctr=lseek(fd,0,SEEK_END);
	if(ctr==-1)
		error_handle("Error occured..");
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
	if(prev_cmd==NULL)
		error_handle("Malloc failed");
	i=0;
	prev_cmd[i]=strtok(buff,"\n");
	while(prev_cmd[i]!=NULL)
		prev_cmd[++i]=strtok(NULL,"\n");
	i=i-1;
	j=0;
	if(i>num)
		j=i-num;
	for(j;j<=i;j++)
		printf("%s\n",prev_cmd[j]);
	if(num<20 && num>i)
		printf("\nSince %d commands was not entered till now so displaying all the commands entered till now\n",num+1);
	else if(num>i)
		printf("\nSince this shell supports only the last 20 commands so couldn't display %d commands, instead displayed all the commands stored(may be less than 20,in case you havn't entered 20 yet\n",num+1);
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
