/*
 * accepter.c
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#include "accepter.h"

//Avvio di un fde reader. Finchè non riceve una poison pill, crea flussi reader.
void *start_accepter(void* args) {
	struct start_accepter_params *p =  (struct start_accepter_params*) args;
	msg_t* current;
	while ((current = get_bloccante(p->accepter_buffer)) != POISON_PILL) {
		create_reader(p->current_readers);
	}
	//received poison pill
	printf("Accepter got poison pill, terminating...");
	return NULL;
}

