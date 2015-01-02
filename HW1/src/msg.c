/*
 * msg.c
 *
 *  Created on: Nov 19, 2014
 *      Author: ugo
 */
#include "msg.h"

msg_t* msg_init_string(void* content) {
	//viene creata una copia "privata" della stringa
	msg_t* new_msg = (msg_t*)malloc( sizeof(msg_t) );
	char* string = (char*)content;
	char* new_content = (char*)malloc(strlen(string)+1); // +1 per \0 finale
	strcpy(new_content, string);
	new_msg->content = new_content;
	new_msg->msg_init = msg_init_string;
	new_msg->msg_destroy = msg_destroy_string;
	new_msg->msg_copy = msg_copy_string;
	return new_msg;
}
void msg_destroy_string(msg_t* msg) {
	free(msg->content); // free copia privata
	free(msg); // free struct
}

msg_t* msg_copy_string(msg_t* msg) {
	return msg->msg_init(msg_init_string( msg->content ));
}

//funzioni per inizializzare e rilasciare un array di messaggi predefiniti
msg_t** init_msgs(int num) {
	msg_t** msgs = (msg_t**) malloc(sizeof(msg_t*) * num);
	char msg_content[15];
	int i;
	for(i=0;i<num;i++) {
		sprintf(msg_content, "Messaggio%d", i);
		msgs[i] = msg_init_string(msg_content);
	}
	return msgs;
}
void free_msgs(msg_t** msgs,int num) {
	int i;
	for(i=0;i<num;i++)
		msg_destroy_string(msgs[i]);
	free(msgs);
}
