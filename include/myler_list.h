#ifndef __MYLER_LIST_H__
#define __MYLER_LIST_H__

#include <myler_config.h>
#include <myler_type.h>
#include <myler_song.h>

typedef struct list_node {
    song_t *song;
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

typedef struct list {
    char name[MAX_FILE_NAME + 1];
    int length;

    list_node_t head;
    list_node_t *current;
} list_t;

list_t *new_list(const char *list_name);

list_t *create_list_from_path(const char *list_name, const char *path);

void insert_song_to_list(list_t *list, song_t *song, int index);

song_t *remove_song_to_list(list_t *list, int index);

song_t *get_song_from_list(list_t *list, int index);

list_node_t *get_node_from_list(list_t *list, int index);

void free_list(list_t *list);

#endif
