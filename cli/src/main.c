
/*
 *      THIS IS THE CLIENT
*/

#include "../../lib/config.h"
#include "../../lib/rsa.h"

int main(int argC, char * argV[]){/*{{{*/

    if(argC < 2){ /*{{{*/
        char * output;
        output = malloc(strlen(argV[0])+20);
        strcpy(output, argV[0]);
        strcat(output, " [address]");
        error(output, 1);
        free(output);
    } /*}}}*/
    rsa_generate_keypair();
    char username[BUFFER_SIZE], buffer[BUFFER_SIZE];
     
    initscr();
    noecho();
    int keypress; 
/*{{{ Socket configuration*/    
    int socketFD;

    struct sockaddr_in servAddr;
    struct hostent * server;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0)
        error("socket opening error", 2);
    server = gethostbyname(argV[1]);
    if(server == NULL)
        error("can not connect to host", -1);

    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &servAddr.sin_addr.s_addr, server->h_length);
    servAddr.sin_port = htons(PORT);

    if(connect(socketFD, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        error("connection error", 3);
/*}}}*/
  
    /*{{{ LOGIN*/
    bzero(username, BUFFER_SIZE);

    do{
        keypress = getch();
        username[strlen(username) - 1] = keypress;
        mvprintw(0, 0, "%s username: %s", argV[1], username);
    }while(keypress != '\n');

    mvprintw(1, 0, "%s           ", username);
    return 0;
    /*}}}*/

    while(1){
        bzero(buffer, BUFFER_SIZE);
        buffer[1] = 0;
        buffer[0] = getch();
        if(write(socketFD, buffer, strlen(buffer)) < 0)
            error("write error", 4);

        if(buffer[0] == 0x1b)
            break;
        
        if(read(socketFD, buffer, BUFFER_SIZE) < 0)
            error("write error", 4);
        mvprintw(2, 0, "%s", buffer);
        }
    printf("DONE!\n");

    close(socketFD);
    return 0;
}/*}}}*/
