#include <stdio.h>
#include <stdlib.h>

#include "Libs/logs.h"

#include "tree.h"
#include "tree_text_logs.h"
#include "tree_graphic_logs.h"

int main()
{
    set_log_level(LOG_LVL_DEBUG);

    Tree tree = tree_init();
    Tree_node root = tree_get_root(tree);

    tree_open_log_file();

    tree_insert(tree, &root, 5);
    tree_insert(tree, &root, 4);
    tree_insert(tree, &root, 6);
    Tree_node current_node = tree_get_child_node(root, LEFT);
    tree_insert(tree, &current_node, 3);
    tree_insert(tree, &current_node, 1);
    current_node = tree_get_child_node(current_node, RIGHT);
    tree_insert(tree, &current_node, 13);
    tree_insert(tree, &current_node, 17);
    current_node = tree_get_child_node(current_node, RIGHT);
    tree_insert(tree, &current_node, 18);
    tree_insert(tree, &current_node, 19);
    current_node = tree_get_child_node(current_node, LEFT);
    tree_insert(tree, &current_node, 2);
    tree_insert(tree, &current_node, 15);
    current_node = tree_get_child_node(root, RIGHT);
    tree_insert(tree, &current_node, 10);
    tree_insert(tree, &current_node, 11);
    current_node = tree_get_child_node(current_node, RIGHT);
    tree_insert(tree, &current_node, 19);
    current_node = tree_get_child_node(current_node, LEFT);
    tree_insert(tree, &current_node, 20);
    current_node = tree_get_child_node(current_node, LEFT);
    tree_insert(tree, &current_node, 21);
    tree_insert(tree, &current_node, 22);
    current_node = tree_get_child_node(current_node, LEFT);
    tree_insert(tree, &current_node, 23);
    tree_insert(tree, &current_node, 24);

    tree_close_log_file();

    tree_pre_order_traversal(root, &tree_text_dump);
    return 0;
}
