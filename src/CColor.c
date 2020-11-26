#include "CColor.h"

int print_color(char * format, ...){
    char dformatter = '~';
    char rgbformatter = '`';
    va_list ap;
    int string_len = 0;
    int i = 0;
    int sequence = 0;
    position place = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int bytes_printed = 0;

    string_len = strnlen(format, BUFFER_SIZE);
    if(-1 == string_len){
        goto cleanup;
    }

    va_start(ap, format);

    for(i=0; i<string_len; i++){
        if(format[i] == dformatter){
            sequence = va_arg(ap, int);
            bytes_printed += printf("\e[%dm", sequence);
        }
        else if(format[i] == rgbformatter){
            place = va_arg(ap, int);
            r = va_arg(ap, int);
            g = va_arg(ap, int);
            b = va_arg(ap, int);
            
            switch(place){
            case FG: bytes_printed += printf("\e[38;2;%d;%d;%dm", r, g, b); break;
            case BG: bytes_printed += printf("\e[48;2;%d;%d;%dm", r, g, b); break;
            }
        }
        else{
            bytes_printed += printf("%c", format[i]);
        }
    }
    va_end(ap);
    
cleanup:
    return bytes_printed;
}
