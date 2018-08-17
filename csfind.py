#!/usr/bin/python2.7
'''Abygail Stiekman
    COP4342
    Assignment 5 - csfind.py'''

import argparse
import os
import stat
import re
import sys

'''csfind.py takes two options with arguments, -name <regex> and -grep <regex>.
The -name option matches file names in the directory against the specified regex
pattern and shows the full pathnames of the matching files foundself.
The -grep option matches file contents against the specified pattern and only
lists the files with the contents shown for each line the matches:'''


def main(): #parse command line args and options
    parser = argparse.ArgumentParser() #used for commandline parsing
    parser.add_argument("file") #filename
    parser.add_argument("-name",action="store",dest="name",nargs='?')
    parser.add_argument("-grep",action="store",dest="grep",nargs='?')
    args = parser.parse_args()
    #extract files
    getdir = os.getcwd() #path
    getdir = getdir + "/" + args.file

    if args.name is None and args.grep is None:
        start_path = '.' # current directory
        for path,dirs,files in os.walk(getdir):
            for filename in files:
                print os.path.join(path,filename) #prints path
    elif args.grep is None: #displays the pathname of the file only
        start_path = '.' # current directory
        for path,dirs,files in os.walk(getdir):
            for filename in files:
                if re.match(args.name,filename):
                    print os.path.join(path,filename) #prints contents
    elif args.name is None:
        start_path = '.' # current directory
        for path,dirs,files in os.walk(getdir):
            for filename in files: #open the file for reading
                for i, line in enumerate(open(os.path.join(path,filename))):
                    count = 0
                    for match in re.finditer(args.grep, line):
                        if count is 0:
                            sys.stdout.write('%s %s: %s' % (os.path.join(path,filename), i+1, line))
                        count += 1
    else:
        start_path = '.' # current directory
        for path,dirs,files in os.walk(getdir):
            for filename in files:
                if re.match(args.name,filename): #opens file for reading
                    for i, line in enumerate(open(os.path.join(path,filename))): #loop and increment
                        count = 0
                        for match in re.finditer(args.grep, line):
                            if count is 0: #if counter does not incriment
                                sys.stdout.write('%s %s: %s' % (os.path.join(path,filename), i+1, line))
                            count += 1

#Runs function main() with program logic
if __name__ == "__main__":
    main()
