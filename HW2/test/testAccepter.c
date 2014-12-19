/*
 * testAccepter.c
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#include "testAccepter.h"

pthread_t thread1;
msg_t** msgs;
buffer_t* accepter_buffer;
buffer_t* reader_buffer;
s_list* reader_list;
iterator_t* reader_list_iterator;
struct start_accepter_params sap;


void init_start_accepter_creazione1Reader(void)  {
	accepter_buffer = buffer_init(2);
	msgs = init_msgs(1);
	put_bloccante(accepter_buffer,msgs[0]);
	put_bloccante(accepter_buffer,POISON_PILL);
	reader_handler_started = condvar_init(0);
	reader_list = safe_list_init();
	sap.accepter_buffer = accepter_buffer;
	sap.current_readers = reader_list;

}
void clean_start_accepter_creazione1Reader(void)  {
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	buffer_destroy(accepter_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,1);
}



void test_start_accepter_creazione1Reader(void) {
	init_start_accepter_creazione1Reader();

	//sollecitazione
	start_accepter(&sap);

	//verifica
	wait_flag_change(reader_handler_started);

	//test passato, reader creato.
	//Istigazione al suicidio.
	wait_readers_termination(reader_list,1);
	clean_start_accepter_creazione1Reader();
}




void init_start_accepter_creazione2Reader(void)  {
	accepter_buffer = buffer_init(3);
	msgs = init_msgs(2);
	reader_handler_started = condvar_init(0);
	put_bloccante(accepter_buffer,msgs[0]);
	put_bloccante(accepter_buffer,msgs[1]);
	put_bloccante(accepter_buffer,POISON_PILL);
	reader_list = safe_list_init();
	sap.accepter_buffer = accepter_buffer;
	sap.current_readers = reader_list;

}

void clean_start_accepter_creazione2Reader(void)  {
	buffer_destroy(accepter_buffer);
	condvar_destroy(reader_handler_started);
	reader_handler_started = NULL;
	safe_list_destroy(reader_list);
	free_msgs(msgs,2);
}
void test_start_accepter_creazione2Reader(void) {
	init_start_accepter_creazione2Reader();

	//sollecitazione
	start_accepter(&sap);

	//verifica
	wait_flag_value(reader_handler_started,2);

	//test passato, reader creato.
	//Istigazione al suicidio.
	wait_readers_termination(reader_list,2);
	CU_PASS();

	clean_start_accepter_creazione2Reader();
}
