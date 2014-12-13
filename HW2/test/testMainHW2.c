/*
 * testMainHW2.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */


#include "testMainHW2.h"

msg_t** msgs;
buffer_t* provider_buffer;

int init_suite_hwc2(void) {
	return 0;
}

int clean_suite_hwc2(void) {
	return 0;
}
msg_t** init_msgs(int num) {
	msg_t** msgs = (msg_t**) malloc(sizeof(msg_t*) * num);
	char msg_content[15];
	int i;
	for(i=0;i<num;i++) {
		sprintf(msg_content, "Messaggio%d", i);
		msgs[i] = msg_init_string(msg_content);
	}
	return msgs;
}

void free_msgs(msg_t** msgs,int num) {
	int i;
	for(i=0;i<num;i++)
		msg_destroy_string(msgs[i]);
	free(msgs);
}
//
void init_providerUnMessaggio(void)  {
	msgs = init_msgs(1);
	provider_buffer = buffer_init(2);
}
void clean_providerUnMessaggio(void)  {
	buffer_destroy(provider_buffer);
	free_msgs(msgs,1);
}
void test_providerUnMessaggio(void) {
	init_providerUnMessaggio();

	int num_sent = send_dispatcher_buf(provider_buffer,msgs,1);
	int msgs_in_buf = get_num_messaggi(provider_buffer);

	//verifico che abbia spedito 2 messaggi, di cui l'ultimo poison pill
	CU_ASSERT_EQUAL(num_sent,msgs_in_buf);
	CU_ASSERT_EQUAL(provider_buffer->msg_array[1],POISON_PILL);

	clean_providerUnMessaggio();
}

void init_readerUnMessaggio(void)  {
	msgs = init_msgs(1);
	provider_buffer = buffer_init(2);
}
void clean_readerUnMessaggio(void)  {
	buffer_destroy(provider_buffer);
	free_msgs(msgs,1);
}
void test_readerUnMessaggio(void) {
	init_providerUnMessaggio();

	int num_sent = send_dispatcher_buf(provider_buffer,msgs,1);
	int msgs_in_buf = get_num_messaggi(provider_buffer);

	//verifico che abbia spedito 2 messaggi, di cui l'ultimo poison pill
	CU_ASSERT_EQUAL(num_sent,msgs_in_buf);
	CU_ASSERT_EQUAL(provider_buffer->msg_array[1],POISON_PILL);

	clean_providerUnMessaggio();
}


const char* callTestsHWC2() {
	CU_pSuite hwc2_suite = NULL;
	CU_initialize_registry();
	hwc2_suite = CU_add_suite("hwc2 Testing", init_suite_hwc2,
			clean_suite_hwc2);

	CU_add_test(hwc2_suite, "Test provider con un messaggio",
			test_providerUnMessaggio);
	/*CU_add_test(hwc2_suite, "Test provider con un messaggio",
				test_providerUnMessaggio);*/

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
