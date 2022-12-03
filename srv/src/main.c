
/*
 *      THIS IS THE SERVER
*/
#include "../../lib/config.h"
#include "../../lib/rsa.h"

int main(int argC, char *argV[]){/*{{{*/

    if(argC < 2){
        char * output;
        output = malloc(strlen(argV[0])+10);
        strcpy(output, argV[0]);
        strcat(output, " [port]");
        error(output, 1);
        free(output);
    }

    rsa_generate_keypair();

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
