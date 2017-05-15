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
	//@ �˳�����ʱ�����ܵĲ�Ҫ����������ִ�����ٲ�������Ϊ��ʱGidplus�����Ѿ��ر��ˡ�
	ASSERT(m_pWndBitmap==NULL);//�ö���������ʾ�����������н�������
	if(m_pWndBitmap!=NULL)
		Destroy();
}

//@ �¼�����
//@ ���ظú�����ʹ���ھ�����Ϣ��������
//@ �ں������ж���Ϣ���ͣ���������Ӧ����
//@ wParam��lParamΪ��Ϣ����
UINT CLWnd::MessageProcessing(UINT nMsgId,WPARAM wParam,LPARAM lParam)
{
	return 0;
}

//@ ����
BOOL CLWnd::Create(int nX,int nY,int nWidth,int nHeight,BOOL bIsVisible,int nZAxisLevel)
{
	//@ͨ��m_pWndBitmap�жϴ����Ƿ��Ѿ�����
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
		FontFamily fontfamily(L"����");
		Font font(&fontfamily,10);
		SolidBrush br(Color(0,0,0));
		graphics.DrawString(m_strText,m_strText.GetLength(),&font,PointF(0.0f,0.0f),&br);
	}
}

//@ ���´���
void CLWnd::UpdateWindow()
{
	DUIRedrawWindow(this);
}

BOOL g_bLBtnDown=FALSE;
Point g_ptPre;

//@ �ƶ�����
//@ ���nWidth����nHeightС�ڵ���0���ͱ��ִ��ڴ�С����
//@ ������ڴ�С�ı䣬��ôλͼҲҪ������С
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

//@ �������ڴ�С
//@ ������ڴ�С�ı䣬��ôλͼҲҪ������С
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

//@ ��ʾ/���ش���
void CLWnd::Show(BOOL bShow)
{
	if(m_bIsVisible==bShow)	return;
	m_bIsVisible=bShow;
	UpdateWindow();
}

//@ ��λͼ����Ϊȫ͸��״̬
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

//@ ���ô���Z����
void CLWnd::SetZAxisLevel(UINT nZAxisLevel)
{
	m_nZAxisLevel=nZAxisLevel;
	DUIRemove(this);
	DUIAdd(this);
}


//@ ������겶��
void CLWnd::SetCapture()
{
	DUISetCapture(this);
}
void CLWnd::ReleaseCapture()
{
	DUIReleaseCapture();
}

//@ ����ڴ������ƶ�
void CLWnd::MouseMove(int x,int y)
{
	if(g_bLBtnDown)
	{
		Move(m_rcWnd.X+x-g_ptPre.X+1,m_rcWnd.Y+y-g_ptPre.Y);
	}
	TRACE("MouseMove: [%d](%d,%d)\n",m_rcWnd.Width,x,y);
}

//@ ����Ƴ�����
//@ û�����λ����Ϊ������һ��û�б�Ҫ������ʹ��TRACKMOUSEEVENT��ϵͳ֪ͨ
//�¼���û�и�������꣬�����GetCursor��ȡ�Ļ���������ܻ�ȡ�Ĺ�껹�ڴ�����
void CLWnd::MouseLeave()
{
	TRACE("MouseLeave: [%d]\n",m_rcWnd.Width);
	g_bLBtnDown=FALSE;
}

//@ ����������
void CLWnd::LButtonDown(int x,int y)
{
	SetCapture();
	TRACE("LButtonDown,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
	g_ptPre=Point(x,y);
	g_bLBtnDown=TRUE;
}
//@ ����������
void CLWnd::LButtonUp(int x,int y)
{
	TRACE("LButtonUp,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
	g_bLBtnDown=FALSE;
	ReleaseCapture();
	Resize(m_rcWnd.Width+2,m_rcWnd.Height+2);
}

//@ ���˫��
void CLWnd::LButtonDblClick(int x,int y)
{
	TRACE("LButtonDblClick,[%d](%d,%d)\n",m_rcWnd.Width,x,y);
}