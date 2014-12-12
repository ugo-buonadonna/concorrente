#ifndef CONSUMER_H
#define CONSUMER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "types.h"

void consume_blocking_and_signal_before(buffer_t *buffer);

void consume_blocking_and_signal_after(buffer_t *buffer);

void consume_non_blocking_wait_signal(buffer_t *buffer);

void consume_non_blocking_and_signal_after(buffer_t *buffer) ;



#endif
