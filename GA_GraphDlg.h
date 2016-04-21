
// GA_GraphDlg.h : 头文件
//

#pragma once
#include "EventHandler.h"


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

protected:
	void DrawCoordinate();
	void InitGA();
	void Begin();

	void Draw();

public:
	void Draw(POINT* pts, int nNum);

	afx_msg void OnBnClickedBtnBegin();
};
