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


//command line params
//argc = num of params
//char *argv[] is the array of params
//int argc, char *argv[]



int main(){

    //directory path
    const char *dirpath = "/home/billDesktop";

    //pointer to a dirent structure
    struct dirent *directoryEntry;

    //pointer to the directory type
    DIR *dir = opendir(dirpath);

    //reference: https://stackoverflow.com/questions/3554120/open-directory-using-c
    while ((directoryEntry = readdir(dir)) != NULL){
        printf ("%s\n", directoryEntry->d_name);
    }

     return 0;
     
}