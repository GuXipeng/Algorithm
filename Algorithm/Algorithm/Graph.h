#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdlib.h>

#define INF INT_MAX

typedef enum VertexColor
{
	Vertex_WHITE = 0,	// δ��������
	Vertex_BLACK = 1,	// �ӽ�㶼���������
	Vertex_GRAY = 2	// �ӽ�����ڱ�����
} VertexColor;

typedef struct GNode
{
	int number;	// ������
	struct GNode *next;
} GNode;

typedef struct Vertex
{
	int number;
	int weight;		// ��(p, v)��Ȩ�أ�������С�������м�¼�ö��㵽����������С���룻�ڼ������·��ʱΪ�ý�㵽Դ��ľ���
	int f;			// ������������б����������Ĵ���/����; ��prim�㷨�б�ʾ�ý���Ƿ��ѱ�������С��������
	VertexColor color;	// �������̱������״̬
	struct Vertex *p;
} Vertex;

typedef struct Graph
{
	GNode *LinkTable;
	Vertex *vertex;
	int VertexNum;
} Graph;

void searchByWidthFirst(Graph *g, int start);
void searchByDepthFirst(Graph *g);
void printPath(Graph *g, int vertex);
void topologySort(Graph *g, int **order, int *n);

void prim(Graph *g, int **w, int root);

bool Bellman_Ford(Graph *g, int **w, int s);
void dagShortestPaths(Graph *g, int **w, int s);
int findKeyRoute(Graph *g, int **w, int s);

#endif