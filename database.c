#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "database.h"

/**********************************************************************************************************************
 *
 * Function Name:
 *      addRecord
 *
 * Function Description:
 *      creates & adds a struct record of provided paramaters to a linkedlist of struct records, sorted in descending
 *      order of value of accountno.
 *
 * Function Paramaters:
 *      start (struct record **): pointer to struct record pointer pointing to the first record in the linkedlist.
 *                                ** in case first record must become new record.
 *
 *      uaccountno (int):         value to set record*->accountno to.
 *
 *      uname (char[25]):         value to set record*->name to.
 *
 *      uaddress(char[80]):       value to set record*->address to.
 *
 * Return Values:
 *      0: added record is unique
 *      1: added record has a duplicate ID
 *
 *********************************************************************************************************************/
int addRecord(struct record ** start, int uaccountno, char uname[25], char uaddress[80])
{
    int retval;
    struct record * newRec;
    struct record * currRec;
    struct record * nextRec;

    retval = 0;

    newRec = malloc(sizeof(struct record));
    newRec->accountno = uaccountno;
    strcpy(newRec->name, uname);
    strcpy(newRec->address, uaddress);
    newRec->next = NULL;

    if (*start == NULL)
    {
        *start = newRec;
    }
    else if (newRec->accountno > (*start)->accountno)
    {
        newRec->next = *start;
        *start = newRec;
    }
    else
    {
        currRec = *start;
        nextRec = currRec->next;

        while (nextRec != NULL && newRec->accountno < nextRec->accountno)
        {     
            currRec = nextRec;
            nextRec = currRec->next; 
        }
        if (currRec->accountno == uaccountno)
        {
            retval = 1;
        }
        currRec->next = newRec;
        newRec->next = nextRec;
    }
    return retval; 
}

/*****************************************************************************************************************
 *
 * Function Name:
 *      deleteRecord
 *
 * Function Description:
 *      Removes all struct records with a specified value for int accountno from a linkedlist of struct records.
 *
 * Function Paramaters:
 *      start (struct record **):  pointer to struct record pointer pointing to the first struct record in the
 *                                 linkedlist. ** in case first record is to be removed.
 *
 *      todelete:                  value to match record*->accountno to in order to determine deletion.
 *
 * Return Values:
 *      -1: no records deleted.
 *      0: 1 or more records deleted.
 *
 ****************************************************************************************************************/
int deleteRecord(struct record ** start, int todelete)
{
    int retval;
    struct record * prevRec;
    struct record * currRec;
    struct record * nextRec;

    retval = -1;

    if (*start != NULL)
    {
        prevRec = NULL; 
        currRec = *start;
        nextRec = currRec->next;

        /* traverses until account number matched or last record reached, last record not processed */
        while (currRec->accountno > todelete && nextRec != NULL)
        {
            prevRec = currRec;
            currRec = nextRec;
            nextRec = currRec->next;
        }

        /* traverses and frees until account number passed or last record reached, last record not processed */
        while (currRec->accountno == todelete && currRec != NULL && nextRec != NULL)
        {
            printf("%d\n", currRec->accountno);
            printf("%s\n", currRec->name);
            printf("%s\n", currRec->address);
            currRec->accountno = 123;
            printf("%d\n", currRec->accountno);
            printf("%p\n", (void *) (currRec));
            printf("%s\n", currRec->name);
            printf("%p\n", (void *) (currRec));
            free(currRec);
            prevRec->next = nextRec;
            currRec = nextRec;
            nextRec = currRec->next;

            retval = 0;
        }

        /* if last record reached (last record if nextRec == NULL, otherwise will always be first record w/ acct num
        * less than target acct # & always irrelevant) is relevant, processes it if it is*/
        if (currRec->accountno == todelete)
        {
            free(currRec);
            if (prevRec != NULL) /* prevRec = NULL only when first record is only record in list */ 
            {
                prevRec->next = nextRec;
            }
            else
            {
                *start = NULL;
            }

            retval = 0;
        }
    }
    
    return retval;
}

/*******************************************************************************************************************
 * 
 * Function Name:
 *      printAllRecords
 *
 * Function Description:
 *      prints the contents of every struct record from a linkedlist of struct records
 *
 * Function Paramaters:
 *      start (struct record *): pointer to the first struct record in the linkedlist
 *
 * Return Values:
 *      None.
 *
 ******************************************************************************************************************/
void printAllRecords(struct record * start)
{
    struct record * currRec;
    currRec = start;
    while (currRec != NULL)
    {

        printf("Account Name: %s\n", currRec->name);
        printf("Account Address: %s\n", currRec->address);
        printf("Account ID: %d\n", currRec->accountno);
        printf("\n");
       	currRec = currRec->next;
    }
}

