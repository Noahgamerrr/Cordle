#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

//Declaration of the enum type which enumerates data types saved in the map
enum type {
    INTEGER,
    CHARACTER,
    STRING,
    FLOAT,
    DOUBLE,
    UNSIGNED_CHARACTER,
    UNSIGNED_INTEGER,
    SHORT,
    UNSIGNED_SHORT,
    LONG,
    LONG_LONG,
    UNSIGNED_LONG,
    UNSIGNED_LONG_LONG,
    LONG_DOUBLE
};

typedef struct mym Map;

typedef struct mye Entry;

Map* mapcreate(enum type key_type, enum type value_type);
bool mapempty(Map *map);
void* mapget(Map *map, void* key);
void* mapgetordefault(Map *map, void* key, void* def);
bool mapcontainskey(Map *map, void* key);
bool mapcontainsvalue(Map *map, void* value);
void mapput(Map *map, void* key, void* value);
bool mapputifabsent(Map *map, void* key, void* value);
size_t mapsize(Map *map);
void** mapkeys(Map *map);
void** mapvalues(Map *map);
void* mapremove(Map *map, void* key);
bool mapremovepair(Map *map, void* key, void* value);
void* mapreplace(Map *map, void* key, void* value);
bool mapreplacepair(Map *map, void* key, void* old_value, void* value);
void mapforeach(Map *map, void (*operation)(void*, void*));
void* mapmerge(Map *map, void *key, void *value, void (*operation)(void*, void*));
void* mapcompute(Map *map, void* key, void (*operation)(void*, void*));
void mapclear(Map *map);
void mapfree(Map *map);
void mapdeepclear(Map *map);
void mapdeepfree(Map *map);
void mapvalueclear(Map *map);
void mapvaluefree(Map *map);
void mapkeyclear(Map *map);
void mapkeyfree(Map *map);

#endif