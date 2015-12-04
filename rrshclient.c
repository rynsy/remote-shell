/*
 * rrshclient.c - a Restricted Remote Shell client
 */
/* $begin rrshclientmain */
#include "csapp.h"
#include "rrshserver.h"

char confirm[MAXNAME] = "RRSH COMMAND COMPLETED\n";
char* prompt = "rrsh > ";

void shell(int clientfd, rio_t* rio) 
{
    char buf[MAXLINE];
   
    memset(buf,'\0',sizeof(buf));
    printf("%s", prompt);
    while( Fgets(buf, MAXLINE, stdin) != NULL ) {
        if( !strcmp(buf, "quit\n") ) {
            return ;   
        }
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(rio, buf, MAXLINE);
        while( strcmp(buf, confirm) != 0 ) {
            Fputs(buf, stdout);
            Rio_readlineb(rio, buf, MAXLINE);
        }
        printf("%s", prompt);
        fflush(stdout);
    }
}

int main(int argc, char **argv) 
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    printf("username: ");
    if(Fgets(buf, MAXLINE, stdin) != NULL) {
	    Rio_writen(clientfd, buf, strlen(buf));
        printf("password: ");
        if(Fgets(buf, MAXLINE, stdin) != NULL) {
	        Rio_writen(clientfd, buf, strlen(buf));
	        Rio_readlineb(&rio, buf, MAXLINE);
            if( !strcmp(buf, "Login Approved\n") ) {
                shell(clientfd, &rio); 
            } else {
                printf("Login Failed\n");
            }
        }
    }
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end rrshclientmain */
