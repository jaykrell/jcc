/* Win32/pthread portability layer. */

#include "jthread.h"

#if _WIN32
#include "windows.h"
jrwlock_t jrwlock_init;
jlock_t jlock_init;
jcondvar_t jcondvar_init;
#else
#include <pthread.h>
#endif

void jlock(jlock_t* lock)
{
#if _WIN32
	AcquireSRWLockExclusive((PSRWLOCK)lock);
#else
#endif
}

void junlock(jlock_t* lock)
{
#if _WIN32
	ReleaseSRWLockExclusive((PSRWLOCK)lock);
#else
	pthread_mutex_unlock(lock);
#endif
}

void jlock_w(jrwlock_t* lock)
{
#if _WIN32
	AcquireSRWLockExclusive((PSRWLOCK)lock);
#else
	pthread_rwlock_wrlock(lock);
#endif
}

void jlock_r(jrwlock_t* lock)
{
#if _WIN32
	AcquireSRWLockShared((PSRWLOCK)lock);
#else
	pthread_rwlock_rdlock(lock);
#endif
}

void junlock_w(jrwlock_t* lock)
{
#if _WIN32
	ReleaseSRWLockExclusive((PSRWLOCK)lock);
#else
	pthread_rwlock_unlock(lock);
#endif
}

void junlock_r(jrwlock_t* lock)
{
#if _WIN32
	ReleaseSRWLockShared((PSRWLOCK)lock);
#else
	pthread_rwlock_unlock(lock);
#endif
}

void jcondvar_sleep(jcondvar_t* condvar, jlock_t* lock)
{
#if _WIN32
	SleepConditionVariableSRW((PCONDITION_VARIABLE)condvar, (PSRWLOCK)lock, INFINITE, 0);
#else
	pthread_cond_wait(condvar, lock);
#endif
}

void jcondvar_wake1(jcondvar_t* condvar)
{
#if _WIN32
	WakeConditionVariable((PCONDITION_VARIABLE)condvar);
#else
	pthread_cond_signal(convar);
#endif
}

void jcondvar_wake_all(jcondvar_t* condvar)
{
#if _WIN32
	WakeAllConditionVariable((PCONDITION_VARIABLE)condvar);
#else
	pthread_cond_broadcast(condvar);
#endif
}
