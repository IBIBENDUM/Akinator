#ifndef AKINATOR_UI_H_
#define AKINATOR_UI_H_

wchar_t akin_get_input();

bool akin_get_answer();

void akin_print_describe_msg(bool async, const Tree_node node, const int property_value);

void akin_print_compare_dont_match_msg(bool async, const Tree_node character_node, const Tree_node obj_1_node, const Tree_node obj_2_node);

void akin_print_compare_match_msg(bool async, const Tree_node node, int property_value);

void akin_print_ask_for_save(bool async);

void akin_print_compare_ask_second_object(bool async);

void akin_print_compare_ask_first_object(bool async);

void akin_print_menu();

void akin_print_add_node_start_msg(bool async);

void akin_print_guess_start_msg(bool async);

void akin_print_describe_start_msg(bool async);

void akin_print_memory_full_err(bool async);

void akin_print_guess_correct(bool async);

void akin_print_obj_question(bool async, wchar_t* str_1, wchar_t* str_2);

void akin_print_describe_success_msg(bool async, const Tree_node root, const Tree_node node);

void akin_print_guess_question(bool async, Tree_node node);

void akin_print_main_wrong_input_msg(bool async);

void akin_print_describe_fail_msg(bool async);

void akin_dump_tree(const Akinator* akin);

#endif
