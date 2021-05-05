#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "include/util.h"
#include "include/zap.h"

int main(int argc, char const *argv[])
{
    argparse(argc, argv);
    if(FLAG & A_FLAG) A_OptZap(username1);
    else if(FLAG & a_FLAG) return a_OptZap(username1, tty1, mmddyy1);
    else if(FLAG & R_FLAG) return R_OptZap(username1, username2, tty1, tty2, mmddyy1, mmddyy2);
    else printf("Options should include at least one of \"AaR\"\n");
    
    return 0;
}