/*
 * testDispatcher.c
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#include "testDispatcher.h"

msg_t** msgs;
buffer_t* provider_buffer;
buffer_t* reader_buffer;
s_list* reader_list;
pthread_t reader;
void* msgs_read;

struct start_dispatcher_args sda;



//Versione lenta del reader utilizzata solo per scopi di testing
int start_reader_lento(buffer_t* reader_buffer) {
	int count=0;
	struct timespec tim, tim2;
	while(get_bloccante(reader_buffer) != POISON_PILL)  {
		//il reader lento aspetta 5 secondi
		sleep(5);
	}
	printf("-Reader poisoned-");
	return count;
}


//funzione per creare un reader lento, utilizzata solo per scopi di testing
int reader_handler_lento(void* reader_list) {
	//inzializzo la struttura dati che rappresenta un reader.
	//E la inserisco nella lista dei reader correnti
	s_list *readers = (s_list *)  reader_list;
	pthread_t self = pthread_self();
	reader_fde* this_reader = init_reader_fde(self);
	this_reader->reader_buffer = buffer_init(1);
	safeAddElement(readers,(void*)this_reader);

	//inizio il reading vero e proprio
	int count = start_reader_lento(this_reader->reader_buffer);

	//Appena finito, rimuovo dalla lista di reader la struttura dati
	//che lo rappresenta
	if(!safeRemoveElement(readers,(void*)this_reader))
		fprintf (stderr, "Errore di concorrenza: rimozione di elemento inesistente");
	destroy_reader_fde(this_reader);

	inc_flag(reader_handler_started);
	return count;
}
//Crea un thread detached rappresentante il reader lento
void create_reader_lento(void* reader_list) {
	pthread_t reader;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&reader,&attr,start_reader_handler,reader_list);
	pthread_attr_destroy (&attr);
}





void init_start_dispatcher_1msg1reader(void)  {
	provider_buffer = buffer_init(2);
	msgs = init_msgs(1);
	reader_list = safe_list_init();
	reader_handler_started = condvar_init(0);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,POISON_PILL);
	pthread_create(&reader,NULL,start_reader_handler,(void*)reader_list);
	wait_flag_change(reader_handler_started);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_1msg1reader(void)  {
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(provider_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,1);
}
void test_start_dispatcher_1msg1reader(void) {
	init_start_dispatcher_1msg1reader();

	//sollecitazione
	start_dispatcher(&sda);
	pthread_join(reader,&msgs_read);

	//verifico che il reader abbia letto 1 messaggio (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read,1);

	clean_start_dispatcher_1msg1reader();
}




void init_start_dispatcher_1readerLento_2messaggi(void)  {
	provider_buffer = buffer_init(3);
	reader_handler_started = condvar_init(0);
	msgs = init_msgs(2);
	reader_list = safe_list_init();
	create_reader_lento(reader_list);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,msgs[1]);
	put_bloccante(provider_buffer,POISON_PILL);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_1readerLento_2messaggi(void)  {
	wait_flag_change(reader_handler_started);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(provider_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,1);
}
void test_start_dispatcher_1readerLento_2messaggi(void) {
	init_start_dispatcher_1readerLento_2messaggi();

	//sollecitazione
	int slow_readers = start_dispatcher(&sda);
	//verifico che il dispatcher abbia trovato 1 reader lento
	CU_ASSERT_EQUAL(slow_readers,1);

	clean_start_dispatcher_1readerLento_2messaggi();
}









void init_start_dispatcher_2msg1reader(void)  {
	provider_buffer = buffer_init(3);
	msgs = init_msgs(2);
	reader_list = safe_list_init();
	reader_handler_started = condvar_init(0);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,msgs[1]);
	put_bloccante(provider_buffer,POISON_PILL);
	pthread_create(&reader,NULL,start_reader_handler,reader_list);
	wait_flag_change(reader_handler_started);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_2msg1reader(void)  {
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(provider_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,2);
}
void test_start_dispatcher_2msg1reader(void) {
	init_start_dispatcher_2msg1reader();

	//sollecitazione
	start_dispatcher(&sda);
	pthread_join(reader,&msgs_read);

	//verifico che il reader abbia letto 2 messaggi (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read,2);

	clean_start_dispatcher_2msg1reader();
}













void init_start_dispatcher_1readerLento(void)  {
	provider_buffer = buffer_init(3);
	msgs = init_msgs(2);
	reader_list = safe_list_init();
	reader_handler_started = condvar_init(0);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,msgs[1]);
	put_bloccante(provider_buffer,POISON_PILL);
	pthread_create(&reader,NULL,start_reader_handler,reader_list);
	wait_flag_change(reader_handler_started);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_1readerLento(void)  {
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(provider_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,2);
}
void test_start_dispatcher_1readerLento(void) {
	init_start_dispatcher_1readerLento();

	//sollecitazione
	start_dispatcher(&sda);
	pthread_join(reader,&msgs_read);

	//verifico che il reader abbia letto 2 messaggi (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read,2);

	clean_start_dispatcher_1readerLento();
}

