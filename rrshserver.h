#define MAXNAME 40
#define MAXUSERS 10
#define MAXCMD 10

int ACCEPT_CONN;
int uid; // id of currently logged in user. Better way of doing this, I'm sure
char confirm[MAXNAME];

char* usersFile;
char* cmdsFile;
char* prompt;

typedef struct user_cred {
    char* name;
    char* pass;
} user_cred;

user_cred* userList[MAXUSERS];
char* cmdList[MAXCMD];
