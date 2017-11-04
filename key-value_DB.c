#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeHandler.h"

int main (void)
{
	Node *node1 = CreateNode("KeyName1", "StringValue1", 0);
	Node *node2 = CreateNode("KeyName2", NULL, 3);
	
	
	printf("Node 1: name: %s, value: %s\n", node1->keyName, node1->strValue);
	printf("Node 2: name: %s, value: %lu\n", node2->keyName, node2->intValue);
}
