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
//for time
#include <time.h>
//for conversion of gid and uid to strings
#include <pwd.h>
#include <grp.h>

int parseFlags(int flags[2], char *argv[]);
int checkMaxParams(int argc, char *argv[]);
void cpyDirectory(char * director, int position, char *argv[]);
int getLastFileModification(struct stat *pfileStats, char *fullPath);
int getFileSize(struct stat *pfileStats, char *fullPath);
int tryOpenDir(DIR **dir, char * dirpath);
int tryReadDir(DIR **dir, struct dirent **dirEntry);
void getFullPath(struct dirent *pdirectoryEntry, char *dirpath, char *fullPath);
int tryStat(struct stat *fileStats, char *fullPath);
void printLs(char *filename);
void printLsl(char *filename, struct stat *pfileStat, int printFlag, char *printString);
void printDir(char *filepath);

char *filePermissionString(mode_t perm);

#define MAX_DIR_LENGTH 256
#define MAX_PARAMS 3
#define MIN_PARAMS 1


#define MAX_BUFFER 4096
#define MAX_FILES 1000


int main( int argc, char *argv[] )
{

    /*********************************
     * GET DIRECTORY AND PARSE FLAGS *
     *********************************/

    char directoryName[MAX_DIR_LENGTH];

    int maxParamReturnVal = checkMaxParams(argc, argv);
    
    /*check if cmdline flags were passed*/
    int flags[2];
    int flagBool = 0;

    if(argc >= 2){
        flagBool = parseFlags(flags, argv);
    }


    /*too many params*/
    if(maxParamReturnVal == 1){ 
        exit(1);
    }


    /*figure out if a dir was passed or not*/
    if(flagBool == 1){

        if(argc <= 2){
            getcwd(directoryName, MAX_BUFFER);
        }
        else{
            cpyDirectory(directoryName, 2, argv);
        }
    }   
    else{
        if(argc <= 1){
            getcwd(directoryName, MAX_BUFFER);
        }
        else{
            cpyDirectory(directoryName, 1, argv);
        }
    }

    /******************
     * GET FILE STATS *
     ******************/

    DIR *dir;
    /*try to open the dir*/
    if(tryOpenDir(&dir, directoryName) == -1){
        exit(1);
    }

    struct dirent *dirEntry;
    char filePath[MAX_BUFFER];
    char filename[MAX_DIR_LENGTH];
    struct stat fileStat;
    struct stat *pfileStat = &fileStat;
    
    /*file array for recursive file printing*/
    char files[MAX_FILES][256];
    int filesCounter =0;
    char lsOutput[MAX_DIR_LENGTH];
    

    /*loop here til error or null entry*/
    while (tryReadDir(&dir, &dirEntry) == 0){

        getFullPath(dirEntry,directoryName,filePath);

        strncpy(filename, dirEntry->d_name, MAX_DIR_LENGTH);

        /*only passes if not "..int parseFlags(int *flags[2], char *argv[])" or "."*/
        if(!((filename[0] == '.' && filename[1] == '.') || (filename[0] == '.' && filename[1] == '\0'))){

            /*if error exit*/
            if(tryStat(pfileStat, filePath) != 0){
                exit(1);
            }
            else {

                /*recurse if a directory and the -R flag was set*/
                if(S_ISDIR(pfileStat->st_mode) == 1 && flags[0] == 1){

                    /*set arguments for recursive call*/
                    char *recurseArgs[2];
                    recurseArgs[0] = "./main";
                    char recursePath[MAX_DIR_LENGTH];
                    strcpy(recursePath, filePath);
                    recurseArgs[1] =  recursePath;

                    main(2, recurseArgs);
                }
                else {
                    printLsl(filename, pfileStat, 0, lsOutput);
                    strcpy(files[filesCounter],lsOutput);
                    filesCounter++;
                }

            }
        }
    }

    printDir(directoryName);
    printf("total: %d\n", filesCounter);
    
    for(int i = 0; i < filesCounter; i++){
        printf("%s", files[i]);
    }

    
    /*should return 0 on close dir*/
    return closedir(dir);
}

/*
will check if flags are present in the [1] parameter in argv. returns 1 if there are. 0 if not.
flags: an array of ints used as bools. If any are set to 1, that is a key that the user passed that flag
flag index: 
[0] -R (recursive)
[1] -l (long listing)
*/
int parseFlags(int flags[2], char *argv[]){

    if(argv[1][0] == '-'){
        for(int i = 1; i < strlen(argv[1]); i++){

            switch (argv[1][i])
            {
            case 'R':
                flags[0] = 1;
                break;

            case 'l':
                flags[1] = 1;
                break;

            default:
                break;
            }
        }

        return 1;
    }

    else{
        return 0;
    }
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

    /*DEBUGGING
    while( parameter_count < argc )
    {
        printf("\tparameter[%d]: %s\n",parameter_count,argv[parameter_count]);
        parameter_count++;
    }
    printf("\n");
    */

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
This function will load the fullpath with the passed dirent's fullpath.
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

/*this function will print the stats of a file in an ls -l format
printFlag: will printf the string if ! 0*/
void printLsl(char *filename, struct stat *pfileStat, int printFlag, char *printString){

    /* convert the gid/uid to group/passwd structs inorder to print string */
    struct group *grp;
    struct passwd *pwd;
    grp = getgrgid(pfileStat->st_gid);
    pwd = getpwuid(pfileStat->st_uid);


    /*get and format time*/
    time_t modTime =  pfileStat->st_mtime;
    struct tm  ts;
    char timeString[80];

    ts = *localtime(&modTime);

    strftime(timeString, sizeof(timeString), "%b %d %Y [%H:%M] ", &ts);
    

    /*print in ls -l format. filePermissionString to format the permissions*/
    snprintf(printString, 500, "%-4s %-6s %-6s %5lu %-18s %-7s\n", filePermissionString(pfileStat->st_mode), pwd->pw_name, grp->gr_name, pfileStat->st_size, timeString, filename);

    if (printFlag != 0){
        printf("%s\n", printString);
    }
    
}

/*this function will print the stats of a file in a regulat ls format*/
void printLs(char *filename){

    printf(" %-7s", filename);

}

/*simple formatting print function for directories
prints a new line and the filepath*/
void printDir(char *filepath){
    printf("\n%s/\n", filepath);
}


/*this function will take the mode_t from the stat type and return a formatted string of permissions
Reference:
    The Linux Programming Interface
    Chapter 15
    pg 296

and

https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
*/
char *filePermissionString(mode_t permissions){
    
    static char permissionsString[9];

    snprintf(permissionsString, 10, "%c%c%c%c%c%c%c%c%c",

    (permissions & S_IRUSR) ? 'r' : '-',
    (permissions & S_IWUSR) ? 'w' : '-', 
    (permissions & S_IXUSR) ? 'x' : '-',

    (permissions & S_IRGRP) ? 'r' : '-',
    (permissions & S_IWGRP) ? 'w' : '-', 
    (permissions & S_IXGRP) ? 'x' : '-',

    (permissions & S_IROTH) ? 'r' : '-',
    (permissions & S_IWOTH) ? 'w' : '-', 
    (permissions & S_IXOTH) ? 'x' : '-'
    );


    return permissionsString;
}