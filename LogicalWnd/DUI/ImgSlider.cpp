// ImgSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImgSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CImgSlider
CImgSlider::CImgSlider()
{
	Init();
}
CImgSlider::CImgSlider(CLWnd *pParent)
{
	VERIFY(pParent!=NULL);
	m_pParent=pParent;
	Init();
}

//使用默认值进行初始化
void CImgSlider::Init()
{
	m_pParent=NULL;
	m_bHorizontal=TRUE;
	m_nMax=100;
	m_nMin=0;
	m_nPos=0;
	
	m_nMarginLeft=0;
	m_nMarginTop=0;
	m_nMarginRight=0;
	m_nMarginBottom=0;
	m_nThumbMarginLT=0;
	m_nThumbMarginRB=0;
	m_nThumbHW=0;
	
	m_nThumbPos=0;
	m_bLButtonDown=FALSE;
	m_bEnableOutline=FALSE;
	m_bEnableBg=FALSE;
	m_clrBg=RGB(128,0,128);
	m_clrOutline=RGB(255,255,255);

	m_pImgChannel=NULL;
	m_pImgActive=NULL;
	m_pImgThumb=NULL;
}
void CImgSlider::SetParent(CLWnd *pParent)
{
	VERIFY(pParent!=NULL);
	m_pParent=pParent;
}


CImgSlider::~CImgSlider()
{
	if(!m_pImgChannel){
		delete m_pImgChannel;
		m_pImgChannel=NULL;
	}
	if(!m_pImgActive){
		delete m_pImgActive;
		m_pImgActive=NULL;
	}
	if(!m_pImgThumb){
		delete m_pImgThumb;
		m_pImgActive=NULL;
	}
}

//创建轨道条,如果bVert为true,为竖直轨道条，否则为水平轨道条
BOOL CImgSlider::Create(BOOL bHorizontal,int x,int y,int w,int h,CLWnd *pParent)
{
	m_bHorizontal=bHorizontal;
	//m_nThumbPos会在OnSize中计算
	return CLWnd::Create(x,y,w,h);
}

//设置轨道的高度、滑块的尺寸
void CImgSlider::SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW)
{
	if(nMarginLeft<0)		nMarginLeft=0;
	if(nMarginTop<0)		nMarginTop=0;
	if(nMarginRight<0)		nMarginRight=0;
	if(nMarginBottom<0)		nMarginBottom=0;
	if(nThumbMarginLT<0)	nThumbMarginLT=0;
	if(nThumbMarginRB<0)	nThumbMarginRB=0;
	if(m_nThumbHW<0)		m_nThumbHW=0;
	m_nMarginLeft=nMarginLeft;
	m_nMarginTop=nMarginTop;
	m_nMarginRight=nMarginRight;
	m_nMarginBottom=nMarginBottom;
	m_nThumbMarginLT=nThumbMarginLT;
	m_nThumbMarginRB=nThumbMarginRB;
	m_nThumbHW=nThumbHW;
	m_nThumbPos=m_bHorizontal ? m_nMarginLeft : m_nMarginTop;
	
	//改变nThumbPos的值，如果窗口已经创建的话
	if(IsWindowCreated())
	{
		SetPos(m_nPos);
		UpdateWindow();
	}
}
//启用轮廓线绘制功能，帮助调整控件中的位图
void CImgSlider::EnableOutline(BOOL bEnable)
{
	m_bEnableOutline=bEnable;
	if(IsWindowCreated()) UpdateWindow();
}
//设置轮廓线颜色
void CImgSlider::SetOutLineColor(COLORREF clr)
{
	m_clrOutline=clr;
	if(IsWindowCreated()) UpdateWindow();
}

//启用背景绘制功能
void CImgSlider::EnableBg(BOOL bEnable/*=TRUE*/)
{
	m_bEnableBg=bEnable;
	if(IsWindowCreated()) UpdateWindow();
}
//设置背景颜色
void CImgSlider::SetBgColor(COLORREF clr)
{
	m_clrBg=clr;
	if(IsWindowCreated()) UpdateWindow();
}


