
#include "Graph.h"

CGraph::CGraph()
{
}

CGraph::~CGraph()
{

}

void CGraph::CreateGraph()
{
	// »¡
    int nIndex = 0;

	// v0->v3
	static EdgeNode n_0_3;
    n_0_3.tail   = 0;
	n_0_3.adjvex = 3;
	n_0_3.next   = NULL; 
    edgeNodes[nIndex++] = n_0_3;

	// v1->v2
	static EdgeNode n_1_2;
    n_1_2.tail   = 1;
	n_1_2.adjvex = 2;
	n_1_2.next   = NULL;
    edgeNodes[nIndex++] = n_1_2;

	// v1->v0
	static EdgeNode n_1_0;
    n_1_0.tail   = 1;
	n_1_0.adjvex = 0;
	n_1_0.next   = &n_1_2;
    edgeNodes[nIndex++] = n_1_0;

	// v2->v1
	static EdgeNode n_2_1;
    n_2_1.tail   = 2;
	n_2_1.adjvex = 1;
	n_2_1.next   = NULL;
    edgeNodes[nIndex++] = n_2_1;

	// v2->v0
	static EdgeNode n_2_0;
    n_2_0.tail   = 2;
	n_2_0.adjvex = 0;
	n_2_0.next   = &n_2_1;
    edgeNodes[nIndex++] = n_2_0;


	graph.numVertexes = 4;
	graph.numEdges    = 5;

	// v0
	graph.adjlist[0].data = 0;
	graph.adjlist[0].firstedge = &n_0_3;

	// v1
	graph.adjlist[1].data = 1;
	graph.adjlist[1].firstedge = &n_1_0;

	// v2
	graph.adjlist[2].data = 2;
	graph.adjlist[2].firstedge = &n_2_0;

	// v4
	graph.adjlist[3].data = 3;
	graph.adjlist[3].firstedge = NULL;

}
