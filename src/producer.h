#ifndef MAIN_SRC_PRODUCER_H
#define MAIN_SRC_PRODUCER_H
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

msg_t * produce_blocking_signal_before(buffer_t *, msg_t* );
msg_t * produce_blocking(buffer_t *, msg_t* );





#endif
