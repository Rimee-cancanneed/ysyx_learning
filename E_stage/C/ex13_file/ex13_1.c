#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("ERROR: You need one argument.\n");
        // this is how you abort a program
        return 1;
    }

    int i = 0;
    for(i = 0; argv[1][i] != '\0'; i++) {
        char letter = argv[1][i] + 32; // convert to lowercase

        switch(letter) {
            case 'a':
                printf("%d: 'a'\n", i);
                break;
            case 'b':
                printf("%d: 'b'\n", i);
                break;
            case 'c':
                printf("%d: 'c'\n", i);
                break;
            case 'd':
                printf("%d: 'd'\n", i);
                break;
            case 'e':
                printf("%d: 'e'\n", i);
                break;
            case 'f':
                printf("%d: 'f'\n", i);
                break;
            case 'g':
                printf("%d: 'g'\n", i);
                break;
            case 'h':
                printf("%d: 'h'\n", i);
                break;
            case 'i':
                printf("%d: 'i'\n", i);
                break;
            case 'j':
                printf("%d: 'j'\n", i);
                break;
            case 'k':
                printf("%d: 'k'\n", i);
                break;
            case 'l':
                printf("%d: 'l'\n", i);
                break;
            case 'm':
                printf("%d: 'm'\n", i);
                break;
            case 'n':
                printf("%d: 'n'\n", i);
                break;
            case 'o':
                printf("%d: 'o'\n", i);
                break;
            case 'p':
                printf("%d: 'p'\n", i);
                break;
            case 'q':
                printf("%d: 'q'\n", i);
                break;
            case 'r':
                printf("%d: 'r'\n", i);
                break;
            case 's':
                printf("%d: 's'\n", i);
                break;
            case 't':
                printf("%d: 't'\n", i);
                break;
            case 'u':
                printf("%d: 'u'\n", i);
                break;
            case 'v':
                printf("%d: 'v'\n", i);
                break;
            case 'w':
                printf("%d: 'w'\n", i);
                break;
            case 'x':
                printf("%d: 'x'\n", i);
                break;
            case 'y':
                printf("%d: 'y'\n", i);
                break;
            case 'z':
                printf("%d: 'z'\n", i);
                break;
            default:
                printf("%d: %c is not a vowel\n", i, letter);
        }
    }

    return 0;
}