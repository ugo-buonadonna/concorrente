#include "testMain.h"

condvar_t *thread_sync;

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
	msg_t* expected_message = msg_init_string("Messaggio gia presente");
	msg_t* msg = msg_init_string("Messaggio da aggiungere");
	buffer_t *buffer_pieno = buffer_init(1);

	buffer_pieno->D = 1;
	buffer_pieno->msg_array[0] = expected_message;
	sem_wait(&(buffer_pieno->vuote));
	sem_post(&(buffer_pieno->piene));
	int vuote;
	int piene;

	//sollecitazione
	msg_t* result = put_non_bloccante(buffer_pieno, msg);

	//verifica
	CU_ASSERT_EQUAL(result, BUFFER_ERROR);
	sem_getvalue(&(buffer_pieno->vuote), &vuote);
	sem_getvalue(&(buffer_pieno->piene), &piene);
	CU_ASSERT_EQUAL(vuote, 0);
	CU_ASSERT_EQUAL(piene, 1);
	CU_ASSERT_EQUAL(buffer_pieno->msg_array[0], expected_message);

	buffer_destroy(buffer_pieno);
	msg_destroy_string(expected_message);
	msg_destroy_string(msg);
}

void test_getBloccanteBufferInizialmenteVuoto(void) {
	//set up
	msg_t* go_msg = msg_init_string("Messaggio da aggiungere");
	msg_t* messaggio_ritornato;
	int piene;
	int vuote;
	buffer_t *buffer_vuoto = buffer_init(1);
	pthread_t consumatore_bloccante;

	thread_sync = condvar_init(0);
	int res = pthread_create(&consumatore_bloccante, NULL, prelevaBloccante,(buffer_t*) buffer_vuoto);
	CU_ASSERT_EQUAL_FATAL(res, 0);

	//sollecitazione
	/*Aspetto la segnalazione che il thread è partito e sta aspettando*/
	aspetta_flag(thread_sync, 3000);
	/*Ora deposito un messaggio*/
	put_non_bloccante(buffer_vuoto, go_msg);
	printf("\nDepositato");

	//verifica
	/* Ora dovrebbe aver cosumato..*/
	pthread_join(consumatore_bloccante, (msg_t *) &messaggio_ritornato);
	CU_ASSERT_EQUAL(go_msg, messaggio_ritornato);
	sem_getvalue(&(buffer_vuoto->piene), &piene);
	CU_ASSERT_EQUAL(piene, 0);
	sem_getvalue(&(buffer_vuoto->vuote), &vuote);
	CU_ASSERT_EQUAL(vuote, 1);

	condvar_destroy(thread_sync);
	buffer_destroy(buffer_vuoto);
	msg_destroy_string(go_msg);
	//msg_destroy_string(messaggio_ritornato);
}

const char* callTests() {
	CU_pSuite hwc1_suite = NULL;
	CU_initialize_registry();
	hwc1_suite = CU_add_suite("HWC1 Testing", NULL, NULL);

	CU_add_test(hwc1_suite, "test di putNonBloccante con buffer pieno",
			test_putNonBloccanteBufferPieno);
	CU_add_test(hwc1_suite,
			"test di un consumatore bloccante con buffer inizialmente vuoto",
			test_getBloccanteBufferInizialmenteVuoto);
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
