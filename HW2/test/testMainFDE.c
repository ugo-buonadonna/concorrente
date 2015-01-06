/*
 * testMainFDE.c
 *
 *  Created on: Dec 28, 2014
 *      Author: ugo
 */


#include "testMainFDE.h"


main_fde_t* main_fde;
pthread_t* main_fde_thread;
msg_t* initial_msg;
msg_t** msg_array;
int len;


//Test di invio corretto di una poison pill all'accepter dopo terminazione del provider
void init_start_all_1ppillToAccepter(void)  {
	msg_array = (msg_t**)malloc(sizeof(msg_t*)*2);
	msg_array[0] = POISON_PILL;
	main_fde = main_fde_init(msg_array,1);

}
void clean_start_all_1ppillToAccepter(void)  {
	free(msg_array);
	main_fde_destroy(main_fde);
}
void test_start_all_1ppillToAccepter(void) {
	init_start_all_1ppillToAccepter();

	start_main_fde(main_fde);
	wait_flag_value(main_fde->test_signal,1);

	//Test passato, so con certezza che ha inviato la ppill all'accepter
	CU_PASS();

	clean_start_all_1ppillToAccepter();
}



void init_start_all_1ppillToReader(void)  {
	msg_array = init_msgs(1);
	msg_array[1] = POISON_PILL;
	main_fde = main_fde_init(msg_array,2);

}
void clean_start_all_1ppillToReader(void)  {
	free_msgs(msg_array,1);
	main_fde_destroy(main_fde);
}
void test_start_all_1ppillToReader(void) {
	init_start_all_1ppillToReader();

	int ppills_sent = start_main_fde(main_fde);
	CU_PASS();

	clean_start_all_1ppillToReader();
}

//può sembrare un test uguale a quello presente in testAccepter,
//ma qua sto testando la creazione di un reader in presenza
//di un sistema di flussi già messo in piedi
void init_start_all_creazione1reader(void)  {
	main_fde = main_fde_init(msg_array,1);
	reader_handler_started = condvar_init(0);
	pthread_create(&main_fde_thread,NULL,start_main_fde,(void*)main_fde);
}
void clean_sstart_all_creazione1reader(void)  {
	put_bloccante(main_fde->provider_buffer,POISON_PILL);
	pthread_join(main_fde_thread,NULL);
	wait_readers_termination(main_fde->current_readers,1);
	condvar_destroy(reader_handler_started);
	main_fde_destroy(main_fde);
}
void test_start_all_creazione1reader(void) {
	init_start_all_creazione1reader();

	//sollecitazione
	submit_request(main_fde->accepter_buffer,"Reader1");

	//verifica
	wait_flag_value(reader_handler_started,1);

	//test passato
	CU_PASS();
	clean_sstart_all_creazione1reader();
}





//Esecuzione intera di un flusso main con invio di 1 poison pill
//da parte di un provider e 0 readers in gioco
void init_start_all_1ppill0readers(void)  {
	msg_array = (msg_t**)malloc(sizeof(msg_t*)*2);
	msg_array[0] = POISON_PILL;
	main_fde = main_fde_init(msg_array,1);

}
void clean_sstart_all_1ppill0readers(void)  {
	free(msg_array);
	main_fde_destroy(main_fde);
}
void test_start_all_1ppill0readers(void) {
	init_start_all_1ppill0readers();

	start_main_fde(main_fde);

	//Tutto ok, test passato
	CU_PASS();
	clean_sstart_all_1ppill0readers();
}




//Esecuzione intera di un flusso main con invio di 1 poison pill
//da parte di un provider e 1 reader in gioco
void init_start_all_1ppill1readers(void)  {
	msg_array = (msg_t**)malloc(sizeof(msg_t*)*2);
	msg_array[0] = POISON_PILL;
	reader_handler_started = NULL;
	main_fde = main_fde_init(msg_array,1);
	submit_request(main_fde->accepter_buffer,"Reader1");

}
void clean_sstart_all_1ppill1readers(void)  {
	free(msg_array);
	main_fde_destroy(main_fde);
}
void test_start_all_1ppill1readers(void) {
	init_start_all_1ppill1readers();

	start_main_fde(main_fde);

	//Tutto ok, test passato
	CU_PASS();
	clean_sstart_all_1ppill1readers();
}



