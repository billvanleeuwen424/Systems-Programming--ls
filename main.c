/* 
Objectives of this program:
    find the largest file
    find the smallest file
    the most recently modified file
    the oldest file
*/
#include <stdio.h>
#include <string.h>


//for opendir and readdir
#include <dirent.h>
//file stats
#include <sys/stat.h>
//for getcwd
#include <unistd.h>

//command line params
//argc = num of params
//char *argv[] is the array of params
//int argc, char *argv[]

#define MAX_BUFFER 4096

int main(){



    //directory path
    const char *dirpath = "/home/billDesktop";

    //get the current dir
    char *cwd = getcwd(cwd, MAX_BUFFER);

    //pointer to a dirent structure
    struct dirent *directoryEntry;

    //pointer to the directory type
    DIR *dir = opendir(dirpath);

    //file stats
    //struct stat fileStats;
    //struct stat *pfileStats = &fileStats;
    //reference: https://stackoverflow.com/questions/3554120/open-directory-using-c
    while ((directoryEntry = readdir(dir)) != NULL){
        //print directory
        printf("%s\n", directoryEntry->d_name);

        //get stats, print stats
        //stat(dirpath, pfileStats);
        //printf("%lu", pfileStats);

    }

    

    
    //close the directory stream
    return closedir(dir);

}