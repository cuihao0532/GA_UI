
// GA_GraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GA_Graph.h"
#include "GA_GraphDlg.h"
#include "afxdialogex.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGraph g_graph;
int g_nDistMin  = 30;
int g_nDistDown = 30;
int g_nAngle    = 30;


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
	m_hThread    = INVALID_HANDLE_VALUE;
	m_nNumArcs = 0; 
}

void CGA_GraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_POPSIZE, m_editPopSize);
	DDX_Control(pDX, IDC_EDIT_MAX_GENERATIONS, m_edtMaxGenerations);
	DDX_Control(pDX, IDC_EDIT_CROSS_RATE, m_edtCrossRate);
	DDX_Control(pDX, IDC_EDIT_MUT_RATE, m_edtMutRate);
	DDX_Control(pDX, IDC_EDIT_X_BEGIN, m_edtXBegin);
	DDX_Control(pDX, IDC_EDIT_X_END, m_edtXEnd);
	DDX_Control(pDX, IDC_EDIT_Y_BEGIN, m_edtYBegin);
	DDX_Control(pDX, IDC_EDIT_Y_END, m_edtYEnd);
	DDX_Control(pDX, IDC_EDIT_POINT_MIN_DIST, m_edtPointMinDist);
	DDX_Control(pDX, IDC_EDIT_DIST_DOWN, m_edtDistDown);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_edtAngle);
	DDX_Control(pDX, IDC_EDIT_FILE, m_edtFileName);
}

BEGIN_MESSAGE_MAP(CGA_GraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CGA_GraphDlg::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_BTN_SET_FILE, &CGA_GraphDlg::OnBnClickedBtnSetFile)
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
	GetDlgItem(IDC_BTN_BEGIN)->MoveWindow(8, 30, 50, 30);

	InitUI();

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
	//g_graph.CreateGraph();
	//g_graph.CreateFromFile(TEXT("J:\\graph.txt"));  

#if 0
	int nPopSize					= 40;
	int nMaxGenerations				= 10;
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
#else

	CString strText;
	float fpSelectRate				= 1.0F; 
	m_editPopSize.GetWindowText(strText);

	TCHAR szBuf[ 100 ] = { 0 };
	m_editPopSize.GetWindowText(szBuf, _countof(szBuf));
	int nVal = _ttoi(szBuf); 
	int nPopSize					= nVal;

	memset(szBuf, 0, sizeof(szBuf));
	m_edtMaxGenerations.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);
	int nMaxGenerations				= nVal;


	memset(szBuf, 0, sizeof(szBuf));
	m_edtCrossRate.GetWindowText(szBuf,_countof(szBuf));
	float fVal = _ttof(szBuf); 
	float fpCrossRate				= fVal;

	memset(szBuf, 0, sizeof(szBuf));
	m_edtMutRate.GetWindowText(szBuf,_countof(szBuf));
	fVal = _ttof(szBuf); 
	float fpMutRate					= fVal;


	memset(szBuf, 0, sizeof(szBuf));
	m_edtXBegin.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);
	int xBegin = nVal;

	memset(szBuf, 0, sizeof(szBuf));
	m_edtXEnd.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf); 
	int xEnd   = nVal;


	memset(szBuf, 0, sizeof(szBuf));
	m_edtYBegin.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);  
	int yBegin = nVal;

	memset(szBuf, 0, sizeof(szBuf));
	m_edtYEnd.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);  
	int yEnd   = nVal;

	memset(szBuf, 0, sizeof(szBuf));
	m_edtPointMinDist.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);  
	g_nDistMin = nVal;


	memset(szBuf, 0, sizeof(szBuf));
	m_edtDistDown.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);  
	g_nDistDown = nVal;


	memset(szBuf, 0, sizeof(szBuf));
	m_edtAngle.GetWindowText(szBuf,_countof(szBuf));
	nVal = _ttoi(szBuf);  
	g_nAngle = nVal;
	 
#endif

	CGA ga(nPopSize, nMaxGenerations, fpSelectRate, fpCrossRate, fpMutRate, xBegin, xEnd, yBegin, yEnd, g_graph, this); 
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
		CString strText;
		strText.Format(TEXT("v[%d]:(%d, %d)"), g_graph.graph.adjlist[ i ].data, m_pPoints[ i ].x, m_pPoints[ i ].y); 
		pDC->TextOut(m_pPoints[ i ].x, m_pPoints[ i ].y, strText);

		int r = 10;
		pDC->Arc(m_pPoints[ i ].x-r,m_pPoints[ i ].y-r,m_pPoints[ i ].x+r,m_pPoints[ i ].y+r,0,0,0,0);
	} 

	if ( m_nNumArcs <= 0 ) return;

	for ( int i = 0; i < m_nNumArcs; ++ i )
	{
#if 0
		DrawArc(m_arcNodes[ i ].ptFrom, m_arcNodes[ i ].ptTo);
#else
		DrawArrowLine(&m_arcNodes[ i ].ptFrom, &m_arcNodes[ i ].ptTo, 8, PI / 6.0, RGB(255, 0, 0) );
#endif
	}

	ReleaseDC(pDC); 
}


