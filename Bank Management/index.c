#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "sqlite3.h"
#include "struct.h"
#include "function.c"
#include "DB.c"



int main()
{
    openDB();
    createSchema();
    // char cont ='y';
  
    while (1)
    {
        user usr;
        int option = 0;

        printf("\n%s BANK MANAGEMENT SYSTEM %s", STAR, STAR);
        printf("\n\n1. Create Account\n");
        printf("2. Credit Account\n");
        printf("3. Withdraw from Account\n");
        printf("4. Check Balance\n");
        printf("5. Close Account\n");

        printf("\nEnter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
            system("cls");
            if (createAccount(&usr))
            continue;
            break;

            case 2:
            system("cls");
            if (creditAccount(&usr))
            continue;
            break;

            case 3:
            system("cls");
            if (withdrawFromAccount(&usr))
            continue;
            break;

            case 4:
            system("cls");
            if (getAccountInfo(&usr))
            continue;
            break;

            case 5:
            system("cls");
            if (closeAccount(&usr))
            break;

            default:
            printf("Invalid entry\n");
            break;
        }

    // printf("Do you want to perform another transaction[y/n]\n");
    // scanf("%c", &ch);
    // continue;
    break;

    }
    return 0;
}