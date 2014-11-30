/*
 * producer.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ugo
 */


#include "../../main/src/producer.h"


extern condvar_t* thread_sync;

void produce_blocking_signal_before(buffer_t *buffer, msg_t* msg) {
	set_flag(thread_sync,1);
	msg_t * mex = put_bloccante(buffer, msg);
	pthread_exit((msg_t *)mex);
}


void produce_blocking(buffer_t *buffer, msg_t* msg) {
	msg_t * mex = put_bloccante(buffer, msg);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)mex);
}

void produce_non_blocking(void** args) {
	buffer_t* buffer = (buffer_t*) args[0];
	msg_t * msg = (msg_t *) args[1];
	msg = put_non_bloccante(buffer, msg);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)msg);
}
