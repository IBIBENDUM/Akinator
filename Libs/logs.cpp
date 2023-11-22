#ifdef _WIN32
    #include <io.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>

#include "logs.h"
#include "colors.h"
#include "time_utils.h"

bool log_mute = false;

log_level current_log_level = LOG_LVL_TRACE;

struct Log_file
{
    FILE*     file_ptr;
    log_level level;
};

const size_t       MAX_LOG_FILES_AMOUNT  =  8;
Log_file LOG_FILES[MAX_LOG_FILES_AMOUNT] = {};
size_t             log_files_amount      =  1;

const char* log_levels_colors[] =
{
    COLOR_LIGHT_GRAY, COLOR_LIGHT_YELLOW, COLOR_LIGHT_CYAN,
    COLOR_LIGHT_MAGENTA, COLOR_LIGHT_RED
};

struct Log_event
{
    log_level   level;
    const char* format;
    const char* file;
    const char* func;
    const int   line;
    struct tm*  time;
    va_list     args;
};

static void init_event(Log_event* log_info)
{
    assert(log_info);

    time_t time_info = time(NULL);
    log_info->time = localtime(&time_info);
}

static void console_out(Log_event* log_info, FILE* output_ptr)
{
    assert(log_info);

    const char* time_str = cast_time_to_str(log_info->time);
    fprintf(output_ptr, COLOR_DARK_GRAY "[%s] %s%-5s: " COLOR_WHITE "%s:%s():%d:\n" TEXT_SETTINGS_RESET, time_str, log_levels_colors[log_info->level], log_levels_strings[log_info->level], log_info->file, log_info->func, log_info->line);
    vfprintf(output_ptr, log_info->format, log_info->args);
    fprintf(output_ptr, "\n\n");
}

static void file_out(Log_event* log_info, FILE* output_ptr)
{
    assert(log_info);

    const char* time_str = cast_time_to_str(log_info->time);
    fprintf(output_ptr, "[%s] %-5s: %s:%s():%d: ", time_str, log_levels_strings[log_info->level], log_info->file, log_info->func, log_info->line);
    vfprintf(output_ptr, log_info->format, log_info->args);
    fprintf(output_ptr, "\n\n");
}

static void write_log(Log_event* log_info, FILE* output_ptr)
{
    assert(log_info);
    int file_descriptor = fileno(output_ptr);

    if (isatty(file_descriptor))
        console_out(log_info, output_ptr);
    else
        file_out(log_info, output_ptr);
}

void set_log_mute(const bool is_mute)
{
    log_mute = is_mute;
}

void mute_log()
{
    set_log_mute(true);
}

void unmute_log()
{
    set_log_mute(false);
}


bool add_log_file(FILE* file_ptr, const log_level level)
{
    if (!file_ptr)
        return false;

    LOG_FILES[log_files_amount] =
    {
        .file_ptr = file_ptr,
        .level    = level
    };
    log_files_amount++;

    return true;
}

void set_log_level(enum log_level level)
{
    current_log_level = level;
}

void init_log(const log_level log_level, const char* file, const char* func, const int line, const char* format, ...)
{
    assert(file);
    assert(func);
    assert(line);
    assert(format);

    struct Log_event log_info =
    {
        .level  = log_level,
        .format = format,
        .file   = file,
        .func   = func,
        .line   = line
    };

    for (size_t i = 0; i < log_files_amount; i++)
    {
        // if (!log_mute && LOG_FILES[i].level >= current_log_level)
        // {
        if (!log_mute)
        {
            if (log_level >= current_log_level)
            {
            init_event(&log_info);
            va_start(log_info.args, format);
            write_log(&log_info, stderr);
            va_end(log_info.args);
            }
        }
        // }
    }
}
