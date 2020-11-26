#include "standard.h"
#include <termios.h>
#include <unistd.h>
#include <math.h>

struct termios attributes;

//This file includes basic/standard functions (Here raw_input and lower)

void swap(int * a, int * b){
    int temp = *a;

    *a = *b;
    *b = temp;
}

/**
 * @brief: Gets raw input
 * @param[IN] prompt: The prompt to print to indicate that it is waiting for user input.
 * @param[OUT] input: A buffer to fill with user input
 * 
 * @return: The number of bytes that the user input
 * @notes: Since this is _raw_ input, the input will be a string (or array of characters). It is up to 
 *         the caller to change the input acccording to whatever datatype is needed
 */
int get_raw_input(IN char * prompt, OUT char ** input){
    size_t size = 0;
    int bytes_read = 0;
    int i = 0;

    if(NULL != prompt){
        printf("%s", prompt);
    }
    if(NULL != *input){
        free(*input);
        *input = NULL;
    }

    bytes_read = getline(input, &size, stdin);
    if(-1 == bytes_read){
        perror("Getline error");
    }
    for(i=0; i<bytes_read; i++){
        if('\n' == (*input)[i]){
            (*input)[i] = 0;
        }
    }

    return bytes_read;
}

/**
 * @brief: Makes all english letters in a string lowercase
 * @param[OUT] string: The string to operate on (cannot be NULL for obvious reasons)
 * @param[IN] len: The length of the string (only matters if len is 1, denoting a character)
 * 
 * @return: The length of the string
 * @notes: Any input of len other than 1 will not matter, and will be discarded by the function
 */
int lower(OUT char * string, IN int len){
    int i = 0;

    if(1 != len){
        len = strnlen(string, BUFFER_SIZE);
        if(-1 == len){
            perror("Strnlen function error");
            goto cleanup;
        }
    }

    for(i=0; i<len; i++){
        if('A' <= string[i] && string[i] <= 'Z'){
            string[i] += 32;
        }
    }

cleanup:
    return len;
}

/**
 * @brief: Makes all english letters in a string uppercase
 * @param[OUT] string: The string to operate on (cannot be NULL for obvious reasons)
 * @param[IN] len: The length of the string (only matters if len is 1, denoting a character)
 * 
 * @return: The length of the string
 * @notes: Any input of len other than 1 will not matter, and will be discarded by the function
 */
int upper(OUT char * string, IN int len){
    int i = 0;

    if(1 != len){
        len = strnlen(string, BUFFER_SIZE);
        if(-1 == len){
            perror("Strnlen function error");
            goto cleanup;
        }
    }

    for(i=0; i<len; i++){
        if('a' <= string[i] && string[i] <= 'z'){
            string[i] -= 32;
        }
    }

cleanup:
    return len;
}

/**
 * @brief: Turns on or off ECHO in a terminal
 * @param[IN] on: True if ECHO should be turned on, false for off
 * 
 * @return: 0 on success, else -1
 */
int change_echo(bool on){
    int error_check = 0;

    error_check = tcgetattr(STDIN_FILENO, &attributes);
    if(-1 == error_check){
        perror("Failed to retrieve terminal attributes");
        goto cleanup;
    }

    if(on){
        attributes.c_lflag |= (ECHO | ICANON);
    }
    else{
        attributes.c_lflag &= ~(ECHO | ICANON);
    }
    error_check = tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    if(-1 == error_check){
        perror("Failed to set terminal attributes");
        goto cleanup;
    }

cleanup:
    return error_check;
}
