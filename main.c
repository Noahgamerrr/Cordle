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
#define ALPHABET_LENGTH 26

Map *wordmap = NULL;
Map *guessmap = NULL;
Map *completemap = NULL;
char **attempts = NULL;
char* word = NULL;
char lettermap[ALPHABET_LENGTH];
int lettercolour[ALPHABET_LENGTH];
int tries = 0;
int haswon = 0;

void cls() {
    if (os) system("clear");
    else system("cls");
}

void freeattempts() {
    for (int i = 0 ; i < ATTEMPS; i++) {
        free(attempts[i]);
        attempts[i] = NULL;
    }
    free(attempts);
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

void init_wordmap() {
    for (size_t i = 0; i < WORD_LENGTH - 1; i++) {
        char *current_char = malloc(sizeof(char));
        int *value = malloc(sizeof(int));
        *current_char = word[i];
        *value = 0;
        *value = (*(int*)mapgetordefault(wordmap, current_char, value)) + 1;
        mapput(wordmap, current_char, value);
    }
}

void draw_colour(int idx) {
    switch (lettercolour[idx]) {
        case 1:
            bold_white();
            break;
        case 2:
            yellow();
            break;
        case 3:
            green();
            break;
        default:
            white();
            break;
    }
}

int get_qwerty_letter(char letter) {
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (letter == lettermap[i]) return i;
    }
    return -1;
}

void init_lettermap() {
    lettermap[0] = 'q';
    lettermap[1] = 'w';
    lettermap[2] = 'e';
    lettermap[3] = 'r';
    lettermap[4] = 't';
    lettermap[5] = 'y';
    lettermap[6] = 'u';
    lettermap[7] = 'i';
    lettermap[8] = 'o';
    lettermap[9] = 'p';
    lettermap[10] = 'a';
    lettermap[11] = 's';
    lettermap[12] = 'd';
    lettermap[13] = 'f';
    lettermap[14] = 'g';
    lettermap[15] = 'h';
    lettermap[16] = 'j';
    lettermap[17] = 'k';
    lettermap[18] = 'l';
    lettermap[19] = 'z';
    lettermap[20] = 'x';
    lettermap[21] = 'c';
    lettermap[22] = 'v';
    lettermap[23] = 'b';
    lettermap[24] = 'n';
    lettermap[25] = 'm';
}

void print_keyboard() {
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        draw_colour(i);
        printf("%c ", lettermap[i]);
        if (i == 9) printf("\n ");
        else if (i == 18) printf("\n  ");
        else if (i == 25) printf("\n");
    }
    white();
}

int max(int a, int b) {
    return a > b ? a : b;
}

void increasevalue(int i, int j, Map *map) {
    int* value = malloc(sizeof(int));
    *value = 0;
    *value = (*(int*)mapgetordefault(map, &attempts[i][j], value)) + 1;
    mapput(map, &attempts[i][j], value);
}

void fillguessmap(int i) {
    if (tries > i) {
        for (int j = 0; j < WORD_LENGTH - 1; j++) {
            if (mapget(wordmap, &attempts[i][j]) != NULL) increasevalue(i, j, guessmap);
            else if (get_qwerty_letter(attempts[i][j]) != -1) lettercolour[get_qwerty_letter(attempts[i][j])] = 1;
        }
    }
}

void print_attempts() {
    for (int i = 0; i < ATTEMPS; i++) {
        mapvalueclear(guessmap);
        mapvalueclear(completemap);
        fillguessmap(i);
        int correct = 0;
        for (int j = 0; j < WORD_LENGTH - 1; j++) {
            if (tries > i) {
                if (attempts[i][j] == word[j]) {
                    green();
                    increasevalue(i, j, completemap);
                    correct++;
                    if (*(int*)mapget(completemap, &attempts[i][j]) == *(int*)mapget(wordmap, &attempts[i][j])) {
                        lettercolour[get_qwerty_letter(attempts[i][j])] = 3;  
                    } else lettercolour[get_qwerty_letter(attempts[i][j])] = 2;
                }
                else if (mapget(wordmap, &attempts[i][j]) != NULL) {
                    if (*(int*)mapget(guessmap, &attempts[i][j]) <= *(int*)mapget(wordmap, &attempts[i][j])) {
                        lettercolour[get_qwerty_letter(attempts[i][j])] = max(lettercolour[get_qwerty_letter(attempts[i][j])], 2);
                        yellow();
                    }
                }
            }
            printf("[");
            if (tries > i) printf("%c", attempts[i][j]);
            else printf(" ");
            printf("]");
            white();
        }
        printf("\n");
        if (correct == 5) haswon = 1;
    }
}

void run_game() {
    int allocated = 0;
    if (!allocated) {
        attempts = malloc(ATTEMPS * sizeof(char*));
        allocated = 1;
    }
    while (tries <= ATTEMPS && !haswon) {
        int attempted = 0;
        char* attempt;
        if (!attempted) {
            attempt = malloc(GUESS_LENGTH * sizeof(char));
            attempted = 1;
        }
        cls();
        printf("%s\n", word);
        print_attempts();
        print_keyboard();
        if (haswon) {
            printf("You've won!");
            free(attempt);
            attempt = NULL;
        } else if (tries != ATTEMPS) {
            printf("Enter your next guess: ");
            gets(attempt);
            while (strlen(attempt) != WORD_LENGTH - 1) {
                printf("Invalid length of word. Enter your next guess: ");
                gets(attempt);
            }
        } else {
            printf("Wrong, the word was: %s", word);
            free(attempt);
            attempt = NULL;
        }
        attempts[tries] = attempt;
        tries++;
    }
}

int main() {
    wordmap = mapcreate(CHARACTER, INTEGER);
    guessmap = mapcreate(CHARACTER, INTEGER);
    completemap = mapcreate(CHARACTER, INTEGER);
    read_word();
    init_wordmap();
    init_lettermap();
    cls();
    run_game();
    freeattempts();
    mapvaluefree(wordmap);
    mapvaluefree(guessmap);
    mapvaluefree(completemap);
    free(word);
    attempts = NULL;
    word = NULL;
    wordmap = NULL;
    guessmap = NULL;
    return 0;
}