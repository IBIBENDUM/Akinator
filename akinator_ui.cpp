#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <assert.h>

#include "Libs/stack.h"
#include "Libs/tree.h"
#include "Libs/tree_console_dump.h"
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

    return input;
}

bool akin_get_answer()
{
    return (akin_get_input() == L'д');
}

//~~~~~~~~~~~~~~~~~~~~~PRINT_FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void akin_print_compare_ask_first_object()
{
    wprintf(L"Введи первое слово для сравнения: \n");
}

void akin_print_compare_ask_second_object()
{
    wprintf(L"Введи второе слово для сравнения: \n");
}

void akin_print_compare_match_msg(const Tree_node node, const int property_value)
{
    wchar_t* character_node_value = NULL;
    tree_get_node_value(node, &character_node_value);

    wprintf(L"Они оба ", character_node_value);
    if (property_value == AKIN_FALSE_VALUE)
        wprintf(L"не ");
    wprintf(L"%ls\n", character_node_value);

}

void akin_print_compare_dont_match_msg(const Tree_node character_node, const Tree_node obj_1_node, const Tree_node obj_2_node)
{
    assert(character_node);
    assert(obj_1_node);
    assert(obj_2_node);

    wchar_t* character_node_value = NULL;
    tree_get_node_value(character_node, &character_node_value);

    wchar_t* obj_1_node_value = NULL;
    tree_get_node_value(obj_1_node, &obj_1_node_value);

    wchar_t* obj_2_node_value = NULL;
    tree_get_node_value(obj_2_node, &obj_2_node_value);

    wprintf(L"%ls отличается от %ls тем, что один из них %ls\n",
            obj_1_node_value, obj_2_node_value, character_node_value);
}

void akin_print_describe_success_msg(const Tree_node root, const Tree_node node)
{
    assert(root);
    assert(node);

    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    wprintf(L"%ls нашелся!\nОн ", value);
}

void akin_print_describe_msg(const Tree_node node, const int property_value)
{
    assert(node);
    assert(node);

    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    if (property_value == AKIN_FALSE_VALUE)
        wprintf(L"не ");

    wprintf(L"%ls;\n", value);
}
void akin_print_describe_fail_msg()
{
    wprintf(L"В базе не было этого элемента!\n");
}

void akin_print_guess_correct()
{
    wprintf(L"🎉🎉🎉 Я угадал 🎉🎉🎉\n");
}

void akin_print_add_node_start_msg()
{
    wprintf(L"🤔 Хорошо, кто же это? 🤔\n");
}

void akin_print_memory_full_err()
{
    wprintf(L"Память для строки заполнена, выйди и зайди нормально!\n");
}

void akin_print_obj_question(wchar_t* str_1, wchar_t* str_2)
{
    wprintf(L"Чем же %ls отличается от %ls?\n", str_1, str_2);
}

void akin_print_describe_start_msg()
{
    wprintf(L"" PAINT_TEXT(COLOR_LIGHT_CYAN, "Давай дам определение!\n"));
    wprintf(L"" PAINT_TEXT(COLOR_LIGHT_CYAN, "Введи слово: "));
}

void akin_dump_tree(Akinator* akin)
{
    assert(akin);
    Tree_node root = tree_get_root(akin->tree);
    tree_console_dump(root);
}

void akin_print_guess_question(Tree_node node)
{
    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    wprintf(L"Оно %ls? ([д]а / [*]ет)\n", value);
}

void akin_print_ask_for_save()
{
    wprintf(L"Перезаписать дерево? ([д]а / [*]ет)\n");
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
