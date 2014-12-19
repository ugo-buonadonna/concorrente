/*
 * varcond.c
 *
 *  Created on: Nov 21, 2014
 *      Author: ugo
 */

#include "condvar.h"

condvar_t * condvar_init(int flag) {
	condvar_t* new_condvar = (condvar_t*) malloc(sizeof(condvar_t));


	// Inizializza mutex associato a variabile condizione
	assert( pthread_mutex_init(&(new_condvar->flag_mutex), NULL) == 0);

	// Inizializza variabile condizione associata a flag
	assert(pthread_cond_init(&(new_condvar->flag_cv), NULL) == 0);

	// Inizializza flag
	new_condvar->flag = flag;
	return new_condvar;
}

void condvar_destroy(condvar_t * cond) {
	pthread_mutex_destroy(&(cond->flag_mutex));
	pthread_cond_destroy(&(cond->flag_cv));
	free(cond);
}

void set_flag (condvar_t * condvar,int flag_value) {
	// Lock del mutex su flag
	int res;
	//printf("set_flag tid: %d",pthread_self());
	if( (res = pthread_mutex_trylock (&(condvar->flag_mutex))) != 0)
		printf("Unable to lock mutex in set_flag: %d",res);

	// cambia il valore del flag
	condvar->flag = flag_value;

	// segnala a chi è in attesa che
	// il valore di flag è cambiato
	assert( pthread_cond_signal (&(condvar->flag_cv)) ==0);

	// unlock del mutex
	assert( pthread_mutex_unlock (&(condvar->flag_mutex)) ==0);
}

void inc_flag (condvar_t * condvar) {
	// Lock del mutex su flag
	int res;
	//printf("set_flag tid: %d",pthread_self());
	if( (res = pthread_mutex_trylock (&(condvar->flag_mutex))) != 0)
		printf("Unable to lock mutex in set_flag: %d",res);

	// cambia il valore del flag
	condvar->flag++;

	// segnala a chi è in attesa che
	// il valore di flag è cambiato
	assert( pthread_cond_signal (&(condvar->flag_cv)) ==0);

	// unlock del mutex
	assert( pthread_mutex_unlock (&(condvar->flag_mutex)) ==0);
}

int wait_flag_change(condvar_t * condvar) {
	int res;

	//printf("wait_flag_change tid: %d",pthread_self());
	pthread_mutex_lock(&(condvar->flag_mutex));
	while (!(condvar->flag))
		res = pthread_cond_wait(&(condvar->flag_cv), &(condvar->flag_mutex));
	pthread_mutex_unlock(&(condvar->flag_mutex));
	return res;
}

int wait_flag_value(condvar_t * condvar,int value) {
	int res;

	//printf("wait_flag_change tid: %d",pthread_self());
	pthread_mutex_lock(&(condvar->flag_mutex));
	while (condvar->flag != value)
		res = pthread_cond_wait(&(condvar->flag_cv), &(condvar->flag_mutex));
	pthread_mutex_unlock(&(condvar->flag_mutex));
	return res;
}



