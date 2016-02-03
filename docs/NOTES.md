# Criticism

These are some issues related to the current CMSIS APIs, with suggestions based on solutions used in CMSIS++.

## RTOS

As a personal remark, the design of the CMSIS RTOS API seems greatly influenced 
by Keil RTX (function names and prototypes, but not necessarily the most
fortunate design), with some ideas from POSIX threads (pthreads, an 
established standard).

However the expected influence from POSIX threads is mostly aparent and 
inconsecvent.

In POSIX threads one of the design characteristics is the use of attributes
to configure various creation parameters, instead of passing all of them
in a long prototype. All functions used to create objects have a pointer
to the attributes. For simple use cases, this pointer can be NULL, and 
reasonable defaults are applied. Following this pointer, the function
pototypes include a minimum 
set of mandatory parameters (like pointer to thread function).

This design pattern allows to clearly separate simple use cases from
advanced ones.

The RTOS design introduces `*Def` structures, and a pointer to them is 
passed to the creation functions, but this pointer is mandatory, it
cannot be NULL, so there is not distinction between simple and advanced 
use cases.

Even worse, not all creation parameters are available, neither
in the `*Def` structure nor in the `*Create()` prototype (for example
the location of the thread stack area, the semaphore max count, etc). 

Neither the split between which parameters go to the `*Def` structure 
and which go to the `*Create()` prototype is not very obvious and 
consecvent (as it is in POSIX threads).


### Makes heavy use of macros to define static object instances and to refer to them

The `*Def` structure passed to the `XyzCreate()` is created using a macro, 
which looks like a function
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

Give up all these macros and use the POSIX approach, i.e. use
attributes. When defults are enough, allow simple creations, with
minimum arguments.

```
#include <cmsis_os_ex.h>

void task_1 (void const *arg);  // function prototype for Task_1

OSThread th1;

int main (void) {

	osThreadCreateEx(&th1, NULL, task_1, NULL);
	
	// ...
	
	osThreadSetPriority(&th1, osPriorityHigh);
	
	// ...
	
	osThreadTerminate(&th1);
}
```

### Makes use of dinamic allocator for stack, queues, etc

Although the macros defining objects allow to statically allocate storage for
objects, some of them internally still allocate storage on the heap, for example 
for the thread stack.

There is no way to avoid these allocations.

Suggestion:

* add attributes and allow to pass a pointer to a user defined
storage.

### All time durations are in milliseconds, while the system timer counts ticks

Although the usual system timer is set for 1000 ticks per second, 
which corresponds to 1 ms per tick, this is not mandatory, and some applications
might very well use different timer settings.

Since all system timeouts are actually implemented as timer ticks, it is
more accurate to use number of ticks, not milliseconds, for all timeouts.

This is also both a POSIX requirement, and a ISO C/C++ specs reqirement, to 
use clock ticks and round timings to the next clock interval.

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

### Small range for osKernelSysTick()

The 32-bits value of the returned counter allows only small time ranges.

For example, for a 100 MHz CPU, at 1000 ticks/sec, the SysTick counter divisor
is 100.000, or 10^5, allowing only 4*10^(9-5), or 40000 ticks, which are
only 40 seconds beween rollover.

Suggestion:

* widen the value to 64-bits.
* return separate ticks & cycles in a structure

### Threads

#### There are very few priority levels

There are only 7 thread priorityes, including idle, which might not be
enough.

Suggestion:

* extend the range to 256 levels, and represent it as unsigned (0-255).

#### There is no mechanism to tell if a thread has terminated or not

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

### Mutexes

#### There is no way to poll the mutex if available

It is not possible to acquire a mutex only if available, and not block if busy.

Suggestion:

* add `try_wait(void)` 

#### There is no recursive mutex

The specs do not define if the mutex is normal or recursive. However, RTX
implements recursive mutexes.

Suggestion:

* add an explicit and documented method to create normal and recursive mutexes.

#### The single osMutexWait() has three different functions

In POSIX there are three different functions to lock a mutex:

```
  lock();
  try_lock();
  timed_lock();
```

In CMSIS RTOS all are multiplexed on a single call, with a timeout parameter
that may be 0, if try_lock() is needed, or a timeout, with 0xFFFFFFFF to mean
_forever_.

Although functionally equivalent, the POSIX design increases readability 
and is prefered.
  
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
