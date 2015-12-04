#include "csapp.h"
#include "rrshserver.h"

user_cred* userList[MAXUSERS];
int uid;

void parseTokens(char* buf, int* argc, char** argv); 

int validateCmd(char* cmd, char** cmdList, int* cNum)
{
    printf("Validating command....\n");
    int i = 0;
    while( i < *cNum ) {
        if(!strcmp(cmd, cmdList[i])) {
            return 1;
        }
        i++;
    }
    return 0;
}

void eval(int connfd, char** cmdList, int* cNum)
{
    size_t n; 
    int argc = 0;
    char* argv[MAXLINE];
    char buf[MAXLINE]; 
    rio_t rio;
    
    memset(buf, '\0', sizeof(buf)); 
    Rio_readinitb(&rio, connfd);
   
    while(1) {
        if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
	        printf("server received %d bytes\n", (int)n);
            strtok(buf,"\n");
	        printf("User %s sent the command '%s' to be executed.\n", userList[uid]->name, buf);
            parseTokens(buf, &argc, argv);
            if(validateCmd(argv[0], cmdList, cNum)) {
	            printf("Forking/Execing the command '%s' on behalf of %s.\n", buf, userList[uid]->name);
            } else {
	            printf("The command '%s' is not allowed.\n", buf);
            }

    	    Rio_writen(connfd, argv[0], strlen(argv[0]));
        }
    }
}
