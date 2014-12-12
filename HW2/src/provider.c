/*
 * provider.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */

#include "provider.h"
/*
il provider spedisce una sequenza finita di messaggi al dispatcher; la
sequenza `e sempre terminata da una poison pill e dopo il suo invio il
provider termina spontaneamente
*/

int send_dispatcher_buf(buffer_t provider_buffer,msg_t* mesgs, int len) {
	int i,count=0;
	for(i=0;i<len;i++) {
		if( putNonBloccante(provider_buffer,mesgs[i]) != BUFFER_ERROR)
			count++;
	}
	putNonBloccante(provider_buffer,POISON_PILL);
	printf("Provider sent %d messages",count);
	return count;
}
