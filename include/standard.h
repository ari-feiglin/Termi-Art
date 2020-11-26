#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE (1024)

#define IN
#define OUT

typedef enum _bool {false=0, true=1}bool;

typedef enum error_code_s {
    ERROR_CODE_UNINITIALIZED = -1,
    ERROR_CODE_SUCCESS = 0,

    ERROR_CODE_EOF,

    ERROR_CODE_COULDNT_OPEN,
    ERROR_CODE_COULDNT_CREATE, 
    ERROR_CODE_COULDNT_RENAME,
    ERROR_CODE_COULDNT_TRUNCATE,
    ERROR_CODE_COULDNT_READ,
    ERROR_CODE_COULDNT_WRITE,
    ERROR_CODE_COULDNT_LSEEK,
    ERROR_CODE_COULDNT_CHMOD,
    ERROR_CODE_COULDNT_GET_PATH,
    ERROR_CODE_COULDNT_GET_STAT,

    ERROR_CODE_COULDNT_ALLOCATE_MEMORY,
    ERROR_CODE_COULDNT_SPRINTF,
    ERROR_CODE_ALREADY_EXISTS,
    ERROR_CODE_COULDNT_GET_STRLEN,

    ERROR_CODE_COULDNT_CHANGE_ECHO,
    ERROR_CODE_COULDNT_GET_INPUT,

    ERROR_CODE_INVALID_INPUT,
    ERROR_CODE_UNDEFINED,
    ERROR_CODE_UNKNOWN
}error_code_t;

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x > y) ? y : x)

static inline int print_error(const char * error_message, int return_value){
    printf("%s: ", error_message);
    fflush(stdout);
    perror(NULL);
    printf("(Errno: %i)\n", errno);

    return return_value;
}

static inline void change_cursor(bool on){
    if(on){
        printf("\e[?25h");
    }
    else{
        printf("\e[?25l");
    }
}

void swap(int * a, int * b);
int get_raw_input(char * prompt, char ** input);
int lower(char * string, int len);
int upper(char * string, int len);
int change_echo(bool on);
