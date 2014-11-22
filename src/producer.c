/*
 * producer.c
 *
 *  Created on: Nov 22, 2014
 *      Author: ugo
 */


#include "producer.h"


extern condvar_t* thread_sync;

msg_t * produce_blocking_signal_before(buffer_t *buffer, msg_t* msg) {
	set_flag(thread_sync,1);
	msg_t * mex = put_bloccante(buffer, msg);
	pthread_exit((msg_t *)mex);
}


msg_t * produce_blocking(buffer_t *buffer, msg_t* msg) {
	msg_t * mex = put_bloccante(buffer, msg);
	set_flag(thread_sync,1);
	pthread_exit((msg_t *)mex);
}
