#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdlib.h>

typedef struct LinkKey
{
	int key;
	struct LinkKey *next;
} LinkKey;

typedef struct LinkNode
{
	void *node;
	struct LinkNode *next;
} LinkNode;

typedef struct BNode
{
	int n;	// ���Ӹ�����so key����Ϊn-1
	bool leaf;
	LinkKey *key;
	LinkNode *children;
	struct BNode *parent;
} BNode;

typedef struct BTree
{
	BNode *root;
	int t;	// B������С����
} Btree;



BTree *initBTree(int t);
void destroyBTree(BTree *tree);
void insertBTree(BTree *tree, int key);
void deleteFromBTree(BTree *tree, int key);

#endif