# include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <sys/inotify.h> //https://man7.org/linux/man-pages/man7/inotify.7.html


#define EXIT_SUCCESS 0
#define FEW_ARGS_ERROR 1
#define EXIT_NULL_PATH 2
#define Inotif_watch_queue_EXIT_ERROR 3
#define Inotif_status_EXIT_ERROR 4
#define EXIT_READ 5


int Inotif_watch_queue = -1;
int Inotif_status = -1;


int main(int argc, char *argv[]){

    char *path = NULL;
    char *token = NULL;
    char buffer[4096];

    ssize_t readSize;

    const struct inotify_event *event;

    char name;

    

    const uint32_t watchMask = IN_CREATE | IN_DELETE | IN_MODIFY | IN_ACCESS | IN_CLOSE_WRITE | IN_MOVED_FROM | IN_MOVE_SELF;

    if(argc < 2){
        fprintf(stderr, "Not enough arguments\n");
        exit(FEW_ARGS_ERROR);    
    }

    path = (char *)malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(path, argv[1]);

   

    token = strtok(path, "/");  //https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

    while(token != NULL){
        path = token;
        token = strtok(NULL, "/");
    }

    if(path == NULL){
        fprintf(stderr, "Error in path\n");
        exit(EXIT_NULL_PATH);
    }

    Inotif_watch_queue = inotify_init();

    if(Inotif_watch_queue == -1){
        fprintf(stderr, "Error in inotify_init()\n");
        exit(Inotif_watch_queue_EXIT_ERROR);
    }

    Inotif_status = inotify_add_watch(Inotif_watch_queue, path, watchMask);

    if(Inotif_status == -1){
        fprintf(stderr, "Error in inotify_add_watch()\n");
        exit(Inotif_status_EXIT_ERROR);
    }

    printf("Waiting for events...\n");

    while(true){

        readSize = read(Inotif_watch_queue, buffer, sizeof(buffer));

        if(readSize == -1){
            fprintf(stderr, "Error in read()\n");
            exit(EXIT_READ);
        }

        for(char* bufferpointer = buffer; bufferpointer < buffer + readSize; bufferpointer+= sizeof(struct inotify_event) + event -> len){ //checkout Inotify man page from the link

            event = (struct inotify_event *)bufferpointer;
            
            if(event -> mask & IN_CREATE){
                printf("File created: %s\n", argv[1]);
            }
            if(event -> mask & IN_DELETE){
                printf("File deleted: %s\n", argv[1]);
            }
            if(event -> mask & IN_MODIFY){
                printf("File modified: %s\n", argv[1]);
            }
            if(event -> mask & IN_ACCESS){
                printf("File accessed: %s\n", argv[1]);
            }
            if(event -> mask & IN_CLOSE_WRITE){
                printf("File closed: %s\n", argv[1]);
            }
            if(event -> mask & IN_MOVED_FROM){
                printf("File moved from: %s\n", argv[1]);
            }
            if(event -> mask & IN_MOVE_SELF){
                printf("File moved to: %s\n", argv[1]);
            }
            else{
                continue;
            }
            
        }
    }
    
}