#include <myler_utils.h>
#include <myler_music.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif 

#define MUISC_BUF_SIZE 255

static struct {
    int  id;
    char cmd[MUISC_BUF_SIZE + 1];
    char err[MUISC_BUF_SIZE + 1];
    char retbuf[MUISC_BUF_SIZE + 1];
    char argbuf[MUISC_BUF_SIZE + 1];
} music_data;

struct _music_t {
    char file_name[256];
    char device[20];
};

static char *get_file_name(char *path)
{
    int i = strlen(path);

    while (--i >= 0 && path[i] != '\\' && path[i] != '/');
    if (i > 0)
        strcpy(path, path + i + 1);
    return path;
}

/* 对mciSendString的一个简易封装
 * - 私有函数
 */
static int send_music_cmd_string(const char *cmd, const char *device, const char *arg)
{
    MCIERROR error;

    sprintf(music_data.cmd, "%s %s %s", cmd, device, arg);
    if ((error = mciSendString(music_data.cmd, music_data.retbuf, sizeof music_data.retbuf, NULL))) {
        mciGetErrorString(error, music_data.err, sizeof music_data.err);
        return MUSIC_ERROR;
    }
    strcpy(music_data.err, "OK");
    return MUSIC_OK;
}

/* 控制mciSendString
 * - 私有函数
 */
static int music_control(music_t *music, const char *cmd, const char *arg)
{
    if (send_music_cmd_string(cmd, music->device, arg))
        return MUSIC_ERROR;
    return MUSIC_OK;
}

/* 创建一个音乐对象并打开对应的音乐文件
 * file_name:  音乐文件的路径
 * return：    创建成功返回Music指针，失败返回NULL
 */
music_t *load_music(const char *file_name)
{
    music_t *music = (music_t *)malloc(sizeof(struct _music_t));

    if (music) {   
        strcpy(music->file_name, file_name);
        get_file_name(music->file_name);
        sprintf(music->device, "myler_music_id_%d", music_data.id++);
        sprintf(music_data.argbuf, "\"%s\" alias %s", file_name, music->device);
        if (send_music_cmd_string("open", "", music_data.argbuf)) {
            free(music);
            music = NULL;
        }
    }
    return music;
}

/* 开始播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int play_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return music_control(music, "play", NULL);
}

/* 删除（释放）Music指针
 * music:    Music指针
 */
void free_music(music_t *music)
{
    myler_assert(music != NULL, "");
    music_control(music, "close", "");
    free(music);
}

/* 暂停播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int pause_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return music_control(music, "pause", "");
}

/* 继续播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int resume_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return music_control(music, "resume", "");
}

/* 停止播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int stop_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return music_control(music, "stop", "");
}

/* 获取音乐时长（毫秒）
 * music:    Music指针
 * return:   成功返回时长，出错返回0
 */
mlen_t get_music_len(music_t *music)
{
    myler_assert(music != NULL, "");

    mlen_t len;
    if (music_control(music, "status", "length"))
        return 0;
    sscanf(music_data.retbuf, "%ld", &len);
    return len;
}

/* 获取音乐当前已播放的时长（毫秒）
 * music:    Music指针
 * return:   成功返回当前已播放的时长，出错返回0
 */
mlen_t get_music_pos(music_t *music)
{
    myler_assert(music != NULL, "");

    mlen_t len;
    if (music_control(music, "status", "position"))
        return 0;
    sscanf(music_data.retbuf, "%ld", &len);
    return len;
}

/* 设置音量（0-1000）
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int set_music_volume(music_t *music, int volume)
{
    myler_assert(music != NULL, "");
    myler_assert(volume >= 0 && volume <= 1000, "音量应在0-1000之间！");

    sprintf(music_data.argbuf, "volume to %d", volume);
    return music_control(music, "setaudio", music_data.argbuf);
}

/* 获取音量
 * music:    Music指针
 * return:   音量(0-1000)，出错返回-1
 */
int get_music_volume(music_t *music)
{
    myler_assert(music != NULL, "");

    int volume;
    if (music_control(music, "status", "volume"))
        return MUSIC_ERROR;
    sscanf(music_data.retbuf, "%d", &volume);
    return volume;
}

 int get_music_status(music_t *music)
 {
    myler_assert(music != NULL, "");
    
    if (music_control(music, "status", "mode"))
        return MUSIC_ERROR;
    if (strstr(music_data.retbuf, "not ready"))
        return MUSIC_NOT_READY;
    else if (strstr(music_data.retbuf, "paused"))
        return MUSIC_PAUSED;
    else if (strstr(music_data.retbuf, "stopped"))
        return MUSIC_STOPPED;
    else if (strstr(music_data.retbuf, "playing"))
        return MUSIC_PLAYING;
    return MUSIC_OK;
 }

/* 获取音乐文件名（含路径）
 * music:    Music指针
 * return:   音乐文件名，出错返回空串
 */
const char *get_music_file_name(music_t *music)
{
    myler_assert(music != NULL, "");
    return music->file_name;
}

/* 获取上一个Music函数的执行错误描述字符串
 * 如果上一个Music函数执行正确，返回"OK"
 */
const char *music_get_last_error(void)
{
    return music_data.err;
}
