#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

typedef char * VAL_TYPE;

typedef struct node {
    VAL_TYPE value;
    struct node * next;
} node_t;


typedef struct list{
	node_t * head; //constant pointer
	node_t * cursor;
	node_t * tail;
	int size;
} list_t;

list_t * list_new(){
	list_t * tmp = (list_t*)malloc(sizeof(list_t));
	tmp->size = 0;
	tmp->head = tmp->cursor = tmp->tail = NULL;
	return tmp;
}
void list_push( list_t * list, VAL_TYPE value){
	node_t * tmp_node = (node_t*)malloc(sizeof(node_t));
	tmp_node->next = NULL;
	tmp_node->value = value;
	if(list->size == 0)
		list->head = list->tail = list->cursor = tmp_node;
	else{
		list->tail->next = tmp_node;
		list->tail = tmp_node;
	}
	list->size += 1;
}

VAL_TYPE list_peek(list_t * list){
	return list->tail->value;
}

int list_size(list_t * list){
	return list->size;
}

VAL_TYPE list_pop(list_t * list){
	if(list->size == 0)
		return NULL;

	VAL_TYPE value = list->tail->value;
	list->cursor = list->head;

	while(list->cursor->next != list->tail && list->cursor->next != NULL){
		list->cursor = list->cursor->next;
	}
	list->tail = list->cursor;
	free(list->tail->next);
	list->tail->next = NULL;
	list->size -= 1;
	return value;
}

void list_free(list_t * list){
	if(list->size > 0 ){
		list->cursor = list->head;
		while(list->cursor->next != NULL){
			list->tail = list->cursor;
			list->cursor = list->tail->next;
			free(list->tail);
		}
	}
	free(list);
}


#endif