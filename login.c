#include "csapp.h"
#include "rrshserver.h"

#define DEBUG 1

user_cred* userList[MAXUSERS];
int uNum;
int uid;

int login(int connfd, rio_t* rio, char* addr, int* port);
int validateUser(char* uName, char* uPass, user_cred** userList, int* uNum);


int login(int connfd, rio_t* rio, char* addr, int* port)
{
    char name[MAXNAME], pass[MAXNAME], outBuf[MAXNAME];
    size_t n;
    if((n = Rio_readlineb(rio, name, MAXNAME)) != 0) {
        strtok(name,"\n");
        printf("User %s logging in from %s at TCP port %d\n", name, addr, *port);
        if((n = Rio_readlineb(rio, pass, MAXNAME)) != 0) {
            strtok(pass,"\n");
            if(validateUser(name, pass, userList, &uNum)) {
                sprintf(outBuf,"Login Approved\n");
                Rio_writen(connfd, outBuf, strlen(outBuf));
                return 1;
            }
        }
    }
    sprintf(outBuf, "Login Failed\n");
    Rio_writen(connfd, outBuf, strlen(outBuf));
    return 0;
}

int validateUser(char* uName, char* uPass, user_cred** uL, int* uN)
{
    int i = 0;
    while( i < *uN ) {
        if( !strcmp(uL[i]->name, uName) ) {
            if( !strcmp(uL[i]->pass, uPass) ) {
                printf("User %s successfully logged in.\n", uName);
                uid = i;
                return 1;
            }
        }
        i++;
    }
    printf("User %s denied access.\n", uName);
    return 0;
}
void echo(int connfd) 
{
    size_t n; 
    char buf[MAXLINE]; 
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	printf("server received %d bytes\n", (int)n);
	Rio_writen(connfd, buf, n);
    }
}
