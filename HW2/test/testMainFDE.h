/*
 * testMainFDE.h
 *
 *  Created on: Dec 28, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_TEST_TESTMAINFDE_H_
#define MAIN_HW2_TEST_TESTMAINFDE_H_

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../../HW1/src/types.h"
#include "../src/types.h"

extern condvar_t *reader_handler_started;

void test_start_all_1ppill0readers(void);
void test_start_all_1ppill1readers(void);
void test_start_all_creazione1reader(void);
void test_start_all_1ppillToAccepter(void);

#endif /* MAIN_HW2_TEST_TESTMAINFDE_H_ */
