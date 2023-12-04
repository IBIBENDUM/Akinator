#include <assert.h>
#include <fcntl.h>

// TODO: debug/release mode should be switchable from outside of the project
#define STK_DEBUG
#include "../Libs/Stack/stack.h"
#include "../Libs/Stack/stack_logs.h"
#include "../Libs/Tree/tree.h"
#include "../Libs/Tree/tree_console_dump.h"
#include "../Libs/Tree/tree_lns_lib.h"
#include "../Libs/Logs/logs.h"
#include "../Libs/Utils/utils.h"
#include "../Includes/akinator.h"
#include "../Includes/akinator_ui.h"

#define ASYNC akin->speak_async
#define TREE  akin->tree

// TODO: naming is a bit confusing, maybe try clarifying what "object" is

// TODO: also, I think it would be clearer if you wrote this function in
//       terms of other, called "is_leaf", you get it.
static bool check_is_object_node(const Tree_node node)
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
    *string = buffer_ptr;

    size_t string_len = 0;
    while (string_len < 2) // BAH: Repeat if "\n" line
    {
        string_len = akin_get_obj_name(buffer_ptr);
    }
    akin->buffer_size += string_len;

    return AKIN_NO_ERR;
}

// TODO: seems generalizable, as always :)
//       Why not create bakhtiyar-error-handling.h?

//       Then such "propagator" could be written generically
//       as long as NO_ERROR corresponds to value 0
#define AKIN_RETURN_IF_ERR(ERROR) \
    do {                          \
        if (ERROR != AKIN_NO_ERR) \
            return ERROR;         \
    } while(0)

static akin_error akin_add_node(Akinator* akin, Tree_node node)
{
    assert(node);
    assert(akin);

    if (!TREE)
        return AKIN_TREE_NULL_PTR;

    akin_print_add_node_start_msg(ASYNC);

    akin_error err = AKIN_NO_ERR;

    wchar_t* left_string = NULL;

    // TODO: why not join these two?
    //       PROPAGATE_ERROR(allocate_word(akin, &left_string));
    err = allocate_word(akin, &left_string);
    AKIN_RETURN_IF_ERR(err);

    wchar_t* node_value = NULL;
    tree_error tree_err = tree_get_node_value(node, &node_value);
    if (tree_err != TREE_NO_ERR)
        return AKIN_TREE_ERR;

    akin_print_obj_question(ASYNC, left_string, node_value);

    wchar_t* question_string = NULL;
    err = allocate_word(akin, &question_string);
    AKIN_RETURN_IF_ERR(err);

    tree_set_node_value(node, question_string);
    tree_insert(TREE, &node, left_string);
    tree_insert(TREE, &node, node_value);

    tree_console_dump(tree_get_root(TREE));

    return AKIN_NO_ERR;
}
#undef AKIN_RETURN_IF_ERR

//~~~~~~~~~~~~~~~~~~~~~~~GUESS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TODO: just a thing to think about. What if tree gets really huge?

//       Let's say that for some reason it happens to be straight line? 
//       Then, in theory, it could overflow stack (and I know, it's 
//       unlikely since humans are the ones guessing and they won't
//       do it for so long), or could it?

//       How could this be rewritten without recursion? Also, try
//       to artificially overflow it. Can it be done? If not, why?

//       I think it a thing you should understand writing function
//       like this one :) You can DM me answer!
static akin_error akin_guess_recurse(Akinator* akin, const Tree_node node)
{
    if (!akin)
        return AKIN_NULL_PTR;

    if (!node)
        return AKIN_NULL_NODE_PTR;

    akin_print_guess_question(ASYNC, node);

    bool answer = akin_get_answer();

    if (check_is_object_node(node))
    {
        if (answer)
        {
            akin_print_guess_correct(ASYNC);
            return AKIN_NO_ERR;
        }
        else
            return akin_add_node(akin, node);
    }

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    if (answer)
        return akin_guess_recurse(akin, left_child);

    Tree_node right_child = tree_get_child_node(node, RIGHT);
    return akin_guess_recurse(akin, right_child);
}

static akin_error akin_guess(Akinator* akin)
{
    akin_print_guess_start_msg(ASYNC);
    return akin_guess_recurse(akin, tree_get_root(TREE));
}
//~~~~~~~~~~~~~~~~~~~~~~guess~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~DEFINITION~~~~~~~~~~~~~~~~~~~~~~~~~
static akin_error akin_get_object_recurse(const Tree_node node, wchar_t* ref, Tree_node* dest, stack* stk)
{
    if (!dest)
        return AKIN_NULL_NODE_PTR;

    if (!node)
        return AKIN_NODE_NOT_FOUND;

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
        push_stack(stk, AKIN_STACK_TRUE_VALUE);
        err = akin_get_object_recurse(left_child, ref, dest, stk);
        if (err == AKIN_NODE_FOUND)
            return err;

        int stk_value = 0; // I should probably fix stack after mentoring, because there are crinja
        pop_stack(stk, &stk_value);

        push_stack(stk, AKIN_STACK_FALSE_VALUE);
        Tree_node right_child = tree_get_child_node(node, RIGHT);
        err = akin_get_object_recurse(right_child, ref, dest, stk);
        if (err == AKIN_NODE_FOUND)
            return err;

        pop_stack(stk, &stk_value);
    }
    return err;
}

