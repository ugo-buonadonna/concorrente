/*
 * consumer.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ugo
 */


#include "../../main/src/consumer.h"

extern condvar_t* thread_sync;

void consume_blocking_and_signal_before(buffer_t *buffer) {
	set_flag(thread_sync,1);
	msg_t * mex = get_bloccante(buffer);
	pthread_exit((msg_t *)mex);
}

void consume_blocking_and_signal_after(buffer_t *buffer) {

	msg_t * mex = get_bloccante(buffer);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)mex);
}

void consume_non_blocking_and_signal_after(buffer_t *buffer) {

	msg_t * mex = get_non_bloccante(buffer);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)mex);
}

void consume_non_blocking_wait_signal(buffer_t *buffer) {
	wait_flag_change(thread_sync);
	msg_t * mex = get_non_bloccante(buffer);
	pthread_exit((msg_t *)mex);
}


