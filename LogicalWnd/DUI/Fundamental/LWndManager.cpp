#include "stdafx.h"
#include "LWnd.h"
#include "LWndManager.h"

//@ 记录所有逻辑子窗口
//@ 将它作为全局变量使容器以外的其他窗口也能创建窗口
CList<CLWnd*> g_listLogicalWnd;

//@ Z轴次序分隔
#define g_pLWndBottom	((CLWnd*)0x1)
#define g_pLWndMiddle	((CLWnd*)0x2)
#define g_pLWndTop		((CLWnd*)0x3)
#define g_pLWndTopMost	((CLWnd*)0x4)
POSITION g_posBottom;
POSITION g_posMiddle;
POSITION g_posTop;
POSITION g_posTopMost;

//@ 记录当前鼠标所在窗口
CLWnd *g_pLWndMouseIn=NULL;
//@ 记录容器的句柄，窗口有显示自己的更新的需要
HWND g_hwndContainer=NULL;

//@ 记录要求捕获鼠标位置的窗口
CLWnd *g_pLWndSetCapture=NULL;

//内存绘图
CDC g_memDC; //首先定义一个显示设备对象
CBitmap g_memBitmap;//定义一个位图对象

//多线程限制
BOOL g_isDrawing = FALSE;//避免多个线程同时发起绘制请求，产生闪烁。比如播gif动画
						//还是会有点闪烁，可能是因为动画那边是100ms刷新一次

void DUIDrawStart();
void DUIDrawnEnd();

//@ 初始化
//@ 主要是将BOTTOM、MIDDLE和TOP三个层次的分隔加入进去
//@ 三个层次是顺序为（从头到尾）：底、中间、顶
//@ 分隔窗口位于某一层的最后面，方便在末尾添加窗口
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

	g_memDC.CreateCompatibleDC(NULL);//创建内存设备
	g_memBitmap.CreateCompatibleBitmap(&dc, nWidth, nHeight);//创建位图
	CBitmap *pOldBit=g_memDC.SelectObject(&g_memBitmap);
	
	ReleaseDC(g_hwndContainer,dc.m_hDC);
}

//@ 添加一个逻辑窗口至列表
//@ 节点中第n个窗口在第n+1个窗口下面
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

//@ 从列表删除一个逻辑窗口
//@ 注意对象不会删除
void DUIRemove(CLWnd *pLWnd)
{
	ASSERT(pLWnd!=NULL);
	POSITION pos=g_listLogicalWnd.Find(pLWnd);
	if(pos!=NULL)
		g_listLogicalWnd.RemoveAt(pos);
}

//@ 让所有子窗口进行绘制，并显示至设备
//@ 参数hDC是为了在OnPaint或者OnDraw中可以直接使用函数的CDC
void DUIDraw(BOOL redrawAll)
{
	//@ 从底部开始显示
	
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

//绘图开始，刷新画布清掉以前的东西
void DUIDrawStart()
{
	if(g_isDrawing) return;
	g_isDrawing = TRUE;

	CRect rect;
	GetWindowRect(g_hwndContainer, rect);
	CBrush br(RGB(12,34,87));
	g_memDC.FillRect(CRect(0, 0, rect.Width(), rect.Height()), &br);
}

//绘图结束，将位图显示到容器上
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

//@ 重新绘制一个窗口，并重新显示界面
void DUIRedrawWindow(CLWnd* pLWnd)
{
	ASSERT(pLWnd!=NULL);
	pLWnd->Draw();
	DUIDraw(false);
}

//@ 销毁所有窗口并删除CLWnd对象
//@ 退出程序时尽可能的不要让析构函数执行销毁操作，因为这时Gidplus可能已经关闭了。
//@ 在容器窗口销毁时调用它是一个比较好的选择
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

//@ 查找包含某个点的窗口
//@ 返回POSITION是为了调用方能够将该窗口激活（即放到前面）
//@ 如果没有找到，pPos和ppLWnd都为NULL
//@ 没有必要将隐藏窗口算在内，不管是鼠标移动判断，还是鼠标点击判断
void GetWindowByPoint(int x,int y,POSITION *pPos,CLWnd **ppLWnd)
{
	//@ 从顶层开始查找
	//@ 隐藏窗口也算
	ASSERT(pPos!=NULL && ppLWnd!=NULL);
	*pPos=NULL;
	*ppLWnd=NULL;
	POSITION pos;
	POSITION posCur;//@ 用来记录当前节点的位置，因为GetPrev后pos是指向下一个节点的
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

//@ 鼠标移动，寻找鼠标所在窗口，并另其响应
void DUIMouseMove(int x,int y)
{
	//设置了鼠标捕获的话，直接发送给捕获窗口
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->MouseMove(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	
	if(pos==NULL){
		if(g_pLWndMouseIn !=NULL){// @鼠标从一个窗口移到没有窗口地方
			g_pLWndMouseIn->MouseLeave();
			g_pLWndMouseIn=NULL;
		}
	}
	else
	{
		//@ 判断与记录的当前所在窗口是否相同
		if(pLWnd==g_pLWndMouseIn){
			pLWnd->MouseMove(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
		}
		else
		{
			//@ 不相同，调用MouseOut通知之前的窗口
			if(g_pLWndMouseIn!=NULL){
				g_pLWndMouseIn->MouseLeave();
			}
			g_pLWndMouseIn=pLWnd;
		}
	}
}

//@ /鼠标左键按下，寻找鼠标所在窗口，并另其响应
void DUILButtonDown(int x,int y)
{
	//设置了鼠标捕获的话，直接发送给捕获窗口
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonDown(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ 将该窗口设置到最前面
		//@ 先删除，然后添加至该层的末尾
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonDown(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ 鼠标左键弹起，寻找鼠标所在窗口，并另其响应
void DUILButtonUp(int x,int y)
{
	//设置了鼠标捕获的话，直接发送给捕获窗口
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonUp(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ 将该窗口设置到最前面
		//@ 先删除，然后添加至该层的末尾
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonUp(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ 鼠标左键双击，寻找鼠标所在窗口，并另其响应
void DUILButtonDblClick(int x,int y)
{
	//设置了鼠标捕获的话，直接发送给捕获窗口
	if(g_pLWndSetCapture!=NULL){
		g_pLWndSetCapture->LButtonDblClick(x-g_pLWndSetCapture->GetWindowRect().X,y-g_pLWndSetCapture->GetWindowRect().Y);
		return;
	}
	
	POSITION pos;
	CLWnd *pLWnd;
	GetWindowByPoint(x,y,&pos,&pLWnd);
	if(pos!=NULL){
		//@ 将该窗口设置到最前面
		//@ 先删除，然后添加至该层的末尾（末尾就是最上面了）
		g_listLogicalWnd.RemoveAt(pos);
		DUIAdd(pLWnd);
		pLWnd->LButtonDblClick(x-pLWnd->GetWindowRect().X,y-pLWnd->GetWindowRect().Y);
	}
}

//@ 设置捕获鼠标位置
void DUISetCapture(CLWnd *pLWnd)
{
	ASSERT(pLWnd!=NULL);
	g_pLWndSetCapture=pLWnd;
	g_pLWndMouseIn=NULL;
	::SetCapture(g_hwndContainer);
}

//@ 取消鼠标捕获
void DUIReleaseCapture()
{
	::ReleaseCapture();
	g_pLWndSetCapture=NULL;
}