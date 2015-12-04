#include "csapp.h"
#include "rrshserver.h"

user_cred* userList[MAXUSERS];
int uNum;
int uid;

char* cmdList[MAXCMD];
int cNum;

char confirm[MAXNAME] = "RRSH COMMAND COMPLETED\n";
char deny[MAXLINE];

void parseTokens(char* buf, int* argc, char** argv); 

int validateCmd(char* cmd) 
{
    int i = 0;
    while( i < cNum ) {
        if(!strcmp(cmd, cmdList[i])) {
            return 1;
        }
        i++;
    }
    return 0;
}

void eval(int connfd, rio_t* rio)
{
    size_t n; 
    char* argv[MAXBUF];
    char buf[MAXLINE]; 
    int argc = 0;
    
    memset(buf, '\0', sizeof(buf)); 
    while((n = Rio_readlineb(rio, buf, MAXLINE)) != 0) { //line:netp:echo:eof
        parseTokens(buf, &argc, argv);
        strtok(buf,"\n");
        printf("User %s sent the command '%s' to be executed.\n", userList[uid]->name,buf);
        if(validateCmd(argv[0])) {
            printf("Forking/Execing the command '%s' on behalf of %s.\n", buf, userList[uid]->name);
            if( Fork() == 0 ) {
                //redirect output
                Dup2(connfd, STDOUT_FILENO);
                Dup2(connfd, STDIN_FILENO);
                Dup2(connfd, STDERR_FILENO);

                if( execvp(argv[0], argv) < 0 ) {
                    printf("%s: Command not found.\n", argv[0]);
                    exit(0);
                }
            } else { 
                Wait(NULL);
                Rio_writen(connfd, confirm, strlen(confirm));
            }
        } else {
            printf("The command '%s' is not allowed.\n", buf);
            sprintf(deny, "The command '%s' is not allowed on this server.\n", buf);
 	        Rio_writen(connfd, deny, strlen(deny));
            Rio_writen(connfd, confirm, strlen(confirm));
        }
        fflush(stdout);
    }
}
