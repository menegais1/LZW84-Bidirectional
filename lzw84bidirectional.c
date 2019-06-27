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
#define TEXT_MALLOC_SIZE 10000
#define DICT_START 0
#define DICT_END 127
#define MAX_DICT_SIZE 5000



void clearString(char *dest, int size)
{
	int i;
	for (i = 0; i < size; i++)
		dest[i] = 0;
}

char* allocateString(int size){
	char* tmp = (char*) malloc((size + 1) * sizeof(char));
	clearString(tmp,size);
	return tmp;
}

void addDictEntry(Dict *dict, char *string, int code)
{
	if (dict == NULL)
		return;
	if (dict->num + 1 > MAX_DICT_SIZE)
		return;
	Entry *new = (Entry *)malloc(sizeof(Entry));

	new->code = code;
	new->string = 0;
	if (code == -1)
	{
		new->code = dict->num + 1;
		dict->num = dict->num + 1;
		new->string = allocateString(strlen(string));
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
	for (i = DICT_START; i < DICT_END; i++)
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

void setString(char *dest, char *src)
{
	strcpy(dest, src);
}


void printOutput(Out **output)
{
	int i = 0;
	printf("\nOutput:\n (");
	Out **temp = output;
	while ((*temp)->index != EOF_CODE)
	{
		printf("(%d,%d) ", (*temp)->index, (*temp)->flag);
		temp++;

		i++;
		if (i == 9)
		{
			printf("\n");
			i = 0;
		}
	}
	printf(")\n");
}

int getEntryString(Dict *dict, char *text, int *length)
{
	if (dict == NULL)
		return 0;
	Entry *node = dict->root;
	while (node != NULL)
	{
		if (node->code != EOF_CODE && strcmp(node->string, text) == 0)
		{
			*length = strlen(node->string);
			return node->code;
		}
		node = node->next;
	}
	return 0;
}

void reverseString(char *str)
{
	if (str == NULL)
		return;
	if (*str == 0)
		return;
	int length = strlen(str);
	int i, j;
	char *aux = allocateString(length + 1);
	for (i = 0, j = length - 1; i < length; i++, j--)
	{
		aux[i] = str[j];
	}
	aux[i] = 0;
	strcpy(str, aux);
	free(aux);
	aux = NULL;
}

int getReverseEntryString(Dict *dict, char *text, int *length)
{
	if (dict == NULL || text == NULL)
		return 0;
	Entry *node = dict->root;
	char *tmp = allocateString(strlen(text) + 1);
	setString(tmp, text);
	reverseString(tmp);
	// printf("\nReverse: %s", text);
	while (node->next != NULL)
	{
		if (node->code != EOF_CODE && strcmp(node->string, tmp) == 0)
		{
			*length = strlen(node->string);
			free(tmp);
			return node->code;
		}
		node = node->next;
	}
	free(tmp);
	return 0;
}

char *getEntryIndex(Dict *dict, int index)
{
	if (dict == NULL)
		return 0;
	Entry *node = dict->root;
	while (node != NULL)
	{
		if (node->code == index)
		{
			char *temp = allocateString(strlen(node->string));
			setString(temp, node->string);
			return temp;
		}
		node = node->next;
	}
	return NULL;
}

void concat(char *dest, char *src1, char *src2)
{
	strcpy(dest, src1);
	strcat(dest, src2);
}

// void dynamicConcat(char *dest, char *src1, int *destTotalSize)
// {
// 	if (strlen(dest) + strlen(src1) >= *destTotalSize - 1)
// 	{
// 		*destTotalSize = *destTotalSize * 1.5f;
// 		printf("\n dest: %d", (int)strlen(dest));
// 		printf("\n src: %d", (int)strlen(src1));
// 		char *tmp = realloc(dest, *destTotalSize);
// 		if (tmp == NULL)
// 		{
// 			printf("NÃO FOI POSSIVEL ALOCAR MAIS MEMÓRIA.");
// 			exit(0);
// 		}
// 		dest = tmp;
// 	}
// 	strcat(dest, src1);
// }

void concatCharOnString(char *dest, char src)
{
	char *tmp = allocateString(1);
	tmp[0] = src;
	tmp[1] = 0;
	strcat(dest, tmp);
	free(tmp);
}

void setCharOnString(char *dest, char src)
{
	dest[0] = src;
	dest[1] = 0;
}

Out *initOut(int code, int flag)
{
	Out *out = (Out *)malloc(sizeof(Out));
	out->index = code;
	out->flag = flag;
	return out;
}

Out **encode(Dict *dict, char *text)
{
	char *prefix = allocateString(TEXT_MALLOC_SIZE);
	char *currentChar = allocateString(1);
	char *word = allocateString(TEXT_MALLOC_SIZE);
	Out **output = (Out **)malloc(sizeof(Out *) * strlen(text));
	int code = 0;
	int cont = 0;
	int forwardMatch = 0;
	int reverseMatch = 0;
	int forwardMatchSize = 0;
	int reverseMatchSize = 0;
	int flag = 0;
	prefix[0] = text[0];
	setCharOnString(prefix, *text);

	currentChar[0] = 0;
	while (*text != 0)
	{
		concat(word, prefix, currentChar);
		int forwardMatch = getEntryString(dict, word, &forwardMatchSize);
		//printf("\nReverse Match: %d", reverseMatch);

		int reverseMatch = getReverseEntryString(dict, word, &reverseMatchSize);

		if (forwardMatch || reverseMatch)
		{
			if (reverseMatchSize > forwardMatchSize)
			{
				flag = 1;
				code = reverseMatch;
			}
			else
			{
				flag = 0;
				code = forwardMatch;
			}
			reverseMatchSize = forwardMatchSize = 0;
			forwardMatch = reverseMatch = 0;
			setString(prefix, word);
			text++;
			setCharOnString(currentChar, *text);
			if (*currentChar == 0)
			{
				output[cont] = initOut(code, flag);
				cont++;
				break;
			}
		}
		else
		{
			addDictEntry(dict, word, -1);
			setString(prefix, currentChar);
			setCharOnString(currentChar, 0);
			output[cont] = initOut(code, flag);
			flag = 0;
			cont++;
		}
	}

	output[cont] = initOut(EOF_CODE, 0);
	cont++;

	free(prefix);
	free(currentChar);
	free(word);
	prefix = currentChar = word = NULL;
	return output;
}

char *decode(Dict *dict, Out **input)
{
	// int currentTextTotalSize = TEXT_MALLOC_SIZE;
	int oldCode;
	int code = (*input)->index;
	char *word = getEntryIndex(dict, code);
	char *oldWord = allocateString(TEXT_MALLOC_SIZE);
	char *output = allocateString(TEXT_MALLOC_SIZE);
	setString(oldWord, word);
	setCharOnString(output, 0);
	strcat(output, word);
	// dynamicConcat(output, word, &currentTextTotalSize);
	input++;
	while ((*input)->index != EOF_CODE)
	{
		code = (*input)->index;
		int flag = (*input)->flag;
		word = getEntryIndex(dict, code);
		if (word != NULL)
		{
			if (flag == 1)
			{
				reverseString(word);
			}

			strcat(output, word);

			// dynamicConcat(output, word, &currentTextTotalSize);
			concatCharOnString(oldWord, word[0]);
			addDictEntry(dict, oldWord, -1);
			setString(oldWord, word);
		}
		else
		{
			printf("\nREVERSE\n");
			concatCharOnString(oldWord, oldWord[0]);
			strcat(output, oldWord);
			// dynamicConcat(output, oldWord, &currentTextTotalSize);
			addDictEntry(dict, oldWord, -1);
		}
		input++;
	}
	// printDict(dict);

	free(oldWord);
	return output;
}

int main()
{
	char *text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce id risus turpis. Praesent blandit vulputate dictum. Vivamus cursus sed velit eu imperdiet. Phasellus quis arcu non ipsum posuere venenatis. Curabitur mauris est, efficitur sed sem vel, pellentesque pulvinar mauris. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Aenean sollicitudin leo dolor, congue imperdiet sem vulputate non. Cras eget est ultrices, bibendum mi et, lacinia ex. Integer ex diam, rutrum sed semper ut, dignissim at leo.Phasellus tristique bibendum finibus. Donec luctus purus eget erat pretium consequat. Curabitur tincidunt eleifend risus eu porttitor. Phasellus eu dolor pulvinar, commodo mi a, molestie tellus. Aliquam erat volutpat. Cras tempus pellentesque auctor. Praesent at vestibulum tellus.Integer posuere sodales turpis, non pulvinar massa mattis vel. Nulla a metus in erat pretium luctus in et arcu. Ut rhoncus nisi eu odio tempor hendrerit. Praesent ultrices tristique quam, non mollis lectus imperdiet vitae. Aenean in ultrices lacus. Nunc egestas orci nisl, id iaculis ligula porta ut. Maecenas iaculis a sapien a semper. Curabitur dictum elit at erat accumsan consectetur eget non ex. Nulla bibendum metus id fringilla ultrices. Duis porta risus quis arcu eleifend, in fringilla libero sagittis. Aliquam accumsan risus quis augue tristique, at lacinia diam aliquet. Etiam eget ipsum vel odio varius tempus. Nunc sed sapien porttitor, consectetur lectus viverra, sollicitudin eros.Fusce laoreet feugiat congue. Vestibulum interdum lectus vitae rutrum pretium. Vivamus semper imperdiet sem eget porttitor. Curabitur sit amet vehicula erat. Ut elementum condimentum turpis eget tempus. Mauris sit amet ultricies quam, eu faucibus massa. Proin luctus augue sapien, sit amet ultrices nunc fermentum sit amet. Nullam at mollis neque. Fusce a tincidunt purus.Morbi sollicitudin sapien justo, in consectetur dolor volutpat sed. Etiam at pharetra eros. Nullam maximus leo at arcu semper, vel sagittis nunc egestas.";

	Dict *dict = (Dict *)malloc(sizeof(Dict));
	initDict(dict);

	Out **output = encode(dict, text);
	// printOutput(output);
	clearDict(dict);
	initDict(dict);
	char *str = decode(dict, output);
	// printOutput(output);

	clearDict(dict);
	printf("%s", str);

	// free(output);
	// free(str);
	return 0;
}
