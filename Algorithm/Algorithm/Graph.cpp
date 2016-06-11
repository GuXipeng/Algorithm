#include "Graph.h"
#include "Queue.h"
#include "Stack.h"
#include <stdio.h>

// ���·�����Ҫ����ͼʱ��Ҫ��ͼ�Ķ����Ŵ�1��ʼ������

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

	// ��ʼ��
	for (int i = 0; i < vertexNum; i++)
	{
		(vs + i)->weight = INF;
		(vs + i)->p = NULL;
		(vs + i)->f = 0;
	}
	(vs + root - 1)->weight = 0;

	while (1)
	{
		int no = min(vs, vertexNum);	// �ҵ���̶���
		if (no == 0)
		{
			break;
		}
		// ������̶������ڶ��㵽��С���ľ���
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
* ���·��
*/

void initialize(Graph *g, int s)
{
	Vertex *vs = g->vertex;
	for (int i = 0; i < g->VertexNum; i++)
	{
		Vertex *v = vs + i;
		v->p = NULL;
		v->weight = INF;
		v->f = 0;
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
* Ѱ�Ҵӱ��Ϊs�Ľ�㿪ʼ�Ĺؼ�·�����÷�������ʱ��ͼ��Ȩ���ڱ����Ҿ�Ϊ��ֵ
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

/**
* Dijkstra�㷨��Ҫ�����бߵ�Ȩ�ؾ�Ϊ�Ǹ�ֵ�����ı�Ŵ�1��ʼ
*/
void dijkstra(Graph *g, int **w, int s)
{
	initialize(g, s);

	Vertex *vs = g->vertex;
	GNode *linkTable = g->LinkTable;
	for (int i = 1; i < g->VertexNum; i++)
	{
		int min = INT_MAX;
		int number = 0;
		// �ҵ�Ŀǰ����s��̵Ķ��㣬�ö���������̾������
		for (int j = 0; j < g->VertexNum; j++)
		{
			if (min > (vs + j)->weight && (vs + j)->f == 0)
			{
				min = (vs + j)->weight;
				number = j + 1;
			}
		}
		if (number == 0)	return;
		(vs + number - 1)->f = 1;
		// ���뵽������number�����Ķ��������ɳڸ��²���
		GNode *node = (linkTable + number - 1)->next;
		Vertex *u = vs + number - 1;
		while (node != NULL)
		{
			Vertex *v = vs + node->number - 1;
			int weight = *((int*)w + (number - 1)*g->VertexNum + node->number - 1);
			relax(u, v, weight);
			node = node->next;
		}
	}
}

/**
* ����ǰ����ͼ��ӡi��j��·�������붥���Ŵ�1��ʼ����������Ŵ�1��ʼ
*/
void printIJPath(int **prior, int vertexNum, int i, int j)
{
	i--; j--;
	printf("%d", j + 1);
	int k = *((int*)prior + i*vertexNum + j);
	while (k != -1)
	{
		printf(" <- %d", k + 1);
		k = *((int*)prior + i*vertexNum + k);
	}
	printf("\n");
}

/**
* Floyd Ѱ�ҽ��Ե����·���㷨
* w Ȩ��ͼ
* vertexNum �������
* lenMatrix �����������·�����ȴ洢���󣨶�ά��
* priorMatrix ǰ����ͼ����ά����·��<i, ..., j>�ص�j��ǰһ������k�洢��priorMatrix[i][j]��
*/
void Floyd_WallShall(int **w, int vertexNum, int **lenMatrix, int **priorMatrix)
{
	// ��ʼ��
	for (int i = 0; i < vertexNum; i++)
	{
		for (int j = 0; j < vertexNum; j++)
		{
			*((int*)lenMatrix + i*vertexNum + j) = *((int*)w + i*vertexNum + j);
			if (*((int*)w + i*vertexNum + j) != INF && i != j)
			{
				*((int*)priorMatrix + i*vertexNum + j) = i;
			}
			else
			{
				*((int*)priorMatrix + i*vertexNum + j) = -1;
			}
		}
	}

	// Floyd�㷨
	for (int k = 0; k < vertexNum; k++)
	{
		for (int i = 0; i < vertexNum; i++)
		{
			for (int j = 0; j < vertexNum; j++)
			{
				int Dij = *((int*)lenMatrix + i*vertexNum + j);
				int Dik = *((int*)lenMatrix + i*vertexNum + k);
				int Dkj = *((int*)lenMatrix + k*vertexNum + j);
				if (Dik != INF && Dkj != INF && Dij > Dik + Dkj)
				{
					*((int*)lenMatrix + i*vertexNum + j) = Dik + Dkj;
					*((int*)priorMatrix + i*vertexNum + j) = *((int*)priorMatrix + k*vertexNum + j);
				}
			}
		}
	}
}

/**
* �жϽ����Ƿ���ڴ��ݱհ�
*/
void transitiveClosure(int **w, int vertexNum, int **result)
{
	// ��ʼ��
	for (int i = 0; i < vertexNum; i++)
	{
		for (int j = 0; j < vertexNum; j++)
		{
			if (*((int*)w + i*vertexNum + j) != INF || i == j)
			{
				*((int*)result + i*vertexNum + j) = 1;
			}
			else
			{
				*((int*)result + i*vertexNum + j) = 0;
			}
		}
	}

	// Ӧ��Floyd�㷨�ж���ͨ��
	for (int k = 0; k < vertexNum; k++)
	{
		for (int i = 0; i < vertexNum; i++)
		{
			for (int j = 0; j < vertexNum; j++)
			{
				int Dij = *((int*)result + i*vertexNum + j);
				int Dik = *((int*)result + i*vertexNum + k);
				int Dkj = *((int*)result + k*vertexNum + j);
				*((int*)result + i*vertexNum + j) = Dij | (Dik & Dkj);
			}
		}
	}
}

void addSinglePoint(Graph *g, int s)
{
	Vertex *vs = g->vertex;
	GNode *linkTable = g->LinkTable;
	int vertexNum = g->VertexNum;

	Vertex *newVs = (Vertex *)malloc(sizeof(Vertex)* (vertexNum + 1));
	GNode *newLinkTable = (GNode *)malloc(sizeof(GNode)* (vertexNum + 1));
	int i;
	for (i = 0; i < vertexNum; i++)
	{
		newVs[i] = vs[i];
		newLinkTable[i] = linkTable[i];
	}
	newVs[i].number = s;
	GNode *node = (GNode *)malloc(sizeof(GNode));
	node->number = s;
	node->next = NULL;
	GNode *del = node;
	for (i = 0; i < vertexNum; i++)
	{
		GNode *next = (GNode *)malloc(sizeof(GNode));
		next->number = i + 1;
		next->next = NULL;
		node->next = next;
		node = next;
	}
	newLinkTable[i] = *del;
	free(del);

	g->LinkTable = newLinkTable;
	g->vertex = newVs;
	g->VertexNum++;
}

/**
* Johnson�㷨����������·��
*/
bool Johnson(Graph *g, int **w, int **lenMatrix, int **priorMatrix)
{
	// save
	Vertex *vs = g->vertex;
	GNode *linkTable = g->LinkTable;
	int vertexNum = g->VertexNum;

	// add s point
	addSinglePoint(g, g->VertexNum + 1);
	int *newW = (int *)malloc(sizeof(int) * g->VertexNum * g->VertexNum);
	for (int i = 0; i < g->VertexNum; i++)
	{
		for (int j = 0; j < g->VertexNum; j++)
		{
			if (i == g->VertexNum - 1 || j == g->VertexNum - 1)
			{
				*(newW + i*g->VertexNum + j) = 0;
			}
			else
			{
				*(newW + i * g->VertexNum + j) = *((int *)w + i * vertexNum + j);
			}
		}
	}

	// Bellman-Ford�㷨
	if (Bellman_Ford(g, (int **)newW, g->VertexNum) == false)
	{
		return false;
	}

	Vertex *newVs = g->vertex;
	// recover
	g->LinkTable = linkTable;
	g->vertex = vs;
	g->VertexNum--;
	// ���¸���Ȩֵ
	for (int i = 0; i < vertexNum; i++)
	{
		GNode *node = linkTable + i;
		Vertex *u = newVs + i;
		node = node->next;
		while (node != NULL)
		{
			Vertex *v = newVs + node->number - 1;
			if (*((int*)w + i*vertexNum + node->number - 1) != INF)
			{
				*((int*)w + i*vertexNum + node->number - 1) += u->weight - v->weight;
			}
			node = node->next;
		}
	}

	// ������ÿ���������Dijkstra�㷨���������̾���
	for (int i = 0; i < vertexNum; i++)
	{
		dijkstra(g, w, i+1);
		Vertex *u = newVs + i;
		for (int j = 0; j < vertexNum; j++)
		{
			Vertex *v = newVs + j;
			*((int*)lenMatrix + i*vertexNum + j) = (g->vertex + j)->weight + v->weight - u->weight;
			if ((g->vertex + j)->p != NULL)
			{
				*((int*)priorMatrix + i*vertexNum + j) = (g->vertex + j)->p->number - 1;
			}
			else
			{
				*((int*)priorMatrix + i*vertexNum + j) = -1;
			}
		}
	}
	return true;
}





/**
* �����
*/

// ����д�����
void calculateENet(int **c, int vertexNum, int **f, int **e)
{
	for (int i = 0; i < vertexNum; i++)
	{
		for (int j = 0; j < vertexNum; j++)
		{
			int a = *((int*)c + i*vertexNum + j);
			if (a != 0)
			{
				*((int*)e + i*vertexNum + j) = a - *((int*)f + i*vertexNum + j);
				*((int*)e + j*vertexNum + i) = *((int*)f + i*vertexNum + j);
			}
			else
			{
				*((int*)e + i*vertexNum + j) = 0;
			}
		}
	}
}

// Ѱ������·��
int findRoute(int **e, int vertexNum, int *priorMatrix, int s,int t)
{
	s--; t--;
	int *d = (int *)malloc(sizeof(int)*vertexNum);
	// initialize
	for (int i = 0; i < vertexNum; i++)
	{
		d[i] = 0;
		priorMatrix[i] = -1;
	}
	d[s] = 1;
	// �����ñ�<i,j>���ɳڲ�������<s,...,j>����Ϊ<s,...,i,j>
	for (int k = 0; k < vertexNum; k++)
	{
		for (int i = 0; i < vertexNum; i++)
		{
			for (int j = 0; j < vertexNum; j++)
			{
				if (d[j] == 0)
				{
					d[j] |= (d[i] & (*((int*)e + i*vertexNum + j) > 0));
					if (d[j] == 1)
					{
						priorMatrix[j] = i;
					}
				}
			}
		}
	}
	if (d[t] == 0)	return 0;

	int min = INT_MAX;
	int pre = priorMatrix[t];
	while (pre != -1)
	{
		if (min > *((int*)e + pre*vertexNum + t))
		{
			min = *((int*)e + pre*vertexNum + t);
		}
		t = pre;
		pre = priorMatrix[t];
	}
	return min;
}

/**
* Ford-Fulkerson������һ��ʵ��
* @param c ��ά���󣬼�¼ÿ���ߵ�����
* @param vertexNum ������������������յ�
* @param s ����ţ���Ŵ�1��ʼ
* @param t �յ���
* @param f �����������󣬶�ά���󣬼�¼ÿ���ߵ�����
*/
void Ford_Fulkerson(int **c, int vertexNum, int s, int t, int **f)
{
	int *e = (int *)malloc(sizeof(int)*vertexNum*vertexNum);	// �д�����
	int *priorMatrix = (int *)malloc(sizeof(int)*vertexNum);	// ����·����ǰ����ͼ

	// initialize
	for (int i = 0; i < vertexNum;i++)
	{
		for (int j = 0; j < vertexNum; j++)
		{
			*(f + i*vertexNum + j) = 0;
		}
	}

	while (1)
	{
		calculateENet(c, vertexNum, (int **)f, (int **)e);	// ����д�����
		int min;
		if ((min = findRoute((int **)e, vertexNum, priorMatrix, s, t)) == 0)	// Ѱ������·��������С��ֵ
		{
			break;
		}
		int pre = priorMatrix[t - 1];
		int next = t - 1;
		while (pre != -1)		// ������·������������
		{
			if (*((int*)c + pre * vertexNum + next) != 0)
			{
				*((int*)f + pre * vertexNum + next) += min;
			}
			else
			{
				*((int*)f + next * vertexNum + pre) -= min;
			}
			next = pre;
			pre = priorMatrix[pre];
		}
	}
}
