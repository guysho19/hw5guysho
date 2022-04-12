//
// Created by cnoam on 2022-04-11
//

// This program tests the implementation of signals (in xv6)
// To run under linux: gcc test_signals.c -o test_signals

#ifdef xv6
#include "types.h"
#include "user.h"
#include "signal.h"
#define assert(x)  if(!(x)){ print("ASSERT FAILED"); exit();}

int wait0(){ return wait();}
void exit0() {exit();}
int print(char* fmt, ...) {
    return 0;//printf(1,fmt,...);
}
typedef int pid_t;

#else // linux

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

typedef  __sighandler_t  sighandler_t;

int sigsend(int pid, int sig){
    return kill(pid, sig);
}

int wait0(){ int s;return wait(&s);}
void exit0() {exit(0);}
int print(char* fmt, ...) {
    return printf(fmt);
}
#endif

int test_terminate(int pid){
    return 0;

}


void test_default(int pid){
    // the default action for all signals is to terminate the process
    sigsend(pid, SIGALRM);
    sleep(1);
    int stat;
    pid_t sta= waitpid(pid,&stat,0);
    assert(sta < 0); // the child must be terminated by now
}
void test_ignore(){
    /* verify we can set signals to be ignored.
     * The signals SIGKILL and SIGSTOP cannot be caught or ignored.
     */
    sighandler_t ret = signal(SIGTERM, SIG_IGN); // ignore TERMINATE
    assert(ret != SIG_ERR);
    ret = signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE
    assert(ret == SIG_ERR);

    sigsend(0,SIGTERM);
}

int child_sent_hup = 0;
int child_got_hup = 0;
void parent_handler(int sig){
    if(sig == SIGHUP){
        child_sent_hup = 1;
    }
}

void child_handler(int sig){
    if(sig == SIGHUP){
        child_got_hup = 1;
    }
}

void test_IPC( int ids[]){
    // send SIGHUP to child # 1.
    // The child should send back the same signal
    signal(SIGHUP, parent_handler);
    int ok = sigsend(ids[1], SIGHUP);
    if(ok != 0){
        print("failed sending a signal");
        exit0();
    }
    sleep(1);
    assert(child_sent_hup);
    print("IPC to parent is OK\n");
}

void run_child(int id, int parent_pid){
    print("process %d with pid %d starts\n", id, getpid());
    if(id == 1){
        signal(SIGHUP, child_handler);
        sleep(1);
        if(child_got_hup){
            sigsend(parent_pid, SIGHUP);
        }
        sleep(1);
        goto end;//exit(0);
    }

    for(int k = 0; k < 3;k++){
        sleep(1);
        print("pid %d \n", getpid());
    }
    end:
    print("process %d exiting normally\n", getpid());
}

int main(int ac, char** av){
    int num_child = 2;
    if(ac == 2){
        num_child = atoi(av[1]);
    }
    int pids[num_child];
    int ppid = getpid();
    // launch 'num_child' processes. Each will call its own 'run_child()'
    for(int i = 0; i< num_child;i++){
        int child_pid = fork();
        if(child_pid == 0){
            // in child
            run_child(i,ppid);
            return 0;
        }else{
            pids[i] = child_pid;
        }
    }

    // wait until the children are ready
    sleep(1);
    //test_default(pids[0]);
    test_IPC(pids);
    //test_ignore();
    test_terminate(pids[1]);
    
    // wait here until all children exited.
    // TODO: add timeout
    for(int k =0; k < num_child;k++){
        int pid = wait0();
        if(pid < 0){
            print("wait failed!");
            exit0();
        }else{
            print("child process pid=%d terminated\n", pid);
        }
    }
}

