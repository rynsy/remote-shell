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
int uNum = 0;
int uid = 0;

char* cmdList[MAXCMD];
int cNum = 0;

int login(int connfd, rio_t* rio, char* addr, int* port);
void eval(int connfd, rio_t* rio);

void create_ul(user_cred** users, int* num);
void destroy_ul(user_cred** users, int* num);
void create_cl(char** cmds, int* num);
void destroy_cl(char** cmds, int* num);
void echo(int connfd);

int main(int argc, char **argv) 
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    rio_t rio;              //initialize in main, pass around
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;

    create_ul(userList, &uNum);
    create_cl(cmdList, &cNum);

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Rio_readinitb(&rio, connfd);
        
        /* Determine the domain name and IP address of the client */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        //printf("server connected to %s (%s)\n", hp->h_name, haddrp);
       
        if(login(connfd, &rio, haddrp, &port)) {
            //We're live, baby
                        
            eval(connfd, &rio);
            printf("User %s disconnected.\n", userList[uid]->name);
        }
        Close(connfd);
    }
    destroy_ul(userList, &uNum);
    destroy_cl(cmdList, &cNum);
    exit(0);
}
/* $end rrshserverimain */
