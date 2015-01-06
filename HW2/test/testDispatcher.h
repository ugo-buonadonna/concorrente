/*
 * testDispatcher.h
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_TEST_TESTDISPATCHER_H_
#define MAIN_HW2_TEST_TESTDISPATCHER_H_

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../../HW1/src/types.h"
#include "../src/types.h"

extern condvar_t *reader_handler_started;

void test_start_dispatcher_1msg1reader(void);
void test_start_dispatcher_2msg1reader(void);
void test_start_dispatcher_2msg2reader(void);
void test_start_dispatcher_1readerLento_2messaggi(void);

#endif /* MAIN_HW2_TEST_TESTDISPATCHER_H_ */
