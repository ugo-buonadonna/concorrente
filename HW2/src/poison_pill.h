/*
 * poison_pill.h
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */

/* poison_pill.h */

#ifndef POISON_PILL_H
#define POISON_PILL_H

#include "../../HW1/src/types.h"
#include <stdlib.h>

extern const msg_t POISON_PILL_msg;
#define POISON_PILL ((msg_t*)&POISON_PILL_msg)



msg_t* messaggio_init_pill(void * );
void messaggio_destroy_pill(msg_t * );
msg_t* messaggio_copy_pill(msg_t* );



#endif /* POISON_PILL */
