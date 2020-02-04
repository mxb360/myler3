#include <myler_ui.h>
#include <myler_utils.h>
#include <myler_window.h>
#include <myler_console.h>

static struct {
    pos_t w, h;

    pos_t list_win_w;

    bool use_title_win;
    bool use_list_win;
    bool use_time_win;

    window_t *title_win;
    window_t *list_win;
    window_t *main_win;
    window_t *time_win;
} ui;

/* 更新界面中各个窗口的位置
 * 当隐藏或者显示某些窗口时，界面窗口的位置可能会发生变化
 * 本函数负责更新这种变化并重新设置窗口的位置
 */
static void update_window_pos(void)
{
    rect_t title_win_pos;
    rect_t list_win_pos;
    rect_t main_win_pos;
    rect_t time_win_pos;

    title_win_pos.right = ui.w - 2;
    title_win_pos.left = 0;
    title_win_pos.top = 0;
    title_win_pos.bottom = ui.use_title_win ? 2 : title_win_pos.top;

    time_win_pos.bottom = ui.h - 2;
    time_win_pos.left = 0;
    time_win_pos.right = ui.w - 2;
    time_win_pos.top = ui.h - (ui.use_time_win ? 6 : 2);

    list_win_pos.left = 0;
    list_win_pos.top = title_win_pos.bottom;
    list_win_pos.bottom = time_win_pos.top;
    list_win_pos.right = ui.use_list_win ? ui.list_win_w : list_win_pos.left;

    main_win_pos.left = list_win_pos.right;
    main_win_pos.top = title_win_pos.bottom;
    main_win_pos.bottom = time_win_pos.top;
    main_win_pos.right = ui.w - 2;

    set_window_pos(ui.title_win, title_win_pos);
    set_window_pos(ui.list_win, list_win_pos);
    set_window_pos(ui.main_win, main_win_pos);
    set_window_pos(ui.time_win, time_win_pos);
}

/* 初始化控制台，创建各个子窗口 */
void init_ui(void)
{
    init_cosole();
    get_console_size(&ui.w, &ui.h);

    ui.title_win = create_window();
    ui.list_win = create_window();
    ui.main_win = create_window();
    ui.time_win = create_window();

    ui.list_win_w = 20;

    set_title_win_enable(true);
    set_list_win_enable(true);
    set_time_win_enable(true);
}

void update_ui(void)
{
    update_window(ui.title_win);
    update_window(ui.time_win);
    update_window(ui.list_win);
    update_window(ui.main_win);
}

void free_ui(void)
{
    free_window(ui.title_win);
    free_console();
}

void set_title_win_enable(bool enable)
{
    ui.use_title_win = enable;
    update_window_pos();
}

void set_list_win_enable(bool enable)
{
    ui.use_list_win = enable;
    update_window_pos();
}

void set_time_win_enable(bool enable)
{
    ui.use_time_win = enable;
    update_window_pos();
}

void set_ui_title(const char *title)
{
    set_window_line(ui.title_win, 0, MYLER_LIGHT_YELLOW, title);
}