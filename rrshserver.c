/* 
 *  * rrsh-server.c - a Restricted-Remote-SHell server
 *   */ 
/* $begin  rrshservermain*/
#include "csapp.h"
#include "rrshserver.h"

char* usersFile = "rrshusers.txt";
char* cmdsFile = "rrshcommands.txt";
//void echo(int connfd);
void login(int connfd);

void parseUsers(user_cred* userList);
void parseCmd(char** cmdList);
void destroy_uc(user_cred* user);

int mainLoop(int argc, char **argv) 
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* Determine the domain name and IP address of the client */
        hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
        sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);

 //       echo(connfd);
        Close(connfd);
    }
    exit(0);
}

int main( int argc, char** argv ) {
   int i = 0; 
   user_cred* userList[MAXUSERS];
   parseUsers(userList);
   while(userList[i] != NULL) {
    printf("(main) name: %s\t pass: %s\n", userList[i]->name, userList[i]->pass);
    fflush(stdout);
    i++;
   }
   exit(0);
}
/* $end rrshserverimain */
