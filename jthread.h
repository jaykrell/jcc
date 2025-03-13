/* Win32/pthread portability layer. */

/* SRWLOCK and CONDITION_VARIABLE */

#if !_WIN32
#include <pthread.h>
#endif

typedef struct jrwlock_t {
#if _WIN32
	void* space;
#else
	void* space;
#endif
} jrwlock_t;

typedef struct jlock_t {
#if _WIN32
	void* space;
#else
	void* space;
#endif
} jlock_t;

typedef struct jcondvar_t {
#if _WIN32
	void* space;
#else
	void* space;
#endif
} jcondvar_t;

void jlock_w(jrwlock_t*);
void jlock_r(jrwlock_t*);
void jlock(jlock_t*);

void junlock_w(jrwlock_t*);
void junlock_r(jrwlock_t*);
void junlock(jlock_t*);

void jcondvar_sleep(jcondvar_t* condvar, jlock_t* lock);
void jcondvar_wake1(jcondvar_t* condvar);
void jcondvar_wake_all(jcondvar_t* condvar);
