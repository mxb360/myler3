#include <myler_utils.h>
#include <myler_console.h>

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <stdarg.h>

#include <windows.h>

#define MAX_CONSOLE_BUF   1023

static struct {
    bool is_init;
    bool color_disable;
    HANDLE out;
    CONSOLE_SCREEN_BUFFER_INFO screen;
} con_info;

static void get_default_data(void)
{
    con_info.out = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(con_info.out, &con_info.screen);
}

/* 初始化控制台 */
void init_cosole(void)
{
    system("cls");
    get_default_data();
    con_info.is_init = true;
}

/* 释放控制台 */
void free_console(void)
{
    if (con_info.is_init) {
        SetConsoleTextAttribute(con_info.out, con_info.screen.wAttributes);
        system("cls");
        con_info.is_init = false;
    }
}

/* 设置当前输出的前景色
 * color: 前景色
 */
void set_color(color_t color)
{
    myler_assert(is_color(color), "");

    if (!con_info.out)
        get_default_data();
    if (!con_info.color_disable) {
        if (color == MYLER_DEFAULT_COLOR)
            SetConsoleTextAttribute(con_info.out, con_info.screen.wAttributes);
        else
            SetConsoleTextAttribute(con_info.out, (con_info.screen.wAttributes & 0xff00) | color);
    }
}

/* 是否启用控制台颜色
 * 如果不启用控制台颜色，set_color函数将失效
 * 默认启用
 * enable: true->启用，false->禁用
 */
void set_color_enable(bool enable)
{
    con_info.color_disable = !enable;
    if (!enable)
        SetConsoleTextAttribute(con_info.out, con_info.screen.wAttributes);
}

/* 设置当前输出的坐标位置
 * x: x坐标
 * y: y坐标
 */
void set_pos(pos_t x, pos_t y)
{
    myler_assert(x >= 0, "无效的横坐标！");
    myler_assert(y >= 0, "无效的纵坐标！");

    if (!con_info.out) 
        get_default_data();
    SetConsoleCursorPosition(con_info.out, (COORD){(SHORT)x, (SHORT)y});
}

/* 获取可使用的控制台的大小
 * w: 宽
 * h: 高
 */
void get_console_size(pos_t *w, pos_t *h)
{
    myler_assert(w != NULL, "");
    myler_assert(h != NULL, "");

    if (!con_info.out) 
        get_default_data();

    *w = con_info.screen.srWindow.Right - con_info.screen.srWindow.Left + 1;
    *h = con_info.screen.srWindow.Bottom - con_info.screen.srWindow.Top + 1;
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

#ifdef USE_WINDOWS_GBK_CONSOLE
/* 将utf8编码的字符串转化为gbk的编码
 * dest: 新编码的字符串
 * src:  原字符串
 * 返回: 新字符串
 */
static char *utf8_to_gbk(char *gbk_string, const char *utf8_string)
{
    myler_assert(gbk_string != NULL, "");
    myler_assert(utf8_string != NULL, "");

    wchar_t *unicodeStr = NULL;
    int nRetLen = 0;
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, NULL, 0);
    unicodeStr = (wchar_t *)malloc(nRetLen * sizeof(wchar_t));
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, unicodeStr, nRetLen);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, 0);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, gbk_string, nRetLen, NULL, 0);
    free(unicodeStr);
    return gbk_string;
}
#endif

int myler_printf(const char *format, ...)
{
#ifndef USE_WINDOWS_GBK_CONSOLE
    int ret;
    va_list ap;

    va_start(ap, format);
    ret = vprintf(format, ap);
    va_end(ap);
    return ret;
#else
    va_list ap;
    char utf8_buf[MAX_CONSOLE_BUF + 1];
    char gbk_buf[MAX_CONSOLE_BUF + 1];

    va_start(ap, format);
    vsprintf(utf8_buf, format, ap);
    va_end(ap);

    utf8_to_gbk(gbk_buf, utf8_buf);
    return printf("%s", gbk_buf);
#endif
}

int myler_putchar(int ch)
{
    return putchar(ch);
}

void myler_delay_ms(int ms)
{
    Sleep(ms);
}
