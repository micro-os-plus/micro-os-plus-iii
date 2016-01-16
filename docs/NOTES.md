# CMSIS Divers 

## Criticism

* non-reentrant callbacks
* return aggregates (version, capabilities, status)
* driver defined as pointer to struct of functions, not to data, which makes code reusability difficult for multiple similar implementations
* Initialize() enable clocks, which means it powers up the device
* callbacks registered together with hardware inits
* initialize/unitialize must be linked to power up/down

### USART

* configuration mixed with operational commands

### USBD

* inconsecvent status name (ARM\_USBD\_STATE, vs ARM\_USART\_STATUS)
