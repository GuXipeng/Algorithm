#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdlib.h>

typedef enum VertexColor
{
	Vertex_WHITE = 0,	// δ��������
	Vertex_BLACK = 1,	// �ӽ�㶼���������
	Vertex_GRAY = 2	// �ӽ�����ڱ�����
} VertexColor;

typedef struct GNode
{
	int number;
	struct GNode *next;
} GNode;

typedef struct Vertex
{
	int number;
	int weight;		// ��(p, v)��Ȩ��
	int f;			// ������������б����������Ĵ���/����
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

#endif