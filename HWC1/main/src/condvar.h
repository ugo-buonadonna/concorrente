/*
 * varcond.h
 *
 *  Created on: Nov 21, 2014
 *      Author: ugo
 */

#ifndef MAIN_SRC_CONDVAR_H_
#define MAIN_SRC_CONDVAR_H_

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



typedef struct condition_variable {
	int flag;
	pthread_cond_t flag_cv;
	pthread_mutex_t flag_mutex;
} condvar_t ;


condvar_t * condvar_init(int ) ;

void set_flag (condvar_t * ,int );

void condvar_destroy(condvar_t * );

int wait_flag_change(condvar_t * ,const struct timespec );

#endif /* MAIN_SRC_CONDVAR_H_ */
