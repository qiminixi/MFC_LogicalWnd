/**
 *@ CContainerWnd是所有逻辑窗口的容器
 *@ 它一个真实窗口，从CWnd派生
 *@ 一般的，逻辑窗口列表在容器窗口销毁时销毁。
**/


#pragma once

#ifndef CONTAINER_WND_H
#define CONTAINER_WND_H

// CContainerWnd
class CLWnd;
class CContainerWnd : public CWnd
{
	DECLARE_DYNAMIC(CContainerWnd)

public:
	CContainerWnd();
	virtual ~CContainerWnd();

private:
	BOOL m_bSetTrackMouseEvent;

public:
	virtual BOOL OnEraseBkgnd(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg LRESULT OnMouseLeave(WPARAM wp,LPARAM lp);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif
