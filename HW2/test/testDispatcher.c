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
pthread_t thread1;

struct start_dispatcher_args sda;


void init_start_dispatcher_1msg1reader(void)  {
	provider_buffer = buffer_init(2);
	msgs = init_msgs(1);
	reader_list = safe_list_init();
	put_bloccante(provider_buffer,msgs[0]);
	put_bloccante(provider_buffer,POISON_PILL);
	create_reader(reader_list);
	sda.current_readers = reader_list;
	sda.provider_buffer = provider_buffer;

}
void clean_start_dispatcher_1msg1reader(void)  {
	buffer_destroy(reader_buffer);
	safe_list_destroy(reader_list);
	free_msgs(msgs,1);
}
void test_start_dispatcher_1msg1reader(void) {
	init_start_dispatcher_1msg1reader();

	start_dispatcher(&sda);
	CU_PASS();
	//Violazione assunzione di progresso finito


	clean_start_dispatcher_1msg1reader();
}
