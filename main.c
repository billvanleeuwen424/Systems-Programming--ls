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




char * getFilepathString(struct dirent *pdirectoryEntry, char *dirpath);
int getLastFileAccess(struct stat *pfileStats, char *fullPath);
int getFileSize(struct stat *pfileStats, char *fullPath);


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


    //pointer to the directory
    DIR *dir = opendir(dirpath);
    
        
    //directory entry
    struct dirent directoryEntry;
    struct dirent *pdirectoryEntry = &directoryEntry;

    //file stats
    struct stat fileStats;
    struct stat *pfileStats = &fileStats;

    //reference: https://stackoverflow.com/questions/3554120/open-directory-using-c
    while ((pdirectoryEntry = readdir(dir)) != NULL){


        char *fullPath = getFilepathString(pdirectoryEntry, dirpath);
        printf("%s\n", fullPath);
        
        int lastAccess = getLastFileAccess(pfileStats, fullPath);
        printf("%i\n", lastAccess);

        int fileSize = getFileSize(pfileStats, fullPath);
        printf("%i\n", fileSize);
        
    }
    
    //close the directory stream
    return closedir(dir);

}



/*
This function will return a filename with its path as a single string.
It requires the files directory entry, and the path of the file.
*/
char * getFilepathString(struct dirent *pdirectoryEntry, char *dirpath){
    //get filename from the dirent
    const char *filename = pdirectoryEntry->d_name;

    char *filepath;

    //put the directory into the filepath, then concat the filename onto it.
    strcpy(filepath,dirpath);
    strcat(filepath,filename);

    return filepath;
}

/*
This function will return the last access time in epoch time.
it requires a pointer to a stat type, and the filepath of the deisred file.
*/
int getLastFileAccess(struct stat *pfileStats, char *fullPath){

    stat(fullPath, pfileStats);

    return pfileStats->st_atime;
}


int getFileSize(struct stat *pfileStats, char *fullPath){

    stat(fullPath, pfileStats);

    return pfileStats->st_size;
}