DEF_TREE_ERR(TREE_NO_ERR, "There is no error")
DEF_TREE_ERR(TREE_NULL_PTR_ERR, "Pointer on tree is NULL")
DEF_TREE_ERR(TREE_NULL_NODE_PTR_ERR, "Node pointer is NULL")
DEF_TREE_ERR(TREE_NODE_IS_FULL, "Node has both children")
DEF_TREE_ERR(TREE_MEM_ALLOC_ERR, "Error at memory allocation")
DEF_TREE_ERR(TREE_TRAVERSE_ERR, "Error at traverse function")
DEF_TREE_ERR(TREE_FILE_CLOSE_ERR, "Error at file closing")
DEF_TREE_ERR(TREE_FILE_OPEN_ERR, "Error at file open")
DEF_TREE_ERR(TREE_NULL_TRAVERSE_FUNCTION_ERR, "Null traverse function pointer")
DEF_TREE_ERR(TREE_CREATE_LOG_FOLDER_ERR, "Error at file creating")
DEF_TREE_ERR(TREE_WRONG_SIZE_ERR, "Wrong tree size")

#undef DEF_TREE_ERR
