#ifndef CONSOLE_ARGS_H
#define CONSOLE_ARGS_H

#include "../Logs/logs.h"


struct Args_values
{
    const char*    input_file_name;
    bool           speak_async;
    log_level      cur_log_level;
};

bool handle_cmd_args(const int argc, char* const* argv, const char* format, Args_values* values);
void print_help(const char* format);

#endif
