#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>

typedef enum RBNodeColor
{
	RED = 0,
	BLACK = 1
} RBNodeColor;

typedef struct Node
{
	int value;
	int size;
	int height;	//�ý������ߣ�����AVL��
	int color;	// �����ɫ�����ں��ɫ
	char name;	// ������ƣ�����HuffmanTree
	struct Node *parent;
	struct Node *left;
	struct Node *right;
} Node;

typedef struct Tree
{
	Node *root;
	Node *nil;	// �ڱ������ں����
} Tree;

/**
* һ�������
*/

Tree* initTree();
void destroyTree(Tree *tree);
int getSize(Tree *tree);
void inOrder(Tree *tree, int **order, int *num);


/**
* ����������
*/
Node* minimum(Tree *tree);
Node* maximum(Tree *tree);
void insertBinarySearchTree(Tree *tree, int value);
void deleteFromBinarySearchTree(Tree *tree, Node *node);
void deleteFromBinarySearchTree(Tree *tree, int value);




/**
* �����
*/

Tree* initRBTree();
void destroyRBTree(Tree *tree);
void insertRBTree(Tree *tree, int value);
void deleteFromRBTree(Tree *tree, Node *node);
void deleteFromRBTree(Tree *tree, int value);
void inOrderRBTree(Tree *tree, int **order, int *num);



/*
* AVL��
*/

Tree* initAVLTree();
void destroyAVLTree(Tree *tree);
void insertAVLTree(Tree *tree, int value);
void deleteFromAVLTree(Tree *tree, Node *node);
void deleteFromAVLTree(Tree *tree, int value);




/**
* Huffman Code
*/

Tree* HuffmanTree(char *collection, int *frequency, int num);
char HuffmanDecode(Tree *tree, char *code);



#endif