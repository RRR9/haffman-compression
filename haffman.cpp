#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "haffman.h"
#include "queue.h"

static const int SIZE_OF_NODES = 256;
static const int SIZE_OF_ARRAY = 256;
static const int SIZE_OF_PATH_TO_FILE = 512;

typedef struct HaffmanNode HaffmanNode;
typedef struct Nodes Nodes;
typedef struct CodesOfSymbols CodesOfSymbols;
typedef unsigned long long ull;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short int usint;

#include "AVLTree.h"

#define _CRT_SECURE_NO_WARNINGS
#define WRITE_ONE_ELEM 1
#define READ_ONE_ELEM 1
#define COUNT_BIT_ON_BYTE 8


static int stringcmp(const void *, const void *);
static int floatcmp(const void *, const void *);
static long int file_size(FILE *);
static void build_first_node(void);
static void build_tree(void);
static void get_data(float, HaffmanNode **);
static void code_of_symbol(HaffmanNode **);
static void coding_to_file(void);
static int count_bit(void);
static HaffmanNode * tree_for_decifer(HaffmanNode *);
static void print_symbol(const char, const char, FILE *);

struct HaffmanNode {
    HaffmanNode *left;
    HaffmanNode *right;
    float val;
    uchar symb;
};

struct Nodes {
    float val;
    HaffmanNode *node;
};

struct CodesOfSymbols {
    char *code;
    uchar symb;
};

static size_t nodeSize, codeSize;
static Nodes *node;
static CodesOfSymbols *codeAndSymbol;
static HaffmanNode *HFRoot, *p;
static float *freqOfSymbol;
static uint *countOfSymbol;
static usint diffSymbol;
static char *fileInputName, *fileOutputName, *code;
static char countBitOfEnd;
static AVLNode *AVLRoot, *AVLSearch;

static int floatcmp(const void *a, const void *b) {
    Nodes *t1 = (Nodes *)a, *t2 = (Nodes *)b;
    if(t1->val > t2->val) {
        return 1;
    } else {
        return -1;
    }
}

static int stringcmp(const void *a, const void *b) {
    CodesOfSymbols *t1 = (CodesOfSymbols *)a, *t2 = (CodesOfSymbols *)b;
    return strcmp(t1->code, t2->code);
}

static long int file_size(FILE *file) {
    long int savePos, sizeOfFile;
    savePos = ftell(file);
    fseek(file, 0L, SEEK_END);
    sizeOfFile = ftell(file);
    fseek(file, savePos, SEEK_SET);
    return sizeOfFile;
}

void codify(const char *in, const char *out) {
    fileInputName = (char *)malloc(SIZE_OF_PATH_TO_FILE);
    fileOutputName = (char *)malloc(SIZE_OF_PATH_TO_FILE);
    strcpy(fileInputName, in);
    strcpy(fileOutputName, out);
    FILE *fin = fopen(fileInputName, "rt");
    size_t i;
    if(fin == NULL) {
        perror(fileInputName);
        exit(EXIT_FAILURE);
    }
    freqOfSymbol = (float *)malloc(SIZE_OF_ARRAY * sizeof(float));
    countOfSymbol = (uint *)malloc(SIZE_OF_ARRAY * sizeof(uint));
    node = (Nodes *)malloc(SIZE_OF_NODES * sizeof(Nodes));
    long int sizeOfFile;
    for(i = 0; i < SIZE_OF_ARRAY; ++i) {
        freqOfSymbol[i] = 0.F;
        countOfSymbol[i] = 0;
    }
    char tempChar;
    tempChar = fgetc(fin);
    if(feof(fin)) {
        printf("Ôàéë ïóñòîé!\n");
        exit(EXIT_FAILURE);
    }
    ungetc(tempChar, fin);
    while((tempChar = fgetc(fin)) != EOF) {
        ++countOfSymbol[(size_t)tempChar];
    }
    //printf("\n");
    fseek(fin, 0L, SEEK_SET);
    sizeOfFile = file_size(fin);
    for(i = 0; i < SIZE_OF_ARRAY; ++i) {
        if(countOfSymbol[i]) {
            freqOfSymbol[i] = (float)countOfSymbol[i] / sizeOfFile;
        }
    }
    fclose(fin);
    build_first_node();
    free(freqOfSymbol);
    free(countOfSymbol);
    free(fileInputName);
    free(fileOutputName);
}

static void build_first_node(void) {
    int i;
    nodeSize = diffSymbol = 0;
    Nodes tempNode;
    for(i = 0; i < SIZE_OF_ARRAY; ++i) {
        if(countOfSymbol[i]) {
            ++diffSymbol;
            HaffmanNode *tempHN = (HaffmanNode *)malloc(sizeof(HaffmanNode));
            tempHN->left = tempHN->right = NULL;
            tempHN->symb = i;
            tempHN->val = freqOfSymbol[i];
            tempNode.node = tempHN;
            tempNode.val = freqOfSymbol[i];
            push(node, &tempNode, &nodeSize, sizeof(Nodes), floatcmp);
            //printf("%d\n", nodeSize);
        }
    }
    build_tree();
}

