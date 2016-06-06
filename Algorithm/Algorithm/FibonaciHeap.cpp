#include "FibonaciHeap.h"

#define FIBONACI_MINUS_INF INT_MIN

FibonaciNode *createFibonaciNode(int value);
void destroyFibonaciNode(FibonaciNode *node);
void linkFibonaciTree(FibonaciNode *p, FibonaciNode *c);
void exchangeFibonaciNode(FibonaciNode *node1, FibonaciNode *node2);
FibonaciNode *insertIntoFibonaciHeap(FibonaciHeap *heap, FibonaciNode *node);
void consolidate(FibonaciHeap *heap);
void cut(FibonaciHeap *heap, FibonaciNode *x, FibonaciNode *y);
void cascandingCut(FibonaciHeap *heap, FibonaciNode *y);

// ��ʼ��һ���յ�쳲�������
FibonaciHeap *initFibonaciHeap()
{
	FibonaciHeap *heap = (FibonaciHeap *)malloc(sizeof(FibonaciHeap));
	heap->min = NULL;
	heap->num = 0;
	return heap;
}

// ��ȡ��С���
int extractMin(FibonaciHeap *heap)
{
	if (heap->min == NULL)	return EMPTY_HEAP;

	int mini = heap->min->value;
	// ����С���ĺ��ӽ����뵽��������
	FibonaciNode *child = heap->min->child;
	if (child != NULL)
	{
		child->left->right = NULL;
		while (child != NULL)
		{
			insertIntoFibonaciHeap(heap, child);
			child = child->right;
		}
	}

	// ɾ����С���
	FibonaciNode *preMin = heap->min;
	heap->min = preMin->right;
	preMin->left->right = preMin->right;
	preMin->right->left = preMin->left;
	free(preMin);

	heap->num--;
	if (heap->num == 0)
	{
		// ��Ϊ��
		heap->min = NULL;
	}
	else
	{
		// �ϲ��������ж�����ͬ�Ľ��
		consolidate(heap);
	}
	
	return mini;
}

// �ϲ��������ж�����ͬ�Ľ��
void consolidate(FibonaciHeap *heap)
{
	int *A = (int *)malloc(sizeof(int)* heap->num);
	for (int i = 0; i < heap->num; i++)	A[i] = NULL;

	FibonaciNode *node = heap->min;
	while (1)
	{
		if (A[node->num] != NULL)
		{
			FibonaciNode *n1 = node;
			FibonaciNode *n2 = (FibonaciNode *)A[node->num];
			if (n1 == n2)	break;	// �ϲ�����
			if (n1->value > n2->value)
			{
				// ����
				exchangeFibonaciNode(n1, n2);
			}
			n2->left->right = n2->right;
			n2->right->left = n2->left;
			A[node->num] = NULL;
			linkFibonaciTree(n1, n2);
		}
		else
		{
			A[node->num] = (int)node;
			node = node->right;
		}
		// ������С���
		if (node->value < heap->min->value)
		{
			heap->min = node;
		}
	}
	free(A);
}

// �����c��Ϊ���ӽ����뵽���p�ĺ���������
void linkFibonaciTree(FibonaciNode *p, FibonaciNode *c)
{
	c->parent = p;
	c->mark = FALSE;
	p->num++;
	if (p->child == NULL)
	{
		p->child = c;
		c->left = c;
		c->right = c;
		return;
	}
	FibonaciNode *child = p->child;
	c->right = child->right;
	c->right->left = c;
	child->right = c;
	c->left = child;
}

// ����������������
void exchangeFibonaciNode(FibonaciNode *node1, FibonaciNode *node2)
{
	int v = node1->value;
	int n = node1->num;
	int m = node1->mark;
	FibonaciNode *c1 = node1->child;
	FibonaciNode *c2 = node2->child;

	node1->value = node2->value;
	node1->num = node2->num;
	node1->mark = node2->mark;
	node1->child = c2;

	node2->value = v;
	node2->num = n;
	node2->mark = m;
	node2->child = c1;

	FibonaciNode *stop = c1;
	do
	{
		c1->parent = node2;
		c1 = c1->right;
	} while (c1 != stop);
	stop = c2;
	do
	{
		c2->parent = node1;
		c2 = c2->right;
	} while (c2 != stop);
}

FibonaciNode *insertIntoFibonaciHeap(FibonaciHeap *heap, int value)
{
	FibonaciNode *node = createFibonaciNode(value);
	return insertIntoFibonaciHeap(heap, node);
}

FibonaciNode *insertIntoFibonaciHeap(FibonaciHeap *heap, FibonaciNode *node)
{
	if (heap->min == NULL)
	{
		heap->min = node;
		node->right = node;
		node->left = node;
	}
	else
	{
		node->right = heap->min->right;
		node->right->left = node;
		heap->min->right = node;
		node->left = heap->min;
		if (node->value < heap->min->value)
		{
			heap->min = node;
		}
	}
	heap->num++;
	return node;
}

// �ؼ��ּ�ֵ
void decreaseKey(FibonaciHeap *heap, FibonaciNode *node, int k)
{
	if (k >= node->value)	return;

	node->value = k;
	FibonaciNode *parent = node->parent;
	if (parent != NULL && node->value < parent->value)
	{
		cut(heap, node, parent);
		cascandingCut(heap, parent);
	}
	if (node->value < heap->min->value)
	{
		heap->min = node;
	}
}

// ɾ��ָ�����
bool deleteFromFibonaciHeap(FibonaciHeap *heap, FibonaciNode *node)
{
	decreaseKey(heap, node, FIBONACI_MINUS_INF);
	return extractMin(heap) == FIBONACI_MINUS_INF;
}

// ��y���г�x����x���õ���������
void cut(FibonaciHeap *heap, FibonaciNode *x, FibonaciNode *y)
{
	x->left->right = x->right;
	x->right->left = x->left;
	if (y->child == x)	y->child = x->right;
	if (y->child == x)	y->child = NULL;

	insertIntoFibonaciHeap(heap, x);
	x->parent = NULL;
	x->mark = NONE;
}

// �ݹ��ж�markΪTRUE�Ľ��
void cascandingCut(FibonaciHeap *heap, FibonaciNode *y)
{
	FibonaciNode *p = y->parent;
	if (p != NULL)
	{
		if (y->mark == FALSE)
		{
			y->mark = TRUE;
		}
		else if (y->mark == TRUE)
		{
			cut(heap, y, p);
			cascandingCut(heap, p);
		}
	}
}

void destroyFibonaciHeap(FibonaciHeap **heap)
{
	destroyFibonaciNode((*heap)->min);
	(*heap)->min = NULL;
	(*heap)->num = 0;
	*heap = NULL;
}

FibonaciNode *createFibonaciNode(int value)
{
	FibonaciNode *node = (FibonaciNode *)malloc(sizeof(FibonaciNode));
	node->child = NULL;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->value = value;
	node->mark = NONE;
	node->num = 0;
	return node;
}

void destroyFibonaciNode(FibonaciNode *node)
{
	node->left->right = NULL;
	while (node != NULL)
	{
		if (node->child != NULL)
		{
			destroyFibonaciNode(node->child);
		}
		FibonaciNode *del = node;
		node = node->right;
		free(del);
	}
}
