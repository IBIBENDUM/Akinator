#include <assert.h>
#include <fcntl.h>

#define STK_DEBUG
#include "Libs/stack.h"
#include "Libs/stack_logs.h"
#include "Libs/tree.h"
#include "Libs/tree_console_dump.h"
#include "Libs/logs.h"
#include "Libs/utils.h"
#include "Libs/tree_lns_lib.h"
#include "akinator.h"
#include "akinator_ui.h"

static bool check_is_object_node(Tree_node node)
{
    if (!node)
        return false;

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    return (!left_child && !right_child);
}

static akin_error allocate_word(Akinator* akin, wchar_t** string)
{
    assert(akin);

    if (akin->buffer_size + MAX_STRING_SIZE > akin->buffer_capacity)
        return AKIN_MEMORY_FULL;

    wchar_t* buffer_ptr = akin->buffer + akin->buffer_size;

    size_t string_len = 0;
    while (string_len < 2) // BAH: Repeat if "\n" line
    {
        if (!fgetws(buffer_ptr, MAX_STRING_SIZE, stdin))
            return AKIN_STR_READ_ERR;
        string_len = wcslen(buffer_ptr);
    }

    *(buffer_ptr + string_len - 1) = L'\0';
    *string = buffer_ptr;
    akin->buffer_size += string_len;

    return AKIN_NO_ERR;
}

#define AKIN_RETURN_IF_ERR(ERROR) \
    do {                          \
        if (ERROR != AKIN_NO_ERR) \
            return ERROR;         \
    } while(0)

static akin_error akin_add_node(Akinator* akin, Tree_node node)
{
    assert(node);
    assert(akin);

    if (!akin->tree)
        return AKIN_TREE_NULL_PTR;

    akin_print_add_node_start_msg();

    akin_error err = AKIN_NO_ERR;

    wchar_t* left_string = NULL;
    err = allocate_word(akin, &left_string);
    AKIN_RETURN_IF_ERR(err);

    wchar_t* node_value = NULL;
    tree_error tree_err = tree_get_node_value(node, &node_value);
    if (tree_err != TREE_NO_ERR)
        return AKIN_TREE_ERR;

    akin_print_obj_question(left_string, node_value);

    wchar_t* question_string = NULL;
    err = allocate_word(akin, &question_string);
    AKIN_RETURN_IF_ERR(err);

    tree_set_node_value(node, question_string);
    tree_insert(akin->tree, &node, left_string);
    tree_insert(akin->tree, &node, node_value);

    tree_console_dump(tree_get_root(akin->tree));

    return AKIN_NO_ERR;
}
#undef AKIN_RETURN_IF_ERR

//~~~~~~~~~~~~~~~~~~~~~~~GUESS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static akin_error akin_guess_recurse(Akinator* akin, Tree_node node)
{
    if (!akin)
        return AKIN_NULL_PTR;

    if (!node)
        return AKIN_NULL_NODE_PTR;

    akin_print_guess_question(node);

    bool answer = akin_get_answer();

    if (check_is_object_node(node))
    {
        if (answer)
        {
            akin_print_guess_correct();
            return AKIN_NO_ERR;
        }
        else
            return akin_add_node(akin, node);
    }

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    if (answer)
        return akin_guess_recurse(akin, left_child);

    return akin_guess_recurse(akin, right_child);
}

