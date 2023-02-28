#include "map.h"

#define MAX_STR_LEN 50

//A struct which saves a Map with Entries
typedef struct mym {
    size_t map_size; //The size of the map
    Entry* entries; //The entries inside the map
    enum type key_type; //The data type of the key
    enum type value_type; //The data type of the value
} Map;


//An entry which stores the key-value-pair
typedef struct mye {
    void* key; //The key
    void* value; //The value
} Entry;

static bool mapkeycmp(enum type key_type, void* map_key, void* key);
static void mapset(Map *map, void* key, void* value);
static Entry* mapentries(Map *map);

/*
    Create a map and returns its pointer
    @param key_type The type of the key
    @param value_type The type of the value
    @return The pointer to the map
*/
Map* mapcreate(enum type key_type, enum type value_type) {
    Map *map = malloc(sizeof(Map));
    map->entries = NULL;
    map->map_size = 0;
    map->key_type = key_type;
    map->value_type = value_type;
    return map;
}

/*
    This function compares two keys and returns if they're equal or not
    @param key_type The type of the key
    @param map_key The key from the map
    @param key The key requested by the user
    @return are the two keys equal or not
*/
static bool mapkeycmp(enum type key_type, void* map_key, void* key) {
    switch (key_type){
        case INTEGER:
            return (*(int*)map_key == *(int*)key);
        case CHARACTER:
            return (*(char*)map_key == *(char*)key);
        case STRING:
            return (strcmp((char*)map_key, (char*)key) == 0);
        case FLOAT:
            return (*(float*)map_key = *(float*)key);
        case DOUBLE:
            return (*(double*)map_key = *(double*)key);
        case UNSIGNED_CHARACTER:
            return (*(unsigned char*)map_key = *(unsigned char*)key);
        case UNSIGNED_INTEGER:
            return (*(unsigned int*)map_key == *(unsigned int*)key);
        case SHORT:
            return (*(short*)map_key == *(short*)key);
        case UNSIGNED_SHORT:
            return (*(unsigned short*)map_key = *(unsigned short*)key);
        case LONG:
            return (*(long*)map_key = *(long*)key);
        case LONG_LONG:
            return (*(long long*)map_key = *(long long*)key);
        case UNSIGNED_LONG:
            return (*(unsigned long*)map_key = *(unsigned long*)key);
        case UNSIGNED_LONG_LONG:
            return (*(unsigned long long*)map_key = *(unsigned long long*)key);
        case LONG_DOUBLE:
            return (*(long double*)map_key = *(long double*)key);
        default:
            return false;
    }
}

/*
    Checks if the map is empty
    @param map The map
    @return Is the map empty or not
*/
bool mapempty(Map *map) {
    return map->map_size == 0;
}

/*
    Gets the value from the map
    @param map The map where the entry is saved
    @param key The key from which the value needs to be extracted
    @return The value behind the key or NULL, if the map doesn't contain such key
*/
void* mapget(Map *map, void* key) {
    for (size_t i = 0; i < map->map_size; i++) {
        if (mapkeycmp(map->key_type, map->entries[i].key, key)) {
            return map->entries[i].value;
        }
    }
    return NULL;
}

/*
    Gets the value from the map or a default value
    @param map The map where the entry is saved
    @param key The key from which the value needs to be extracted
    @param def The default value if the key is not in the map
    @return The value behind the key or the default value, if the map doesn't contain such key
*/
void* mapgetordefault(Map *map, void* key, void* def) {
    void* result = mapget(map, key);
    if (result != NULL) return result;
    return def;
}


/*
    Checks if the map contains a specific key
    @param map The map
    @param key The key 
    @return Does the map contain the key
*/
bool mapcontainskey(Map *map, void* key) {
    return (mapget(map, key) != NULL);
}

/*
    Checks if the map contains a specific value
    @param map The map
    @param value The value 
    @return Does the map contain the value
*/
bool mapcontainsvalue(Map *map, void* value) {
    void** values = mapvalues(map);
    bool contains_value = false;
    for (size_t i = 0; i < mapsize(map); i++) {
        if (mapkeycmp(map->value_type, values[i], value)) {
            contains_value = true;
            break;
        }
    }
    values = NULL;
    return contains_value;
}

/*
    Sets the value of an already existing key in a map
    @param map The map where the value needs to be set
    @param key The key where the value needs to be changed
    @param value The new value
*/
static void mapset(Map *map, void* key, void* value) {
    for (size_t i = 0; i < map->map_size; i++) {
        if (mapkeycmp(map->key_type, map->entries[i].key, key)) {
            map->entries[i].value = value;
            return;
        }
    }
}

/*
    Puts a key-value-pair into the map
    @param map The map in which the entry is saved
    @param key The key of the entry
    @param The value linked to the key
*/
void mapput(Map *map, void* key, void* value) {
    if (!mapcontainskey(map, key)) {
        map->map_size++;
        if (map->map_size == 1) map->entries = (Entry*)malloc(sizeof(Entry));
        else map->entries = (Entry*)realloc(map->entries, map->map_size*sizeof(Entry));
        map->entries[map->map_size - 1].key = key;
        map->entries[map->map_size - 1].value = value;
    } else mapset(map, key, value);
}

/*
    Puts a key-value-pair into the map if the key isn't already reserved
    @param map The map in which the entry is saved
    @param key The key of the entry
    @param The value linked to the key
    @return If the key was able to be put in the map
*/
bool mapputifabsent(Map *map, void* key, void* value) {
    if (mapget(map, key) != NULL) return false;
    mapput(map, key, value);
    return true;
}

