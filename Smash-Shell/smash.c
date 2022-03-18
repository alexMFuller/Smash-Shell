#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>

#define WHITESPACE ";\n"



int toArgv(char* source, char** dest[], int size){
	int numTok = 0;
	//dest = malloc(size*sizeof(char*));
	char **wordArray = malloc(size*sizeof(char*));
	
	char* token = strtok(source, " ");

   /* walk through other tokens */
    while( numTok!=size ) {
		wordArray[numTok] = malloc(100*sizeof(char));
     	wordArray[numTok]= token;
		//printf( "%s\n", token );
        token = strtok(NULL, " ");
		numTok++;
   }

	*dest = wordArray;
	
	dest[size]=NULL;
	//printf("%s\n",dest[size]);

	return numTok;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


int main(int argsc, char **argsv)
{
    
    int done = 0;
    

    if (argsc==2){
        //FILE PROCESSING


        char * buffer = 0;
        long length;
        FILE * f = fopen (argsv[1], "rb");
        if (errno!=0){
            perror("BAD INPUT FILE");
        }

        size_t t;

        if (f)
        {
          fseek (f, 0, SEEK_END);
          length = ftell (f);
          fseek (f, 0, SEEK_SET);
          buffer = malloc (length+1);
          if (buffer)
          {
            t = fread (buffer, 1, length, f);
          }
          fclose (f);
        }

        

        buffer[length] = '\0';

           

        char *command;
        char *commands[100];
            
            

        //char *s = strdup(buffer);

            
        //printf("%s\n",buffer);

            int i = 0;

        while ((command = strsep(&buffer, WHITESPACE)) != NULL)
            {
                if (command == NULL || strstr(command, "\0") == 0)
                {
                    // printf("Here\n");
                    break;
                }
                commands[i++] = command;
                
            }
        

            for (int in = 0; in < i-1; in++)
            {   
                //printf("%d\n",in);

                if (strstr("exit", commands[in])!= NULL)
                {
                    exit(0);
                }

                else if (strstr("cd",  commands[in]) != NULL )
                {
                    //Uses CHRDIR() to change the active directory of the user

                }
                else if (strstr("path",  commands[in]) != NULL )
                {
                    //add
                        //adds given path to the path file if path is valid
                    //remove
                        //removes given path from path file if path is valid
                    //clear
                        //removes all contents from path file



                }


                else {

                    

                    int numTok = 0;
	                //dest = malloc(size*sizeof(char*));
	                char **wordArray = malloc(100*sizeof(char*));
                    char* token = strtok(commands[in], "&");
                    
                    
                    /* walk through other tokens */

                    while( token!=NULL ) {
		                wordArray[numTok] = malloc(100*sizeof(char));
     	                wordArray[numTok]= token;
                        token = strtok(NULL, "&");
	                    numTok++;
                    }



                    
                    
                    for (int j = 0; j < numTok; j++){

                        int redirect = 0;

                        char** tempArray;
                        char* fileName = malloc(100*sizeof(char));
                        char* tempString1;
                        char* tempString2;
                        


                        if (strstr(wordArray[j], ">" )!=NULL){
                            
                            tempString1 = strtok(wordArray[j],">");
                            tempString2 = strtok(NULL,">");

                            tempString2 = trimwhitespace(tempString2);

                            redirect = 1;
                            
                            toArgv(tempString1, &tempArray, 10);
                            
                            strcpy(fileName , tempString2);

                        }

                        else{
                            toArgv(wordArray[j], &tempArray, 10);
                        }
		                
		                pid_t id = fork();

                        if(id==-1){ 
			                printf("fork failed");
			                
		                }

		                else if (id==0){

                            
                            
                            if(redirect == 1){

                                
                                //Found help for redirection at this link https://www.unix.com/programming/268879-c-unix-how-redirect-stdout-file-c-code.html
                                char outf[100] = "outputFiles/";
                                strcat( outf , fileName);

                                // Open file to save standard out to
                                    int out = open( outf , O_RDWR | O_CREAT | O_APPEND, 0600);
                                    if (-1 == out)
                                    {
                                      perror( ("opening %s", fileName) );
                                      
                                    }

                                    // Open file to save standard err to
                                   

                                    // Save the file descriptors of standard out and standard error
                                    int save_out = dup(fileno(stdout));
                                    int save_err = dup(fileno(stderr));


                                    // Duplicate out to standard out (replace stdout with out)
                                    if (-1 == dup2(out, fileno(stdout)))
                                    {
                                      perror("cannot redirect stdout");
                                      
                                    }
                                    // Duplicate err to standard err (replace stderr with err)
                                    if (-1 == dup2(out, fileno(stderr)))
                                    {
                                      perror("cannot redirect stderr");
                                      
                                    }

                                    
                                    execvp(tempArray[0], tempArray);
                                    if (errno!=0){
                                        char error_message[30] = "An error has occurred\n"; 
                                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                                    }


                                    // Flush out all data to files before closing, then close them
                                    fflush(stdout);
                                    fflush(stderr);
                                    close(out);
                                    

                                    // Replace stdout and stderr with saved file descriptors
                                    dup2(save_out, fileno(stdout));
                                    dup2(save_err, fileno(stderr));

                                    // Close the file descriptors
                                    close(save_out);
                                    close(save_err);   
                                    
                            }

                            else{

                                execvp(tempArray[0], tempArray);
                                if (errno!=0){
                                    char error_message[30] = "An error has occurred\n"; 
                                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                                }
                                
                            }
			                
		                }

		                else{
			                wait(NULL);
                            
		                }
                    }
                        
		            

                }

        }
    }

    else if (argsc>2){

        //ERROR FOR MORE THEN ONE FILE
        char error_message[30] = "An error has occurred\n"; 
        write(STDERR_FILENO, error_message, strlen(error_message)); 

    }

    else{

        //SHELL PROCESSING
        while (done == 0){

            //printf("%d\n", done);
            char *line_buf = NULL;
            size_t line_buf_size = 0;
            int line_count = 0;

            // print the prompt, then get a line of input from the user

            printf("smash> ");

            int line_size = getline(&line_buf, &line_buf_size, stdin);

            

           

            char *command;
            char *commands[100];
            
            

            char *s = strdup(line_buf);

            //printf("here\n");
            // printf("%s\n",s);
            int i = 0;

            while ((command = strsep(&s, WHITESPACE)) != NULL)
            {
                if (command == NULL || strstr(command, "\0") == 0)
                {
                    // printf("Here\n");
                    break;
                }
                commands[i++] = command;
                
            }

            for (int in = 0; in < i-1; in++)
            {   
                

                if (strstr("exit", commands[in])!= NULL)
                {
                    exit(0);
                }

                else if (strstr("cd ",  commands[in]) != NULL )
                {

                    //Uses CHRDIR() to change the active directory of the user
                    
                    char** cdList;

                    printf("Here\n");

                    toArgv(commands[in], &cdList , 10 );

                    printf("%s\n",cdList[1]);

                    int x = chdir(cdList[1]);

                    if (x = -1){
                        perror("Bad File path for CD");
                    }


                }
                else if (strstr("path",  commands[in]) != NULL )
                {
                    
                    //add
                        //adds given path to the path file if path is valid
                    //remove
                        //removes given path from path file if path is valid
                    //clear
                        //removes all contents from path file

                }

                

                else {

                    

                    int numTok = 0;
	                //dest = malloc(size*sizeof(char*));
	                char **wordArray = malloc(100*sizeof(char*));
                    char* token = strtok(commands[in], "&");
                    
                    
                    /* walk through other tokens */

                    while( token!=NULL ) {
		                wordArray[numTok] = malloc(100*sizeof(char));
     	                wordArray[numTok]= token;
                        token = strtok(NULL, "&");
	                    numTok++;
                    }



                    
                    
                    for (int j = 0; j < numTok; j++){

                        int redirect = 0;

                        char** tempArray;
                        char* fileName = malloc(100*sizeof(char));
                        char* tempString1;
                        char* tempString2;
                        


                        if (strstr(wordArray[j], ">" )!=NULL){
                            
                            tempString1 = strtok(wordArray[j],">");
                            tempString2 = strtok(NULL,">");

                            tempString2 = trimwhitespace(tempString2);

                            redirect = 1;
                            
                            toArgv(tempString1, &tempArray, 10);
                            
                            strcpy(fileName , tempString2);

                        }

                        else{
                            toArgv(wordArray[j], &tempArray, 10);
                        }
		                
		                pid_t id = fork();

                        if(id==-1){ 
			                printf("fork failed");
			                
		                }

		                else if (id==0){

                            
                            
                            if(redirect == 1){

                                
                                //Found help for redirection at this link https://www.unix.com/programming/268879-c-unix-how-redirect-stdout-file-c-code.html
                                char outf[100] = "outputFiles/";
                                strcat( outf , fileName);


                                // Open file to save standard out to
                                    int out = open(outf, O_RDWR | O_CREAT | O_APPEND, 0600);
                                    if (-1 == out)
                                    {
                                      perror( ("opening %s", fileName) );
                                      
                                    }

                                    // Open file to save standard err to
                                   

                                    // Save the file descriptors of standard out and standard error
                                    int save_out = dup(fileno(stdout));
                                    int save_err = dup(fileno(stderr));


                                    // Duplicate out to standard out (replace stdout with out)
                                    if (-1 == dup2(out, fileno(stdout)))
                                    {
                                      perror("cannot redirect stdout");
                                      
                                    }
                                    // Duplicate err to standard err (replace stderr with err)
                                    if (-1 == dup2(out, fileno(stderr)))
                                    {
                                      perror("cannot redirect stderr");
                                      
                                    }

                                    
                                    execvp(tempArray[0], tempArray);
                                    if (errno!=0){
                                        char error_message[30] = "An error has occurred\n"; 
                                        write(STDERR_FILENO, error_message, strlen(error_message)); 
                                    }


                                    // Flush out all data to files before closing, then close them
                                    fflush(stdout);
                                    fflush(stderr);
                                    close(out);
                                    

                                    // Replace stdout and stderr with saved file descriptors
                                    dup2(save_out, fileno(stdout));
                                    dup2(save_err, fileno(stderr));

                                    // Close the file descriptors
                                    close(save_out);
                                    close(save_err);   
                                    
                            }

                            else{

                                execvp(tempArray[0], tempArray);
                                if (errno!=0){
                                    char error_message[30] = "An error has occurred\n"; 
                                    write(STDERR_FILENO, error_message, strlen(error_message)); 
                                }
                                
                            }
			                
		                }

		                else{
			                wait(NULL);
                            
		                }
                    }
                        
		            

                }




            }

            
        } 

    } 

    return 0;
}