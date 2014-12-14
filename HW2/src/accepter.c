/*
 * accepter.c
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#include "accepter.h"

void submitRequest(buffer_t *requests, char name[]);

void *accept_requests(buffer_t *accepted_buffer) {
	msg_t* current;
	while ((current = get_bloccante(accepted_buffer)) != POISON_PILL) {

	}
}

