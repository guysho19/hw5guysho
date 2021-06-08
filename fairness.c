#include "types.h"
#include "user.h"

extern void check_proc_order();


// entry point when calling from the shell
int main(int argc, char *argv[]){
    check_proc_order();
    exit();
    return 0;
}


