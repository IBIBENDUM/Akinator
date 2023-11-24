#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include <fcntl.h>

#include "tree.h"
#include "textlib.h"
#include "logs.h"
#include "tree_lns_Lib.h"

static tree_lns_error insert_node(wchar_t** buffer_ptr, Tree tree, Tree_node* node_ptr)
{
    assert(buffer_ptr);
    assert(tree);
    assert(node_ptr);

    wchar_t* str = move_to_non_space_sym(*buffer_ptr);
    str++;

    size_t   str_len = wcscspn(str, L"\"");
    *buffer_ptr = str + str_len;
    **buffer_ptr = L'\0';
    (*buffer_ptr)++;

    tree_insert(tree, node_ptr, str);

    return TREE_LNS_NO_ERR;
}

#define TREE_LNS_RETURN_IF_ERR(ERROR) \
    do {                              \
        if (ERROR != TREE_LNS_NO_ERR) \
            return ERROR;             \
    } while(0)

static tree_lns_error parse_tree(wchar_t** buffer_ptr, Tree tree, Tree_node* node_ptr, Tree_child_side side)
{
    assert(buffer_ptr);
    assert(tree);
    assert(node_ptr);

    tree_lns_error err = TREE_LNS_NO_ERR;
    *buffer_ptr = move_to_non_space_sym(*buffer_ptr);
    if (**buffer_ptr == L'{')
    {
        (*buffer_ptr)++;
        err = insert_node(buffer_ptr, tree, node_ptr);
        TREE_LNS_RETURN_IF_ERR(err);

        Tree_node current_node = *node_ptr;
        if (tree_get_size(tree) != 1)
            current_node = tree_get_child_node(*node_ptr, side);

        err = parse_tree(buffer_ptr, tree, &current_node, LEFT);
        TREE_LNS_RETURN_IF_ERR(err);

        err = parse_tree(buffer_ptr, tree, &current_node, RIGHT);
        return err;
    }
    else if (**buffer_ptr == L'}')
    {
        (*buffer_ptr)++;
        return parse_tree(buffer_ptr, tree, node_ptr, side);
    }
    else if (wcsncmp(NULL_CODE_NAME, *buffer_ptr, NULL_CODE_NAME_LEN) == 0)
    {
        *buffer_ptr += NULL_CODE_NAME_LEN;
        return TREE_LNS_NO_ERR;
    }
    return TREE_LNS_WRONG_FILE_FORMAT;
}

static tree_lns_error verify_tree_buffer(const wchar_t* buffer)
{
    assert(buffer);

    const wchar_t* pointer = buffer;
    size_t closing_br_amount = 0;
    size_t opening_br_amount = 0;

    while (*pointer)
    {
        if (*pointer == L'{')
            closing_br_amount++;
        else if (*pointer == L'}')
            opening_br_amount++;

        pointer++;
    }
    if (closing_br_amount != opening_br_amount)
        return TREE_LNS_WRONG_FILE_FORMAT;

    return TREE_LNS_NO_ERR;
}

static tree_lns_error read_tree(wchar_t* buffer, Tree tree)
{
    assert(buffer);
    assert(tree);

    TREE_LNS_RETURN_IF_ERR(verify_tree_buffer(buffer));
    buffer = move_to_non_space_sym(buffer);
    if (*buffer)
    {
        if (*buffer != L'{')
            return TREE_LNS_WRONG_FILE_FORMAT;

        Tree_node root = tree_get_root(tree);
        return parse_tree(&buffer, tree, &root, (Tree_child_side) 0xAB0BA);
    }
    return TREE_LNS_NO_ERR;
}

#undef TREE_LNS_RETURN_IF_ERR

tree_lns_error tree_load(const char* file_name, Tree tree)
{
    if (!tree)
        return TREE_LNS_TREE_NULL_PTR;

    if (!file_name)
        return TREE_LNS_NULL_FILE_NAME;

    wchar_t* buffer = read_file(file_name, TEXT);
    if (!buffer)
        return TREE_LNS_FILE_READ_ERR;

    tree_lns_error err = read_tree(buffer, tree);

    return err;
}

#define print(...) fwprintf(file_ptr, __VA_ARGS__)

static void tree_print_node_in_file(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);

    if (!node)
        return;

    const Tree_node left_child  = tree_get_child_node(node, LEFT );
    const Tree_node right_child = tree_get_child_node(node, RIGHT);

    tree_elem_t value = 0;
    tree_get_node_value(node, &value);

    print(L"{ \"%ls\" ", value);

    if (left_child)
        tree_print_node_in_file(file_ptr, left_child);
    else
        print(L"%ls ", NULL_CODE_NAME);

    if (right_child)
        tree_print_node_in_file(file_ptr, right_child);
    else
        print(L"%ls ", NULL_CODE_NAME);

    print(L"} ");
}

#undef print

tree_lns_error tree_save(const char* file_name, Tree tree)
{
    if (!tree)
        return TREE_LNS_TREE_NULL_PTR;

    if (!file_name)
        return TREE_LNS_NULL_FILE_NAME;

    FILE* file_ptr = fopen(file_name, "wb");
    if (!file_ptr)
        return TREE_LNS_FILE_OPEN_ERR;

    setmode(fileno(file_ptr), _O_U8TEXT);
    tree_print_node_in_file(file_ptr, tree_get_root(tree));

    const int fclose_ret_val = fclose(file_ptr);
    file_ptr = NULL;
    if (fclose_ret_val != 0)
        return TREE_LNS_FILE_CLOSE_ERR;

    return TREE_LNS_NO_ERR;
}
