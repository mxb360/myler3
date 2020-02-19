#include <myler_list.h>
#include <myler_listwin.h>
#include <myler_window.h>
#include <myler_utils.h>

listwin_t *create_listwin(void)
{
    listwin_t *listwin = myler_malloc(sizeof(listwin_t));
    listwin->win = create_window();
    listwin->choice_index = 0;
    listwin->start_index = 0;
    listwin->list = NULL;
    return listwin;
}

void free_listwin(listwin_t *listwin)
{
    free_window(listwin->win);
    myler_free(listwin);
}

void set_listwin_list(listwin_t *listwin, list_t *list)
{
    myler_assert(listwin != NULL, "");
    myler_assert(list != NULL, "");

    listwin->list = list;
}

void load_list_to_listwin(listwin_t *listwin)
{
    myler_assert(listwin != NULL, "");
    if (listwin->list == NULL)
        return;

    list_t *list = listwin->list;
    window_t *win = listwin->win;
    set_window_align_style(win, ALIGN_LEFT);
    listwin->list = list;

    int index;
    int start_index = listwin->start_index;
    int row = get_window_text_lines(win); 
    list_node_t *node = get_node_from_list(list, start_index);

    for (index = 0; index < row && node; index++) {
        const char *name = node->song->file_info.filename;
        if (node != list->current)
            set_window_row_text(win, index, MYLER_DEFAULT_COLOR, "%3d %s", index + start_index + 1, name);
        else
            set_window_row_text(win, index, MYLER_LIGHT_GREEN, "||> %s", name);
        node = node->next;
    }

    for (; index < row; index++)
        set_window_row_text(win, index, MYLER_DEFAULT_COLOR, "");
}
