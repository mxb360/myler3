#include <myler_utils.h>
#include <myler_window.h>
#include <myler_console.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LINE_LEN        1023
#define MAX_LINE_CNT        63

typedef struct window_line {
    char str[MAX_LINE_LEN + 1];
    char str_show[MAX_LINE_LEN + 1];
    char str_show_buf[MAX_LINE_LEN + 1];
    color_t color;
} window_line_t;

/* window_t 结构的真正实现 */
struct _window {
    rect_t pos;
    rect_t pos_buf;
    window_line_t line[MAX_LINE_CNT];
    align_style_t align_style;
};

/* 判断window的位置是否发生变化 
 * set_window_pos可能会引发window发生变化
 */
static bool is_window_pos_changed(window_t *win)
{
    return win->pos.bottom != win->pos_buf.bottom ||
           win->pos.top != win->pos_buf.top ||
           win->pos.left != win->pos_buf.left ||
           win->pos.right != win->pos_buf.right;
}

/* 创建一个新的窗口对象
 * 注意：
 *     新建的窗口默认是不可用的，因为其长宽为0
 *     应该通过函数set_window_pos设置窗口大小，并使用函数update_window更新绘制窗口
 */
window_t *create_window(void)
{
    window_t *win = (window_t *)malloc_and_check(sizeof(window_t));

    memset(win, 0, sizeof(window_t));

    set_window_align_style(win, ALIGN_CENTER);

    return win;
}

/* 设置窗口的位置和大小
 * 可以通过此函数动态地修改窗口位置和大小
 * 窗口的位置和大小用rect_t类型的变量表示
 */
void set_window_pos(window_t *win, rect_t pos)
{
    myler_assert(win != NULL, "");

    /* 将新的大小放在放在缓冲空间里
     * 在更新窗口时，通过比较pos和pos_buf是否相等来判断窗口位置大小是否变化
     */
    win->pos_buf = pos;
}

/* 绘制窗口 
 * 该函数会完整的绘制窗口的所有部分
 * 如果窗口的大小为0，将不会绘制任何东西
 */
void draw_window(window_t *win)
{
    myler_assert(win != NULL, "");

    /* 窗口大小 */
    pos_t h = win->pos.bottom - win->pos.top;
    pos_t w = win->pos.right - win->pos.left;

    if (h == 0 || w == 0)
        return;

    /* 绘制边框 */
    for (pos_t y = 0; y <= h; y++) {
        if (y > 0 && y < h)
            strcpy(win->line[y - 1].str_show, win->line[y - 1].str_show_buf);
        set_pos(win->pos.left, win->pos.top + y);
        for (pos_t x = 0; x <= w; x++) {
            if (x == 0 || y == 0 || x == w || y == h)
                set_color(UI_FRAMEWORK_COLOR);
            else
                set_color(win->line[y - 1].color);

            if ((x == 0 && y == 0) || (x == w && y == 0) ||
                (x == 0 && y == h) || (x == w && y == h))
                putchar('+');
            else if (x == 0 || x == w)
                putchar('|');
            else if (y == 0 || y == h)
                putchar('-');
            else
                putchar(win->line[y - 1].str_show[x - 1]);
        }
        putchar('\n');
    }
}

/* 更新窗口 
 * 如果窗口的大小和位置发生变化，将重绘窗口
 * 否则，只更新有变化的行
 */
void update_window(window_t *win)
{
    myler_assert(win != NULL, "");

    /* 如果窗口的位置或者大小发生变化，重绘窗口 */
    if (is_window_pos_changed(win)) {
        win->pos = win->pos_buf;
        draw_window(win);
    } else 
        panic("error");
}

/* 销毁窗口，释放空间 */
void free_window(window_t *win)
{
    myler_assert(win != NULL, "");

    free(win);
}

/* 设置字符串的显示形式，如果字符串超出指定长度，则以“...”结尾，使总长度为length；
 * 否则按设置的对齐方式进行对齐，空格填充空白；该函数所得到的函数长度必为length。
 * string：待设置的字符串
 * length：字符串的允许长度
 * align_style：字符串的对齐方式 (左对齐:AlignLeft 居中对齐:AlignCenter 右对齐:AlignRight)
 * 返回：string本身
 */
static char *string_limit_and_format(char *string, int length, int align_style)
{
    int i = 0;
    char buf[MAX_LINE_LEN + 1];

    if (length <= 0)
        return strcpy(string, "");

    // 先全部填充空格
    strcpy(buf, string);
    memset(string, ' ', length);                
    string[length] = 0;
#if 01
    while (buf[i] && i < length) {
        if (buf[i++] < 0)                           // 非ASCII字符
            i++;
        if (i == length - 2) {                      // 位置位于非ASCII字符内
            strcpy(buf + length - 4, " ...");
            i += 4;
            break;
        } else if (i == length - 3) {               // ASCII字符
            strcpy(buf + length - 3, "...");
            i += 3;
            break;
        }
    }

    int len = i; 

    // 按照所设置对齐方式移动字符串
    if (i == length - 2 || i == length - 3) {
        strncpy(string, buf, len);
    } else {
        if (align_style == ALIGN_LEFT)
            strncpy(string, buf, len);
        else if (align_style == ALIGN_CENTER) {
            strncpy(string + (length - len) / 2, buf, len);
            /*printf("%s\n", string);
            while (1)
            {
            }*/
        }
        else if (align_style == ALIGN_RIGHT)
            strncpy(string + length - len, buf, len);
    }
    #endif

    return string;
}

static void format_window_str_to_show(window_t *win, int line)
{
    pos_t w = win->pos.right - win->pos.left - 1;

    strcpy(win->line[line].str_show_buf, win->line[line].str);
    string_limit_and_format(win->line[line].str_show_buf, w, win->align_style);
}

/* 设置文字的对齐方式 */
void set_window_align_style(window_t *win, align_style_t align_style)
{
    myler_assert(win != NULL, "");
    myler_assert(align_style >= ALIGN_LEFT && align_style <= ALIGN_RIGHT, "");

    win->align_style = align_style;
}

void set_window_line(window_t *win, pos_t line, color_t color, const char *format, ...)
{
    myler_assert(win != NULL, "");
    myler_assert(line < MAX_LINE_CNT, "");

    win->line[line].color = color;

    va_list ap;
    va_start(ap, format);
    vsprintf(win->line[line].str, format, ap);
    va_end(ap);

    format_window_str_to_show(win, line);
}
