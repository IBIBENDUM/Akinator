#ifdef    _WIN32
#define   TX_COMPILED
#include "Libs/TXLib/TXLib.h"
#endif

#include "Libs/Logs/logs.h"
#include "Libs/Console_args/console_args.h"
#include "akinator.h"

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
    LOG_DEBUG("err = %d", err);

    return 0;
}
