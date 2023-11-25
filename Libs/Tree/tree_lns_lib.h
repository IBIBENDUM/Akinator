#ifndef TREE_LNS_LIB_H_
#define TREE_LNS_LIB_H_

// LNS stands for Load aNd Save

#include <wchar.h>

const wchar_t NULL_CODE_NAME[] = L"_";
const size_t NULL_CODE_NAME_LEN = sizeof(NULL_CODE_NAME) / sizeof(NULL_CODE_NAME[0]) - 1;

enum tree_lns_error
{
    TREE_LNS_NO_ERR,
    TREE_LNS_TREE_NULL_PTR,
    TREE_LNS_NULL_FILE_NAME,
    TREE_LNS_NULL_BUFFER_PTR,
    TREE_LNS_FILE_READ_ERR,
    TREE_LNS_FILE_OPEN_ERR,
    TREE_LNS_FILE_CLOSE_ERR,
    TREE_LNS_WRONG_FILE_FORMAT,
};

tree_lns_error tree_load(const char* file_name, Tree tree);

tree_lns_error tree_save(const char* file_name, Tree tree);

#endif
