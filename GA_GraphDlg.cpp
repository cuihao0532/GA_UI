
// GA_GraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GA_Graph.h"
#include "GA_GraphDlg.h"
#include "afxdialogex.h"
#include "Graph.h"
#include "GA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGraph g_graph;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGA_GraphDlg 对话框




CGA_GraphDlg::CGA_GraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGA_GraphDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pPoints = NULL;
	m_nNumPoints = 0;
}

void CGA_GraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGA_GraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CGA_GraphDlg::OnBnClickedBtnBegin)
END_MESSAGE_MAP()


// CGA_GraphDlg 消息处理程序

BOOL CGA_GraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(0, 0, nWidth, nHeight, FALSE);

	GetDlgItem(IDC_BTN_BEGIN)->MoveWindow(10, 30, 30, 30);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGA_GraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGA_GraphDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	DrawCoordinate();
	Draw();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGA_GraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGA_GraphDlg::DrawCoordinate()
{
	CRect rcClient;
	GetClientRect(rcClient);

	int nBottom = 100;
	int nLeft   = 100;

	m_nOrignX = nLeft;
	m_nOrignY = rcClient.Height() - nBottom;

	CDC* pDC = GetDC();

	int nXFrom = 0;
	int nXTo = rcClient.Width();

	int nYFrom = rcClient.Height();
	int nYTo   = 0;

	int nLineWidth = 2;
	COLORREF colorLine = RGB(44, 61, 90);
	CPen penLine(PS_SOLID, nLineWidth, colorLine); 
	HPEN HOldPen =  (HPEN)pDC->SelectObject(penLine);

	
	pDC->MoveTo(m_nOrignX / 2, m_nOrignY);
	pDC->LineTo(nXTo - 50, m_nOrignY);

	pDC->MoveTo(m_nOrignX, nYFrom - nBottom / 2);
	pDC->LineTo(m_nOrignX, 50);

	pDC->SelectObject(HOldPen);

	ReleaseDC(pDC);

}

void CGA_GraphDlg::InitGA()
{
	g_graph.CreateGraph();

	int nPopSize					= 1;
	int nMaxGenerations				= 1;
	float fpSelectRate				= 1.0F;
	float fpCrossRate				= 0.55F;
	float fpMutRate					= 0.015F;

	int xBegin = 100;
	int xEnd   = 600;
	int yBegin = 100;
	int yEnd   = 600;
	int nDistMin  = 20;
	int nDistDown = 20;
	int nAngle    = 20;

	CGA ga(nPopSize, nMaxGenerations, fpSelectRate, fpCrossRate, fpMutRate, xBegin, xEnd, yBegin, yEnd, g_graph, this);
	ga.SetParam(nDistMin, nDistDown, nAngle);

	ga.Init();
	ga.Evolution();
	ga.Output();
}

void CGA_GraphDlg::Draw()
{
	if ( !m_pPoints || m_nNumPoints <= 0 ) return;
 
	CDC* pDC = GetDC();

	COLORREF cr = RGB(255, 0, 0);
	for ( int i = 0; i < m_nNumPoints; ++ i )
	{
		pDC->SetPixel(m_pPoints[ i ], cr);
		pDC->TextOut(m_pPoints[ i ].x, m_pPoints[ i ].y, CString(TEXT("hello")));
	} 

	ReleaseDC(pDC); 
}


void CGA_GraphDlg::Draw(POINT* pts, int nNum)
{
	if ( !pts || nNum <= 0 ) return;

	delete[] m_pPoints;
	m_pPoints = NULL;

	m_nNumPoints = nNum;
	m_pPoints = new POINT[ nNum ]();
	memcpy_s(m_pPoints, nNum * sizeof(POINT), pts, nNum * sizeof(POINT));

	Invalidate();
}

void CGA_GraphDlg::OnBnClickedBtnBegin()
{
	InitGA();
}
