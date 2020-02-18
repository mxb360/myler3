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
    list_node_t *start;
    list_node_t *end;
} list_t;

#endif
