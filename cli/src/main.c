
/*
 *      THIS IS THE CLIENT
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <netdb.h>

#include <curses.h>

#define BUFFER_SIZE 0x800

void error(const char *output, ...){/*{{{*/

    va_list vList;
    va_start(vList, output);
    
    printf("%s", output);

    int errorValue = va_arg(vList, int);
    if(errorValue)
        exit(errorValue);
    exit(-1);
}/*}}}*/

int main(int argC, char * argV[]){/*{{{*/

    if(argC < 3){
        char * output;
        output = malloc(strlen(argV[0])+20);
        strcpy(output, argV[0]);
        strcat(output, " [address] [port]");
        error(output, 1);
        free(output);
    }

    initscr();

    noecho();
    int keypress;

/*{{{ Socket configuration*/    
    int socketFD, portNumber;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in servAddr;
    struct hostent * server;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0)
        error("socket opening error", 2);
    portNumber = atoi(argV[2]);
    server = gethostbyname(argV[1]);
    if(server == NULL)
        error("can not connect to host", -1);

    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &servAddr.sin_addr.s_addr, server->h_length);
    servAddr.sin_port = htons(portNumber);

    if(connect(socketFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        error("connection error", 3);
/*}}}*/
    while(1){
        bzero(buffer, BUFFER_SIZE);
        //fgets(buffer, BUFFER_SIZE, stdin);
        buffer[1] = 0;
        buffer[0] = getch();
        if(write(socketFD, buffer, strlen(buffer)) < 0)
            error("write error", 4);

        }
    printf("DONE!\n");

    close(socketFD);
    return 0;
}/*}}}*/
