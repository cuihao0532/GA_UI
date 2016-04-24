
#include <Windows.h> 
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using std::vector;

#pragma once

#define  MAX_VERTEX 100        // ��󶥵����

#ifndef  NULL
#define  NULL 0
#endif

#define  INI_SUMMARY          TEXT("summary")
#define  INI_SUMMARY_POINTNUM TEXT("pointnum")
#define  INI_SUMMARY_ARCNUM   TEXT("arcnum")


#define  INI_POINTS           TEXT("points")
#define  INI_POINTS_PREFIX    TEXT("point") 

#define  INI_ARCS             TEXT("arcs")
#define  INI_ARCS_PREFIX      TEXT("arc")

// ���ڵ�
typedef struct _tagEdgeNode
{
    int tail;                   // �û��ڵ�Ļ�β���ڵ�index
	int adjvex;                 // �ýڵ��ڶ�����е�����index
	struct _tagEdgeNode* next;  // ��һ�ڵ�

	_tagEdgeNode& operator=(const _tagEdgeNode& edge)
	{
		if ( this != &edge )
		{
			this->tail = edge.tail;
			this->adjvex = edge.adjvex;
			this->next = edge.next;
		}

		return ( *this );
	}
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
	bool CreateFromFile(LPCTSTR lpszFileName);

public:
	void CreateGraph();

//protected: 
	GraphAdjList graph;

    // ���л��ڵ�
    EdgeNode    edgeNodes[MAX_VERTEX * ( MAX_VERTEX - 1) ];
};