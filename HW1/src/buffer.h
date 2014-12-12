#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_ERROR (msg_t *) NULL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <errno.h>
#include "msg.h"

typedef struct buffer {
	unsigned int size; // grandezza del buffer
	int T;	  // indice estrazione
	int D;	  // indice inserimento
	msg_t **msg_array;  //array con messaggi
	sem_t  vuote;		//semafori per gestione accessi concorrenti
	sem_t  piene;
	pthread_mutex_t  uso_t;
	pthread_mutex_t  uso_d;

} buffer_t;

buffer_t* buffer_init(unsigned int maxsize) ;

void buffer_destroy(buffer_t* buffer);

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);

int get_num_messaggi(buffer_t * );

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg) ;

msg_t* get_bloccante(buffer_t* buffer);

msg_t* get_non_bloccante(buffer_t* buffer);

#endif
