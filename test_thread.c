/* Test Win32/pthread portability layer. */

#include "jthread.h"
#include <stdio.h>
#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
static jcondvar_t condvar = JCONDVAR_INIT;
static jlock_t lock = JLOCK_INIT;
static jrwlock_t rwlock = JRWLOCK_INIT;
int cond;

static int thread1(void *p) {
  printf("thread1\n");

  jlock(&lock);
  jcondvar_wake_all(&condvar);
  jcondvar_wake1(&condvar);
  cond = 1;
  junlock(&lock);

  return 2;
}

int main() {
  jthread_handle_t thread1_handle = {0};

  jlock(&lock);
  junlock(&lock);

  jlock_w(&rwlock);
  junlock_w(&rwlock);

  jlock_r(&rwlock);
  junlock_r(&rwlock);

  jthread_create(thread1, 0, &thread1_handle);

  while (!cond) {
    jlock(&lock);
    jcondvar_sleep(&condvar, &lock);
    junlock(&lock);
  }

  printf("thread_wait:%d\n", jthread_wait(thread1_handle));
  return 0;
}
