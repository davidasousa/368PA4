#include "hbt.h"

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

Tnode* insert_to_tree(Tnode* new_node, int key);

int deletion(Tnode** head, Tnode* curr, Tnode* prev, int key);

#endif
