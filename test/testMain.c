#include "testMain.h"

//variabile condizione per sincronizzare threads
condvar_t *thread_sync;

//variabili dei test
pthread_t thread_fun;
pthread_t *producers;
pthread_t *consumers;
buffer_t *buffer_unitario_pieno;
buffer_t *buffer_unitario_vuoto;
buffer_t *buffer_multi_pieno;
buffer_t *buffer_multi_vuoto;

msg_t *msg_in_buffer;
msg_t *inserted_in_buffer;
msg_t *retrieved_from_buffer;
msg_t *retrieved_from_thread;
msg_t **msgs_to_insert;
msg_t **msgs_retrieved;
void ** arguments;

const int dim_buffer = 4;
const int num_messaggi_disponibili = 20;
const int thread_disponibili = 20;

int init_suite_hwc1(void) {

	int i = 0;
	char msg_content[13];

	inserted_in_buffer = msg_init_string("Messaggio inserito in buffer");
	msg_in_buffer = msg_init_string("Messaggio gia presente");
	buffer_unitario_pieno = buffer_init(1);
	buffer_unitario_pieno->D = 1;
	buffer_unitario_pieno->msg_array[0] = msg_in_buffer;
	//Se uno dei due non ritorna zero, l'inizializzazione fallisce
	if ((sem_wait(&(buffer_unitario_pieno->vuote)) != 0)
			|| (sem_post(&(buffer_unitario_pieno->piene)) != 0))
		return -1;
	buffer_unitario_vuoto = buffer_init(1);

	buffer_multi_pieno = buffer_init(dim_buffer);
	for (i = 0; i < dim_buffer; i++) {
		sprintf(msg_content, "M%d", i);
		put_non_bloccante(buffer_multi_pieno, msg_init_string(msg_content));
	}
	//buffer_multi_pieno->D = dim_buffer;
	buffer_multi_vuoto = buffer_init(dim_buffer);

	msgs_to_insert = (msg_t**) malloc(
			sizeof(msg_t*) * num_messaggi_disponibili);
	for (i = 0; i < num_messaggi_disponibili; i++) {
		sprintf(msg_content, "Messaggio%d", i);
		msgs_to_insert[i] = msg_init_string(msg_content);
	}

	msgs_retrieved = (msg_t**) malloc(
			sizeof(msg_t*) * num_messaggi_disponibili);

	producers = (pthread_t *) malloc(sizeof(pthread_t) * thread_disponibili);

	consumers = (pthread_t *) malloc(sizeof(pthread_t) * thread_disponibili);

	arguments = (void **) malloc(sizeof(void*) * 2);

	thread_sync = condvar_init(0);
	return 0;

	/*
	 sem_getvalue(&(buffer_pieno->vuote), &sem_value_vuote);
	 sem_getvalue(&(buffer_pieno->piene), &sem_value_piene);
	 CU_ASSERT_EQUAL(sem_value_vuote, 0);
	 CU_ASSERT_EQUAL(sem_value_piene, 1);*/
}
int reinit_suite_hwc1(void) {
	clean_suite_hwc1();
	init_suite_hwc1();
}
int clean_suite_hwc1(void) {
	/*buffer_destroy(buffer_unitario_pieno);
	buffer_destroy(buffer_unitario_vuoto);
	//buffer_destroy(buffer_multi_pieno);
	free(msgs_to_insert);
	free(msgs_retrieved);
	free(consumers);
	free(producers);
	free(arguments);*/

	condvar_destroy(thread_sync);
	return 0;
}

//Inizio test veri e propri
void test_putNonBloccanteBufferPieno(void) {
	init_suite_hwc1();
	//sollecitazione
	retrieved_from_buffer = put_non_bloccante(buffer_unitario_pieno,
			inserted_in_buffer);

	//verifica
	CU_ASSERT_EQUAL(retrieved_from_buffer, BUFFER_ERROR);
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_pieno), 1);
	CU_ASSERT_EQUAL(buffer_unitario_pieno->msg_array[0], msg_in_buffer);

}

void test_getBloccanteBufferInizialmenteVuoto(void) {
	init_suite_hwc1();
	//set up
	pthread_create(&thread_fun, NULL, consume_blocking_and_signal_before,
			(buffer_t*) buffer_unitario_vuoto);

	//VIOLAZIONE DELL'ASSUNZIONE DI PROGRESSO FINITO
	//Forzo una seq. di interleaving tale da mandare in blocco il thread consumatore
	sleep(2);
	put_non_bloccante(buffer_unitario_vuoto, inserted_in_buffer);
	wait_flag_change(thread_sync);

	//verifica
	pthread_join(thread_fun, (msg_t *) &retrieved_from_thread);
	CU_ASSERT_EQUAL(inserted_in_buffer, retrieved_from_thread);
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_vuoto), 0);
}

void test_primaConsumaPoiProduciBufferPieno(void) {
	init_suite_hwc1();
	//set up
	pthread_create(&thread_fun, NULL, consume_blocking_and_signal_after,
			(buffer_t*) buffer_unitario_pieno);

	//produco solo dopo la consumazione
	wait_flag_change(thread_sync);
	put_bloccante(buffer_unitario_pieno, inserted_in_buffer);

	//verifica
	pthread_join(thread_fun, (msg_t *) &retrieved_from_thread);
	CU_ASSERT_EQUAL(retrieved_from_thread, msg_in_buffer);
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_pieno), 1);
}

