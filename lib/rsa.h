#ifndef RSA_H
#define RSA_H

#include "config.h"

void rsa_generate_keypair(){
    if(access(PRIVATE_KEY, F_OK) == 0 && access(PUBLIC_KEY, F_OK) == 0)
        return;
    if(access(PRIVATE_KEY, F_OK) != 0){
        
        int pid = fork();
        if(pid==-1) error("error at fork", 10);
        if(!pid){
            execlp("openssl", "openssl", "genpkey", "-algorithm", "RSA", "-out", PRIVATE_KEY, "-pkeyopt", "rsa_keygen_bits:2048", NULL);
            error("error at generating private key, please check if openssl is installed", 11);
        } else{
            wait(&pid);
        }
    }
    int pid = fork();
    if(pid==-1) error("error at fork", 10);
    if(!pid){
        int publicKeyFD = open(PUBLIC_KEY, O_WRONLY|O_CREAT, 0666);
        dup2(publicKeyFD, 1);
        close(publicKeyFD);
        execlp("openssl", "openssl", "rsa", "-in", PRIVATE_KEY, "-pubout", NULL);
        error("error at generating public key, please check if openssl is installed", 12);  
    } else{
        wait(&pid);
    }
}


#endif
