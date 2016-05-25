#include "BTree.h"

/**
* ����������Ĳ���
*/
LinkKey *initLinkKey()
{
	LinkKey *head = (LinkKey *)malloc(sizeof(LinkKey));
	head->key = INT_MIN;
	head->next = NULL;
	return head;
}

void destroyLinkKey(LinkKey *link)
{
	while (link != NULL)
	{
		LinkKey *next = link->next;
		free(link);
		link = next;
	}
}

int getKey(LinkKey *link, int which)
{
	int i = -1;
	while (i < which && link->next != NULL)
	{
		link = link->next;
		i++;
	}
	if (i == -1)		return INT_MIN;
	return link->key;
}

void insertLinkKey(LinkKey *link, int key)
{
	while (link->next != NULL)
	{
		if (key <= link->next->key)
		{
			break;
		}
		link = link->next;
	}
	LinkKey *node = (LinkKey *)malloc(sizeof(LinkKey));
	node->key = key;
	node->next = link->next;
	link->next = node;
}

bool deleteLinkKey(LinkKey *link, int key)
{
	while (link->next != NULL && key > link->next->key)
	{
		link = link->next;
	}
	if (key == link->next->key)
	{
		link->next = link->next->next;
		return true;
	}
	return false;
}





LinkNode *initLinkNode()
{
	LinkNode *head = (LinkNode *)malloc(sizeof(LinkNode));
	head->next = NULL;
	head->node = NULL;
	return head;
}

void destroyLinkNode(LinkNode *link)
{
	while (link != NULL)
	{
		LinkNode *next = link->next;
		free(link);
		link = next;
	}
}

BNode *getLinkNode(LinkNode *link, int which)
{
	int i = -1;
	while (i < which && link->next != NULL)
	{
		link = link->next;
		i++;
	}
	if (i == -1)		return NULL;
	return (BNode*)(link->node);
}

void insertLinkNode(LinkNode *link, BNode *node, int which)
{
	int i = 0;
	while (i < which && link->next != NULL)
	{
		link = link->next;
		i++;
	}
	LinkNode *linkNode = (LinkNode *)malloc(sizeof(LinkNode));
	linkNode->node = node;
	linkNode->next = link->next;
	link->next = linkNode;
}

bool deleteLinkNode(LinkNode *link, int which)
{
	int i = 0;
	while (link->next != NULL && i < which)
	{
		link = link->next;
		i++;
	}
	if (i == which)
	{
		link->next = link->next->next;
		return true;
	}
	return false;
}




/**
* ������B���Ĳ���
*/

void destroyBNode(BNode *node);
BNode *createBNode();
void splitBNode(BNode *parent, int which);
void moveKey(BNode *parent, int from, int to);
void unionBNodes(BNode *parent, int a, int b);
int getMinKey(BNode *tree);
int getMaxKey(BNode *tree);

BTree *initBTree(int t)
{
	BTree *tree = (BTree *)malloc(sizeof(BTree));
	tree->root = NULL;
	tree->t = t;
	return tree;
}

void destroyBTree(BTree *tree)
{
	destroyBNode(tree->root);
	tree->root = NULL;
}

void destroyBNode(BNode *node)
{
	if (node->leaf == false)
	{
		int i;
		LinkNode *link = node->children->next;
		while (link != NULL)
		{
			destroyBNode((BNode*)(link->node));
			LinkNode *next = link->next;
			free(link);
			link = next;
		}
		free(node->children);
	}
	free(node);
}

BNode *createBNode()
{
	BNode *node = (BNode *)malloc(sizeof(BNode));
	node->parent = NULL;
	node->key = initLinkKey();
	node->children = initLinkNode();
	node->n = 1;
	return node;
}

void insertBTree(BTree *tree, int key)
{
	BNode *root = tree->root;

	// �¸�
	if (root == NULL)
	{
		BNode *newRoot = createBNode();
		newRoot->leaf = true;
		insertLinkKey(newRoot->key, key);
		newRoot->n++;
		tree->root = newRoot;
		return;
	}

	// ���Ѹ�
	if (root->n == tree->t * 2)
	{
		BNode *newRoot = createBNode();
		newRoot->leaf = false;
		newRoot->n = 1;
		insertLinkNode(newRoot->children, root, 0);
		root->parent = newRoot;
		splitBNode(newRoot, 0);
		tree->root = newRoot;
	}

	// Ѱ�Ҳ���·��
	BNode *node = tree->root;
	while (node->leaf == false)
	{
		LinkKey *keys = node->key;
		int i = 0;
		while (keys->next != NULL && key > keys->next->key)
		{
			keys = keys->next;
			i++;
		}
		BNode *next = getLinkNode(node->children, i);
		if (next->n == tree->t * 2)
		{
			splitBNode(node, i);	// ���������
			if (key < getKey(node->key, i))
			{
				node = next;
			}
			else
			{
				node = getLinkNode(node->children, i + 1);
			}
		}
		else
		{
			node = next;
		}
	}

	// ��key����Ҷ�ӽ��
	LinkKey *pKeys = node->key;
	insertLinkKey(pKeys, key);
	node->n++;
}

