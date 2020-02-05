#ifndef __MYLER_UI_H__
#define __MYLER_UI_H__

#include <myler_config.h>
#include <myler_type.h>

typedef enum {
    LYRIC_MAIN_WINDOW,
    SEARCH_MAIN_WINDOW,
    DETAIL_MAIN_WINDOW,
    CMD_RES_MAIN_WINDOW,

    MAIN_WIN_COUNT
} main_win_type_t;

void init_ui(void);

void update_ui(bool force);

void free_ui(void);

void set_title_win_enable(bool enable);

void set_list_win_enable(bool enable);

void set_time_win_enable(bool enable);

void set_main_title_win_enable(bool enable);

void set_lyric_win_text(int row, const char *text);

void set_lyric_win_no_lyric(void);

void set_time_win_time(int current_time, int total_time);


#endif
