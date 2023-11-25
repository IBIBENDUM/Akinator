#include <stdlib.h>
#include <cerrno>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "utils.h"

bool create_folder(const char* path)
{
    int err = false;
    #ifdef _WIN32
        err = mkdir(path);
    #else
        const mode_t mode = 0755;
        err = mkdir(path, mode);
    #endif

    if (err && errno == EEXIST)
        err = false;

    return !err;
}
