#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//for opendir and readdir
#include <dirent.h>
//file stats
#include <sys/stat.h>
//for getcwd
#include <unistd.h>

int checkMaxParams(int argc, char *argv[]);
void cpyDirectory(char * director, int position, char *argv[]);
int getLastFileModification(struct stat *pfileStats, char *fullPath);
int getFileSize(struct stat *pfileStats, char *fullPath);

#define MAX_DIR_LENGTH 256
#define MAX_PARAMS 2
#define MIN_PARAMS 1


#define MAX_BUFFER 4096


int main( int argc, char *argv[] )
{

    /*************************
     * GET DIRECTORY SECTION *
     *************************/

    char directoryName[MAX_DIR_LENGTH];

    int maxParamReturnVal = checkMaxParams(argc, argv);
    
    if(maxParamReturnVal == 1){ /*too many params*/
        exit(1);
    }

    else if(maxParamReturnVal == -1){   /*empty cmdline. getcwd*/
        getcwd(directoryName, MAX_BUFFER);
    }

    else{
        cpyDirectory(directoryName, 1, argv);   /*copy the dirname from argv*/
    }

    printf(" %s", directoryName);
    
    

    /**************************
     * GET FILE STATS SECTION *
     **************************/

    

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
        printf("Too many parameters. Quitting. \n");
        returnval = 1;
    }
    else if (argc < MIN_PARAMS)
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

int openDirectory(char * dirpath){
    
}