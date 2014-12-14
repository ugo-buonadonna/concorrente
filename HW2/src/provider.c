/*
 * provider.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */

#include "provider.h"
#include "../../HW1/src/types.h"
/*
il provider spedisce una sequenza finita di messaggi al dispatcher; la
sequenza `e sempre terminata da una poison pill e dopo il suo invio il
provider termina spontaneamente
*/

//funzione per inviare msg nel provider_buffer. Ritorna il num. di messaggi inviati.
void *send_dispatcher_buf(void* arg) {
	struct send_dispatcher_buf_params *p = (struct send_dispatcher_buf_params*) arg;

	int i,count=0;
	for(i=0;i<p->len;i++) {
		if( put_non_bloccante(p->provider_buffer,p->mesgs[i]) != BUFFER_ERROR)
			count++;
	}
	if( put_non_bloccante(p->provider_buffer,POISON_PILL)!= BUFFER_ERROR)
		count++;
	//printf("Provider sent %d messages",count);
	void* thread_return_value = (void*)count;
	return thread_return_value;
}