// TODO: seems like a lot of functions use DFS to get what they want,
//       can DFS be extracted from them and generalized?
static akin_error akin_describe_recurse(const Akinator* akin, const Tree_node node, elem_t* data)
{
    if (!data)
        return AKIN_NULL_PTR;

    if (!node || check_is_object_node(node))
        return AKIN_NO_ERR;

    akin_error err = AKIN_NO_ERR;
    akin_print_describe_msg(ASYNC, node, *data);
    if (*data == AKIN_STACK_TRUE_VALUE)
    {
        Tree_node left_child = tree_get_child_node(node, LEFT);
        err = akin_describe_recurse(akin, left_child, data + 1);
    }
    else if (*data == AKIN_STACK_FALSE_VALUE)
    {
        Tree_node right_child = tree_get_child_node(node, RIGHT);
        err = akin_describe_recurse(akin, right_child, data + 1);
    }
    return err;
}

static akin_error akin_get_object(const Akinator* akin, stack* stk_ptr, Tree_node* node)
{
    if (!akin)
        return AKIN_NULL_PTR;

    wchar_t buffer[MAX_STRING_SIZE] = L"";
    akin_get_obj_name(buffer);
    if (!buffer)
        return AKIN_STR_NULL_PTR;

    Tree_node root = tree_get_root(TREE);

    akin_error err = akin_get_object_recurse(root, buffer, node, stk_ptr);

    return err;
}

static akin_error akin_describe_object(const Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_describe_start_msg(ASYNC);

    Tree_node node = NULL;
    stack stk = {};
    init_stack(stk);
    akin_error err = akin_get_object(akin, &stk, &node);

    Tree_node root = tree_get_root(TREE);
    if (err == AKIN_NODE_FOUND)
    {
        akin_print_describe_success_msg(ASYNC, root, node);
        akin_describe_recurse(akin, root, stk.data);
        err = AKIN_NO_ERR;
    }
    else
    {
        akin_print_describe_fail_msg(ASYNC);
    }
    destruct_stack(&stk);
    return err;
}
//~~~~~~~~~~~~~~~~~~~~~definition~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~COMPARE~~~~~~~~~~~~~~~~~~~~~~~~~~~
static Tree_node akin_compare_nodes_recurse(const Akinator* akin, const Tree_node node, elem_t* data, const size_t ref_depth, size_t cur_depth)
{
    assert(akin);
    assert(node);
    assert(data);

    if (cur_depth == ref_depth)
        return node;

    akin_print_compare_match_msg(ASYNC, node, *data);

    Tree_node diff_node = NULL;
    if (*data == AKIN_STACK_TRUE_VALUE)
    {
        Tree_node left_child = tree_get_child_node(node, LEFT);
        diff_node = akin_compare_nodes_recurse(akin, left_child, data + 1, ref_depth, cur_depth + 1);
    }
    else if(*data == AKIN_STACK_FALSE_VALUE)
    {
        Tree_node right_child = tree_get_child_node(node, RIGHT);
        diff_node = akin_compare_nodes_recurse(akin, right_child, data + 1, ref_depth, cur_depth + 1);
    }
    return diff_node;
}

// TODO: if it's comparing two nodes, then where are them (nodes I mean)?
//       I think it's more like finds a node in a specific spot described by data.
static Tree_node akin_compare_nodes(const Akinator* akin, elem_t* data, const size_t depth)
{
    assert(akin);
    assert(data);

    return akin_compare_nodes_recurse(akin, tree_get_root(TREE), data, depth, 0);
}

