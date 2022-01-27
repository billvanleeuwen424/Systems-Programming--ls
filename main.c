/* 
Objectives of this program:
    find the largest file
    find the smallest file
    the most recently modified file
    the oldest file
*/
#include <stdio.h>
#include <string.h>

//for create directory
#include <sys/stat.h>

//for opendir
#include <dirent.h>
//command line params
//argc = num of params
//char *argv[] is the array of params
//int argc, char *argv[]
int main(){
    printf("Hello World!\n");

    const char *dirpath = "./";

    DIR *opendir(const char *dirpath);

    mode_t mode = 04444;
    const char *pathname = "./yeet.txt";

    int yes = mkdir(pathname, mode);
    
     return yes;
}