void CGA_GraphDlg::Draw(POINT* pts, int nNum, ArcNode* pArcNodes, int nNumArc )
{
	if ( !pts || nNum <= 0 ) return; 
	if (!pArcNodes || nNumArc <= 0 ) return;

	delete[] m_pPoints;
	m_pPoints = NULL;

	m_nNumPoints = nNum;
	m_pPoints = new POINT[ nNum ]();
	memcpy_s(m_pPoints, nNum * sizeof(POINT), pts, nNum * sizeof(POINT));
	 

	CString strLog;
	strLog.Format(TEXT("Points num = %d: "), nNum);
	for ( int i = 0; i < nNum; ++ i )
	{
		CString strTemp; 
		strTemp.Format(TEXT("(%d, %d)"), m_pPoints[ i ].x, m_pPoints[ i ].y);

		if ( i < nNum - 1) 
			strTemp += CString(TEXT(", ")); 

		strLog += strTemp;		
	}

	//afxDump << strLog;
	OutputDebugString(strLog);


	//////////////////////////////////////////////////////////////////////////
	memset(m_arcNodes, 0, sizeof(m_arcNodes));
	memcpy_s(m_arcNodes, sizeof(m_arcNodes), pArcNodes, nNumArc * sizeof(ArcNode));
	m_nNumArcs = nNumArc;

	Invalidate();
}

void CGA_GraphDlg::OnBnClickedBtnBegin()
{
	UpdateData(TRUE);	 
	CString strText; 
	m_edtFileName.GetWindowText(strText);
	if ( strText.GetLength() <= 0 ) return;

	bool b = g_graph.CreateFromFile(strText);
	if ( !b ) return;


	if ( INVALID_HANDLE_VALUE == m_hThread || NULL == m_hThread )
	{
		m_hThread = (HANDLE)_beginthreadex(
			NULL,
			0,
			WorkThread,
			this,
			0,
			NULL);

	}

	if ( NULL != m_hThread && INVALID_HANDLE_VALUE != m_hThread )
	{
		SetUIEnable(FALSE);
	}
}

void CGA_GraphDlg::Finish()
{
	CloseHandle(m_hThread);
	m_hThread = NULL;
	SetUIEnable(TRUE);
}

void CGA_GraphDlg::DrawArc(const CPoint& ptFrom, const CPoint& ptTo)
{
	CDC* pDC = GetDC();

	CPen pen(PS_SOLID, 2, RGB(0, 255, 255));
	HPEN hOldPen = (HPEN)( pDC->SelectObject(pen) );

	pDC->MoveTo(ptFrom);
	pDC->LineTo(ptTo); 

	pDC->SelectObject(hOldPen);
	ReleaseDC(pDC);  
}

void CGA_GraphDlg::InitUI()
{
	int nTop = 10;
	int nLeft = 20;
	 
	m_editPopSize.SetWindowText(TEXT("40"));
	m_edtMaxGenerations.SetWindowText(TEXT("10"));
	m_edtCrossRate.SetWindowText(TEXT("0.55")); 
	m_edtMutRate.SetWindowText(TEXT("0.015"));
	m_edtXBegin.SetWindowText(TEXT("100"));
	m_edtXEnd.SetWindowText(TEXT("600"));
	m_edtYBegin.SetWindowText(TEXT("100"));
	m_edtYEnd.SetWindowText(TEXT("600"));
	m_edtPointMinDist.SetWindowText(TEXT("30"));
	m_edtDistDown.SetWindowText(TEXT("30"));
	m_edtAngle.SetWindowText(TEXT("30")); 

	UpdateData(FALSE);
	
}

void CGA_GraphDlg::SetUIEnable(BOOL b /*= TRUE*/)
{
	GetDlgItem(IDC_BTN_BEGIN)->EnableWindow(b);  
	GetDlgItem(IDC_BTN_SET_FILE)->EnableWindow(b);  

	m_editPopSize.EnableWindow(b);
	m_edtMaxGenerations.EnableWindow(b);
	m_edtCrossRate.EnableWindow(b);
	m_edtMutRate.EnableWindow(b);
	m_edtXBegin.EnableWindow(b);
	m_edtXEnd.EnableWindow(b);
	m_edtYBegin.EnableWindow(b);
	m_edtYEnd.EnableWindow(b);
	m_edtPointMinDist.EnableWindow(b);
	m_edtDistDown.EnableWindow(b);
	m_edtAngle.EnableWindow(b);
	m_edtFileName.EnableWindow(b);

}

 


