
/*
 *      THIS IS THE CLIENT
*/

#include "../../lib/config.h"
#include "../../lib/rsa.h"

int main(int argC, char * argV[]){/*{{{*/

    if(argC < 3){
        char * output;
        output = malloc(strlen(argV[0])+20);
        strcpy(output, argV[0]);
        strcat(output, " [address] [port]");
        error(output, 1);
        free(output);
    }
    rsa_generate_keypair();

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
        buffer[1] = 0;
        buffer[0] = getch();
        if(write(socketFD, buffer, strlen(buffer)) < 0)
            error("write error", 4);
        
        if(read(socketFD, buffer, BUFFER_SIZE) < 0)
            error("write error", 4);
        mvprintw(0, 0, "%s", buffer);
        }
    printf("DONE!\n");

    close(socketFD);
    return 0;
}/*}}}*/
