/*
 * accepter.h
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_ACCEPTER_H_
#define MAIN_HW2_SRC_ACCEPTER_H_

#include "types.h"
struct start_accepter_params {
	buffer_t *accepter_buffer;
	s_list *current_readers;
};

void *start_accepter(void*);



#endif /* MAIN_HW2_SRC_ACCEPTER_H_ */
