#include <myler_ui.h>
#include <myler_utils.h>
#include <myler_window.h>
#include <myler_console.h>

static struct {
    pos_t w, h;
    pos_t cmd_row;

    pos_t list_win_w;

    bool use_title_win;
    bool use_list_win;
    bool use_time_win;
    bool use_main_title_win;

    main_win_type_t active_main_win_type;

    window_t *title_win;
    window_t *list_win;
    window_t *main_win[MAIN_WIN_COUNT];
    window_t *main_title_win[MAIN_WIN_COUNT];
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
    rect_t main_title_win_pos;
    rect_t time_win_pos;
    rect_t the_main_win_pos;

    title_win_pos.right = ui.w - 2;
    title_win_pos.left = 0;
    title_win_pos.top = 0;
    title_win_pos.bottom = title_win_pos.top + (ui.use_title_win ? 2 : 0);

    time_win_pos.bottom = ui.h - 2;
    time_win_pos.left = 0;
    time_win_pos.right = ui.w - 2;
    time_win_pos.top = ui.h - (ui.use_time_win ? 6 : 2);

    list_win_pos.left = 0;
    list_win_pos.top = title_win_pos.bottom;
    list_win_pos.bottom = time_win_pos.top;
    list_win_pos.right = ui.use_list_win ? ui.list_win_w : list_win_pos.left;

    main_title_win_pos.left = list_win_pos.right;
    main_title_win_pos.top = title_win_pos.bottom;
    main_title_win_pos.bottom = main_title_win_pos.top + (ui.use_main_title_win ? 2 : 0);
    main_title_win_pos.right = ui.w - 2;
    the_main_win_pos = main_title_win_pos;

    main_win_pos.left = list_win_pos.right;
    main_win_pos.top = main_title_win_pos.bottom;
    main_win_pos.bottom = time_win_pos.top;
    main_win_pos.right = ui.w - 2;

    set_window_pos(ui.title_win, title_win_pos);
    set_window_pos(ui.list_win, list_win_pos);
    set_window_pos(ui.time_win, time_win_pos);

    pos_t main_title_w = (main_title_win_pos.right - main_title_win_pos.left) / MAIN_WIN_COUNT;
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++) {
        the_main_win_pos.left = main_title_win_pos.left + main_title_w * type;
        if (type < MAIN_WIN_COUNT - 1)
            the_main_win_pos.right = main_title_win_pos.left + main_title_w * (1 + type);
        else
            the_main_win_pos.right = main_title_win_pos.right;

        //printf("%d: main_title_w=%d, left=%d right=%d\n", type, main_title_w, the_main_win_pos.left, the_main_win_pos.right);getchar();
        set_window_pos(ui.main_title_win[type], the_main_win_pos);
        
        set_window_pos(ui.main_win[type], main_win_pos);
    }
}

/* 创建界面需要的所有的窗口 */
static void create_ui_window(void)
{
    ui.title_win = create_window();
    ui.list_win = create_window();
    ui.time_win = create_window();
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++) {
        ui.main_win[type] = create_window();
        ui.main_title_win[type] = create_window();
    }

    ui.list_win_w = ui.w / 3;
    ui.active_main_win_type = LYRIC_MAIN_WINDOW;

    set_title_win_enable(true);
    set_list_win_enable(true);
    set_time_win_enable(true);
    set_main_title_win_enable(true);
}

/* 设置一些窗口的标题 */
static void set_ui_win_title(void)
{
    static const char *title[MAIN_WIN_COUNT] = {"歌词", "搜索", "命令"};
    
    set_window_row_text(ui.title_win, 0, UI_TITLE_COLOR, "Myler 命令行音乐播放器 V3.0");
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++) {
        if (type == ui.active_main_win_type)
            set_window_row_text(ui.main_title_win[type], 0, MAIN_TITLE_ACTIVE_COLOR, title[type]);
        else
            set_window_row_text(ui.main_title_win[type], 0, MAIN_TITLE_DEFAULT_COLOR, title[type]);
    }

}

void ui_window_size_check(void)
{
    get_console_size(&ui.w, &ui.h);

    pos_t min_w = MAIN_WIN_COUNT * 12;
    pos_t min_h = 12;
    if (ui.w < min_w || ui.h < min_h) {
        myler_print_error("界面初始化失败！可用的窗口太小：要求最小%dx%d，实际为%dx%d。", min_w, min_h, ui.w, ui.h);
        myler_print_prompt("你可以通过“%s --help-ui”了解如何修改窗口大小。", get_myler_name());
        myler_exit(1);
    }
}

/* 初始化控制台，创建各个子窗口 */
void init_ui(void)
{
    ui_window_size_check();

    init_cosole();
    create_ui_window();
    set_ui_win_title();
}

/* 更新UI */
void update_ui(void)
{
    update_window(ui.title_win);
    update_window(ui.time_win);
    update_window(ui.list_win);
    update_window(ui.main_win[ui.active_main_win_type]);
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++)
        update_window(ui.main_title_win[type]);
}

/* 销毁UI，释放内存 */
void free_ui(void)
{
    free_window(ui.title_win);
    free_window(ui.list_win);
    free_window(ui.time_win);
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++) {
        free_window(ui.main_win[type]);
        free_window(ui.main_title_win[type]);
    }

    free_console();
}

/* 标题窗口是否可见 */
void set_title_win_enable(bool enable)
{
    ui.use_title_win = enable;
    update_window_pos();
}

/* 列表窗口是否可见 */
void set_list_win_enable(bool enable)
{
    ui.use_list_win = enable;
    update_window_pos();
}

/* 主窗口的标题窗口是否可见 */
void set_main_title_win_enable(bool enable)
{
    ui.use_main_title_win = enable;
    update_window_pos();
}

/* 时间进度窗口是否可见 */
void set_time_win_enable(bool enable)
{
    ui.use_time_win = enable;
    update_window_pos();
}
