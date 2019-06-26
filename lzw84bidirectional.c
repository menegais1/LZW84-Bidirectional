#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry
{
	int code;
	char *string;

	struct entry *next;
};

typedef struct entry Entry;

typedef struct dict
{
	int num;
	Entry *root;
} Dict;

typedef struct out
{
	int index;
	int flag;
} Out;

#define EOF_CODE 0

void addDictEntry(Dict *dict, char *string, int code)
{
	if (dict == NULL)
		return;
	Entry *new = (Entry *)malloc(sizeof(Entry));

	new->code = code;
	new->string = 0;
	if (code == -1)
	{
		new->code = dict->num + 1;
		dict->num = dict->num + 1;
		new->string = (char *)malloc((strlen(string) + 1) * sizeof(char));
		strcpy(new->string, string);
	}

	new->next = NULL;
	Entry *node = dict->root;
	if (node == NULL)
		dict->root = new;
	else
	{
		while (node->next != NULL)
			node = node->next;
		node->next = new;
	}
}

void initDict(Dict *dict)
{
	if (dict == NULL)
		return;
	dict->num = 0;
	dict->root = NULL;
	char i = 0;
	for (i = 'a'; i <= 'z'; i++)
	{
		char c[2];
		c[0] = i;
		c[1] = 0;
		addDictEntry(dict, c, -1);
	}
	addDictEntry(dict, 0, EOF_CODE);
}

void removeNode(Entry *node)
{
	if (node == NULL)
		return;
	free(node->string);
	node->string = NULL;

	Entry *nextNode = node->next;
	free(node);
	node = NULL;
	removeNode(nextNode);
}

void clearDict(Dict *dict)
{
	if (dict == NULL)
		return;

	removeNode(dict->root);
	dict->root = NULL;
}

void printDict(Dict *dict)
{
	if (dict == NULL)
		return;
	Entry *node = dict->root;
	printf("\nDictionary: {\n");
	int cont = 0;
	while (node != NULL)
	{

		printf("(%s,%d) ", node->string, node->code);
		node = node->next;
		if (cont == 9)
		{
			printf("\n");
			cont = 0;
		}
		cont++;
	}
	printf("\n}\n");
}

void printOutput(Out **output, int size)
{
	int i = 0;
	printf("\nOutput: (");
	for (i = 0; i < size; i++)
	{
		printf("(%d,%d) ", output[i]->index, output[i]->flag);
	}
	printf(")\n");
}

int hasEntryString(Dict *dict, char *text)
{
	if (dict == NULL)
		return 0;
	Entry *node = dict->root;
	while (node != NULL && node->code != EOF_CODE)
	{
		if (strcmp(node->string, text) == 0)
		{
			return node->code;
		}
		node = node->next;
	}
	return 0;
}

char* hasEntryIndex(Dict *dict, int index)
{
	if (dict == NULL)
		return 0;
	Entry *node = dict->root;
	while (node != NULL)
	{
		if (node->code == index)
		{
			return node->string;
		}
		node = node->next;
	}
	return NULL;
}

char *encode(Dict *dict, char *text)
{
	char *prefix = malloc(100);
	char *current = malloc(100);
	char *prefixCurrent = malloc(100);
	Out *output[100];
	prefix[0] = text[0];
	prefix[1] = 0;
	current[0] = 0;
	int code = 0;
	int cont = 0;
	while (*text != 0)
	{
		strcpy(prefixCurrent, prefix);
		strcat(prefixCurrent, current);
		int tempCode = hasEntryString(dict, prefixCurrent);
		if (tempCode)
		{
			strcpy(prefix, prefixCurrent);
			text++;
			current[0] = *text;
			current[1] = 0;
			code = tempCode;
		}
		else
		{
			addDictEntry(dict, prefixCurrent, -1);
			strcpy(prefix, current);
			current[0] = 0;
			output[cont] = (Out *)malloc(sizeof(Out));
			output[cont]->index = code;
			output[cont]->flag = 0;
			cont++;
		}
	}

	printDict(dict);
	printOutput(output, cont);
}

void decode(Dict *dict, Out **input)
{
	int oldCode;
	int code = (*input)->index;
	char* word = hasEntryIndex(dict,code);
	char* oldWord = word;
	char* output = malloc(100);
	strcat(output, word);
	while ((*input)->index != EOF_CODE)
	{
		code = (*input)->index;
		word = hasEntryIndex(dict,code);
		if(word != NULL){
			strcat(output, word);
			char* temp = malloc(100);
		}else{

		}
	}
}

int main()
{
	char *text = "aabcabacb";
	Dict *dict = (Dict *)malloc(sizeof(Dict));
	initDict(dict);
	printDict(dict);

	encode(dict, text);
	printDict(dict);

	return 0;
}
