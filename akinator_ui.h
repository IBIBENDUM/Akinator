#ifndef AKINATOR_UI_H_
#define AKINATOR_UI_H_

wchar_t akin_get_input();

bool akin_get_answer();

void akin_print_describe_msg(const Tree_node node, const int property_value);

void akin_print_compare_dont_match_msg(const Tree_node character_node, const Tree_node obj_1_node, const Tree_node obj_2_node);

void akin_print_compare_match_msg(const Tree_node node, int property_value);

void akin_print_ask_for_save();

void akin_print_compare_ask_second_object();

void akin_print_compare_ask_first_object();

void akin_print_menu();

void akin_print_add_node_start_msg();

void akin_print_guess_start_msg();

void akin_print_describe_start_msg();

void akin_print_memory_full_err();

void akin_print_word_wrong_input();

void akin_print_guess_correct();

void akin_print_obj_question(wchar_t* str_1, wchar_t* str_2);

void akin_print_describe_success_msg(const Tree_node root, const Tree_node node);

void akin_print_guess_question(Tree_node node);

void akin_print_add_node_question();

void akin_print_main_wrong_input_msg();

void akin_print_describe_fail_msg();

void akin_dump_tree(Akinator* akin);

#endif
