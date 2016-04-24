
// GA_GraphDlg.h : 头文件
//

#pragma once
#include "EventHandler.h"
#include "Graph.h"
#include "GA.h"
#include "DataType.h"
#include "afxwin.h"

 

// CGA_GraphDlg 对话框
class CGA_GraphDlg : public CDialogEx, public IUIInterface
{
// 构造
public:
	CGA_GraphDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GA_GRAPH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	

protected:
	int m_nOrignX; // 坐标轴原点x坐标
	int m_nOrignY; // 坐标轴原点y坐标
	POINT* m_pPoints;
	int    m_nNumPoints;

	int    m_nNumArcs;  // 弧条数
	ArcNode m_arcNodes[ MAX_VERTEX * (MAX_VERTEX - 1) ];

protected:
	void DrawCoordinate();
	void InitGA();
	void Begin(); 
	void Draw();
	HANDLE m_hThread;
	friend unsigned int __stdcall WorkThread(void* p);
	void DrawArc(const CPoint& ptFrom, const CPoint& ptTo);

	void InitUI();
	void SetUIEnable(BOOL b = TRUE);
	int DrawArrowLine(/*HDC hDC, */LPPOINT lpPointStart, LPPOINT lpPointEnd, double nArrowBorderLen, double xAngleInRadians, COLORREF cr);
	

public:
	void Draw(POINT* pts, int nNum, ArcNode* pArcNodes, int nNumArc ); 
	void Finish();

	afx_msg void OnBnClickedBtnBegin();
	CEdit m_editPopSize;
	CEdit m_edtMaxGenerations;
	CEdit m_edtCrossRate;
	CEdit m_edtMutRate;
	CEdit m_edtXBegin;
	CEdit m_edtXEnd;
	CEdit m_edtYBegin;
	CEdit m_edtYEnd;
	CEdit m_edtPointMinDist;
	CEdit m_edtDistDown;
	CEdit m_edtAngle;
	CEdit m_edtFileName;
	afx_msg void OnBnClickedBtnSetFile();
};
