#include <stdio.h>
#include <time.h>
#include "map.h"

#define MAP_LENGTH 5
#define FILE_LENGTH 15918
#define WORD_LENGTH 6

const char* read_word() {
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

void init_map(struct Map* map) {
    const char *word = read_word();
    for (int i = 0; i < MAP_LENGTH; i++) {
        map[i].key = i;
        map[i].value = word[i];
    }
    printf("%s", word);
}

int main() {
    struct Map map[MAP_LENGTH];
    init_map(map);
    return 0;
}