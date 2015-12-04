#include "csapp.h"
#include "rrshserver.h"

#define DEBUG 1

int uid;

int login(int connfd, user_cred** userList, int* uNum);
int validateUser(char* uName, char* uPass, user_cred** userList, int* uNum);


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
            if(validateUser(name, pass, userList, uNum)) {
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

int validateUser(char* uName, char* uPass, user_cred** userList, int* uNum)
{
    int i = 0;
    while( i < *uNum ) {
        if( !strcmp(userList[i]->name, uName) ) {
            if( !strcmp(userList[i]->pass, uPass) ) {
                printf("User %s successfully logged in.\n", userList[i]->name);
                uid = i;
                return 1;
            }
        }
        i++;
    }
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
