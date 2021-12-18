#include <stdio.h>
#include <string.h>
#include "record.h"
#include "database.h"

int add(struct record **);
void delete(struct record **);
void find(struct record *);
int quit(struct record **);

int main (int argc, char * argv[])
{
    struct record * start;
    int doneLooping;
    char buf[10];
    int len;

    printf("\n");
    printf("+-------------------------------------------+\n");
    printf("|Welcome to The Bank(TM)'s account database!|\n");
    printf("+-------------------------------------------+\n\n");
    readfile(&start, "database.txt");
    doneLooping = 0;
    
    while (doneLooping != 1)
    {
        printf("+-----------------------------------------------+\n");
        printf("|Please select an operation you wish to perform:|\n");
        printf("|    1. (add) a new record                      |\n");
        printf("|    2. (printall) records                      |\n");
        printf("|    3. (find) and print a specific record      |\n");
        printf("|    4. (delete) an existing record             |\n");
        printf("|    5. (quit) this program                     |\n");
        printf("+-----------------------------------------------+\n");
        printf("Operation: ");

        fgets(buf, 10, stdin);
        printf("\n");

        len = 0;
        while (buf[len] != '\n' && buf[len] != '\0')
        {
            len++;
        }
        if (buf[len] == '\n')
        {
            buf[len] = '\0';
        }
        else
        {
            while (fgetc(stdin) != '\n');
        }

        if (len == 0)
        {
            printf("Error: Please enter an operation! Operations shown in parentheses.\n");
        }
        else if (strncmp(buf, "add", len) == 0)
        {
            add(&start);
        }
        else if (strncmp(buf, "printall", len) == 0)
        {
            if (start == NULL)
            {
                printf("Error: No records found in database. Please populate database before attempting to read from it.\n");
            }
            printAllRecords(start);
        }
        else if (strncmp(buf, "find", len) == 0)
        {
            find(start);
        }
        else if (strncmp(buf, "delete", len) == 0)
        {
            delete(&start);
        }
        else if (strncmp(buf, "quit", len) == 0)
        {
            quit(&start);
            doneLooping = 1;
        }
        else
        {
            printf("Error: Please enter a valid operation! Operations shown in parentheses.\n");
        }
    }

    return 0;
}

int add(struct record ** start)
{
    int newacctno;
    int i;
    int retval;
    int valid;
    char newname[25];
    char newaddress[80];
    char firstChar;

    valid = 0;

    while (valid == 0)
    {
        /* resetting sentinel value */
        newacctno = -9876;

        printf("Please type the account number of the account you wish to add: ");

        /* causes code to wait for an input using fgetc instead of scanf. User still pushes whole line in at once though,
           and as such nothing changes from the user's PoV. if the first char is a newline (ie, user entered only newline)
           stdin is populated with f\n instead of just \n. Necessary because scanf doesn't properly understand that
           there's nothing for it to read in cases where it's reading a buffer containing just a newline for unknown reasons,
           therefore another character must be there as well. */       
        firstChar = fgetc(stdin);
        if (firstChar == '\n')
        {
            ungetc('\n', stdin);
            ungetc('f', stdin);
        }
        else
        {
            ungetc(firstChar, stdin);
        }

        scanf("%d", &newacctno);

        /* empty stdin, guaranteed 1 \n */
        while (fgetc(stdin) != '\n');

        /* test for non-int input */
        if (newacctno == -9876)
        {
            printf("Error: please enter an integer value for account number.\n");
        }

        /* test for negative int input */
        else if (newacctno <= 0)
        {
            printf("Error: please enter a positive integer value for account number.\n");
        }

        /* if positive int inputted */
        else
        {
            valid = 1;
        }
    }

    printf("Please type the name tied to the account you wish to add: ");

    fgets(newname, 25, stdin);
    i = 0;
    while (newname[i] != '\n' && newname[i] != '\0')
    {
        i++;
    }
    if (newname[i] == '\n')
    {
        newname[i] = '\0';
    }
    else
    {
        while (fgetc(stdin) != '\n');
    }

    printf("Please type the address tied to the account you wish to add: ");

    fgets(newaddress, 80, stdin);
 
    i = 0;
    while (newaddress[i] != '\0' && newaddress[i] != '\n')
    {
        i++;
    }

    if (newaddress[i] == '\n')
    {
        newaddress[i] = '\0';
    }
    else
    {
        while (fgetc(stdin) != '\n');
    }

    printf("\n");

    retval = addRecord(start, newacctno, newname, newaddress);
    return retval;
}

void find(struct record * start)
{
    int targetacct;
    int recordFound;    
    int valid;
    char firstChar;

    valid = 0;
    while (valid == 0)
    {
        targetacct = -9876;

        printf("Please type the account number of the account you wish to find: ");
        
        firstChar = fgetc(stdin);
        if (firstChar != '\n')
        {
            ungetc(firstChar, stdin);
        }
        else
        {
            ungetc('\n', stdin);
            ungetc('f', stdin);
        }

        scanf("%d", &targetacct);

        while (fgetc(stdin) != '\n');
        
        if (targetacct == -9876)
        {
            printf("Error: please enter an integer account number\n");
        }
        else if (targetacct <= 0)
        {
            printf("Error: please enter a positive integer account number\n");
        }
        else
        {
            valid = 1;
        }
    }

    recordFound = findRecord(start, targetacct);
    
    if (recordFound != 0)
    {
        printf("Error: No record found with matching account ID.\n");
    }
}

void delete(struct record ** start)
{
    int targetacct;
    int recFound;
    int valid;
    char firstChar;

    valid = 0;
    while (valid == 0)
    {
        targetacct = -9876;

        printf("Please type the account number of the account you wish to delete: ");

        firstChar = fgetc(stdin);
        if (firstChar == '\n')
        {
            ungetc('\n', stdin);
            ungetc('f', stdin);
        }
        else
        {
            ungetc(firstChar, stdin);
        }

        scanf("%d", &targetacct);

        while (fgetc(stdin) != '\n');

        if (targetacct == -9876)
        {
            printf("Error: please enter an integer account number\n");
        }
        else if (targetacct <= 0)
        {
            printf("Error: please enter a positive integer account number\n");
        }
        else
        {
            valid = 1;
        }
    }

    recFound = deleteRecord(start, targetacct);
    
    if (recFound != 0)
    {
        printf("Error: No record found with specified account ID.\n");
    }
}

int quit(struct record ** start)
{
    int retval;
    retval = writefile(*start, "database.txt");
    cleanup(start);
    return retval;
}
