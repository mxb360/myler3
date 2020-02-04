#ifndef __MYLER_WINDOW_H__
#define __MYLER_WINDOW_H__

#include <myler_config.h>
#include <myler_type.h>
#include <myler_console.h>

/* 矩形结构体，用来表示窗口的位置和大小 */
typedef struct rect {
    pos_t left, top, right, bottom;
} rect_t;

/* 窗口文字对齐方式 */
typedef enum {
    /* 左对齐     居中对齐      右对齐 */
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT,
} align_style_t;

/* 窗口对象，不允许直接使用，只能定义其指针变量，然后通过相关函数操作窗口 */
typedef struct _window window_t;

window_t *create_window(void);

void set_window_pos(window_t *win, rect_t pos);

void draw_window(window_t *win);

void update_window(window_t *win);

void free_window(window_t *win);

void set_window_align_style(window_t *win, align_style_t align_style);

void set_window_line(window_t *win, pos_t line, color_t color, const char *format, ...);

#endif
