#include <assert.h>
#include <fcntl.h>

#include "Libs/tree.h"
#include "Libs/tree_text_logs.h"
#include "Libs/logs.h"
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

static akin_error akin_add_node(Tree tree)
{
    if (!tree)
        return AKIN_TREE_NULL_PTR;

    akin_print_add_node_start_msg();

}

//~~~~~~~~~~~~~~~~~~~~GUESS_WHO~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_guess(Tree tree, Tree_node node)
{
    // CHECKS...
    akin_print_guess_question();

    bool answer = akin_get_answer();

    if (check_is_object_node(node))
    {
        if (answer)
        {
            akin_guess_correct();
            return AKIN_NO_ERR;
        }
        else
            return akin_add_node();
    }

    Tree_node left_child  = tree_get_child_node(node, LEFT );
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    if (answer)
        return akin_guess(tree, right_child);
    else
        return akin_guess(tree, left_child);
}

static akin_error akin_guess_main(Tree tree)
{
    // tree_verify();
    akin_print_guess_start_msg();
    akin_guess(tree, tree_get_root(tree));
    return AKIN_NO_ERR;
}

//~~~~~~~~~~~~~~~~~~~~guess_who~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static akin_error akin_main(Tree tree)
{
    assert(tree);

    akin_print_menu();

    int input = akin_get_input();
    switch ((wchar_t)input)
    {
        case L'у': tree_text_dump(tree_get_root(tree)); break;
        case L'д': tree_text_dump(tree_get_root(tree)); break;
          default: return AKIN_WRONG_INPUT;
    }
    return AKIN_NO_ERR;
}

akin_error akin_play(const char* file_name)
{
    int  in_prev_mode = setmode(fileno(stdin) , _O_U8TEXT);
    int out_prev_mode = setmode(fileno(stdout), _O_U8TEXT);

    Tree tree = tree_init();
    tree_lns_error tree_err = tree_load(file_name, tree);
    if (tree_err)
        return AKIN_TREE_READ_ERR;

    akin_error akin_err = AKIN_NO_ERR;

    while (akin_err == AKIN_NO_ERR)
        akin_err = akin_main(tree);

    tree_destruct(tree);

    setmode(fileno(stdin) , in_prev_mode);
    setmode(fileno(stdout), out_prev_mode);

    return akin_err;
}
