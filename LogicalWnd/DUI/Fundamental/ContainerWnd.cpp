// ContainerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "LWnd.h"
#include "ContainerWnd.h"
#include "LWndManager.h"



// CContainerWnd

IMPLEMENT_DYNAMIC(CContainerWnd, CWnd)

CContainerWnd::CContainerWnd()
{
	m_bSetTrackMouseEvent=FALSE;
}

CContainerWnd::~CContainerWnd()
{
}


BEGIN_MESSAGE_MAP(CContainerWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CContainerWnd message handlers

int CContainerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	DUIInit(m_hWnd);
	return 0;
}


BOOL CContainerWnd::OnEraseBkgnd(CDC* pDc) 
{
	return true;
     //return cview::onerasebkgnd(pdc);
}

void CContainerWnd::OnPaint()
{
	CPaintDC dc(this);
	DUIDraw();
}

void CContainerWnd::OnDestroy()
{
	CWnd::OnDestroy();
	
	DUICleanup();
}

void CContainerWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bSetTrackMouseEvent)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize=sizeof(TRACKMOUSEEVENT);
		tme.dwFlags=TME_LEAVE;
		tme.hwndTrack=m_hWnd;
		_TrackMouseEvent(&tme);
		m_bSetTrackMouseEvent=TRUE;
	}
	DUIMouseMove(point.x,point.y);
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CContainerWnd::OnMouseLeave(WPARAM wp,LPARAM lp)
{
	//@ 当系统发送这个消息时，GetCursor获取的光标位置可能还在窗口内
	//所以传递一个距窗口非常非常遥远的坐标
	DUIMouseMove(0x1FFFFFF,0x1FFFFFF);
	m_bSetTrackMouseEvent=FALSE;
	TRACE("OnMouseLeave\n");
	return 0;
}

void CContainerWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	DUILButtonDown(point.x,point.y);
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}


void CContainerWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	DUILButtonUp(point.x,point.y);
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CContainerWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DUILButtonDblClick(point.x,point.y);
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}
