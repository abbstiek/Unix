/* Abygail Stiekman
Unix - Homework 3
03/20/2018

test with "gcc -Wall -ansi -pedantic cssh.c" */

/*Assignment Description:

In this programming assignment, I implemented a customized simplified version of a Unix Shell
called “cssh”. There are no built-in programming structures or variables in cssh. The cssh
shell program is able to:
• Execute programs that are in the current directory or on the $PATH environment variable
and that are compiled executables (object files) with permission ‘x’ set, and handle IO
redirects for programs.
• Pass arguments that are separated by spaces and string arguments quoted with single
’-quotes, such as ’this is a single argument’, to commands.
• Exit on the ``exit’’ command.
• Change directory with the ``cd’’ command.*/


#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


/*shifts the string of the 2D arr of pointers and removes char*/

int rem_shift(char **args, int i, int size){
   for(;i<size;i++){
       args[i]=args[i+1];
   }
   return size-1;
}

/*shifts the string of character arrays and removes char*/

int shift_str(char *args, int i, int size){
   for(;i<size;i++){
       args[i]=args[i+1];
   }
   return size-1;
}

/*to be used if the shell needs to quit unsuccessfully*/

void failure(){
   exit(EXIT_FAILURE);
}

/*executes and runs all given commands with arguments and redirection with > & <*/

int exec(char **args, int argc){
    int file_in, file_out;
    int i;
    int status;
    pid_t pid;

     /*uses tty for write and read functionality*/

    int tty = open("/dev/tty", O_RDWR);
    file_in = 0;
    file_out = 0;

    /*checks for redirection*/

    for(i=0;i<argc;i++){
        if (i < argc && strcmp(args[i],"<")==0 && file_in==0){
             /*removes <*/
            argc = rem_shift(args,i,argc);
            /*input redirect*/
            file_in = open(args[i],O_RDONLY);
            /*remove input file from args*/
            argc = rem_shift(args,i,argc);
        }
        if (i < argc && strcmp(args[i],">")==0 && file_out==0){

            /*remove > from arguments*/

          argc = rem_shift(args,i,argc);

          /*Create input redirect file descriptor*/

          file_out = open(args[i],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            /*removes output file from arguments*/

          argc = rem_shift(args,i,argc);
        }
    }

    /*forks*/

    pid = fork();
    if (pid == 0) {

       /*check for redirection in*/

        if (file_in!=0){
            dup2(file_in, 0);
            close(file_in);
        }

        else{
            dup2(tty, 0);
        }

        /*checks for redirection out*/

        if (file_out!=0){
            dup2(file_out, 1);
            close(file_out);
        }

        else{
            dup2(tty, 1);
        }

        dup2(tty,2);
        close(tty);

        /*error message, if the user does not use an appropriate command*/

        if (execvp(args[0], args) == -1) {
            printf("%s: Command not found.\n",args[0]);
            exit(EXIT_FAILURE);
        }
        failure();
      }

      /*if there is an error forking*/

    else if (pid < 0) {
        failure();
      }

    else {
        do {
          /*this is for the child process*/
          waitpid(pid, &status, WUNTRACED);
          /*check status of process*/
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
      }
      /*exit*/
      return 1;
}

/*name of shell*/

static const char prompt[] = "cssh> ";
static const char sep[] = "\n";

int main(int argc, char **argv)
{
    int argcount;
    char *argval[10];

    while (1)
    {
        char *arg;
        char *line;
        char curr_dir[1024];
        int i=0;
        int len;


        /*get the current directory*/

        if (getcwd(curr_dir, sizeof(curr_dir)) != NULL)
            fprintf(stdout, "%s ", curr_dir);

        /*readline after prompt*/

        line = readline(prompt);
        if (line == NULL)
            break;

      /*replace tabs and white space with \n*/

        len = strlen(line);
        for(;i<len;i++)
        {
          /*checks for tab,replaces with \n*/
            if (line[i]=='  ')
                line[i]='\n';
            else if (line[i]=='\''){
                len=shift_str(line,i,len);
                /*searches for end quotation mark*/
                for(;i<len;i++){
                    if(line[i]=='\''){
                        /*removes end quotation mark*/
                        len=shift_str(line,i,len);
                        /*all quotes have been found*/
                        break;
                    }
                }
            }
        }

        /*tokenize the line into argval[]*/

        argcount = 0;
        for (arg = strtok(line, sep); arg && argcount < 10; arg = strtok(NULL, sep))
            argval[argcount++] = arg;

        /*pointer array has to end with NULL*/

        argval[argcount]=NULL;

        /*tests exit function*/

        if (argcount > 0 && strcmp(argval[0], "exit") == 0){
            break;
        }

        /*checks if directory has changed*/

        else if (argcount > 0 && strcmp(argval[0], "cd") == 0){
            chdir(argval[1]);
        }

        /*tests execute cmmd*/

        else exec(argval,argcount);

       /*line is no longer used*/

        free(line);

    }

    /*successful exit*/

    exit(EXIT_SUCCESS);

}
