#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeHandler.h"

//A bit verbose with the if/else, but the output should be crystal clear
int main (void)
{
	int status;
	ROOTNODE = CreateNode(ROOTNAME);
	
	//Part 1
	GenerateTree();
	printf("Tree:\n");
	PrintTree(ROOTNODE->down);
	
	//Part 2
	//GetType
	char *typeKey = "config.update.server1";
	
	printf("\nGetType: %s:\n", typeKey);
	int type = GetType(typeKey);
	if(type == NODE_TEXT_TYPE){
		printf("Node is text type\n");
	} else if(type == NODE_NUMERIC_TYPE){
		printf("Node is numeric type\n");
	} else{
		printf("Node is invalid\n");
	}
	
	//GetString
	char *strKey = "config.update.server1";
	
	printf("\nGetString: \n");	
	char *strValue = NULL;
	status = GetString(strKey, &strValue);
	if(status == NODE_OK){
		printf("%s = %s\n", strKey, strValue);
	} else if(status == NODE_WRONG_TYPE){
		printf("%s did not point to node with type string\n", strKey);
	} else{
		printf("Could not find node with key: %s\n", strKey);
	}
	
	//GetInt
	char *numKey = "config.update.interval";
	
	printf("\nGetInt: \n");
	ULONG numValue = 0;
	status = GetInt(numKey, &numValue);
	if(status == NODE_OK){
		printf("%s = %lu\n", numKey, numValue);
	} else if(status == NODE_WRONG_TYPE){
		printf("%s did not point to node with type number\n", numKey);
	} else{
		printf("Could not find node with key: %s\n", numKey);
	}
	
	//GetValue with text
	printf("\nGetValue with text: \n");
	status = GetValue(strKey, NODE_TEXT_TYPE, &strValue);
	if(status == NODE_OK){
		printf("%s = %s\n", strKey, strValue);
	} else if(status == NODE_WRONG_TYPE){
		printf("%s did not point to node with type string\n", strKey);
	} else{
		printf("Could not find node with key: %s\n", strKey);
	}
	
	//GetValue with number
	printf("\nGetValue with number: \n");
	status = GetValue(numKey, NODE_NUMERIC_TYPE, &numValue);
	if(status == NODE_OK){
		printf("%s = %lu\n", numKey, numValue);
	} else if(status == NODE_WRONG_TYPE){
		printf("%s did not point to node with type number\n", numKey);
	} else{
		printf("Could not find node with key: %s\n", numKey);
	}
	
	//Part 3
	char *enumerateKey = "config.update.*";
	
	printf("\nEnumerate: %s\n", enumerateKey);
	status = Enumerate(ROOTNODE, enumerateKey);
	if(status == NODE_NOT_FOUND){
		printf("Could not find node with key: %s\n", enumerateKey);
	}
	
	//Part 4
	char *deleteKey = "config.loglevel";
	
	printf("\nDelete: %s\n", deleteKey);
	status = Delete(deleteKey);
	if(status == NODE_OK){
		printf("Tree after deleting %s:\n", deleteKey);
		PrintTree(ROOTNODE->down);
	} else{
		printf("Could not find node with key: %s\n", deleteKey);
	}	
	
	//Part 5
	char *getTextKey = "button_ok";
	
	char *language = "no";
	printf("\nGetText: \n");
	char* text = GetText(getTextKey, language);
	if(text != NULL){
		printf("Value: %s\n", text);
	} else {
		printf("GetText: Found no value at that location\n");
	}
	
	ClearNodes(ROOTNODE);
}
