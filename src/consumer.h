#ifndef CONSUMER_H
#define CONSUMER_H
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

msg_t * consume_blocking_signal_after(buffer_t *buffer);

msg_t * consume_blocking(buffer_t *buffer);





#endif