/*
    Gets the size of the map
    @param map The map from which the size needs to be retrieved
    @return The size of the map
*/
size_t mapsize(Map *map) {
    return map->map_size;
}

/*
    Gets all the entries saved in the map
    @param map The map from which the entries need to be retrieved
    @return All the entries saved in the map
*/
static Entry* mapentries(Map *map) {
    return map->entries;
}

/*
    Gets all the keys saved in the map
    @param map The map from which the keys need to be retrieved
    @return All the keys saved in the map
*/
void** mapkeys(Map *map) {
    void** keys = malloc(map->map_size * sizeof(void*));
    for (size_t i = 0; i < map->map_size; i++) keys[i] = map->entries[i].key;
    return keys;
}

/*
    Gets all the values saved in the map
    @param map The map from which the values need to be retrieved
    @return All the values saved in the map
*/
void** mapvalues(Map *map) {
    void** values = malloc(map->map_size * sizeof(void*));
    for (size_t i = 0; i < map->map_size; i++) values[i] = map->entries[i].value;
    return values;
}

/*
    Removes a key from the map
    @param map The map where the key needs to be removed
    @param key The key to be removed
    @return The value which was linked to the key
*/
void* mapremove(Map *map, void* key) {
    Entry* entries = mapentries(map);
    void* value = NULL;
    for (size_t i = 0; i < mapsize(map); i++) {
        if (mapkeycmp(map->key_type, entries[i].key, key)) {
            value = entries[i].value;
            for (size_t j = i; j < mapsize(map) - 1; j++) entries[i] = entries[i+1];
            map->map_size--;
            if (!mapempty(map)) map->entries = (Entry*)realloc(map->entries, map->map_size*sizeof(Entry));
            else map->entries = NULL;
            break;
        }
    }
    entries = NULL;
    return value;
}

/*
    Removes a key-value-pair from the map
    @param map The map where the key needs to be removed
    @param key The key to be removed
    @param value The value to the pair
    @return Returns true if the key-value-relation had been present in the map and has thus been removed
*/
bool mapremovepair(Map *map, void* key, void* value) {
    if (!(mapcontainskey(map, key) && mapkeycmp(map->value_type, mapget(map, key), value))) return false;
    mapremove(map, key);
    return true;
}

/*
    Replaces a value of a key if key is present in map
    @param map The map where the value needs to be changed
    @param key The key where the value needs to be changed
    @param value The new value
    @return Returns the old value linked to the key before the replacement
*/
void* mapreplace(Map *map, void* key, void* value) {
    Entry* entries = mapentries(map);
    void* old_value = NULL;
    for (size_t i = 0; i < mapsize(map); i++) {
        if (mapkeycmp(map->key_type, entries[i].key, key)) {
            old_value = entries[i].value;
            entries[i].value = value;
            break;
        }
    }
    entries = NULL;
    return old_value;
}

/*
    Replaces a value of a key if key is present in map and old_value is linked to key
    @param map The map where the value needs to be changed
    @param key The key where the value needs to be changed
    @param old_value The value previously linked to the key
    @param value The new value
    @return Returns true if the old_value actually was linked to the key, thus was replaced by value
*/
bool mapreplacepair(Map *map, void* key, void* old_value, void* value) {
    if (!(mapcontainskey(map, key) && mapkeycmp(map->value_type, mapget(map, key), old_value))) return false;
    mapreplace(map, key, value);
    return true;
}

/*
    Takes a function and runs it through every entry in the map
    @param map The map where the function should be looped through
    @param operation The function which is run through every Entry 
        (takes as parameters (void* key, void* value))
*/
void mapforeach(Map *map, void (*operation)(void*, void*)) {
    Entry* entries = mapentries(map);
    for (size_t i = 0; i < mapsize(map); i++) operation(entries[i].key, entries[i].value);
    entries = NULL;
}

/*
    Takes a value and calculates with the old_value already assigned to the key
    a new value
    @param map The map
    @param key The key for which the new value needs to be calculated
    @param value The new value
    @param operation The operation with which the new value gets calculated
*/
void* mapmerge(Map *map, void *key, void *value, void (*operation)(void*, void*)) {
    void* map_val = mapget(map, key);
    if (map_val == NULL && value != NULL) {
        mapput(map, key, value);
        return NULL;
    }
    else operation(map_val, value);
    mapset(map, key, map_val);
    return map_val;
}

/*
    Computes the new value to a given key given a computing function (only if a value
    if already mapped to the key)
    @param map The map
    @param key The key
    @param operation The computing function
    @returns The newly computed value
*/
void* mapcompute(Map *map, void* key, void (*operation)(void*, void*)) {
    void* val = mapget(map, key);
    if (val == NULL) return NULL;
    operation(key, val);
    return val;
}

/*
    Delocates the memory from the map (only the entries-pointer, the keys and
    values need to be freed individually if dynamically allocated), but keeps the map
    itself initialized
    @param map The map to be resets
*/
void mapclear(Map *map) {
    free(map->entries);
    map->map_size = 0;
}

/*
    Delocates the memory from the map (only the entries-pointer and the map-pointer,
    the keys and values need to be freed individually if dynamically allocated)
    @param map The map to be resets
*/
void mapfree(Map *map) {
    free(map->entries);
    free(map);
}