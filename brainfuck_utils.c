/// file: brainfuck_utils.c
/// description: supporting functions for the brainfuck language
/// @author Nicholas Jones dojoman19@gmail.com

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cells.h"

#define _UTILS_

char* loop_dat;
unsigned int left_bracket_count = 0;

void process_line(char* line, LinkedList* list){
	for(size_t i = 0; i < strlen(line); i++){
		char c = line[i];
		switch(c){
			case '<':
				list->current = list->current->prev;
				list->size--;
				break;
			case '>':
				if(list->current->next == NULL){
					resize_list(list);
				}
				list->current = list->current->next;
				list->size++;
				break;
			case '+':
				list->current->data++;
				break;
			case '-':
				list->current->data--;
				break;
			case '.':
				printf("%c", (char)list->current->data);
				break;
		}
		if(c == '['){
			char* nested_loop;
			int curr_i;
			if(loop_dat == NULL){
				loop_dat = calloc(strlen(line), sizeof(char));
				curr_i = i+1;
				left_bracket_count++;
				while(left_bracket_count != 0){
					if(line[curr_i] == ']')
						left_bracket_count--;
					if(line[curr_i] == '[')
						left_bracket_count++;
					if(!left_bracket_count)
						break;
					strncat(loop_dat, &line[curr_i], 1);
					curr_i++;
				}
				while(list->current->data){
					process_line(loop_dat, list);
				}
				loop_dat = NULL;
			}
			else{
				nested_loop = calloc(strlen(line), sizeof(char));
				curr_i = i+1;
				left_bracket_count++;
				while(left_bracket_count != 0){
					if(line[curr_i] == ']')
						left_bracket_count--;
					if(line[curr_i] == '[')
						left_bracket_count++;
					if(!left_bracket_count)
						break;
					strncat(nested_loop, &line[curr_i], 1);
					curr_i++;
				}
				while(list->current->data){
					process_line(nested_loop, list);
				}
			}
			i = curr_i;
		}
	}
}

void process_input(FILE* file, LinkedList* list){
	size_t len;
	char* line = NULL;
	printf("> ");
	while(getline(&line, &len, file) != -1){
		process_line(line, list);
		if(file == stdin){
			printf("\n");
			printf("> ");
		}
	}
	free(line);
}
