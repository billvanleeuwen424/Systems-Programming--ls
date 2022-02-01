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


/*for opendir and readdir*/
#include <dirent.h>
/*file stats*/
#include <sys/stat.h>
/*for getcwd*/
#include <unistd.h>




char * getFilepathString(struct dirent *pdirectoryEntry, char *dirpath);
int getLastFileModification(struct stat *pfileStats, char *fullPath);
int getFileSize(struct stat *pfileStats, char *fullPath);
void printFileStats(struct dirent *pdirectoryEntry, struct stat *pfileStats, char *dirpath);

/*command line params*/
/*argc = num of params*/
/*char *argv[] is the array of params*/
/*int argc, char *argv[]*/

#define MAX_BUFFER 4096

int main(){


    char path[MAX_BUFFER];

    getcwd(path, MAX_BUFFER);
    printf("Current working directory: %s\n\n", path);
    
    /*directory path*/
    char *dirpath = path;

    /*pointer to the directory*/
    DIR *dir = opendir(dirpath);



    /*directory entry*/
    struct dirent directoryEntry;
    struct dirent *pdirectoryEntry = &directoryEntry;

    /*file stats*/
    struct stat fileStats;
    struct stat *pfileStats = &fileStats;



    /*objective files storage*/
    char * largestFilepath;
    struct dirent largestDirent;
    struct dirent *plargestDirent = &largestDirent;
    struct stat largestStats;
    struct stat *plargestStats = &largestStats;

    char * smallestFilepath;
    struct dirent smallestDirent;
    struct dirent *psmallestDirent = &smallestDirent;
    struct stat smallestStats;
    struct stat *psmallestStats = &smallestStats;

    char * mostRecentFilepath;
    struct dirent mostRecentDirent;
    struct dirent *pmostRecentDirent = &mostRecentDirent;
    struct stat mostRecentStats;
    struct stat *pmostRecentStats = &mostRecentStats;

    char * leastRecentFilepath;
    struct dirent leastRecentDirent;
    struct dirent *pleastRecentDirent = &leastRecentDirent;
    struct stat leastRecentStats;
    struct stat *pleastRecentStats = &leastRecentStats;


    /*reference: https://stackoverflow.com/questions/3554120/open-directory-using-c*/
    while ((pdirectoryEntry = readdir(dir)) != NULL){


        char *fullPath = getFilepathString(pdirectoryEntry, dirpath);

        int lastModification = getLastFileModification(pfileStats, fullPath);

        int fileSize = getFileSize(pfileStats, fullPath);
        

        /*check largest*/
        if(plargestStats == NULL || pfileStats->st_size > plargestStats->st_size){
            largestFilepath = fullPath;
            largestDirent = directoryEntry;
            largestStats = fileStats;
        }
        /*check smallest*/
        if(psmallestStats == NULL || pfileStats->st_size < psmallestStats->st_size){
            smallestFilepath = fullPath;
            smallestDirent = directoryEntry;
            smallestStats = fileStats;
        }
        /*check mostRecent*/
        if(pmostRecentStats == NULL || pfileStats->st_atime > pmostRecentStats->st_atime){
            mostRecentFilepath = fullPath;
            mostRecentDirent = directoryEntry;
            mostRecentStats = fileStats;
        }
        /*check leastRecent*/
        if(pleastRecentStats == NULL || pfileStats->st_atime < pleastRecentStats->st_atime){
            leastRecentFilepath = fullPath;
            leastRecentDirent = directoryEntry;
            leastRecentStats = fileStats;
        }
    }

    printFileStats(plargestDirent,plargestStats,dirpath);
    printFileStats(psmallestDirent,psmallestStats,dirpath);
    printFileStats(pmostRecentDirent,pmostRecentStats,dirpath);
    printFileStats(pleastRecentDirent,pleastRecentStats,dirpath);



    /*close the directory stream*/
    return closedir(dir);

}



/*
This function will return a filename with its path as a single string.
It requires the files directory entry, and the path of the file.
*/
char * getFilepathString(struct dirent *pdirectoryEntry, char *dirpath){
    /*get filename from the dirent*/
    const char *filename = pdirectoryEntry->d_name;

    char *filepath;

    /*put the directory into the filepath, then concat the filename onto it.*/
    strcpy(filepath,dirpath);
    strcat(filepath,filename);

    return filepath;
}

/*
This function will return the last modification time in epoch time.
it requires a pointer to a stat type, and the filepath of the deisred file.
*/
int getLastFileModification(struct stat *pfileStats, char *fullPath){

    stat(fullPath, pfileStats);

    return pfileStats->st_atime;
}

/*
This function will return the size of the file (bytes).
it requires a pointer to a stat type, and the filepath of the deisred file.
*/
int getFileSize(struct stat *pfileStats, char *fullPath){

    stat(fullPath, pfileStats);

    return pfileStats->st_size;
}

void printFileStats(struct dirent *pdirectoryEntry, struct stat *pfileStats, char *fullPath){

    printf(" %s\n", pdirectoryEntry->d_name);

    printf(" %i\n", getFileSize(pfileStats, fullPath));
    printf(" %i\n", getLastFileModification(pfileStats, fullPath));
}