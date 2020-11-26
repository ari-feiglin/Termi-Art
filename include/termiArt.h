#include "CColor.h"
#include "standard.h"

typedef enum cursor_type_s {NORMAL, FILL}cursor_type_t;

typedef struct node_s{
    unsigned char r;
    unsigned char g;
    unsigned char b;
}node_t;

typedef struct pen_s{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    int x;
    int y;
    cursor_type_t cursor_type;
}pen_t;

error_code_t canvas_handler();
error_code_t pen_handler(node_t * canvas, int width, int height);
void print_canvas(node_t * canvas, pen_t * pen, int width, int height);
error_code_t open_file(char * file_name, bool edit);

