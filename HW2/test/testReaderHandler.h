/*
 * testReaderHandler.h
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_TEST_TESTREADERHANDLER_H_
#define MAIN_HW2_TEST_TESTREADERHANDLER_H_


#include "../../HW1/src/types.h"
#include "../src/types.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

//Variabile condizione usata per assicurarsi dell'avvio di un reader_handler.
//Insieme a un flag per utilizzarla o no (la uso solo quando testo specifici casi)
extern condvar_t *reader_handler_started;

void test_startReaderHandler_creazione1reader(void);
void test_startReaderHandler_creazione2reader(void);
void test_startReaderHandler_inserimentoLista1reader(void);
void test_startReaderHandler_inserimentoLista2reader(void);
void test_createReader_creazione1reader(void);
void test_createReader_creazione2reader(void);

#endif /* MAIN_HW2_TEST_TESTREADERHANDLER_H_ */