static akin_error akin_guess(Akinator* akin)
{
    akin_print_guess_start_msg();
    return akin_guess_recurse(akin, tree_get_root(akin->tree));
}
//~~~~~~~~~~~~~~~~~~~~~~guess~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~DEFINITION~~~~~~~~~~~~~~~~~~~~~~~~~
static akin_error akin_find_object(Tree_node node, wchar_t* ref, Tree_node* dest, stack* stk)
{
    if (!node || !dest)
        return AKIN_NULL_NODE_PTR;

    if (!ref)
        return AKIN_STR_NULL_PTR;

    wchar_t* value = NULL;
    tree_get_node_value(node, &value);

    if (!value)
        return AKIN_STR_NULL_PTR;

    akin_error err = AKIN_NO_ERR;
    if (check_is_object_node(node) && wcscmp(value, ref) == 0)
    {
        *dest = node;
        return AKIN_NODE_FOUND;
    }
    else
    {
        Tree_node left_child  = tree_get_child_node(node, LEFT );
        Tree_node right_child = tree_get_child_node(node, RIGHT);

        push_stack(stk, AKIN_TRUE_VALUE);
        err = akin_find_object(left_child, ref, dest, stk);
        if (err == AKIN_NODE_FOUND)
            return err;

        int stk_value = 0;
        pop_stack(stk, &stk_value);

        push_stack(stk, AKIN_FALSE_VALUE);
        err = akin_find_object(right_child, ref, dest, stk);
        if (err == AKIN_NODE_FOUND)
            return err;

        pop_stack(stk, &stk_value);
    }
    return err;
}

static akin_error akin_describe_recurse(const Tree_node node, elem_t* data)
{
    if (!data)
        return AKIN_NULL_PTR;

    if (!node || check_is_object_node(node))
        return AKIN_NO_ERR;

    akin_error err = AKIN_NO_ERR;
    akin_print_describe_msg(node, *data);
    if (*data == AKIN_TRUE_VALUE)
    {
        Tree_node left_child = tree_get_child_node(node, LEFT);
        err = akin_describe_recurse(left_child, data++);
    }
    else if(*data == AKIN_FALSE_VALUE)
    {
        Tree_node right_child = tree_get_child_node(node, RIGHT);
        err = akin_describe_recurse(right_child, data++);
    }
    return err;
}

static akin_error akin_get_object(Akinator* akin, stack* stk_ptr, Tree_node* node)
{
    if (!akin)
        return AKIN_NULL_PTR;

    wchar_t buffer[MAX_STRING_SIZE] = L"";
    if (!fgetws(buffer, MAX_STRING_SIZE, stdin))
        return AKIN_STR_READ_ERR;

    size_t string_len = wcslen(buffer);
    buffer[string_len - 1] = L'\0';
    Tree_node root = tree_get_root(akin->tree);

    akin_error err = akin_find_object(root, buffer, node, stk_ptr);

    return err;
}

static akin_error akin_describe_object(Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_describe_start_msg();

    stack stk = {};
    init_stack(stk);

    Tree_node node = NULL;
    akin_error err = akin_get_object(akin, &stk, &node);

    Tree_node root = tree_get_root(akin->tree);
    if (err == AKIN_NODE_FOUND)
    {
        akin_print_describe_success_msg(root, node);
        akin_describe_recurse(root, stk.data);
        err = AKIN_NO_ERR;
    }
    else
    {
        akin_print_describe_fail_msg();
    }
    destruct_stack(&stk);
    return err;
}
//~~~~~~~~~~~~~~~~~~~~~definition~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~COMPARE~~~~~~~~~~~~~~~~~~~~~~~~~~~
static Tree_node akin_compare_nodes_recurse(const Tree_node node, elem_t* data, const size_t ref_depth, size_t cur_depth)
{
    if (cur_depth == ref_depth)
        return node;

    akin_print_compare_match_msg(node, *data);

    Tree_node diff_node = NULL;
    if (*data == AKIN_TRUE_VALUE)
    {
        Tree_node left_child = tree_get_child_node(node, LEFT);
        diff_node = akin_compare_nodes_recurse(left_child, data++, ref_depth, cur_depth + 1);
    }
    else if(*data == AKIN_FALSE_VALUE)
    {
        Tree_node right_child = tree_get_child_node(node, RIGHT);
        diff_node = akin_compare_nodes_recurse(right_child, data++, ref_depth, cur_depth + 1);
    }
    return diff_node;
}

static Tree_node akin_compare_nodes(const Tree tree, elem_t* data, const size_t depth)
{
    return akin_compare_nodes_recurse(tree_get_root(tree), data, depth, 0);
}