//从资源载入图像
//lpszType是资源类型
BOOL CImgSlider::LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType)
{
	if(m_pImgChannel){ delete m_pImgChannel; m_pImgChannel=NULL;}
	ImageFromIDResource(nIDChannel,lpszType,m_pImgChannel);
	if(m_pImgChannel){
		if(m_pImgChannel->GetLastStatus()!=Ok){
			delete m_pImgChannel; m_pImgChannel=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgActive){ delete m_pImgActive; m_pImgActive=NULL;}
	ImageFromIDResource(nIDActive,lpszType,(Image*&)m_pImgActive);
	if(m_pImgActive){
		if(m_pImgActive->GetLastStatus()!=Ok){
			delete m_pImgActive; m_pImgActive=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgThumb){ delete m_pImgThumb; m_pImgThumb=NULL;}
	ImageFromIDResource(nIDThumb,lpszType,(Image*&)m_pImgThumb);
	if(m_pImgThumb){
		if(m_pImgThumb->GetLastStatus()!=Ok){
			delete m_pImgThumb; m_pImgThumb=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(IsWindowCreated()) UpdateWindow();
	return TRUE;
}
//从文件载入图像(GDI+)
BOOL CImgSlider::LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb)
{
	//当文件不存在时依旧返回Image对象，须通过status判断读取失败还是成功
	if(m_pImgChannel){ delete m_pImgChannel; m_pImgChannel=NULL;}
	m_pImgChannel=Image::FromFile( lpszChannel); 
	if(m_pImgChannel){
		if(m_pImgChannel->GetLastStatus()!=Ok){
			delete m_pImgChannel; m_pImgChannel=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgActive){ delete m_pImgActive; m_pImgActive=NULL;}
	m_pImgActive=Image::FromFile(lpszActive); 
	if(m_pImgActive){
		if(m_pImgActive->GetLastStatus()!=Ok){
			delete m_pImgActive; m_pImgActive=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgThumb){ delete m_pImgThumb; m_pImgThumb=NULL;}
	m_pImgThumb=Image::FromFile(lpszThumb); 
	if(m_pImgThumb){
		if(m_pImgThumb->GetLastStatus()!=Ok){
			delete m_pImgThumb; m_pImgThumb=NULL;//无效的Image对象，删除
			return FALSE;
		}
	}
	else return FALSE;
	
	if(IsWindowCreated()) UpdateWindow();
	return TRUE;
}



//设置滚动范围
void CImgSlider::SetRange(int nMin,int nMax,BOOL bRedraw/*=TRUE*/)
{
	if(nMin>nMax || nMin<0 || nMax<0)
		return;
	m_nMin=nMin;
	m_nMax=nMax;
	if(bRedraw){
		m_nPos=nMin;
		SetPos(m_nPos);//更改m_nThumbPos的位置
	}
	else{
		m_nPos=GetPos();//更新m_nPos,m_nThumbPos标识的滑块位置不变
	}
	
	if(IsWindowCreated()) UpdateWindow();	
}

//获取滚动范围
void CImgSlider::GetRange(int &nMin,int &nMax)
{
	nMin=m_nMin;
	nMax=m_nMax;
}

//设置当前位置
void CImgSlider::SetPos(int n)
{
	if(n<m_nMin || n>m_nMax)
		return;
	m_nPos=n;
	
	if(!IsWindowCreated())//如果窗口还有没有创建，m_nThumbPos的计算就放弃，交给OnSize
		return;
	CRect rectClient;
	GetClientRect(rectClient);
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)rectClient.Width()-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(dRate*(n-m_nMin))+m_nMarginLeft;
		double f=(dRate*(n-m_nMin))+m_nMarginLeft;
		if(f>double(m_nThumbPos))//如果计算的不是整数，比如42*1.6=67.2，那么值42的底线为68，67号位置是41
			m_nThumbPos++;//说白了修正操作就是将m_nThumbPos置在刚好计算出dRate的位置
	}
	else{
		dRate=((float)rectClient.Height()-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(rectClient.Height()-m_nMarginBottom-m_nThumbHW-dRate*(n-m_nMin));
		//乘积被减时会因为小数不为零自动向上移一个单位避免误差，如果减的是取整后的乘积则不会，且会出现水平slider的误差
		//比如值n的位置42-11.6=30.4，30就是n值的位置，31是n-1的位置
	}
	UpdateWindow();
}

//获取当前位置
int CImgSlider::GetPos()
{
	if(!IsWindowCreated())
		return m_nPos;
	CRect rectClient;
	GetClientRect(rectClient);
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)rectClient.Width()-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nPos=(int)((m_nThumbPos-m_nMarginLeft)/dRate+m_nMin);
		return m_nPos;
	}
	else{
		dRate=((float)rectClient.Height()-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nPos=(int)((rectClient.Height()-m_nMarginBottom-m_nThumbHW-m_nThumbPos)/dRate)+m_nMin;
		return m_nPos;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CImgSlider message handlers


void CImgSlider::Draw()
{
	Graphics graphics(GetBitmap());
	
	//@ 先把位图重置为透明
	ResetBitmap();
	
	Color clr(GetRValue(m_clrBg),GetGValue(m_clrBg),GetBValue(m_clrBg));
	SolidBrush br(clr);
	if(m_bEnableBg)
		graphics.FillRectangle(&br,0,0,GetWindowRect().Width,GetWindowRect().Height);
	if(m_bHorizontal) DrawHorizontal(graphics);
	else	DrawVert(graphics);
}

//绘制水平方向的控件//GDI+
void CImgSlider::DrawHorizontal(Graphics &g)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	Pen pen(Color(GetRValue(m_clrOutline),GetGValue(m_clrOutline),GetBValue(m_clrOutline),1));
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,rectClient.top,rectClient.left,rectClient.Width()-1,rectClient.Height()-1);//多画了一个px
	
	//绘制轨道
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);
	//使图像没有载入或者载入失败时轮廓线正常显示
	if(m_pImgChannel==NULL || m_pImgActive==NULL || m_pImgThumb==NULL)
	{
		h=rectClient.Height()-m_nThumbMarginLT-m_nThumbMarginRB;
		if(m_bEnableOutline)
			g.DrawRectangle(&pen,m_nThumbPos,m_nThumbMarginLT,m_nThumbHW-1,h-1);
		return;
	}
	//在x方向上填充轨道
	int i=0;
	int nImgW=m_pImgChannel->GetWidth();
	int nImgH=m_pImgChannel->GetHeight();
	if(nImgH>h) nImgH=h;
	while((i+1)*nImgW<=w)
	{
		g.DrawImage(m_pImgChannel,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(w>i*nImgW)
		g.DrawImage(m_pImgChannel,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,w-i*nImgW,nImgH,UnitPixel);
	
	//绘制Active，在x方向上填充，水平方向上不能超出Active区域
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgH>h) nImgH=h;
	//x方向上填充active
	while((i+1)*nImgW<=m_nThumbPos-m_nMarginLeft)
	{
		g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(m_nThumbPos-m_nMarginLeft>i*nImgW)
	g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,m_nThumbPos-m_nMarginLeft-i*nImgW,nImgH,UnitPixel);

	//绘制滑块
	//根据滑块的大小画而不是根据图像的大小画，因此不需要对图像进行裁剪
	h=rectClient.Height()-m_nThumbMarginLT-m_nThumbMarginRB;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nThumbPos,m_nThumbMarginLT,m_nThumbHW-1,h-1);
	g.DrawImage(m_pImgThumb,m_nThumbPos,m_nThumbMarginLT,0,0,m_nThumbHW,h,UnitPixel);
}

void CImgSlider::DrawVert(Graphics &g)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	Pen pen(Color(GetRValue(m_clrOutline),GetGValue(m_clrOutline),GetBValue(m_clrOutline),1));
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,rectClient.top,rectClient.left,rectClient.Width()-1,rectClient.Height()-1);
	
	//绘制轨道
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);
	//使图像没有载入或者载入失败时轮廓线正常显示
	if(m_pImgChannel==NULL || m_pImgActive==NULL || m_pImgThumb==NULL)
	{
		w=rectClient.Width()-m_nThumbMarginLT-m_nThumbMarginRB;
		if(m_bEnableOutline)
			g.DrawRectangle(&pen,m_nThumbMarginLT,m_nThumbPos,w-1,m_nThumbHW-1);
		return;
	}
	//在y方向上填充轨道，从上到下
	int i=0;
	int nImgW=m_pImgChannel->GetWidth();
	int nImgH=m_pImgChannel->GetHeight();
	if(nImgW>w) nImgW=w;
	while((i+1)*nImgH<=h)
	{
		g.DrawImage(m_pImgChannel,m_nMarginLeft,nImgH*i+m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(h>i*nImgH)
		g.DrawImage(m_pImgChannel,m_nMarginLeft,nImgH*i+m_nMarginTop,0,0,nImgW,h-i*nImgH,UnitPixel);
	
	//绘制Active，在x方向上填充，从上到下
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgW>w) nImgW=w;
	while((i+1)*nImgH<=h-m_nThumbHW+m_nMarginTop-m_nThumbPos)//Active区域在滑块下面
	{
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(h-m_nThumbHW+m_nMarginTop-m_nThumbPos>i*nImgH)
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,nImgH-(h+m_nMarginTop-m_nThumbPos-m_nThumbHW-i*nImgH),
			nImgW,h+m_nMarginTop-m_nThumbPos-m_nThumbHW-i*nImgH,UnitPixel);
	
	//绘制滑块
	//根据滑块的大小画而不是根据图像的大小画，因此不需要对图像进行裁剪
	w=rectClient.Width()-m_nThumbMarginLT-m_nThumbMarginRB;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nThumbMarginLT,m_nThumbPos,w-1,m_nThumbHW-1);
	g.DrawImage(m_pImgThumb,m_nThumbMarginLT,m_nThumbPos,0,0,w,m_nThumbHW,UnitPixel);
}

