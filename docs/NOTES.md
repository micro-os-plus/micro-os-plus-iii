# CMSIS Divers 

## Criticism

* non-reentrant callbacks
* pointer to functions, not to data
* initialize enable clocks, which means powers up the device
* register callbacks linked to hardware inits
* initialize/unitialize linked to power up/down
* register callbacks linked with hardware inits

### USART

* configuration mixed with operational commands

### USBD

* inconsecvent status name (ARM\_USBD\_STATE, vs ARM\_USART\_STATUS)
