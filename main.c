/* 
Objectives of this program:
    find the largest file
    find the smallest file
    the most recently modified file
    the oldest file
*/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//for opendir and readdir
#include <dirent.h>
//file stats
#include <sys/stat.h>
//for getcwd
#include <unistd.h>




char * getFilepathString(struct dirent *directoryEntry, char *dirpath);


//command line params
//argc = num of params
//char *argv[] is the array of params
//int argc, char *argv[]

#define MAX_BUFFER 4096

int main(){

    //get the current dir
    char *cwd = getcwd(cwd, MAX_BUFFER);

    //directory path
    char *dirpath = "/home/billDesktop/Documents/TrentU/3380/";


    //pointer to a dirent structure
    struct dirent *directoryEntry;

    //pointer to the directory type
    DIR *dir = opendir(dirpath);

    //file stats
    struct stat fileStats;
    struct stat *pfileStats = &fileStats;


    //reference: https://stackoverflow.com/questions/3554120/open-directory-using-c
    while ((directoryEntry = readdir(dir)) != NULL){


        char *fullPath = getFilepathString(directoryEntry, dirpath);

        printf("%s\n", fullPath);
        /*
        //get stats, print stats
        stat(filepath, &fileStats);
        printf("%i\n", pfileStats->st_atime);
        */
    }
    
    //close the directory stream
    return closedir(dir);

}



/*
This function will return a filename with its path as a single string.
It requires the files directory entry, and the path of the file.
*/
char * getFilepathString(struct dirent *directoryEntry, char *dirpath){
    //get filename from the dirent
    const char *filename = directoryEntry->d_name;

    char *filepath;

    //put the directory into the filepath, then concat the filename onto it.
    strcpy(filepath,dirpath);
    strcat(filepath,filename);

    return filepath;
}