#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "tree_config.h"
#include "Libs/logs.h"
#include "Libs/utils.h"

struct Tree_node_
{
    tree_elem_t      value;
    Tree_node_* parent;
    Tree_node_* left;
    Tree_node_* right;
};

struct Tree_
{
    Tree_node_* root;
    size_t      size;
};

Tree_node tree_get_root(const Tree tree)
{
    LOG_TRACE("Root obtained!");
    return tree->root;
}

Tree_node tree_get_parent_node(const Tree_node node)
{
    return node->parent;
}

Tree tree_init()
{
    LOG_TRACE("Tree initializing...");

    Tree tree = (Tree) calloc(1, sizeof(Tree_));
//     if (!tree)
//         return TREE_MEM_ALLOC_ERR;
//
//     *tree = (Tree_){};
//     LOG_TRACE("Tree initialized!");

    return tree;
}

static tree_error tree_node_destruct(Tree_node* node_ptr)
{
    LOG_TRACE("Node destructing...");
    if (!(*node_ptr))
        return TREE_NULL_NODE_PTR_ERR;

    // BAH: or make by post-order??? (but i will need remake traverse function)
    tree_node_destruct(&(*node_ptr)->left);
    tree_node_destruct(&(*node_ptr)->right);

    free_and_null((void**) node_ptr);

    LOG_TRACE("Node destructed!");

    return TREE_NO_ERR;
}

static tree_error tree_node_init(Tree_node* node_ptr, Tree_node parent_ptr, const tree_elem_t value)
{
    LOG_TRACE("Node initializing...");
    *node_ptr = (Tree_node) calloc(1, sizeof(Tree_node_));
    if (!(*node_ptr))
        return TREE_MEM_ALLOC_ERR;

    (*node_ptr)->parent = parent_ptr;
    (*node_ptr)->value  = value;

    LOG_TRACE("Node initialized!");

    return TREE_NO_ERR;
}

Tree_node tree_get_child_node(const Tree_node node, const Tree_child child)
{
    if (!node)
        return NULL;

    if (child == LEFT)
        return node->left;

    return node->right;
}

tree_error tree_insert(const Tree tree, Tree_node* node, const tree_elem_t value)
{
    LOG_TRACE("Inserting node...");
    tree_error err = TREE_NO_ERR;
    if (!*node)
    {
        if (tree->size == 0)
        {
            err = tree_node_init(node, NULL, value);
            tree->size++;
            return err;
        }
        else
            return TREE_NULL_NODE_PTR_ERR;
    }
    // BAH: ???
    if (!(*node)->left)
        err = tree_node_init(&(*node)->left, *node, value);

    else if (!(*node)->right)
        err = tree_node_init(&(*node)->right, *node, value);

    else
        return TREE_NODE_IS_FULL;

    tree->size++;
    return err;

}

tree_error tree_destruct(Tree tree)
{
    LOG_TRACE("Tree destructing...");

    if (!tree)
        return TREE_NULL_PTR_ERR;

    LOG_TRACE("Tree destructed!");

    return tree_node_destruct(&tree->root);
}

size_t tree_get_node_depth(const Tree_node node)
{
    if (!node || !node->parent)
        return 0;

    return tree_get_node_depth(node->parent) + 1;
}

tree_error tree_get_node_value(const Tree_node node, tree_elem_t* value)
{
    if (!node)
        return TREE_NULL_NODE_PTR_ERR;

    *value = node->value;
    return TREE_NO_ERR;
}

tree_error tree_pre_order_traversal(Tree_node node, bool (*function) (Tree_node node))
{
    if (!node)
        return TREE_NULL_NODE_PTR_ERR;

    if (!function)
        return TREE_NULL_TRAVERSE_FUNCTION_ERR;

    tree_error err = TREE_NO_ERR;

    bool func_err = function(node);
    if (!func_err)
        return TREE_TRAVERSE_ERR;

    err = tree_pre_order_traversal(node->left, function);
    if (err == TREE_TRAVERSE_ERR)
        return err;

    err = tree_pre_order_traversal(node->right, function);
    if (err == TREE_NULL_NODE_PTR_ERR)
        err = TREE_NO_ERR;

    return err;
}

tree_error tree_in_order_traversal(Tree_node node, bool (*function) (Tree_node node))
{
    if (!node)
        return TREE_NULL_NODE_PTR_ERR;

    if (!function)
        return TREE_NULL_TRAVERSE_FUNCTION_ERR;

    tree_error err = TREE_NO_ERR;

    err = tree_pre_order_traversal(node->left, function);
    if (err == TREE_TRAVERSE_ERR)
        return err;

    bool func_err = function(node);
    if (!func_err)
        return TREE_TRAVERSE_ERR;

    err = tree_pre_order_traversal(node->right, function);
    if (err == TREE_NULL_NODE_PTR_ERR)
        err = TREE_NO_ERR;

    return err;
}

tree_error tree_post_order_traversal(Tree_node node, bool (*function) (Tree_node node))
{
    if (!node)
        return TREE_NULL_NODE_PTR_ERR;

    if (!function)
        return TREE_NULL_TRAVERSE_FUNCTION_ERR;

    tree_error err = TREE_NO_ERR;

    err = tree_pre_order_traversal(node->left, function);
    if (err == TREE_TRAVERSE_ERR)
        return err;

    err = tree_pre_order_traversal(node->right, function);
    if (err == TREE_NULL_NODE_PTR_ERR)
        err = TREE_NO_ERR;

    bool func_err = function(node);
    if (!func_err)
        return TREE_TRAVERSE_ERR;

    return err;
}
