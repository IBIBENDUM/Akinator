#ifndef TREE_GRAPHIC_LOGS_H_
#define TREE_GRAPHIC_LOGS_H_

#include <time.h>

#include "tree.h"
#include "tree_config.h"

#define tree_log(TREE_PTR, MSG, ...)                  \
    do {                                              \
        time_t time_info = time(NULL);                \
        log_call_line_info call_line_info =           \
        {                                             \
            .file = __FILE__,                         \
            .func = __PRETTY_FUNCTION__,              \
            .line = __LINE__,                         \
            .time = localtime(&time_info)             \
        };                                            \
        tree_log_(TREE_PTR, MSG, &call_line_info);    \
    } while (0)

// Information about line from which log is called
struct log_call_line_info
{
    const char* file;
    const char* func;
    const int   line;
    struct tm*  time;
};

tree_error tree_log_(const Tree tree, const char* message, log_call_line_info* line_info);

tree_error tree_close_log_file();

tree_error tree_open_log_file();

void tree_set_log_status(bool status);

#endif
