/*
 * reader_handler.h
 *
 *  Created on: Dec 16, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_READER_HANDLER_H_
#define MAIN_HW2_SRC_READER_HANDLER_H_

#include "types.h"

//Variabile condizione usata per assicurarsi dell'avvio di un reader_handler.
//Insieme a un flag per utilizzarla o no (la uso solo quando testo specifici casi)
extern condvar_t* reader_handler_started;

void create_reader(void* args);
int start_reader_handler(void* args);

#endif /* MAIN_HW2_SRC_READER_HANDLER_H_ */
