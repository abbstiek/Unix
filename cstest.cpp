/* Abygail Stiekman
Unix Tools
Homework 1
02/16/2018*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//checks if the file is a regular directory
//returns true if so, false if not true

int isDir(const char* file){
  struct stat fileStat = {0};
  stat(file, &fileStat);
  return S_ISREG(fileStat.st_mode);
}

//checks if the file is a regular file
//returns true if so, false if not true

int isReg(const char* file){
  struct stat fileStat = {0};
  stat(file, &fileStat);
  return S_ISREG(fileStat.st_mode);
}

//checks if the file exists
//returns true if so, false if not true

int isFile(const char* file){
if (access(file, F_OK) != -1)
  return 1;
//else
return 0;
}

//checks if the file is readable
//returns true if so, false if not true

int isRead(const char* file){
  if (access(file, R_OK) != -1)
                return 1;
      //else
return 0;
}

//checks if the file is writable
//returns true if so, false if not true

int isWrite(const char* file){
  if (access(file, W_OK) != -1)
                return 1;
//else
return 0;
}

//checks if the file is executable,
//returns true if so, false if not true

int isExecute(const char* file){
  if (access(file, X_OK) != -1)
                return 1;
//else
return 0;
}


//checks if the argument =0 or contains an empty string
//returns true if so, false if not true

int isEmpty(const char* file){
  if(strcmp(file, "0") == 0)
  return 1;
else if(strcmp(file, "") == 0)
  return 1;
else
  return 0;
}

//checks if file1 is newer than file2
//returns newer file, then older

int isNew(const char* file1, const char* file2){
  struct stat s1 = {0};
  stat(file1, &s1);
  struct stat s2 = {0};
  stat(file2, &s2);
  return s1.st_mtime > s2.st_mtime;
}


//tests if the exit has passed or failed, flags this response to later be
//ran in main

void test(int argresult, int* fail, int* Nflag){
  if(argresult && *Nflag == 1){
    *fail = 1;
    *Nflag = 0;
  }

//if there is no result, then the default is to fail
  else if(!argresult){
    *fail = 1;
    if(*Nflag == 1){
      *fail = 0;
      *Nflag = 0;
    }
  }
}


//prints exit failed statement error and exits

void usage()
{
 fprintf(stderr, "Usage: cstest EXPRESSION\n");
 exit(EXIT_FAILURE);
}



int main(int argc, char **argv)
{
         int Nflag, ch, fail;
         int  optreset=0;
         Nflag = 0;
         fail = 0;

         while (1) {//goes through switch statement depending on the flag submitted by the user and the file that they are testing
                 while ((ch = getopt(argc, argv, "+Nf:d:e:r:w:x:n:z:")) != -1) {
                   //switch statement to test file attributes and either fail or succeed
                         switch (ch) {
                                case 'N':       //Nflag test
                                        Nflag = 1;
                                        break;
                                case 'f':
                                        //regular file test
                                        test(isReg(optarg), &fail, &Nflag);
                                        break;
                                case 'd':
                                        //directory test
                                        test(isDir(optarg), &fail, &Nflag);
                                        break;
                                case 'e':
                                        //file exists test
                                        test(isFile(optarg), &fail, &Nflag);
                                        break;
                                case 'r':
                                        //file has read permissions test
                                        test(isRead(optarg), &fail, &Nflag);
                                        break;
                                case 'w':
                                        //file has write permissions test
                                        test(isWrite(optarg), &fail, &Nflag);
                                        break;
                                case 'x':
                                        //file has execute permissions test
                                        test(isExecute(optarg), &fail, &Nflag);
                                        break;
                                case 'z':
                                        //empty file test
                                        test(isEmpty(optarg), &fail, &Nflag);
                                        break;
                                case 'n':
                                        //file1 vs file2 modification test
                                        if(optind >= 4){
                                          if(!Nflag)
                                            test(isNew(argv[optind-3], argv[optind-1]), &fail, &Nflag);
                                          else if(Nflag)
                                            test(isNew(argv[optind-4], argv[optind-1]), &fail, &Nflag);
                                        }
                                        else if(optind < 4){
                                              fprintf(stderr, "Usage: cstest ARGUMENT -n ARGUMENT\n");
                                              exit(EXIT_FAILURE);
                                        }

                                        break;
                                case ':':
                                        //cannot be empty, needs to have a flag
                                        fprintf(stderr, "option -%c requires an argument", optopt);
                                        usage();
                                        break;
                                case '?':
                                        //just exits if the character is unknown
                                          break;
                                default:
                                        //to be used if a character entered is not part of our flags
                                        fprintf(stderr, "unrecognized option -%c\n", optopt);
                                        usage();
                }

                //if there is one failure, the entire thing fails
                if(fail){
                  printf("exit failure\n");
                  exit(EXIT_FAILURE);
                }
              }

              //exits loop if there are no more arguments
              if (optind >= argc){
                break;
              }

              // handle argument here
              ++optind;
              }


           printf("exit success\n");
           exit(EXIT_SUCCESS);
           return 0;
}
