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
int send_dispatcher_buf(buffer_t *provider_buffer,msg_t** mesgs, int len) {
	int i,count=0;
	for(i=0;i<len;i++) {
		if( put_non_bloccante(provider_buffer,mesgs[i]) != BUFFER_ERROR)
			count++;
	}
	if( put_non_bloccante(provider_buffer,POISON_PILL)!= BUFFER_ERROR)
		count++;
	//printf("Provider sent %d messages",count);
	pthread_exit((int*)&count);
	return count;
}
