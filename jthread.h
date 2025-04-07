/* Win32/pthread portability layer. */

#include "jconfig.h"

/* SRWLOCK and CONDITION_VARIABLE */

#if _WIN32
typedef struct jrwlock_t {
  void *space;
} jrwlock_t;
typedef struct jlock_t {
  void *space;
} jlock_t;
typedef struct jcondvar_t {
  void *space;
} jcondvar_t;
#define JCONDVAR_INIT {0}
#define JLOCK_INIT {0}
#define JRWLOCK_INIT {0}

typedef void *jthread_handle_t;

#else

#include <pthread.h>
typedef pthread_cond_t jcondvar_t;
typedef pthread_mutex_t jlock_t;
typedef pthread_rwlock_t jrwlock_t;
#define JCONDVAR_INIT PTHREAD_COND_INITIALIZER
#define JLOCK_INIT PTHREAD_MUTEX_INITIALIZER
#define JRWLOCK_INIT PTHREAD_RWLOCK_INITIALIZER

typedef pthread_t jthread_handle_t;

#endif

#if __cplusplus
extern "C" {
#endif

typedef int (*thread_entry_t)(void *);

int jthread_create(thread_entry_t thread_entry, void *thread_arg,
                   jthread_handle_t *thread_handle);
int jthread_wait(jthread_handle_t thread_handle);

void jlock_w(jrwlock_t *);
void jlock_r(jrwlock_t *);
void jlock(jlock_t *);

void junlock_w(jrwlock_t *);
void junlock_r(jrwlock_t *);
void junlock(jlock_t *);

void jcondvar_sleep(jcondvar_t *condvar, jlock_t *lock);
void jcondvar_wake1(jcondvar_t *condvar);
void jcondvar_wake_all(jcondvar_t *condvar);

#if __cplusplus
} /* extern "C" */
#endif
