#include <string.h>
#include <time.h>
#include "map.h"

#ifdef _WIN32
    int os = 0;
#else
    int os = 1;
#endif

#define MAP_LENGTH 5
#define FILE_LENGTH 15919
#define WORD_LENGTH 6
#define GUESS_LENGTH 50
#define ATTEMPS 6
#define ALPHABET_LENGTH 26

char* wordlist = ".\\words.txt";

Map *wordmap = NULL;
Map *guessmap = NULL;
Map *completemap = NULL;
char *attempts[ATTEMPS];
char* word = NULL;
char lettermap[ALPHABET_LENGTH];
int lettercolour[ALPHABET_LENGTH];
int tries = 0;
int haswon = 0;

/*
    Clears the screen
*/
void cls() {
    if (os) system("clear");
    else system("cls");
}

/*
    Frees all the attempts that have been made
*/
void freeattempts() {
    for (int i = ATTEMPS - 1; i >= 0; i--) {
        free(attempts[i]);
        attempts[i] = NULL;
    }
}

/*
    Changes the font-colour to yellow
*/
void yellow() {
    printf("\033[0;33m");
}

/*
    Changes the font-colour to green
*/
void green() {
    printf("\033[0;32m");
}

/*
    Changes the font-colour to white
*/
void white() {
    printf("\033[0;37m");
}

/*
    Changes the font-colour to bold white
*/
void bold_white() {
    printf("\033[1;37m");
}

/*
    Reads the word to be guessed from the file
*/
void read_word() {
    FILE* ptr;
    word = malloc(WORD_LENGTH * sizeof(char));
    srand(time(NULL));
    int r = rand() % FILE_LENGTH;
    int i = 0;
    ptr = fopen(wordlist, "r");
    do {
        fgets(word, WORD_LENGTH + 1, ptr);
        i++;
    } while (i < r);
    fclose(ptr);
}

/* 
    Looks if the guess is a valid word or not
    @param guess The guess made by the user
    @returns 1 if the guess is a valid word, else 0
*/
int isguessvalid(char* guess) {
    FILE* ptr;
    ptr = fopen(wordlist, "r");
    char* comp = malloc(WORD_LENGTH * sizeof(char));
    int valid = 0;
    int words = 0;
    do {
        fgets(comp, WORD_LENGTH + 1, ptr);
        int equals = 1;
        for (int i = 0; i < WORD_LENGTH - 1; i++) {
            if (guess[i] != comp[i]) equals = 0;
        }
        if (equals) valid = 1;
        words++;
    } while (!valid && words <= FILE_LENGTH);
    free(comp);
    fclose(ptr);
    return valid;
}

/*
    Initializes the map which contains information about
    how often a letter appears in the word
*/
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

/*
    Changes the colour on the console for every letter on the keyboard
    @param idx The index
*/
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

/*
    Returns the place of the letter in the QWERTY-order
    @param letter The letter
    @returns The position of the letter in QWERTY-order
*/
int get_qwerty_letter(char letter) {
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (letter == lettermap[i]) return i;
    }
    return -1;
}

/*
    Initializes the lettermap
*/
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

/*
    Clears any information about the letters on the keyboard
*/
void clearcolours() {
    for (int i = 0; i < ALPHABET_LENGTH; i++) lettercolour[i] = 0;
}

/*
    Prints the keyboard to the screen;
*/
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

/*
    Finds the bigger values of two integers
    @param a The first integer
    @param b The second integer
    @returns The bigger value
*/
int max(int a, int b) {
    return a > b ? a : b;
}

/*
    Increases the value of a letter inside the map by one
    @param i The index for attempts
    @param j The index for the letter inside attempts[i]
    @param map The map
*/
void increasevalue(int i, int j, Map *map) {
    int* value = malloc(sizeof(int));
    *value = 0;
    *value = (*(int*)mapgetordefault(map, &attempts[i][j], value)) + 1;
    mapput(map, &attempts[i][j], value);
}

/*
    Fills the guessmap with all the "green" values
    @param i The amount of guesses that already have been made
*/
void prefillguessmap(int i) {
    if (tries > i) {
        for (int j = 0; j < WORD_LENGTH - 1; j++) {
            if (mapget(wordmap, &attempts[i][j]) != NULL) {
                if (attempts[i][j] == word[j]) increasevalue(i, j, guessmap);
            }
            else if (get_qwerty_letter(attempts[i][j]) != -1) lettercolour[get_qwerty_letter(attempts[i][j])] = 1;
        }
    }
}

/*
    Prints out all the attempts that have been made
*/
void print_attempts() {
    for (int i = 0; i < ATTEMPS; i++) {
        mapvalueclear(guessmap);
        mapvalueclear(completemap);
        prefillguessmap(i);
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
                    increasevalue(i, j, guessmap);
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

/*
    Main-function for handling the game
*/
void run_game() {
    while (tries <= ATTEMPS && !haswon) {
        int attempted = 0;
        char* attempt;
        if (!attempted) {
            attempt = malloc(GUESS_LENGTH * sizeof(char));
            attempted = 1;
        }
        cls();
        print_attempts();
        print_keyboard();
        if (haswon) printf("You've won!\n");
        else if (tries != ATTEMPS) {
            printf("Enter your next guess: ");
            gets(attempt);
            while (strlen(attempt) != WORD_LENGTH - 1 || !isguessvalid(attempt)) {
                if (strlen(attempt) != WORD_LENGTH - 1) printf("Invalid length of word. Enter your next guess: ");
                else printf("Invalid word. Enter your next guess: ");
                gets(attempt);
            }
        } else printf("Wrong, the word was: %s", word);
        attempts[tries] = attempt;
        tries++;
    }
}

/*
    Prints out the menue
*/
int menu() {
    int choice = 0;
    printf("/----------Cordle----------\\\n");
    printf("|0. Leave                  |\n");
    printf("|1. Play                   |\n");
    printf("\\--------------------------/\n");
    printf("Please choose: ");
    scanf("%d", &choice);
    getchar();
    while (choice < 0 || choice > 1) {
        printf("\nInvalid choice\n");
        printf("Please choose: ");
        scanf("%d", &choice);
        getchar();
    }
    return choice;
}

/*
    Resets the game after it has been finished
*/
void resetgame() {
    freeattempts();
    mapdeepclear(wordmap);
    mapdeepclear(guessmap);
    mapdeepclear(completemap);
    clearcolours();
    haswon = 0;
    tries = 0;
}

int main() {
    wordmap = mapcreate(CHARACTER, INTEGER);
    guessmap = mapcreate(CHARACTER, INTEGER);
    completemap = mapcreate(CHARACTER, INTEGER);
    init_lettermap();
    while (menu()) {
        read_word();
        init_wordmap();
        cls();
        run_game();
        resetgame();
    }
    printf("Thanks for playing!");
    mapdeepfree(wordmap);
    mapdeepfree(guessmap);
    mapdeepfree(completemap);
    word = NULL;
    wordmap = NULL;
    guessmap = NULL;
    return 0;
}