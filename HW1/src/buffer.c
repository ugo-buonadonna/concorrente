#include "buffer.h"

buffer_t* buffer_init(unsigned int maxsize) {
	buffer_t* new_buffer = (buffer_t*) malloc(sizeof(buffer_t));
	new_buffer->size = maxsize;
	new_buffer->T = 0;
	new_buffer->D = 0;
	new_buffer->msg_array = (msg_t**) malloc(sizeof(msg_t*) * maxsize);
	assert(sem_init(&(new_buffer->vuote), 0, maxsize) != -1);
	assert(sem_init(&(new_buffer->piene), 0, 0) != -1);
	assert(pthread_mutex_init(&(new_buffer->uso_t),NULL) == 0);
	assert(pthread_mutex_init(&(new_buffer->uso_d),NULL) == 0);
	return new_buffer;
}

void buffer_destroy(buffer_t* buffer) {
	int i;
	for(i=0;i<buffer->size;i++)
		if(buffer->msg_array[i])
			msg_destroy_string(buffer->msg_array[i]);
	free(buffer->msg_array);
	sem_destroy(&(buffer->vuote));
	sem_destroy(&(buffer->piene));
	pthread_mutex_destroy(&(buffer->uso_t));
	pthread_mutex_destroy(&(buffer->uso_d));
	free(buffer);
}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg) {
	assert(msg != NULL);
	sem_wait(&(buffer->vuote));
	pthread_mutex_lock(&(buffer->uso_d));
	buffer->msg_array[(buffer->D) % (buffer->size)] = msg;
	(buffer->D)++;
	pthread_mutex_unlock(&(buffer->uso_d));
	sem_post(&(buffer->piene));
	return msg;
}

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg) {
	assert(msg != NULL);
	if (sem_trywait(&(buffer->vuote)) == -1) {
		//printf(strerror(errno));
		return BUFFER_ERROR ;
	}
	pthread_mutex_lock(&(buffer->uso_d));
	buffer->msg_array[(buffer->D) % (buffer->size)] = msg;
	(buffer->D)++;
	pthread_mutex_unlock(&(buffer->uso_d));
	assert(sem_post(&(buffer->piene)) == 0);
	return msg;

}
msg_t* get_bloccante(buffer_t* buffer) {

	msg_t* message;
	sem_wait(&(buffer->piene));
	pthread_mutex_lock(&(buffer->uso_t));
	message = buffer->msg_array[(buffer->T) % (buffer->size)];
	(buffer->T)++;
	pthread_mutex_unlock(&(buffer->uso_t));
	sem_post(&(buffer->vuote));
	return message;
}

msg_t* get_non_bloccante(buffer_t* buffer) {
	msg_t* message;
	if (sem_trywait(&(buffer->piene)) == -1) {
		//printf(strerror(errno));
		return BUFFER_ERROR ;
	}
	pthread_mutex_lock(&(buffer->uso_t));
	message = buffer->msg_array[(buffer->T) % (buffer->size)];
	(buffer->T)++;
	pthread_mutex_unlock(&(buffer->uso_t));
	sem_post(&(buffer->vuote));
	return message;
}

int get_num_messaggi(buffer_t * buffer)
{
	int num;
	if( pthread_mutex_trylock(&(buffer->uso_t)) || pthread_mutex_trylock(&(buffer->uso_d))) {
			printf("Cannot acquire mutexes in get_num_messaggi");
			return errno;
	}
	num=buffer->D-buffer->T;
	pthread_mutex_unlock(&(buffer->uso_d));
	pthread_mutex_unlock(&(buffer->uso_t));
	return num;
}
