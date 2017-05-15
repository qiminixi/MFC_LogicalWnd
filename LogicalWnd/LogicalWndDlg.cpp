// LogicalWndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LogicalWnd.h"
#include "LogicalWndDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogicalWndDlg dialog
CContainerWnd g_container;


CLogicalWndDlg::CLogicalWndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogicalWndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogicalWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogicalWndDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CLogicalWndDlg message handlers

BOOL CLogicalWndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CRect rect;
	GetClientRect(&rect);
	rect.left += 10;
	rect.right -= 10;
	rect.top += 10;
	rect.bottom -= 10;
	g_container.Create(AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),L"123",WS_CHILD|WS_VISIBLE,rect,this,0);

	CLWnd *pLWnd=new CLWnd();
	pLWnd->Create(0,0,50,50,TRUE,0);
	pLWnd->SetWindowText(_T("Normal1"));
	
	pLWnd=new CLWnd();
	pLWnd->Create(25,25,75,75,TRUE,0);
	pLWnd->SetWindowText(_T("Normal2"));

	pLWnd=new CLWnd();
	pLWnd->Create(50,50,100,100,TRUE,1);
	pLWnd->SetWindowText(_T("Top"));
	
	CImgButton *pbtn=new CImgButton();
	pbtn->Create(100,10,NULL);
	pbtn->LoadImage(L"res\\button.png");
	//pbtn->LoadImage(IDR_PNG_BUTTON,_T("PNG"));
	//pbtn->LoadImage(IDB_BITMAP_BUTTON,RT_BITMAP);
	
	CImgSlider *pslider=new CImgSlider;
	pslider->Create(TRUE,240,20,200,17,NULL);
	pslider->SetSizes(0,5,0,5,0,0,17);
	pslider->LoadImagesFromFile(L"res\\progress\\channel.png",L"res\\progress\\active.png",L"res\\progress\\thumb.png");
	//pslider->EnableOutline();
	pslider->SetPos(50);
	//pslider->EnableBg();
	//DUIRemove(pslider);
	
	CCombination *pcomb=new CCombination;
	pcomb->Create(75,100,150,150);
	//pcomb->Show(0);
	pcomb->Show(1);
	/*pcomb->DeleteLWnds();
	pcomb->Destroy();
	delete pcomb;*/
	
	CGifPlayer *gp=new CGifPlayer();
	gp->Create(150,85);
	gp->SetWindowText(_T("Top"));
	gp->LoadGif(L"res\\gif.gif");
	gp->Play();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CLogicalWndDlg::OnEraseBkgnd(CDC* pDc) 
{
	return true;
     //return cview::onerasebkgnd(pdc);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLogicalWndDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLogicalWndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CLogicalWndDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

void CLogicalWndDlg::OnSize(UINT nType, int cx, int cy)
{
	
}