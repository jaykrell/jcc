/* Win32/pthread portability layer.
 * On Windows this requires Vista or newer. TODO? */
#include "jthread.h"
#include <stdint.h>
#if _WIN32
#include "windows.h"
#endif

int jthread_create(thread_entry_t thread_entry, void *thread_arg,
                   jthread_handle_t *thread_handle) {
#if _WIN32
  return (*thread_handle = CreateThread(
              0, 0, (LPTHREAD_START_ROUTINE)thread_entry, thread_arg, 0, 0))
             ? 0
             : -1;
#else
  return pthread_create(thread_handle, 0, (void *(*)(void *))thread_entry,
                        thread_arg);
#endif
}

int jthread_wait(jthread_handle_t thread_handle) {
#if _WIN32
  unsigned long exit_code = 0;
  WaitForSingleObject(thread_handle, INFINITE);
  GetExitCodeThread(thread_handle, &exit_code);
  return (int)exit_code;
#else
  void *value = 0;
  pthread_join(thread_handle, &value);
  return (int)(intptr_t)value;
#endif
}

void jlock(jlock_t *lock) {
#if _WIN32
  AcquireSRWLockExclusive((PSRWLOCK)lock);
#else
  pthread_mutex_lock(lock);
#endif
}

void junlock(jlock_t *lock) {
#if _WIN32
  ReleaseSRWLockExclusive((PSRWLOCK)lock);
#else
  pthread_mutex_unlock(lock);
#endif
}

void jlock_w(jrwlock_t *lock) {
#if _WIN32
  AcquireSRWLockExclusive((PSRWLOCK)lock);
#else
  pthread_rwlock_wrlock(lock);
#endif
}

void jlock_r(jrwlock_t *lock) {
#if _WIN32
  AcquireSRWLockShared((PSRWLOCK)lock);
#else
  pthread_rwlock_rdlock(lock);
#endif
}

void junlock_w(jrwlock_t *lock) {
#if _WIN32
  ReleaseSRWLockExclusive((PSRWLOCK)lock);
#else
  pthread_rwlock_unlock(lock);
#endif
}

void junlock_r(jrwlock_t *lock) {
#if _WIN32
  ReleaseSRWLockShared((PSRWLOCK)lock);
#else
  pthread_rwlock_unlock(lock);
#endif
}

void jcondvar_sleep(jcondvar_t *condvar, jlock_t *lock) {
#if _WIN32
  SleepConditionVariableSRW((PCONDITION_VARIABLE)condvar, (PSRWLOCK)lock,
                            INFINITE, 0);
#else
  pthread_cond_wait(condvar, lock);
#endif
}

void jcondvar_wake1(jcondvar_t *condvar) {
#if _WIN32
  WakeConditionVariable((PCONDITION_VARIABLE)condvar);
#else
  pthread_cond_signal(condvar);
#endif
}

void jcondvar_wake_all(jcondvar_t *condvar) {
#if _WIN32
  WakeAllConditionVariable((PCONDITION_VARIABLE)condvar);
#else
  pthread_cond_broadcast(condvar);
#endif
}
