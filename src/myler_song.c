#include <myler_utils.h>
#include <myler_song.h>
#include <myler_music.h>

#include <string.h>

song_t *new_song(const char *abs_file_path)
{
    myler_assert(abs_file_path != NULL, "");

    music_t *music = load_music(abs_file_path);
    if (music == NULL) {
        myler_print_error("load music failed: %s", get_music_last_error());
        return NULL;
    }

    song_t *song = myler_malloc(sizeof(song_t));
    strncpy(song->file_info.abspath, abs_file_path, MAX_FILE_NAME);
    strncpy(song->file_info.filename, get_music_file_name(music), MAX_FILE_NAME);
    get_music_info(music, &song->music_info);
    song->lyric = NULL;

    free_music(music);

    return song;
}