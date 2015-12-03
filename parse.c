#include "csapp.h"
#include "rrshserver.h"

#define DEBUG 1
#define IsWhiteSpace(x) ((x == ' ') || (x == '\t'))

char* usersFile;
char* cmdsFile;

user_cred* create_uc()
{
    user_cred* data = (user_cred*) Malloc(sizeof(user_cred));
    if(data)
    {
        data->name = (char*)Malloc(sizeof(char)*MAXNAME);
        data->pass = (char*)Malloc(sizeof(char)*MAXNAME);
    }
    return data;
}

void destroy_uc(user_cred* data)
{
    Free(data->name);
    Free(data->pass);
    Free(data);
}

void parseUsers(user_cred** userList) 
{
    int fd;
    char buf[MAXBUF];

    if( DEBUG ) printf("In parseUsers, about to open the file: %s\n", usersFile);
    if( (fd = Open(usersFile, O_RDONLY|O_RDWR, S_IRUSR|S_IRGRP)) >= 0 ) {
        //read line from file
        //add user and password to user_cred struct 
        if( Read(fd, buf, MAXBUF) > 0 ) {
        }
    }
}



void parseCmd(char** cmdList)
{
    int fd;
    int argc = 0;
    char buf[MAXLINE]; 
    if( DEBUG ) printf("In parseCmd, about to open the file: %s\n", cmdsFile);
    if( (fd = Open(cmdsFile, O_RDONLY, S_IRUSR)) >= 0 ) {
        while( Read(fd, buf, MAXLINE) > 0 ) {
            if( DEBUG ) printf("Read in a line, about to add to list\n");
            if( (cmdList[argc++] = strtok(buf, " ")) == NULL ) {
                unix_error("Couldn't load cmds");
                exit(0);
            }
        }
    }
    if( DEBUG ) {
        printf("About to print out list\n");
        argc = 0;
        while( cmdList[argc] != NULL ) {
            printf("(parser) command: %s", cmdList[argc++]);
            fflush(stdout);
        }
    }
}