// TODO: grammar: get unmatched
static akin_error akin_get_dont_matched_properties(const size_t capacity, elem_t* data_1, elem_t* data_2, size_t* pos)
{
    assert(pos);

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

static akin_error akin_compare_objects(const Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_compare_ask_first_object(ASYNC);

    // Initialize first stack with object path
    stack obj_1_stk = {};
    init_stack(obj_1_stk); // TODO: this just seems a bit too expensive. Can you, maybe, reuse stacks?
    Tree_node obj_1_node = NULL;
    akin_error err = AKIN_NO_ERR;
    err = akin_get_object(akin, &obj_1_stk, &obj_1_node);
    if (err != AKIN_NODE_FOUND)
    {
        akin_print_describe_fail_msg(ASYNC);
        destruct_stack(&obj_1_stk);
        return err;
    }

    // Initialize second stack with object path
    akin_print_compare_ask_second_object(ASYNC);
    stack obj_2_stk = {};
    init_stack(obj_2_stk);
    Tree_node obj_2_node = NULL;
    err = akin_get_object(akin, &obj_2_stk, &obj_2_node);

    if (err == AKIN_NODE_FOUND)
    {
        err = AKIN_NO_ERR;
        size_t position = 0;
        // TODO: can't you make an array thingy that stores it's size? So that unmatched_properties
        //       function can decide size itself:
        size_t capacity = (obj_1_stk.size > obj_2_stk.size)? obj_2_stk.size : obj_1_stk.size;
        akin_get_dont_matched_properties(capacity, obj_1_stk.data, obj_2_stk.data, &position); // Get the node where the differences start
        Tree_node character_node = akin_compare_nodes(akin, obj_1_stk.data, position);
        akin_print_compare_dont_match_msg(ASYNC, character_node, obj_1_node, obj_2_node);
    }
    else
    {
        akin_print_describe_fail_msg(ASYNC);
    }

    destruct_stack(&obj_1_stk);
    destruct_stack(&obj_2_stk);

    return err;
}
//~~~~~~~~~~~~~~~~~~~~~~compare~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_save(const Akinator* akin)
{
    if (akin->buffer_size != 0)  // If the tree has been changed ask about saving
    {
        akin_print_ask_for_save(ASYNC);
        if (akin_get_answer())
        {
            if (!tree_save(akin->load_file_name, TREE))
                return AKIN_TREE_SAVE_ERR;
        }
    }
    return AKIN_EXIT;
}

static akin_error akin_main(Akinator* akin)
{
    if (!akin)
        return AKIN_NULL_PTR;

    akin_print_menu();

    akin_error err = AKIN_NO_ERR;
    int input = akin_get_char();
    switch (input)
    {
        case L'у':
        {
            err = akin_guess(akin);
            if (err == AKIN_MEMORY_FULL)
                akin_print_memory_full_err(ASYNC);
            break;
        }
        case L'о':
        {
            err = akin_describe_object(akin);
            if (err == AKIN_NODE_FOUND || err == AKIN_NODE_NOT_FOUND)
                err = AKIN_NO_ERR;
            break;
        }
        case L'с':
        {
            err = akin_compare_objects(akin);
            if (err == AKIN_NODE_FOUND || err == AKIN_NODE_NOT_FOUND)
                err = AKIN_NO_ERR;
            break;
        }
        case L'д': akin_dump_tree(akin);                   break;
        case L'ы': err = akin_save(akin);                  break;
        default: akin_print_main_wrong_input_msg(ASYNC);   break;
    }
    return err;
}
#undef ASYNC

// Akinator tree must have zero or two subtrees
static akin_error akin_tree_verify_recurse(const Tree_node node)
{
    if (!node)
        return AKIN_NO_ERR;

    akin_error err = AKIN_NO_ERR;

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    err = akin_tree_verify_recurse(left_child);
    if (err != AKIN_NO_ERR)
        return err;

    Tree_node right_child = tree_get_child_node(node, RIGHT);
    err = akin_tree_verify_recurse(right_child);
    if (err != AKIN_NO_ERR)
        return err;

    if ((!left_child && !right_child) || (left_child && right_child))
        return AKIN_NO_ERR;

    return AKIN_TREE_READ_ERR;
}

static akin_error akin_tree_verify(const Tree tree)
{
    return akin_tree_verify_recurse(tree_get_root(tree));
}

akin_error akin_play(const char* input_name, const bool speak_async)
{
    Tree tree = tree_init();

    // BAH: handle errors with nested or separate ifs?
    if (tree)
    {
        tree_lns_error tree_read_err = tree_load(input_name, tree);
        if (tree_read_err == TREE_LNS_NO_ERR)
        {
            akin_error akin_tree_verify_err = akin_tree_verify(tree);
            if (akin_tree_verify_err == AKIN_NO_ERR)
            {
                #ifdef _WIN32
                int out_prev_mode = setmode(fileno(stdout), _O_U8TEXT);
                #endif

                Akinator akin = {
                        .tree = tree,
                        .load_file_name  = input_name,
                        .speak_async     = speak_async,
                        .buffer_capacity = BUFFER_SIZE
                    };

                akin_error akin_err = AKIN_NO_ERR;

                while (akin_err == AKIN_NO_ERR)
                    akin_err = akin_main(&akin);

                #ifdef _WIN32
                setmode(fileno(stdout), out_prev_mode);
                #endif

                return akin_err;
            }
        }
        akin_print_tree_load_err(speak_async);
        tree_destruct(tree);

        return AKIN_TREE_READ_ERR;
    }
    return AKIN_TREE_NULL_PTR;
}

