#ifndef __MYLER_SONG_H__
#define __MYLER_SONG_H__

#include <myler_config.h>
#include <myler_type.h>
#include <myler_music.h>
#include <myler_lyric.h>

typedef struct song_file {
    char file_name[MAX_SONG_NAME + 1];
    char singer[MAX_SINGER_NAME + 1];
    char name[MAX_FILE_NAME + 1];
} song_file_t;

typedef struct song {
    song_file_t file;
    music_t *music;
    lyric_t *lyric;
    music_info_t info; 
} song_t;


#endif
