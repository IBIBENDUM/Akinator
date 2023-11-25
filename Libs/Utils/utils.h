#ifndef UTILS_H
#define UTILS_H

#define FREE_AND_NULL(PTR) \
do{                        \
    free(PTR);             \
    PTR = NULL;            \
} while(0)

bool create_folder(const char* path);

#endif
