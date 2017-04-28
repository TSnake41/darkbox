/*
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/* C11 threads-like portable thread implementation
   Fully compatible with Windows and POSIX systems.

   Slightly based on tinycthread.
*/

#ifndef H_STHREAD
#define H_STHREAD

#include <stdbool.h>

#ifndef WIN32
#include <pthread.h>
#else
#include <windows.h>
#endif

enum sthread_return {
    sthread_success = false,
    sthread_error = true,
};

#ifndef WIN32
typedef pthread_t sthread_t;
typedef pthread_mutex_t smutex_t;
#else
typedef struct {
    HANDLE handle;
    DWORD id;
} sthread_t;
typedef CRITICAL_SECTION smutex_t;
#endif

/**
 * Function used to start thread.
 */
typedef void (*sthread_start_t)(void *);

/**
 * Create (and start) a new thread.
 */
bool sthread_new(sthread_t *thread, sthread_start_t thread_start, void *arg);

/**
 * Get the current thread.
 */
sthread_t sthread_current(void);

/**
 * Detach a thread to the current process, the
 * resources are automaticaly freed on thread exit.
 */
bool sthread_detach(sthread_t thread);

/**
 * Wait the end of the thread.
 */
bool sthread_join(sthread_t thread);

/**
 * Return 1 if the threads a and b are the same, otherwise, 0.
 */
int sthread_equals(sthread_t a, sthread_t b);

/**
 * Exit the current thread.
 */
void sthread_exit(void);

/**
 * Wait ms milliseconds.
 */
void sthread_sleep(unsigned long ms);

/**
 * Create a new mutex.
 */
bool smutex_new(smutex_t *mutex);

/**
 * Destroy a mutex.
 */
void smutex_free(smutex_t *mutex);

/**
 * Lock a mutex.
 */
bool smutex_lock(smutex_t *mutex);

/**
 * Try to lock a mutex.
 * Return true if the mutex is busy.
 */
bool smutex_try_lock(smutex_t *mutex);

/**
 * Unlock a mutex.
 */
bool smutex_unlock(smutex_t *mutex);

#endif /* H_STHREAD */
