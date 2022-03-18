NAME         
       smash

SYNOPSIS         
       smash: (OPTIONAL) file 

DESCRIPTION      

       This funtion will open up a shell interface for the user and allow them to enter commands and have them be processed. If a file argument is provided at launch, it will process all commands found in that file.

       This function provides special implementations for the following commmands:

              Built-In Commands:

                     exit: Allows the user to exit the shell.

                     (Not Implemented) cd [directory path] : Changes the current working directory for the shell. Returns an error if path can not be found.

                     (Not implemented) path [arg]:

                            arg list: 
                                   add [path]: Takes a path arg and appends it to the beginning of the path list. For example, path add /usr/bin results in the path list containing /usr/bin and /bin 

                                   remove [path]: Searches through the current path list and removes the corresponding one. If the path cannot be found, returns an error

                                   clear: Removes everything from the path list. The shell will not be able to run any programs (except built-in commands).

              path Commands:

                     Any executable found in the path of the shell, that has the name of a command inputed into smash, will be executed in the format provided by the command input. For example, if an executable existed in bin with the name helloWorld, and a command such as [helloWorld] were entered into the smash, with would run the executable found in the bin. These can also be directly modified via the paths file, which contains all path information. 

              Other Commmands:

                     Any other command passed is processed by the function as a standard linux commmand. For example, while ls is not hard coded, it will still be able to be ran as it is a standard linux function. Please refer to individual MAN pages for the commands you would like to use with this shell. 

              Notes:

                     There is a built-in order of operations for the shell. The order is as follows:
                     
                     Built-in Commands > path Commands > Other Commands

                     For example, if there were an executable with the name exit in the path, it would never be ran as the built-in command will always take precedence over it. 


       
       SPECIAL FUNCTIONS: 
              &: 
                     SYNOPSIS: cmd args & cmd args
                     DESCRIPTION: Allows for parallel proccessing of all commands connected by &'s
              ;:
                     SYNOPSIS: cmd args ; cmd args
                     DESCRIPTION: Allows for multiple commands for all commands seperated by ;'s
              >:
                     SYNOPSIS: cmd args > file 
                     DESCRIPTION: Will output the command to the file the > is pointing at. Only does this for one command. All files will be put into the outputFiles folder
                            (EX. [pwd ; ls -l > out.txt] will only put ls -l in out.txt. [pwd > out.txt ; ls -l > out.txt] will put both in out.txt )
