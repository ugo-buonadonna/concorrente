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
pthread_t reader1;
pthread_t reader2;
void* msgs_read1;
void* msgs_read2;


struct start_dispatcher_args sda;



//Versione lenta del reader utilizzata solo per scopi di testing
int start_reader_lento(buffer_t* reader_buffer) {
	int count=0;
	while(get_bloccante(reader_buffer) != POISON_PILL)  {
		//il reader lento aspetta 3 secondi
		sleep(3);
	}
	printf("-Reader poisoned-");
	return count;
}


//funzione per creare un reader lento, utilizzata solo per scopi di testing
//quasi analoga alla reader_handler di reader_handler.c
//Per questo ometto i commenti
int reader_handler_lento(void* reader_list) {
	s_list *readers = (s_list *)  reader_list;
	pthread_t self = pthread_self();
	reader_fde* this_reader = init_reader_fde(self);
	this_reader->reader_buffer = buffer_init(1);
	safeAddElement(readers,(void*)this_reader);

	int count = start_reader_lento(this_reader->reader_buffer);

	safeRemoveElement(readers,(void*)this_reader);
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
	pthread_create(&reader,&attr,reader_handler_lento,reader_list);
	pthread_attr_destroy (&attr);
}





void init_start_dispatcher_1msg1reader(void)  {
	provider_buffer = buffer_init(2);
	msgs = init_msgs(1);
	reader_list = safe_list_init();
	reader_handler_started = condvar_init(0);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,POISON_PILL);
	pthread_create(&reader1,NULL,start_reader_handler,(void*)reader_list);
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
	pthread_join(reader1,&msgs_read1);

	//verifico che il reader abbia letto 1 messaggio (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read1,1);

	clean_start_dispatcher_1msg1reader();
}




void init_start_dispatcher_1readerLento_2messaggi(void)  {
	provider_buffer = buffer_init(4);
	reader_handler_started = condvar_init(0);
	msgs = init_msgs(3);
	reader_list = safe_list_init();
	create_reader_lento(reader_list);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,msgs[1]);
	put_bloccante(provider_buffer,msgs[2]);
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
	free_msgs(msgs,3);
}
void test_start_dispatcher_1readerLento_2messaggi(void) {
	init_start_dispatcher_1readerLento_2messaggi();
	//Aspetto che il thread detached Reader sia in attesa di messaggi
	//Violazione dell'assunzione di progresso finito
	sleep(1);
	//sollecitazione
	int slow_readers = start_dispatcher(&sda);
	//verifico che il dispatcher abbia
	//eliminato dalla lista il reader lento
	//Altra violazione di progresso finito
	CU_ASSERT_EQUAL(size(reader_list->list),0);

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
	pthread_create(&reader1,NULL,start_reader_handler,reader_list);
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
	pthread_join(reader1,&msgs_read1);

	//verifico che il reader abbia letto 2 messaggi (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read1,2);

	clean_start_dispatcher_2msg1reader();
}

void init_start_dispatcher_2msg2reader(void)  {
	provider_buffer = buffer_init(3);
	msgs = init_msgs(2);
	reader_list = safe_list_init();
	reader_handler_started = condvar_init(0);
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,msgs[1]);
	put_bloccante(provider_buffer,POISON_PILL);
	pthread_create(&reader1,NULL,start_reader_handler,reader_list);
	pthread_create(&reader2,NULL,start_reader_handler,reader_list);
	wait_flag_value(reader_handler_started,2);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_2msg2reader(void)  {
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(provider_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,2);
}
void test_start_dispatcher_2msg2reader(void) {
	init_start_dispatcher_2msg2reader();

	//sollecitazione
	start_dispatcher(&sda);
	pthread_join(reader1,&msgs_read1);
	pthread_join(reader2,&msgs_read2);

	//verifico che i reader abbiano letto 2 messaggi (esclusa ppill)
	CU_ASSERT_EQUAL((int)msgs_read1,2);
	CU_ASSERT_EQUAL((int)msgs_read1,2);

	clean_start_dispatcher_2msg2reader();
}
