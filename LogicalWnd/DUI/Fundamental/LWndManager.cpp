#include "stdafx.h"
#include "LWnd.h"
#include "LWndManager.h"

//@ ��¼�����߼��Ӵ���
//@ ������Ϊȫ�ֱ���ʹ�����������������Ҳ�ܴ�������
CList<CLWnd*> g_listLogicalWnd;

//@ Z�����ָ�
#define g_pLWndBottom	((CLWnd*)0x1)
#define g_pLWndMiddle	((CLWnd*)0x2)
#define g_pLWndTop		((CLWnd*)0x3)
#define g_pLWndTopMost	((CLWnd*)0x4)
POSITION g_posBottom;
POSITION g_posMiddle;
POSITION g_posTop;
POSITION g_posTopMost;

//@ ��¼��ǰ������ڴ���
CLWnd *g_pLWndMouseIn=NULL;
//@ ��¼�����ľ������������ʾ�Լ��ĸ��µ���Ҫ
HWND g_hwndContainer=NULL;

//@ ��¼Ҫ�󲶻����λ�õĴ���
CLWnd *g_pLWndSetCapture=NULL;

//�ڴ��ͼ
CDC g_memDC; //���ȶ���һ����ʾ�豸����
CBitmap g_memBitmap;//����һ��λͼ����

//���߳�����
BOOL g_isDrawing = FALSE;//�������߳�ͬʱ����������󣬲�����˸�����粥gif����
						//���ǻ��е���˸����������Ϊ�����Ǳ���100msˢ��һ��

void DUIDrawStart();
void DUIDrawnEnd();

//@ ��ʼ��
//@ ��Ҫ�ǽ�BOTTOM��MIDDLE��TOP������εķָ������ȥ
//@ ���������˳��Ϊ����ͷ��β�����ס��м䡢��
//@ �ָ�����λ��ĳһ�������棬������ĩβ��Ӵ���
void DUIInit(HWND hwndContainer)
{
	ASSERT(hwndContainer!=NULL);
	g_hwndContainer=hwndContainer;
	g_posBottom=g_listLogicalWnd.AddTail(g_pLWndBottom);
	g_posMiddle=g_listLogicalWnd.AddTail(g_pLWndMiddle);
	g_posTop=g_listLogicalWnd.AddTail(g_pLWndTop);
	g_posTopMost=g_listLogicalWnd.AddTail(g_pLWndTopMost);
	
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);
	CDC dc;
	dc.Attach(GetDC(hwndContainer));

	g_memDC.CreateCompatibleDC(NULL);//�����ڴ��豸
	g_memBitmap.CreateCompatibleBitmap(&dc, nWidth, nHeight);//����λͼ
	CBitmap *pOldBit=g_memDC.SelectObject(&g_memBitmap);
	
	ReleaseDC(g_hwndContainer,dc.m_hDC);
}

//@ ���һ���߼��������б�
//@ �ڵ��е�n�������ڵ�n+1����������
void DUIAdd(CLWnd *pLWnd)
{
	ASSERT(pLWnd!=NULL);
	switch(pLWnd->GetZAxisLevel())
	{
	case LWND_ZAXIS_LEVEL_BOTTOM:
		g_listLogicalWnd.InsertBefore(g_posBottom,pLWnd);
		break;
	case LWND_ZAXIS_LEVEL_MIDDLE:
		g_listLogicalWnd.InsertBefore(g_posMiddle,pLWnd);
		break;
	case LWND_ZAXIS_LEVEL_TOP:
		g_listLogicalWnd.InsertBefore(g_posTop,pLWnd);
		break;
	case LWND_ZAXIS_LEVEL_TOPMOST:
		g_listLogicalWnd.InsertBefore(g_posTopMost,pLWnd);
		break;
	}
}

//@ ���б�ɾ��һ���߼�����
//@ ע����󲻻�ɾ��
void DUIRemove(CLWnd *pLWnd)
{
	ASSERT(pLWnd!=NULL);
	POSITION pos=g_listLogicalWnd.Find(pLWnd);
	if(pos!=NULL)
		g_listLogicalWnd.RemoveAt(pos);
}

//@ �������Ӵ��ڽ��л��ƣ�����ʾ���豸
//@ ����hDC��Ϊ����OnPaint����OnDraw�п���ֱ��ʹ�ú�����CDC
void DUIDraw(BOOL redrawAll)
{
	//@ �ӵײ���ʼ��ʾ
	
	POSITION pos;
	pos = g_listLogicalWnd.GetHeadPosition();
	if(NULL==pos) return;
	
	DUIDrawStart();

	Graphics graphics(g_memDC.m_hDC);

	while(pos)
	{
		CLWnd *pLWnd=NULL;

		pLWnd=g_listLogicalWnd.GetNext(pos);
		if(pLWnd > g_pLWndTopMost)
		{
			if(pLWnd->IsVisible())
			{
				if(redrawAll)
					pLWnd->Draw();
				graphics.DrawImage(pLWnd->GetBitmap(),pLWnd->GetWindowRect());
			}
		}
	}
	
	DUIDrawnEnd();
}

//��ͼ��ʼ��ˢ�»��������ǰ�Ķ���
void DUIDrawStart()
{
	if(g_isDrawing) return;
	g_isDrawing = TRUE;

	CRect rect;
	GetWindowRect(g_hwndContainer, rect);
	CBrush br(RGB(12,34,87));
	g_memDC.FillRect(CRect(0, 0, rect.Width(), rect.Height()), &br);
}

