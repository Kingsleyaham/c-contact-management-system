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

bool addNewContact(char *filename, Contact *data, int total);
void searchContact();
void editContact(Contact *data, Contact *newUpdate);
void removeContact();
void printContacts();
void runCommand();

#endif // CONTACT
