#ifndef __MYLER_UI_H__
#define __MYLER_UI_H__

#include <myler_config.h>
#include <myler_type.h>

typedef enum {
    LYRIC_MAIN_WINDOW,
    SEARCH_MAIN_WINDOW,
    CMD_RES_MAIN_WINDOW,

    MAIN_WIN_COUNT
} main_win_type_t;

void init_ui(void);

void update_ui(void);

void free_ui(void);

void set_title_win_enable(bool enable);

void set_list_win_enable(bool enable);

void set_time_win_enable(bool enable);

void set_main_title_win_enable(bool enable);

#endif