void splitBNode(BNode *parent, int which)
{
	int i;
	BNode *node = getLinkNode(parent->children, which);
	BNode *right = createBNode();

	// ���÷��ѳ������ҽ��
	right->leaf = node->leaf;
	right->n = node->n / 2;
	for (i = 0; i < right->n; i++)
	{
		if (!right->leaf)		insertLinkNode(right->children, getLinkNode(node->children, right->n + i), i);
		if (i != right->n - 1)	insertLinkKey(right->key, getKey(node->key, right->n + i));
	}
	if (!right->leaf)		insertLinkNode(right->children, getLinkNode(node->children, right->n + i), i);
	right->parent = parent;

	// ԭ����node��Ϊ����
	node->n -= right->n;
	i = 0;
	LinkKey *keys = node->key;
	while (i < node->n - 1)
	{
		keys = keys->next;
		i++;
	}
	int key = keys->next->key;
	keys->next = NULL;	// �Ͽ��뱻���ѳ�ȥ�Ĺؼ��ֵ���ϵ

	// ��key�ͷ��Ѻ�Ľ�����parent���
	LinkKey *pKeys = parent->key;
	LinkNode *pNodes = parent->children;
	insertLinkKey(pKeys, key);
	insertLinkNode(pNodes, right, which + 1);
	parent->n++;
}

void deleteFromBTree(BTree *tree, int key)
{
	BNode *node = tree->root;
	if (node == NULL)	return;

	while (!node->leaf)
	{
		LinkKey *keys = node->key;
		// Ѱ��ƥ��key��node
		int which = 0;
		while (keys->next != NULL && key > keys->next->key)
		{
			which++;
			keys = keys->next;
		}

		BNode *next = getLinkNode(node->children, which);
		if (key == keys->next->key)
		{
			// ��keyƥ�䵽��ǰ���
			if (next->n > tree->t)
			{
				keys->next->key = getMaxKey(next);
				key = keys->next->key;	// ��next��㿪ʼɾ����key
			}
			else
			{
				next = getLinkNode(node->children, which + 1);
				if (next->n > tree->t)
				{
					keys->next->key = getMinKey(next);
					key = keys->next->key;	// ��next��㿪ʼɾ����key
				}
				else
				{
					unionBNodes(node, which, which + 1);
					next = getLinkNode(node->children, which);
				}
			}
		}
		else
		{
			// ��key�������У��������Ķ�������Ҫ��
			if (next->n <= tree->t)
			{
				BNode *left = getLinkNode(node->children, which - 1);
				BNode *right = getLinkNode(node->children, which + 1);
				if (left != NULL && left->n > tree->t)
				{
					moveKey(node, which - 1, which);		// ���������ƶ�key
				}
				else if (right != NULL && right->n > tree->t)
				{
					moveKey(node, which + 1, which);	// ���������ƶ�key
				}
				else
				{
					unionBNodes(node, which, which + 1);	// ���������ϲ�
				}
			}
		}

		node = next;
	}

	LinkKey *keys = node->key;
	if (deleteLinkKey(keys, key))
	{
		node->n--;
	}
}

void moveKey(BNode *parent, int from, int to)
{
	BNode *f = getLinkNode(parent->children, from);
	BNode *t = getLinkNode(parent->children, to);
	if (from < to)
	{
		LinkKey *pLink = parent->key;
		int i = -1;
		while (i < from && pLink->next != NULL)
		{
			pLink = pLink->next;
			i++;
		}
		insertLinkKey(t->key, pLink->key);

		LinkKey *fLink = f->key;
		while (fLink->next->next != NULL)	fLink = fLink->next;
		pLink->key = fLink->next->key;
		free(fLink->next);
		fLink->next = NULL;
		f->n--;

		LinkNode *fNodes = f->children;
		while (fNodes->next->next != NULL)	fNodes = fNodes->next;
		insertLinkNode(t->children, (BNode*)(fNodes->next->node), 0);
		free(fNodes->next);
		fNodes->next = NULL;
	}
	else if (from > to)
	{
		LinkKey *pLink = parent->key;
		int i = -1;
		while (i < to && pLink->next != NULL)
		{
			pLink = pLink->next;
			i++;
		}
		insertLinkKey(t->key, pLink->key);
		t->n++;

		LinkKey *fLink = f->key;
		pLink->key = fLink->next->key;
		LinkKey *nextKey = fLink->next;
		fLink->next = fLink->next->next;
		free(nextKey);
		f->n--;

		LinkNode *fNodes = f->children;
		insertLinkNode(t->children, (BNode*)(fNodes->next->node), t->n);
		LinkNode *nextNode = fNodes->next;
		fNodes->next = fNodes->next->next;
		free(nextNode);
	}
}

void unionBNodes(BNode *parent, int a, int b)
{
	int middleKey = getKey(parent->key, a);
	BNode *aNode = getLinkNode(parent->children, a);
	BNode *bNode = getLinkNode(parent->children, b);
	LinkKey *aKey = aNode->key;
	LinkKey *bKey = bNode->key;
	LinkNode *aNodes = aNode->children;
	LinkNode *bNodes = bNode->children;

	while (aKey->next != NULL)	aKey = aKey->next;
	LinkKey *middle = (LinkKey *)malloc(sizeof(LinkKey));
	middle->key = middleKey;
	aKey->next = middle;
	middle->next = bKey->next;
	free(bKey);

	while (aNodes->next != NULL)		aNodes = aNodes->next;
	aNodes->next = bNodes->next;
	free(bNodes);

	aNode->n += bNode->n;

	deleteLinkKey(parent->key, middleKey);
	deleteLinkNode(parent->children, b);
	parent->n--;
}

int getMinKey(BNode *tree)
{
	while (!tree->leaf)
	{
		LinkNode *link = tree->children;
		tree = (BNode *)(link->next->node);
	}
	return tree->key->next->key;
}

int getMaxKey(BNode *tree)
{
	while (!tree->leaf)
	{
		LinkNode *link = tree->children;
		while (link->next != NULL)	link = link->next;
		tree = (BNode *)(link->next->node);
	}
	LinkKey *key = tree->key;
	while (key->next != NULL)	key = key->next;
	return key->key;
}
