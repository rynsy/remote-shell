#include "csapp.h"
#include "rrshserver.h"

#define DEBUG 1

int login(int connfd, user_cred** userList, int* uNum);
int validate(char* uName, char* uPass, user_cred** userList, int* uNum);


int login(int connfd, user_cred** userList, int* uNum)
{
    char name[MAXNAME], pass[MAXNAME];
    char* outBuf;
    size_t n;
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    if((n = Rio_readlineb(&rio, name, MAXNAME)) != 0) {
        strtok(name,"\n");
        if( DEBUG ) printf("entered name: %s\n", name);
        if( DEBUG ) fflush(stdout);
        if((n = Rio_readlineb(&rio, pass, MAXNAME)) != 0) {
            strtok(pass,"\n");
            if( DEBUG ) printf("entered pass: %s\n", pass);
            if( DEBUG ) fflush(stdout);
            if(validate(name, pass, userList, uNum)) {
                outBuf = "Login Approved\n";
                Rio_writen(connfd, outBuf, strlen(outBuf));
                return 1;
            }
        }
    }
    outBuf = "Login Failed\n";
    Rio_writen(connfd, outBuf, strlen(outBuf));
    return 0;
}

int validate(char* uName, char* uPass, user_cred** userList, int* uNum)
{
    int i = 0;
    if( DEBUG ) printf("validating credentials...\n");
    while( i < *uNum ) {
        if( !strcmp(userList[i]->name, uName) ) {
            if( DEBUG ) printf("name was right!\n");
            if( !strcmp(userList[i]->pass, uPass) ) {
                if( DEBUG ) printf("pass was right!\n");
                if( DEBUG ) printf("success!\n");
                return 1;
            }
        }
        i++;
    }
    if( DEBUG ) printf("failure!\n");
    return 0;
}
