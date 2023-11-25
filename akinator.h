#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "Libs/tree.h"

enum akin_error
{
    AKIN_NO_ERR,
    AKIN_NULL_PTR,
    AKIN_EXIT,
    AKIN_NODE_FOUND,
    AKIN_NO_MATCH,
    AKIN_NULL_FILE_NAME,
    AKIN_NULL_NODE_PTR,
    AKIN_TREE_READ_ERR,
    AKIN_TREE_SAVE_ERR,
    AKIN_TREE_NULL_PTR,
    AKIN_STR_NULL_PTR,
    AKIN_WRONG_INPUT,
    AKIN_MEM_ALLOC_ERR,
    AKIN_STR_READ_ERR,
    AKIN_TREE_ERR,
    AKIN_MEMORY_FULL,
};

const size_t MIN_BUFFER_CAPACITY = 32;
const size_t MAX_STRING_SIZE = 30;
const size_t BUFFER_SIZE = 150;

const int AKIN_TRUE_VALUE = 1;
const int AKIN_FALSE_VALUE = 0;

struct Akinator
{
    Tree        tree;
    const char* load_file_name;
    bool        speak_async;
    wchar_t     buffer[BUFFER_SIZE];
    size_t      buffer_size;
    size_t      buffer_capacity;
};

akin_error akin_play(const char* input_name, bool speak_async);

#endif
