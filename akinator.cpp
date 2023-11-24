#include <assert.h>
#include <fcntl.h>

#include "Libs/tree.h"
#include "Libs/tree_console_dump.h"
#include "Libs/logs.h"
#include "Libs/utils.h"
#include "tree_lns_lib.h"
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

static akin_error realloc_buffer(Akinator* akin)
{
    const size_t size = akin->buffer_size;
    const size_t capacity = akin->buffer_capacity;

    if ( size == capacity || size + MIN_STRING_SIZE > capacity)
    {
        wchar_t* buffer = (wchar_t*) realloc(akin->buffer, 2 * capacity);
        if (!buffer)
            return AKIN_MEM_ALLOC_ERR;
    }
    return AKIN_NO_ERR;
}

#define AKIN_RETURN_IF_ERR(ERROR) \
    do {                          \
        if (ERROR != AKIN_NO_ERR) \
            return ERROR;         \
    } while(0)

static akin_error allocate_word(Akinator* akin, wchar_t** string)
{
    assert(akin);

    akin_error err = realloc_buffer(akin);
    AKIN_RETURN_IF_ERR(err);

    wchar_t* buffer_ptr = akin->buffer + akin->buffer_size;
    const int free_size = akin->buffer_capacity - akin->buffer_size;

    size_t string_len = 0;
    while (string_len < 2)
    {
        if (!fgetws(buffer_ptr, free_size, stdin))
            return AKIN_STR_READ_ERR;
        string_len = wcslen(buffer_ptr);
    }

    *(buffer_ptr + string_len - 1) = L'\0';
    *string = buffer_ptr;
    akin->buffer_size += string_len;

    return AKIN_NO_ERR;
}

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

//~~~~~~~~~~~~~~~~~~~~GUESS_WHO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_guess(Akinator* akin, Tree_node node)
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
        return AKIN_NO_ERR;
    }

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    if (answer)
        return akin_guess(akin, left_child);

    return akin_guess(akin, right_child);
}

static akin_error akin_guess_main(Akinator* akin)
{
    // tree_verify();
    akin_print_guess_start_msg();
    akin_guess(akin, tree_get_root(akin->tree));
    return AKIN_NO_ERR;
}

//~~~~~~~~~~~~~~~~~~~~guess_who~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_save(Akinator* akin)
{
    if (!tree_save(akin->save_file_name, akin->tree))
        return AKIN_TREE_SAVE_ERR;

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
        case L'у': err = akin_guess_main(akin); break;
        case L'д': akin_dump_tree(akin); break;
        case L'ы': err = akin_save(akin); break;
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

    wchar_t* buffer = (wchar_t*) calloc(MIN_BUFFER_CAPACITY, sizeof(buffer[0]));
    if (!buffer)
        return AKIN_MEM_ALLOC_ERR;

    Akinator akin = {
                        .tree = tree,
                        .load_file_name = input_name,
                        .save_file_name = output_name,
                        .buffer = buffer,
                        .buffer_capacity = MIN_BUFFER_CAPACITY
                    };

    akin_error akin_err = AKIN_NO_ERR;

    while (akin_err == AKIN_NO_ERR)
        akin_err = akin_main(&akin);

    tree_destruct(tree);

    setmode(fileno(stdin) , in_prev_mode);
    setmode(fileno(stdout), out_prev_mode);
    FREE_AND_NULL(akin.buffer);

    return akin_err;
}
