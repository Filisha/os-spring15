#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include "command.h"

/*************
command list
**************/

typedef struct cmd_node {
    command_t cmd;
    struct cmd_node * next;
} * cmd_node_t;


typedef struct cmd_list{
	cmd_node_t head; //constant pointer
	cmd_node_t cursor;
	cmd_node_t tail;
	int size;
} * cmd_list_t;

cmd_list_t cmd_list_new(){
	cmd_list_t tmp = (cmd_list_t)malloc(sizeof(cmd_list_t));
	tmp->size = 0;
	tmp->head = tmp->cursor = tmp->tail = NULL;
	return tmp;
}

void cmd_list_push(cmd_list_t list, command_t cmd){
	cmd_node_t tmp_node =  (cmd_node_t)malloc(sizeof(cmd_node_t));
	tmp_node->next = NULL;
	tmp_node->cmd = cmd;
	if(list->size == 0)
		list->head = list->tail = list->cursor = tmp_node;
	else{
		list->tail->next = tmp_node;
		list->tail = tmp_node;
	}
	list->size += 1;
}

command_t cmd_list_peek(cmd_list_t list){
	return list->tail->cmd;
}

int cmd_list_size(cmd_list_t list){
	return list->size;
}


command_t cmd_list_pop(cmd_list_t list){
	if(list->size == 0)
		return NULL;

	command_t cmd = list->tail->cmd;
	list->cursor = list->head;

	while(list->cursor->next != list->tail && list->cursor->next != NULL){
		list->cursor = list->cursor->next;
	}
	list->tail = list->cursor;
	free(list->tail->next);
	list->tail->next = NULL;
	list->size -= 1;
	return cmd;
}

bool cmd_list_isempty(cmd_list_t list){
	return list->size == 0;
}

void cmd_list_free(cmd_list_t list){
	;
}

/*************
string list
**************/

typedef char * STRING;

typedef struct str_node {
    STRING value;
    struct str_node * next;
} * str_node_t;


typedef struct str_list{
	str_node_t head; //constant pointer
	str_node_t cursor;
	str_node_t tail;
	int size;
} * str_list_t;

str_list_t str_list_new(){
	str_list_t tmp = (str_list_t)malloc(sizeof(str_list_t));
	tmp->size = 0;
	tmp->head = tmp->cursor = tmp->tail = NULL;
	return tmp;
}

void str_list_push(str_list_t list, STRING str){
	str_node_t tmp_node =  (str_node_t)malloc(sizeof(str_node_t));
	tmp_node->next = NULL;
	tmp_node->value = str;
	if(list->size == 0)
		list->head = list->tail = list->cursor = tmp_node;
	else{
		list->tail->next = tmp_node;
		list->tail = tmp_node;
	}
	list->size += 1;
}

STRING str_list_peek(str_list_t list){
	return list->tail->value;
}
int str_list_size(str_list_t list){
	return list->size;
}
STRING str_list_pop(str_list_t list){
	if(list->size == 0)
		return NULL;

	STRING value = list->tail->value;
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

void str_list_free(str_list_t list){
	;
	/*if(list->size > 0 ){
		list->cursor = list->head;
		while(list->cursor->next != NULL){
			list->tail = list->cursor;
			list->cursor = list->tail->next;
			free(list->tail);
		}
	}
	free(list);*/
}


#endif