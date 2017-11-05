#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeHandler.h"

void FreeTree(NODE *node);
void PrintTree(NODE *node);
int GetType(NODE *rootNode, char *key);

int main (void)
{
	//Entrance to tree so methods has a context
	NODE *rootNode = CreateNode("root");
	ParseFile(rootNode);
	
	char *key = "config.update.server1";
	int type = GetType(rootNode, key);
	
	switch(type){
	case TEXT_NODE:
		printf("Node is text type\n");
		break;
	case NUMERIC_NODE:
		printf("Node is numeric type\n");
		break;
	case INVALID_NODE:
		printf("Node is invalid\n");
		break;
	}
	
	PrintTree(rootNode);
	FreeTree(rootNode);
	free(rootNode);
}



int GetType(NODE *rootNode, char *key){
	NODE *node = FindNode(rootNode, key);
	if(node == NULL){
		return INVALID_NODE;
	} else if(node->strValue != NULL){
		return TEXT_NODE;
	} else{
		return NUMERIC_NODE;
	}
}

void PrintTree(NODE *node){
	printf("%s.", node->keyName);
	if(node->down == NULL){		
		if(node->strValue != NULL){
			printf(" val: %s\n", node->strValue);
		} else{
			printf(" val: %lu\n", node->intValue);
		}
	}
	if(node->down != NULL){
		PrintTree(node->down);
	}
	if(node->next != NULL){
		PrintTree(node->next);
	}
}

void FreeTree(NODE *node){
	if(node->down != NULL){
		FreeTree(node->down);
	}
	if(node->next != NULL){
		FreeTree(node->next);
	}
	if(node->down == NULL && node->next == NULL){
		if(node->strValue){
			free(node->strValue);
		}
		free(node->keyName);
		free(node);
		node = NULL;
	}
}