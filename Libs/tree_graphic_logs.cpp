#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "logs.h"
#include "utils.h"
#include "time_utils.h"

#include "tree.h"
#include "tree_graphic_logs.h"
#include "tree_config.h"

#define print(...)                 fwprintf(file_ptr, __VA_ARGS__)
#define log(...)                   fwprintf(log_file_ptr, __VA_ARGS__)
#define log_color(COLOR, STR, ...) fwprintf(log_file_ptr, "<font color = #%06X>" STR "</font>", COLOR, ##__VA_ARGS__)

#define COLOR_FORMAT "%06X"
#define NODE_SETTINGS_FORMAT L"[color = \"#" COLOR_FORMAT "\", weight = %d]\n"

const  size_t GRAPH_MAX_PATH_LEN = 256;
static FILE* log_file_ptr        = NULL;
static size_t graph_idx          = 1;
bool   log_is_enabled            = 1;

void tree_set_log_status(const bool status)
{
    log_is_enabled = status;
}

// TODO: make common function for node creation
static void write_node_info(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);
    assert(node);

    print(L"\"{PTR: 0x%llX |", (long long unsigned) node);

    tree_elem_t value = 0;
    tree_get_node_value(node, &value);
    print(L"VALUE: %ls | ", value);

    Tree_node left_child = tree_get_child_node(node, LEFT);
    if (left_child)
        print(L"{<left> LEFT: 0x%llX |", (long long unsigned) left_child);
    else
        print(L"{<left> LEFT: NULL |");

    Tree_node right_child = tree_get_child_node(node, RIGHT);
    if (right_child)
        print(L"<right> RIGHT: 0x%llX}}\"];\n", (long long unsigned) right_child);
    else
        print(L"<right> RIGHT: NULL}}\"];\n");
}

static void write_node_settings(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);
    assert(node);

    Tree_node left_child  = tree_get_child_node(node, LEFT);
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    int fill_color    = 0;
    int outline_color = 0;
    if (left_child && right_child)
    {
        fill_color    = QST_FILL_COLOR;
        outline_color = QST_OUTLINE_COLOR;
    }
    else
    {
        fill_color    = OBJ_FILL_COLOR;
        outline_color = OBJ_OUTLINE_COLOR;
    }
    print(L"[shape = Mrecord, style = filled, fillcolor = \"#"\
          COLOR_FORMAT "\", color = \"#" COLOR_FORMAT "\", label = ",
          fill_color, outline_color);
}

static void write_nodes(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);

    if (!node)
        return;

    print(L"\"%llX\" ", (long long unsigned) node);
    write_node_settings(file_ptr, node);
    write_node_info(file_ptr, node);

    write_nodes(file_ptr, tree_get_child_node(node, LEFT));
    write_nodes(file_ptr, tree_get_child_node(node, RIGHT));
}

static void link_node_with_child(FILE* file_ptr, const Tree_node node, const Tree_node child_node)
{
    assert(file_ptr);
    assert(node);
    assert(child_node);

    Tree_node child_parent_node = tree_get_parent_node(child_node);
    if (child_parent_node == node)
    {
        print(L"[dir = both]");
    }
    else
    {
        print(L"\"%llX\":<left> -> \"%llX\"", (long long unsigned) child_node, (long long unsigned) child_parent_node);
        print(NODE_SETTINGS_FORMAT, NEXT_COLOR, 1);
    }
}

static void link_nodes(FILE* file_ptr, const Tree_node node)
{
    assert(file_ptr);
    if (!node)
        return;

    Tree_node left_child  = tree_get_child_node(node, LEFT);
    Tree_node right_child = tree_get_child_node(node, RIGHT);

    if (left_child)
    {
        print(L"\"%llX\":<left> -> \"%llX\"", (long long unsigned) node, (long long unsigned) left_child);
        print(NODE_SETTINGS_FORMAT, NEXT_COLOR, 1);
        link_node_with_child(file_ptr, node, left_child);
    }
    if (right_child)
    {
        print(L"\"%llX\":<right> -> \"%llX\"", (long long unsigned) node, (long long unsigned) right_child);
        print(NODE_SETTINGS_FORMAT, NEXT_COLOR, 1);
        link_node_with_child(file_ptr, node, right_child);
    }
    link_nodes(file_ptr, left_child);
    link_nodes(file_ptr, right_child);

    print(L"\n");
}

