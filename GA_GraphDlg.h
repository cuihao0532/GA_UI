
// GA_GraphDlg.h : ͷ�ļ�
//

#pragma once
#include "EventHandler.h"


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

protected:
	void DrawCoordinate();
	void InitGA();
	void Begin();

	void Draw();

public:
	void Draw(POINT* pts, int nNum);

	afx_msg void OnBnClickedBtnBegin();
};
