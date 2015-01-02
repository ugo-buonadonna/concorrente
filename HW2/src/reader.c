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

//Funzione costruttrice di un elemento rappresentante un flusso di esecuzione Reader
//Usata dal reader_handler per gestire il flusso in coda.
reader_fde * init_reader_fde(pthread_t reader_id) {
	reader_fde* new_reader = (reader_fde*)malloc(sizeof(reader_fde));
	buffer_t* new_buffer = buffer_init(READER_BUF_DIM);
	new_reader->reader_id = reader_id;
	new_reader->reader_buffer = new_buffer;
	return new_reader;
}

void destroy_reader_fde(reader_fde* reader) {
	buffer_destroy(reader->reader_buffer);
	free(reader);
}

//Legge a velocità variabile messaggi dal reader_buffer. Ritorna il num. di messaggi letti
int start_reader(buffer_t* reader_buffer) {
	int count=0;
	while(get_bloccante(reader_buffer) != POISON_PILL)  {
		count++;
		sleep((rand() % 2500) / 1000); //Elemento di variabilità temporale (tra 0 e 2.5 secondi)
	}
	printf("-Reader poisoned-");
	return count;
}