static void write_head_info(FILE* file_ptr, const Tree tree)
{
    assert(file_ptr);
    assert(tree);

    print(L"\"{ROOT_PTR: 0x%llX | SIZE: %llu}\"]",
          (long long unsigned)tree_get_root(tree), tree_get_size(tree));
}

static void write_header_info(FILE* file_ptr, const Tree tree)
{
    assert(tree);
    assert(file_ptr);

    print(L"HEADER");
    print(L"[shape = Mrecord, style = filled, fillcolor = \"#"\
          COLOR_FORMAT "\", color = \"#" COLOR_FORMAT "\", label = ",
          OBJ_FILL_COLOR, OBJ_OUTLINE_COLOR);
    write_head_info(file_ptr, tree);
    print(L"\n");
}

static tree_error create_log_folders()
{
    char log_imgs_folder_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(log_imgs_folder_path, "%s", LOG_FOLDER_NAME);
    if (!create_folder(log_imgs_folder_path))
        return TREE_CREATE_LOG_FOLDER_ERR;

    sprintf(log_imgs_folder_path, "%s/%s", LOG_FOLDER_NAME, LOG_IMGS_FOLDER_NAME);
    if (!create_folder(log_imgs_folder_path))
        return TREE_CREATE_LOG_FOLDER_ERR;

    sprintf(log_imgs_folder_path, "%s/%s", LOG_FOLDER_NAME, LOG_DOTS_FOLDER_NAME);

    if (!create_folder(log_imgs_folder_path))
        return TREE_CREATE_LOG_FOLDER_ERR;

    return TREE_NO_ERR;
}
static void write_graph_label(FILE* file_ptr, log_call_line_info* line_info, const char* message)
{
    assert(file_ptr);
    assert(line_info);

    const char* time_str = cast_time_to_str(line_info->time);
    print(L"[label = \"");
    print(L"[%s] ", time_str);
    print(L"%s:%s:%d: ", line_info->file, line_info->func, line_info->line);
    print(L"%s", message);
    print(L"\"]\n");
}

static void write_graph_header(FILE* file_ptr, log_call_line_info* line_info, const char* message)
{
    print(L"digraph G{\n");
    print(L"rankdir = TB;\n");
    print(L"graph [splines = ortho]");
    write_graph_label(file_ptr, line_info, message);
    print(L";\n");
    print(L"bgcolor = \"#" COLOR_FORMAT "\";\n", BG_COLOR);
}

static tree_error open_graph_file(FILE** file_ptr)
{
    char dot_file_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(dot_file_path, "%s/%s/%s_%02llu.dot", LOG_FOLDER_NAME, LOG_DOTS_FOLDER_NAME, LOG_GRAPH_NAME, graph_idx);
    *file_ptr = fopen(dot_file_path, "w");
    #ifdef _WIN32
    setmode(fileno(*file_ptr), _O_U8TEXT);
    #endif
    if (!*file_ptr)
        return TREE_FILE_OPEN_ERR;

    return TREE_NO_ERR;
}

static void clear_log_files()
{
    char command_string[GRAPH_MAX_PATH_LEN] = "";

    #ifdef _WIN32
        sprintf(command_string, "del /s /q %s", LOG_FOLDER_NAME);
        system(command_string);
    #else
        sprintf(command_string, "rm -r %s", LOG_FOLDER_NAME);
        system(command_string);
    #endif
}

