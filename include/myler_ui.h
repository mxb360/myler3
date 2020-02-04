#ifndef __MYLER_UI_H__
#define __MYLER_UI_H__

#include <myler_config.h>
#include <myler_type.h>

void init_ui(void);

void update_ui(void);

void free_ui(void);

void set_title_win_enable(bool enable);

void set_list_win_enable(bool enable);

void set_time_win_enable(bool enable);

void set_ui_title(const char *title);

#endif
