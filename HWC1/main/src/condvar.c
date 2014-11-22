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
	pthread_mutex_init(&(new_condvar->flag_mutex), NULL);

	// Inizializza variabile condizione associata a flag
	pthread_cond_init(&(new_condvar->flag_cv), NULL);

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
		printf("%d",res);

	// cambia il valore del flag
	condvar->flag = flag_value;

	// segnala a chi è in attesa che
	// il valore di flag è cambiato
	assert( pthread_cond_signal (&(condvar->flag_cv)) ==0);

	// unlock del mutex
	assert( pthread_mutex_unlock (&(condvar->flag_mutex)) ==0);
}

int wait_flag_change(condvar_t * condvar,const struct timespec abstime) {
	int res;

	//printf("wait_flag_change tid: %d",pthread_self());
	pthread_mutex_lock(&(condvar->flag_mutex));
	while (!(condvar->flag))
		res = pthread_cond_timedwait(&(condvar->flag_cv), &(condvar->flag_mutex),&abstime);
	pthread_mutex_unlock(&(condvar->flag_mutex));
	return res;
}



