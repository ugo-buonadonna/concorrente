#include "testMain.h"

//variabile condizione per sincronizzare threads
condvar_t *thread_sync;

//variabili dei test
msg_t* expected_message;
msg_t* msg;
buffer_t *buffer_pieno;
buffer_t *buffer_vuoto;
int sem_value_vuote;
int sem_value_piene;

int init_suite_hwc1(void)
{
	expected_message = msg_init_string("Messaggio gia presente");
	msg = msg_init_string("Messaggio da aggiungere");
	buffer_pieno = buffer_init(1);
	buffer_pieno->D = 1;
	buffer_pieno->msg_array[0] = expected_message;
	//Se uno dei due non ritorna zero, l'inizializzazione fallisce
	if( (sem_wait(&(buffer_pieno->vuote))!=0) || (sem_post(&(buffer_pieno->piene))!=0))
		return -1;
	buffer_vuoto = buffer_init(1);
	thread_sync = condvar_init(0);
	return 0;
}


int clean_suite_hwc1(void)
{
	buffer_destroy(buffer_pieno);
	buffer_destroy(buffer_vuoto);
	msg_destroy_string(expected_message);
	msg_destroy_string(msg);
	sem_value_vuote = 0;
	sem_value_piene = 0;
	condvar_destroy(thread_sync);
	return 0;
}


int aspetta_flag(condvar_t * condvar, const int msec) {

	struct timespec ts;
	struct timeval tp;

	gettimeofday(&tp, NULL);

	ts.tv_sec = tp.tv_sec;
	ts.tv_nsec = tp.tv_usec * 1000;
	ts.tv_nsec += msec;
	return wait_flag_change(condvar, ts);
}

void test_putNonBloccanteBufferPieno(void) {
	//set up
	//Niente, ho gia il buffer pieno

	//sollecitazione
	msg_t* result = put_non_bloccante(buffer_pieno, msg);

	//verifica
	CU_ASSERT_EQUAL(result, BUFFER_ERROR);
	sem_getvalue(&(buffer_pieno->vuote), &sem_value_vuote);
	sem_getvalue(&(buffer_pieno->piene), &sem_value_piene);
	CU_ASSERT_EQUAL(sem_value_vuote, 0);
	CU_ASSERT_EQUAL(sem_value_piene, 1);
	CU_ASSERT_EQUAL(buffer_pieno->msg_array[0], expected_message);


}

void test_getBloccanteBufferInizialmenteVuoto(void) {
	//set up
	msg_t* messaggio_ritornato;
	pthread_t consumatore_bloccante;
	int res = pthread_create(&consumatore_bloccante, NULL, consume_blocking_signal_after,(buffer_t*) buffer_vuoto);
	CU_ASSERT_EQUAL_FATAL(res, 0);

	//sollecitazione
	/*Aspetto la segnalazione che il thread è partito e sta aspettando*/
	aspetta_flag(thread_sync, 3000);
	/*Ora deposito un messaggio*/
	put_non_bloccante(buffer_vuoto, expected_message);

	//verifica
	/* Ora dovrebbe aver cosumato..*/
	pthread_join(consumatore_bloccante, (msg_t *) &messaggio_ritornato);
	CU_ASSERT_EQUAL(expected_message, messaggio_ritornato);
	sem_getvalue(&(buffer_vuoto->piene), &sem_value_piene);
	CU_ASSERT_EQUAL(sem_value_piene, 0);
	sem_getvalue(&(buffer_vuoto->vuote), &sem_value_vuote);
	CU_ASSERT_EQUAL(sem_value_vuote, 1);

}

void test_primaConsumaPoiProduciBufferPieno(void) {
	//set up
	pthread_t consumatore_bloccante;
	msg_t* msg_prelevato;
	int res = pthread_create(&consumatore_bloccante, NULL, consume_blocking,(buffer_t*) buffer_pieno);
	CU_ASSERT_EQUAL_FATAL(res, 0);

	//sollecitazione
	/*Aspetto la segnalazione del consumatore*/
	aspetta_flag(thread_sync, 3000);
	/*Sono sicuro che il thread ha consumato,  ora produco */
	msg = produce_blocking(buffer_pieno,msg);

	//verifica
	sem_getvalue(&(buffer_vuoto->piene), &sem_value_piene);
	CU_ASSERT_EQUAL(sem_value_piene, 1);
	sem_getvalue(&(buffer_vuoto->vuote), &sem_value_vuote);
	CU_ASSERT_EQUAL(sem_value_vuote, 0);
	pthread_join(consumatore_bloccante, (msg_t *) &msg_prelevato);
	CU_ASSERT_EQUAL(msg_prelevato, msg);
}

const char* callTests() {
	CU_pSuite hwc1_suite = NULL;
	CU_initialize_registry();
	hwc1_suite = CU_add_suite("HWC1 Testing", init_suite_hwc1, clean_suite_hwc1);

	CU_add_test(hwc1_suite, "Produzione in un buffer pieno",
			test_putNonBloccanteBufferPieno);
	CU_add_test(hwc1_suite,
			"Consumazione in un buffer vuoto",
			test_getBloccanteBufferInizialmenteVuoto);
	CU_add_test(hwc1_suite,
				"Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore",
				test_primaConsumaPoiProduciBufferPieno);
//	CU_add_test(hwc1_suite,
//				"Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il produttore",
//				test_getBloccanteBufferInizialmenteVuoto);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
