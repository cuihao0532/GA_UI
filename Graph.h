
#pragma once

#define  MAX_VERTEX 100        // 最大顶点个数

#ifndef  NULL
#define  NULL 0
#endif

// 弧节点
typedef struct _tagEdgeNode
{
    int tail;                   // 该弧节点的弧尾所在的index
	int adjvex;                 // 该节点在顶点表中的数组index
	struct _tagEdgeNode* next;  // 下一节点
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

public:
	void CreateGraph();

//protected: 
	GraphAdjList graph;

    // 所有弧节点
    EdgeNode    edgeNodes[MAX_VERTEX * ( MAX_VERTEX - 1) ];
};