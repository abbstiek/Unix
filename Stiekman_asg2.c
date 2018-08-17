/* Abygail Stiekman
Unix Tools
Homework 2
03/02/2018*/

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#define MAX_ROWS 50
#define MAX_COLS 101

/*this program is an extension of The diff command line utility
on UNIX and UNIX-based platforms compares two text files line
by line and yields the difference if the files are not identical*/



/*checks if file1 is newer than file2,
checks when they were modified and removes.
This is modified from cstest.cpp*/

int isNew(const char* file1, const char* file2){

    struct stat s1 = {0};
    struct stat s2 = {0};

    stat(file1, &s1);
    stat(file2, &s2);

    if (s1.st_mtime < s2.st_mtime)
          return remove(file1);
    return remove(file2);

}

/*to be used if files are not identical*/

void usage(){

 exit(EXIT_FAILURE);

}


int main(int argc, char **argv){

  /*arrays to store file contents*/
  char f1[MAX_ROWS][MAX_COLS];
  char f2[MAX_ROWS][MAX_COLS];
  /*array to store the differences between file1 and file2*/
  char diffArray[MAX_ROWS*2+MAX_ROWS][MAX_COLS];
  /*used to iterate through the files*/
  int f1length=0;
  int f2length=0;
  char *f3 = NULL;
  int ch;
  int begin;
  int end;
  int same=1;
  int loop, del= 0;
  int i=1;
  int j, k = 0;
  FILE *file;

        while (ch!=-1) {
        i+=1;
        while ((ch = getopt(argc, argv, ":do:")) != -1) {
            switch (ch) {
              /*takes in arguements and the information*/
                case 'o':
                    f3 = argv[i];   /*grabbing output file name*/
                    break;
                case 'd':
                    del= 1;
                    break;
                default:
                  /*to be used if the user types in the wrong flag*/
                    fprintf(stderr, "option -%c not recognized\n", optopt);
                    usage();
            }
        }

        if (optind >= argc)
            break;
        ++optind;
        }

          /*opens file one to store in file 1 arr*/
          file = fopen(argv[optind-1], "r");
          if(file == NULL) /*if file is empty - exits*/
              usage();
          while(fgets(f1[f1length], MAX_COLS+1, file)!=NULL)    /*iterates through each line, stores data*/
              f1length+=1; /*increments file1's length to go through each character in the file*/

          /*close file1 to move on to the second file- file2*/
          fclose(file);

          /*opens second file and stores in file 2 arr*/
          file = fopen(argv[optind], "r");
          if(file == NULL) /*if file is empty - exits*/
          usage();
          /*stores data up to end of file*/
          while(fgets(f2[f2length], MAX_COLS+1, file)!=NULL)
          f2length+=1; /* incrememnts file2's length and goes through each character in file*/

          /*close file2 to begin working with the arrays*/
          fclose(file);

          /*determines which file has more content
          ch is used to force quit the looping*/
          if (f1length>f2length)
          ch = f1length;
          else
          ch = f2length;
          begin = end = -1;  /*stores when there has been any type of modification*/

          for(i=0;i<ch;){
          /*iterates through and checks contents of each file*/
          if (strcmp(f1[i],f2[i])!=0){
              begin = i;
              same = 0;

              /*delete*/
              if (f2[i][0]==0){
                  sprintf(diffArray[j], "%d,%dd%d\n", begin+1,ch,begin);
                  j++;
                /*for the deleted information to be stored*/
                  for(;i<ch;i++){
                      sprintf(diffArray[j], "< %s", f1[i]);
                      j++;
                  }
              }

              else if (f1[i][0]==0){
                  sprintf(diffArray[j], "%da%d,%d\n",begin, begin+1,ch);
                  j++;
                  /*for the added information to the other file*/
                  for(;i<ch;i++){
                      sprintf(diffArray[j], "> %s", f2[i]);
                      j++;
                  }
              }
              i++;

              /*checks for changes between files*/
              while(i<=ch){
                  if(strcmp(f1[i],f2[i])==0 || f1[i][0]==0 ||f2[i][0]==0 || loop == 1){

                      end = i;
                      /*distance between each line*/
                      k = end-begin+1;
                      sprintf(diffArray[j], "%d,%dc%d,%d\n", begin+1,end,begin+1,end);
                      j++;

                      /*stores changed data from both files into one array*/
                      for(i=begin;i<end;i++){
                          sprintf(diffArray[j], "< %s", f1[i]);
                          sprintf(diffArray[j+k+1], "> %s", f2[i]);
                          j++;
                      }
                      sprintf(diffArray[j], "---\n");
                      j=j+k+1;
                      i=end-1;
                      begin = end = -1;
                      break;
                  }
                  i++;

                  if (i>=ch)
                      loop = 1;
              }
          }
          i++;
       }

        if(f3){

          /*opens the file in write mode rather than read mode
          in order to print the differences to this file*/
            file = fopen(f3, "w");

            /*this will write all of the differences to a file*/
            for (i=0;i<j;i++)
                fprintf(file,diffArray[i]);

            fclose(file);   /*closes f3*/

        }

        else  /*prints (stdout)*/
          for (i=0;i<j;i++)
              printf(diffArray[i]);


        /*exit failed*/

       if (same && del && isNew(argv[optind-1],argv[optind])!=0)
                usage();

exit(EXIT_SUCCESS); /* program exits successfully*/
}
