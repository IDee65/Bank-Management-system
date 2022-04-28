#include<stdio.h>
#include<time.h>
#include "struct.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

short createJSON (user * details);
short parseJSON (user *details);

int generateAccountNo (void)
{
    int num;
    int lower = 1000000000, upper = 2000000000;
    srand(time(0));
    num = (rand() % (upper - lower + 1) + lower);
    return num;
}


short createAccount (user * details) 
{
    char firstName [20] = {'\0'};
    char lastName [20] = {'\0'};
    char middleName [20] = {'\0'};
    accountType accntType = INVALID_ACCOUNT;
    char phoneNumber [20] = {'\0'};
    int accountNumber = 0;
    float balance = 0.00;

    printf("Select Account Type\n");
    printf("1. SAVINGS ACCOUNT\n");
    printf("2. CURRENT ACCOUNT\n");
    printf("Enter selection for account type: ");
    scanf("%d", &accntType);
    if (accntType == INVALID_ACCOUNT)
    {
        printf("Invalid account type selected\n");
        return -1;
    }
    
    system("cls");
    printf("Enter first name: ");
    scanf("%s", firstName);

    printf("Enter middle name: ");
    scanf("%s", middleName);

    printf("Enter last name: ");
    scanf("%s", lastName);

    printf("Enter your phone number: ");
    scanf("%s", phoneNumber);

 
    accountNumber = generateAccountNo();

    memset(details, 0x00, sizeof(user));
    strncpy(details->fName, firstName, sizeof(details->fName));
    strncpy(details->lName, lastName, sizeof(details->lName));
    strncpy(details->mName, middleName, sizeof(details->mName));
    strncpy(details->phone_no, phoneNumber, sizeof(details->phone_no));
  
    details->accntType = accntType;
    details->accountNumber = accountNumber;
    details->balance = balance;
    
    if (insertIntoDataBase(details))
    {
        return -1;
    }

    printf("Account created successfully\n");

    printf("Your Account number is: %d\n", details->accountNumber);
    printf("Account Name: %s %s %s\n", details->lName, details->fName, details->mName);

    return 0;

}


short createJSON (user * details)   //create cjson object
{
    cJSON* information = NULL;

    information = cJSON_CreateObject();
    
    if (!information || information == NULL){
        printf("Unable to create details object!\n");
        return -1;
    }

    cJSON_AddItemToObject(information, "FirstName", cJSON_CreateString(details->fName));
    cJSON_AddItemToObject(information, "LastName", cJSON_CreateString(details->lName));
    cJSON_AddItemToObject(information, "MiddleName", cJSON_CreateString(details->mName));
    cJSON_AddItemToObject(information, "AccountNumber", cJSON_CreateNumber(details->accountNumber));
    cJSON_AddItemToObject(information, "PhoneNumber", cJSON_CreateString(details->phone_no));
    cJSON_AddItemToObject(information, "AccountBalance", cJSON_CreateNumber(details->balance));
    cJSON_AddItemToObject(information, "AccountType", cJSON_CreateNumber(details->accntType));

    char *buffer = cJSON_PrintUnformatted(information);
   
    memset(details->buffer, 0x00, sizeof(details->buffer));
    strncpy(details->buffer, buffer, sizeof(details->buffer));

    //  printf("The information in json format is %s\n", details->buffer);

    return 0;

}

short parseJSON (user *details)   //parsed cjson
{
    cJSON *json = NULL;
    cJSON *firstName = NULL;
    cJSON *lastName = NULL;
    cJSON *middleName = NULL;
    cJSON *accntType = NULL;
    cJSON *accntBalance = NULL;
    cJSON *phoneNo = NULL;
    cJSON *accountNo = NULL;

    const char *body;
    body = strchr (details->buffer, '{');
    if (body == NULL)
    {
        return -1;
    }
    // printf("The body is: %s\n", body);

    json = cJSON_Parse(body);
    if (!json || cJSON_IsNull(json))
    {
        cJSON_Delete(json);
        return -1;
    }

    firstName = cJSON_GetObjectItemCaseSensitive(json, "FirstName");
    if (!firstName || !cJSON_IsString(firstName)){
        cJSON_Delete(firstName);
        return -1;
    }
    strncpy(details->fName, firstName->valuestring, sizeof(firstName)-1);
    

    lastName = cJSON_GetObjectItemCaseSensitive(json, "LastName");
    if (!lastName || !cJSON_IsString(lastName)){
        cJSON_Delete(lastName);
        return -1;
    }
    strncpy(details->lName, lastName->valuestring, sizeof(lastName)-1);

    middleName = cJSON_GetObjectItemCaseSensitive(json, "MiddleName");
    if (!middleName || !cJSON_IsString(middleName)){
        cJSON_Delete(middleName);
        return -1;
    }
    strncpy(details->mName, middleName->valuestring, sizeof(middleName)-1);

    accountNo = cJSON_GetObjectItemCaseSensitive(json, "AccountNumber");
    if (!accountNo || !cJSON_IsNumber(accountNo)){
        cJSON_Delete(accountNo);
        return -1;
    }
    details->accountNumber = accountNo->valueint;

    phoneNo = cJSON_GetObjectItemCaseSensitive(json, "PhoneNumber");
    if (!phoneNo || !cJSON_IsString(phoneNo)){
        cJSON_Delete(phoneNo);
        return -1;
    }
    strncpy(details->phone_no, phoneNo->valuestring, sizeof(phoneNo)-1);

    accntBalance = cJSON_GetObjectItemCaseSensitive(json, "AccountBalance");
    if (!accountNo || !cJSON_IsNumber(accntBalance)){
        cJSON_Delete(accntBalance);
        return -1;
    }
    details->balance = accntBalance->valuedouble;

    accntType = cJSON_GetObjectItemCaseSensitive(json, "AccountType");
    if (!accntType || !cJSON_IsNumber(accntType)){
        cJSON_Delete(accntType);
        return -1;
    }
    details->accntType = accntType->valueint;


    // printf("%s %s %s %s\n", details->fName, details->lName, details->mName, details->phone_no);


    return 0;
}


short creditAccount (user *details)
{
 
    if (creditDbAccount(details))
    {
        return -1;
    }

    printf("Deposit successful\n");

   return 0;
}


short withdrawFromAccount(user *details)
{
   if (debitDbAccount(details))
   {
       return -1;
   }

   printf("Withdrawal Successful\n");

   return 0;
}

short getAccountInfo(user * details)
{
    if (getdbAccountInfo(details))
    {
        return -1;
    }

    // readDBbyAccountNo(details);
    // if (creditDbAccount(details))
    // {
    //     return -1;
    // }
    // // debitDbAccount(details);
   
    // printf("account name: %s %s %s\n", details->fName, details->mName, details->lName);
    // printf("Account Balance: %ld\n", details->balance);

    return 0;
}


short closeAccount(user *details)
{

    if(deleteDbRecord(details))
    {
        return -1;
    }

    return 0;
}