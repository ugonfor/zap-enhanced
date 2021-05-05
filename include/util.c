#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

void opt_argv(int idx, int argc, char* argv[], char* dst, char opt){
    if(idx < argc && argv[idx][0] != '-'){
        memcpy(dst, argv[idx], strnlen(argv[idx], 254));
    }
    else printf("-%c option need argument(s)\n",opt); 
    return ;
}

void argparse(int argc, char* argv[]){
    int c = 0;
    while ((c=getopt(argc, argv, "AaRtd")) != -1) 
    {
        switch (c)
        {
        case 'a':
            FLAG |= a_FLAG;
            opt_argv(optind, argc, argv, username1, c);
            break;

        case 'A':
            FLAG |= A_FLAG;
            opt_argv(optind, argc, argv, username1, c);
            break;

        case 'R':
            FLAG |= R_FLAG;
            opt_argv(optind, argc, argv, username1, c);
            opt_argv(optind+1, argc, argv, username2, c);
            break;

        case 't':
            FLAG |= t_FLAG;
            opt_argv(optind, argc, argv, tty1, c);
            if (FLAG & R_FLAG) opt_argv(optind+1, argc, argv, tty2, c);
            break;

        case 'd':
            FLAG |= d_FLAG;
            opt_argv(optind, argc, argv, mmddyy1, c);
            if (FLAG & R_FLAG) opt_argv(optind+1, argc, argv, mmddyy2, c);
            break;


        case '?':
            break;
        }
    }

    return ;
}