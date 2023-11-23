#ifndef TREE_FILE_LIB_H_
#define TREE_FILE_LIB_H_

#include <wchar.h>

const wchar_t NULL_CODE_NAME[] = L"_";
const size_t NULL_CODE_NAME_LEN = sizeof(NULL_CODE_NAME) / sizeof(NULL_CODE_NAME[0]) - 1;

enum tfl_error
{
    TFL_NO_ERR,
    TFL_TREE_NULL_PTR,
    TFL_NULL_FILE_NAME,
    TFL_NULL_BUFFER_PTR,
    TFL_FILE_READ_ERR,
    TFL_FILE_OPEN_ERR,
    TFL_FILE_CLOSE_ERR,
    TFL_WRONG_FILE_FORMAT,
};

tfl_error tree_load(const char* file_name, Tree tree);

tfl_error tree_save(const char* file_name, Tree tree);

#endif
