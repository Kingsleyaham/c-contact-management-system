/**
 * @file main.c
 * Project Title: Phonebook Management System
 * @brief A phonebook management system that helps individual to manage their contacts
 * @ref https://github.com/Kingsleyaham/c-contact-management-system
 * @author KINGSLEY AHAM
 * @version 0.1
 * @date 2024-02-13
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h> // defines the integer varibale errno
#include "main.h"

#define _GNU_SOURCE // to get the strcasestr function
// define constants
#define FILE_NAME "contacts.bin"
// #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr[0])))
#define MAX_CONTACT_CAPACITY 1000
#define BUFFER_SIZE 100
#define MAX_DIGITS 5

Contact contacts[MAX_CONTACT_CAPACITY];
int currentLen = 0;

int main(int argc, char const *argv[])
{

    if (fileExists(FILE_NAME))
    {

        Contact *contact = retrieveContacts(FILE_NAME, &currentLen);

        if (contacts == NULL)
        {
            perror("Error reading from file");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < currentLen; i++)
        {
            strcpy(contacts[i].name, contact[i].name);
            strcpy(contacts[i].phone, contact[i].phone);
            strcpy(contacts[i].email, contact[i].email);
            strcpy(contacts[i].address, contact[i].address);
        }

        free(contact);
    }

    printCommandsAndUsage();
    runApp();

    return 0;
}

void runApp()
{

    int choice;
    // get the command from which an action will be performed from user
    printf("Enter command to run or enter [8] to show list of commands: ");
    scanf("%d", &choice);

    clearInputBuffer(); // flush pending input

    // fgets(command, BUFFER_SIZE, stdin);
    while (1)
    {
        runCommand(choice);

        printf("Enter command to run or enter [8] to show list of commands: ");
        scanf("%d", &choice);

        clearInputBuffer(); // flush pending input
    }
}

void runCommand(int command)
{
    switch (command)
    {
    case 1:
        addNewContact();
        break;

    case 2:
        updateContact();
        break;

    case 3:
        printContacts();
        break;
    case 4:
        searchByNumber();
        break;
    case 5:
        searchByName();
        break;
    case 6:
        deleteContact();
        break;
    case 7:
        deleteAllContact();
        break;
    case 8:
        printCommandsAndUsage();
        break;
    case 9:
        clearTerminal();
        break;
    case 0:
        exitApp();
        break;

    default:
        printf("please enter a valid command\n");
        break;
    }
}

/**
 * @brief Add new contact to phonebook
 *
 */
void addNewContact()
{
    char nameInput[MAX_NAME_LEN];
    char phoneInput[MAX_PHONE_LEN];
    char emailInput[MAX_EMAIL_LEN];
    char addressInput[MAX_ADDRESS_LEN];

    printf("\nFill the details below to create a new contact\n");

    printf("Enter name: ");
    fgets(nameInput, MAX_NAME_LEN, stdin);

    printf("Enter phone number [+234]: ");
    fgets(phoneInput, MAX_PHONE_LEN, stdin);

    printf("Enter email: ");
    fgets(emailInput, MAX_EMAIL_LEN, stdin);

    printf("Enter address:");
    fgets(addressInput, MAX_ADDRESS_LEN, stdin);

    if (nameInput == NULL || addressInput == NULL || emailInput == NULL || phoneInput == NULL)
    {
        perror("Error occurred collecting user input");
        exit(EXIT_FAILURE);
    }

    // convert name input and address input to lowercase
    toLowerCase(nameInput);
    toLowerCase(addressInput);

    strcpy(contacts[currentLen].name, nameInput);
    strcpy(contacts[currentLen].phone, phoneInput);
    strcpy(contacts[currentLen].email, emailInput);
    strcpy(contacts[currentLen].address, addressInput);

    currentLen += 1;

    saveContact();

    printf("\n----------------------------------------\n");
    printf("contact added succesfully\n");
    printf("----------------------------------------\n");
}

