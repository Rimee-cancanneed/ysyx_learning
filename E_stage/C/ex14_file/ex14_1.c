#include <stdio.h>
#include <ctype.h>
#include <string.h>
// forward declaration
void print_letters(char arg[], size_t len);

void print_arguments(int argc, char *argv[])
{
    int i = 0;
    for(i = 0; i < argc; i++) {
        print_letters(argv[i], strlen(argv[i]));
    }
}

void print_letters(char arg[], size_t len)
{
    int i = 0;

    for(i = 0; i < len; i++) {
        char ch = arg[i];
        if (isalpha(arg[i]) || isblank(arg[i]))
        {
            printf("'%c' == %d ", ch, ch);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}