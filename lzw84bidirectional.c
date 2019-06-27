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
	char flag;
	int index;
} Out;

#define EOF_CODE 0
#define TEXT_MALLOC_SIZE 50000
#define DICT_START 0
#define DICT_END 127
#define MAX_DICT_SIZE 5000
#define FILE_MAX_CHAR_SIZE 50000

void clearString(char *dest, int size)
{
	int i;
	for (i = 0; i < size; i++)
		dest[i] = 0;
}

char *allocateString(int size)
{
	char *tmp = (char *)malloc((size + 1) * sizeof(char));
	clearString(tmp, size);
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

char *loadTxtFileInMemory(char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		printf("Não foi possivel ler o arquivo");
		exit(0);
	}
	char *text = malloc(FILE_MAX_CHAR_SIZE);
	char c;
	int i = 0;
	if (fseek(f, 0, SEEK_END) < 0)
		printf("Erro no seek");
	long eof = ftell(f);
	rewind(f);
	while (eof != ftello(f))
	{
		c = getc(f);
		text[i] = c;
		i++;
		if (i >= FILE_MAX_CHAR_SIZE - 1)
		{
			break;
		}
	}
	text[i] = 0;
	fclose(f);
	return text;
}

int getOutputSize(Out **output)
{
	Out **temp = output;
	int size = 1;
	while ((*temp)->index != EOF_CODE)
	{
		size++;
		temp++;
	}

	return size;
}

void saveTxtFileInMemory(char *path, char *str)
{
	FILE *f = fopen(path, "w");
	if (f == NULL)
	{
		printf("Não foi possivel ler o arquivo");
		exit(0);
	}
	while (*str != 0)
	{
		fputc(*str, f);
		str++;
	}
	fclose(f);
}

void saveBinFileInMemory(char *path, Out **output)
{
	FILE *f = fopen(path, "wb");
	if (f == NULL)
	{
		printf("Não foi possivel ler o arquivo");
		exit(0);
	}
	int outSize = getOutputSize(output);
	int i;
	Out *newOutput = malloc(outSize * sizeof(Out));
	for (i = 0; i < outSize; i++)
	{
		newOutput[i] = *(output[i]);
	}

	fwrite(newOutput, sizeof(Out), outSize, f);
	fclose(f);
	free(newOutput);
	newOutput = NULL;
}

Out **loadBinFileInMemory(char *path)
{
	FILE *f = fopen(path, "rb");
	if (f == NULL)
	{
		printf("Não foi possivel ler o arquivo");
		exit(0);
	}

	if (fseek(f, 0, SEEK_END) < 0)
		printf("Erro no seek");
	long eof = ftell(f);
	rewind(f);
	printf("%d", (int)eof);
	Out *newOutput = malloc(eof);

	fread(newOutput, sizeof(Out), eof / sizeof(Out), f);

	Out **output;
	output = malloc((eof / sizeof(Out)) * sizeof(Out *));
	int i = 0;
	while ((*newOutput).index != EOF_CODE)
	{
		output[i] = newOutput;
		i++;
		newOutput++;
	}
	output[i] = newOutput;

	fclose(f);
	return output;
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
	if (dict == NULL || dict->root == NULL)
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
	int currentTextTotalSize = TEXT_MALLOC_SIZE;
	int oldCode;
	int code = (*input)->index;
	char *word = getEntryIndex(dict, code);
	char *oldWord = allocateString(TEXT_MALLOC_SIZE);
	char *output = allocateString(TEXT_MALLOC_SIZE);
	setString(oldWord, word);
	setCharOnString(output, 0);
	strcat(output, word);
	free(word);
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
			free(word);
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

	free(oldWord);
	return output;
}

void freeOut(Out **output)
{
	Out **temp = output;
	while ((*temp)->index != EOF_CODE)
	{
		free(*temp);
		temp++;
	}
	free(*temp);
}

int main()
{
	int opt = 0;
	Dict *dict = (Dict *)malloc(sizeof(Dict));
	char *filePath = malloc(100);
	do
	{
		printf("\nOlá!!!");
		printf("\nPara comprimir um arquivo, digite 1:");
		printf("\nPara descomprimir um arquivo, digite 2:");
		printf("\nPara sair, digite 99:\n");
		scanf("%d", &opt);

		switch (opt)
		{
		case 1:
			printf("\nInsira o caminho do arquivo a ser comprimido: ");
			scanf("%s", filePath);
			char *txt = loadTxtFileInMemory(filePath);
			initDict(dict);
			Out **outputEncoded = encode(dict, txt);
			saveBinFileInMemory("ENCODED.bin", outputEncoded);
			freeOut(outputEncoded);
			free(outputEncoded);
			free(txt);
			break;
		case 2:
			printf("\nInsira o caminho do arquivo a ser descomprimido: ");
			scanf("%s", filePath);
			Out **encodedTxt = loadBinFileInMemory(filePath);
			initDict(dict);
			char *outputDecoded = decode(dict, encodedTxt);
			saveTxtFileInMemory("DECODED.txt", outputDecoded);
			clearDict(dict);
			*filePath = 0;
			freeOut(encodedTxt);
			//free(encodedTxt);

			//free(outputDecoded);
			break;
		case 99:
			printf("\nAté mais!!");
			break;
		default:
			break;
		}
	} while (opt != 99);

	clearDict(dict);
	free(dict);
	free(filePath);
	dict = NULL;
	filePath = NULL;

	return 0;
}
