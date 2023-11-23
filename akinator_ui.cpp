#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

#include "Libs/tree.h"
#include "akinator_ui.h"

wchar_t akin_get_input()
{
    int prev_mode = setmode(fileno(stdin), _O_U8TEXT);

    wchar_t input = getwchar();

    wchar_t ch = 0;
    do {
        ch = getwchar();
    } while (ch && ch != L'\n' && ch != EOF);

    setmode(fileno(stdin), prev_mode);

    return towlower(input);
}

bool akin_get_answer()
{
    return (akin_get_input() == L'д');
}

//~~~~~~~~~~~~~~~~~~~~~PRINT_FUNCTIONS~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void akin_print_add_node_start_msg()
{
    printf("Хорошо, кто же это?");
}

void akin_print_guess_question(Tree_node node)
{
    printf("Оно %ls? ([д]а / [*]ет", tree_get_node_value(node));
}

void akin_print_guess_start_msg()
{
    printf("Приступим к угадыванию!");
}

void akin_print_menu()
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Что сегодня?\n"
           "[у] - угадывание\n"
           "[д] - дерево\n"
           "[о] - определение\n"
           "[с] - сравнение\n"
           "[ы] - ыйти\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
