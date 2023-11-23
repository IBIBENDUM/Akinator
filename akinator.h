#ifndef AKINATOR_H_
#define AKINATOR_H_

enum akin_error
{
    AKIN_NO_ERR,
    AKIN_NULL_FILE_NAME,
    AKIN_TREE_READ_ERR,
    AKIN_TREE_NULL_PTR,
    AKIN_WRONG_INPUT,
};

akin_error akin_play(const char* file_name);

#endif
