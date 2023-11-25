#ifndef TREE_CONFIG_H_
#define TREE_CONFIG_H_

// ~~~~~~~~~~~~~~~~~~~~~~~LIST_LOGS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const char* const LOG_FOLDER_NAME      =  "log";       ///< log folder name
const char* const LOG_IMGS_FOLDER_NAME =  "images";    ///< images folder name (this folder is nested in the logs folder)
const char* const LOG_DOTS_FOLDER_NAME =  "dot";       ///< dots   folder name (this folder is nested in the logs folder)
const char* const LOG_GRAPH_NAME       =  "graph";     ///< graph file (name without extension)
const char* const LOG_FILE_NAME        =  "tree_log";  ///< log file (name without extension)

const size_t IMAGE_WIDTH       = 500;                  ///< Images width in html file
const size_t MESSAGE_MAX_LEN   = 128;

// Graphviz graph colors
const int BG_COLOR             = 0xFFFFFF;
const int OBJ_FILL_COLOR       = 0xF5CDC1;
const int OBJ_OUTLINE_COLOR    = 0xA0694D;
const int QST_FILL_COLOR       = 0x778BEB;
const int QST_OUTLINE_COLOR    = 0x556EE6;
const int NEXT_COLOR           = 0x1695E2;
const int PREV_COLOR           = 0x29C56E;

// Html messages colors
const int MSG_ERROR_COLOR      = 0xFF0000;
const int MSG_TIME_COLOR       = 0x9DAABB;
const int MSG_LINE_INFO_COLOR  = 0x000000;

#endif
