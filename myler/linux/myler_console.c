#include <myler_utils.h>
#include <myler_console.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

static struct {
    bool is_init;
    bool color_disable;
} con_info;

static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

static int _getch(void)
{
    int c;
    struct termios org_opts, new_opts;

    tcgetattr(STDIN_FILENO, &org_opts);
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);

    return c;
}


/* 初始化控制台 */
void init_cosole(void)
{
    system("clear");
    con_info.is_init = 1;
}

/* 释放控制台 */
void free_console(void)
{
    con_info.is_init = 0;
    system("clear");
}

/* 设置当前输出的前景色
 * color: 前景色
 */
void set_color(color_t color)
{
    myler_assert(is_color(color), "");

    if (con_info.color_disable)
        color = MYLER_DEFAULT_COLOR;

    static const int color_tab[8] = {0, 4, 2, 6, 1, 5, 3, 7};
    const char *bold_str = (color < 8 || color == 16 ? "" : "2;");
    int color_cole = (color == 16 ? 0 : 30 + color_tab[color % 8]);

    printf("\033[%s%dm", bold_str, color_cole);
}

/* 是否启用控制台颜色
 * 如果不启用控制台颜色，set_color函数将失效
 * 默认启用
 * enable: true->启用，false->禁用
 */
void set_color_enable(bool enable)
{
   con_info.color_disable = !enable;
}

/* 设置当前输出的坐标位置
 * x: x坐标
 * y: y坐标
 */
void set_pos(pos_t x, pos_t y)
{
    myler_assert(x >= 0, "无效的横坐标！");
    myler_assert(y >= 0, "无效的纵坐标！");

    printf("\033[%d;%dH", y + 1, x + 1);
}

/* 获取可使用的控制台的大小
 * w: 宽
 * h: 高
 */
void get_console_size(pos_t *w, pos_t *h)
{
    myler_assert(w != NULL, "");
    myler_assert(h != NULL, "");

    struct winsize size;

    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    *w = size.ws_col;
    *h = size.ws_row;
}

/* 如果有按键按下，返回对应的按键码，否则返回0
 */

myler_key_t get_key(void)
{
    if (!kbhit())
        return 0;

    myler_key_t key = _getch(); 
    
    if (key == -32) {
        switch (_getch()) {
        case 72: return MYLER_KEY_UP;
        case 75: return MYLER_KEY_LEFT;
        case 77: return MYLER_KEY_RIGHT;
        case 80: return MYLER_KEY_DOWN;
        }
    } 
        
    return (myler_key_t)toupper(key);
}

int console_printf(const char *format, ...)
{
    int ret;
    va_list ap;

    va_start(ap, format);
    ret = vprintf(format, ap);
    va_end(ap);
    return ret;
}

int console_putchar(int ch)
{
    return putchar(ch);
}
