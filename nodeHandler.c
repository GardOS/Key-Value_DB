#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeHandler.h"

Node *CreateNode (char *keyName, char *strValue, int intValue){
	Node *node = calloc(1, sizeof(Node));
	if(node != NULL){
		node->keyName = keyName;
		
		if(strValue){
			node->strValue = calloc(1, strlen(strValue) + 1); //+ '\0'
			
			if(node->strValue) {
				strcpy(node->strValue, strValue);
			} else{
				free(node);
				node = NULL;
			}
		} else{
			node->intValue = intValue;
		}		
		
		node->next = NULL;
		node->down = NULL;
	}
	return node;
}

bool isFolder(Node *node){
	return node->down != NULL;
}
