// #include <stdbool.h>

#ifndef CONTACT_H

#define CONTACT_H

#define MAX_NAME_LEN 100
#define MAX_PHONE_LEN 30
#define MAX_ADDRESS_LEN 200
#define MAX_EMAIL_LEN 100

typedef struct
{
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char address[MAX_ADDRESS_LEN];
    char email[MAX_EMAIL_LEN];

} Contact;

void addNewContact();
Contact *retrieveContacts(char *filename, int *total);
void updateContact();
void deleteContact();
void printContacts();
void saveContact();
void searchByName();
void searchByNumber();
void deleteAllContact();

void runCommand(int command);
void printCommandsAndUsage();
void runApp();
void runCommand(int command);
void printContacts();
bool fileExists(char *filename);
void clearInputBuffer();
bool numberExists(char *num);
bool isEmpty(const char *str);
bool isDigitsOnly(const char *str);
int getContactIndex(char *str);
void getIntegerFromStdin(int *inputInteger);
void toLowerCase(char *str);
void clearTerminal();
void exitApp();

#endif // CONTACT
