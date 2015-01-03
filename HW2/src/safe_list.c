/*
 * safe_list.c)
 *
 *  Created on: Dec 13, 2014
 *      Author: ugo
 */

#include "safe_list.h"

s_list* safe_list_init(void) {
	s_list* new_list = (s_list*)malloc(sizeof(s_list));
	new_list->list = list_init();
	if(pthread_mutex_init(&(new_list->uso_list),NULL) != 0)
		perror("Error inizializing safe_list mutex");
	new_list->init = safe_list_init;
	new_list->destroy = safe_list_destroy;
	return new_list;
}

void safe_list_destroy(s_list* list) {
	if(pthread_mutex_destroy(&(list->uso_list)))
		perror("Error while destroying mutex!");
	list_destroy(list->list);
	free(list);
}
int lock_list(s_list* list) {
	int res = pthread_mutex_lock(&(list->uso_list));
	if (res != 0)
		perror("Error while locking list:");
	return res;
}

int unlock_list(s_list* list) {
	int res = pthread_mutex_unlock(&(list->uso_list));
	if (res != 0)
		perror("Error while unlocking list: %d",errno);
	return res;
}

void safeAddElement(s_list* list, void* element) {
	assert(lock_list(list) == 0);
	addElement(list->list,element);
	if(unlock_list(list))
		perror("Error while unlocking safe list(add)");

}

int safeRemoveElement(s_list* list, void* element) {
	int res;
	res = lock_list(list);
	if (res != 0) {
			perror("Error while locking safe list(remove):");
			return res;
	}
	res = removeElement(list->list,element);
	if(unlock_list(list))
		perror("Error while unlocking safe list(remove)");
	return res;

}
