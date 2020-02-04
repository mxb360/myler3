#include <myler_utils.h>
#include <myler_console.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* 以崩溃的形式，终止程序运行
 * 不建议直接使用此函数，请使用宏myler_panic
 * file: 终止的文件：传入 __FILE__
 * func: 终止的函数：传入 __FUNC__
 * line: 终止的行数：传入 __LINE__
 * s：   终止原因
 */
void _myler_panic(const char *file, const char *func, int line, const char *s)
{
    set_color(MYLER_LIGHT_RED);
    myler_printf("!!!程序出现运行错误而被迫终止!!!\n");
    
    set_color(MYLER_LIGHT_WHITE);
    myler_printf("--------------------------------------------------\n");
    myler_printf("错误描述：");
    set_color(MYLER_LIGHT_RED);
    myler_printf("%s    \n", s);

    set_color(MYLER_LIGHT_WHITE);
    myler_printf("错误位置：");
    set_color(MYLER_LIGHT_PURPL);
    myler_printf("%s 第%d行，位于函数“%s”里    \n", file, line, func);
    set_color(MYLER_DEFAULT_COLOR);

    /* 引发一个异常，终止程序 */
    myler_crash();
}

/* 退出系统 */
void myler_exit(int code)
{
    if (code)
        myler_print_error("播放器异常退出。返回码：%d。\n", code);
    exit(code);
}

/* 分配内存并做空指针检查 */
void *malloc_and_check(int size)
{
    void *p = malloc(size);
    if (p == NULL)
        myler_panic("malloc: 内存分配失败：可能是内存不足！");
    return p;
}


void myler_print_error(const char *format, ...)
{
    va_list ap;
    char buf[1024];

    va_start(ap, format);
    vsprintf(buf, format, ap);

    set_color(MYLER_DEFAULT_COLOR);
    myler_printf("%s: ", get_myler_name());

    set_color(MYLER_LIGHT_RED);
    myler_printf("错误：");

    set_color(MYLER_DEFAULT_COLOR);
    myler_printf(buf);
    myler_putchar('\n');
}

void myler_print_prompt(const char *format, ...)
{
    va_list ap;
    char buf[1024];

    va_start(ap, format);
    vsprintf(buf, format, ap);

    set_color(MYLER_DEFAULT_COLOR);
    myler_printf("%s: ", get_myler_name());

    set_color(MYLER_LIGHT_PURPL);
    myler_printf("提示：");
    
    set_color(MYLER_DEFAULT_COLOR);
    myler_printf(buf);
    myler_putchar('\n');
}

int _argc;
char **_argv;

void init_myler_utils(int argc, char *argv[])
{
    _argc = argc;
    _argv = argv;
}

const char *get_myler_name(void)
{
    return _argv[0];
}