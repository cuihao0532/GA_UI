#include "StdAfx.h"
#include "GA.h"
#include <ctime>
#include <algorithm>

int g_nDistMin  = 30;
int g_nDistDown = 30;
int g_nAngle    = 30;

extern CGraph g_graph;


CGA::CGA(void)
{
	m_vecChromosomes.clear();

    m_nXBegin = 0;
    m_nXEnd   = 0;
    m_nYBegin = 0;
    m_nYEnd   = 0;
	m_pUI     = NULL;

    m_nBestFitness = MAX_FITNESS;
    m_nBestGeneration = 0;

    srand(unsigned(time(0)));
}

CGA::CGA(int nPopSize,        // ��Ⱥ��ģ
	int nMaxGenerations,      // ���ֳ����
	float fpSelectRate,       // ѡ�����
	float fpCrossRate,        // �������
	float fpMutRate,          // �������
	int xRangeBegin,          // ��ֲ������x�����
	int xRangeEnd,            // ��ֲ������x���յ�
	int yRangeBegin,          // ��ֲ������y�����
	int yRangeEnd,            // ����������y���յ�
	CGraph& graph,
	IUIInterface* pUI)            
{
	m_nPopSize        = nPopSize;
	m_nMaxGenerations = nMaxGenerations;
	m_fpSelectRate    = fpSelectRate;
	m_fpCrossRate     = fpCrossRate;
	m_fpMutRate       = fpMutRate;
	m_nGeneration     = 0;

    m_nXBegin         = xRangeBegin;
    m_nXEnd           = xRangeEnd;
    m_nYBegin         = yRangeBegin;
    m_nYEnd           = yRangeEnd;
    m_graph           = graph;
    m_nBestFitness    = MAX_FITNESS;
    m_nBestGeneration = 0;
	m_pUI             = pUI;

    m_vecChromosomes.clear();  
    srand(unsigned(time(0))); 
}


CGA::~CGA(void)
{
	m_vecChromosomes.clear(); 
}


void CGA::Init()
{
    // ����ͼ�ж���������������[��Ⱥ��ģ] �� [��������������]

    // �������
    int nVertexNum = m_graph.graph.numVertexes;
    
    for ( int i = 0; i < m_nPopSize; ++ i )
    {
        // һ��Ⱦɫ��
        CChromosome chr;

        for ( int index = 0; index < nVertexNum; ++ index )
        {
            // �������(x, y)����
            Gene gene;
            gene.nX = Rand(m_nXBegin, m_nXEnd);
            gene.nY = Rand(m_nYBegin, m_nYEnd);
            
            chr.Add(gene);            
        }

        // ���һ��Ⱦɫ��
        Add(chr);        
    }


    // ��ʼ����Ⱥ��� 
}


void CGA::Select()
{

}




/************************************************************************/
/*  �����ӽ����ʣ�����Ⱦɫ����ԣ���һ���������µ�����vector��
	* ��Ӧֱ�߷��̣�x = 0, ����a = 1, b = 0, c = 0	Ⱦɫ��ch1, ch2
	* Ⱦɫ��ch1 ch2��ÿ���� �ֱ������������vector�ľ�������

	* V1(10, 10) V2(10, 0)
	* ÿ��Ⱦɫ���еĵ�V�� ����V1V������V1V2�����ϵ�ͶӰ�Ĵ�С����С��������                                                                      */
