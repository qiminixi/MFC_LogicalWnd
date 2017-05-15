#include "stdafx.h"
#include "Fundamental/LWnd.h"
#include "Fundamental/LWndManager.h"
#include "Combination.h"

// @���캯��
CCombination::CCombination()
{
}

// @��������
CCombination::~CCombination()
{
}

//@ �¼�����
UINT CCombination::MessageProcessing(UINT nMsgId,WPARAM wParam/*=0*/,LPARAM lParam/*=0*/)
{
	CLWnd *pLWnd=(CLWnd *)wParam;
	TRACE("CCombination::MessageProcessing:[%d],[%d]\n",nMsgId,pLWnd->GetWindowRect().Width);
	return 0;
}

//@ ����
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

//@ �ƶ������
void CCombination::Move(int nX,int nY,int nWidth,int nHeight)
{
	int a=nX-GetWindowRect().X;//�ƶ�������x
	int b=nY-GetWindowRect().Y;//�ƶ�������y
	
	//@ ��������ϴ��ڽ����ƶ�
	POSITION pos=m_listCombinedLWnd.GetHeadPosition();
	CLWnd *pLWnd;
	while(pos!=NULL)
	{
		pLWnd=m_listCombinedLWnd.GetNext(pos);
		pLWnd->Move(pLWnd->GetWindowRect().X+a,pLWnd->GetWindowRect().Y+b);
	}
	CLWnd::Move(nX,nY,nWidth,nHeight);
}

//@ ��ʾ/���������
void CCombination::Show(BOOL bShow/*=TRUE*/)
{
	CLWnd::Show(bShow);
	//@ ��������ϴ��ڵ���Show
	POSITION pos=m_listCombinedLWnd.GetHeadPosition();
	CLWnd *pLWnd;
	while(pos!=NULL)
	{
		pLWnd=m_listCombinedLWnd.GetNext(pos);
		pLWnd->Show(bShow);
	}
	CLWnd::Show(bShow);
}

//@ ���һ���ؼ�
void CCombination::AddLWnd(CLWnd* pLWnd)
{
	//@ �ؼ�����ʱָ����ʱ������������꣬��ת��Ϊ������������ꡣ
	pLWnd->Move(pLWnd->GetWindowRect().X+GetWindowRect().X,
		pLWnd->GetWindowRect().Y+GetWindowRect().Y);
	m_listCombinedLWnd.AddTail(pLWnd);
}

//@ ɾ�����д���
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