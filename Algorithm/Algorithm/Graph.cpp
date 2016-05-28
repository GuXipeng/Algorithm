#include "Graph.h"
#include "Queue.h"
#include "Stack.h"
#include <stdio.h>

/**
* �������������Ҫ������ͼg�Ľ���Ŵ�1��ʼ
*/
void searchByWidthFirst(Graph *g, int start)
{
	int VertexNum = g->VertexNum;
	Queue *q = initQueue(VertexNum);
	Vertex *vs = g->vertex;
	GNode *linkTable = g->LinkTable;
	Vertex *s = vs + start - 1;

	for (int i = 0; i < VertexNum; i++)
	{
		Vertex *v = vs + i;
		v->color = Vertex_WHITE;
	}

	s->color = Vertex_GRAY;
	s->p = NULL;
	enqueue(&q, s->number);
	while (!isEmpty(q))
	{
		int number = dequeue(&q);
		Vertex *u = vs + number - 1;
		GNode *links = linkTable + number - 1;
		links = links->next;
		while (links != NULL)
		{
			Vertex *v = vs + links->number - 1;
			if (v->color == Vertex_WHITE)
			{
				v->color = Vertex_GRAY;
				v->p = u;
				enqueue(&q, links->number);
			}
			links = links->next;
		}
		u->color = Vertex_BLACK;
	}
}

/**
* �������������Ҫ������ͼg�Ľ���Ŵ�1��ʼ
*/
void searchByDepthFirst(Graph *g)
{
	int VertexNum = g->VertexNum;
	Stack *stack = initStack();
	Vertex *vs = g->vertex;
	GNode *linkTable = g->LinkTable;
	int order = 0;

	for (int i = 0; i < VertexNum; i++)
	{
		Vertex *v = vs + i;
		v->color = Vertex_WHITE;
		v->p = NULL;
		push(&stack, v->number);
	}

	while (!isEmpty(stack))
	{
		int number = pop(&stack);
		Vertex *u = vs + number - 1;
		if (u->color == Vertex_WHITE) 
		{
			// ��ʼ�����ý����ӽ��
			u->color = Vertex_GRAY;
			push(&stack, number);
		}
		else if (u->color == Vertex_GRAY)
		{
			// �ý����ӽ���Ѿ�����������
			u->color = Vertex_BLACK;
			u->f = order++;
			continue;
		}
		else
		{
			continue;
		}
		GNode *links = linkTable + number - 1;
		links = links->next;
		while (links != NULL)
		{
			// չ���ӽ�㲢��ջ
			Vertex *v = vs + links->number - 1;
			if (v->color == Vertex_WHITE)
			{
				v->p = u;
				push(&stack, links->number);
			}
			links = links->next;
		}
	}
}

// Ҫ������ͼ�Ľ���Ŵ�1��ʼ
void printPath(Graph *g, int vertex)
{
	Vertex *vs = g->vertex;
	Vertex *v = vs + vertex - 1;
	
	while (v != NULL)
	{
		printf("%d ", v->number);
		v = v->p;
	}
}

/**
* �����޻�ͼ����������
*/
void topologySort(Graph *g, int **order, int *n)
{
	searchByDepthFirst(g);
	*n = g->VertexNum;
	*order = (int *)malloc(sizeof(int) * *n);
	for (int i = 0; i < *n; i++)
	{
		(*order)[*n - 1 - g->vertex[i].f] = i + 1;
	}
}



/**
* ��С������
*/

int min(Vertex *vs, int num);

// prim�㷨������ͼg�Ľ���Ŵ�1��ʼ
void prim(Graph *g, int **w, int root)
{
	Vertex *vs = g->vertex;
	GNode *links = g->LinkTable;
	int vertexNum = g->VertexNum;

	for (int i = 0; i < vertexNum; i++)
	{
		(vs + i)->weight = INF;
		(vs + i)->p = NULL;
		(vs + i)->f = 0;
	}
	(vs + root - 1)->weight = 0;

	while (1)
	{
		int no = min(vs, vertexNum);
		if (no == 0)
		{
			break;
		}
		Vertex *u = vs + no - 1;
		u->f = 1;
		GNode *link = links + no - 1;
		link = link->next;
		while (link != NULL)
		{
			int weight = *((int *)w + (no - 1)*vertexNum + link->number - 1);
			Vertex *v = vs + link->number - 1;
			if (v->f == 0 && weight < v->weight)
			{
				v->weight = weight;
				v->p = u;
			}
			link = link->next;
		}
	}
}

