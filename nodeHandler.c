#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "nodeHandler.h"

//Private methods
void ParseLine(char *line, NODE *parentNode);
void SetValueToKey(int linePosition, char *line, NODE *currentNode);
char *GetParentNodeKey(char *key);

//Part 1
NODE *CreateNode (char *keyName){
	//Leak :(
	NODE *node = calloc(1, sizeof(NODE));
	if(node != NULL){
		node->keyName = keyName;
	} else{
		printf("Error, could not calloc new node\n");
		exit(1);
	}
	return node;
}

void GenerateTree(){
	char *line = NULL;
	size_t bufferSize = 0;
	
	FILE *file = fopen(FILENAME, "r");
	
	if(file == NULL){
		printf("Could not find file: %s\n", FILENAME);
		exit(1);
	}
	
	while((getline(&line, &bufferSize, file)) != -1){
		ParseLine(line, ROOTNODE);
	}
	
	free(line);
	fclose(file);
}

void ParseLine(char *line, NODE *parentNode){
	int linePosition = 0;
	NODE *currentNode = NULL;
	//Iterate through keys
	for(int i = 0; i < strlen(line); i++){
		if(line[i] == '.' ||  line[i] == ' ' ||  line[i] == '='){
			char *nodeName = SubString(linePosition, i, line);
			linePosition = i + 1; //Skip '.'
			
			currentNode = CreateNode(nodeName);
			
			if(parentNode->down == NULL){
				parentNode->down = currentNode;
			} else {
				//->down is taken, iterate through ->next to find a spot
				NODE *tempNode = parentNode->down;
				while(tempNode){
					if(strcmp(tempNode->keyName, currentNode->keyName) == 0){
						currentNode = tempNode;
						break;
					} else if(tempNode->next == NULL){
						tempNode->next = currentNode;
						break;
					}
					tempNode = tempNode->next;
				}
				
			}
			//Done checking keys. Set value to the last key found
			if(line[i] == ' ' ||  line[i] == '='){
				linePosition = i;
				SetValueToKey(linePosition, line, currentNode);
				break;
			}
			parentNode = currentNode;
		}
		if(i == strlen(line)){
			printf("File is in invalid format. Exiting.");
			exit(1);
		}
	}
}

void SetValueToKey(int linePosition, char *line, NODE *currentNode){
	//Iterate through whitespaces & the equal sign and then find value
	for(int i = linePosition; i < strlen(line); i++){
		if(line[i] == '\"'){
			linePosition = i + 1;
			currentNode->strValue = SubString(linePosition, strlen(line) - 2, line);
			break;
		} else if(strchr("1234567890", line[i])){
			linePosition = i;
			char *value = SubString(linePosition, strlen(line) - 1, line);
			currentNode->numValue = atoi(value);
			free(value);
			break;
		}
		if(i == strlen(line)){
			printf("File is in invalid format. Exiting.");
			exit(1);
		}
	}
}

//Part 2
int GetType(char *key){
	NODE *node = FindNode(ROOTNODE, key);
	if(node == NULL){
		return NODE_NOT_FOUND;
	} else if(node->strValue != NULL){
		return NODE_TEXT_TYPE;
	} else{
		return NODE_NUMERIC_TYPE;
	}
}

int GetString(char *key, char **strValue){
	NODE *node = FindNode(ROOTNODE, key);
	
	if(node == NULL){
		return NODE_NOT_FOUND;
	}
	if(node->strValue == NULL || node->down != NULL){
		return NODE_WRONG_TYPE;
	}
	
	*strValue = node->strValue;
	
	return NODE_OK;
}

int GetInt(char *key, ULONG *numValue){
	NODE *node = FindNode(ROOTNODE, key);
	
	if(node == NULL){
		return NODE_NOT_FOUND;
	}
	if(node->strValue != NULL || node->down != NULL){
		return NODE_WRONG_TYPE;
	}
	
	numValue = (ULONG*)node->numValue;
	
	return NODE_OK;
}

int GetValue(char *key, int nodeType, ...){
	int status;
	va_list args;
	va_start(args, nodeType);
	
	if(nodeType == NODE_TEXT_TYPE){
		char **strValue = va_arg(args, char**);
		status = GetString(key, strValue);
	} else if(nodeType == NODE_NUMERIC_TYPE){
		ULONG *numValue = va_arg(args, ULONG*);
		status = GetInt(key, numValue);
	} else{
		status = NODE_WRONG_TYPE;
	}
	va_end(args);
	return status;
}

//Part 3
typedef void (CALLBACK)(char *name, VALUE value);

void CallBack(char *name, VALUE value){
	if(value.strValue != NULL){
		printf("%s: %s\n", name, value.strValue);
	} else{
		printf("%s: %lu\n", name, value.numValue);
	}
}

int Enumerate(NODE *startNode, char *key){
	CALLBACK *callBack = &CallBack;
	key = SubString(0, strlen(key) - 2, key); //Remove ".*"
	NODE *node = FindNode(startNode, key);
	
	if(node == NULL){
		free(key);
		return NODE_NOT_FOUND;
	}
	
	node = node->down;
	
	while(node != NULL){
		VALUE value;
		value.strValue = NULL;
		if(node->down == NULL){
			if(node->strValue){
				value.strValue = node->strValue;
				callBack(node->keyName, value);
			} else{
				value.numValue = node->numValue;
				callBack(node->keyName, value);
			}
		}
		node = node->next;
	}
	free(key);
	return NODE_OK;
}

