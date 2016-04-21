
#include "EventHandler.h"
#include <vector>
#include <iostream>
#include "Graph.h"
using std::cout;
using std::endl;
using std::vector;


#pragma once

// 路径最大权值
#define  MAX_WIGHT  99999999999
#define  PI         3.14159265358979323846

// 基因（一个坐标对）
typedef struct _tagGene
{
public:
    _tagGene() { nX = nY = 0; }
    int nX;  
    int nY;
     

}Gene, *PGene;


// 染色体(若干个坐标对)
class CChromosome
{
public:
	CChromosome();
	virtual ~CChromosome();

	int  CalFitness();            // 计算适应度
	void Output() const;          // 输出信息
    int  Add(const Gene& gene);   // 往染色体中添加基因
    typedef std::vector<Gene> GeneGroup;

    // 计算两个两个基因（点）之间的距离
    int Dist(const Gene& gene1, const Gene& gene2);
	GeneGroup& GetGenes();

	//void Draw();

protected:
	int m_nFitness;              // 适应度
	GeneGroup m_vecGenes;     // 记录所有坐标对
};


class CGA
{
public:
	CGA(void);
	CGA(
		int nPopSize,             // 种群规模
		int nMaxGenerations,      // 最大繁殖代数
		float fpSelectRate,       // 选择概率
		float fpCrossRate,        // 交叉概率
		float fpMutRate,          // 变异概率
		int xRangeBegin,          // 点分布区域的x轴起点
		int xRangeEnd,            // 点分布区域的x轴终点
		int yRangeBegin,          // 点分布区域的y轴起点
		int yRangeEnd,            // 点分布区域的y轴终点
		CGraph& graph,
		IUIInterface* pUI	
		);          

	virtual ~CGA(void);


    /************************************************************************/
	/* 初始化种群
	 * 图有N个点，每个点用一组平面坐标(x, y)来表示其位置，所以一条染色体即为
	 (x1, y1, ... , xN, yN)
	 * 在指定x轴，y轴区间范围内，随机生成若干个染色体
	/************************************************************************/
	void  Init();					// 初始化种群


    // 进化
	void Evolution();				 


    // 输出
	void Output() const;			// 输出


	// 设置参数
    void  SetParam(
        int nDistMin,             // 任意两点之间的最小间隔
        int nDistDown,            // 向下的弧的头结点比尾节点向下的最小距离
        int nAngle);              // 相同弧尾的弧之间的最小夹角)

protected:
	int m_nPopSize;					// 种群规模
	int m_nMaxGenerations;			// 最大繁殖代数
	float m_fpSelectRate;			// 选择概率
	float m_fpCrossRate;			// 交叉概率
	float m_fpMutRate;				// 变异概率
     
	int   m_nGeneration;            // 第几代

	int m_nXBegin;                  // 点分布区域的x轴起点
	int m_nXEnd;                    // 点分布区域的x轴终点
	int m_nYBegin;                  // 点分布区域的y轴起点
	int m_nYEnd;                    // 点分布区域的y轴终点

    int m_nDistMin;                 // 任意两点之间的最小间隔
    int m_nDistDown;                // 向下的弧的头结点比尾节点向下的最小距离
    int m_nAngle;                   // 相同弧尾的弧之间的最小夹角

    int m_nBestFitness;             // 记录所有迭代中的最优解
    int m_nBestGeneration;          // 记录最优解所在的代数


	IUIInterface*   m_pUI;

protected:
	int   IncreaseGeneration();     // 增加代数 
    int   CalFitness();             // 计算适应度
	void  Select();					// 选择
	void  Cross();					// 交叉
	void  Mut();					// 变异
    int   Rand(int begin, int end); // 生成随机数

    typedef std::vector<CChromosome> ChromosomeGroup;
    ChromosomeGroup    m_vecChromosomes; // 染色体种群

    int  Add(const CChromosome& chr);    // 添加染色体
	void Draw();

    CGraph m_graph;
};

