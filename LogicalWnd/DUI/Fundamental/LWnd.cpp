#include "stdafx.h"
#include "LWnd.h"
#include "LWndManager.h"

CLWnd::CLWnd()
{
	m_pWndBitmap=NULL;
	m_nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE;
}
CLWnd::~CLWnd()
{
	//@ 退出程序时尽可能的不要让析构函数执行销毁操作，因为这时Gidplus可能已经关闭了。
	ASSERT(m_pWndBitmap==NULL);//该断言用来提示在析构函数中进行销毁
	if(m_pWndBitmap!=NULL)
		Destroy();
}

//@ 事件处理
//@ 重载该函数已使窗口具有消息处理能力
//@ 在函数中判断消息类型，并调用相应函数
//@ wParam和lParam为消息参数
UINT CLWnd::MessageProcessing(UINT nMsgId,WPARAM wParam,LPARAM lParam)
{
	return 0;
}

//@ 创建
BOOL CLWnd::Create(int nX,int nY,int nWidth,int nHeight,BOOL bIsVisible,int nZAxisLevel)
{
	//@通过m_pWndBitmap判断窗口是否已经创建
	ASSERT(m_pWndBitmap==NULL);
	if(nWidth<=0) nWidth=1;
	if(nHeight<=0) nHeight=1;
	
	m_rcWnd.X=nX;
	m_rcWnd.Y=nY;
	m_rcWnd.Width=nWidth;
	m_rcWnd.Height=nHeight;
	m_bIsVisible=bIsVisible;
	if(nZAxisLevel<LWND_ZAXIS_LEVEL_BOTTOM || nZAxisLevel>LWND_ZAXIS_LEVEL_TOPMOST)
		m_nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE;
	else m_nZAxisLevel=nZAxisLevel;
	
	m_pWndBitmap=new Bitmap(nWidth,nHeight,PixelFormat32bppARGB );
	Status stat=m_pWndBitmap->GetLastStatus();
	if(stat!=Ok){
		delete m_pWndBitmap;
		m_pWndBitmap=NULL;
		return FALSE;
	}
	DUIAdd(this);
	return TRUE;
}

void CLWnd::Destroy()
{
	if(m_pWndBitmap!=NULL)
	{
		delete m_pWndBitmap;
		m_pWndBitmap=NULL;
	}
	DUIRemove(this);
}

void CLWnd::Draw()
{
	ASSERT(m_pWndBitmap!=NULL);
	Graphics graphics(m_pWndBitmap);
	SolidBrush br(Color(204,51,153));
	Pen pen(Color(0,0,0));
	graphics.FillRectangle(&br,0,0,m_rcWnd.Width,m_rcWnd.Height);
	graphics.DrawRectangle(&pen,0,0,m_rcWnd.Width-1,m_rcWnd.Height-1);
	if(!m_strText.IsEmpty())
	{
		FontFamily fontfamily(L"宋体");
		Font font(&fontfamily,10);
		SolidBrush br(Color(0,0,0));
		graphics.DrawString(m_strText,m_strText.GetLength(),&font,PointF(0.0f,0.0f),&br);
	}
}

//@ 更新窗口
void CLWnd::UpdateWindow()
{
	DUIRedrawWindow(this);
}

BOOL g_bLBtnDown=FALSE;
Point g_ptPre;

//@ 移动窗口
//@ 如果nWidth或者nHeight小于等于0，就保持窗口大小不变
//@ 如果窗口大小改变，那么位图也要调整大小
void CLWnd::Move(int nX,int nY,int nWidth,int nHeight)
{
	m_rcWnd.X=nX;
	m_rcWnd.Y=nY;
	if(nWidth >0 && nHeight >0)
	{
		if(nWidth!=m_rcWnd.Width || nHeight!=m_rcWnd.Height)
		{
			m_rcWnd.Width=nWidth;
			m_rcWnd.Height=nHeight;
			delete m_pWndBitmap;
			m_pWndBitmap=new Bitmap(nWidth,nHeight,PixelFormat32bppARGB);
			Resized(nWidth,nHeight);
		}
	}
	UpdateWindow();
}

//@ 调整窗口大小
//@ 如果窗口大小改变，那么位图也要调整大小
void CLWnd::Resize(int nWidth,int nHeight)
{
	if(nWidth<0 || nHeight<0)
		return;
	if(nWidth!=m_rcWnd.Width || nHeight!=m_rcWnd.Height)
	{
		m_rcWnd.Width=nWidth;
		m_rcWnd.Height=nHeight;
		delete m_pWndBitmap;
		m_pWndBitmap=new Bitmap(nWidth,nHeight,PixelFormat32bppARGB);
		Resized(nWidth,nHeight);
		UpdateWindow();
		return;
	}
}

//@ 显示/隐藏窗口
void CLWnd::Show(BOOL bShow)
{
	if(m_bIsVisible==bShow)	return;
	m_bIsVisible=bShow;
	UpdateWindow();
}

//@ 将位图重置为全透明状态
void CLWnd::ResetBitmap()
{
	if(!IsWindowCreated()) return;
	BitmapData bd;
	Rect rc(0,0,m_rcWnd.Width,m_rcWnd.Height);
	if(m_pWndBitmap->LockBits(&rc,ImageLockModeWrite,PixelFormat32bppARGB,&bd)==Ok)
	{
		memset(bd.Scan0,0,4*bd.Height*bd.Width);
		m_pWndBitmap->UnlockBits(&bd);
	}
}

//@ 设置窗口Z轴层次
void CLWnd::SetZAxisLevel(UINT nZAxisLevel)
{
	m_nZAxisLevel=nZAxisLevel;
	DUIRemove(this);
	DUIAdd(this);
}


//@ 设置鼠标捕获
void CLWnd::SetCapture()
{
	DUISetCapture(this);
}
void CLWnd::ReleaseCapture()
{
	DUIReleaseCapture();
}

//@ 鼠标在窗口中移动
void CLWnd::MouseMove(int x,int y)
{
	if(g_bLBtnDown)
	{
		Move(m_rcWnd.X+x-g_ptPre.X+1,m_rcWnd.Y+y-g_ptPre.Y);
	}
	TRACE("MouseMove: [%d](%d,%d)\n",m_rcWnd.Width,x,y);
}

//@ 鼠标移出窗口
//@ 没有鼠标位置作为参数，一是没有必要，二是使用TRACKMOUSEEVENT，系统通知
//事件中没有给鼠标坐标，如果用GetCursor获取的话，结果可能获取的光标还在窗口内
void CLWnd::MouseLeave()
{
	TRACE("MouseLeave: [%d]\n",m_rcWnd.Width);
	g_bLBtnDown=FALSE;
}

//@ 鼠标左键按下
void CLWnd::LButtonDown(int x,int y)
{
	SetCapture();
	TRACE("LButtonDown,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
	g_ptPre=Point(x,y);
	g_bLBtnDown=TRUE;
}
//@ 鼠标左键弹起
void CLWnd::LButtonUp(int x,int y)
{
	TRACE("LButtonUp,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
	g_bLBtnDown=FALSE;
	ReleaseCapture();
	Resize(m_rcWnd.Width+2,m_rcWnd.Height+2);
}

//@ 鼠标双击
void CLWnd::LButtonDblClick(int x,int y)
{
	TRACE("LButtonDblClick,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
}