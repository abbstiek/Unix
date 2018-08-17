#!/bin/bash

declare -i dir=0 #directories
declare -i fil=0 #files
declare -i exe=0 #executables
declare -i num=1 #used to track the amt of each filetype

#prompts the user to quit
while [ 1 ]; do #runs script
  trap ctrl_c INT #cleanup function
  function ctrl_c() {
    print "\n"
    read -p "Type 'q' as a choice to exit: " quit
    if [[ $quit == "q" || $quit == "Q" ]]; then
      printf "exiting\n"
      exit 0 #exits successfully
    fi
  }

#to be printed at the top of each execution, user interface
  printf "\n-- cscomdr --\n"
  printf "$(pwd)\n"

#list all of the contents depending on files
  arr=(*)
  arr[${#arr[@]}]=".."

  #count numbers of each file type
  for file in ${arr[@]}; do
    if [ -d $file ]; then
      dir=$((dir + 1))
    elif [ -x $file ]; then
      exe=$((exe + 1))
    elif [ -f $file ]; then
      fil =$((fil + 1))
    fi
  done
  dir=$((dir - 1))

  #lists the amount of each file type
  printf "$dir directories, $fil files, $exe executables \n\n"

  #prints all of the contents of the directory
  num=1
  for i in "${!arr[@]}"; do
    if [ -d ${arr[i]} ]; then
      printf "%-25s" "$num) DIR:${arr[i]}"
    elif [ -x ${arr[i]} ]; then
      printf "%-25s" "$num) EXE: ${arr[i]}"
    elif [ -f ${arr[i]} ]; then
      printf "%-25s" "$num) ${arr[i]}"
    fi
    if (( $num % 4 == 0 )); then
      printf "\n"
    fi
    num=$((num+1))
  done
  printf "\n"

  #picks up the user's entry
  read -p "Choose any entry from the list: " entry

  #do the things + secret key
  num=1
  for file in ${arr[@]}; do
    if [[ $file == "secret" && $entry == $num ]]; then
      if [[ -e secret.key && -r secret.key ]]; then
        key=$(<secret.key) #checks if secret key has been found
        printf "$key\n" #decrypt secret and key
        openssl enc -d -a -k $key -aes-256-cbc -in secret
      else
        read -p "Please enter the key: " key
        printf "$key\n" #decrypt secret and key
        openssl enc -d -a -k $key -aes-256-cbc -in secret
      fi
    elif [[ -d $file && $entry == $num ]]; then
      if [[ ! -r $file ]]; then #checks readability of file
        printf "$file is locked\n"
      else #changes directory to file
        printf "Changing directory to $file... \n"
        cd $file
      fi #executes the file
    elif [[ -x $file && $entry == $num ]]; then
      printf "Executing $file...\n" #prints the file
      $file
    elif [[ -f $file && $entry == $num ]]; then
      if [[ ! -r $file ]]; then #checks readability of file
        printf "$file is locked\n" #print if we are unable to read file
      else #prints contents of file
        printf "Paging $file...\n"
        printf "$(less $file)\n"
      fi
    fi
    num=$((num+1))
  done

#reset all values back to 0
  dir=0
  fil=0
  exe=0

done #script done
