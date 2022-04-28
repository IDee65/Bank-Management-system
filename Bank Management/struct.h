#ifndef STRUCT
#define STRUCT
#define STAR "***********"


typedef enum accountType
{
    INVALID_ACCOUNT = -1,
    SAVINGS_ACCOUNT = 1,
    CURRENT_ACCOUNT
}accountType;


typedef struct user
{
    char fName[20];
    char lName[20];
    char mName [20];
    int accountNumber;
    char phone_no[11];
    float balance;
    accountType accntType;
    char buffer [1024];
}user;

//Functions prototype
short createJSON (user * details);
short parseJSON (user *details);
short createAccount (user * details);
int generateAccountNo (void);
int openDB();
int createSchema ();
short insertIntoDataBase (user * details);
short readDBbyAccountNo (user *details);
short creditDbAccount (user *details);
short debitDbAccount (user *details);
short getdbAccountInfo (user *details);
short deleteDbRecord(user *details);

#endif