/************************************************************************/
void CGA::Cross()
{ 
	// ��Ⱥ��Ⱦɫ�����
    int nSize = m_vecChromosomes.size(); 
	
	// �Ȱѵ�ǰ��Ⱥ��������
	ChromosomeGroup groupCopy(m_vecChromosomes.begin(), m_vecChromosomes.end()); 

	// ����Ⱦɫ�������ӽ�
    for ( int i = 0; i < ( m_vecChromosomes.size() / 2 ) * 2; i += 2 )
    {
        float fRate = 0.0F;
		if ( fRate <= m_fpCrossRate )
		{ 
			// ����i �� i + 1�� 
			// ��¼��Ⱦɫ����ÿ�����򣨵�V�����������(V1V2)�� ����V1V ������ V1V2�ϵ�ͶӰ�ĳ��� �� �õ�V������
			std::vector<Index2Dist> vecIndeies1; 
			auto genes = m_vecChromosomes[ i ].GetGenes();

			for (int index = 0; index < genes.size(); ++ index )
			{
				int nVX = genes[ index ].nX;
				int nVY = genes[ index ].nY;

				int nV1X = 10;
				int nV1Y = 2000;

				int nV2X = 10;
				int nV2Y = -2000;

				// ����V1V(nVX - nV1X, nVY - nV1Y)
				// ����V1V2(nV2X - nV1X, nV1Y - nV2Y)
				int x1 = nVX - nV1X;
				int y1 = nVY - nV1Y;

				int x2 = nV2X - nV1X;
				int y2 = nV2Y - nV1Y; 


				// ����V1V��V1V2
				float fArcCos = (x1 * x2 + y1 * y2) / 
					( sqrt(1.0F * x1 * x1 + y1 * y1) * sqrt(1.0F * x2 * x2 + y2 * y2) );

				// ͶӰ = |V1V| * cosX
				float fLength = sqrt( 1.0F * x1 * x1 + y1 * y1 ) * fArcCos;

				Index2Dist id;
				id.nIndex = index;
				id.fDist  = fLength;
				id.gene = genes[ index ];
				vecIndeies1.push_back(id);
			} // for

			// ����fDist����
			std::sort(vecIndeies1.begin(), vecIndeies1.end());

			//////////////////////////////////////////////////////////////////////////

			// ��¼��Ⱦɫ����ÿ�����򣨵�V�����������(V1V2)�� ����V1V ������ V1V2�ϵ�ͶӰ�ĳ��� �� �õ�V������
			std::vector<Index2Dist> vecIndeies2; 
			auto genes2 = m_vecChromosomes[ i + 1 ].GetGenes();

			for (int index = 0; index < genes2.size(); ++ index )
			{
				int nVX = genes2[ index ].nX;
				int nVY = genes2[ index ].nY;

				int nV1X = 10;
				int nV1Y = 2000;

				int nV2X = 10;
				int nV2Y = -2000;

				// ����V1V(nVX - nV1X, nVY - nV1Y)
				// ����V1V2(nV2X - nV1X, nV1Y - nV2Y)
				int x1 = nVX - nV1X;
				int y1 = nVY - nV1Y;

				int x2 = nV2X - nV1X;
				int y2 = nV2Y - nV1Y; 


				// ����V1V��V1V2
				float fArcCos = (x1 * x2 + y1 * y2) / 
					( sqrt(1.0F * x1 * x1 + y1 * y1) * sqrt(1.0F * x2 * x2 + y2 * y2) );

				// ͶӰ = |V1V| * cosX
				float fLength = sqrt( 1.0F * x1 * x1 + y1 * y1 ) * fArcCos;

				Index2Dist id;
				id.nIndex = index;
				id.fDist  = fLength;
				id.gene = genes2[ index ];
				vecIndeies2.push_back(id);
			} // for

			// ����dist��С��������
			std::sort(vecIndeies2.begin(), vecIndeies2.end());



			// �ӽ���������¸��壨Ⱦɫ�壩
			GeneGroup g1, g2; 

			// ��������������vec
			std::vector<Index2Dist> vec1Copy(vecIndeies1.begin(), vecIndeies1.end());
			std::vector<Index2Dist> vec2Copy(vecIndeies2.begin(), vecIndeies2.end()); 

			int cnt1 = 0;
			int cnt2 = vecIndeies2.size() - 1; 

			for ( ; 
				cnt1 < vecIndeies1.size() && 
				cnt2 >= 0 && 
				g1.size() < vecIndeies1.size(); 
				++ cnt1, -- cnt2 )
			{
				auto it1 = std::find(g1.begin(), g1.end(), vecIndeies1[ cnt1 ].gene );
				if ( it1 == g1.end() )
				{
					g1.push_back( vecIndeies1[ cnt1 ].gene );  
					auto itTemp = std::find(vec1Copy.begin(), vec1Copy.end(), vecIndeies1[ cnt1 ].gene);
					vec1Copy.erase(itTemp);
				}
				 

				auto it2 = std::find(g1.begin(), g1.end(), vecIndeies2[ cnt2 ].gene ); 
				if ( it2 == g1.end() )
				{
					g1.push_back( vecIndeies2[ cnt2 ].gene); 
					auto itTemp = std::find(vec2Copy.begin(), vec2Copy.end(), vecIndeies2[ cnt2 ].gene);
					vec2Copy.erase(itTemp);
				}
				 
			} // for  

			// ��vec1Copy �� vec2Copy ��ʣ��ĵ����g2
			for ( auto iter = vec1Copy.begin(); iter != vec1Copy.end(); ++ iter )
			{
				g2.push_back(iter->gene);
			}

			for ( auto iter = vec2Copy.begin(); iter != vec2Copy.end(); ++ iter )
			{
				g2.push_back(iter->gene);
			} 
			
			// ���¸����������������
		    // g1����iλ��Ⱦɫ�壬 g2����i+1λ��Ⱦɫ��
			m_vecChromosomes[ i ].SetGenes(g1);
			m_vecChromosomes[ i + 1 ].SetGenes(g2); 

		} // if

    } // for 
    
}

