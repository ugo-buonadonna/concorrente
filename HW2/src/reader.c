/*
 * reader.c
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#include "reader.h"
#include "../../HW1/src/types.h"

/*
i reader sono creati a seguito di esplicite richieste asincrone presentate
al flusso accepter e quindi collocati all’interno della Lista dei reader; so-
lo dopo l’evasione della richiesta possono essere presi in considerazione
dal dispatcher
*/

//Legge a velocità variabile messaggi dal reader_buffer. Ritorna il num. di messaggi letti
void* read_until_ppill(void *arg) {
	buffer_t* reader_buffer = (buffer_t*) arg;
	int count=0;
	while(get_bloccante(reader_buffer) != POISON_PILL)  {
		count++;
		sleep((rand() % 2500) / 1000); //Elemento di variabilità temporale (tra 0 e 2.5 secondi)
	}
	void* thread_return_value = (void*)count;
	return thread_return_value;
}
