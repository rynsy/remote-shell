#define MAXNAME 40
#define MAXUSERS 10
#define MAXCMD 10
char* usersFile;
char* cmdsFile;

typedef struct user_cred {
    char* name;
    char* pass;
} user_cred;
