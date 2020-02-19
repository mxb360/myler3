#ifndef __MYLER_LISTWIN_H__
#define __MYLER_LISTWIN_H__

#include <myler_config.h>
#include <myler_type.h>
#include <myler_window.h>
#include <myler_list.h>

typedef struct listwin {
    window_t *win;
    list_t *list;
    pos_t start_index;
    pos_t choice_index;
} listwin_t;

listwin_t *create_listwin(void);

void free_listwin(listwin_t *listwin);

void set_listwin_list(listwin_t *listwin, list_t *list);

void load_list_to_listwin(listwin_t *win);



#endif
