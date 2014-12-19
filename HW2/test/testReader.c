/*
 * testReader.c
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */


#include "testReader.h"


msg_t** msgs;
buffer_t* reader_buffer;
s_list* reader_list;
pthread_t thread1;

//Testo solamente la funzione core del reader, escludendo quindi
//il reader_handler
void init_start_reader_1reader1Messaggio(void)  {
	msgs = init_msgs(1);
	reader_buffer = buffer_init(2);
	put_non_bloccante(reader_buffer,msgs[0]);
	put_non_bloccante(reader_buffer,POISON_PILL);
}
void clean_start_reader_1reader1Messaggio(void)  {
	buffer_destroy(reader_buffer);
	free_msgs(msgs,1);
}
void test_start_reader_1reader1Messaggio(void) {
	init_start_reader_1reader1Messaggio();

	void* num_letti;
	pthread_create(&thread1,NULL,start_reader,(buffer_t*)reader_buffer);
	pthread_join(thread1,&num_letti);
	//verifico che abbia letto 1 messaggio, esclusa la poison pill
	CU_ASSERT_EQUAL((int)num_letti,1);

	clean_start_reader_1reader1Messaggio();
}

