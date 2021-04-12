# Welcome to the README file for the _krikash_ shell, Read the full file before proceeding

## How to Compile and Run the Code
* To Compile just run __make__ on the terminal
* To run the executable file just run the command __./krikash__

## What Each File Implements
### Files made in part-1 of the assignment
1. __main.c__ : It doesn't implement anything specific. Its just the starter code.
1. __interface.c__ : This contains the Code for providing the interface of the shell that is the _prompt_.
1. __check_end_bg.c__ : This file represents the code to check which of the _background_ processes have ended themselves and needs to be informed to the user about it.
1. __commands.c__ : This file is basically for taking the _command inputs_ and _parsing them_ in the root command and its arguments and then _executing_ them. In more depth this file also contains the implementation of the _pwd_ command.
1. **manage_bg.c** : This is the file for managing the _background processes/commands to be executed in the background_, basically it executes all the commands appended with an _&_.
1. **manage_cd.c** : The file contains the implementation of _cd_ command.
1. **manage_ls.c** : The code implementation of _ls_ command.
1. **manage_echo.c** : This file contains the implementation of the _echo_ command.
1. **manage_pinfo.c** : This file is the implementation of the _pinfo_ command.
1. **manage_fg.c** : This file manages the _foreground processes/commands to be executed in foreground_.
1. **add_in_history.c** : This File is the code part to enter the _command line commands entered by user_ to a file storing the last 20 commands entered in the __krikash__ shell in all sessions.
1. **manage_history.c** : This is the file implementing the _history(bonus part)_ command that is to output the last n(default value 10 and less than 20)entered by the user.
1. **error_handle.c** : This file basically handles all the _errors_ which will occur while _handling system calls_ and _other functions_. It just uses _perror_ and _exits_ the shell(forcefully).
1. **log_function.c** : This implements the log function to the base 10, i used this in ls command to tabularise the output.
1. __makefile__ : Makefile contains all the commands that need to be executed to run the _krikash_ shell.
1. **h_files.h** : This file contains all the header files that needs to be added in different c files.
1. **created_functions.h** : This file contains all the declarations of the different functions used across all the c files.
### Files made in part-2 of the assignment
#### Changes from part 1
1. **commands.c** : Previously this file also contained the implementation of pwd command but now a separate file is made for that.
1. **manage_pwd.c** : This file contains the code that is responsible for the execution of pwd command, this implementation was previously contained in commands.c file.
#### New files
1. **handle_pipe.c** : This file is to handle piping, what it does is separate the commands on the basis of the position of pipe in the given command and then pass the separated command to handle_redirect.c which is responsible for its execution.
1. **handle_redirect.c** : It basically handles the redirection. This file can either be called from commands.c or from handle_pipe.c. When called from commands.c that means i had just redirection in the command entered by the user and when called from handle_pipe.c that means the command entered was an intermix of piping and redirect.
1. **handle_setenv.c** : This file corresponds to the setenv inbuilt functionality. It basically sets the value of the environment variable specified in the arguments of setenv.
1. **handle_unsetenv.c** : This file basically handles the unsetenv command, similar to setenv.
1. **handle_jobs.c** : This file specifies the code to handle the jobs command which lists all the background processes and their current state which can be either stopped or running.
1. **handle_kjob.c** : This file is for kjob command which can be used to send signals to a background process using its reference id (assigned by krikash).
1. **handle_fag.c** : This file corresponds to fg command which can be used to bring a background process to foreground.
1. **handle_bag.c** : This file is to handle bg command which is used to change the state of a background process from stopped to running.
1. **handle_overkill.c** : This file is the implementation of overkill which is used to destroy all the processes currently in the background.


## Special Points to be Noted

* By now you guys might have judged that I have named my shell as _krikash_ :)  .
* To exit the shell enter _quit_ or press _ctrl+D_ that is _EOF_ , entering this in the krikash shell will make you to exit it with a nice message.
* _history_ command is implemented from the bonus part.(part 1 of the assignment)
* "_-_" flag is implemented for the cd command (bonus specification 1 of part 2 of the assignment)  
* _cd_ is implemented without the use _chdir_ command. (thought to add this because all my friends used chdir and they were done with it really fast)
* Colorful interface makes it a bit elegant to see.
* The case if you guys provide more than one argument to pinfo is properly handled.
* Similar is the case with history command(same as above point).
* Tried to modularize the code to maximum extent, making it easier to handle the different code snippets.
* My code tries to adapt itself to the user demands to the most when it comes to memory requirements as in more than 90% of the code dynamic memory allocation is used.
* These four signs that is __'~'  ,  '/' 
 ,  '.'  ,  '..'__ are carefully implemented for each of the shell builtin commands(wherever they are applicable).
* _ls_ command is tried to make it to resemble the ls command of the bash shell to the most considering all the edge cases.
* Fairly tested code.
* **.cmd_kk_st_gr_60(history_krikash).txt** ,this is the file used to store the commands entered by the user( used in history command ). This is a hidden file.
* signals are handled with utmost care.

## Assumptions
* There is an assumption in _ls_ command specifically in the _-l_ flag which is that when retrieving the time for a file which is when it was modified most recently using the _struct tm_.
I have assumed that it will give the time according to _gmt_ instead of _ist_.