/**
 * @brief Fetch all contacts from phonebook
 *
 * @param filename
 * @param total
 * @return Contact*
 */
Contact *retrieveContacts(char *filename, int *total)
{
    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fread(total, sizeof(int), 1, fptr) != 1)
        exit(EXIT_FAILURE);

    Contact *data = malloc(sizeof(Contact) * *total);

    if (fread(data, sizeof(Contact), *total, fptr) != *total)
    {
        perror("Error reading file");
        free(data);
        exit(EXIT_FAILURE);
    }

    if (fclose(fptr) == EOF)
    {
        free(data);
        exit(EXIT_FAILURE);
    }

    return data;
}

/**
 * @brief print lists of commands for phonebook
 *
 */
void printCommandsAndUsage()
{
    printf("\t\t\t-----------------------------------------\n");
    printf("\t\t\t\tPhonebook Management System \n");
    printf("\t\t\t-----------------------------------------\n\n");
    printf("\t\tA command line application that allows users to manage their contacts\n\n");

    printf("[1] Add new Contact. \n");
    printf("[2] Update Contact. \n");
    printf("[3] Display All Contact. \n");
    printf("[4] Search By Number. \n");
    printf("[5] Search By Name. \n");
    printf("[6] Delete Contact. \n");
    printf("[7] Delete All Contact. \n");
    printf("[8] HELP. show lists of commands. \n");
    printf("[9] Clear Terminal. \n");
    printf("[0] EXIT \n");

    printf("\n\n");
}

