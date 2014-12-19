/*
 * safe_list.h
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#ifndef MAIN_HW2_SRC_SAFE_LIST_H_
#define MAIN_HW2_SRC_SAFE_LIST_H_

#include "../hwc2list/list.h"
#include "assert.h"
#include "errno.h"

//soluzione molto conservativa: se io sto usando la lista per qualsiasi cosa,
//nessun altro la deve usare. La gestione dei lock sul mutex
//è responsabilità di chi la utilizza
typedef struct safe_list{
	list_t* list;
	pthread_mutex_t  uso_list;
	struct safe_list* (*init)(void);
	void (*destroy)(struct safe_list*);
} s_list;

s_list* safe_list_init(void) ;
void safe_list_destroy(s_list* ) ;
void safeAddElement(s_list* list, void* element);
int safeRemoveElement(s_list* list, void* element);
int lock_list(s_list* );
int unlock_list(s_list* );

#endif /* MAIN_HW2_SRC_SAFE_LIST_H_ */
