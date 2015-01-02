/*
 * dispatcher.c
 *
 *  Created on: Dec 16, 2014
 *      Author: ugo
 */

#include "dispatcher.h"

int copy_and_send_to_all(int is_poison_pill, iterator_t* it, msg_t* current) {
	int count=0;
	while (hasNext(it)) {
		count++;
		msg_t* to_send;
		if (!is_poison_pill)
			to_send = msg_copy_string(current);
		else
			to_send = msg_copy_pill(current);

		reader_fde* reader = (reader_fde*) next(it);
		put_non_bloccante(reader->reader_buffer, to_send);
	}
	return count;
}

void *start_dispatcher(void* args) {
	struct start_dispatcher_args *p = (struct start_dispatcher_args*) args;
	s_list* readers = p->current_readers;
	buffer_t* buffer = p->provider_buffer;
	msg_t* current;
	int is_poison_pill=0;
	do {
		current = get_bloccante(buffer);
		is_poison_pill = current == POISON_PILL;
		lock_list(readers);
		iterator_t* it = iterator_init(readers->list);
		copy_and_send_to_all(is_poison_pill, it, current);
		iterator_destroy(it);
		unlock_list(readers);
	} while(!is_poison_pill);
	printf("-Dispatcher poisoned-");
	return 0;
}
