#include "StdAfx.h"
#include "ImgButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CImgButton::CImgButton()
{
	m_pImage = NULL;
	m_buttonstate=ButtonStateNormal;
}

CImgButton::~CImgButton()
{
}

//@ 不需要指定窗口尺寸，会根据图像自动调整
BOOL CImgButton::Create(int x,int y,CLWnd *pParent,int nZAxisLevel/*=LWND_ZAXIS_LEVEL_MIDDLE*/)
{
	m_pParent=pParent;
	return CLWnd::Create(x,y,1,1,TRUE,nZAxisLevel);
}

void CImgButton::Destroy()
{
	if(m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	m_buttonstate=ButtonStateNormal;
	CLWnd::Destroy();
}

BOOL CImgButton::LoadImage(UINT nIDResource,LPCTSTR szType)
{
	if(m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	if(!ImageFromIDResource(nIDResource, szType, m_pImage))
		return FALSE;
	
	Status stat=m_pImage->GetLastStatus();
	if(stat!=Ok) return FALSE;
	Resize(m_pImage->GetWidth()/3,m_pImage->GetHeight());
	return TRUE;
}

BOOL CImgButton::LoadImage(WCHAR *szFileName)
{
	if(m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	m_pImage = new Image(szFileName);
	
	Status stat=m_pImage->GetLastStatus();
	if(stat!=Ok) return FALSE;
	Resize(m_pImage->GetWidth()/3,m_pImage->GetHeight());
	return TRUE;
}

void CImgButton::Enable(BOOL bEnable)
{
	m_buttonstate=bEnable ? ButtonStateNormal : ButtonStateDisable;
	CLWnd::Enable(bEnable);
}

void CImgButton::Draw()
{
	if(m_pImage == NULL) return;
	Graphics graphics(GetBitmap());
	int w=GetWindowRect().Width;
	int h=GetWindowRect().Height;
	int n=m_buttonstate != ButtonStateDisable ? m_buttonstate:ButtonStateNormal;
	graphics.DrawImage(m_pImage,0,0,w*n,0,w,h,UnitPixel);
}

void CImgButton::MouseMove(int x,int y)
{
	if(m_buttonstate==ButtonStateDisable) return;
	if(m_buttonstate == ButtonStateActive) return;
	m_buttonstate=ButtonStateHover;
	UpdateWindow();
}

void CImgButton::MouseLeave()
{
	if(m_buttonstate==ButtonStateDisable) return;
	if(m_buttonstate == ButtonStateActive) return;
	m_buttonstate=ButtonStateNormal;
	UpdateWindow();
}

void CImgButton::LButtonDown(int x,int y)
{
	if(m_buttonstate==ButtonStateDisable) return;
	m_buttonstate=ButtonStateActive;
	if(m_pParent)
		m_pParent->MessageProcessing(LUM_IMGBUTTON_LBUTTON_DOWN,(WPARAM)(this));
	UpdateWindow();
}

void CImgButton::LButtonUp(int x,int y)
{
	if(m_buttonstate==ButtonStateDisable) return;
	m_buttonstate=ButtonStateHover;
	UpdateWindow();
}

void CImgButton::LButtonDblClick(int x,int y)
{
	if(m_buttonstate==ButtonStateDisable) return;
	m_buttonstate=ButtonStateActive;
	UpdateWindow();
}