#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "tree_graphic_logs.h"
#include "tree_config.h"
#include "../Logs/logs.h"
#include "../Utils/utils.h"

struct Tree_node_
{
    tree_elem_t value;
    Tree_node_* parent;
    Tree_node_* left;
    Tree_node_* right;
};

struct Tree_
{
    Tree_node_* root;
    size_t      size;
};

static size_t tree_calc_size(const Tree_node node)
{
    if (!node || !node->parent)
        return 1;

    size_t size = 0;
    size += tree_calc_size(node->left);
    size += tree_calc_size(node->right);
    return size;
}

tree_error tree_verify(const Tree tree)
{
    if (!tree)
        return TREE_NULL_PTR_ERR;

    if (tree_calc_size(tree->root) != tree->size)
        return TREE_WRONG_SIZE_ERR;

    return TREE_NO_ERR;
}

Tree_node tree_get_root(const Tree tree)
{
    LOG_TRACE("Root obtained!");
    return tree->root;
}

size_t tree_get_size(const Tree tree)
{
    LOG_TRACE("Tree size obtained!");
    return tree->size;
}

Tree_node tree_get_parent_node(const Tree_node node)
{
    LOG_TRACE("Parent node obtained!");
    return node->parent;
}

tree_error tree_set_node_value(Tree_node node, const tree_elem_t value)
{
    LOG_TRACE("Value setted!");
    node->value = value;
    return TREE_NO_ERR;
}

Tree tree_init()
{
    LOG_TRACE("Tree initializing...");
    tree_open_log_file();
    Tree tree = (Tree) calloc(1, sizeof(Tree_));
    if (tree)
        LOG_TRACE("Tree initialized!");
    return tree;
}

static tree_error tree_node_destruct(Tree_node* node_ptr)
{
    LOG_TRACE("Node destructing...");
    if (!(*node_ptr))
        return TREE_NULL_NODE_PTR_ERR;

    tree_node_destruct(&(*node_ptr)->left);
    tree_node_destruct(&(*node_ptr)->right);

    FREE_AND_NULL(*node_ptr);

    LOG_TRACE("Node destructed!");

    return TREE_NO_ERR;
}

tree_error tree_node_init(Tree_node* node_ptr, Tree_node parent_ptr, const tree_elem_t value)
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

Tree_node tree_get_child_node(const Tree_node node, const Tree_child_side child_side)
{
    if (!node)
        return NULL;

    if (child_side == LEFT)
        return node->left;

    return node->right;
}

tree_error tree_insert(const Tree tree, Tree_node* node, const tree_elem_t value)
{
    tree_verify(tree);
    LOG_TRACE("Inserting node...");
    tree_error err = TREE_NO_ERR;
    if (!*node)
    {
        if (tree->size == 0)
        {
            err = tree_node_init(node, NULL, value);
            tree->size++;
            tree->root = *node;
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

    char message[MESSAGE_MAX_LEN] = "";
    sprintf(message, "%s tree = 0x%llX node = 0x%llX value = " TREE_ELEM_FORMAT,
            __func__, tree, *node, value);

    tree_log(tree, message);
    return err;
}

tree_error tree_destruct(Tree tree)
{
    LOG_TRACE("Tree destructing...");

    if (!tree)
        return TREE_NULL_PTR_ERR;

    tree_error err = tree_node_destruct(&tree->root);

    LOG_TRACE("Tree destructed!");

    tree_close_log_file();

    return err;
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
