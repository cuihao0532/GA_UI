 
#include "stdafx.h"
#include "Graph.h"

CGraph::CGraph()
{
}

CGraph::~CGraph()
{

}

void CGraph::CreateGraph()
{
	// 弧
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

bool CGraph::CreateFromFile(LPCTSTR lpszFileName)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(lpszFileName);
	if ( !bFind ) return false;

	// 读取点数和弧数
	int nPointNum = ::GetPrivateProfileInt(INI_SUMMARY, INI_SUMMARY_POINTNUM, 0, lpszFileName);
	int nArcNum   = ::GetPrivateProfileInt(INI_SUMMARY, INI_SUMMARY_ARCNUM, 0, lpszFileName);

	graph.numVertexes = nPointNum;
	graph.numEdges    = nArcNum;

	if ( nPointNum <= 0 || nArcNum <=  0) return false; 

	// 所有点
	std::vector<int> iVecPoints;
	for ( int i = 0; i < nPointNum; ++ i )
	{
		CString strPointName(INI_POINTS_PREFIX);	
		TCHAR szBuf[20] = { 0 };
		
		_itot(i, szBuf, 10);
		strPointName += CString(szBuf);

		int nPtName = ::GetPrivateProfileInt(INI_POINTS, strPointName, 0, lpszFileName);
		iVecPoints.push_back(nPtName);  

		graph.adjlist[ i ].data = i;
		graph.adjlist[ i ].firstedge = NULL;
	}
	
	typedef std::vector<EdgeNode> VecEdgeNodes;
	std::map<int, VecEdgeNodes* > graphTable;

	int nCount = 0;

	// 所有弧
	for ( int i = 0; i < nArcNum; ++ i )
	{
		CString strArcName(INI_ARCS_PREFIX);	
		TCHAR szBuf[20] = { 0 };

		_itot(i, szBuf, 10);
		strArcName += CString(szBuf);

		TCHAR szBuf2[ MAX_VERTEX ] = { 0 };
		int nRet = ::GetPrivateProfileString(INI_ARCS, strArcName, TEXT(""), szBuf2, _countof(szBuf2), lpszFileName);

		CString strBuf2(szBuf2);
		int nPos = strBuf2.Find(TEXT('-')); 
		CString strFrom, strTo;
		int nFrom = -1;
		int nTo   = -1;

		if ( nPos >= 0 )
		{
			strFrom = strBuf2.Left(nPos);
			strTo   = strBuf2.Mid(nPos + 1, strBuf2.GetLength() - nPos);

			nFrom = _ttoi(strFrom);
			nTo   = _ttoi(strTo);


			EdgeNode* pEdge = new EdgeNode;
			pEdge->tail = nFrom;
			pEdge->adjvex = nTo;
			pEdge->next   = NULL;

			std::map<int, VecEdgeNodes* >::iterator iter = graphTable.find(nFrom);  


			if ( iter == graphTable.end() )
			{ 
				std::vector<EdgeNode>* pVecEdges = new std::vector<EdgeNode>;
				pVecEdges->push_back(*pEdge);
				graph.adjlist[ nFrom ].firstedge = pEdge;

				graphTable.insert(make_pair(nFrom, pVecEdges)); 
			}
    		else
			{
				 ( ( *(iter->second) )  [ iter->second->size() - 1]).next = pEdge; 
				 ( *(iter->second) ).push_back(*pEdge);  
			}  

			edgeNodes[ i ] = *pEdge; 
		} 
		else
		{
			++ nCount;
		}
	}  // for

	if ( nCount >= nArcNum )
		return false;

	return true;
}