static void get_data(float *n, HaffmanNode **HN) {
    Nodes tempNode;
    pop(node, &tempNode, &nodeSize, sizeof(Nodes), floatcmp);
    *HN = tempNode.node;
    *n = tempNode.val;
}

static void build_tree(void) {
    HaffmanNode *tempHN1, *tempHN2;
    float x1, x2;
    usint i;
    Nodes tempNode;
    //printf("%d\n", diffSymbol);
    for(i = 0; i < diffSymbol - 1; ++i) {
        HaffmanNode *tempHN = (HaffmanNode *)malloc(sizeof(HaffmanNode));
        get_data(&x1, &tempHN1);
        get_data(&x2, &tempHN2);
        tempHN->left = tempHN1;
        tempHN->right = tempHN2;
        tempHN->val = x1 + x2;
        tempHN->symb = '\0';
        tempNode.node = tempHN;
        tempNode.val = x1 + x2;
        push(node, &tempNode, &nodeSize, sizeof(Nodes), floatcmp);
    }
    //printf("%d\n", nodeSize);
    pop(node, &tempNode, &nodeSize, sizeof(Nodes), floatcmp);
    HFRoot = tempNode.node;
    //printf("%.2f\n", HFRoot->val);
    code = (char *)malloc(SIZE_OF_ARRAY);
    codeAndSymbol = (CodesOfSymbols *)malloc(SIZE_OF_ARRAY * sizeof(CodesOfSymbols));
    memset(code, 0, SIZE_OF_ARRAY);
    codeSize = 0;
    code_of_symbol(&HFRoot);
    coding_to_file();
    free(code);
}

static void code_of_symbol(HaffmanNode **HN) {
    if(*HN != NULL) {
        if((*HN)->symb != '\0') {
            size_t i = (size_t)(*HN)->symb;
            codeAndSymbol[i].symb = (*HN)->symb;
            codeAndSymbol[i].code = (char *)malloc(strlen(code) + 1);
            memset(codeAndSymbol[i].code, 0, strlen(code) + 1);
            strcpy(codeAndSymbol[i].code, code);
            printf("%c:  %s\n", codeAndSymbol[i].symb, codeAndSymbol[i].code);
            //printf("\n%c:  %s\n", (*HN)->symb, code);
        }
        code[codeSize++] = '0';
        code_of_symbol(&(*HN)->left);
        --codeSize;
        code[codeSize++] = '1';
        code_of_symbol(&(*HN)->right);
        code[--codeSize] = '\0';
    }
}

static int count_bit(void) {
    size_t i;
    int res = 0;
    for(i = 0; i < SIZE_OF_ARRAY; ++i) {
        if(countOfSymbol[i]) {
            res += countOfSymbol[i] * (int)strlen(codeAndSymbol[i].code);
        }
    }
    return res;
}

static void coding_to_file(void) {
    FILE *fout = fopen(fileOutputName, "wb");
    FILE *fin = fopen(fileInputName, "rt");
    char bit, byte, shift;
    countBitOfEnd = !(count_bit() % COUNT_BIT_ON_BYTE) ? COUNT_BIT_ON_BYTE : count_bit() % COUNT_BIT_ON_BYTE;
    //countBitOfEnd = count_bit() % COUNT_BIT_ON_BYTE;
    fwrite(&countBitOfEnd, sizeof(char), WRITE_ONE_ELEM, fout);
    fwrite(&diffSymbol, sizeof(usint), WRITE_ONE_ELEM, fout);
    size_t i;
    for(i = 0; i < SIZE_OF_ARRAY; ++i) {
        if(countOfSymbol[i]) {
            char *seqOfBit = codeAndSymbol[i].code;
            usint lengthOfSeqOfBit = (usint)strlen(seqOfBit);
            char tempSymb = codeAndSymbol[i].symb;
            fwrite(&tempSymb, sizeof(char), WRITE_ONE_ELEM, fout);
            fwrite(&lengthOfSeqOfBit, sizeof(usint), WRITE_ONE_ELEM, fout);
            size_t j;
            byte = shift = 0;
            for(j = 0; j < strlen(seqOfBit); ++j) {
                bit = seqOfBit[j] - 48;
                byte |= bit << (7 - shift);
                ++shift;
                if(shift == COUNT_BIT_ON_BYTE) {
                    fwrite(&byte, sizeof(char), WRITE_ONE_ELEM, fout);
                    byte = shift = 0;
                }
            }
            if(shift) {
                fwrite(&byte, sizeof(char), WRITE_ONE_ELEM, fout);
                byte = shift = 0;
            }
        }
    }
    char tempCh;
    shift = byte = 0;
    while((tempCh = fgetc(fin)) != EOF) {
        char *seqOfBit = codeAndSymbol[(size_t)tempCh].code;
        for(i = 0; i < strlen(seqOfBit); ++i) {
            bit = seqOfBit[i] - '0';
            byte = byte | (bit << (7 - shift));
            ++shift;
            if(shift == COUNT_BIT_ON_BYTE) {
                fwrite(&byte, sizeof(char), WRITE_ONE_ELEM, fout);
                byte = shift = 0;
            }
        }
    }
    if(shift) {
        //printf("\n\n ============== %d ============== \n\n", byte);
        fwrite(&byte, sizeof(char), WRITE_ONE_ELEM, fout);
    }
    fclose(fin);
    fclose(fout);
}

