/*
 * testProvider.c
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#include "testProvider.h"


msg_t** msgs;
buffer_t* provider_buffer;
struct send_dispatcher_buf_params sdbp;
pthread_t thread1;

void init_providerUnMessaggio(void)  {
	msgs = init_msgs(1);
	provider_buffer = buffer_init(2);
	sdbp.provider_buffer = provider_buffer;
	sdbp.mesgs = msgs;
	sdbp.len = 1;
}
void clean_providerUnMessaggio(void)  {
	buffer_destroy(provider_buffer);
	free_msgs(msgs,1);
}
void test_providerUnMessaggio(void) {
	init_providerUnMessaggio();

	pthread_create(&thread1,NULL,send_dispatcher_buf,&sdbp);
	pthread_join(thread1,NULL);

	//verifico che abbia spedito 2 messaggi, di cui l'ultimo poison pill
	CU_ASSERT_EQUAL(provider_buffer->msg_array[0],msgs[0]);
	CU_ASSERT_EQUAL(provider_buffer->msg_array[1],POISON_PILL);

	clean_providerUnMessaggio();
}


void init_providerDueMessaggi(void)  {
	msgs = init_msgs(2);
	provider_buffer = buffer_init(3);
	sdbp.provider_buffer = provider_buffer;
	sdbp.mesgs = msgs;
	sdbp.len = 2;
}
void clean_providerDueMessaggi(void)  {
	buffer_destroy(provider_buffer);
	free_msgs(msgs,2);
}
void test_providerDueMessaggi(void) {
	init_providerDueMessaggi();

	pthread_create(&thread1,NULL,send_dispatcher_buf,&sdbp);
	pthread_join(thread1,NULL);

	//verifico che abbia spedito 3 messaggi, di cui l'ultimo poison pill
	CU_ASSERT_EQUAL(provider_buffer->msg_array[0],msgs[0]);
	CU_ASSERT_EQUAL(provider_buffer->msg_array[1],msgs[1]);
	CU_ASSERT_EQUAL(provider_buffer->msg_array[2],POISON_PILL);

	clean_providerDueMessaggi();
}
