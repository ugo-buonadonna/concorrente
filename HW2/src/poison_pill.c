/*
 * poison_pill.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */


#include "poison_pill.h"



msg_t* msg_init_pill(void * content){
	return POISON_PILL;
}
void msg_destroy_pill(msg_t * msg){
	/* do nothin */
}
msg_t* msg_copy_pill(msg_t* msg){
	return POISON_PILL;
}

const msg_t POISON_PILL_msg = {
		NULL,
		msg_init_pill,
		msg_destroy_pill,
		msg_copy_pill
};