//Part 4
int Delete(char *key){
	NODE *parentNode;
	char *trimmedKey;
	NODE *node = FindNode(ROOTNODE, key);
	
	if(node == NULL){
		return NODE_NOT_FOUND;
	}
	
	trimmedKey = GetParentNodeKey(key);
	parentNode = FindNode(ROOTNODE, trimmedKey);
	if(parentNode == NULL){
		parentNode = ROOTNODE;
	}
	
	if(parentNode->down == node){
		ClearNodes(parentNode->down);
		parentNode->down = NULL;
		
		if(parentNode->next != NULL){
			node = parentNode;
			trimmedKey = GetParentNodeKey(trimmedKey);
			parentNode = FindNode(ROOTNODE, trimmedKey);
			if(parentNode == NULL){
				parentNode = ROOTNODE;
			}
			
			parentNode->down = node->next;
			node->next = NULL;
			node = NULL;
		} 
	}else{
		NODE *tempNode = parentNode->down;
		while(tempNode != NULL){
			if(tempNode->next == node){
				ClearNodes(tempNode->next);
				tempNode->next = NULL;
				break;
			}
			tempNode = tempNode->next;
		}
	}	
	
	if(parentNode == ROOTNODE){
		if(ROOTNODE->down == NULL){
			//printf("Tree is empty, nothing more to do. Exiting.\n");
			//exit(0);
		}
	} if(parentNode->down == NULL && parentNode->next == NULL){
		Delete(trimmedKey);
		free(trimmedKey);
	}
	return NODE_OK;
}

char *GetParentNodeKey(char *key){
	for(int i = strlen(key); i-- > 0;){
		if(key[i] == '.'){
			char *trimmedKey = SubString(0, i, key);
			return trimmedKey;
		}
	}
	return ROOTNAME;
}

void ClearNodes(NODE *node){
	if(node->down != NULL){
		ClearNodes(node->down);
		node->down = NULL;
	}
	if(node->next != NULL){
		ClearNodes(node->next);
		node->next = NULL;
	}
	if(node->strValue){
		free(node->strValue);
		node->strValue = NULL;
	}
	if(node != ROOTNODE){
		free(node->keyName);
		node->keyName = NULL;
	}
	free(node);
	node = NULL;
}

//Part 5
char *GetText(char *keyName, char *language){
	//Hardcoded values since the task was very specific.
	if((strcmp(language, "no") != 0) && (strcmp(language, "en") != 0)){
		printf("Invalid language\n");
		return NULL;
	}
	
	char *stringNodeName = "strings.";
	
	int keyLength = strlen(stringNodeName) + strlen(keyName) + strlen(language) + 2; //+2 for '.' and '\0'
	char *key = calloc(1, keyLength);
	strcat(key, stringNodeName);
	strcat(key, language);
	strcat(key, ".");
	strcat(key, keyName);
	
	NODE *node = FindNode(ROOTNODE, key);
	
	if(node == NULL && strcmp(language, "no") == 0){
		printf("Found no value in 'no', checking 'en'.\n");
		key[strlen(stringNodeName)] = 'e';
		key[strlen(stringNodeName) + 1] = 'n';
		node = FindNode(ROOTNODE, key);
	}
	free(key);
	if(node != NULL && node->strValue != NULL){
		return node->strValue;
	} else{
		return NULL;
	}
}

//Generic helpers etc.
NODE *FindNode(NODE *node, char *key){
	NODE *GetNodeWithKey(NODE *node, char *keyName);
	int linePosition = 0;
	NODE *currentNode = node;
	
	for(int i = 0; i < strlen(key) + 1; i++){
		if(key[i] == '.' ||  i == strlen(key)){
			char *nodeName = SubString(linePosition, i, key);
			linePosition = i + 1; //Skip '.'
			currentNode = GetNodeWithKey(currentNode, nodeName);
			free(nodeName);
			if(currentNode == NULL){
				return NULL;
			}
		}
	}
	if(currentNode == node){
		return NULL;
	}
	return currentNode;
}

NODE *GetNodeWithKey(NODE *node, char *keyName){
	if(node->down == NULL){
		return NULL;
	}
	
	if(strcmp(node->down->keyName, keyName) == 0){
		return node->down;
	} else {
		NODE *tempNode = node->down;
		while(tempNode->next != NULL){
			tempNode = tempNode->next;
			if(strcmp(tempNode->keyName, keyName) == 0){
				return tempNode;
			}
		}
	}
	if(node == ROOTNODE){
		return ROOTNODE;
	}
	return NULL;
}

char *SubString(int start, int end, char *line){
	int nameLength = end - start;
	char *nodeName = calloc(1, nameLength + 1);
	strncpy(nodeName, line + start, nameLength);
	nodeName[nameLength] = '\0';
	return nodeName;
}

void PrintTree(NODE *node){
	if(node->down == NULL){
		if(node->strValue != NULL){
			printf("   %s = %s\n", node->keyName, node->strValue);
		} else{
			printf("   %s = %lu\n", node->keyName, node->numValue);
		}
	} else{
		printf("Folder: %s\n", node->keyName);
	}
	if(node->down != NULL){
		PrintTree(node->down);
	}
	if(node->next != NULL){
		PrintTree(node->next);
	}
}