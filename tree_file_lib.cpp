#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include <fcntl.h>

#include "Libs/tree.h"
#include "Libs/textlib.h"
#include "Libs/logs.h"
#include "tree_file_Lib.h"

static tfl_error insert_node(wchar_t** buffer_ptr, Tree tree, Tree_node* node_ptr)
{
    assert(buffer_ptr);
    assert(tree);
    assert(node_ptr);

    size_t   word_len = 0;
    wchar_t* word     = get_word(*buffer_ptr, &word_len);

    *buffer_ptr += word_len + 1;
    **buffer_ptr = L'\0';
    (*buffer_ptr)++;

    tree_insert(tree, node_ptr, word);

    return TFL_NO_ERR;
}

#define TFL_RETURN_IF_ERR(ERROR) \
    do {                         \
        if (ERROR != TFL_NO_ERR) \
            return ERROR;        \
    } while(0)

static tfl_error parse_tree(wchar_t** buffer_ptr, Tree tree, Tree_node* node_ptr, Tree_child_side side)
{
    assert(buffer_ptr);
    assert(tree);
    assert(node_ptr);

    tfl_error err = TFL_NO_ERR;
    *buffer_ptr = move_to_non_space_sym(*buffer_ptr);
    if (**buffer_ptr == L'{')
    {
        (*buffer_ptr)++;
        err = insert_node(buffer_ptr, tree, node_ptr);
        TFL_RETURN_IF_ERR(err);

        Tree_node current_node = *node_ptr;
        if (tree_get_size(tree) != 1)
            current_node = tree_get_child_node(*node_ptr, side);

        err = parse_tree(buffer_ptr, tree, &current_node, LEFT);
        TFL_RETURN_IF_ERR(err);

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
        return TFL_NO_ERR;
    }
    return TFL_WRONG_FILE_FORMAT;
}

#undef TFL_RETURN_IF_ERR

static tfl_error read_tree(wchar_t* buffer, Tree tree)
{
    assert(buffer);
    assert(tree);

    // verify_tree_buffer() (amount of '{' == amount of '}')
    buffer = move_to_non_space_sym(buffer);
    if (*buffer)
    {
        if (*buffer != L'{')
            return TFL_WRONG_FILE_FORMAT;

        Tree_node root = tree_get_root(tree);
        return parse_tree(&buffer, tree, &root, (Tree_child_side) 0xAB0BA);
    }
    return TFL_NO_ERR;
}

tfl_error tree_load(const char* file_name, Tree tree)
{
    if (!tree)
        return TFL_TREE_NULL_PTR;

    if (!file_name)
        return TFL_NULL_FILE_NAME;

    wchar_t* buffer = read_file(file_name, TEXT);
    if (!buffer)
        return TFL_FILE_READ_ERR;

    tfl_error err = read_tree(buffer, tree);

    return err;
}

#define print(...) fwprintf(file_ptr, __VA_ARGS__)

static void print_node_in_file(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);

    if (!node)
        return;

    const Tree_node left_child  = tree_get_child_node(node, LEFT );
    const Tree_node right_child = tree_get_child_node(node, RIGHT);

    tree_elem_t value = 0;
    tree_get_node_value(node, &value);

    print(L"{ %d ", value);

    if (left_child)
        print_node_in_file(file_ptr, left_child);
    else
        print(L"%ls ", NULL_CODE_NAME);

    if (right_child)
        print_node_in_file(file_ptr, right_child);
    else
        print(L"%ls ", NULL_CODE_NAME);

    print(L"} ");
}

#undef print

tfl_error tree_save(const char* file_name, Tree tree)
{
    if (!tree)
        return TFL_TREE_NULL_PTR;

    if (!file_name)
        return TFL_NULL_FILE_NAME;

    FILE* file_ptr = fopen(file_name, "wb");
    if (!file_ptr)
        return TFL_FILE_OPEN_ERR;

    setmode(fileno(file_ptr), _O_U8TEXT);
    print_node_in_file(file_ptr, tree_get_root(tree));

    const int fclose_ret_val = fclose(file_ptr);
    file_ptr = NULL;
    if (fclose_ret_val != 0)
        return TFL_FILE_CLOSE_ERR;

    return TFL_NO_ERR;
}