int min(Vertex *vs, int num)
{
	int min = INT_MAX;
	int m = -1;
	for (int i = 0; i < num; i++)
	{
		if ((vs + i)->f == 0 && (vs + i)->weight < min)
		{
			min = (vs + i)->weight;
			m = i;
		}
	}
	return m + 1;
}





/**
* ��Դ���·��
*/

void initialize(Graph *g, int s)
{
	Vertex *vs = g->vertex;
	for (int i = 0; i < g->VertexNum; i++)
	{
		Vertex *v = vs + i;
		v->p = NULL;
		v->weight = INF;
	}
	(vs + s - 1)->weight = 0;
}

// �ɳڲ��������<s, ..., v>�ľ����Ƿ��<s, ..., u, v>���������<s, ..., v>Ϊ<s, ..., u, v>
void relax(Vertex *u, Vertex *v, int w)
{
	if (u->weight == INF || w == INF)	return;
	if (v->weight > u->weight + w)
	{
		v->weight = u->weight + w;
		v->p = u;
	}
}

/**
* Bellman Ford ��Դ���·���㷨
* @return true û�и���·�� false �и���·�����·������ʧ��
*/
bool Bellman_Ford(Graph *g, int **w, int s)
{
	initialize(g, s);

	GNode *linkTable = g->LinkTable;
	for (int i = 1; i < g->VertexNum; i++)
	{
		// �������߼��뵽���е���С·��ͼ�У�����Ƿ��и���·��
		for (int j = 0; j < g->VertexNum; j++)
		{
			GNode *node = (linkTable + j)->next;
			Vertex *u = g->vertex + j;
			while (node != NULL)
			{
				Vertex *v = g->vertex + node->number - 1;
				int weight = *((int*)w + j * g->VertexNum + node->number - 1);
				relax(u, v, weight);
				node = node->next;
			}
		}
	}

	// ͨ������Ƿ��Ѵﵽ���·��������Ƿ���ڸ���·
	for (int j = 0; j < g->VertexNum; j++)
	{
		GNode *node = (linkTable + j)->next;
		Vertex *u = g->vertex + j;
		while (node != NULL)
		{
			Vertex *v = g->vertex + node->number - 1;
			int weight = *((int*)w + j * g->VertexNum + node->number - 1);
			if (v->weight > u->weight + weight)
			{
				return false;
			}
			node = node->next;
		}
	}
	return true;
}

/**
* �����޻�ͼ�ĵ�Դ���·�����������������˳������ɳڲ���
*/
void dagShortestPaths(Graph *g, int **w, int s)
{
	int *order, n;
	GNode *linkTable = g->LinkTable;
	Vertex *vs = g->vertex;

	topologySort(g, &order, &n);

	initialize(g, s);

	for (int i = 0; i < n; i++)
	{
		int number = order[i];
		GNode *node = linkTable + number - 1;
		node = node->next;
		Vertex *u = vs + number - 1;
		while (node != NULL)
		{
			Vertex *v = vs + node->number - 1;
			int weight = *((int*)w + (number - 1) * g->VertexNum + node->number - 1);
			relax(u, v, weight);
			node = node->next;
		}
	}
}

/**
* Ѱ�Ҵӱ��Ϊs�Ľ�㿪ʼ�Ĺؼ�·�����÷�������ʱ��ͼ��Ȩ��λ�ڽ���϶����Ǳ��ϣ�����Ȩ�ؾ�Ϊ��ֵ
* @return �ؼ�·�������һ�����ı�ţ����Ծݴ˵���printPath������ӡ�ؼ�·��
*/
int findKeyRoute(Graph *g, int **w, int s)
{
	for (int i = 0; i < g->VertexNum; i++)
	{
		for (int j = 0; j < g->VertexNum; j++)
		{
			*((int*)w + i*g->VertexNum + j) *= -1;
		}
	}

	dagShortestPaths(g, w, s);

	Vertex *vs = g->vertex;
	int m = 0;
	int min = INT_MAX;
	for (int i = 0; i < g->VertexNum; i++)
	{
		if (min > (vs + i)->weight)
		{
			m = i;
			min = (vs + i)->weight;
		}
	}
	return m + 1;
}
