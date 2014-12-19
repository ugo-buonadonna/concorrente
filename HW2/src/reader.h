/*
 * reader.h
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_READER_H_
#define MAIN_HW2_SRC_READER_H_

#include "types.h"

//ogni flusso reader è caratterizzato da un thread identifier
//e da un reader_buffer.
//Inizializzare un reader significa anche avviarlo, ponendolo in attesa di messaggi
//ricevuti nel proprio buffer

typedef struct  {
	pthread_t reader_id;
	buffer_t *reader_buffer;
} reader_fde;

reader_fde * init_reader_fde(pthread_t);
void destroy_reader_fde(reader_fde *);

int start_reader(buffer_t*);

#endif /* MAIN_HW2_SRC_READER_H_ */
