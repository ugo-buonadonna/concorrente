/*
 * main_fde.c
 *
 *  Created on: Dec 22, 2014
 *      Author: ugo
 */

#include "main_fde.h"

#define NANO_SECOND_MULTIPLIER  1000000

main_fde_t* main_fde_init(msg_t** provider_msgs,int len) {
	main_fde_t* new_main = (main_fde_t*)malloc(sizeof(main_fde_t));
	new_main->initial_provider_msgs = provider_msgs;
	new_main->initial_provider_msgs_len = len;
	new_main->accepter_buffer = buffer_init(READER_BUF_DIM);
	new_main->current_readers = safe_list_init();
	new_main->provider_buffer = buffer_init(READER_BUF_DIM);
	new_main->test_signal = condvar_init(0);
	new_main->init = main_fde_init;
	new_main->destroy = main_fde_destroy;
	new_main->start = start_main_fde;
	return new_main;
}

void main_fde_destroy(main_fde_t* fde) {
	condvar_destroy(fde->test_signal);
	buffer_destroy(fde->accepter_buffer);
	buffer_destroy(fde->provider_buffer);
	safe_list_destroy(fde->current_readers);
	free(fde);
}
void* start_main_fde (void* arg) {
	main_fde_t* main = (main_fde_t*)arg;
	pthread_t provider;
	pthread_t dispatcher;
	pthread_t accepter;

	struct start_accepter_params sap;
	sap.accepter_buffer = main->accepter_buffer;
	sap.current_readers = main->current_readers;

	struct start_dispatcher_args sda;
	sda.current_readers = main->current_readers;
	sda.provider_buffer = main->provider_buffer;

	struct start_provider_params spa;
	spa.provider_buffer = main->provider_buffer;
	spa.mesgs = main->initial_provider_msgs;
	spa.len = main->initial_provider_msgs_len;

	int res=0;

	res = res || pthread_create(&dispatcher,NULL,start_dispatcher,&sda);
	res = res || pthread_create(&accepter,NULL,start_accepter,&sap);
	res = res || pthread_create(&provider,NULL,start_provider,&spa);

	if(res != 0 ) {
		perror("Error while creating threads in main fde");
		return 2;
	}

	//qua arriveranno submit_request dall'esterno

	pthread_join(provider,NULL);
	//il provider è terminato, invio poison_pill ad accepter
	if(put_bloccante(main->accepter_buffer,POISON_PILL) == POISON_PILL)
		inc_flag(main->test_signal);

	pthread_join(dispatcher,NULL);
	pthread_join(accepter,NULL);

	lock_list(main->current_readers);
	iterator_t* it = iterator_init(main->current_readers->list);
	int count = copy_and_send_to_all(1,it,POISON_PILL);

	iterator_destroy(it);
	unlock_list(main->current_readers);

	printf("-main_fde terminating-");
	return count;

}
