#include <myler_utils.h>
#include <myler_system.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

char *get_dir(void)
{
    return getcwd(NULL, 0);
}

int change_dir(const char *path)
{
    int ret = chdir(path);
    if (ret != 0)
        myler_print_warning("changer_dir(): %s: %s", path, strerror(errno));
    return ret;
}

const char *get_all_file_abspath(const char *path, char *file_path)
{
    myler_assert(file_path != NULL, "");

    struct dirent *ptr;
    static DIR *dir = NULL;
    static const char *_path = NULL;

    if (path) {
        if (_path)
            closedir(dir);
        dir = opendir(path);
        if (!dir) {
            myler_print_error("open failed: %s: %s", path, strerror(errno));
            _path = NULL;
            return NULL;
        }
        _path = path;
    }

    if (!_path)
        return NULL;

    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_type != 8)  /* DT_REG 常规文件 */
            continue;

        char *last_dir = getcwd(NULL, 0);
        chdir(_path);
        char *current_dir = getcwd(NULL, 0);

        strcpy(file_path, current_dir);
        strcat(file_path, "/");
        strcat(file_path, ptr->d_name);

        chdir(last_dir);
        myler_free(current_dir);
        myler_free(last_dir);
        return file_path;
    }

    closedir(dir);
    _path = NULL;
    return NULL;
}
