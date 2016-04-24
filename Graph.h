
#include <Windows.h> 
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using std::vector;

#pragma once

#define  MAX_VERTEX 100        // 最大顶点个数

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

// 弧节点
typedef struct _tagEdgeNode
{
    int tail;                   // 该弧节点的弧尾所在的index
	int adjvex;                 // 该节点在顶点表中的数组index
	struct _tagEdgeNode* next;  // 下一节点

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

// 顶点节点
typedef struct _tagVertexNode
{
	int data;            // 数据区域
	EdgeNode* firstedge; // 第一条以该顶点为尾的弧

}VertexNode, AdjList[MAX_VERTEX];


// 图的邻接表结构
typedef struct _tagGraphAdjList
{
	AdjList adjlist;  // 所有顶点
	int numVertexes;  // 定点数
	int numEdges;     // 弧数MAX_VERTEX

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

    // 所有弧节点
    EdgeNode    edgeNodes[MAX_VERTEX * ( MAX_VERTEX - 1) ];
};