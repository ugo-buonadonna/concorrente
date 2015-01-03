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

struct put_bloccante_params {
	buffer_t* buffer;
	msg_t* message;
};
void* start_dispatcher(void* args);
int copy_and_send_to_all(s_list* reader_list, msg_t* current);

#endif /* MAIN_HW2_SRC_DISPATCHER_H_ */
