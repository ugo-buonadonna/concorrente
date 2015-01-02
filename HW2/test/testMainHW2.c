/*
 * testMainHW2.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */


#include "testMainHW2.h"

condvar_t *reader_handler_started;

int init_suite_hwc2(void) {
	reader_handler_started = NULL;
	return 0;
}

int clean_suite_hwc2(void) {
	return 0;
}


//Funzioni ausiliarie utilizzate nelle inizalizzazioni dei test
void wait_readers_termination(s_list* reader_list,int thread_number) {
	iterator_t *reader_list_iterator = iterator_init(reader_list->list);
	copy_and_send_to_all(1, reader_list_iterator, POISON_PILL);
	wait_flag_value(reader_handler_started, thread_number*2);
	iterator_destroy(reader_list_iterator);
}




const char* callTestsHWC2() {
	/*
	 CU_initialize_registry();

	CU_TestInfo suiteHWC2[]= {
			 { "Test della spedizione di 1 messaggio e 1 ppill da parte di un provider", test_providerUnMessaggio},
			 { "Test della spedizione di 2 messaggi e 1 ppill da parte di un provider", test_providerDueMessaggi},
			 { "Test funzione del reader ricevendo 1 messaggio e 1 ppill", test_readerUnMessaggio},
			 { "Test gestore del reader, creazione di 1 reader", test_unGestoreReader},
			 { "Test creazione di 1 reader da parte di 1 Accepter", test_accepterUnReader},
			 { "Test creazione di 2 reader da parte di 1 Accepter", test_accepterDueReader},
			 { "Test dispatching di 1 messaggio e 1 ppill ad 1 reader", test_dispacherUnMsgUnreader},
			 CU_TEST_INFO_NULL};

	 CU_SuiteInfo suites[] = {
			 {"Test Flussi HWC2", init_suite_hwc2, clean_suite_hwc2, suiteHWC2},
		        CU_SUITE_INFO_NULL
		    };

	 //INSPIEGABILE SEGMENTATION FAULT IN CU_register_suites
	 CU_ErrorCode error = CU_register_suites(suites);
	 if(error != CUE_SUCCESS){
	 	        printf("Errore con CUnit - codice %d\n", error);
	 	    }
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_ErrorCode tuttoOk = CU_basic_run_tests();
		    printf("CUnit error code-->%d\n", tuttoOk);
	CU_cleanup_registry();
	return CU_get_error_msg();
	*/
	CU_pSuite hwc2_suite = NULL;
	CU_initialize_registry();
	hwc2_suite = CU_add_suite("hwc2 Testing", init_suite_hwc2,
			clean_suite_hwc2);

	//PROVIDER
	CU_add_test(hwc2_suite, "spedizione di 1 messaggio da parte di un provider",
			test_providerUnMessaggio);
	CU_add_test(hwc2_suite, "spedizione di 2 messaggi da parte di un provider",
			test_providerDueMessaggi);

	//READER
	CU_add_test(hwc2_suite, "funzione core del reader ricevendo 1 messaggio",
			test_start_reader_1reader1Messaggio);

	//READER HANDLER
	CU_add_test(hwc2_suite, "funzione creatrice di reader, creazione di 1 reader",
				test_createReader_creazione1reader);
	CU_add_test(hwc2_suite, "funzione creatrice di reader, creazione di 2 reader",
				test_createReader_creazione2reader);
	CU_add_test(hwc2_suite, "gestore del reader, creazione di 1 reader",
			test_startReaderHandler_creazione1reader);
	CU_add_test(hwc2_suite, "gestore del reader, inserimento in lista di 1 reader",
			test_startReaderHandler_inserimentoLista1reader);
	CU_add_test(hwc2_suite, "gestore del reader, inserimento in lista di 2 reader",
			test_startReaderHandler_inserimentoLista2reader);

	//ACCEPTER
	CU_add_test(hwc2_suite, "creazione di 1 reader da parte di 1 Accepter",
			test_start_accepter_creazione1Reader);
	CU_add_test(hwc2_suite, "creazione di 2 reader da parte di 1 Accepter",
			test_start_accepter_creazione2Reader);
	CU_add_test(hwc2_suite, "creazione di 1 reader tramite submit_request",
			test_submit_request_1reader);
	CU_add_test(hwc2_suite, "creazione di 2 reader tramite submit_request",
			test_submit_request_2reader);

	//DISPATCHER
	CU_add_test(hwc2_suite, "dispatching di 1 messaggio ad 1 reader",
			test_start_dispatcher_1msg1reader);
	CU_add_test(hwc2_suite, "dispatching di 2 messaggi ad 1 reader",
				test_start_dispatcher_2msg1reader);
	CU_add_test(hwc2_suite, "dispatching di 2 messaggi ad 1 reader Lento",
			test_start_dispatcher_1readerLento_2messaggi);

	//FLUSSO PRINCIPALE
	CU_add_test(hwc2_suite, "invio poison pill all'accepter dopo terminazione provider",
			test_start_all_1ppillToAccepter);
	CU_add_test(hwc2_suite, "creazione di 1 reader a partire dal sistema di flussi",
			test_start_all_creazione1reader);
	CU_add_test(hwc2_suite, "esecuzione intera flusso main con 1 poison pill da provider e 0 readers",
			test_start_all_1ppill0readers);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
