#include "termiArt.h"

void print_canvas(node_t * canvas, pen_t * pen, int width, int height){
    int row = 0;
    int column = 0;
    colors pen_color = BLACK;

    for(row=0; row<height; row++){
        for(column=0; column<width; column++){
            if(pen != NULL && pen->x == column && pen->y == row){
                if(MAX(MAX(canvas[row * width + column].r, canvas[row * width + column].g), canvas[row * width + column].b) < 200){
                    pen_color = WHITE;
                }
                else{
                    pen_color = BLACK;
                }
                if(NORMAL == pen->cursor_type){
                    printf("\e[48;2;%d;%d;%dm\e[%dm[]\e[0m", canvas[row * width + column].r, canvas[row * width + column].g, canvas[row * width + column].b, pen_color);
                }
                else if(FILL == pen->cursor_type){
                    printf("\e[48;2;%d;%d;%dm\e[%dm()\e[0m", canvas[row * width + column].r, canvas[row * width + column].g, canvas[row * width + column].b, pen_color);
                }
            }
            else{
                printf("\e[48;2;%d;%d;%dm  \e[0m", canvas[row * width + column].r, canvas[row * width + column].g, canvas[row * width + column].b);
            }
        }
        puts("");
    }
}

error_code_t change_pen_color(pen_t * pen){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    int error_check = 0;
    int difference = 0;
    char * input = NULL;

    error_check = change_echo(true);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    change_cursor(true);

    print_color("~~R:~ ", RED, BOLD, RESET);
    error_check = get_raw_input(NULL, &input);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_GET_INPUT;
        goto cleanup;
    }
    difference = strncmp("q", input, 2);
    if(0 == difference){
        return_value = ERROR_CODE_SUCCESS;
        goto cleanup;
    }
    pen->r = (int)strtol(input, NULL, 10);

    print_color("~~G:~ ", GREEN, BOLD, RESET);
    error_check = get_raw_input(NULL, &input);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_GET_INPUT;
        goto cleanup;
    }
    difference = strncmp("q", input, 2);
    if(0 == difference){
        return_value = ERROR_CODE_SUCCESS;
        goto cleanup;
    }
    pen->g = (int)strtol(input, NULL, 10);

    print_color("~~B:~ ", BLUE, BOLD, RESET);
    error_check = get_raw_input(NULL, &input);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_GET_INPUT;
        goto cleanup;
    }
    difference = strncmp("q", input, 2);
    if(0 == difference){
        return_value = ERROR_CODE_SUCCESS;
        goto cleanup;
    }
    pen->b = (int)strtol(input, NULL, 10);

    return_value = ERROR_CODE_SUCCESS;

cleanup:
    if(NULL != input){
        free(input);
    }

    error_check = change_echo(false);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
    }

    change_cursor(false);

    return return_value;
}

void draw(node_t * canvas, int width, pen_t pen){
    canvas[pen.y * width + pen.x].r = pen.r;
    canvas[pen.y * width + pen.x].g = pen.g;
    canvas[pen.y * width + pen.x].b = pen.b;
}

void fill(node_t * canvas, int width, int height, pen_t pen){
    int i = 0;
    int j = 0;

    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            canvas[i * width + j].r = pen.r;
            canvas[i * width + j].g = pen.g;
            canvas[i * width + j].b = pen.b;
        }
    }
}

error_code_t canvas_handler(){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    int width = 0;
    int height = 0;
    int error_check = 0;
    node_t * canvas = NULL;

    error_check = change_echo(true);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    print_color("~~Width of canvas:~ ", BLUE, BOLD, RESET);
    scanf("%d", &width);
    print_color("~~Height of canvas:~ ", BLUE, BOLD, RESET);
    scanf("%d", &height);

    canvas = calloc(width * height, sizeof(node_t));
    if(NULL == canvas){
        print_error("MAIN: Calloc error", 0);
        goto cleanup;
    }

    memset(canvas, 255, width * height * sizeof(node_t));

    pen_handler(canvas, width, height);

    return_value = ERROR_CODE_SUCCESS;

cleanup:
    if(NULL != canvas){
        free(canvas);
    }
    return return_value;
}

void query_color(node_t * canvas, int width, pen_t pen){
    printf("\n\e[33m\e[1mR: %3d G: %3d B: %3d\e[0m\n", canvas[pen.y * width + pen.x].r, canvas[pen.y * width + pen.x].g, canvas[pen.y * width + pen.x].b);
    printf("Press any key to continue ");
    getchar();
}

