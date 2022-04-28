#include<stdio.h>
#include "struct.h"
#include<sqlite3.h>


sqlite3 *db;

int openDB()
{
    int rc = -1;
    rc= sqlite3_open("bank.db", &db);
    if (rc != SQLITE_OK)
    {
        printf("Unable to open data base");
        return -1;
    }

    return 0;
}

int createSchema()
{
    char *errMsg;
    int rc = -1;
    char accountNumber[11] = {'\0'};
    char formedJSON [500] = {'\0'};
    char *sql;
    if (openDB())
    {
        return -1;
    }
    sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS Trans(accountNumber INT, formedJSON TEXT NOT NULL);");
    rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Unable to create database: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
    }
    else
    sqlite3_close(db);

    return 0;
}

short insertIntoDataBase (user * details)
{
    if (createJSON(details))
    {
        return -1;
    }
    if (openDB())
    {
        return -1;
    }

    char * errMsg;
    int rc = -1;
    sqlite3_stmt *stmt = 0;
    char *sql;
    sql = sqlite3_mprintf("INSERT INTO Trans VALUES('%d', '%q');", details->accountNumber, details->buffer);

    rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Insertion error %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    return 0;

}

short readDBbyAccountNo (user *details)
{
    sqlite3_stmt *res = 0;
    char *errMsg;
    char data[500] = {'\0'};
    memset(details, 0x00, sizeof(details));
    int account = 0;

    printf("Enter your account number: ");
    scanf("%d", &account);

    if (openDB())
    {
        return -1;
    }

    char *sql = sqlite3_mprintf("SELECT accountNumber, formedJSON FROM Trans WHERE accountNumber = ('%d');", account);
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int64(res, 1, 3);
    }else
    {
        printf("Failed: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW)
    {
        strncpy(details->buffer, sqlite3_column_text(res, 1), sizeof(details->buffer));

        if (!&details->buffer[2])
        {
            printf("Invalid account number\n");
            return -1;
        }

    }

    if (parseJSON(details))
    {
        return -1;

    }
    return 0;
}

short creditDbAccount(user *details)
{
    sqlite3_stmt *res = 0;
    char *errMsg;
    char data[500] = {'\0'};
    memset(details, 0x00, sizeof(details));
    float amount = 0;

    if (readDBbyAccountNo(details))
    {
        return -1;
    }


    printf("Enter amount: ");
    scanf("%f", &amount);

    details->balance += amount;

    if (createJSON (details))
    {
        return -1;
    }
    char *sql = sqlite3_mprintf("UPDATE Trans  SET formedJSON = '%q' WHERE accountNumber  = '%d';", details->buffer, details->accountNumber);
    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    return 0;
}

short debitDbAccount(user *details)
{
    sqlite3_stmt *res = 0;
    char *errMsg;
    char data[500] = {'\0'};
    memset(details, 0x00, sizeof(details));
    float amount = 0;

    if (readDBbyAccountNo(details))
    {
        return -1;
    }

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    if (amount > details->balance)
    {
        printf("Insufficeint Fund\n");
        return -1;
    }
    
    details->balance -= amount;

    if (createJSON (details))
    {
        return -1;
    }
    char *sql = sqlite3_mprintf("UPDATE Trans  SET formedJSON = '%q' WHERE accountNumber  = '%d';", details->buffer, details->accountNumber);
    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    return 0;
}

short getdbAccountInfo(user *details)
{
    sqlite3_stmt *res = 0;
    char *errMsg;
    char data[500] = {'\0'};
    memset(details, 0x00, sizeof(details));
    float balance = 0;

    if (readDBbyAccountNo(details))
    {
        return -1;
    }

    char *sql = sqlite3_mprintf("SELECT accountNumber, formedJSON FROM Trans WHERE formedJSON = ('%d');", details->balance);
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK)
    {
        printf("Error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    printf("Account name: %s %s %s\n", details->fName, details->lName, details->mName);
    printf("Account balance: %.2lf\n", details->balance);
    // printf("Phone number: %s\n", details->phone_no);




    return 0;
}

short deleteDbRecord(user *details)
{
    sqlite3_stmt *res = 0;
    char *errMsg;
    char data[500] = {'\0'};
    memset(details, 0x00, sizeof(details));
    int account = 0;

    // printf("Enter your account number: ");
    // scanf("%d", &account);

    if (readDBbyAccountNo(details))
    {
        return -1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM Trans WHERE accountNumber = '%d';", details->accountNumber);
    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("Failed: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }

    printf("Account deleted\n");

    return 0;
}