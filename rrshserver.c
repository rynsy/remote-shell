/* 
 *  * rrsh-server.c - a Restricted-Remote-SHell server
 *   */ 
/* $begin  rrshservermain*/
#include "csapp.h"
#include "rrshserver.h"

int ACCEPT_CONN = 1;
char* usersFile = "rrshusers.txt";
char* cmdsFile = "rrshcommands.txt";
user_cred* userList[MAXUSERS];

//void echo(int connfd);
int login(int connfd, user_cred** list, int* num);
void eval(int connfd, char** cmdList, int* num);

void create_ul(int* uNum, user_cred** userList);
void destroy_ul(int* uNum, user_cred** userList);
void create_cl(int* cNum, char** cmdList);
void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    char* cmdList[MAXCMD];
    int uNum = 0, cNum = 0;

    create_ul(&uNum, userList);
    create_cl(&cNum, cmdList);

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
        if(ACCEPT_CONN) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        /* Determine the domain name and IP address of the client */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        if(login(connfd, userList, &uNum)) {
            ACCEPT_CONN = 0;
            //We're live, baby
            eval(connfd, cmdList, &cNum);
        }
        Close(connfd);
        }
    }
    destroy_ul(&uNum, userList);
    exit(0);
}
/* $end rrshserverimain */
