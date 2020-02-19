#ifndef __MYLER_SYSTEM_H__
#define __MYLER_SYSTEM_H__

#include <myler_config.h>
#include <myler_type.h>

char *get_dir(void);

int change_dir(const char *path);

const char *get_all_file_abspath(const char *path, char *file_path);

#endif