void test_primaProduciPoiConsumaBufferPieno(void) {
	init_suite_hwc1();
	//set up
	pthread_create(&thread_fun, NULL, consume_non_blocking_wait_signal,
			(buffer_t*) buffer_unitario_pieno);

	//consumo solo dopo la produzione
	inserted_in_buffer = put_non_bloccante(buffer_unitario_pieno,
			inserted_in_buffer);
	set_flag(thread_sync, 1);

	//verifica
	pthread_join(thread_fun, (msg_t *) &retrieved_from_thread);
	CU_ASSERT_EQUAL(retrieved_from_thread, msg_in_buffer);
	CU_ASSERT_EQUAL(inserted_in_buffer, BUFFER_ERROR)
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_pieno), 0);
}

void test_dueProduzioniBufferVuoto(void) {
	//set up
	init_suite_hwc1();
	arguments[0] = (buffer_t*) buffer_unitario_vuoto;
	arguments[1] = (msg_t*) msgs_to_insert[0];
	pthread_create(&thread_fun, NULL, produce_non_blocking, arguments);

	//sollecitazione
	inserted_in_buffer = put_non_bloccante(buffer_unitario_vuoto,
			msgs_to_insert[1]);
	pthread_join(thread_fun, (msg_t *) &retrieved_from_thread);

	//verifica
	CU_ASSERT_TRUE(
			(inserted_in_buffer==BUFFER_ERROR) || (retrieved_from_thread==BUFFER_ERROR));
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_vuoto), 1);
}

void test_dueConsumazioniBufferPieno(void) {
	init_suite_hwc1();
	//set up
	pthread_create(&thread_fun, NULL, consume_non_blocking_and_signal_after,
			(buffer_t*) buffer_unitario_pieno);

	//sollecitazione
	retrieved_from_buffer = get_non_bloccante(buffer_unitario_pieno);
	pthread_join(thread_fun, (msg_t *) &retrieved_from_thread);

	//verifica
	CU_ASSERT_TRUE(
			(retrieved_from_buffer==BUFFER_ERROR) || (retrieved_from_thread==BUFFER_ERROR));
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_unitario_pieno), 0);
}

void test_piuProduzioniNonDiventaPieno(void) {
	init_suite_hwc1();
	//set up
	const int num_messaggi = 3;
	int i;
	arguments[0] = buffer_multi_vuoto;
	//sollecitazione
	for (i = 0; i < num_messaggi; i++) {
		arguments[1] = msgs_to_insert[i];
		pthread_create(&producers[i], NULL, produce_non_blocking, arguments);
	}
	for (i = 0; i < num_messaggi; i++)
		pthread_join(producers[i], NULL);
	//verifica
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_multi_vuoto), num_messaggi);
}

void test_piuProduzioniDiventaPieno(void) {
	init_suite_hwc1();
	const int num_messaggi = 5;
	//set up
	arguments[0] = (buffer_t*) buffer_multi_vuoto;
	int i = 0;

	//sollecitazione
	for (i = 0; i < num_messaggi; i++) {
		arguments[1] = msgs_to_insert[i];
		pthread_create(&producers[i], NULL, produce_non_blocking, arguments);
	}
	for (i = 0; i < num_messaggi; i++)
		pthread_join(producers[i], NULL);

	//verifica
	CU_ASSERT_EQUAL(get_num_messaggi(buffer_multi_vuoto),dim_buffer);
}

void test_piuConsumazioniPiuProduzioni(void) {
	init_suite_hwc1();
	const int produzioni = 5;
	const int consumazioni = 4;
	//set up
	arguments[0] = (buffer_t*) buffer_multi_vuoto;
	int i;

		//sollecitazione (produzioni > consumazioni)
		for (i = 0; i < produzioni; i++) {
			arguments[1] = msgs_to_insert[i];
			pthread_create(&producers[i], NULL, produce_non_blocking, arguments);
			if(i<=consumazioni)
				pthread_create(&consumers[i], NULL, consume_non_blocking_and_signal_after, arguments);
		}
		for (i = 0; i < produzioni; i++)
			pthread_join(producers[i], NULL);
		for (i = 0; i <= consumazioni; i++)
			pthread_join(consumers[i], NULL);

		//verifica
		printf("\n%d,%d\n",get_num_messaggi(buffer_multi_vuoto),produzioni - consumazioni);
		CU_ASSERT_EQUAL(get_num_messaggi(buffer_multi_vuoto),produzioni - consumazioni);
}

const char* callTests() {
	CU_pSuite hwc1_suite = NULL;
	CU_initialize_registry();
	hwc1_suite = CU_add_suite("HWC1 Testing", init_suite_hwc1,
			clean_suite_hwc1);

	CU_add_test(hwc1_suite, "Produzione in un buffer pieno",
			test_putNonBloccanteBufferPieno);
	CU_add_test(hwc1_suite,
			"Consumazione bloccante in un buffer inizialmente vuoto",
			test_getBloccanteBufferInizialmenteVuoto);
	CU_add_test(hwc1_suite,
			"Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore",
			test_primaConsumaPoiProduciBufferPieno);
	CU_add_test(hwc1_suite,
			"Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il produttore",
			test_primaProduciPoiConsumaBufferPieno);
	CU_add_test(hwc1_suite,
			"Produzione concorrente di molteplici messaggi in un buffer unitario vuoto",
			test_dueProduzioniBufferVuoto);
	CU_add_test(hwc1_suite,
			"Consumazione concorrente di molteplici messaggi in un buffer unitario pieno",
			test_dueConsumazioniBufferPieno);
	CU_add_test(hwc1_suite,
			"Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempie",
			test_piuProduzioniNonDiventaPieno);
	CU_add_test(hwc1_suite,
			"Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso",
			test_piuProduzioniDiventaPieno);
	CU_add_test(hwc1_suite,
	 "Consumazioni e produzioni concorrenti in un buffer",
	 test_piuConsumazioniPiuProduzioni);
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
