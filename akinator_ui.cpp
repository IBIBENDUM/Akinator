#include <wchar.h>
#include <wctype.h>
#include <assert.h>

#include "Libs/speak.h"
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

#ifdef _WIN32

#define PRINT_AND_SPEAK(COLOR, FORMAT, ...)                    \
    do {                                                       \
        wprintf(L"" PAINT_TEXT(COLOR, FORMAT), ##__VA_ARGS__); \
        speak(L"" FORMAT, ##__VA_ARGS__);                      \
    } while (0)

#else

#define PRINT_AND_SPEAK(COLOR, FORMAT, ...)                    \
    do {                                                       \
        wprintf(L"" PAINT_TEXT(COLOR, FORMAT), ##__VA_ARGS__); \
    } while (0)

#endif

//~~~~~~~~~~~~~~~~~~~~~PRINT_FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void akin_print_compare_ask_first_object()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Введи первый объект для сравнения: \n");
}

void akin_print_compare_ask_second_object()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Введи второй объект для сравнения: \n");
}

void akin_print_compare_match_msg(const Tree_node node, const int property_value)
{
    wchar_t* character_node_value = NULL;
    tree_get_node_value(node, &character_node_value);

    PRINT_AND_SPEAK(COLOR_STD, "Они оба ");
    if (property_value == AKIN_FALSE_VALUE)
        PRINT_AND_SPEAK(COLOR_STD, "не ");

    PRINT_AND_SPEAK(COLOR_STD, "%ls\n", character_node_value);

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

    PRINT_AND_SPEAK(COLOR_STD, "%ls отличается от %ls тем, что один из них %ls\n",
                                obj_1_node_value, obj_2_node_value, character_node_value);
}

void akin_print_describe_success_msg(const Tree_node root, const Tree_node node)
{
    assert(root);
    assert(node);

    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    PRINT_AND_SPEAK(COLOR_STD, "%ls нашелся!\nОн ", value);
}

void akin_print_describe_msg(const Tree_node node, const int property_value)
{
    assert(node);
    assert(node);

    wchar_t* value = NULL;
    tree_get_node_value(node, &value);
    if (property_value == AKIN_FALSE_VALUE)
        PRINT_AND_SPEAK(COLOR_STD, "не ");

    PRINT_AND_SPEAK(COLOR_STD, "%ls;\n", value);
}

void akin_print_describe_fail_msg()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "В базе не было этого элемента!\n");
}

void akin_print_guess_correct()
{
    wprintf(L"🎉🎉🎉 ");
    PRINT_AND_SPEAK(COLOR_STD, "Я угадал");
    wprintf(L"🎉🎉🎉\n");
}

void akin_print_add_node_start_msg()
{
    wprintf(L"🤔 ");
    PRINT_AND_SPEAK(COLOR_STD, "Хорошо, кто же это?");
    wprintf(L" 🤔\n");
}

void akin_print_memory_full_err()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Память для строки заполнена, выйди и зайди нормально!\n");
}

void akin_print_obj_question(wchar_t* str_1, wchar_t* str_2)
{
    PRINT_AND_SPEAK(COLOR_STD, "Чем же %ls отличается от %ls?\n", str_1, str_2);
}

void akin_print_describe_start_msg()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Давай дам определение!\nВведи слово: ");
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
    PRINT_AND_SPEAK(COLOR_STD, "Это %ls?", value);
    wprintf(L" ([д]а / [*]ет)\n");
}

void akin_print_ask_for_save()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Перезаписать дерево?");
    wprintf(L" ([д]а / [*]ет)\n");
}

void akin_print_guess_start_msg()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Приступим к угадыванию!\n");
}

void akin_print_main_wrong_input_msg()
{
    PRINT_AND_SPEAK(COLOR_LIGHT_CYAN, "Неправильная опция, попробуй еще раз!\n");
}

void akin_print_menu()
{
    wprintf(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    wprintf(L"😏😏😏 Что сегодня? 😏😏😏\n");
    wprintf(L"[у] - угадывание 🌐\n"
            L"[д] - дерево 🌳\n"
            L"[о] - определение ☝\n"
            L"[с] - сравнение 💕\n"
            L"[ы] - ыйти 😖\n");
    wprintf(L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
