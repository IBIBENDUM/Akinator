#ifndef TREE_H_
#define TREE_H_

#include <stddef.h>

#include "tree_config.h"

enum tree_error
{
    #define DEF_TREE_ERR(NAME, ...) NAME,
    #include "tree_errors.inc"
    // There is undef inside "tree_errors.inc"
};

const char* const tree_error_strings[]
{
    #define DEF_TREE_ERR(NAME, MESSAGE, ...) MESSAGE,
    #include "tree_errors.inc"
    // There is undef inside "tree_errors.inc"
};

enum Tree_child
{
    LEFT,
    RIGHT
};

// BAH: Tree is a pointer to Tree_struct,
//      but the user does not know
//      the declaration of the structure (is it right???)
struct  Tree_;
typedef Tree_* Tree;

struct  Tree_node_;
typedef Tree_node_* Tree_node;

// tree_error tree_init(Tree tree);
Tree tree_init();

tree_error tree_insert(const Tree tree, Tree_node* node, const tree_elem_t value);

tree_error tree_destruct(Tree tree);

Tree_node tree_get_root(const Tree tree);

Tree_node tree_get_parent_node(const Tree_node node);

Tree_node tree_get_child_node(const Tree_node node, const Tree_child child);

size_t tree_get_node_depth(const Tree_node node);

tree_error tree_get_node_value(const Tree_node node, tree_elem_t* value);

tree_error tree_pre_order_traversal(Tree_node node, bool (*function) (Tree_node node));

tree_error tree_in_order_traversal(Tree_node node, bool (*function) (Tree_node node));

tree_error tree_post_order_traversal(Tree_node node, bool (*function) (Tree_node node));

#endif
