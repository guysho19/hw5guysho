//
// Created by cnoam on 2022-04-11
//

#ifndef XV6_PUBLIC_SIGNAL_H
#define XV6_PUBLIC_SIGNAL_H

/// handler for signals. This is how we declare pointer to function
typedef void (*sighandler_t)(int);

/** This system call will register a new handler for a given signal number (signum).
 * If failed, -1 is returned.
 * Otherwise, the previous value of the signal handler is returned.
*/
sighandler_t signal(int signum, sighandler_t handler);

/** send a signal to process with 'pid'
 *
 * @param pid process ID of the targer process
 * @param signum signal number to set
 * @return 0 on success, -1 on error
 */
int sigsend(int pid, int signum);

// copied from /usr/include/x86_64-linux-gnu/bits/signum-generic.h
#define SIG_ERR  ((sighandler_t) -1)  /* Error return.  */
#define SIG_DFL  ((sighandler_t)  0)  /* Default action.  */
#define SIG_IGN  ((sighandler_t)  1)  /* Ignore signal.  */

#define SIGTERM         15      /* Termination request.  */
/* Historical signals specified by POSIX. */
#define SIGHUP          1       /* Hangup.  */
#define SIGQUIT         3       /* Quit.  */
#define SIGTRAP         5       /* Trace/breakpoint trap.  */
#define SIGKILL         9       /* Killed.  */
#define SIGPIPE         13      /* Broken pipe.  */
#define SIGALRM         14      /* Alarm clock.  */

#endif //XV6_PUBLIC_SIGNAL_H
