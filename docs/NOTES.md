# Criticism

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

## RTOS

* makes use of dinamic allocator for stack, queues, etc
* makes heavy use of macros to define static object instances and to refer to them
* should not return agregate (osEvent)
* has no support for critical sections (interrupts & scheduler)
* has no public memory allocator
* inconsecvent names (thread name, timer name & function)
* missing osPoolDelete(), osMessageDelete(), osMailDelete()

Suggestions:

* add interrupts mask/restore
* add scheduler stop/restore
* add malloc/free

### Semaphore

* confusing definition for binary

Suggestion:

* add max count, default 0xFFFFFFFF
