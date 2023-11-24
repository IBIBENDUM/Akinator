#ifndef AKINATOR_UI_H_
#define AKINATOR_UI_H_

wchar_t akin_get_input();

bool akin_get_answer();

void akin_print_menu();

void akin_print_add_node_start_msg();

void akin_print_guess_start_msg();

void akin_print_word_wrong_input();

void akin_print_guess_correct();

void akin_print_obj_question(wchar_t* str_1, wchar_t* str_2);

void akin_print_guess_question(Tree_node node);

void akin_print_add_node_question();

void akin_print_main_wrong_input_msg();

void akin_dump_tree(Akinator* akin);

#endif