/*********************************************************************************************************************
 *
 * Function Name:
 *      findRecord
 *
 * Function Description:
 *      searches a linkedlist of struct records for all records with an account number matching an account number
 *      provided by the user
 *
 * Function Paramaters:
 *      start (struct record *):        pointer to the head of the linkedlist
 *      targetaccount (int):            account number for which to search the linkedlist for matches
 *
 * Return Values:
 *      -1:     No matching records found
 *      0:      Matching record(s) found
 ********************************************************************************************************************/
int findRecord(struct record * start, int targetaccount)
{
    struct record * currRec;
    int retval;

    currRec = start;
    retval = -1;

    while (currRec != NULL && currRec->accountno > targetaccount)
    {
        currRec = currRec->next;
    }
    
    while (currRec != NULL && currRec->accountno == targetaccount)
    {
        printf("Account Name: %s\n", currRec->name);
        printf("Account Address: %s\n", currRec->address);
        printf("Account ID: %d\n", currRec->accountno);
        currRec = currRec->next;
        retval = 0;
    }

    return retval;
}

/***********************************************************************************************************************
 *
 * Function Name:
 *      readfile
 *
 * Function Description:
 *      reads a list of the contents of struct records in from a file with a provided name and converts them back
 *      into struct records in a linkedlist on the heap, using a pointer to a struct record pointer to set a struct
 *      record pointer outside of this function to point at the head of the linkedlist.
 *
 * Function Paramaters:
 *      start (struct record **):       pointer to a struct record pointer which is to be set to point to the head
 *                                      of the linkedlist
 *      filename (char[80]):            name of the file to be read from
 *
 *
 **********************************************************************************************************************/
int readfile (struct record ** start, char filename[80])
{
    int retval;
    FILE * fp;
    char buf[81];

    struct record * newRec;
    struct record * nextRec;
    int newacctno;
    char newname[25];
    char newaddress[80];
    int i;

    retval = 0;


    fp = fopen(filename, "r");
    
    fgetc(fp);
    if (!feof(fp))
    {
        fseek(fp, -1, SEEK_CUR);
        newRec = malloc(sizeof(struct record));
        *start = newRec;
    }
    else
    {
        *start = NULL;
    }
    while (!feof(fp))
    {
        /* Delineator */
        fgets(buf, 6, fp);

        /* ID */
        fscanf(fp, "%d", &newacctno);
        fgetc(fp);

        /* Name */
        fgets(buf, 26, fp);
        strncpy(newname, buf, 25);
        
        i = 0;
        while(newname[i] != '\0' && newname[i] != '\n')
        {
            i++;
        }
        newname[i] = '\0';

        /* Address */
        fgets(buf, 81, fp);
        strncpy(newaddress, buf, 80);        

        i = 0;
        while (newaddress[i] != '\0' && newaddress[i] != '\n')
        {
            i++;
        }
        newaddress[i] = '\0';

        /* Next */
        fgetc(fp);
        if (feof(fp))
        {
            nextRec = NULL;
        }
        else
        {
            fseek(fp, -1, SEEK_CUR);
            nextRec = malloc(sizeof(struct record));
        }
        
        newRec->accountno = newacctno;
        strcpy(newRec->name, newname);
        strcpy(newRec->address, newaddress);
        newRec->next = nextRec;
        newRec = nextRec;       
    }
    
    return retval;
}

/**********************************************************************************
 *
 * Function Name:
 *      writefile
 * 
 * Function Description:
 *      opens a file pointer to a file w/ a name passed as a paramter to writing,
 *      and writes the contents of a struct record linkedlist to it.
 *
 * Function Paramaters:
 *      start (struct record *):        Pointer to the first struct record in
 *                                      the linkedlist
 *      filename (char[80]):            name of the file to open to writing
 *
 * Return Values:
 *      1:      Function ran succesfully
 *********************************************************************************/
int writefile (struct record * start, char filename[80])
{
    int retval;
    FILE * fp;
    struct record * currRec;

    retval = 0;
    fp = fopen(filename, "w");
    currRec = start;

    while (currRec != NULL)
    {
        fprintf(fp, "-----\n");
        fprintf(fp, "%d\n", currRec->accountno);
        fprintf(fp, "%s\n", currRec->name);
        fprintf(fp, "%s\n", currRec->address);
        currRec = currRec->next;
    }

    return retval;
}

/*************************************************************************
 *
  * Function Name: 
 *      cleanup
 *
 * Function Description:	
 *      frees memory from the heap containing a linkedlist of record
 *      structures.
 *
 * Function Paramaters:
 *      Start:  A pointer to a record pointer containing the address
 *      of the first record in the linkedlist on the heap
 *
 * Return Values:
 *      None.
 *
************************************************************************/
void cleanup (struct record ** start)
{	
    struct record * currRec;
    struct record * nextRec;

    if (*start != NULL)
    {
        currRec = *start;
        nextRec = currRec -> next;

        while (nextRec != NULL)
        {
            free(currRec);
            currRec = nextRec;
            nextRec = nextRec->next;
        }
        free(currRec);
    }
}
