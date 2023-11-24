#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Libs/logs.h"
#include "Libs/console_args.h"
#include "Libs/tree.h"
#include "Libs/tree_console_dump.h"
#include "Libs/tree_graphic_logs.h"
#include "tree_lns_lib.h"
#include "akinator.h"

int main(const int argc, char* const* argv)
{
    Args_values values = {.log_level = LOG_LVL_DEBUG};
    if (!handle_cmd_args(argc, argv, "i:o:m:h", &values))
        return 1;

    set_log_level(values.log_level);

    int err = akin_play(values.input_file_name, values.output_file_name);
    LOG_DEBUG("err = %d", err);

    // Tree tree = tree_init();

    // tree_insert(tree, &root, L"sdasdsa");
    // tree_insert(tree, &root, L"aboba");
//
//     tree_load("aboba.txt", tree);
//     Tree_node root = tree_get_root(tree);
//     tree_console_dump(root);

    // tree_insert(tree, &root, 6);
    // Tree_node current_node = tree_get_child_node(root, LEFT);
    // tree_insert(tree, &current_node, 3);
    // tree_insert(tree, &current_node, 1);
    // current_node = tree_get_child_node(current_node, RIGHT);
    // tree_insert(tree, &current_node, 13);
    // tree_insert(tree, &current_node, 17);
    // current_node = tree_get_child_node(current_node, RIGHT);
    // tree_insert(tree, &current_node, 18);
    // tree_insert(tree, &current_node, 19);
    // current_node = tree_get_child_node(current_node, LEFT);
    // tree_insert(tree, &current_node, 2);
    // tree_insert(tree, &current_node, 15);
    // current_node = tree_get_child_node(root, RIGHT);
    // tree_insert(tree, &current_node, 10);
    // tree_insert(tree, &current_node, 11);
    // current_node = tree_get_child_node(current_node, RIGHT);
    // tree_insert(tree, &current_node, 19);
    // current_node = tree_get_child_node(current_node, LEFT);
    // tree_insert(tree, &current_node, 20);
    // current_node = tree_get_child_node(current_node, LEFT);
    // tree_insert(tree, &current_node, 21);
    // tree_insert(tree, &current_node, 22);
    // current_node = tree_get_child_node(current_node, LEFT);
    // tree_insert(tree, &current_node, 23);
    // tree_insert(tree, &current_node, 24);

    // tree_destruct(tree);
    // tree_save("aboba.txt", tree);

    return 0;
}
