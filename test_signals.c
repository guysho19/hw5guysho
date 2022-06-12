// This program tests the implementation of signals (in xv6)

#include "types.h"
#include "user.h"
#include "signal.h"
#define assert(x)  if(!(x)){ print("ASSERT FAILED\n\n"); exit();}

typedef int pid_t;

// DUMMY implementation of signal and sigsend system calls.
// You MUST delete them (or put in comment) and implement the actual system calls
sighandler_t signal(int signum, sighandler_t handler){
    printf(1,"DUMMY signal() called\n");
    return 0;
}

int sigsend(int p, int s){
    printf(1,"DUMMY sigsend() called\n");
    return 0;
}

void my_handler(int sig){
    printf(1,"signal handler called with signal=%d\n", sig);
}

// test the simple case: child process registers a handler, the parent signals the child
void test_simple_handling(){
     printf(1,"%s starts\n", __FUNCTION__ );
    int child_pid = fork();
    if(child_pid < 0){
        printf(1,"fork failed");
        exit();
    }
    if(child_pid == 0){
        // in child
        signal(SIGHUP, my_handler);
        for(int i =0; i < 3;i++) {
            sleep(1);
            printf(1, "child is sleeping\n");
        }
        printf(1, "child exiting\n");
        exit();
    }else {
        // parent
        sigsend(child_pid, SIGALRM);
        wait();
        printf(1, "parent exiting\n");
    }
    printf(1,"%s completed\n\n", __FUNCTION__ );

}

// test the use case that a parent sets a signal , and a child inherits it.
// TODO: just do it!
void test_inheritance(){
    printf(1,"%s starts\n", __FUNCTION__ );
    printf(1,"%s completed\n\n", __FUNCTION__ );
}

// the default action for all signals is to print a message
void test_default(){
    printf(1,"%s starts\n", __FUNCTION__ );
    int child_pid = fork();
    if(child_pid < 0){
        printf(1,"fork failed");
        exit();
    }
    if(child_pid == 0){
        // in child
        for(int i =0; i < 3;i++) {
            sleep(1);
            printf(1, "child is sleeping\n");
        }
        printf(1, "child exiting\n");
        exit();
    }else {
        // parent
        sigsend(child_pid, SIGALRM);
        wait();
        printf(1, "parent exiting\n");
    }
    printf(1,"%s completed\n\n", __FUNCTION__ );
}

/* verify we can set signals to be ignored.
 * The signals SIGKILL and SIGSTOP cannot be caught or ignored.
 */
void test_ignore(){
    printf(1,"%s starts\n", __FUNCTION__ );
    int child_pid = fork();
    if(child_pid < 0){
        printf(1,"fork failed");
        exit();
    }
    if(child_pid == 0){
        // in child
       sighandler_t ret = signal(SIGTERM, SIG_IGN); // ignore TERMINATE
       if(ret == SIG_ERR){
           printf(1,"FAILED\n");
           return;
       }
       ret = signal(SIGPIPE, SIGSTOP); // impossible to ignore SIGSTOP
       if(ret != SIG_ERR){
           printf(1,"FAILED\n");
           return;
       }
       for(int i =0; i < 3;i++) {
            sleep(1);
            printf(1, "[%d] child is sleeping\n", i);
       }
       printf(1, "child exiting\n");
       exit();
    }else {
        // parent
        sleep(1);
        sigsend(child_pid, SIGTERM);
        wait();
        printf(1, "%s: parent exiting\n", __FUNCTION__);
    }
    printf(1,"%s completed\n\n", __FUNCTION__ );
}

void test_bad_value(){
    printf(1,"%s starts\n", __FUNCTION__ );
    sighandler_t ret = signal(588,0);
    if(ret >=0){
        printf(1,"%s: FAIL! signal should fail\n", __FUNCTION__ );
    }

    sigsend(42, SIGALRM);
    if(ret ==0){
        printf(1,"%s: FAIL! sigsend should fail\n", __FUNCTION__ );
    }
    printf(1,"%s completed\n\n", __FUNCTION__ );
}

int main(int ac, char** av){
    test_simple_handling();
    test_default();
    test_ignore();
    test_inheritance();
    test_bad_value();

    exit();
}

