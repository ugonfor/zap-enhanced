#define A_FLAG 1
#define a_FLAG 2
#define t_FLAG 4
#define d_FLAG 8
#define R_FLAG 16

int FLAG;
char username1[256];
char username2[256];
char tty1[256];
char tty2[256];
char mmddyy1[256];
char mmddyy2[256];

void argparse(int argc, char* argv[]);