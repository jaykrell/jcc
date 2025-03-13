/* Test Win32/pthread portability layer. */

#include "jthread.h"
/*
void jlock_w(jrwlock_t*);
void jlock_r(jrwlock_t*);
void jlock(jlock_t*);

void junlock_w(jrwlock_t*);
void junlock_r(jrwlock_t*);
void junlock(jlock_t*);

void jcondvar_sleep(jcondvar_t* condvar, jlock_t* lock);
void jcondvar_wake1(jcondvar_t* condvar);
void jcondvar_wake_all(jcondvar_t* condvar);
*/

int main()
{
	jrwlock_t rwlock = JRWLOCK_INIT;
	jrwlock_t lock = JLOCK_INIT;
	jcondvar_t convar = JRWLOCK_INIT;

	jlock(&lock);
	junlock(&lock);

	jlock_w(&rwlock);
	junlock_w(&rwlock);

	jlock_r(&rwlock);
	junlock_r(&rwlock);
	
	jcondvar_wake1(&convar);
	jcondvar_wake_all(&convar);
	jcondvar_sleep(&convar, &lock);
}
