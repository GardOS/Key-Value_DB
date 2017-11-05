#ifndef NODEHANDLER_H_
#define NODEHANDLER_H_

#define FILENAME "key-values.txt"

#define TEXT_NODE 0
#define NUMERIC_NODE 1
#define INVALID_NODE 2

typedef struct _NODE {
	char *keyName;
	char *strValue;
	unsigned long intValue;
	struct _NODE *next;
	struct _NODE *down;
} NODE;

typedef int bool; //Outside naming convention, but lets pretend its a type
enum { false, true };

NODE *CreateNode (char *keyName);
void ParseFile(NODE *rootNode);
char *SubString(int start, int end, char *str);
NODE *FindNode(NODE *node, char *location);
size_t getline(char **line, size_t *bufferSize, FILE *file);

#endif //NODEHANDLER_H_
