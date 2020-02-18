#ifndef __MYLER_CONFIG_H__
#define __MYLER_CONFIG_H__

/* 调试模式 */
#define MYLER_DEBUG

/* Windows下，控制台使用的是GBK编码（该配置仅在Windows下有效） */
#define USE_WINDOWS_GBK_CONSOLE

/* 歌曲信息配置 */
#define MAX_SONG_NAME    63
#define MAX_SINGER_NAME  63
#define MAX_FILE_NAME    255

/* 颜色配置 */
#define M_ERROR_COLOR               MYLER_LIGHT_RED

#define WINDOW_DEFAULT_COLOR        MYLER_LIGHT_WHITE
#define UI_FRAMEWORK_COLOR          MYLER_LIGHT_YELLOW

#define UI_TITLE_COLOR              MYLER_LIGHT_RED
#define MAIN_TITLE_ACTIVE_COLOR     MYLER_LIGHT_BLUE
#define MAIN_TITLE_DEFAULT_COLOR    MYLER_LIGHT_WHITE

#define CURRENT_LYRIC_COLOR         MYLER_LIGHT_CYGN
#define DEFAULT_LYRIC_COLOR         MYLER_LIGHT_WHITE

#define TIME_COLOR1                 MYLER_LIGHT_PURPL
#define TIME_COLOR2                 MYLER_PURPL
#define TIME_COLOR3                 MYLER_LIGHT_WHITE

#define UI_STATUS_COLOR             MYLER_LIGHT_GREEN

#endif
