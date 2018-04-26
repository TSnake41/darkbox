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

#ifndef WIN32
#define _XOPEN_SOURCE 700
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#else
#define _WIN32_WINNT 0x0502
#include <windows.h>
#endif

#include <stdlib.h>
#include <sthread.h>

/* We need a wrapper because we use void as return type, to avoid crashes,
  warnings and errors, we the wrap thread_start to make it compatible with
  the Operating System threading system (both POSIX (pthread) and Win32 API).
*/

typedef struct {
  sthread_start start;
  void *arg;
  #ifdef WIN32
  DWORD *thread_id_ptr;
  #endif
} _sthread_wrap_arg;

#ifndef WIN32
static void *sthread_wrap(void *arg)
#else
static DWORD __stdcall sthread_wrap(void *arg)
#endif
{
  _sthread_wrap_arg *wrap_ptr = arg;
  _sthread_wrap_arg wrap = *wrap_ptr;

  #ifdef WIN32
  /* Define thread id */
  *(wrap_ptr->thread_id_ptr) = GetCurrentThreadId();
  #endif

  /* Because arg is allocated on the heap, we need to free it manually. */
  free(wrap_ptr);

  wrap.start(wrap.arg);

  return 0;
}

bool sthread_new(sthread *thread, sthread_start thread_start, void *arg)
{
  if (thread_start == NULL)
    return sthread_error;

  /* We need to define wrapper args. */
  _sthread_wrap_arg *wrap_args = malloc(sizeof(_sthread_wrap_arg));
  if (wrap_args == NULL)
      return sthread_error;

  wrap_args->start = thread_start;
  wrap_args->arg = arg;

  #ifndef WIN32
  if (pthread_create(thread, NULL, sthread_wrap, wrap_args) != 0)
    *thread = 0;

  return *thread == 0;
  #else
  wrap_args->thread_id_ptr = &thread->id;
  thread->handle = CreateThread(NULL, 0, sthread_wrap, wrap_args, 0, NULL);

  return thread->handle == 0;
  #endif
}

sthread sthread_current(void)
{
  #ifndef WIN32
  return pthread_self();
  #else
  return (sthread){ .handle = GetCurrentThread(), .id = GetCurrentThreadId() };
  #endif
}

int sthread_equals(sthread a, sthread b)
{
  #ifndef WIN32
  return pthread_equal(a, b);
  #else
  return a.id == b.id;
  #endif
}

bool sthread_detach(sthread thread)
{
  #ifndef WIN32
  return pthread_detach(thread) != 0;
  #else
  return CloseHandle(thread.handle) == 0;
  #endif
}

bool sthread_join(sthread thread)
{
  #ifndef WIN32
  return pthread_join(thread, NULL) != 0;
  #else
  if (WaitForSingleObject(thread.handle, INFINITE) == WAIT_FAILED)
    return sthread_error;

  return CloseHandle(thread.handle) == 0;
  #endif
}

void sthread_exit(void)
{
  #ifndef WIN32
  pthread_exit(NULL);
  #else
  ExitThread(0);
  #endif
}

void sthread_sleep(unsigned long ms)
{
  #ifndef WIN32
  struct timespec req;
  time_t sec = (int)(ms / 1000);
  ms -= sec * 1000;
  req.tv_sec = sec;
  req.tv_nsec = ms * 1e+6L;
  while (nanosleep(&req, &req) == -1)
    continue;
  #else
  Sleep(ms);
  #endif
}

bool smutex_new(smutex *mutex)
{
  #ifndef WIN32
  return pthread_mutex_init(mutex, NULL);
  #else
  InitializeCriticalSection(mutex);
  return sthread_success;
  #endif
}

void smutex_free(smutex *mutex)
{
  #ifndef WIN32
  pthread_mutex_destroy(mutex);
  #else
  DeleteCriticalSection(mutex);
  #endif
}

bool smutex_lock(smutex *mutex)
{
  #ifndef WIN32
  return pthread_mutex_lock(mutex) != 0;
  #else
  EnterCriticalSection(mutex);
  return sthread_success;
  #endif
}

bool smutex_try_lock(smutex *mutex)
{
  #ifndef WIN32
  return pthread_mutex_trylock(mutex) != 0;
  #else
  return TryEnterCriticalSection(mutex) == 0;
  #endif
}

bool smutex_unlock(smutex *mutex)
{
  #ifndef WIN32
  return pthread_mutex_unlock(mutex) != 0;
  #else
  LeaveCriticalSection(mutex);
  return sthread_success;
  #endif
}
