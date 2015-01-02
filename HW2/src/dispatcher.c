/*
 * dispatcher.c
 *
 *  Created on: Dec 16, 2014
 *      Author: ugo
 */

#include "dispatcher.h"



//Funzioni per creare un thread detached che invia
//in modo bloccante POISON_PILL a reader giudicati lenti,
//in modo da non influenzare la velocità del dispatcher
void* wrapper_put_bloccante(void* args){
	struct get_bloccante_params* p = (struct get_bloccante_params*)args;
	return (void*)put_bloccante(p->buffer,p->message);
}

void send_detached_ppill(struct get_bloccante_params* arg) {
	pthread_t reader;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&reader,&attr,wrapper_put_bloccante,arg);
	pthread_attr_destroy (&attr);
}

int copy_and_send_to_all(int is_poison_pill, iterator_t* it, msg_t* current) {
	int slow_readers=0;
	while (hasNext(it)) {
		msg_t* to_send;
		if (!is_poison_pill)
			to_send = msg_copy_string(current);
		else
			to_send = msg_copy_pill(current);

		reader_fde* reader = (reader_fde*) next(it);
		if(put_non_bloccante(reader->reader_buffer, to_send) == BUFFER_ERROR) {
			//Siamo in presenza di un reader troppo lento, quindi mando una POISON_PILL
			//in maniera non bloccante
			printf("WEEEEEEEE");
			slow_readers++;
			struct get_bloccante_params* p;
			p->buffer = reader->reader_buffer;
			p->message = POISON_PILL;
			send_detached_ppill(&p);
		}
	}
	return slow_readers;
}

void *start_dispatcher(void* args) {
	struct start_dispatcher_args *p = (struct start_dispatcher_args*) args;
	s_list* readers = p->current_readers;
	buffer_t* buffer = p->provider_buffer;
	msg_t* current;
	int is_poison_pill=0;
	int slow_readers;
	do {
		current = get_bloccante(buffer);
		is_poison_pill = current == POISON_PILL;
		lock_list(readers);
		iterator_t* it = iterator_init(readers->list);
		slow_readers = copy_and_send_to_all(is_poison_pill, it, current);
		iterator_destroy(it);
		unlock_list(readers);
	} while(!is_poison_pill);
	printf("-Dispatcher poisoned-");
	return slow_readers;
}
