#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

#define FILE_NAME 'contacts.bin'

int main(int argc, char const *argv[])
{

    return 0;
}

bool addNewContact(char *filename, Contact *data)
{
    FILE *fptr;

    // open a file for writing to in binary
    fptr = fopen(filename, "wb");

    if (fptr == NULL)
    {
        perror('Error openning file');
        exit(EXIT_FAILURE);
    }

    // write the array of struct containing contacts to the binary file
    size_t content_written = fwrite(data, sizeof(Contact), 1, fptr);
    if (content_written != 1)
    {
        perror("Error writting to file");
        exit(EXIT_FAILURE);
    }

    if (fclose(fptr) == EOF)
    {
        perror("Erro closing file");
        exit((EXIT_FAILURE));
    };

    exit(EXIT_SUCCESS);
}
