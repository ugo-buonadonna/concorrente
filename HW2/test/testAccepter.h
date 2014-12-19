/*
 * testAccepter.h
 *
 *  Created on: Dec 17, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_TEST_TESTACCEPTER_H_
#define MAIN_HW2_TEST_TESTACCEPTER_H_

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../../HW1/src/types.h"
#include "../src/types.h"

//Variabile condizione usata per assicurarsi dell'avvio di un reader_handler.
//Insieme a un flag per utilizzarla o no (la uso solo quando testo specifici casi)
extern condvar_t *reader_handler_started;

void test_start_accepter_creazione1Reader(void);
void test_start_accepter_creazione2Reader(void) ;
#endif /* MAIN_HW2_TEST_TESTACCEPTER_H_ */