static akin_error akin_get_dont_matched_properties(const size_t capacity, elem_t* data_1, elem_t* data_2, size_t* pos)
{
    if (!data_1 || !data_2)
        return AKIN_NULL_PTR;

    if (*pos < capacity)
    {
        if (data_1[*pos] != data_2[*pos])
            return AKIN_NO_ERR;

        (*pos)++;
    }
    return AKIN_NO_MATCH;
}

static akin_error akin_compare_objects(Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_compare_ask_first_object();
    stack obj_1_stk = {};
    init_stack(obj_1_stk);
    Tree_node obj_1_node = NULL;
    akin_error err_1 = akin_get_object(akin, &obj_1_stk, &obj_1_node);

    akin_print_compare_ask_second_object();
    stack obj_2_stk = {};
    init_stack(obj_2_stk);
    Tree_node obj_2_node = NULL;
    akin_error err_2 = akin_get_object(akin, &obj_2_stk, &obj_2_node);

    if (err_1 == AKIN_NODE_FOUND && err_2 == AKIN_NODE_FOUND)
    {
        err_1 = AKIN_NO_ERR;
        err_2 = AKIN_NO_ERR;
        size_t position = 0;
        size_t capacity = (obj_1_stk.size > obj_2_stk.size)? obj_2_stk.size : obj_1_stk.size;
        akin_get_dont_matched_properties(capacity, obj_1_stk.data, obj_2_stk.data, &position);
        Tree_node character_node = akin_compare_nodes(akin->tree, obj_1_stk.data, position);
        akin_print_compare_dont_match_msg(character_node, obj_1_node, obj_2_node);
    }
    else
    {
        akin_print_describe_fail_msg();
    }
    destruct_stack(&obj_1_stk);
    destruct_stack(&obj_2_stk);

    if (err_1 != AKIN_NO_ERR)
        return err_1;

    return err_2;
}
//~~~~~~~~~~~~~~~~~~~~~~compare~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_save(Akinator* akin)
{
    akin_print_ask_for_save();
    if (akin_get_answer())
    {
        if (!tree_save(akin->save_file_name, akin->tree))
            return AKIN_TREE_SAVE_ERR;
    }
    return AKIN_EXIT;
}

static akin_error akin_main(Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_menu();

    akin_error err = AKIN_NO_ERR;
    int input = akin_get_input();
    switch (input)
    {
        case L'у':
        {
            err = akin_guess(akin);
            if (err == AKIN_MEMORY_FULL)
                akin_print_memory_full_err();
            break;
        }
        case L'д': akin_dump_tree(akin);              break;
        case L'о': err = akin_describe_object(akin);
        {
            if (err == AKIN_NODE_FOUND)
                err = AKIN_NO_ERR;
            break;
        }
        case L'с': err = akin_compare_objects(akin);  break;
        case L'ы': err = akin_save(akin);             break;
          default: akin_print_main_wrong_input_msg(); break;
    }
    return err;
}

akin_error akin_play(const char* input_name, const char* output_name)
{
    int  in_prev_mode = setmode(fileno(stdin) , _O_U8TEXT);
    int out_prev_mode = setmode(fileno(stdout), _O_U8TEXT);

    Tree tree = tree_init();
    tree_lns_error tree_err = tree_load(input_name, tree);
    if (tree_err)
        return AKIN_TREE_READ_ERR;

    Akinator akin = {
                        .tree = tree,
                        .load_file_name = input_name,
                        .save_file_name = output_name,
                        .buffer_capacity = BUFFER_SIZE
                    };

    akin_error akin_err = AKIN_NO_ERR;

    while (akin_err == AKIN_NO_ERR)
        akin_err = akin_main(&akin);

    tree_destruct(tree);

    setmode(fileno(stdin) , in_prev_mode);
    setmode(fileno(stdout), out_prev_mode);

    return akin_err;
}
