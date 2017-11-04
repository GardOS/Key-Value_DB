#ifndef NODEHANDLER_H_
#define NODEHANDLER_H_

typedef unsigned long ULONG;

typedef struct _Node {
	char *keyName;
	char *strValue;
	ULONG intValue;
	struct _Node *next;
	struct _Node *down;
} Node;

typedef int bool;
enum { false, true};

Node *CreateNode (char *keyName, char *strValue, int intValue);
bool isFolder(Node *node);

#endif //NODEHANDLER_H_
