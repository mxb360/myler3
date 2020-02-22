#ifndef __MYLER_UTILS_H__
#define __MYLER_UTILS_H__

#include <myler_config.h>
#include <myler_type.h>

/* myler_assert: 断言宏 */
#ifdef MYLER_DEBUG
#define myler_assert(op, s) ((op) ? (void)0 : myler_panic("断言“" #op "”失败：" s))
#else
#define myler_assert(op, s) ((void)0)
#endif

/* 引发一个运行时错误，使程序崩溃 */
#define myler_crash()      do {(*(volatile int *)0 = 0); exit(1); } while (0)

/* 发生了不可挽回的错误，终止程序运行 */
#define myler_panic(s)           _myler_panic(__FILE__, __func__, __LINE__, s)
void   _myler_panic(const char *file, const char *func, int line, const char *s);

#define myler_max(a, b)    ((a) > (b) ? (a) : (b))
#define myler_min(a, b)    ((a) < (b) ? (a) : (b))

/* no-null-malloc and free */
void *myler_malloc(int size);
void myler_free(void *p);

void init_myler_utils(int argc, char *argv[]);
const char *get_myler_name(void);

void myler_print_error(const char *format, ...);
void myler_print_prompt(const char *format, ...);
void myler_print_warning(const char *format, ...);
void myler_exit(int code);

char *get_file_name_from_path(char *file_name, const char *path);


#endif