void CImgSlider::GetClientRect(CRect &rect)
{
	Rect rc=GetWindowRect();
	rect.left=0;
	rect.top=0;
	rect.right=rc.Width;
	rect.bottom=rc.Height;
}

void CImgSlider::LButtonDown(int x,int y) 
{
	CPoint point(x,y);
	CRect rectClient;
	GetClientRect(rectClient);
	if(m_bHorizontal)//水平滑杆条
	{
		int top,bottom;//滑块的高
		top=m_nThumbMarginLT;//滑块的高
		bottom=rectClient.bottom-m_nThumbMarginRB;
		if( (m_nThumbPos<=point.x && point.x<=m_nThumbPos+m_nThumbHW) &&
			(top<=point.y && point.y<=bottom) )//判断是否在滑块中，在的话不移动
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//显示按下去了的滑块图标
			UpdateWindow();
			return;
		}
		top=m_nMarginTop;//滑道的高
		bottom=rectClient.bottom-m_nMarginBottom;//滑道的高
		if( (m_nMarginLeft<=point.x && point.x<=rectClient.Width()-m_nMarginRight) &&
			(top<=point.y && point.y<=bottom) )//判断是否在轨道中，在的话移动（即鼠标在轨道中单击了）
		{
			//让中心为于point.x处
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//位于(~,滑块宽度/2+左空白]，将滑块置于最左边,~为正无穷或者负无穷大
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//正常位置，位于(滑块宽度/2+左空白，轨道长度-滑块宽度/2-右空白)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//位于[轨道长度-滑块宽度/2-右空白,~]，将滑块置于最右边
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetPos(m_nPos);//修正m_nThumbPos
			SetCapture();
			UpdateWindow();
		}
	}
	else
	{
		int left,right;//滑块的宽
		left=m_nThumbMarginLT;//滑块的宽
		right=rectClient.right-m_nThumbMarginRB;
		if( (left<=point.x && point.x<=right) &&
			(m_nThumbPos<=point.y && point.y<=m_nThumbPos+m_nThumbHW) )//判断是否在滑块中，在的话不移动
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//显示按下去了的滑块图标
			UpdateWindow();
			return;
		}
		left=m_nMarginLeft;//滑道的宽
		right=rectClient.right-m_nMarginRight;//滑道的宽
		if( (left<=point.x && point.x<=right) &&
			(m_nMarginTop<=point.y && point.y<=rectClient.Height()-m_nMarginRight) )//判断是否在轨道中，在的话移动（即鼠标在轨道中单击了）
		{
			//让中心为于point.y处
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//位于(~,滑块高度/2+上空白]，将滑块置于最上面
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//正常位置，位于(滑块高度/2+上空白，轨道高度-滑块高度/2-下空白)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//位于[轨道高度-滑块高度/2-下空白,~)，将滑块置于最下边
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetPos(m_nPos);//修正m_nThumbPos
			SetCapture();
			UpdateWindow();
		}
	}
}

