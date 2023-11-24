#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Libs/logs.h"
#include "Libs/console_args.h"
#include "Libs/tree.h"
#include "Libs/tree_console_dump.h"
#include "Libs/tree_graphic_logs.h"
#include "Libs/tree_lns_lib.h"
#include "akinator.h"

int main(const int argc, char* const* argv)
{
    Args_values values = {.log_level = LOG_LVL_DEBUG};
    if (!handle_cmd_args(argc, argv, "i:o:m:h", &values))
        return 1;

    set_log_level(values.log_level);

    int err = akin_play(values.input_file_name, values.output_file_name);
    LOG_DEBUG("err = %d", err);

    return 0;
}
