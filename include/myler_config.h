#ifndef __MYLER_CONFIG_H__
#define __MYLER_CONFIG_H__

/*  OS */
#define MYLER_LINUX   1
#define MYLER_WINDOWS 2

#ifndef MYLER_OS
  #if defined(__linux)
    #define MYLER_OS  MYLER_LINUX
  #elif defined(_WIN32)
    #define MYLER_OS  MYLER_WINDOWS
  #endif
#endif

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

#define WINDOW_DEFAULT_COLOR        MYLER_DEFAULT_COLOR
#define UI_FRAMEWORK_COLOR          MYLER_LIGHT_YELLOW

#define UI_TITLE_COLOR              MYLER_LIGHT_RED
#define MAIN_TITLE_ACTIVE_COLOR     MYLER_LIGHT_BLUE
#define MAIN_TITLE_DEFAULT_COLOR    MYLER_DEFAULT_COLOR

#define CURRENT_LYRIC_COLOR         MYLER_LIGHT_CYAN
#define DEFAULT_LYRIC_COLOR         MYLER_DEFAULT_COLOR

#define TIME_COLOR1                 MYLER_LIGHT_PURPL
#define TIME_COLOR2                 MYLER_PURPL
#define TIME_COLOR3                 MYLER_DEFAULT_COLOR

#define UI_STATUS_COLOR             MYLER_LIGHT_GREEN

#endif