static tree_error create_dot_file(const Tree tree, log_call_line_info* line_info, const char* message)
{
    assert(line_info);
    assert(tree);
    tree_error err = TREE_NO_ERR;

    FILE* file_ptr = NULL;
    err = open_graph_file(&file_ptr);
    if (err != TREE_NO_ERR)
        return err;

    Tree_node root = tree_get_root(tree);
    write_graph_header(file_ptr, line_info, message);
    write_header_info(file_ptr, tree);
    write_nodes(file_ptr, root);
    link_nodes(file_ptr, root);

    print(L"}\n");

    if (fclose(file_ptr))
        return TREE_FILE_CLOSE_ERR;

    return TREE_NO_ERR;
}

tree_error tree_open_log_file()
{
    tree_error err = create_log_folders();
    if (err != TREE_NO_ERR)
        return err;

    clear_log_files();

    char log_file_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(log_file_path, "%s/%s.html", LOG_FOLDER_NAME, LOG_FILE_NAME);

    FILE* file_ptr = fopen(log_file_path, "w");
    if (!file_ptr)
        return TREE_FILE_OPEN_ERR;

    log_file_ptr = file_ptr;

    #ifdef _WIN32
    int prev_mode = setmode(fileno(log_file_ptr), _O_U8TEXT);
    #endif

    log(L"<pre>\n");

    return TREE_NO_ERR;
}

tree_error tree_close_log_file()
{
    if (fclose(log_file_ptr))
        return TREE_FILE_CLOSE_ERR;

    return TREE_NO_ERR;
}

static void compile_dot()
{
    char terminal_command[GRAPH_MAX_PATH_LEN] = "";

    char dot_file_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(dot_file_path, "%s/%s/%s_%02llu.dot", LOG_FOLDER_NAME, LOG_DOTS_FOLDER_NAME, LOG_GRAPH_NAME, graph_idx);
    size_t symbols_amount = sprintf(terminal_command, "dot -Tsvg %s ", dot_file_path);

    char svg_file_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(svg_file_path, "%s/%s/%s_%02llu.svg", LOG_FOLDER_NAME, LOG_IMGS_FOLDER_NAME, LOG_GRAPH_NAME, graph_idx);
    sprintf(terminal_command + symbols_amount, "-o %s", svg_file_path);

    system(terminal_command);
}

static void add_image_to_log()
{
    char svg_file_path[GRAPH_MAX_PATH_LEN] = "";
    sprintf(svg_file_path, "%s/%s_%02llu.svg", LOG_IMGS_FOLDER_NAME, LOG_GRAPH_NAME, graph_idx);

    log(L"\n<img src = \"%s\", width = \"%llu\">\n", svg_file_path, IMAGE_WIDTH);
}

static tree_error write_graph(const Tree tree, log_call_line_info* line_info, const char* message)
{
    assert(tree);

    tree_error err = create_dot_file(tree, line_info, message);
    if (err != TREE_NO_ERR)
        return err;

    compile_dot();
    add_image_to_log();

    graph_idx++;

    return TREE_NO_ERR;
}

static void write_event_header(log_call_line_info* line_info, const char* message)
{
    assert(line_info);

    const char* time_str = cast_time_to_str(line_info->time);
    log_color(MSG_TIME_COLOR, L"[%s] ", time_str);
    log_color(MSG_LINE_INFO_COLOR, L"%s:%s:%d: ", line_info->file, line_info->func, line_info->line);
    log_color(MSG_LINE_INFO_COLOR, L"%s", message);
}

tree_error tree_log_(const Tree tree, const char* message, log_call_line_info* line_info)
{
    assert(line_info);

    if(!log_is_enabled)
        return TREE_NO_ERR;

    if (!tree)
    {
        log_color(MSG_ERROR_COLOR, L"ERROR: NULL tree pointer\n");
        return TREE_NULL_PTR_ERR;
    }
    write_event_header(line_info, message);
    write_graph(tree, line_info, message);

    log(L"\n\n");

    return TREE_NO_ERR;
}
