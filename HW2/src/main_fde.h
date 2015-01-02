/*
 * main_fde.h
 *
 *  Created on: Dec 22, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_MAIN_FDE_H_
#define MAIN_HW2_SRC_MAIN_FDE_H_

#include "types.h"
#include "../../HW1/src/types.h"

typedef struct main_fde {
	buffer_t* accepter_buffer;
	buffer_t* provider_buffer;
	s_list* current_readers;
	msg_t** initial_provider_msgs;
	int initial_provider_msgs_len;
	condvar_t* test_signal;
	struct main_fde* (*init)(msg_t**,int);
	void (*start)(void*);
	void (*destroy)(struct main_fde*);
}main_fde_t;

void* start_main_fde(void* );
void main_fde_destroy(main_fde_t* );
main_fde_t* main_fde_init(msg_t**,int);

#endif /* MAIN_HW2_SRC_MAIN_FDE_H_ */
