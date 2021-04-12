#ifndef __CREATED_FUNCTIONS
#define __CREATED_FUNCTIONS


int log_function(int num);
void handle_overkill(int pidbg[],int *bgproct,char **bgcmds[],int rd_end,int wr_end);
void handle_fag(char **ecmd_brk,int pidbg[], char **bgcmds[],int rd_end,int wr_end);
void handle_bag(char **ecmd_brk,int pidbg[],int *bgproct,int rd_end,int wr_end);
void handle_kjob(char **ecmd_brk,int pidbg[],char **bgcmds[],int rd_end,int wr_end);
void handle_jobs(char **bgcmds[],int pidbg[],int *bgproct,int rd_end,int wr_end);
void handle_unsetenv(char **ecmd_brk,int rd_end,int wr_end);
void handle_setenv(char **ecmd_brk,int rd_end,int wr_end);
void handle_redirect(char **ecmd_brk,int tot,char act_dir[],char home_dir[],int rd_end,int wr_end,char curdir[],int *curdirsz,char lastdir[],char prev_act[],int *bgproct,int pidbg[],char **bgcmds[]);
void handle_pipe(char **ecmd_brk,int ind,int tot,char act_dir[],char home_dir[],int rd_end,int wr_end,char curdir[],int *curdirsz,char lastdir[],char prev_act[],int *bgproct,int pidbg[],char **bgcmds[]);
void add_in_history(char **ecmd_brk);
void manage_history(char **ecmd_brk,int rd_end,int wr_end);
void error_handle(char str[]);
void check_end_bg(int pidbg[],int *bgproct,char **bgcmds[]);
void manage_bg(char **ecmd_brk,char act_dir[],char home_dir[],int *bgproct,int pidbg[],char **bgcmds[],int num);
void manage_pinfo(char **ecmd_brk,char home_dir[],int rd_end,int wr_end);
void manage_fg(char **ecmd_brk,char act_dir[],char home_dir[],int rd_end,int wr_end,int *bgproct,int pidbg[],char **bgcmds[],int num);
void manage_echo(char **to_op,int rd_end,int wr_end);
void manage_ls(char **flad,char act_dir[],char home_dir[],int num,int rd_end,int wr_end);
void manage_pwd(char act_curdir[],int rd_end,int wr_end);
void manage_cd(char** ecmd_brk,char act_dir[],char curdir[],int *curdirsz,char home_dir[],int rd_end,int wr_end,char lastdir[],char prev_act[]);
void commands(char act_curdir[], char curdir[], int *curdirsz, char home_dir[],int *bgproct,int *exc,int pidbg[],int *szbg,char **bgcmds[],char lastdir[],char prev_act[]);
void interface();

#endif
