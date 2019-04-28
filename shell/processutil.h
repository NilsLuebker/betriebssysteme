#ifndef PROCESSUTIL_H
#define PROCESSUTIL_H

#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define DEFAULT_PROCESS_COUNT 25

static size_t process_list_size = DEFAULT_PROCESS_COUNT;
static size_t process_list_len = 0;
static pid_t* process_list = NULL;

void process_list_add_pid(pid_t);
void process_list_remove_pid(pid_t);

static void process_sigint(int signal);
static void process_sigcld(int signal);
static void process_sigstop(int signal);

#endif /* PROCESSUTIL_H */
