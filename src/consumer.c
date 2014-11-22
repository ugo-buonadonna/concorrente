/*
 * consumer.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ugo
 */


#include "consumer.h"

extern condvar_t* thread_sync;

msg_t * consume_blocking_signal_after(buffer_t *buffer) {
	set_flag(thread_sync,1);
	msg_t * mex = get_bloccante(buffer);
	pthread_exit((msg_t *)mex);
}

msg_t * consume_blocking(buffer_t *buffer) {
	msg_t * mex = get_bloccante(buffer);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)mex);
}
