/*
 * dispatcher.h
 *
 *  Created on: Dec 16, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_DISPATCHER_H_
#define MAIN_HW2_SRC_DISPATCHER_H_

#include "types.h"

struct start_dispatcher_args {
	buffer_t* provider_buffer;
	s_list* current_readers;
};
void* start_dispatcher(void* args);
void copy_and_send_to_all(int , iterator_t* , msg_t* );

#endif /* MAIN_HW2_SRC_DISPATCHER_H_ */