void CImgSlider::LButtonUp(int x,int y) 
{
	CPoint point(x,y);
	if(m_bLButtonDown)
	{
		m_bLButtonDown=FALSE;
		ReleaseCapture();
		//if(m_pParent) m_pParent->PostMessage(UM_IMGSLIDER_RELEASED_CAPTURE,(WPARAM)GetSafeHwnd());
	}
}

void CImgSlider::MouseMove(int x,int y) 
{
	CPoint point(x,y);
	CRect rectClient;
	GetClientRect(rectClient);
	
	if(m_bHorizontal)
	{
		//由于用了SetCapture，所以不需要在y方向上判断
		if(m_bLButtonDown)
		{
			//让中心为于point.x处
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//位于(~,滑块宽度/2+左空白]，将滑块置于最左边,~为正无穷或者负无穷大
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//正常位置，位于(滑块宽度/2+左空白，轨道长度-滑块宽度/2-右空白)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//位于[轨道长度-滑块宽度/2-右空白,~]，将滑块置于最右边
			//if(m_pParent) m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			m_nPos=GetPos();
			SetPos(m_nPos);//修正m_nThumbPos
			UpdateWindow();
		}
	}
	else
	{
		if(m_bLButtonDown)
		{
			//由于用了SetCapture，所以不需要在x方向上判断
			//让中心为于point.y处
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//位于(~,滑块高度/2+上空白]，将滑块置于最上面
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//正常位置，位于(滑块高度/2+上空白，轨道高度-滑块高度/2-下空白)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//位于[轨道高度-滑块高度/2-下空白,~)，将滑块置于最下边
			//if(m_pParent) m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			m_nPos=GetPos();
			SetPos(m_nPos);//修正m_nThumbPos
			UpdateWindow();
		}
	}
}

void CImgSlider::Resize(int w,int h)
{
	//根据m_nPos计算新的m_nThumbPos
	SetPos(m_nPos);
	CLWnd::Resize(w,h);
}
