
/*
 *      THIS IS THE SERVER
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


#include <openssl/rsa.h>
#include <openssl/pem.h>

#define BUFFER_SIZE 0x800
#define NUMBER_OF_USERS 2

void error(const char *output, ...){/*{{{*/ 

    //triple brakets represent a fold in nvim if you set foldmethod=marker

    va_list vList;
    va_start(vList, output);
    
    printf("%s", output);

    int errorValue = va_arg(vList, int);
    if(errorValue)
        exit(errorValue);
    exit(-1);
}/*}}}*/


int main(int argC, char *argV[]){/*{{{*/

    if(argC < 2){
        char * output;
        output = malloc(strlen(argV[0])+10);
        strcpy(output, argV[0]);
        strcat(output, " [port]");
        error(output, 1);
        free(output);
    }



    /*{{{ Socket configuration*/    
    int socketFD, newSocketFD, portNumber;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in servAddr, cliAddr;
    socklen_t cliLen;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0)
        error("socket opening error", 2);

    bzero((char *) &servAddr, sizeof(servAddr));
    bzero((char *) &cliAddr, sizeof(cliAddr));
    portNumber = atoi(argV[1]);
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(portNumber);

    if(bind(socketFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        error("binding error", 3);

    listen(socketFD, NUMBER_OF_USERS);
    cliLen = sizeof(cliAddr);

    newSocketFD = accept(socketFD, (struct sockaddr *) &cliLen, &cliLen);
       if(newSocketFD < 0)
        error("socket accept error", 4);
     /*}}}*/

    // {{{ Herein layeth the logic
    while(1){
        bzero(buffer, BUFFER_SIZE);
        if(read(newSocketFD, buffer, BUFFER_SIZE) < 0)
            error("Reading error", 5);
        if(!buffer[0])
            break;

        printf("Client: %s %x\n", buffer, buffer[0]);

    }
    /*}}}*/
    close(newSocketFD);
    close(socketFD);
    return 0;
}/*}}}*/
