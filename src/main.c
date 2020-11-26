#include "termiArt.h"
#include <signal.h>

void sigint_handler(int signum){
    change_cursor(true);
    change_echo(true);
    exit(0);
}

void segfault_handler(int signum){
    change_cursor(true);
    change_echo(true);
    printf("\e[31m\e[1mSEGMENTATION FAULT\e[0m\n");
    exit(0);
}

int main(int argc, char ** argv){
    int height = 0;
    int width = 0;
    void * signal_check = NULL;
    node_t * canvas = NULL;

    signal_check = signal(SIGINT, sigint_handler);
    if(SIG_ERR == signal_check){
        print_error("MAIN: Signal error", 0);
        goto cleanup;
    }

    signal_check = signal(SIGSEGV, segfault_handler);
    if(SIG_ERR == signal_check){
        print_error("MAIN: Signal error", 0);
        goto cleanup;
    }

    if(argc < 2 || argv[1][0] != '-' || argv[1][2] != 0 || (('o' == argv[1][1] || 'e' == argv[1][1]) && argc != 3) ){
        open_file(".termiArt.tart", false);
        printf("\n\e[107m\e[\e[30m\e[3mTermi Art version 0.0.0\e[0m\n\n\e[1m\e[31mUSAGE: %s: <flag> [file path]\e[0m\n\n\e[1mFLAGS:\e[0m\n-o: Open without editing\n-e: Open with editing\n-n: Create new\n", argv[0]);
        goto cleanup;
    }

    if('o' == argv[1][1]){
        open_file(argv[2], false);
    }
    else if('e' == argv[1][1]){
        open_file(argv[2], true);
    }
    else if('n' == argv[1][1]){
        canvas_handler();
    }

cleanup:
    exit(0);
}