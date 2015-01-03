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
	printf("Executing put bloccante");
	struct put_bloccante_params* p = (struct put_bloccante_params*)args;
	msg_t* ret_value = put_bloccante(p->buffer,p->message);
	free(args);
	return (void*)ret_value;

}

void send_detached_ppill(buffer_t* buffer) {
	struct put_bloccante_params *p = (struct put_bloccante_params*)malloc(sizeof(struct put_bloccante_params));
	p->buffer = buffer;
	p->message = msg_copy_pill(POISON_PILL);
	pthread_t reader;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&reader,&attr,wrapper_put_bloccante,p);
	pthread_attr_destroy (&attr);

}


int copy_and_send_to_all(s_list* reader_list, msg_t* current) {
	int slow_readers=0;
	iterator_t* it = iterator_init(reader_list->list);
	while (hasNext(it)) {
		msg_t* to_send;
		if (to_send == POISON_PILL)
			to_send = msg_copy_pill(current);
		else
			to_send = msg_copy_string(current);

		reader_fde* reader = (reader_fde*) next(it);
		if(put_non_bloccante(reader->reader_buffer, to_send) == BUFFER_ERROR) {
			//Siamo in presenza di un reader troppo lento, quindi mando una POISON_PILL
			//in maniera bloccante con un thread a parte. Contemporaneamente
			//rimuovo il reader dalla lista, altrimenti rimanderò ancora
			//poison pills
			printf("Trovato un reader lento");
			slow_readers++;
			send_detached_ppill(reader->reader_buffer);
			removeElement(reader_list->list,reader);
		}
	}
	iterator_destroy(it);
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
		slow_readers = copy_and_send_to_all(readers, current);
		unlock_list(readers);
	} while(!is_poison_pill);
	printf("-Dispatcher poisoned-");
	return slow_readers;
}
