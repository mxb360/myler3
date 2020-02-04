#include <myler_utils.h>
#include <myler_console.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* 终止程序运行，返回1
 * file: 终止的文件：传入 __FILE__
 * func: 终止的函数：传入 __FUNC__
 * line: 终止的行数：传入 __LINE__
 * s：   终止原因
 */
void myler_panic(const char *file, const char *func, int line, const char *s)
{
    set_color(MYLER_LIGHT_RED);
    console_printf("!!!程序出现运行错误而被迫终止!!!\n");
    
    set_color(MYLER_LIGHT_WHITE);
    console_printf("--------------------------------------------------\n");
    console_printf("错误描述：");
    set_color(MYLER_LIGHT_RED);
    console_printf("%s    \n", s);

    set_color(MYLER_LIGHT_WHITE);
    console_printf("错误位置：");
    set_color(MYLER_LIGHT_PURPL);
    console_printf("%s 第%d行，位于函数“%s”里    \n", file, line, func);
    set_color(MYLER_DEFAULT_COLOR);

    myler_crash();
}

/* 退出系统 */
void myler_exit(int code)
{
    exit(code);
}

void *malloc_and_check(int size)
{
    void *p = malloc(size);
    if (p == NULL)
        panic("malloc: 内存分配失败：可能是内存不足！");
    return p;
}