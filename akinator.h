#ifndef AKINATOR_H_
#define AKINATOR_H_

enum akin_error
{
    AKIN_NO_ERR,
    AKIN_NULL_PTR,
    AKIN_EXIT,
    AKIN_NULL_FILE_NAME,
    AKIN_NULL_NODE_PTR,
    AKIN_TREE_READ_ERR,
    AKIN_TREE_SAVE_ERR,
    AKIN_TREE_NULL_PTR,
    AKIN_WRONG_INPUT,
    AKIN_MEM_ALLOC_ERR,
    AKIN_STR_READ_ERR,
    AKIN_TREE_ERR,
};

const size_t MIN_BUFFER_CAPACITY = 32;
const size_t MIN_STRING_SIZE = 10;
const size_t CAPACITY_MULTIPLIER = 2;

struct Akinator
{
    Tree tree;
    const char* load_file_name;
    const char* save_file_name;
    wchar_t* buffer;
    size_t buffer_size;
    size_t buffer_capacity;
};

akin_error akin_play(const char* input_name, const char* output_name);

#endif
