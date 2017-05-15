#include "stdafx.h"
#include "Fundamental/LWnd.h"
#include "Fundamental/LWndManager.h"
#include "Combination.h"

// @构造函数
CCombination::CCombination()
{
}

// @析构函数
CCombination::~CCombination()
{
}

//@ 事件处理
UINT CCombination::MessageProcessing(UINT nMsgId,WPARAM wParam/*=0*/,LPARAM lParam/*=0*/)
{
	CLWnd *pLWnd=(CLWnd *)wParam;
	TRACE("CCombination::MessageProcessing:[%d],[%d]\n",nMsgId,pLWnd->GetWindowRect().Width);
	return 0;
}

//@ 创建
BOOL CCombination::Create(int nX,int nY,int nWidth,int nHeight)
{
	CLWnd::Create(nX,nY,nWidth,nHeight,TRUE,LWND_ZAXIS_LEVEL_TOP);
	SetWindowText(_T("Combination"));
	
	CImgButton *pbtn=new CImgButton();
	pbtn->Create(20,20,this,LWND_ZAXIS_LEVEL_TOPMOST);
	pbtn->LoadImage(L"res\\button.png");
	AddLWnd(pbtn);
	return TRUE;
}

//@ 移动组合体
void CCombination::Move(int nX,int nY,int nWidth,int nHeight)
{
	int a=nX-GetWindowRect().X;//移动向量的x
	int b=nY-GetWindowRect().Y;//移动向量的y
	
	//@ 对所有组合窗口进行移动
	POSITION pos=m_listCombinedLWnd.GetHeadPosition();
	CLWnd *pLWnd;
	while(pos!=NULL)
	{
		pLWnd=m_listCombinedLWnd.GetNext(pos);
		pLWnd->Move(pLWnd->GetWindowRect().X+a,pLWnd->GetWindowRect().Y+b);
	}
	CLWnd::Move(nX,nY,nWidth,nHeight);
}

//@ 显示/隐藏组合体
void CCombination::Show(BOOL bShow/*=TRUE*/)
{
	CLWnd::Show(bShow);
	//@ 对所有组合窗口调用Show
	POSITION pos=m_listCombinedLWnd.GetHeadPosition();
	CLWnd *pLWnd;
	while(pos!=NULL)
	{
		pLWnd=m_listCombinedLWnd.GetNext(pos);
		pLWnd->Show(bShow);
	}
	CLWnd::Show(bShow);
}

//@ 添加一个控件
void CCombination::AddLWnd(CLWnd* pLWnd)
{
	//@ 控件创建时指定的时相对组合体的坐标，需转换为相对容器的坐标。
	pLWnd->Move(pLWnd->GetWindowRect().X+GetWindowRect().X,
		pLWnd->GetWindowRect().Y+GetWindowRect().Y);
	m_listCombinedLWnd.AddTail(pLWnd);
}

//@ 删除所有窗口
void CCombination::DeleteLWnds()
{
	POSITION pos=m_listCombinedLWnd.GetHeadPosition();
	CLWnd *pLWnd;
	while(pos!=NULL)
	{
		pLWnd=m_listCombinedLWnd.GetNext(pos);
		pLWnd->Destroy();
		delete pLWnd;
	}
}