#ifdef    _WIN32
#define   TX_COMPILED
#include "../Libs/TXLib/TXLib.h"
#endif

// TODO: You should use -I and avoid accessing outer folder (".." I mean),
//       this way it's easier to reorganize your project if you decide to
//       move files around and it's also nicer since "../Libs/" doesn't 
//       really give any useful information:

//       To sum up: clang++ -I Libs/ main.cpp -o main.o
//       
// ===== main.cpp:
//       #include "Logs/logs.h"

//       There is even a common project structure that is used for libraries
//       to get precision control over what can be included and how:

//       - library-folder
//         - src
//           library-file0.cpp
//           library-file1.cpp
//           internal-header.h // Only public headers should go to include
//
//         - include
//           - library-name // This gives includes in style "lib/file.h"
//             library-file0.h
//             library-file1.h

//       Then, when you want to use this library you can just:
//       clang++ -I library-folder/include ...

//       You may or may not like it, but it has proven quite useful in my
//       experience (especially for libraries shared via git submodules)

#include "../Libs/Logs/logs.h"
#include "../Libs/Console_args/console_args.h"
#include "../Includes/akinator.h"
#include "../Includes/akinator_ui.h"

int main(const int argc, char* const* argv)
{
    #ifdef _WIN32
    txDisableAutoPause();
    #endif

    Args_values values = {.cur_log_level = LOG_LVL_DEBUG};
    if (!handle_cmd_args(argc, argv, "i:am:h", &values))
        return 1;

    set_log_level(values.cur_log_level);

    int err = akin_play(values.input_file_name, values.speak_async);
    if (err != AKIN_EXIT)
    {
        akin_print_exit_error_msg(values.speak_async);
    }
    LOG_DEBUG("err = %d", err);

    return 0;
}
