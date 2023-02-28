#include <string.h>
#include <time.h>
#include "map.h"

#define MAP_LENGTH 5
#define FILE_LENGTH 15918
#define WORD_LENGTH 6
#define GUESS_LENGTH 50
#define ATTEMPS 6

char **attempts;
int tries = 0;

char* read_word() {
    FILE* ptr;
    char *word = malloc(WORD_LENGTH * sizeof(char));
    srand(time(NULL));
    int r = rand() % FILE_LENGTH;
    int i = 0;
    ptr = fopen(".\\words.txt", "r");
    do {
        fgets(word, WORD_LENGTH + 1, ptr);
        i++;
    } while (i < r);
    fclose(ptr);
    return word;
}

char* init_map(Map* map) {
    char *word = read_word();
    for (size_t i = 0; i < WORD_LENGTH - 1; i++) {
        char *current_char = malloc(sizeof(char));
        int *value = malloc(sizeof(int));
        *current_char = word[i];
        *value = 0;
        *value = (*(int*)mapgetordefault(map, current_char, value)) + 1;
        mapput(map, current_char, value);
    }
    return word;
}

void print_attempts(Map* map) {
    for (int i = 0; i < ATTEMPS; i++) {
        for (int j = 0; j < WORD_LENGTH - 1; j++) {
            printf("[");
            if (tries > i) printf("%c", attempts[i][j]);
            else printf(" ");
            printf("]");
        }
        printf("\n");
    }
}

void run_game(Map *map) {
    static int allocated = 0;
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
        printf("\e[1;1H\e[2J");
        print_attempts(map);
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
    Map *map = mapcreate(CHARACTER, INTEGER);
    init_map(map);
    printf("\e[1;1H\e[2J");
    run_game(map);
    mapfree(map);
    return 0;
}