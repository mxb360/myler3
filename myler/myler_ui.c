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

    int current_time;
    int total_time;
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
        set_window_pos(ui.main_title_win[type], the_main_win_pos);
        
        set_window_pos(ui.main_win[type], main_win_pos);
    }
}

static void update_time_win(bool force)
{
    static int current_time = -1, total_time = -1;

    if (!force && ui.current_time == current_time && ui.total_time == total_time)
        return;

    current_time = ui.current_time;
    total_time = ui.total_time;
    window_t *time_win = ui.time_win;
    rect_t pos = get_window_pos(time_win);
    pos_t time_w = pos.right - pos.left - 20;
    pos_t current_time_w = total_time ? (pos_t)(time_w * ((float)current_time / total_time)) : 0;

    set_color(TIME_COLOR1);
    set_pos(pos.left + 1, pos.top + 2);
    myler_printf(" ◎");
    for (pos_t w = 0; w < current_time_w; w++)
        myler_putchar('*');
    set_color(TIME_COLOR2);
    for (pos_t w = current_time_w; w < time_w; w++)
        myler_putchar('=');
    myler_putchar('>');
    
    set_color(TIME_COLOR3);
    myler_printf(" %02d:%02d/%02d:%02d", current_time / 60, current_time % 60, 
                                         total_time / 60, total_time % 60);
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
    static const char *title[MAIN_WIN_COUNT] = {"歌词", "搜索", "详情", "命令"};
    
    set_window_row_text(ui.title_win, 0, UI_TITLE_COLOR, "Myler 命令行音乐播放器 V3.0");
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++) {
        if (type == ui.active_main_win_type)
            set_window_row_text(ui.main_title_win[type], 0, MAIN_TITLE_ACTIVE_COLOR, "★%s", title[type]);
        else
            set_window_row_text(ui.main_title_win[type], 0, MAIN_TITLE_DEFAULT_COLOR, title[type]);
    }
}

static void ui_window_size_check(void)
{
    get_console_size(&ui.w, &ui.h);

    pos_t min_w = MAIN_WIN_COUNT * 12;
    pos_t min_h = 12;
    if (ui.w < min_w || ui.h < min_h) {
        myler_print_error("界面初始化失败！界面窗口太小：要求最小%dx%d，实际为%dx%d。", min_w, min_h, ui.w, ui.h);
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

    set_lyric_win_text(-4, "你总是微笑如花");
    set_lyric_win_text(-3, "总是看我沉醉和绝望");
    set_lyric_win_text(-2, "我却迟迟都没发现真爱");
    set_lyric_win_text(-1, "原来在身旁");
    set_lyric_win_text(0, "你应该被呵护被珍惜被认真被深爱");
    set_lyric_win_text(1, "被捧在手掌心上");
    set_lyric_win_text(2, "像一艘从来都不曾靠岸的船");
    set_lyric_win_text(3, "终于有了你的港湾");
    set_lyric_win_text(4, "你应该更自私更贪心更坚持更明白");
    set_lyric_win_text(5, "将我的心全部霸占");
    set_time_win_time(0, 0);

    update_ui(true);
}

/* 更新UI */
void update_ui(bool force)
{
    update_window(ui.title_win, force);
    update_window(ui.time_win, force);
    update_window(ui.list_win, force);
    update_window(ui.main_win[ui.active_main_win_type], force);
    update_time_win(force);
    for (main_win_type_t type = 0; type < MAIN_WIN_COUNT; type++)
        update_window(ui.main_title_win[type], force);

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

/********************** lyric window *******************************/

void set_lyric_win_text(int row, const char *text)
{
    window_t *lyric_win = ui.main_win[LYRIC_MAIN_WINDOW];
    pos_t h = get_window_text_lines(lyric_win);
    
    pos_t real_row = row + h / 2;

    if (real_row < 0 || real_row >= h)
        return;

    if (row == 0) {
        set_window_row_text(lyric_win, real_row, CURRENT_LYRIC_COLOR, ">>> %s <<<", text);
        set_window_row_text(ui.time_win, 2, CURRENT_LYRIC_COLOR, "%s", text);
    } else
        set_window_row_text(lyric_win, real_row, DEFAULT_LYRIC_COLOR, text);
}

void set_lyric_win_no_lyric(void)
{
    window_t *lyric_win = ui.main_win[LYRIC_MAIN_WINDOW];
    pos_t h = get_window_text_lines(lyric_win);

    for (pos_t row = 0; row < h; row++) {
        if (row == h / 2)
            set_window_row_text(lyric_win, row, MYLER_LIGHT_RED, ">>> 没有找到歌词 <<<");
        else
            set_window_row_text(lyric_win, row, DEFAULT_LYRIC_COLOR, "");
    }
}

/********************** time window *******************************/

/* 时间进度窗口是否可见 */
void set_time_win_enable(bool enable)
{
    ui.use_time_win = enable;
    update_window_pos();
}

void set_time_win_time(int current_time, int total_time)
{
    myler_assert(current_time >= 0, "");
    myler_assert(current_time <= total_time, "");
    myler_assert(total_time < 59940, "");

    ui.current_time = current_time;
    ui.total_time = total_time;
}
