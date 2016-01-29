# Criticism

These are some issues related to the current CMSIS APIs, with suggestions based on solutions used in CMSIS++.

## RTOS


### Makes heavy use of macros to define static object instances and to refer to them

Instead of passing multiple arguments to the `XyzCreate()`, most arguments are 
stored in a `*Def` structure and the pointer to it is passed to the `XyzCreate()`.

This structure is created using a macro, which looks like a function
call, making the definition quite confusing.

For example

```
#include "cmsis_os.h"
 
void Thread_1 (void const *arg);                           // function prototype for Thread_1
osThreadDef (Thread_1, osPriorityNormal, 1, 0);            // define Thread_1
 
int main (void) {
  osThreadId id;
  
  id = osThreadCreate (osThread (Thread_1), NULL);         // create the thread
  if (id == NULL) {                                        // handle thread creation
    // Failed to create a thread
  }
  
  osThreadTerminate (id);                                  // stop the thread
}
```

Suggestion:

Give up all these macros and create objects with multiple arguments, as usual.

```
#include <cmsis_os_ex.h>

void task_1 (void const *arg);  // function prototype for Task_1

OSThread th1;

int main (void) {

	osThreadCreateEx(&th1, "task1", NULL, 0, osPriorityNormal, task_1, NULL);
	
	// ...
	
	osThreadSetPriority(&th1, osPriorityHigh);
	
	// ...
	
	osThreadTerminate(&th1);
}
```

### Makes use of dinamic allocator for stack, queues, etc

Although the macros defining objects allow to statically allocate storage for
objects, some of them internally still allocate storage on the heap, for example 
the thread stack.

There is no way to avoid these allocations.

Suggestion:

* add parameters to all functions and allow to pass a pointer to a user defined
storage.

### All time durations are in milliseconds, while the system timer counts ticks

Although the usual setting for the system timer is with 1000 ticks per second, 
which corresponds to 1 ms per tick, this is not mandatory, and some applications
might very well use different timer settings.

Since all system timeouts are actually implemented as timer ticks, it is
more accurate to use number of ticks, not milliseconds, for all timeouts.

Suggestion:

* change all prototypes to use sys_ticks_t instead of milliseconds

### The scheme to name objects is inconsecvent 

The macros used to define object names are not consecvent, for example the 
thread name is derived from the thread function name, but the timer name should be
explicitly defined, different from the timer function.

```
void Thread_1 (void const *arg);                          
osThreadDef (Thread_1, osPriorityNormal, 1, 0); 

void Timer1_Callback  (void const *arg);                  
osTimerDef (Timer1, Timer1_Callback);   

```

Suggestion:

* use a separate name for all objects.

```
	osThreadCreateEx(&th1, "task1", NULL, 0, osPriorityNormal, task_1, NULL);

	// ...
	
	osTimerCreateEx(&tm1, "timer1", timer_1, osTimerOnce, NULL);
```

### There is no support for critical sections (interrupts & scheduler)

The current specs do not inlude any means to define critical sections.
Depending on the purpose of the protected region, there might be regions
where some interrupts are not allowed, or regions where scheduler context
switches are not allowed.

Suggestion:

* define interrupts critical sections (with interupts mask/restore)
* define scheduler criticals ections (with scheduler lock/unlock)

```

  scheduler_status_t
  osSchedulerLock (void);

  void
  osSchedulerUnlock (scheduler_status_t);

  // ...

  interrupts_status_t
  osCriticalEnter (void);

  void
  osCriticalExit (interrupts_status_t);

```


### Has no public memory allocator

Although internally the system requires an allocator for some objects 
(like thread stacks), and the application might also need to allocate
memory for specific purposes, it is not specified how this public allocator
should behave, especially how it should handle inter-thread synchronisation.

Suggestion:

* add support for synchronised malloc()/free(); syncronisation can be done
either by mutex or scheduler critical sections (assuming no allocations will
be performed on interrupts).

### Should not return agregate (osEvent)

Although allowed by the C language, and supported by most compilers, returning
bitfiled structures directly is not a good ideea, since it will trigger lots of
warnings in build environments where all warnings are enabled.

Suggestion:

* redesign the API to not return structure with bitfields

### Missing osPoolDelete(), osMessageDelete(), osMailDelete()

Some objects can be deleted, like threads and timers, other cannot, like pools and queues.

Suggestion:

* extend the API to add osPoolDelete(), osMessageDelete(), osMailDelete()

### Threads

#### There is no mechanism to tell if a thread terminated or not

The specs currently do not define what happens if a thread exits, and there
is no mechanism to tell is a thread terminated or not.

Suggestion:

* add join(), to allow one thread to wait for a thread to terminate

#### There is no mechanism to enumerate all registered threads

For writing monitor commands that list all available threads, it is required to
enumerate existing threads.

Suggestion:

* add `get_tread_count()` and `get_thread(size_t i)`.


### Semaphores

#### Confusing definition for binary semaphore

The specs mention that creating a semaphore with a count=1 is equivalent with
creating a binary semaphore, but functionality in RTX is not consistent.

Suggestion:

* add max_count, default 0xFFFFFFFF; to create a binary semaphore use max_count = 1.

```

  osSemaphoreId
  osSemaphoreCreateEx (osSemaphoreId addr, const char* name,
                       int32_t initial_count, uint32_t max_count);

```

### Mutexes

#### There is no way to poll the mutex if available

It is not possible to acquire a mutex only if available, and not block if busy.

Suggestion:

* add `try_wait(void)` 

#### There is no recursive mutex

Suggestion:

* add Recursive_mutex()

## CMSIS Drivers 

* uses non-reentrant callbacks
* should not return aggregates (version, capabilities, status)
* Initialize() enable clocks, which means it powers up the device
* callbacks registered together with hardware inits
* initialize/unitialize must be linked to power up/down

Driver implementation

* poor code reusability: when writing multiple similar implementations it is 
more difficult to share code between them
* drivers are defined as separate pointers to RO struct of functions, instead 
of a common set of functions and different pointers to data

### USART

* configuration calls mixed with operational commands

### USBD

* inconsecvent status name (ARM\_USBD\_STATE, vs ARM\_USART\_STATUS)
