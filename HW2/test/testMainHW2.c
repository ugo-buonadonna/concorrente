/*
 * testMainHW2.c
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */


#include "testMainHW2.h"


int init_suite_hwc2(void) {
}
int reinit_suite_hwc2(void) {

}
int clean_suite_hwc2(void) {

}

//Inizio test veri e propri
void test_putNonBloccanteBuff(void) {

}

const char* callTestsHWC2() {
	CU_pSuite hwc2_suite = NULL;
	CU_initialize_registry();
	hwc2_suite = CU_add_suite("hwc2 Testing", init_suite_hwc2,
			clean_suite_hwc2);

	CU_add_test(hwc2_suite, "Produzione in un buffer pieno",
			test_putNonBloccanteBuff);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error_msg();
}
