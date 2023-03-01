#include <string.h>
#include <time.h>
#include "map.h"

#ifdef _WIN32
    int os = 0;
#else
    int os = 1;
#endif

#define MAP_LENGTH 5
#define FILE_LENGTH 15918
#define WORD_LENGTH 6
#define GUESS_LENGTH 50
#define ATTEMPS 6

Map *wordmap;
Map *guessmap;
char **attempts;
int tries = 0;
char* word;

void cls() {
    if (os) system("clear");
    else system("cls");
}

void yellow() {
    printf("\033[0;33m");
}

void green() {
    printf("\033[0;32m");
}

void white() {
    printf("\033[0;37m");
}

void bold_white() {
    printf("\033[1;37m");
}

void read_word() {
    FILE* ptr;
    word = malloc(WORD_LENGTH * sizeof(char));
    srand(time(NULL));
    int r = rand() % FILE_LENGTH;
    int i = 0;
    ptr = fopen(".\\words.txt", "r");
    do {
        fgets(word, WORD_LENGTH + 1, ptr);
        i++;
    } while (i < r);
    fclose(ptr);
}

void init_map() {
    for (size_t i = 0; i < WORD_LENGTH - 1; i++) {
        char *current_char = malloc(sizeof(char));
        int *value = malloc(sizeof(int));
        *current_char = word[i];
        *value = 0;
        *value = (*(int*)mapgetordefault(wordmap, current_char, value)) + 1;
        mapput(wordmap, current_char, value);
    }
}

void print_attempts() {
    for (int i = 0; i < ATTEMPS; i++) {
        mapclear(guessmap);
        for (int j = 0; j < WORD_LENGTH - 1; j++) {
            if (tries > i) {
                if (attempts[i][j] == word[j]) green();
                else if (mapget(wordmap, &attempts[i][j]) != NULL) {
                    int* value = malloc(sizeof(int));
                    *value = 0;
                    *value = (*(int*)mapgetordefault(guessmap, &attempts[i][j], value)) + 1;
                    mapput(guessmap, &attempts[i][j], value);
                    if (*(int*)mapget(guessmap, &attempts[i][j]) <= *(int*)mapget(wordmap, &attempts[i][j])) yellow();
                }
            }
            printf("[");
            if (tries > i) printf("%c", attempts[i][j]);
            else printf(" ");
            printf("]");
            white();
        }
        printf("\n");
    }
}

void run_game() {
    int allocated = 0;
    if (!allocated) {
        attempts = malloc(ATTEMPS * sizeof(char*));
        allocated = 1;
    }
    while (tries <= ATTEMPS) {
        int attempted = 0;
        char* attempt;
        if (!attempted) {
            attempt = malloc(GUESS_LENGTH * sizeof(char));
            attempted = 1;
        }
        cls();
        printf("%s\n", word);
        print_attempts();
        if (tries != ATTEMPS) {
            printf("Enter your next guess: ");
            gets(attempt);
            while (strlen(attempt) != WORD_LENGTH - 1) {
                printf("Invalid length of word. Enter your next guess: ");
                gets(attempt);
            }
        } else printf("Thanks for playing");
        attempts[tries] = attempt;
        tries++;
    }
}

int main() {
    wordmap = mapcreate(CHARACTER, INTEGER);
    guessmap = mapcreate(CHARACTER, INTEGER);
    read_word();
    init_map();
    cls();
    run_game();
    mapfree(wordmap);
    mapfree(guessmap);
    return 0;
}