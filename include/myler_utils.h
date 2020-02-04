#ifndef __MYLER_UTILS_H__
#define __MYLER_UTILS_H__

#include <myler_config.h>
#include <myler_type.h>

#ifdef MYLER_DEBUG
#define myler_assert(op, s) ((op) ? (void)0 : myler_panic(__FILE__, __func__, __LINE__, "断言“" #op "”失败：" s))
#else
#define myler_assert(op, s) ((void)0)
#endif

#define myler_crash()      do {(*(int *)0 = 0); exit(1); } while (0)

#define panic(s)           myler_panic(__FILE__, __func__, __LINE__, s)

#define myler_max(a, b)    ((a) > (b) ? (a) : (b))
#define myler_min(a, b)    ((a) < (b) ? (a) : (b))

void myler_panic(const char *file, const char *func, int line, const char *s);

void *malloc_and_check(int size);

#endif
