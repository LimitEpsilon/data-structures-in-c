#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HASH_TABLE_SIZE 1

typedef enum { Red, Black } color;

/* direction is hard-coded, please do not change the order of def */
typedef enum { Left = 0, Right, Root } direction;

typedef struct rb_node {
	struct rb_node *parent;
	struct rb_node *children[2];
	color color;
	char *key;
	void *value;
} rb_node;

extern void *delete(rb_node*);
extern rb_node *insert(char*, void*);
extern rb_node *find (char*);
extern void print_table(void);
extern void failwith(const char *msg);

#endif
