
#include "EventHandler.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include "Graph.h"
#include "DataType.h"
using std::cout;
using std::endl;
using std::vector;
using std::map;



#pragma once

// �����Ч����Ӧ��
#define  MAX_FITNESS  999999
#define  PI           3.14159265358979323846

// ����һ������ԣ�
typedef struct _tagGene
{
public:
	_tagGene() { nX = nY = 0; }
	int nX;  
	int nY; 

	bool operator==(const _tagGene& gene)
	{
		bool b = ( this->nX == gene.nX && this->nY == gene.nY ); 
		return b; 
	}
	_tagGene& operator=(const _tagGene& gene)
	{
		if ( this != &gene )
		{
			this->nX = gene.nX;
			this->nY = gene.nY;
		}

		return ( *this );
	}

}Gene, *PGene;


// �������潻������е��м���
typedef struct _tagIndex2Dist
{
public:
	_tagIndex2Dist() { memset(this, 0, sizeof(*this)); }

	bool operator<(const _tagIndex2Dist& obj)
	{
		if ( this->fDist < obj.fDist )
			return true;

		return false; 
	}

	bool operator==(const _tagIndex2Dist& obj)
	{
		bool b = ( this->gene == obj.gene );
		bool b2 = ( this->nIndex == obj.nIndex && this->fDist == obj.fDist );
		return ( b && b2 );
	}

	bool operator==(const Gene& gene)
	{
		bool b = ( this->gene == gene );
		return b;
	}

	int nIndex;
	float fDist; 
	Gene gene;

}Index2Dist, *PIndex2Dist;

typedef std::vector<Gene> GeneGroup;


// Ⱦɫ��(���ɸ������)
class CChromosome
{
public:
	CChromosome();
	virtual ~CChromosome();

	int  CalFitness();								// ������Ӧ��
	void Output() const;							// �����Ϣ
    int  Add(const Gene& gene);						// ��Ⱦɫ������ӻ���
	int  SetGenes(const GeneGroup& genes);			// ���û���

    
    int Dist(const Gene& gene1, const Gene& gene2); // ���������������򣨵㣩֮��ľ���
	const GeneGroup& GetGenes() const;              // ���ظ�Ⱦɫ�Ļ�����
	bool  SetGene(int nPos, const Gene& gene);      // �޸�nPosλ�õĻ���
	int Size() const; 

protected:
	int m_nFitness;									// ��Ӧ��
	GeneGroup m_vecGenes;							// ��¼���������(����
};

typedef std::vector<CChromosome> ChromosomeGroup;

// ��¼���������е�һ������Ϣ
typedef struct _tagGenerations
{
public:
	_tagGenerations() 
	{
		nCurMinIndex = -1;
		nCurMinIndex = MAX_FITNESS;
	}

	int nCurMinIndex;
	int nCurFitness;
	ChromosomeGroup group;
}Generations, *PGenerations;

typedef std::vector<Generations> All_Generations;




class CGA
{
public:
	CGA(void);
	CGA(
		int nPopSize,             // ��Ⱥ��ģ
		int nMaxGenerations,      // ���ֳ����
		float fpSelectRate,       // ѡ�����
		float fpCrossRate,        // �������
		float fpMutRate,          // �������
		int xRangeBegin,          // ��ֲ������x�����
		int xRangeEnd,            // ��ֲ������x���յ�
		int yRangeBegin,          // ��ֲ������y�����
		int yRangeEnd,            // ��ֲ������y���յ�
		CGraph& graph,
		IUIInterface* pUI	
		);          

	virtual ~CGA(void);


    /************************************************************************/
	/* ��ʼ����Ⱥ
	 * ͼ��N���㣬ÿ������һ��ƽ������(x, y)����ʾ��λ�ã�����һ��Ⱦɫ�弴Ϊ
	 (x1, y1, ... , xN, yN)
	 * ��ָ��x�ᣬy�����䷶Χ�ڣ�����������ɸ�Ⱦɫ��
	/************************************************************************/
	void  Init();					// ��ʼ����Ⱥ


    // ����
	void Evolution();				 


    // �����Ϣ
	void Output() const;		 


	// ���ò���
    void  SetParam(
        int nDistMin,						// ��������֮�����С���
        int nDistDown,						// ���µĻ���ͷ����β�ڵ����µ���С����
        int nAngle);						// ��ͬ��β�Ļ�֮�����С�н�)

protected:
	int m_nPopSize;							// ��Ⱥ��ģ
	int m_nMaxGenerations;					// ���ֳ����
	float m_fpSelectRate;					// ѡ�����
	float m_fpCrossRate;					// �������
	float m_fpMutRate;						// �������
     
	int   m_nGeneration;					// �ڼ���

	int m_nXBegin;							// ��ֲ������x�����
	int m_nXEnd;							// ��ֲ������x���յ�
	int m_nYBegin;							// ��ֲ������y�����
	int m_nYEnd;							// ��ֲ������y���յ�

    int m_nDistMin;							// ��������֮�����С���
    int m_nDistDown;						// ���µĻ���ͷ����β�ڵ����µ���С����
    int m_nAngle;							// ��ͬ��β�Ļ�֮�����С�н�

    int m_nBestFitness;						// ��¼���е����е����Ž�
    int m_nBestGeneration;					// ��¼���Ž����ڵĴ���
	ChromosomeGroup    m_vecChromosomes;    // Ⱦɫ����Ⱥ
	IUIInterface*   m_pUI;                  // UI�ӿ�ָ��
	All_Generations m_vecAllGenerations;    // ��¼���������е�ÿһ������Ϣ
	CGraph m_graph;                         // ͼ

protected:
	int   IncreaseGeneration();				// ���Ӵ��� 
    int   CalFitness(int& nIndex);			// ������Ӧ��
	void  Select();							// ѡ��
	void  Cross();							// ����
	void  Mut();							// ����
    int   Rand(int begin, int end) const;	// ���������
	float Randf() const;                    // ����[0, 1]֮������������
	 

    int  Add(const CChromosome& chr);       // ���Ⱦɫ��
	void Draw();                            // ֪ͨUI����
	void Draw(const CChromosome& chr);      // ֪ͨUI����
	void Finish();                          // ֪ͨUI �������  
};

