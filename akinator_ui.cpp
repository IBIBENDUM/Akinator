#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <assert.h>

#include "Libs/tree.h"
#include "Libs/tree_text_logs.h"
#include "Libs/colors.h"
#include "akinator.h"
#include "akinator_ui.h"

wchar_t akin_get_input()
{


    wchar_t input = getwchar();
    while (iswspace(input))
        input = getwchar();

    wchar_t ch = 0;
    do {
        ch = getwchar();
    } while (ch && ch != L'\n' && ch != WEOF);

    return towlower(input);
}

bool akin_get_answer()
{
    return (akin_get_input() == L'д');
}

//~~~~~~~~~~~~~~~~~~~~~PRINT_FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void akin_print_guess_correct()
{
    wprintf(L"🎉🎉🎉 Я угадал 🎉🎉🎉\n");
}

void akin_print_add_node_start_msg()
{
    wprintf(L"🤔 Хорошо, кто же это? 🤔\n");
}

void akin_print_obj_question(wchar_t* str_1, wchar_t* str_2)
{
    wprintf(L"Чем же %ls отличается от %ls?\n", str_1, str_2);
}

void akin_dump_tree(Akinator* akin)
{
    assert(akin);
    Tree_node root = tree_get_root(akin->tree);
    tree_text_dump(root);
}

void akin_print_guess_question(Tree_node node)
{
    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    wprintf(L"Оно %ls? ([д]а / [*]ет)\n", value);
}

void akin_print_guess_start_msg()
{
    wprintf(L"" PAINT_TEXT(COLOR_LIGHT_CYAN, "Приступим к угадыванию!\n"));
}

void akin_print_main_wrong_input_msg()
{
    wprintf(L"" PAINT_TEXT(COLOR_LIGHT_CYAN, "Неправильная опция, попробуй еще раз!\n"));
}

void akin_print_menu()
{
    wprintf(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    wprintf(L"Что сегодня?\n"
           L"[у] - угадывание\n"
           L"[д] - дерево\n"
           L"[о] - определение\n"
           L"[с] - сравнение\n"
           L"[ы] - ыйти\n");
    wprintf(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
