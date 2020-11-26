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
    int color = 0;

    error_check = change_echo(true);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    change_cursor(true);

    print_color("~~R:~ ", RED, BOLD, RESET);
    scanf("%d", &color);
    pen->r = color;
    print_color("~~G:~ ", GREEN, BOLD, RESET);
    scanf("%d", &color);
    pen->g = color;
    print_color("~~B:~ ", BLUE, BOLD, RESET);
    scanf("%d", &color);
    pen->b = color;

    error_check = change_echo(false);
    if(-1 == error_check){
        return_value = ERROR_CODE_COULDNT_CHANGE_ECHO;
        goto cleanup;
    }

    change_cursor(false);

    return_value = ERROR_CODE_SUCCESS;

cleanup:
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
                "~c:~ Change color\n"
                "~ (space):~ Draw\n"
                "~f:~ Type twice to fill entire canvas\n"
                "~/:~ Fill region (first hit sets start, second hit fills)\n"
                "~\\:~ Stops fill region (drawing will also stop)\n"
                "~?:~ Prints the selected pixel's color\n"
                "~q:~ Quits drawing (allows save)\n\n"
                "Press any key to return to drawing\n", BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET, BOLD, RESET);
    
    getchar();
}

error_code_t pen_handler(node_t * canvas, int width, int height){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    int error_check = 0;
    int x1 = -1;
    int y1 = -1;
    pen_t pen = {0};
    char input = 0;
    char * file_name = NULL;
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

    error_check = fwrite(canvas, sizeof(node_t), width * height, file);
    if(-1 == error_check){
        return_value = print_error("PEN_HANDLER: Fwrite error", ERROR_CODE_COULDNT_WRITE);
        goto cleanup;
    }

    return_value = ERROR_CODE_SUCCESS;

cleanup:
    if(NULL != file_name){
        free(file_name);
    }

    if(NULL != file){
        fclose(file);
    }
    return return_value;
}

error_code_t open_file(char * file_name, bool edit){
    error_code_t return_value = ERROR_CODE_UNINITIALIZED;
    FILE * file = NULL;
    int error_check = 0;
    int width = 0;
    int height = 0;
    node_t * canvas = NULL;

    file = fopen(file_name, "r");
    if(NULL == file){
        return_value = print_error("OPEN_FILE: Fopen error", ERROR_CODE_COULDNT_OPEN);
        goto cleanup;
    }

    error_check = fread(&width, sizeof(width), 1, file);
    if(-1 == error_check){
        return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
        goto cleanup;
    }

    error_check = fread(&height, sizeof(height), 1, file);
    if(-1 == error_check){
        return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
        goto cleanup;
    }

    canvas = calloc(width * height, sizeof(node_t));
    if(NULL == canvas){
        return_value = print_error("OPEN_FILE: Calloc error", ERROR_CODE_COULDNT_ALLOCATE_MEMORY);
        goto cleanup;
    }

    error_check = fread(canvas, sizeof(node_t), width * height, file);
    if(-1 == error_check){
        return_value = print_error("OPEN_FILE: Fread error", ERROR_CODE_COULDNT_READ);
        goto cleanup;
    }

    if(!edit){
        print_canvas(canvas, NULL, width, height);
    }
    else{
        pen_handler(canvas, width, height);
    }

cleanup:
    return return_value;
}
