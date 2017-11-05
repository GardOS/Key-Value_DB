#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeHandler.h"

void ParseLine(char *line, NODE *parentNode);
void SetValueToKey(int linePosition, char *line, NODE *currentNode);

NODE *CreateNode (char *keyName){
	NODE *node = calloc(1, sizeof(NODE));
	if(node != NULL){
		node->keyName = keyName;
		node->strValue = NULL;
		node->intValue = 0; //Any better way?
		node->next = NULL;
		node->down = NULL;
	}
	return node;
}

void ParseFile(NODE *rootNode){	
	char *line = NULL;
	size_t bufferSize = 0;
	size_t charCount;
	
	FILE *file = fopen(FILENAME, "r");
	
	if(file == NULL){
		printf("Could not find file: %s\n", FILENAME);
		exit(1);
	}
	
	while((charCount = getline(&line, &bufferSize, file)) != -1){
		NODE *parentNode = rootNode;
		ParseLine(line, parentNode);
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
			parentNode = currentNode;
		
			//Done checking keys
			if(line[i] == ' ' ||  line[i] == '='){
				linePosition = i;
				SetValueToKey(linePosition, line, currentNode);
				break;
			}
		}
		
	}
}

void SetValueToKey(int linePosition, char *line, NODE *currentNode){
	//Iterate through whitespaces, the equal sign and then find value
	for(int i = linePosition; i < strlen(line); i++){
		if(line[i] == '\"'){
			linePosition = i + 1;
			currentNode->strValue = SubString(linePosition, strlen(line) - 2, line); //leak
			break;
		} else if(strchr("1234567890", line[i])){
			linePosition = i;
			currentNode->intValue = atoi(SubString(linePosition, strlen(line) - 1, line));
			break;
		}
	}
}

NODE *FindNode(NODE *node, char *key){
	NODE *GetNodeWithKey(NODE *node, char *keyName);
	int linePosition = 0;
	NODE *currentNode = node;
	
	for(int i = 0; i < strlen(key) + 1; i++){
		if(key[i] == '.' ||  i == strlen(key)){
			char *nodeName = SubString(linePosition, i, key);
			linePosition = i + 1; //Skip '.'
			currentNode = GetNodeWithKey(currentNode, nodeName);
		}
	}
	return currentNode;
}

NODE *GetNodeWithKey(NODE *node, char *keyName){
	if(node->down == NULL){
		return NULL;
	}
	
	if(node->down->keyName == keyName){
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
	return NULL;
}

char *SubString(int start, int end, char *line){
	int nameLength = end - start;
	char *nodeName = calloc(1, nameLength + 1);
	strncpy(nodeName, line + start, nameLength);
	nodeName[nameLength] = '\0';
	return nodeName;
}
