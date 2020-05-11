/*
  Copyright (C) 2017-2018 Teddy ASTIE

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/* C11-threads-like cross-platform thread library.
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
typedef pthread_t sthread;
typedef pthread_mutex_t smutex;
#else
typedef struct {
  HANDLE handle;
  DWORD id;
} sthread;
typedef CRITICAL_SECTION smutex;
#endif

/**
 * Function used to start thread.
 */
typedef void (*sthread_start)(void *);

/**
 * Create (and start) a new thread.
 */
bool sthread_new(sthread *thread, sthread_start thread_start, void *arg);

/**
 * Get the current thread.
 */
sthread sthread_current(void);

/**
 * Detach a thread to the current process, the
 * resources are automaticaly freed on thread exit.
 */
bool sthread_detach(sthread thread);

/**
 * Wait the end of the thread.
 */
bool sthread_join(sthread thread);

/**
 * Return 1 if the threads a and b are the same, otherwise, 0.
 */
int sthread_equals(sthread a, sthread b);

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
bool smutex_new(smutex *mutex);

/**
 * Destroy a mutex.
 */
void smutex_free(smutex *mutex);

/**
 * Lock a mutex.
 */
bool smutex_lock(smutex *mutex);

/**
 * Try to lock a mutex.
 * Returns true if the mutex is busy.
 */
bool smutex_try_lock(smutex *mutex);

/**
 * Unlock a mutex.
 */
bool smutex_unlock(smutex *mutex);

#endif /* H_STHREAD */
