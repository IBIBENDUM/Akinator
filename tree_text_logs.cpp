#include <stdio.h>
#include <wchar.h>
#include <fcntl.h>

#include "tree.h"
#include "tree_text_logs.h"
#include "tree_config.h"

FILE* TEXT_DUMP_FILE = stderr;

// BAH: Move to other file (textlib.h mb?)
static void print_n_wchar(FILE* file_ptr, const size_t amount, const wchar_t sym)
{
    for (size_t i = 0; i < amount; i++)
        putwc(sym, file_ptr);
}

static bool tree_check_for_neighbour(Tree_node node)
{
    if (!node)
        return false;

    Tree_node parent = tree_get_parent_node(node);
    Tree_node parent_right_child = tree_get_child_node(parent, RIGHT);
    if (parent_right_child && node != parent_right_child)
        return true;

    return false;
}

static bool tree_print_linker(Tree_node node)
{
    if (!node)
        return false;

    bool has_offset = false;
    has_offset = tree_print_linker(tree_get_parent_node(node));

    size_t offset = (tree_get_node_depth(node))? 2 : 0;
    print_n_wchar(TEXT_DUMP_FILE, offset - has_offset, L' ');

    has_offset = false;
    if (tree_check_for_neighbour(node))
    {
        putwc(L'│', TEXT_DUMP_FILE);
        has_offset = true;
    }
    return has_offset;
}

bool tree_text_dump(Tree_node node)
{
    setmode(fileno(TEXT_DUMP_FILE), _O_U8TEXT);

    Tree_node parent = tree_get_parent_node(node);
    if (parent)
    {
        bool has_offset = tree_print_linker(parent);

        size_t offset = (has_offset)? 1 : 2;
        print_n_wchar(TEXT_DUMP_FILE, offset, L' ');

        Tree_node parent_right_child = tree_get_child_node(parent, RIGHT);
        if (parent_right_child && node != parent_right_child)
            putwc(L'├', TEXT_DUMP_FILE);
        else
            putwc(L'└', TEXT_DUMP_FILE);
    }
    else
    {
        putwc(L'└', TEXT_DUMP_FILE);
    }

    tree_elem_t value = 0;
    tree_get_node_value(node, &value);
    wprintf(L" %d\n", value);

    return true;
}