void get_color(node_t * canvas, int width, pen_t * pen){
    pen->r = canvas[pen->y * width + pen->x].r;
    pen->g = canvas[pen->y * width + pen->x].g;
    pen->b = canvas[pen->y * width + pen->x].b;
}

void fill_region(node_t * canvas, int width, int x1, int y1, pen_t pen){
    int i = 0;
    int j = 0;

    for(i=MIN(y1, pen.y); i<=MAX(y1, pen.y); i++){
        for(j=MIN(x1, pen.x); j<=MAX(x1, pen.x); j++){
            canvas[i * width + j].r = pen.r;
            canvas[i * width + j].g = pen.g;
            canvas[i * width + j].b = pen.b;
        }
    }
}

void help(){
    system("clear");

    print_color("~COMMANDS:~\n"
                "~h~: Help (prints this menu)\n"
                "~w,a,s,d:~ Moves the cursor\n"
                "~c:~ Change color (entering q will exit change color)\n"
                "~ (space):~ Draw\n"
                "~f:~ Type twice to fill entire canvas\n"
                "~/:~ Fill region (first hit sets start, second hit fills)\n"
                "~\\:~ Stops fill region (drawing will also stop)\n"
                "~?:~ Prints the selected pixel's color\n"
                "~.:~ Changes the pen color to the current selected color on the canvas\n"
                "~q:~ Quits drawing (allows save)\n\n"
                "Press any key to return to drawing\n", BOLD, RESET, BOLD, RESET, BOLD, BOLD, RESET, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET);
    
    getchar();
}

error_code_t pen_handler(node_t * canvas, int width, int height){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    int error_check = 0;
    int x1 = -1;
    int y1 = -1;
    int compression_length = 0;
    pen_t pen = {0};
    char input = 0;
    char * file_name = NULL;
    char * compressed = NULL;
    FILE * file = NULL;
    bool can_fill = false;
    bool run = true;

    error_check = change_echo(false);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    change_cursor(false);
    
    while(run){
        if(-1 == x1){
            pen.cursor_type = NORMAL;
        }

        system("clear");
        print_canvas(canvas, &pen, width, height);
        printf("%*s", width-2, "");
        print_color("`  ~\n", BG, pen.r, pen.g, pen.b, RESET);

        input = getchar();

        if('f' != input){
            can_fill = false;
        }
        
        switch(input){
            case 'w': pen.y--; break;
            case 's': pen.y++; break;
            case 'a': pen.x--; break;
            case 'd': pen.x++; break;
            case 'c': change_pen_color(&pen); break;
            case ' ': draw(canvas, width, pen); x1 = -1; break;
            case 'f': 
                if(can_fill){  
                    fill(canvas, width, height, pen); x1 = -1;
                    can_fill = false;
                }
                else{
                    can_fill = true;
                }
                break;
            case '?': query_color(canvas, width, pen); break;
            case '.': get_color(canvas, width, &pen); break;
            case '/': 
                if(x1 == -1){
                    x1 = pen.x;
                    y1 = pen.y;
                    pen.cursor_type = FILL;
                }
                else{
                    fill_region(canvas, width, x1, y1, pen);
                    x1 = -1;
                }
                break;
            case '\\': x1 = -1; pen.cursor_type = NORMAL; break;
            case 'h': help(); break;
            case 'q': run = false; break;
        }

        if(pen.x >= 0){
            pen.x %= width;
        }
        else{
            pen.x = (width + pen.x) % width;
        }
        if(pen.y >= 0){
            pen.y %= height;
        }
        else{
            pen.y = (height + pen.y) % height;
        }
    }

    error_check = change_echo(true);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    change_cursor(true);

    system("clear");
    print_canvas(canvas, &pen, width, height);

    print_color("\n~~Would you like to save this painting? ([y]/n)~ ", BLUE, BOLD, RESET);
    input = getchar();
    getchar();

    if('y' != input){
        print_color("\n~~Are you sure you don't want to save? ([y]/n)~ ", RED, BOLD, RESET);
        input = getchar();
        getchar();
        
        if('y' != input){
            return_value = ERROR_CODE_SUCCESS;
            goto cleanup;
        }
    }

    error_check = get_raw_input("File path: ", &file_name);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_GET_INPUT;
        goto cleanup;
    }

    compression_length = compress_canvas(canvas, width, height, &compressed);
    if(-1 == compression_length){
        return_value = ERROR_CODE_UNDEFINED;
        goto cleanup;
    }

    file = fopen(file_name, "w+");
    if(NULL == file){
        return_value = print_error("PEN_HANDLER: Fopen error", ERROR_CODE_COULDNT_OPEN);
        goto cleanup;
    }

    error_check = fwrite(&width, sizeof(width), 1, file);
    if(-1 == error_check){
        return_value = print_error("PEN_HANDLER: Fwrite error", ERROR_CODE_COULDNT_WRITE);
        goto cleanup;
    }

    error_check = fwrite(&height, sizeof(height), 1, file);
    if(-1 == error_check){
        return_value = print_error("PEN_HANDLER: Fwrite error", ERROR_CODE_COULDNT_WRITE);
        goto cleanup;
    }

    error_check = fwrite(compressed, 1, compression_length, file);
    if(-1 == error_check){
        return_value = print_error("PEN_HANDLER: Fwrite error", ERROR_CODE_COULDNT_WRITE);
        goto cleanup;
    }

    return_value = ERROR_CODE_SUCCESS;

