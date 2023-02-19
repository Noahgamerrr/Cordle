#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct Map {
    int map_size;
    struct Entry* entries;
} Map_init = {0, NULL};

struct Entry {
    void* key;
    void* value;
};

static void mapset(struct Map map, void* key, void* value) {
    for (int i = 0; i < map.map_size; i++) {
        if (*(char*)map.entries[i].key == *(char*)key) {
            free(map.entries[i].value);
            map.entries[i].value = value;
            return;
        }
    }
}

void* mapget(struct Map map, void* key) {
    for (int i = 0; i < map.map_size; i++) {
        if (*(char*)map.entries[i].key == *(char*)key) return map.entries[i].value;
    }
    return NULL;
}

void* mapgetordefault(struct Map map, void* key, void* def) {
    void* result = mapget(map, key);
    if (result != NULL) return result;
    return def;
}

struct Map mapput(struct Map map, void* key, void* value) {
    if (mapget(map, key) == NULL) {
        map.map_size++;
        if (map.map_size == 1) map.entries = (struct Entry*)malloc(sizeof(struct Entry));
        else map.entries = (struct Entry*)realloc(map.entries, map.map_size*sizeof(struct Entry));
        map.entries[map.map_size - 1].key = key;
        map.entries[map.map_size - 1].value = value;
    } else mapset(map, key, value);
    return map;
}