static HaffmanNode * tree_for_decifer(HaffmanNode *p) {
    p = (HaffmanNode *)malloc(sizeof(HaffmanNode));
    p->val = 0.F;
    p->left = p->right = NULL;
    find(AVLRoot, code, &AVLSearch);
    if(AVLSearch != NULL) {
        p->symb = AVLSearch->symbol;
        //printf("%-20s %c\n", code, p->symb);
    } else {
        p->symb = '\0';
        code[codeSize++] = '0';
        p->left = tree_for_decifer(p->left);
        --codeSize;
        code[codeSize++] = '1';
        p->right = tree_for_decifer(p->right);
        code[--codeSize] = '\0';
    }
    return p;
}

static void print_symbol(const char byte, const char count, FILE *f) {
    char bit;
    int i;
    for(i = 0; i < count; ++i) {
        bit = byte & (1 << (7 - i));
        if(bit) {
            p = p->right;
        } else {
            p = p->left;
        }
        if(p->symb != '\0') {
            fprintf(f, "%c", p->symb);
            p = HFRoot;
        }
    }
}

void decifer(const char *in, const char *out) {
    FILE *fin, *fout;
    int i;
    char tempSymb, byte, bit, shift, tempByte;
    fileInputName = (char *)malloc(SIZE_OF_PATH_TO_FILE);
    fileOutputName = (char *)malloc(SIZE_OF_PATH_TO_FILE);
    strcpy(fileInputName, in);
    strcpy(fileOutputName, out);
    fin = fopen(fileInputName, "rb");
    if(fin == NULL) {
        perror(in);
        exit(EXIT_FAILURE);
    }
    fout = fopen(fileOutputName, "wt");
    fread(&countBitOfEnd, sizeof(char), READ_ONE_ELEM, fin);
    fread(&diffSymbol, sizeof(usint), READ_ONE_ELEM, fin);
    //printf("\ndiffSymb = %d\n\n", diffSymbol);
    for(i = 0; i < diffSymbol; ++i) {
        char seqOfBit[SIZE_OF_ARRAY];
        memset(seqOfBit, 0, SIZE_OF_ARRAY);
        usint lenOfSeqOfBit;
        fread(&tempSymb, sizeof(char), READ_ONE_ELEM, fin);
        fread(&lenOfSeqOfBit, sizeof(usint), READ_ONE_ELEM, fin);
        fread(&byte, sizeof(char), READ_ONE_ELEM, fin);
        usint j;
        shift = 0;
        for(j = 1; j <= lenOfSeqOfBit / COUNT_BIT_ON_BYTE;) {
            bit = byte & (1 << (7 - shift));
            if(bit) {
                strcat(seqOfBit, "1");
            } else {
                strcat(seqOfBit, "0");
            }
            ++shift;
            if(shift == COUNT_BIT_ON_BYTE) {
                if(lenOfSeqOfBit - j * COUNT_BIT_ON_BYTE > 0) {
                    fread(&byte, sizeof(char), READ_ONE_ELEM, fin);
                }
                shift = 0;
                ++j;
            }
        }
        for(j = 0; j < lenOfSeqOfBit % COUNT_BIT_ON_BYTE; ++j) {
            bit = byte & (1 << (7 - j));
            if(bit) {
                strcat(seqOfBit, "1");
            } else {
                strcat(seqOfBit, "0");
            }
        }
        printf("%-20s %c\n", seqOfBit, tempSymb);
        AVLRoot = insert(AVLRoot, seqOfBit, tempSymb);
    }
    code = (char *)malloc(SIZE_OF_ARRAY);
    memset(code, 0, SIZE_OF_ARRAY);
    codeSize = 0;
    HFRoot = NULL;
    HFRoot = tree_for_decifer(HFRoot);
    p = HFRoot;
    fread(&tempByte, sizeof(char), READ_ONE_ELEM, fin);
    byte = tempByte;
    while(fread(&tempByte, sizeof(char), READ_ONE_ELEM, fin)) {
        print_symbol(byte, COUNT_BIT_ON_BYTE, fout);
        byte = tempByte;
    }
    print_symbol(byte, countBitOfEnd, fout);
    free(code);
    free(fileInputName);
    free(fileOutputName);
    fclose(fin);
    fclose(fout);
}
