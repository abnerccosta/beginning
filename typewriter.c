#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DICTIONARY_LENGTH   100
#define WORD_LENGTH          30

void check_misspelling(char typed_word[], char correct_word[]);
int erase_char(int letter_position, char typed_word[]);
void get_input(int position);
void get_results(void);
void initialize_dictionary(void);
void load_text(void);
void menu(void);
void reset_counters(void);
void show_text(void);
void typing_test(void);

char dictionary[DICTIONARY_LENGTH][WORD_LENGTH];
int wrong = 0, correct = 0, position = 0;
bool one_word = false, debug = false, random_order = false, ignore_case = false;

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        /* This is horrible (like the whole code), but I'm drunk 
           TODO: implement random */
        for (int i = 1; i < argc; i++)
        {
            if((strcmp(argv[i], "--one-word")) == 0)
                one_word = true;
            else if((strcmp(argv[i], "--debug")) == 0)
                debug = true;
            else if ((strcmp(argv[i], "--random")) == 0)
                random_order = true;
            else if ((strcmp(argv[i], "--ignore-case")) == 0)
                ignore_case = true;
            else
            {
                printf("Usage: %s <command>. [--debug] [--one-word] [--random] [--ignore-case]\n", argv[0]);

                exit(1);
            }
        }
    }

    menu();

    return 0;
}

void menu(void) {

    char choice;

    reset_counters();

    system("/bin/stty cooked");

    printf("Welcome to the typing test!\
    \n1.Start\
    \n2.Exit\n\n");

    choice = getchar();

    switch(choice)
    {
        case '1':
            typing_test();
            break;
        default:
            exit(0);
    }

    return;
}

void check_misspelling(char typed_word[], char correct_word[])
{
    char correct_word_formated[WORD_LENGTH];

    int i = 0, j = 0;

    if (debug)
    {
        char argument[100];

        sprintf(argument, "echo \"%s\" >> tw_debug.txt", typed_word);

        system(argument);
    }

    while (correct_word[i] != '\0')
    {
        if (correct_word[i] >= 65 && correct_word[i] <= 90 || correct_word[i] >= 97 && correct_word[i] <= 122)
        {
            correct_word_formated[j] = correct_word[i];
            j++;
        }
        
        i++;
    }

    correct_word_formated[j] = '\0';

    if (ignore_case == true)
    {
        // One line is great. Again, thx StackOverFlow
        for (char *letter = correct_word_formated; *letter; ++letter) *letter = tolower(*letter);
        
        for (char *letter = typed_word; *letter; ++letter) *letter = tolower(*letter);
    }
    
    if (strcmp(typed_word, correct_word_formated) == 0)
        correct++;
    else
        wrong++;

    return;
}

int erase_char(int letter_position, char typed_word[])
{
    // This is horrible, I know. Thx Reddit
    printf("\n");
    printf("\x1b[1F");
    printf("\x1b[2K");

    typed_word[letter_position - 1] = 0;

    printf("Text: ");

    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (typed_word[i] == 0)
            break;
        
        printf("%c", typed_word[i]);
    }

    // Just GOOD
    if (letter_position != 0)
        return letter_position - 1;
    else
        return letter_position;
}

void get_input(int position)
{
    int buffer, i = 0;
    char typed_word[WORD_LENGTH];

    printf("Text: ");

    while((buffer=getchar()) != ' ')
    {
        switch(buffer)
        {
            // ESC
            case 27:
                get_results();
                break;
            case 127:
                i = erase_char(i, typed_word);
                break;
        }

        if (buffer >= 65 && buffer <= 90 || buffer >= 97 && buffer <= 122)
        {
            typed_word[i] = buffer;
            i++;
        }
    }

    typed_word[i] = '\0';

    // Reset comparison position when list ends
    if(dictionary[position][0] == 0 || position >= 100)
        position = 0;

    check_misspelling(typed_word, dictionary[position]);

    return;
}

void get_results(void)
{
    printf("\n\nWrong: %i, Correct: %i\n\n", wrong, correct);

    // I said it was horrible. Fix this
    menu();
}

void initialize_dictionary(void)
{
    for (int i = 0; i < DICTIONARY_LENGTH; i++)
        dictionary[i][0] = 0;

    return;
}

void load_text(void)
{
    FILE *words = fopen("words.txt", "r");

    char buffer;
    int i = 0, j = 0;

    if (words == NULL)
    {
        printf("Cannot open words.txt!\n\n");

        exit(1);
    }

    while((buffer=fgetc(words)) != EOF)
    {
        if (buffer == ' ')
        {
            dictionary[i][j] = '\0';

            i++;
            j = 0;
        }
        
        dictionary[i][j] = buffer;
        j++;
    }

    fclose(words);

    return;
}

void reset_counters(void)
{
    wrong = 0, correct = 0, position = 0;

    return;
}

void show_text(void)
{
    if (one_word == false)
    {
        for (int i = 0; i < DICTIONARY_LENGTH; i++)
        {
            if (dictionary[i][0] == 0)
                break;
            
            printf("%s\t", dictionary[i]);
        }
    }
    else
    {
        printf("%s", dictionary[position]);
    }

    return;
}

void typing_test(void)
{
    initialize_dictionary();

    load_text();

    position = 0;

    time_t begin = clock(), end;

    while (true)
    {
        end = clock();

        if (((double) end - (double) begin) / 1000 > 30)
            break;

        system("clear");

        show_text();

        printf("\n\n");

        // Disable input treatment
        system("/bin/stty raw");

        get_input(position);

        // Set terminal back to normal
        system("/bin/stty cooked");

        position++;

        if (dictionary[position][0] == 0)
            position = 0;
    }

    get_results();

    return;
}