cleanup:
    if(NULL != file_name){
        free(file_name);
    }
    if(NULL != compressed){
        free(compressed);
    }

    if(NULL != file){
        fclose(file);
    }
    return return_value;
}

error_code_t open_file(char * file_name, bool edit, bool is_compressed){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    int width = 0;
    int height = 0;
    node_t * canvas = NULL;
    char * compressed = NULL;

    return_value = get_canvas(file_name, &canvas, &width, &height, is_compressed);
    if(ERROR_CODE_SUCCESS != return_value){
        goto cleanup;
    }

    if(!edit){
        print_canvas(canvas, NULL, width, height);
        return_value = ERROR_CODE_SUCCESS;
    }
    else{
        return_value = pen_handler(canvas, width, height);
    }

cleanup:
    if(NULL != canvas){
        free(canvas);
    }
    if(NULL != compressed){
        free(compressed);
    }

    return return_value;
}

error_code_t get_canvas(char * file_name, node_t ** canvas, int * width, int * height, bool compressed){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    FILE * file = NULL;
    int error_check = 0;
    int canvas_pointer = 0;
    unsigned char run_length = 0;
    int i = 0;
    node_t node = {0};

    file = fopen(file_name, "r");
    if(NULL == file){
        return_value = print_error("OPEN_FILE: Fopen error", ERROR_CODE_COULDNT_OPEN);
        goto cleanup;
    }

    error_check = fread(width, sizeof(*width), 1, file);
    if(-1 == error_check){
        return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
        goto cleanup;
    }

    error_check = fread(height, sizeof(*height), 1, file);
    if(-1 == error_check){
        return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
        goto cleanup;
    }

    *canvas = calloc(*width * *height, sizeof(node_t));
    if(NULL == canvas){
        return_value = print_error("OPEN_FILE: Calloc error", ERROR_CODE_COULDNT_ALLOCATE_MEMORY);
        goto cleanup;
    }

    if(compressed){
        while(canvas_pointer < *width * *height){
            error_check = fread(&run_length, sizeof(run_length), 1, file);
            if(-1 == error_check){
                return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
                goto cleanup;
            }

            error_check = fread(&node, sizeof(node), 1, file);
            if(-1 == error_check){
                return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
                goto cleanup;
            }

            for(i=0; i<run_length; i++){
                memcpy(&((*canvas)[canvas_pointer + i]), &node, sizeof(node));
            }

            canvas_pointer += run_length;
        }
    }
    else{
        error_check = fread(*canvas, *height * *width, sizeof(node), file);
        if(-1 == error_check){
            return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
            goto cleanup;
        }
    }

    return_value = ERROR_CODE_SUCCESS;

cleanup:
    if(NULL != file){
        fclose(file);
    }

    return return_value;
}

int compress_canvas(node_t * canvas, int width, int height, char ** compression){
    int canvas_pointer = 0;
    int compression_length = 0;
    int i = 0;
    unsigned char run_length = 0;
    node_t first_node = {0};
    node_t current_node = {0};

    while(canvas_pointer < width * height){
        first_node = canvas[canvas_pointer];
        current_node = first_node;
        run_length = 0;

        while(current_node.r == first_node.r && current_node.g == first_node.g && current_node.b == first_node.b){
            run_length++;
            canvas_pointer++;
            current_node = canvas[canvas_pointer];
        }

        compression_length += 4 * sizeof(unsigned char);
        *compression = realloc(*compression, compression_length);
        if(NULL == *compression){
            compression_length = print_error("COMPRESS CANVAS: Realloc error", -1);
            goto cleanup;
        }


        (*compression)[compression_length - 4 * sizeof(unsigned char)] = run_length;
        memcpy(*compression + compression_length - 3*sizeof(unsigned char), &first_node, 3 * sizeof(unsigned char));
    }

cleanup:
    return compression_length;
}
