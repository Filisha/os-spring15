#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include "command.h"

typedef command_t CMD_T;
typedef char * STRING;

typedef enum{
	COMMAND_LIST,
	STRING_LIST
}list_type;

typedef struct cmd_node {
    CMD_T value;
    struct cmd_node * next;
} cmd_node_t;


typedef struct cmd_list{
	cmd_node_t * head; //constant pointer
	cmd_node_t * cursor;
	cmd_node_t * tail;
	int size;
} cmd_list_t;

typedef struct str_node {
    char * value;
    struct str_node * next;
} str_node_t;


typedef struct str_list{
	str_node_t * head; //constant pointer
	str_node_t * cursor;
	str_node_t * tail;
	int size;
} str_list_t;

void * list_new(list_type type){
	if(type == COMMAND_LIST){
		cmd_list_t * tmp = (cmd_list_t*)malloc(sizeof(cmd_list_t));
		tmp->size = 0;
		tmp->head = tmp->cursor = tmp->tail = NULL;
		return tmp;
	}
	if(type == STRING_LIST){
		str_list_t * tmp = (str_list_t*)malloc(sizeof(str_list_t));
		tmp->size = 0;
		tmp->head = tmp->cursor = tmp->tail = NULL;
		return tmp;
	}
}
void cmd_list_push( cmd_list_t * list, CMD_T value){
	cmd_node_t * tmp_node = (cmd_node_t*)malloc(sizeof(cmd_node_t));
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

CMD_T cmd_list_peek(cmd_list_t * list){
	return list->tail->value;
}

int list_size(cmd_list_t * list){
	return list->size;
}

CMD_T list_pop(cmd_list_t * list){
	if(list->size == 0)
		return NULL;

	CMD_T value = list->tail->value;
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

void list_free(cmd_list_t * list){
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