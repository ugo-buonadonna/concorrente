#ifndef MAIN_SRC_PRODUCER_H
#define MAIN_SRC_PRODUCER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "../../main/src/types.h"

void produce_blocking_signal_before(buffer_t *, msg_t* );
void produce_blocking(buffer_t *, msg_t* );
void produce_non_blocking(void**);





#endif
