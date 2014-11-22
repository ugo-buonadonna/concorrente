/*
 * consumer.c
 *
 *  Created on: Nov 20, 2014
 *      Author: ugo
 */


#include "consumer.h"

extern condvar_t* thread_sync;

msg_t * prelevaBloccante(buffer_t *buffer) {
	/* sto assumendo di venir chiamato come codice di un nuovo thread */
	int ret;
	set_flag(thread_sync,1);
	msg_t * mex = get_bloccante(buffer);
	pthread_exit((msg_t *)mex);
}
