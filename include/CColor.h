#ifndef _CColorLib_H_
#define _CColorLib_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <string.h>

    #define BUFFER_SIZE (1024)

    typedef enum sequences {RESET=0, BOLD=1, ITALIC=3, UNDERLINE=4}sequences;
    typedef enum colors {BLACK=30, RED=31, GREEN=32, YELLOW=33, BLUE=34, MAGENTA=35, CYAN=36, WHITE=37, B_BLACK=90, B_RED=91, B_GREEN=92, B_YELLOW=93, B_BLUE=94, B_MAGENTA=95, B_CYAN=96, B_WHITE=97}colors;
    typedef enum bg_colors {BG_BLACK=40, BG_RED=41, BG_GREEN=42, BG_YELLOW=43, BG_BLUE=44, BG_MAGENTA=45, BG_CYAN=46, BG_WHITE=47, BG_B_BLACK=100, BG_B_RED=101, BG_B_GREEN=102, BG_B_YELLOW=103, BG_B_BLUE=104, BG_B_MAGENTA=105, BG_B_CYAN=106, BG_B_WHITE=107}bg_colors;
    typedef enum position {FG, BG}position;

    int print_color(char * format, ...);
    static inline void CLEAR_FORMAT() {printf("\e[%dm", RESET);}
    static inline void NCLEAR_FORMAT() {printf("\e[%dm\n", RESET);}
    static inline void BIU(sequences biu) {printf("\e[%dm", biu);}
    static inline void COLOR(colors color) {printf("\e[%dm", color);}
    static inline void RGB(unsigned char r, unsigned char g, unsigned char b) {printf("\e[38;2;%d;%d;%dm", r, g, b);}
    static inline void BG_RGB(unsigned char r, unsigned char g, unsigned char b) {printf("\e[48;2;%d;%d;%dm", r, g, b);}
    static inline void BG_COLOR(bg_colors bg_color) {printf("\e[%dm", bg_color);}

#endif