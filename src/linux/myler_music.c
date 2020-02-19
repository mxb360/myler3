#include <myler_utils.h>
#include <myler_music.h>

#include <string.h>

struct _music_t {
    char file_name[256];
};


/* 创建一个音乐对象并打开对应的音乐文件
 * file_name:  音乐文件的路径
 * return：    创建成功返回Music指针，失败返回NULL
 */
music_t *load_music(const char *file_name)
{
    music_t *music = myler_malloc(sizeof(struct _music_t));
    
    get_file_name_from_path(music->file_name, file_name);

    return music;
}

/* 开始播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int play_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 删除（释放）Music指针
 * music:    Music指针
 */
void free_music(music_t *music)
{
    myler_assert(music != NULL, "");
    myler_free(music);
}

/* 暂停播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int pause_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 继续播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int resume_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 停止播放音乐
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int stop_music(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 获取音乐时长（毫秒）
 * music:    Music指针
 * return:   成功返回时长，出错返回0
 */
mlen_t get_music_len(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 获取音乐当前已播放的时长（毫秒）
 * music:    Music指针
 * return:   成功返回当前已播放的时长，出错返回0
 */
mlen_t get_music_pos(music_t *music)
{
    myler_assert(music != NULL, "");
    return 0;
}

/* 设置音量（0-1000）
 * music:    Music指针
 * return:   成功返回0，出错返回-1
 */
int set_music_volume(music_t *music, int volume)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

/* 获取音量
 * music:    Music指针
 * return:   音量(0-1000)，出错返回-1
 */
int get_music_volume(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;
}

int get_music_status(music_t *music)
{
    myler_assert(music != NULL, "");
    return MUSIC_ERROR;

}

void get_music_info(music_t *music, music_info_t *music_info)
{
    myler_assert(music != NULL, "");
    myler_assert(music_info != NULL, "");

    strcpy(music_info->name, "");
    strcpy(music_info->singer, "");
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
const char *get_music_last_error(void)
{
    return "";
}

bool is_music_file(const char *file_name)
{
    static const char *suport_file_ext[] = {".mp3", };
    char *ext = strrchr(file_name, '.');

    for (int i = 0; i < sizeof(suport_file_ext)/sizeof(const char *); i++) {
        if (!strcmp(ext, suport_file_ext[i]))
            return true;
    }
    return false;
}
