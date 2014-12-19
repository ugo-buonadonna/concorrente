/*
 * testReaderHandler.c
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#include "testReaderHandler.h"

msg_t** msgs;
buffer_t* reader_buffer;
s_list* reader_list;
pthread_t thread1;
pthread_t thread2;
iterator_t* reader_list_iterator;

void init_startReaderHandler_creazione1reader(void)  {
	reader_list = safe_list_init();
	reader_handler_started =  condvar_init(0);
}
void clean_startReaderHandler_creazione1reader(void)  {
	safe_list_destroy(reader_list);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
}
void test_startReaderHandler_creazione1reader(void) {
	init_startReaderHandler_creazione1reader();

	//sollecitazione
	pthread_create(&thread1,NULL,start_reader_handler,(void*)reader_list);

	//verifica
	wait_flag_change(reader_handler_started);

	//A questo punto il test è già passato, invio poison pill al reader
	wait_readers_termination(reader_list,1);
	pthread_join(thread1,NULL);
	CU_PASS();

	clean_startReaderHandler_creazione1reader();
}


void init_startReaderHandler_inserimentoLista1reader(void)  {
	reader_list = safe_list_init();
	reader_handler_started =  condvar_init(0);
}
void clean_startReaderHandler_inserimentoLista1reader(void)  {
	safe_list_destroy(reader_list);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	iterator_destroy(reader_list_iterator);
}
void test_startReaderHandler_inserimentoLista1reader(void) {
	init_startReaderHandler_creazione1reader();

	//sollecitazione
	pthread_create(&thread1,NULL,start_reader_handler,(s_list*)reader_list);

	//Verifica (size non thread safe ma per scopi di testing).
	wait_flag_change(reader_handler_started);
	CU_ASSERT_EQUAL(size(reader_list->list),1);

	//Termino il reader
	wait_readers_termination(reader_list,1);
	pthread_join(thread1,NULL);

	clean_startReaderHandler_creazione1reader();
}

void init_startReaderHandler_inserimentoLista2reader(void)  {
	reader_handler_started =  condvar_init(0);
	reader_list = safe_list_init();
}
void clean_startReaderHandler_inserimentoLista2reader(void)  {
	safe_list_destroy(reader_list);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	iterator_destroy(reader_list_iterator);
}
void test_startReaderHandler_inserimentoLista2reader(void) {
	init_startReaderHandler_inserimentoLista2reader();

	//sollecitazione
	pthread_create(&thread1,NULL,start_reader_handler,(s_list*)reader_list);
	pthread_create(&thread2,NULL,start_reader_handler,(s_list*)reader_list);

	//Verifica (size non thread safe ma per scopi di testing).
	wait_flag_value(reader_handler_started,2);
	CU_ASSERT_EQUAL(size(reader_list->list),2);

	//A questo punto il test è già passato, invio poison pill ai reader
	//UNABLE TO LOCK MUTEX 16
	wait_readers_termination(reader_list,2);
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	CU_PASS();

	clean_startReaderHandler_inserimentoLista2reader();
}




void init_createReader_creazione1reader(void)  {
	reader_list = safe_list_init();
	reader_handler_started =  condvar_init(0);
}
void clean_createReader_creazione1reader(void)  {
	safe_list_destroy(reader_list);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
}
void test_createReader_creazione1reader(void) {
	init_createReader_creazione1reader();

	//sollecitazione
	create_reader(reader_list);

	//Verifica
	wait_flag_change(reader_handler_started);

	//Test passato. Invio poison pill
	wait_readers_termination(reader_list,1);
	CU_PASS();

	clean_createReader_creazione1reader();
}



void init_createReader_creazione2reader(void)  {
	reader_list = safe_list_init();
	reader_handler_started =  condvar_init(0);
}
void clean_createReader_creazione2reader(void)  {
	safe_list_destroy(reader_list);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
}
void test_createReader_creazione2reader(void) {
	init_createReader_creazione2reader();

	//sollecitazione
	create_reader(reader_list);
	create_reader(reader_list);

	//Verifica
	wait_flag_value(reader_handler_started,2);

	//Test passato. Invio poison pill
	wait_readers_termination(reader_list,2);
	CU_PASS();

	clean_createReader_creazione2reader();
}