void CGA::Mut()
{

}

void CGA::Output() const
{
	cout << "PopSize = " << m_nPopSize << ", Cur Generation = " << m_nGeneration << endl;

	for ( int i = 0; i < m_vecChromosomes.size(); ++ i )
	{
		m_vecChromosomes[ i ].Output();
	}

	cout << " --------------------------------  " << endl;
}

int CGA::IncreaseGeneration()
{
	++ m_nGeneration;
	return m_nGeneration;
}

void CGA::Draw()
{
	for ( int i = 0; i < m_vecChromosomes.size(); ++ i )
	{
		const GeneGroup& genes =  m_vecChromosomes[ i ].GetGenes(); 

		int nSize = genes.size();
		CPoint *pts = new CPoint[ nSize ]();
		for ( int j = 0; j < nSize; ++ j )
		{
			pts[ j ].x = genes[ j ].nX;
			pts[ j ].y = genes[ j ].nY;
		}
		if ( m_pUI )
		{
			m_pUI->Draw(pts, nSize);
		}

		delete[] pts;
		pts = NULL;
	}
}

void CGA::Draw(const CChromosome& chr)
{ 
	const GeneGroup& genes =  chr.GetGenes();
 	int nSize = genes.size();
	CPoint *pts = new CPoint[ nSize ]();
	for ( int j = 0; j < nSize; ++ j )
	{
		pts[ j ].x = genes[ j ].nX;
		pts[ j ].y = genes[ j ].nY;
	}
	if ( m_pUI )
	{
		m_pUI->Draw(pts, nSize);
	}

	delete[] pts;
	pts = NULL;
	 
}



// ����
void CGA::Evolution()
{
    int nCount = 0;
     
    while ( ++nCount <= m_nMaxGenerations )
    {  
        Select();
        Cross();
        Mut();

		int nIndex = -1;
        // ����ô�����Ӧ�ȣ��ҳ�����Ⱥ�����ŵĽ� 
        int nMinFitness = CalFitness(nIndex);
        int nCurGeneration = IncreaseGeneration(); 

        if ( nMinFitness < m_nBestFitness )
        {
            m_nBestFitness = nMinFitness;
            m_nBestGeneration = nCurGeneration;
        }

		// ��¼��ǰ��Ⱥ����С��Ӧ�ȡ��Լ���С��Ӧ�����ĸ�Ⱦɫ�� 
		Generations curGeneration;
		curGeneration.group = m_vecChromosomes;
		curGeneration.nCurFitness = nMinFitness;
		curGeneration.nCurMinIndex = nIndex;
		//m_vecAllGenerations.push_back(curGeneration);
		
		if ( nIndex >= 0 )
		{
			//Sleep(1000 * 2); 
			Draw(m_vecChromosomes[ nIndex ]); 
			Sleep(1000 * 2);
		} 

        // ��������������������˳�
        if ( 0 )
        {
            break;
        }

    } 

    int a = 0;
	Finish();
}

int CGA::CalFitness(int& nIndex)
{
    // ����Ⱥ����ÿ��Ⱦɫ�����Ӧ��
    int nMin = MAX_FITNESS; 
    
    for ( int i = 0; i < m_vecChromosomes.size(); ++ i )
    {
        int nFitness = m_vecChromosomes[i].CalFitness();     

        if ( nFitness < nMin )
        {
            nMin = nFitness;
            nIndex = i;
        }
    }

    return nMin;
}

int CGA::Rand( int begin, int end )
{ 
    int nMin = begin;
    int nMax = end;

    if ( begin > end )
    {
        nMin = end;
        nMax = begin;
    }
     
    int nVal = (int)((double)rand() / ((RAND_MAX + 1.0) / (end - begin + 1.0)) + begin);  
     
    return nVal;
}

int CGA::Add( const CChromosome& chr )
{
    m_vecChromosomes.push_back(chr);
    return m_vecChromosomes.size();
}

void CGA::SetParam( 
    int nDistMin,  /* ��������֮�����С��� */ 
    int nDistDown, /* ���µĻ���ͷ����β�ڵ����µ���С���� */ 
    int nAngle     /* ��ͬ��β�Ļ�֮�����С�н�*/)
{
    m_nDistMin  = nDistMin;
    m_nDistDown = nDistDown;
    m_nAngle    = nAngle;
}

