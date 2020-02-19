#include <myler_utils.h>
#include <myler_list.h>
#include <myler_song.h>
#include <myler_system.h>

#include <string.h>

static list_node_t *new_list_node(song_t *song)  /* !! song must be molloc !! */
{
    myler_assert(song != NULL, "");

    list_node_t *node = myler_malloc(sizeof(list_node_t));
    node->song = song;
    return node;
}

list_t *new_list(const char *list_name)
{
    myler_assert(list_name != NULL, "");

    list_t *list = myler_malloc(sizeof(list_t));
    strncpy(list->name, list_name, MAX_FILE_NAME);

    list->head.next = list->head.prev = NULL;
    list->length = 0;
    return list;
}

list_t *create_list_from_path(const char *list_name, const char *path)
{
    char name[MAX_FILE_NAME + 1];
    list_t *list = new_list(list_name);

    if (get_all_file_abspath(path, name)) {
        if (is_music_file(name))
            insert_song_to_list(list, new_song(name), -1);
        while (get_all_file_abspath(NULL, name))
            if (is_music_file(name))
                insert_song_to_list(list, new_song(name), -1);
    } else {
        free_list(list);
        list = NULL;
        myler_print_error("create_list_from_path(): get_all_file_abspath() return NULL");
    }
    return list;
}

void insert_song_to_list(list_t *list, song_t *song, int index)
{
    myler_assert(list != NULL, "");
    myler_assert(song != NULL, "");
    myler_assert(index >= -1, "index should be >= -1");

    list_node_t *node = &list->head;
    for (int i = 0; (i == -1 || i < index) && node->next; i++)
        node = node->next;

    list_node_t *node_next = node->next;
    node->next = new_list_node(song);
    node->next->prev = node;
    node->next->next = node_next;
    list->length++;
}

song_t *remove_song_to_list(list_t *list, int index)
{
    myler_assert(list != NULL, "");
    myler_assert(index >= -1, "index should be >= -1");
    if (list->length == 0)
        return NULL;

    list_node_t *node = &list->head;
    for (int i = 0; (i == -1 || i < index) && node->next; i++)
        node = node->next;

    song_t *song = node->song;

    list->length--;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    myler_free(node);
    return song;
}

song_t *get_song_from_list(list_t *list, int index)
{
    myler_assert(list != NULL, "");
    myler_assert(index >= -1, "index should be >= -1");
    if (list->length == 0)
        return NULL;

    list_node_t *node = &list->head;
    for (int i = 0; (i == -1 || i < index) && node->next; i++)
        node = node->next;
    return node->song;
}

void free_list(list_t *list)
{
    myler_assert(list != NULL, "");
    list_node_t *node = list->head.next;

    while (node) {
        song_t *song = node->song;
        if (song) {
            if (song->music)
                free_music(song->music);
            
            // !!! lyric !!!

            myler_free(song);
        }
        list_node_t *_node = node;
        node = node->next;
        myler_free(_node);
    }

    list->length = 0;
    myler_free(list);
}