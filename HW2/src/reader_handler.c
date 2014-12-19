/*
 * reader_handler.c
 *
 *  Created on: Dec 16, 2014
 *      Author: ugo
 */

#include "reader_handler.h"

//Creo un thread che si occupa della gestione di un reader.
//Ha la responsabilità di aggiungere alla coda il reader,
//nonchè rimuoverlo e deallocarlo una volta terminato.
//
void create_reader(void* reader_list) {
	pthread_t reader;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&reader,&attr,start_reader_handler,reader_list);
	pthread_attr_destroy (&attr);
}



int start_reader_handler(void* reader_list) {
	//inzializzo la struttura dati che rappresenta un reader.
	//E la inserisco nella lista dei reader correnti
	s_list *readers = (s_list *)  reader_list;
	pthread_t self = pthread_self();
	reader_fde* this_reader = init_reader_fde(self);
	safeAddElement(readers,(void*)this_reader);

	//Subito prima di inziare a leggere dal buffer, segnalo che sto per partire
	if(reader_handler_started != NULL)
		inc_flag(reader_handler_started);

	//inizio il reading vero e proprio
	int count = start_reader(this_reader->reader_buffer);

	//Appena finito, rimuovo dalla lista di reader la struttura dati
	//che lo rappresenta
	if(!safeRemoveElement(readers,(void*)this_reader))
		fprintf (stderr, "Errore di concorrenza: rimozione di elemento inesistente");
	destroy_reader_fde(this_reader);

	//Segnalo che sto per uscire, quindi la distruzione
	//della coda di liste è ora safe
	if(reader_handler_started != NULL)
		inc_flag(reader_handler_started);
	return count;
}


