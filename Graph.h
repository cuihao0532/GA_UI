
#pragma once

#define  MAX_VERTEX 100        // ��󶥵����

#ifndef  NULL
#define  NULL 0
#endif

// ���ڵ�
typedef struct _tagEdgeNode
{
    int tail;                   // �û��ڵ�Ļ�β���ڵ�index
	int adjvex;                 // �ýڵ��ڶ�����е�����index
	struct _tagEdgeNode* next;  // ��һ�ڵ�
}EdgeNode, *PEdgeNode;

// ����ڵ�
typedef struct _tagVertexNode
{
	int data;            // ��������
	EdgeNode* firstedge; // ��һ���Ըö���Ϊβ�Ļ�

}VertexNode, AdjList[MAX_VERTEX];


// ͼ���ڽӱ�ṹ
typedef struct _tagGraphAdjList
{
	AdjList adjlist;  // ���ж���
	int numVertexes;  // ������
	int numEdges;     // ����MAX_VERTEX

}GraphAdjList, *PGraphAdjList;


class CGraph
{
public:
	CGraph();
	virtual ~CGraph();

public:
	void CreateGraph();

//protected: 
	GraphAdjList graph;

    // ���л��ڵ�
    EdgeNode    edgeNodes[MAX_VERTEX * ( MAX_VERTEX - 1) ];
};