void CGA::Finish()
{
	if ( m_pUI )
	{
		m_pUI->Finish();
	}
}








/************************************************************************/
/*                 Ⱦɫ��                                               */
/************************************************************************/

CChromosome::CChromosome()
{
	m_nFitness = MAX_FITNESS;
	m_vecGenes.clear(); 
}

CChromosome::~CChromosome()
{
	m_vecGenes.clear();
}

int CChromosome::CalFitness()
{
    int k1 = 50;
    int k2 = 100;
    int k3 = 20;
    int k4 = 30;
    int k5 = 100;
    int k6 = 1000;

    // ������������֮��ľ������
    float f1 = 0.0F;

	if ( m_vecGenes.size() == 0 )
	{
		return MAX_FITNESS;
	}

    for ( int i = 0; i < m_vecGenes.size() - 1; ++ i )
    {
        for ( int index = i + 1; index < m_vecGenes.size(); ++ index )
        {
            f1 += k1 * ( g_nDistMin - Dist(m_vecGenes[i], m_vecGenes[index]) );
        }
    }


    // ÿ�����Ļ�ͷ�뻡β��Y����ľ��� ���
    float f2 = 0.0F;
    for ( int i = 0; i < g_graph.graph.numVertexes; ++ i )
    {
        int nTail = g_graph.edgeNodes[i].tail;
        int nHead = g_graph.edgeNodes[i].adjvex;

        // ����������Ļ�ͷ ��β��Y����֮��ľ���
        float fTemp = m_vecGenes[nTail].nY - m_vecGenes[nHead].nY;

        f2 += k2 * ( fTemp + g_nDistDown );
    }

     


    // ÿ�����ĳ��� ���
    float f3 = 0.0F;
    for ( int i = 0; i < g_graph.graph.numVertexes; ++ i )
    {
        int nTail = g_graph.edgeNodes[i].tail;
        int nHead = g_graph.edgeNodes[i].adjvex;

        float fTemp = Dist(m_vecGenes[nTail], m_vecGenes[nHead]);
        
        f3 += k3 * fTemp;
    }

    // ���㵽ÿ�����ľ��� ���
    float f4 = 0.0F;

    // ����ÿ�����㵽ÿ���ߵľ���
    for ( int i = 0; i < g_graph.graph.numVertexes; ++ i )
    {

        for ( int index = 0; index < g_graph.graph.numEdges; ++ index )
        {
            // �������� A
            int nX = m_vecGenes[i].nX;
            int nY = m_vecGenes[i].nY;

            // �ߵ���� v1
            int nTail = g_graph.edgeNodes[index].tail;
            int nHead = g_graph.edgeNodes[index].adjvex;
            int nXTail = m_vecGenes[nTail].nX;
            int nYTail = m_vecGenes[nTail].nY;

            // �ߵ��յ� v2
            int nXHead = m_vecGenes[nHead].nX;
            int nYHead = m_vecGenes[nHead].nY;

            // ��Av1v2 ��Av2v1 
            // ��������Av1 �� v1v2��������  
            // v1A(nX - nXTail, nY - nYTail)   v1v2(nXHead - nXTail, nYHead - nYTail)
            // v2A(nX - nXHead, nY - nYHead)   v2v1(nXTail - nXHead, nYTail - nYHead) 
            // cos��=(X1X2+Y1Y2)/(���ţ�X1^2+X2^2�� * ���ţ�X2^2+Y2^2))

            int x1 = nX - nXTail;
            int y1 = nY - nYTail;
            int x2 = nXHead - nXTail;
            int y2 = nYHead - nYTail;

            float fCosAngle1 = ( x1 * x2 + y1 * y2) /
                (
                    sqrt( 1.0F * (x1 * x1 + y1 * y1) ) * sqrt( 1.0F * (x2 * x2 + y2 * y2) )
                );


            x1 = nX - nXHead;
            y1 = nY - nYHead;
            x2 = nXTail - nXHead;
            y2 = nYTail - nYHead;

            float fCosAngle2 = ( x1 * x2 + y1 * y2) /
                (
                sqrt( 1.0F * (x1 * x1 + y1 * y1) ) * sqrt( 1.0F * (x2 * x2 + y2 * y2) )
                );
             
          

            // ������������нǶ�Ϊ��ǣ������ö��㵽�����ߵľ��루����������ֱ���ϵ������
            if ( fCosAngle1 * fCosAngle2 > 0 )
            {
                // ֱ��v1v1б��
                float k = 1.0F * (nYHead - nYTail) / (nXHead - nXTail);               // ֱ�߷��� y = kx + b������v1���꣬��b
                float b = nYTail - k * nXTail;
                float a = k; 
                float c = b;
                b = -1;

                // ���㶥�㵽ֱ�ߵľ���
                float fDist = (a * nX + b * nY + c) / (a * a + b * b); 

                f4 += fDist;
            }
        }
    }


    // ��ͬ��β�Ļ�֮��ļн� ���
    float f5 = 0.0F;
    
    for ( int i = 0; i < g_graph.graph.numVertexes; ++ i )
    {
        // ����
        VertexNode* pVertex = &g_graph.graph.adjlist[i];

        // �����Ըö���Ϊ��β�Ļ�
        std::vector<EdgeNode*> vecEdgeNodePtrs;
 
        EdgeNode* pEdge = pVertex->firstedge;
        while ( pEdge )
        {
            vecEdgeNodePtrs.push_back(pEdge); 
            pEdge = pEdge->next;
        }

        // ������Щ��ͬ��β�Ļ�֮��ļн�
        int nSize = vecEdgeNodePtrs.size();
        for ( int index = 0; index < nSize - 1; ++ index )
        {
            for ( int m = index + 1; m < nSize; ++ m )
            {
                // ����1�����յ��������е�λ��
                int nTailPos = vecEdgeNodePtrs[ index ]->tail;
                int nHeadPos = vecEdgeNodePtrs[ index ]->adjvex;

                int nTailX_1 = m_vecGenes[nTailPos].nX;
                int nTailY_1 = m_vecGenes[nTailPos].nY;
                int nHeadX_1 = m_vecGenes[nHeadPos].nX;
                int nHeadY_1 = m_vecGenes[nHeadPos].nY;



                // ����2�����յ��������е�λ��
                nTailPos = vecEdgeNodePtrs[ m ]->tail;
                nHeadPos = vecEdgeNodePtrs[ m ]->adjvex;

                int nTailX_2 = m_vecGenes[ nTailPos ].nX;
                int nTailY_2 = m_vecGenes[ nTailPos ].nY;
                int nHeadX_2 = m_vecGenes[ nHeadPos ].nX;
                int nHeadY_2 = m_vecGenes[ nHeadPos ].nY;

                // vector1(
                int vector1 = (nHeadX_1 - nTailX_1, nHeadY_1 - nTailY_1);
                int vector2 = (nHeadX_2 - nTailX_2, nHeadY_2 - nTailY_2);

                int x1 = nHeadX_1 - nTailX_1;
                int y1 = nHeadY_1 - nTailY_1; 

                int x2 = nHeadX_2 - nTailX_2;
                int y2 = nHeadY_2 - nTailY_2;

                // ��β(nXTail, nYTail)  ��ͷ(nXHead, nYHead)
                // ����н�����
                float fCosAngle = ( x1 * x2 + y1 * y2) / (
                    sqrt( 1.0F * x1 * x1 + y1 * y1 ) * sqrt( 1.0F * x2 * x2 + y2 * y2)
                    );
                 
                float fAngle = acosf(fCosAngle) * 180 / PI;

                f5 += k5 * ( g_nAngle - fAngle );
            }
        } // for

 
    } // for



    // �������
    float f6 = 0.0F;


    float fFitness = f1 + f2 + f3 + f4 + f5 + f6;

	return ( m_nFitness = fFitness );
}

