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
