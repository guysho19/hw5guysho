// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

//#define SINGLESHOT

char *argv[] = { "sh", 0 };
extern void check_proc_order();

int main(void) {
    if (open("console", O_RDWR) < 0) {
        mknod("console", 1, 1);
        open("console", O_RDWR);
    }
    dup(0);  // stdout
    dup(0);  // stderr

#ifdef SINGLESHOT
    check_proc_order();
    halt();
    return 0;

#else
    int pid, wpid;
    for(;;){
        printf(1, "init: starting sh\n");
        pid = fork();
    if(pid < 0){
            printf(1, "init: fork failed\n");
            exit();
        }
    if(pid == 0){
            exec("sh", argv);
            printf(1, "init: exec sh failed\n");
            exit();
        }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
    return 0;
#endif

}
