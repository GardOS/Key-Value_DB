#ifndef NODEHANDLER_H_
#define NODEHANDLER_H_

#define FILENAME "key-values.txt"
#define ROOTNAME "root"

#define NODE_TEXT_TYPE 0
#define NODE_NUMERIC_TYPE 1
#define NODE_NOT_FOUND 2
#define NODE_OK 3
#define NODE_WRONG_TYPE 4

typedef unsigned long ULONG;

typedef struct _NODE {
	char *keyName;
	char *strValue;
	ULONG numValue;
	struct _NODE *next;
	struct _NODE *down;
} NODE;

typedef struct _VALUE{
	ULONG numValue;
	char *strValue;
} VALUE;

//Top node so the context dont need to be passed around.
NODE *ROOTNODE;

//Part 1
NODE *CreateNode (char *keyName);
void GenerateTree();

//Part 2
int GetType(char *key);
int GetString(char *strKey, char **strValue); 
int GetInt(char *key, ULONG *numValue); //Although it is an unsigned long ¯\(ツ)/¯
int GetValue(char *key, int nodeType, ...);

//Part 3
int Enumerate(NODE *rootNode, char *key);

//Part 4
int Delete(char *key);
void ClearNodes(NODE *node);

//Part 5
char *GetText(char *keyName, char *language);

//Generic helpers etc.
NODE *FindNode(NODE *node, char *location);
char *SubString(int start, int end, char *str);
void PrintTree(NODE *node);

#endif //NODEHANDLER_H_