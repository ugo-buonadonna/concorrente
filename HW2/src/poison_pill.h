/*
 * poison_pill.h
 *
 *  Created on: Dec 12, 2014
 *      Author: ugo
 */

/* poison_pill.h */

#ifndef POISON_PILL
#define POISON_PILL ((msg_t*)&POISON_PILL_messaggio)

#include "types.h"
#include <stdlib.h>

msg_t* messaggio_init_pill(void * );
void messaggio_destroy_pill(msg_t * );
msg_t* messaggio_copy_pill(msg_t* );

extern const msg_t POISON_PILL_messaggio;

#endif /* POISON_PILL */