//��ͼ��������λͼ��ʾ��������
void DUIDrawnEnd()
{
	CRect rect;
	GetWindowRect(g_hwndContainer, rect);
	CDC dc;
	dc.Attach(GetDC(g_hwndContainer));
	
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&g_memDC,0,0,SRCCOPY);
	
	ReleaseDC(g_hwndContainer,dc.m_hDC);

	g_isDrawing = FALSE;
}

//@ ���»���һ�����ڣ���������ʾ����
void DUIRedrawWindow(CLWnd* pLWnd)
{
	ASSERT(pLWnd!=NULL);
	pLWnd->Draw();
	DUIDraw(false);
}

//@ �������д��ڲ�ɾ��CLWnd����
//@ �˳�����ʱ�����ܵĲ�Ҫ����������ִ�����ٲ�������Ϊ��ʱGidplus�����Ѿ��ر��ˡ�
//@ ��������������ʱ��������һ���ȽϺõ�ѡ��
void DUICleanup()
{
	POSITION pos;
	pos = g_listLogicalWnd.GetHeadPosition();
	while(pos)
	{
		CLWnd *pLWnd=NULL;
		pLWnd=g_listLogicalWnd.GetNext(pos);
		if(pLWnd > g_pLWndTopMost)
		{
			pLWnd->Destroy();
			delete pLWnd;
		}
	}
	g_listLogicalWnd.RemoveAll();
}

//@ ���Ұ���ĳ����Ĵ���
//@ ����POSITION��Ϊ�˵��÷��ܹ����ô��ڼ�����ŵ�ǰ�棩
//@ ���û���ҵ���pPos��ppLWnd��ΪNULL
//@ û�б�Ҫ�����ش��������ڣ�����������ƶ��жϣ�����������ж�
void GetWindowByPoint(int x,int y,POSITION *pPos,CLWnd **ppLWnd)
{
	//@ �Ӷ��㿪ʼ����
	//@ ���ش���Ҳ��
	ASSERT(pPos!=NULL && ppLWnd!=NULL);
	*pPos=NULL;
	*ppLWnd=NULL;
	POSITION pos;
	POSITION posCur;//@ ������¼��ǰ�ڵ��λ�ã���ΪGetPrev��pos��ָ����һ���ڵ��
	pos = g_listLogicalWnd.GetTailPosition();
	
	while(pos)
	{
		CLWnd *pLWnd=NULL;
		posCur=pos;
		pLWnd=g_listLogicalWnd.GetPrev(pos);

		if(pLWnd > g_pLWndTopMost)
		{
			Rect rect=pLWnd->GetWindowRect();
			if((rect.X <= x && x < rect.X+rect.Width) &&
				(rect.Y <= y && y < rect.Y+rect.Height) &&
				pLWnd->IsVisible() && pLWnd->IsEnable())
			{
				*pPos=posCur;
				*ppLWnd=pLWnd;
				return;
			}
		}
	}
	return;
}

//@ ����ƶ���Ѱ��������ڴ��ڣ���������Ӧ
void DUIMouseMove(int x,int y)
{
	//��������겶��Ļ���ֱ�ӷ��͸����񴰿�
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->MouseMove(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	
	if(pos==NULL){
		if(g_pLWndMouseIn !=NULL){// @����һ�������Ƶ�û�д��ڵط�
			g_pLWndMouseIn->MouseLeave();
			g_pLWndMouseIn=NULL;
		}
	}
	else
	{
		//@ �ж����¼�ĵ�ǰ���ڴ����Ƿ���ͬ
		if(pLWnd==g_pLWndMouseIn){
			pLWnd->MouseMove(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
		}
		else
		{
			//@ ����ͬ������MouseOut֪֮ͨǰ�Ĵ���
			if(g_pLWndMouseIn!=NULL){
				g_pLWndMouseIn->MouseLeave();
			}
			g_pLWndMouseIn=pLWnd;
		}
	}
}

//@ /���������£�Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonDown(int x,int y)
{
	//��������겶��Ļ���ֱ�ӷ��͸����񴰿�
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonDown(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ ���ô������õ���ǰ��
		//@ ��ɾ����Ȼ��������ò��ĩβ
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonDown(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ ����������Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonUp(int x,int y)
{
	//��������겶��Ļ���ֱ�ӷ��͸����񴰿�
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonUp(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ ���ô������õ���ǰ��
		//@ ��ɾ����Ȼ��������ò��ĩβ
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonUp(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ ������˫����Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonDblClick(int x,int y)
{
	//��������겶��Ļ���ֱ�ӷ��͸����񴰿�
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonDblClick(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ ���ô������õ���ǰ��
		//@ ��ɾ����Ȼ��������ò��ĩβ��ĩβ�����������ˣ�
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonDblClick(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ ���ò������λ��
void DUISetCapture(CLWnd *pLWnd)
{
	ASSERT(pLWnd!=NULL);
	g_pLWndSetCapture=pLWnd;
	g_pLWndMouseIn=NULL;
	::SetCapture(g_hwndContainer);
}

//@ ȡ����겶��
void DUIReleaseCapture()
{
	::ReleaseCapture();
	g_pLWndSetCapture=NULL;
}