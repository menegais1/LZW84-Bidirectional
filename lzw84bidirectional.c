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
#define MALLOC_SIZE 1000
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
	for (i = 0; i < 127; i++)
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

int getEntryString(Dict *dict, char *text)
{
	if (dict == NULL)
		return 0;
	Entry *node = dict->root;
	while (node != NULL)
	{
		if (node->code != EOF_CODE && strcmp(node->string, text) == 0)
		{
			return node->code;
		}
		node = node->next;
	}
	return 0;
}

char* getEntryIndex(Dict *dict, int index)
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

void concat(char* dest,char* src1, char* src2){
	strcpy(dest,src1);
	strcat(dest,src2);
}

void concatCharOnString(char* src, char dest){
	char* tmp = malloc(2);
	tmp[0] = dest;
	tmp[1] = 0;
	strcat(src,tmp);
	free(tmp);
}
void setString(char* dest, char*src){
	strcpy(dest, src);
	
}

void setCharOnString(char*dest, char src){
	dest[0] = src;
	dest[1] = 0;
}

Out* initOut(int code,int flag){
	Out* out = (Out *)malloc(sizeof(Out));
	out->index = code;
	out->flag = 0;
	return out;
}

Out** encode(Dict *dict, char *text)
{
	char *prefix = malloc(MALLOC_SIZE);
	char *currentChar = malloc(MALLOC_SIZE);
	char *word = malloc(MALLOC_SIZE);
	Out** output = (Out**) malloc(sizeof(Out*) * MALLOC_SIZE);
	int code = 0;
	int cont = 0;
	prefix[0] = text[0];
	setCharOnString(prefix, *text);
	
	currentChar[0] =0;
	while(*text != 0)
	{
		concat(word,prefix,currentChar);
		int tempCode = getEntryString(dict, word);
		if (tempCode)
		{
			setString(prefix, word);
			text++;
			setCharOnString(currentChar,*text);
			code = tempCode;
			if(*currentChar == 0){
			output[cont] = initOut(code,0);
			cont++;
			break;
			}
		}
		else
		{
			addDictEntry(dict, word, -1);
			setString(prefix, currentChar);
			setCharOnString(currentChar, 0);
			output[cont] = initOut(code,0);
			cont++;
		}
		
	}

	output[cont] = initOut(EOF_CODE,0);	
	cont++;
	printDict(dict);
	printOutput(output, cont);
	return output;
}

void decode(Dict *dict, Out **input)
{
	int oldCode;
	int code = (*input)->index;
	char* word = getEntryIndex(dict,code);
	char* oldWord = malloc(MALLOC_SIZE);
	setString(oldWord, word);
	char* output = malloc(MALLOC_SIZE);
	int i;
	for(i=0;i<MALLOC_SIZE;i++){
		output[i] = 0;
	}
	strcat(output, word);
	input++;
	while ((*input)->index != EOF_CODE)
	{
		code = (*input)->index;

		word = getEntryIndex(dict,code);
		if(word != NULL){
			strcat(output, word);
			concatCharOnString(oldWord, word[0]);
			addDictEntry(dict,oldWord,-1);
			setString(oldWord,word);
		}else{
			concatCharOnString(oldWord, oldWord[0]);
			strcat(output, oldWord);
			addDictEntry(dict,oldWord,-1);
			printDict(dict);
		}
		input++;
	}

	printf("%s", output);
}

int main()
{
	char *text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam quis elementum nibh. Maecenas fermentum volutpat turpis, id sollicitudin tellus vestibulum at. Mauris molestie turpis a venenatis malesuada. Cras ut laoreet enim. Nam id maximus quam. Mauris tempor blandit nunc id tempus. Fusce tincidunt velit nec enim porttitor ornare. Aliquam id vulputate nisl, eu tempor est. In ultrices et lacus sed lobortis. Suspendisse ligula dui, posuere molestie ultricies vitae, ultrices eget lorem. Donec sodales pharetra finibus. Duis vitae nisi eget ante facilisis blandit et vel arcu. Donec a facilisis augue. Sed auctor aliquet lectus sit amet consectetur. Integer nibh dui, pretium.";
	Dict *dict = (Dict *)malloc(sizeof(Dict));
	initDict(dict);
	printDict(dict);

	Out** output = encode(dict, text);
	clearDict(dict);
	initDict(dict);
	decode(dict,output);
	return 0;
}
