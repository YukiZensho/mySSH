#ifndef CONFIG_H
#define CONFIG_H
/*{{{ INCLUDES*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <netdb.h>
#include <curses.h>
#include <fcntl.h>
/*}}}*/

#define BUFFER_SIZE 0x800

#define PORT 8080
#define NUMBER_OF_USERS 2

#define PUBLIC_KEY "public.pub"
#define PRIVATE_KEY "private.pem"
#define SEND_KEY "send.pub"



void error(const char *output, ...){/*{{{ error behaveour definition*/

    va_list vList;
    va_start(vList, output);
    
    printf("%s", output);

    int errorValue = va_arg(vList, int);
    if(errorValue)
        exit(errorValue);
    exit(-1);
}/*}}}*/



#endif
