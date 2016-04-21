
#include "EventHandler.h"
#include <vector>
#include <iostream>
#include "Graph.h"
using std::cout;
using std::endl;
using std::vector;


#pragma once

// ·�����Ȩֵ
#define  MAX_WIGHT  99999999999
#define  PI         3.14159265358979323846

// ����һ������ԣ�
typedef struct _tagGene
{
public:
    _tagGene() { nX = nY = 0; }
    int nX;  
    int nY;
     

}Gene, *PGene;


// Ⱦɫ��(���ɸ������)
class CChromosome
{
public:
	CChromosome();
	virtual ~CChromosome();

	int  CalFitness();            // ������Ӧ��
	void Output() const;          // �����Ϣ
    int  Add(const Gene& gene);   // ��Ⱦɫ������ӻ���
    typedef std::vector<Gene> GeneGroup;

    // ���������������򣨵㣩֮��ľ���
    int Dist(const Gene& gene1, const Gene& gene2);
	GeneGroup& GetGenes();

	//void Draw();

protected:
	int m_nFitness;              // ��Ӧ��
	GeneGroup m_vecGenes;     // ��¼���������
};


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


    // ���
	void Output() const;			// ���


	// ���ò���
    void  SetParam(
        int nDistMin,             // ��������֮�����С���
        int nDistDown,            // ���µĻ���ͷ����β�ڵ����µ���С����
        int nAngle);              // ��ͬ��β�Ļ�֮�����С�н�)

protected:
	int m_nPopSize;					// ��Ⱥ��ģ
	int m_nMaxGenerations;			// ���ֳ����
	float m_fpSelectRate;			// ѡ�����
	float m_fpCrossRate;			// �������
	float m_fpMutRate;				// �������
     
	int   m_nGeneration;            // �ڼ���

	int m_nXBegin;                  // ��ֲ������x�����
	int m_nXEnd;                    // ��ֲ������x���յ�
	int m_nYBegin;                  // ��ֲ������y�����
	int m_nYEnd;                    // ��ֲ������y���յ�

    int m_nDistMin;                 // ��������֮�����С���
    int m_nDistDown;                // ���µĻ���ͷ����β�ڵ����µ���С����
    int m_nAngle;                   // ��ͬ��β�Ļ�֮�����С�н�

    int m_nBestFitness;             // ��¼���е����е����Ž�
    int m_nBestGeneration;          // ��¼���Ž����ڵĴ���


	IUIInterface*   m_pUI;

protected:
	int   IncreaseGeneration();     // ���Ӵ��� 
    int   CalFitness();             // ������Ӧ��
	void  Select();					// ѡ��
	void  Cross();					// ����
	void  Mut();					// ����
    int   Rand(int begin, int end); // ���������

    typedef std::vector<CChromosome> ChromosomeGroup;
    ChromosomeGroup    m_vecChromosomes; // Ⱦɫ����Ⱥ

    int  Add(const CChromosome& chr);    // ���Ⱦɫ��
	void Draw();

    CGraph m_graph;
};

