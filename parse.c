#include "csapp.h"
#include "rrshserver.h"

#define DEBUG 0
#define IsWhiteSpace(x) ((x == ' ') || (x == '\t'))
#define IsNewLine(x) (x == '\n')
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

void destroy_ul(user_cred** users, int* num)
{
    int i = 0;
    while( i < *num ) {
        destroy_uc(users[i]);
        i++;
    }
}

void destroy_cl(char** cmds, int* num)
{
    int i = 0;
    while( i < *num ) {
        Free(cmds[i]);
        i++;
    }
}

void parseTokens(char* buf, int* argc, char** argv)
{

    char* bptr;
    int intoken = 0;
    
    if( *argc > 0 ) {
        bptr = argv[0];
        memset(bptr, '\0', MAXBUF);
        *argc = 0;
    } //If argv has elements, reuse the memory.
    else {
        bptr = (char*)Malloc(MAXBUF);
    }
    
    memcpy(bptr, buf, MAXBUF);
    while( (*bptr) != EOF && (*bptr) != NULL ) {
        if( IsWhiteSpace(*bptr) || IsNewLine(*bptr) ) {
            intoken = 0;
            *bptr = '\0';
            bptr++;
        } else if ( intoken ) {
            bptr++;
        } else {
            intoken = 1;
            argv[*argc] = bptr;
            if( DEBUG ) {
                printf("Just assigned to argv[%d]: %s\n", *argc, argv[*argc]);
                fflush(stdout);
            }
            (*argc)++;
            bptr++;
        } 
    }
    argv[(*argc)] = NULL;
}

void create_ul(user_cred** userList, int* userNum) 
{
    int fd, argc, i;
    char buf[MAXBUF];
    user_cred* user;
    char* argv[MAXUSERS*2];
    argc = i = 0;
    memset(buf, '\0', sizeof(buf)); 
    if( DEBUG ) printf("In parseUsers, about to open the file: %s\n", usersFile);
    if( (fd = Open(usersFile, O_RDONLY|O_RDWR, S_IRUSR|S_IRGRP)) >= 0 ) {
        if( Read(fd, buf, MAXBUF) > 0 ) {
            parseTokens(buf, &argc, argv);
            if( argc % 2 == 0 ) {
                while( i < argc ) {
                    user = create_uc();
                    memcpy(user->name, argv[i], strlen(argv[i]));
                    i++;
                    memcpy(user->pass, argv[i], strlen(argv[i]));
                    i++;
                    userList[(*userNum)++] = user;
                }
            } else {
                unix_error("Couldn't read users file");
            }
        }
        Close(fd);
    }
}



void create_cl(char** cmdList, int* cmdNum)
{
    int fd;
    char buf[MAXBUF]; 
    memset(buf, '\0', sizeof(buf)); 
    if( DEBUG ) printf("In parseCmd, about to open the file: %s\n", cmdsFile);
    if( (fd = Open(cmdsFile, O_RDONLY, S_IRUSR)) >= 0 ) {
        if( Read(fd, buf, MAXBUF) > 0 ) {
            if( DEBUG ) printf("Read in a line, about to add to list\n");
            parseTokens(buf, cmdNum, cmdList);
        }
        Close(fd);
    }
    if( DEBUG ) {
        int i = 0;
        printf("About to print out list\n");
        while( i < *cmdNum ) {
            printf("(parser) command: %s\n", cmdList[i++]);
            fflush(stdout);
        }
    }
}
