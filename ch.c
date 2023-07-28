#include <stdio.h>
#include <stdlib.h>

#define NUM_LIMIT  100

#ifdef __linux__
    int OS = 0;
#elif _WIN32_
    int OS = 1;
#endif

/* 
    ch - "carga horária", ver 0.1. Abner Carvalho Costa
    a sum calculator inspired by ed from UNIX 
*/

// avoids the compiler take node * as int *
typedef struct node node;

/* 
    define the structure that will loop until
    find the end of input
*/
typedef struct node
{
    int n;
    node *ptr;
}
node;

void caller(void);

void nup(node *buffer);

int sum(int *number, node *buffer);

int fn(node *buffer);

/* 
    see returns an int because of error handling.
    Maybe it should be aplied to other functions
*/
int see(node *buffer);

// stores total value
int y = 0;

// calculation history
node history;

int main(void)
{
    printf("Usage: type a number and press enter, they'll be stored.\
            \nTo see all commands type \"?\" and hit enter.\n");

    caller();

    return 0;
}

void caller(void)
{
    /* caller() is used to avoid text repetition */

    // represents numbers
    int x [NUM_LIMIT];

    // represents input itself
    char string[NUM_LIMIT];

        scanf("%s", string);

    if(*string == 'q')
    {
        fn(&history);
        
        exit(0);
    }
    else if (*string == 's')
    {
        see(&history);
    }
    else if (*string == 'c')
    {
        if(OS == 0)
            system("clear");
        else if(OS == 1)
            system("cls");
        else
            printf("Couldn't determine the OS\n");

        main();
    }
    else if (*string == '.')
    {
        // 'r' stands for reset
        if(history.ptr)
            fn(&history);

        *string = 'c';

        history.ptr = NULL;

        y = 0;

        caller();
    }
    else if (*string == '?')
    {
        printf("\n . - reset counting\
                \n c - clear the screen\
                \n s - see calculation history\
                \n q - quit the program\n");
        
        caller();
    }

    *x = atoi(string);

    // updates number history
    nup(&history);

    y += sum(x, &history);

    printf("%i\n", y);

    caller();
}

void nup(node *buffer)
{
    node *search = buffer;

    while (search->ptr != NULL)
    {
        search = search->ptr;
    }

    // adds a new node
    node *new = malloc(sizeof(node));
    new->ptr = NULL;
    search->ptr = new;

    return;
}

int sum(int *number, node *buffer)
{
    node *add = buffer;

    while (add->ptr != NULL)
    {
        add = add->ptr;
    }

    // adds the current typed number to the history
    add->n = *number;

    return add->n;
}

int see(node *buffer)
{
    node *sequence = buffer;

    printf("Calculation history: \n");

    while (sequence->ptr != NULL)
    {
        printf("%i\n", sequence->n);

        sequence = sequence->ptr;
    }

    // prints the last number
    printf("%i\n", sequence->n);

    // shows total sum
    printf("\nTotal: \n");

    return 0;
}

// free nodes
int fn(node *buffer)
{
    node *tmp;

    // cannot handle when pressing CTRL + C

    // the first node is invalid
    if(buffer->ptr != NULL)
        buffer = buffer->ptr;
    else
        return 0;

    // should check if memory was correctly freed
    while(buffer->ptr != NULL)
    {
        tmp = buffer->ptr;
        free(buffer);
        buffer = tmp;
    }

    free(buffer);

    return 0;
}