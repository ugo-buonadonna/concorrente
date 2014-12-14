/*
 * provider.h
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_PROVIDER_H_
#define MAIN_HW2_SRC_PROVIDER_H_

#include "types.h"

struct send_dispatcher_buf_params {
	buffer_t *provider_buffer;
	msg_t** mesgs;
	int len;
};

void* send_dispatcher_buf(void*);


#endif /* MAIN_HW2_SRC_PROVIDER_H_ */
