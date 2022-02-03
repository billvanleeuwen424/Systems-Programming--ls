#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//for opendir and readdir
#include <dirent.h>
//file stats
#include <sys/stat.h>
//for getcwd
#include <unistd.h>
//for errno
#include <errno.h>

int checkMaxParams(int argc, char *argv[]);
void cpyDirectory(char * director, int position, char *argv[]);
int getLastFileModification(struct stat *pfileStats, char *fullPath);
int getFileSize(struct stat *pfileStats, char *fullPath);
int tryOpenDir(DIR **dir, char * dirpath);
int tryReadDir(DIR **dir, struct dirent **dirEntry);
void getFullPath(struct dirent *pdirectoryEntry, char *dirpath, char *fullPath);
int tryStat(struct stat *fileStats, char *fullPath);

#define MAX_DIR_LENGTH 256
#define MAX_PARAMS 2
#define MIN_PARAMS 1


#define MAX_BUFFER 4096


int main( int argc, char *argv[] )
{
    /*debugging*/
    argc = 2;
    argv[0] = "./main";
    //argv[1] = "/fakeDir/Fake";
    argv[1] = "/home/billslaptop/Documents/TrentU/3380";

    /*************************
     * GET DIRECTORY SECTION *
     *************************/

    char directoryName[MAX_DIR_LENGTH];

    int maxParamReturnVal = checkMaxParams(argc, argv);
    
    /*error check and get either cwd or cmdline arg */
    if(maxParamReturnVal == 1){ /*too many params*/
        exit(1);
    }

    else if(maxParamReturnVal == -1){   /*empty cmdline. getcwd*/
        getcwd(directoryName, MAX_BUFFER);
    }

    else{
        cpyDirectory(directoryName, 1, argv);   /*copy the dirname from argv*/
    }  
    

    /**************************
     * GET FILE STATS SECTION *
     **************************/

    DIR *dir;
    /*try to open the dir*/
    if(tryOpenDir(&dir, directoryName) == -1){
        exit(1);
    }

    struct dirent *dirEntry;
    char filePath[MAX_BUFFER];
    struct stat fileStat;
    struct stat *pfileStat = &fileStat;
    

    /*loop here til error or null entry*/
    while (tryReadDir(&dir, &dirEntry) == 0){

        getFullPath(dirEntry,directoryName,filePath);

        printf(" %s", dirEntry->d_name);
        
        if(tryStat(pfileStat, filePath) != 0){
            exit(1);
        }
        else {
            printf(" %lu\n", pfileStat->st_size);
            printf(" %lu\n", pfileStat->st_atime);
        }
    }

    return 0;
}


/*  pass this function a string, the position of the directory in argv, and of course argv
 *  this will check if the string is longer than the MAX_DIR_LENGTH
 */
void cpyDirectory(char * directory, int position, char *argv[]){

    if(strlen(argv[position]) > MAX_DIR_LENGTH ){
        printf("Directory length longer than %d. Quitting.\n", MAX_DIR_LENGTH);
        exit(1);
    }

    else{
        strncpy(directory, argv[position], MAX_DIR_LENGTH );
    }
}


/* Pass this function the input from the cmd line
 * Checks if more params than max
 * will exit if too many, returns 0 on successful
 *
 * return values:
 *      0: just right amount of params
 *      1: too many params
 *      -1: too few params
 *      
 * 
 * Reference: I took this from the /home/COIS/3380/sample_scripts directory.
 * I also refactored this to make it my own
 */
int checkMaxParams(int argc, char *argv[]){

    int parameter_count = 0;

    int returnval = 0;

    if( argc > MAX_PARAMS ){
        printf("Too many parameters. Exit. \n");
        returnval = 1;
    }
    else if (argc <= MIN_PARAMS)
    {
        returnval = -1;
    }

    /*DEBUGGING*/
    while( parameter_count < argc )
    {
        printf("\tparameter[%d]: %s\n",parameter_count,argv[parameter_count]);
        parameter_count++;
    }
    printf("\n");

    return returnval;
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

/*
This function just opens the directory specificed in the string passed.
it will check if the directory stream is NULL and return -1 if that is the case.
*/
int tryOpenDir(DIR **dir, char * dirpath){
    
    /*clear errno*/
    errno = 0; 

    int returnVal = 0;
    
    *dir = opendir(dirpath);

    if(*dir == NULL){

        /*reference. GNU C Library - Section 2.3 */
        fprintf (stderr, "%s: Couldn't open directory %s; %s\n", "tryOpenDir", dirpath, strerror(errno));

        returnVal = -1;
    }

    return returnVal;
}

/*
This function will read the next entry in the directory.
it will check if the directory entry is NULL and return -1 if error, or return 1 if just end of directory
*/
int tryReadDir(DIR **dir, struct dirent **dirEntry){

    errno = 0;

    int returnVal = 0;

    *dirEntry = readdir(*dir);

    if(*dirEntry == NULL){   
        if (errno != 0){    /*if there is an error*/

            returnVal = -1;        
            fprintf (stderr, "%s: Couldn't open next entry in directory; %s\n", "tryReadDir", strerror(errno));

        }
        else{               /*if we just reached the end of the directory*/
            returnVal = 1;
        }
    }                       /* else should still just be zero */
    
    return returnVal;
}


/*
This function will return a filename with its path as a single string.
It requires the files directory entry, and the path of the file.
*/
void getFullPath(struct dirent *pdirectoryEntry, char *dirpath, char *fullPath){

    strncpy(fullPath, dirpath, MAX_BUFFER );

    strncat(fullPath,"//",1);

    strncat(fullPath,pdirectoryEntry->d_name,MAX_BUFFER);

}

/*this function will try stat, upon any errors will return -1*/
int tryStat(struct stat *fileStats, char *fullPath){

    int returnVal = 0;

    errno = 0;

    stat(fullPath, fileStats);

    if(errno != 0){
        fprintf (stderr, "%s: Couldn't open stats on file %s; %s\n", "tryStat",fullPath, strerror(errno));
        returnVal = -1;
    }

    return returnVal;
}