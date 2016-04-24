
// GA_GraphDlg.h : ͷ�ļ�
//

#pragma once
#include "EventHandler.h"
#include "Graph.h"
#include "GA.h"
#include "DataType.h"
#include "afxwin.h"

 

// CGA_GraphDlg �Ի���
class CGA_GraphDlg : public CDialogEx, public IUIInterface
{
// ����
public:
	CGA_GraphDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GA_GRAPH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	

protected:
	int m_nOrignX; // ������ԭ��x����
	int m_nOrignY; // ������ԭ��y����
	POINT* m_pPoints;
	int    m_nNumPoints;

	int    m_nNumArcs;  // ������
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
