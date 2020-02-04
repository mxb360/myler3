#ifndef __MYLER_MUSIC_H__
#define __MYLER_MUSIC_H__

#include <myler_config.h>
#include <myler_type.h>

#define MUSIC_NOT_READY  0
#define MUSIC_PAUSED     1
#define MUSIC_PLAYING    2
#define MUSIC_STOPPED    3

#define MUSIC_ERROR     -1
#define MUSIC_OK         0

#ifdef __cplusplus
extern "C" {
#endif 

typedef unsigned long mlen_t;
typedef struct _music_t music_t;

music_t *load_music(const char *file_name);

int play_music(music_t *music);

void free_music(music_t *music);

int pause_music(music_t *music);

int resume_music(music_t* music);

int stop_music(music_t *music);

mlen_t get_music_len(music_t *music);

mlen_t get_music_pos(music_t *music);

mlen_t set_music_pos(music_t *music, mlen_t pos);

int set_music_volume(music_t *music, int volume);

int get_music_volume(music_t *music);

int get_music_status(music_t *music);

const char *get_music_file_name(music_t *music);

const char *get_music_last_error(void);

#ifdef __cplusplus
}   /* extern "C" { */
#endif

#endif  /* !__MYLER_MUSIC_H__ */