void CChromosome::Output() const
{
	cout << " Fitness = " << m_nFitness << ", Path = { ";
	for ( int i = 0; i < m_vecGenes.size(); ++ i )
	{
        cout << "(";
		cout << m_vecGenes[ i ].nX << ", " << m_vecGenes[ i ].nY;
        cout << ")";

		if ( i < m_vecGenes.size() - 1 )
		{
			cout << ",    "; 
		}
	}

	cout << " }" << endl;
}

int CChromosome::Add( const Gene& gene )
{
    m_vecGenes.push_back(gene);
    return m_vecGenes.size();
}

int CChromosome::SetGenes(const GeneGroup& genes)
{
	m_vecGenes.clear();
	m_vecGenes.assign(genes.begin(), genes.end());

	int nSize = this->Size();
	return nSize;
}

int CChromosome::Dist( const Gene& gene1, const Gene& gene2 )
{
    int nDist = sqrt( 1.0F * (gene1.nX - gene2.nX) * (gene1.nX - gene2.nX) + (gene1.nY - gene2.nY) * (gene1.nY - gene2.nY) );
    return nDist;
}

const GeneGroup& CChromosome::GetGenes()  const
{
	return m_vecGenes;
}

int CChromosome::Size() const
{
	return ( m_vecGenes.size() );
}