void printContacts()
{
    if (currentLen == 0)
    {
        printf("\t\t-------------------------------------------------------\n");
        printf("\t\t\tYou currently do not have any contact in your phonebook\n");
        printf("\t\t-------------------------------------------------------\n");
        return;
    }

    printf("\n\t\t+-------------------------------------------------------------------------+\n");
    printf("\t\t\t\t\t\tLIST OF CONTACTS\n");
    printf("\t\t+-------------------------------------------------------------------------+\n");
    printf("\n");

    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\t%-3s | %-14s\t | %-12s\t | %-20s\t | %-35s\n", "S/N", "Name", "Phone No ", "Email Address", "Address");
    printf("\t-----------------------------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < currentLen; i++)
    {

        // remove newline character from strings
        contacts[i].name[strcspn(contacts[i].name, "\n")] = 0;
        contacts[i].phone[strcspn(contacts[i].phone, "\n")] = 0;
        contacts[i].address[strcspn(contacts[i].address, "\n")] = 0;
        contacts[i].email[strcspn(contacts[i].email, "\n")] = 0;

        printf("\t%-3lu | %-14s\t | %-12s\t | %-20s\t | %-35s\n", (i + 1), contacts[i].name, contacts[i].phone, contacts[i].email, contacts[i].address);
    }
    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

void saveContact()
{

    FILE *fptr;

    // open a file for writing to in binary
    fptr = fopen(FILE_NAME, "wb");

    if (fptr == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fwrite(&currentLen, sizeof(int), 1, fptr) != 1)
    {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    // write the array of struct containing contacts to the binary file
    size_t content_written = fwrite(contacts, sizeof(Contact), currentLen, fptr);

    if (content_written != currentLen)
    {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    if (fclose(fptr) == EOF)
    {
        perror("Erro closing file");
        exit((EXIT_FAILURE));
    };
}

void updateContact()
{
    int contactId;

    printf("Please enter ID of contact to edit: ");
    getIntegerFromStdin(&contactId);

    if (contactId < 1 || contactId > currentLen)
    {
        printf("Please enter a valid contact Id\n");
        updateContact();
        return;
    }

    int index = contactId - 1;

    for (size_t i = 0; i < currentLen; i++)
    {
        if (index == i)
        {
            char nameInput[MAX_NAME_LEN];
            char phoneInput[MAX_PHONE_LEN];
            char emailInput[MAX_EMAIL_LEN];
            char addressInput[MAX_ADDRESS_LEN];

            printf("Enter new name [%s] or press enter to use existing: ", contacts[i].name);
            fgets(nameInput, MAX_NAME_LEN, stdin);

            printf("Enter new phone number [+234 %s] or press enter to use existing: ", contacts[i].phone);
            fgets(phoneInput, MAX_PHONE_LEN, stdin);

            printf("Enter new email [%s] or press enter to use existing: ", contacts[i].email);
            fgets(emailInput, MAX_EMAIL_LEN, stdin);

            printf("Enter new address [%s] or press enter to use existing: ", contacts[i].address);
            fgets(addressInput, MAX_ADDRESS_LEN, stdin);

            if (nameInput == NULL || addressInput == NULL || emailInput == NULL || phoneInput == NULL)
            {
                perror("Error occurred collecting user input");
                exit(EXIT_FAILURE);
            }

            if (!(nameInput[0] == '\n'))
                strcpy(contacts[i].name, nameInput);

            if (!(phoneInput[0] == '\n'))
                strcpy(contacts[i].phone, phoneInput);

            if (!(emailInput[0] == '\n'))
                strcpy(contacts[i].email, emailInput);

            if (!(addressInput[0] == '\n'))
                strcpy(contacts[i].address, addressInput);

            saveContact();

            printf("\n----------------------------------------\n");
            printf("contact updated successfully\n");
            printf("----------------------------------------\n");

            return;
        }
    }

    printf("An error occured updating contact");
}

/**
 * @brief Search a contact by name
 *
 */
void searchByName()
{
    char search[MAX_NAME_LEN];
    printf("Please enter name of contact to search: ");
    fgets(search, BUFFER_SIZE, stdin);

    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\t %-14s\t | %-12s\t | %-20s\t | %-35s\n", "Name", "Phone No ", "Email Address", "Address");
    printf("\t-----------------------------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < currentLen; i++)
    {
        toLowerCase(search);               // convert search input to lowercase
        search[strcspn(search, "\n")] = 0; // remove newline character from input

        char contactName[MAX_NAME_LEN];
        strcpy(contactName, contacts[i].name);

        if (!(strstr(contactName, search) == NULL))
        {
            printf("\t%-14s\t | %-12s\t | %-20s\t | %-35s\n", contacts[i].name, contacts[i].phone, contacts[i].email, contacts[i].address);
        }
    }

    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

/**
 * @brief Search a contact by phone nummber
 *
 */
void searchByNumber()
{
    char search[MAX_PHONE_LEN];
    printf("Please phone number to search in phonebook: ");
    fgets(search, MAX_PHONE_LEN, stdin);

    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\t %-14s\t | %-12s\t | %-20s\t | %-35s\n", "Name", "Phone No ", "Email Address", "Address");
    printf("\t-----------------------------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < currentLen; i++)
    {
        char phoneNum[MAX_PHONE_LEN];

        strcpy(phoneNum, contacts[i].phone);

        // remove newline character from input
        search[strcspn(search, "\n")] = 0;
        phoneNum[strcspn(phoneNum, "\n")] = 0;

        if (strcmp(search, phoneNum) == 0)
        {
            printf("\t%-14s\t | %-12s\t | %-20s\t | %-35s\n", contacts[i].name, contacts[i].phone, contacts[i].email, contacts[i].address);
        }
    }

    printf("\t-----------------------------------------------------------------------------------------------------\n");
    printf("\n");
}

/**
 * @brief delete a given contact using contact Id
 *
 */
void deleteContact()
{

    int contactId;

    printf("Please enter ID of contact to delete: ");
    getIntegerFromStdin(&contactId);

    if (contactId < 1 || contactId > currentLen)
    {
        printf("Please enter a valid contact Id\n");
        deleteContact();
        return;
    }

    int index = contactId - 1;

    for (size_t i = index; i < (currentLen - 1); i++)
    {
        contacts[i] = contacts[i + 1];
    }

    // decrement the current length of the array by 1
    currentLen -= 1;

    saveContact();

    printf("\n--------------------------------------------\n");
    printf("contact with Id %d deleted successfully\n", contactId);
    printf("--------------------------------------------\n");
}

/**
 * @brief deletes every contacts in the phonebook
 *
 */
void deleteAllContact()
{

    if (fileExists(FILE_NAME))
    {
        if (remove(FILE_NAME) == 0)
        {
            memset(contacts, 0, sizeof(contacts));
            currentLen = 0;

            printf("\n--------------------------------------------\n");
            printf("Contacts deleted successfully\n");
            printf("--------------------------------------------\n");

            return;
        }

        perror("Error  deleting contact");
    }
}

/**
 * @brief Checks if a file exists in the current working directory
 *
 * @param filename
 * @return true
 * @return false
 */
bool fileExists(char *filename)
{
    if (access(filename, F_OK) == 0)
        return true;

    return false;
}

/**
 * @brief Clears buffer overflow that occurs after collect input from user using scanf
 *
 */
void clearInputBuffer()
{
    char c = 0;
    // Loop over input buffer and consume chars until buffer is empty
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * Check if contact already exist in phonebook
 */
bool numberExists(const char *number) {}

/**
 * @brief Checks if a string is empty
 */
bool isEmpty(const char *str)
{
    return (str[0] == '\n');
}

/**
 * @brief Get an integer From Stdin stream
 *
 * @param inputInteger
 */
void getIntegerFromStdin(int *inputInteger)
{
    char *inputBuffer = malloc(sizeof(char) * MAX_DIGITS);
    memset(inputBuffer, 0, MAX_DIGITS);

    char *input = NULL;

    while (input == NULL)
    {

        // Note that fgets returns inputBuffer on success - free either `input` or
        // `inputBuffer` to avoid an attempted double-free error.

        input = fgets(inputBuffer, MAX_DIGITS, stdin);

        if (inputBuffer[strlen(inputBuffer) - 1] != '\n')
        {
            printf("Maximum input length is %d digits\n", (MAX_DIGITS - 1));
            clearInputBuffer();
            input = NULL;

            return;
        }

        // convert input to integer using `strtol()`
        errno = 0;
        char *endptr = NULL;
        *inputInteger = strtol(input, &endptr, 10);

        if (input == endptr)
        {
            printf("Input not a valid integer\n");

            input = NULL;

            return;
        }

        if (errno != 0)
        {
            printf("Invalid input\n");
            input = NULL;

            return;
        }
    }

    free(inputBuffer); // free the memory allocated in the heap4fb
}

/**
 * @brief Checks if a given string contains only numbers
 *
 * @param str
 * @return true
 * @return false
 */
bool isDigitsOnly(const char *str)
{
    if (isEmpty(str))
        return false;

    while (*str)
    {
        if (isdigit(*str++) == 0)
            return false;
    }

    return true;
}

/**
 * @brief Get the Index on a contact in contacts array
 *
 * @param str
 * @return int
 */
int getContactIndex(char *str)
{
    for (size_t i = 0; i < currentLen; i++)
    {
        if (isDigitsOnly(str))
        {
            if (strcmp(contacts[i].phone, str) == 0)

                return i;

            return -1;
        }
    }

    printf("Invalid number");
    return -1;
}

/**
 * @brief Covert a text to lowercase
 *
 * @param str
 */
void toLowerCase(char *str)
{
    for (size_t i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

/**
 * @brief Clear the terminal
 *
 */

void clearTerminal()
{
    // checking for windows 32 & 64bit system
#ifdef _WIN32
    system("cls");

    // Checking for linux OS with
#elif __linux__
    system("clear");

#endif
}

void exitApp()
{
    exit(EXIT_FAILURE);
}
