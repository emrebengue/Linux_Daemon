# include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FEW_ARGS_ERROR 1

int main(int argc, char *argv[]){

    char *path = NULL;
    char *token = NULL;

    if(argc < 2){
        fprintf(stderr, "Not enough arguments\n");
        exit(FEW_ARGS_ERROR);    
    }

    path = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(path, argv[1]);

    //https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

    token = strtok(path, "/");

    while(token != NULL){
        path = token;
        token = strtok(NULL, "/");
    }

    printf("%s\n", path);
}