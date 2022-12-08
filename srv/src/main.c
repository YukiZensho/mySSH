
/*
 *      THIS IS THE SERVER
*/
#include "../../lib/config.h"
#include "../../lib/rsa.h"

//#include "rsa.h"
//#include "config.h"

void connect_socket(int * socketFD, int * newSocketFD, struct sockaddr_in * servAddr, struct sockaddr_in * cliAddr, socklen_t * cliLen){/*{{{*/

    *socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(*socketFD < 0)
        error("socket opening error", 2);

    bzero((char *) servAddr, sizeof(*servAddr));
    bzero((char *) cliAddr, sizeof(*cliAddr));
   
    servAddr->sin_family = AF_INET;
    servAddr->sin_addr.s_addr = INADDR_ANY;
    servAddr->sin_port = htons(PORT);

    if(bind(*socketFD, (struct sockaddr *) servAddr, sizeof(*servAddr)) < 0)
        error("binding error", 3);

    listen(*socketFD, NUMBER_OF_USERS);
    *cliLen = sizeof(*cliAddr);


}
/*}}}*/

void instance(int * socketFD, socklen_t * cliLen){ /*{{{*/

    char buffer[BUFFER_SIZE], username[BUFFER_SIZE],keypress;
    
    int newSocketFD;
    newSocketFD = accept(*socketFD, (struct sockaddr *) cliLen, cliLen);
    if(newSocketFD < 0)
        error("socket accept error", 4); 

    bzero(username, BUFFER_SIZE);
    if(read(newSocketFD, username, BUFFER_SIZE) < 0)
        error("Reading error", 5);
   printf("%s",username);

    bzero(buffer, BUFFER_SIZE);
    if(read(newSocketFD, buffer, BUFFER_SIZE) < 0)
        error("Reading error", 5);
    printf("#%s\n", buffer);

    while(1){
        bzero(buffer, BUFFER_SIZE);
        if(read(newSocketFD, buffer, BUFFER_SIZE) < 0)
            error("Reading error", 5);
        if(!buffer[0])
            break;
        printf("pressed = %c 0x%x\n", buffer[0], buffer[0]);
        keypress = buffer[0];
        bzero(buffer, BUFFER_SIZE);
        sprintf(buffer, "Pressed: %c 0x%x", keypress, keypress);
        //sprintf(buffer, "You pressed : %c 0x%x\n", keypress, keypress);
        if(write(newSocketFD, buffer, strlen(buffer)) < 0)
            error("Writing error", 6);

        if(keypress == 0x1b)
            break;

    }
    printf("Done with this instance!\n");
    close(newSocketFD);
    exit(0);

}/*}}}*/

int main(int argC, char *argV[]){/*{{{*/
/*{{{
    if(argC < 2){
        char * output;
        output = malloc(strlen(argV[0])+10);
        strcpy(output, argV[0]);
        strcat(output, " [port]");
        error(output, 1);
        free(output);
    }
    }}}*/
    rsa_generate_keypair();

    /*{{{ Socket configuration*/    
    int socketFD, newSocketFD, secSocketFD, portNumber;
    struct sockaddr_in servAddr, cliAddr;
    socklen_t cliLen;
    
    connect_socket(&socketFD, &newSocketFD, &servAddr, &cliAddr, &cliLen);
    /*}}}*/
    int activeUsers = 0;
    while(1){
        if(activeUsers < NUMBER_OF_USERS){
            int pid;
            if((pid = fork()) < 0)
                error("Forking error", 7);
            if(!pid)
                instance(&socketFD, &cliLen);
            else{
                activeUsers += 1;
                continue;
            }
        }
    //instance(&socketFD, &cliLen);
    }
    close(socketFD);
    return 0;
}/*}}}*/
