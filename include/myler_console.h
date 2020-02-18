#ifndef __MYLER_CONSOLE_H__
#define __MYLER_CONSOLE_H__

#include <myler_config.h>
#include <myler_type.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 初始化控制台 */
void init_cosole(void);

/* 释放控制台 */
void free_console(void);

/* color_t: 控制台颜色类型
 * 控制台颜色支持十六种基本颜色：
 *   黑色，蓝色，  绿色，  浅绿色，  红色， 紫色，  黄色， 白色
 *   灰色，淡蓝色，淡绿色，淡浅绿色，淡红色，淡紫色，淡黄色，亮白色
 * 同意列的颜色实际上是同种颜色，不过，下面的颜色更深
 */
typedef uint16_t color_t;

/* 控制台支持的颜色 */
#define MYLER_BLCAK             0
#define MYLER_BLUE              1
#define MYLER_GREEN             2
#define MYLER_CYGN              3          
#define MYLER_RED               4        
#define MYLER_PURPL             5          
#define MYLER_YELLOW            6     
#define MYLER_WHITE             7   
#define MYLER_GRAY              8
#define MYLER_LIGHT_BLUE        9
#define MYLER_LIGHT_GREEN       10
#define MYLER_LIGHT_CYGN        11          
#define MYLER_LIGHT_RED         12        
#define MYLER_LIGHT_PURPL       13          
#define MYLER_LIGHT_YELLOW      14     
#define MYLER_LIGHT_WHITE       15    
#define MYLER_DEFAULT_COLOR     16   

/* 设置当前输出的前景色
 * color: 前景色
 */
void set_color(color_t color);

/* 判断颜色是否合法 */
#define is_color(color) ((color) >= 0 && (color) <= MYLER_DEFAULT_COLOR)

/* 是否启用控制台颜色
 * 如果不启用控制台颜色，set_color函数将失效
 * 默认启用
 * enable: true->启用，false->禁用
 */
void set_color_enable(bool enable);

/* pos_t: 控制台坐标类型
 * 控制台的坐标系统：原点在界面左上角，x轴向下，单位为行，y轴向右，单位为一个英文字符宽度
 * 本系统所支持的终端需要使用等宽字体，且ASCII字符占一个单位，一个非ASCII字符站两个单位
 */
typedef int16_t pos_t;

/* 设置当前输出的坐标位置
 * x: x坐标
 * y: y坐标
 */
void set_pos(pos_t x, pos_t y);

/* 获取可使用的控制台的大小
 * w: 宽
 * h: 高
 */
void get_console_size(pos_t *w, pos_t *h);

/* myler_key_t: 按键码类型
 * 当按下字母时，值为对应大写字母的ASCII码（即不区分大小写），
 * 按下数字或者符号时，值为对应数字或者字母的ASCII码。
 * 按下方向键时，值为MYLER_KEY_UP, MYLER_KEY_DOWN, MYLER_KEY_LEFT, MYLER_KEY_RIGHT之一
 */
typedef int8_t myler_key_t;

#define MYLER_KEY_UP        -1
#define MYLER_KEY_DOWN      -2
#define MYLER_KEY_LEFT      -3
#define MYLER_KEY_RIGHT     -4

/* 如果有按键按下，返回对应的按键码，否则返回0
 */
myler_key_t get_key(void);

/* 控制台输出
 * 即putchar
 */
int myler_putchar(int ch);

/* 控制台输出
 * 即printf
 */
int myler_printf(const char *format, ...);

/*  延时ms毫秒
 */
void myler_delay_ms(int ms);

#ifdef __cplusplus
}   /* extern "C" { */
#endif

#endif
