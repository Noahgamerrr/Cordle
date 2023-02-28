#include <string.h>
#include <time.h>
#include "map.h"

#define MAP_LENGTH 5
#define FILE_LENGTH 15918
#define WORD_LENGTH 6

char* read_word() {
    FILE* ptr;
    char *word = malloc(WORD_LENGTH * sizeof(char));
    srand(time(NULL));
    int r = rand() % FILE_LENGTH;
    int i = 0;
    ptr = fopen("..\\words.txt", "r");
    do {
        fgets(word, WORD_LENGTH + 1, ptr);
        i++;
    } while (i < r);
    fclose(ptr);
    return word;
}

void init_map(Map* map) {
    char *word = "hallo";
    printf("%s\n", word);
    for (size_t i = 0; i < strlen(word); i++) {
        char *current_char = malloc(sizeof(char));
        int *value = malloc(sizeof(int));
        *current_char = word[i];
        *value = 0;
        *value = (*(int*)mapgetordefault(map, current_char, value)) + 1;
        mapput(map, current_char, value);
    }
    free(word);
}

int main() {
    Map *map = mapcreate(CHARACTER, INTEGER);
    init_map(map);
    char* test = malloc(sizeof(char));
    *test = 'a';
    printf("%d", *(int*)mapget(map, test));
    free(test);
    mapfree(map);
    return 0;
}