unsigned int __stdcall WorkThread(void* p)
{
	if ( !p ) return 0;

	CGA_GraphDlg* pDlg = (CGA_GraphDlg*)( p ); 
	pDlg->InitGA();

	return 0;
}


int CGA_GraphDlg::DrawArrowLine(/*HDC hDC, */LPPOINT lpPointStart, LPPOINT lpPointEnd, double nArrowBorderLen, double xAngleInRadians, COLORREF cr)
{ 
	CDC* pDC = GetDC();

	double xAngleLine = 0;
	if (lpPointStart->x == lpPointEnd->x)
		xAngleLine = PI / 2;
	else
	{
		xAngleLine = atan((double(lpPointEnd->y) - double(lpPointStart->y)) / (double(lpPointEnd->x) - double(lpPointStart->x)));
	}

	CPoint ptLineLeft;
	if (lpPointStart->x > lpPointEnd->x)
	{
		ptLineLeft.x = long(lpPointEnd->x + cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		ptLineLeft.y = long(lpPointEnd->y + sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
	}
	else if (lpPointStart->x == lpPointEnd->x)
	{
		if (lpPointStart->y > lpPointEnd->y)
		{
			ptLineLeft.x = long(lpPointEnd->x + cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
			ptLineLeft.y = long(lpPointEnd->y + sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		}
		else if (lpPointStart->y == lpPointEnd->y)
		{
			return 0;
		}
		else
		{
			ptLineLeft.x = long(lpPointEnd->x - cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
			ptLineLeft.y = long(lpPointEnd->y - sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		}
	}
	else
	{
		ptLineLeft.x = long(lpPointEnd->x - cos(xAngleLine + xAngleInRadians) * nArrowBorderLen);
		ptLineLeft.y = long(lpPointEnd->y - sin(xAngleLine + xAngleInRadians) * nArrowBorderLen);
	}

	CPoint ptLineRight;
	if (lpPointStart->x > lpPointEnd->x)
	{
		ptLineRight.x = long(lpPointEnd->x + cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		ptLineRight.y = long(lpPointEnd->y + sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
	}
	else if (lpPointStart->x == lpPointEnd->x)
	{
		if (lpPointStart->y > lpPointEnd->y)
		{
			ptLineRight.x = long(lpPointEnd->x + cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
			ptLineRight.y = long(lpPointEnd->y + sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		}
		else if (lpPointStart->y > lpPointEnd->y)
		{
			return 0;
		}
		else
		{
			ptLineRight.x = long(lpPointEnd->x - cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
			ptLineRight.y = long(lpPointEnd->y - sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		}
	}
	else
	{
		ptLineRight.x = long(lpPointEnd->x - cos(xAngleLine - xAngleInRadians) * nArrowBorderLen);
		ptLineRight.y = long(lpPointEnd->y - sin(xAngleLine - xAngleInRadians) * nArrowBorderLen);
	}

	HPEN hPen = ::CreatePen(PS_SOLID, 1, cr);
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = cr;
	HBRUSH hBrush = ::CreateBrushIndirect(&lb);

	//HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HPEN hOldPen = (HPEN)( pDC->SelectObject(hPen));


	// Draw line
	pDC->MoveTo(lpPointStart->x, lpPointStart->y);
	//::MoveToEx(hDC, lpPointStart->x, lpPointStart->y, NULL);
	//::LineTo(hDC, lpPointEnd->x, lpPointEnd->y);
	pDC->LineTo(lpPointEnd->x, lpPointEnd->y);

	// Draw arrow
	//HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	HBRUSH hOldBrush = (HBRUSH)( pDC->SelectObject(hBrush));

	POINT ptPolygon[3] = {lpPointEnd->x, lpPointEnd->y, ptLineLeft.x, ptLineLeft.y, ptLineRight.x, ptLineRight.y};
	//::Polygon(hDC, ptPolygon, 3);
	pDC->Polygon(ptPolygon, 3);

	pDC->SelectObject(hOldBrush);
	//::SelectObject(hDC, hOldBrush);

	pDC->SelectObject(hOldPen);
	//::SelectObject(hDC, hOldPen);

	::DeleteObject(hBrush);
	::DeleteObject(hPen);

	ReleaseDC(pDC);
	return 0;
}

void CGA_GraphDlg::OnBnClickedBtnSetFile()
{
	CFileDialog dlg(TRUE, TEXT("txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, TEXT("(*.txt)|*.txt||"));
	if (dlg.DoModal() == IDOK)
	{
		m_edtFileName.SetWindowText(dlg.GetPathName());
	}
}
