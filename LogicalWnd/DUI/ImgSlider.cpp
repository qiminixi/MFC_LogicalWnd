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

//ʹ��Ĭ��ֵ���г�ʼ��
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

//���������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
BOOL CImgSlider::Create(BOOL bHorizontal,int x,int y,int w,int h,CLWnd *pParent)
{
	m_bHorizontal=bHorizontal;
	//m_nThumbPos����OnSize�м���
	return CLWnd::Create(x,y,w,h);
}

//���ù���ĸ߶ȡ�����ĳߴ�
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
	
	//�ı�nThumbPos��ֵ����������Ѿ������Ļ�
	if(IsWindowCreated())
	{
		SetPos(m_nPos);
		UpdateWindow();
	}
}
//���������߻��ƹ��ܣ����������ؼ��е�λͼ
void CImgSlider::EnableOutline(BOOL bEnable)
{
	m_bEnableOutline=bEnable;
	if(IsWindowCreated()) UpdateWindow();
}
//������������ɫ
void CImgSlider::SetOutLineColor(COLORREF clr)
{
	m_clrOutline=clr;
	if(IsWindowCreated()) UpdateWindow();
}

//���ñ������ƹ���
void CImgSlider::EnableBg(BOOL bEnable/*=TRUE*/)
{
	m_bEnableBg=bEnable;
	if(IsWindowCreated()) UpdateWindow();
}
//���ñ�����ɫ
void CImgSlider::SetBgColor(COLORREF clr)
{
	m_clrBg=clr;
	if(IsWindowCreated()) UpdateWindow();
}


//����Դ����ͼ��
//lpszType����Դ����
BOOL CImgSlider::LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType)
{
	if(m_pImgChannel){ delete m_pImgChannel; m_pImgChannel=NULL;}
	ImageFromIDResource(nIDChannel,lpszType,m_pImgChannel);
	if(m_pImgChannel){
		if(m_pImgChannel->GetLastStatus()!=Ok){
			delete m_pImgChannel; m_pImgChannel=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgActive){ delete m_pImgActive; m_pImgActive=NULL;}
	ImageFromIDResource(nIDActive,lpszType,(Image*&)m_pImgActive);
	if(m_pImgActive){
		if(m_pImgActive->GetLastStatus()!=Ok){
			delete m_pImgActive; m_pImgActive=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgThumb){ delete m_pImgThumb; m_pImgThumb=NULL;}
	ImageFromIDResource(nIDThumb,lpszType,(Image*&)m_pImgThumb);
	if(m_pImgThumb){
		if(m_pImgThumb->GetLastStatus()!=Ok){
			delete m_pImgThumb; m_pImgThumb=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(IsWindowCreated()) UpdateWindow();
	return TRUE;
}
//���ļ�����ͼ��(GDI+)
BOOL CImgSlider::LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb)
{
	//���ļ�������ʱ���ɷ���Image������ͨ��status�ж϶�ȡʧ�ܻ��ǳɹ�
	if(m_pImgChannel){ delete m_pImgChannel; m_pImgChannel=NULL;}
	m_pImgChannel=Image::FromFile( lpszChannel); 
	if(m_pImgChannel){
		if(m_pImgChannel->GetLastStatus()!=Ok){
			delete m_pImgChannel; m_pImgChannel=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgActive){ delete m_pImgActive; m_pImgActive=NULL;}
	m_pImgActive=Image::FromFile(lpszActive); 
	if(m_pImgActive){
		if(m_pImgActive->GetLastStatus()!=Ok){
			delete m_pImgActive; m_pImgActive=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(m_pImgThumb){ delete m_pImgThumb; m_pImgThumb=NULL;}
	m_pImgThumb=Image::FromFile(lpszThumb); 
	if(m_pImgThumb){
		if(m_pImgThumb->GetLastStatus()!=Ok){
			delete m_pImgThumb; m_pImgThumb=NULL;//��Ч��Image����ɾ��
			return FALSE;
		}
	}
	else return FALSE;
	
	if(IsWindowCreated()) UpdateWindow();
	return TRUE;
}



//���ù�����Χ
void CImgSlider::SetRange(int nMin,int nMax,BOOL bRedraw/*=TRUE*/)
{
	if(nMin>nMax || nMin<0 || nMax<0)
		return;
	m_nMin=nMin;
	m_nMax=nMax;
	if(bRedraw){
		m_nPos=nMin;
		SetPos(m_nPos);//����m_nThumbPos��λ��
	}
	else{
		m_nPos=GetPos();//����m_nPos,m_nThumbPos��ʶ�Ļ���λ�ò���
	}
	
	if(IsWindowCreated()) UpdateWindow();	
}

//��ȡ������Χ
void CImgSlider::GetRange(int &nMin,int &nMax)
{
	nMin=m_nMin;
	nMax=m_nMax;
}

//���õ�ǰλ��
void CImgSlider::SetPos(int n)
{
	if(n<m_nMin || n>m_nMax)
		return;
	m_nPos=n;
	
	if(!IsWindowCreated())//������ڻ���û�д�����m_nThumbPos�ļ���ͷ���������OnSize
		return;
	CRect rectClient;
	GetClientRect(rectClient);
	double dRate=0;
	if(m_bHorizontal){
		dRate=((float)rectClient.Width()-m_nThumbHW-m_nMarginLeft-m_nMarginRight)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(dRate*(n-m_nMin))+m_nMarginLeft;
		double f=(dRate*(n-m_nMin))+m_nMarginLeft;
		if(f>double(m_nThumbPos))//�������Ĳ�������������42*1.6=67.2����ôֵ42�ĵ���Ϊ68��67��λ����41
			m_nThumbPos++;//˵���������������ǽ�m_nThumbPos���ڸպü����dRate��λ��
	}
	else{
		dRate=((float)rectClient.Height()-m_nThumbHW-m_nMarginTop-m_nMarginBottom)/(float(m_nMax-m_nMin));
		m_nThumbPos=(int)(rectClient.Height()-m_nMarginBottom-m_nThumbHW-dRate*(n-m_nMin));
		//�˻�����ʱ����ΪС����Ϊ���Զ�������һ����λ���������������ȡ����ĳ˻��򲻻ᣬ�һ����ˮƽslider�����
		//����ֵn��λ��42-11.6=30.4��30����nֵ��λ�ã�31��n-1��λ��
	}
	UpdateWindow();
}

//��ȡ��ǰλ��
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
	
	//@ �Ȱ�λͼ����Ϊ͸��
	ResetBitmap();
	
	Color clr(GetRValue(m_clrBg),GetGValue(m_clrBg),GetBValue(m_clrBg));
	SolidBrush br(clr);
	if(m_bEnableBg)
		graphics.FillRectangle(&br,0,0,GetWindowRect().Width,GetWindowRect().Height);
	if(m_bHorizontal) DrawHorizontal(graphics);
	else	DrawVert(graphics);
}

//����ˮƽ����Ŀؼ�//GDI+
void CImgSlider::DrawHorizontal(Graphics &g)
{
	CRect rectClient;
	GetClientRect(rectClient);
	
	Pen pen(Color(GetRValue(m_clrOutline),GetGValue(m_clrOutline),GetBValue(m_clrOutline),1));
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,rectClient.top,rectClient.left,rectClient.Width()-1,rectClient.Height()-1);//�໭��һ��px
	
	//���ƹ��
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);
	//ʹͼ��û�������������ʧ��ʱ������������ʾ
	if(m_pImgChannel==NULL || m_pImgActive==NULL || m_pImgThumb==NULL)
	{
		h=rectClient.Height()-m_nThumbMarginLT-m_nThumbMarginRB;
		if(m_bEnableOutline)
			g.DrawRectangle(&pen,m_nThumbPos,m_nThumbMarginLT,m_nThumbHW-1,h-1);
		return;
	}
	//��x�����������
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
	
	//����Active����x��������䣬ˮƽ�����ϲ��ܳ���Active����
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgH>h) nImgH=h;
	//x���������active
	while((i+1)*nImgW<=m_nThumbPos-m_nMarginLeft)
	{
		g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(m_nThumbPos-m_nMarginLeft>i*nImgW)
	g.DrawImage(m_pImgActive,nImgW*i+m_nMarginLeft,m_nMarginTop,0,0,m_nThumbPos-m_nMarginLeft-i*nImgW,nImgH,UnitPixel);

	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
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
	
	//���ƹ��
	int h,w;
	h=rectClient.Height()-m_nMarginTop-m_nMarginBottom;
	w=rectClient.Width()-m_nMarginLeft-m_nMarginRight;
	if(m_bEnableOutline)
		g.DrawRectangle(&pen,m_nMarginLeft,m_nMarginTop,w-1,h-1);
	//ʹͼ��û�������������ʧ��ʱ������������ʾ
	if(m_pImgChannel==NULL || m_pImgActive==NULL || m_pImgThumb==NULL)
	{
		w=rectClient.Width()-m_nThumbMarginLT-m_nThumbMarginRB;
		if(m_bEnableOutline)
			g.DrawRectangle(&pen,m_nThumbMarginLT,m_nThumbPos,w-1,m_nThumbHW-1);
		return;
	}
	//��y����������������ϵ���
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
	
	//����Active����x��������䣬���ϵ���
	i=0;
	nImgW=m_pImgActive->GetWidth();
	nImgH=m_pImgActive->GetHeight();
	if(nImgW>w) nImgW=w;
	while((i+1)*nImgH<=h-m_nThumbHW+m_nMarginTop-m_nThumbPos)//Active�����ڻ�������
	{
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,0,nImgW,nImgH,UnitPixel);
		i++;
	}
	if(h-m_nThumbHW+m_nMarginTop-m_nThumbPos>i*nImgH)
		g.DrawImage(m_pImgActive,m_nMarginLeft,m_nThumbPos+m_nThumbHW+nImgH*i,0,nImgH-(h+m_nMarginTop-m_nThumbPos-m_nThumbHW-i*nImgH),
			nImgW,h+m_nMarginTop-m_nThumbPos-m_nThumbHW-i*nImgH,UnitPixel);
	
	//���ƻ���
	//���ݻ���Ĵ�С�������Ǹ���ͼ��Ĵ�С������˲���Ҫ��ͼ����вü�
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
	if(m_bHorizontal)//ˮƽ������
	{
		int top,bottom;//����ĸ�
		top=m_nThumbMarginLT;//����ĸ�
		bottom=rectClient.bottom-m_nThumbMarginRB;
		if( (m_nThumbPos<=point.x && point.x<=m_nThumbPos+m_nThumbHW) &&
			(top<=point.y && point.y<=bottom) )//�ж��Ƿ��ڻ����У��ڵĻ����ƶ�
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//��ʾ����ȥ�˵Ļ���ͼ��
			UpdateWindow();
			return;
		}
		top=m_nMarginTop;//�����ĸ�
		bottom=rectClient.bottom-m_nMarginBottom;//�����ĸ�
		if( (m_nMarginLeft<=point.x && point.x<=rectClient.Width()-m_nMarginRight) &&
			(top<=point.y && point.y<=bottom) )//�ж��Ƿ��ڹ���У��ڵĻ��ƶ���������ڹ���е����ˣ�
		{
			//������Ϊ��point.x��
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//λ��(~,������/2+��հ�]�����������������,~Ϊ��������߸������
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//����λ�ã�λ��(������/2+��հף��������-������/2-�ҿհ�)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//λ��[�������-������/2-�ҿհ�,~]���������������ұ�
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetPos(m_nPos);//����m_nThumbPos
			SetCapture();
			UpdateWindow();
		}
	}
	else
	{
		int left,right;//����Ŀ�
		left=m_nThumbMarginLT;//����Ŀ�
		right=rectClient.right-m_nThumbMarginRB;
		if( (left<=point.x && point.x<=right) &&
			(m_nThumbPos<=point.y && point.y<=m_nThumbPos+m_nThumbHW) )//�ж��Ƿ��ڻ����У��ڵĻ����ƶ�
		{
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetCapture();
			//��ʾ����ȥ�˵Ļ���ͼ��
			UpdateWindow();
			return;
		}
		left=m_nMarginLeft;//�����Ŀ�
		right=rectClient.right-m_nMarginRight;//�����Ŀ�
		if( (left<=point.x && point.x<=right) &&
			(m_nMarginTop<=point.y && point.y<=rectClient.Height()-m_nMarginRight) )//�ж��Ƿ��ڹ���У��ڵĻ��ƶ���������ڹ���е����ˣ�
		{
			//������Ϊ��point.y��
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//λ��(~,����߶�/2+�Ͽհ�]������������������
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//����λ�ã�λ��(����߶�/2+�Ͽհף�����߶�-����߶�/2-�¿հ�)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//λ��[����߶�-����߶�/2-�¿հ�,~)���������������±�
			m_bLButtonDown=TRUE;
			m_nPos=GetPos();
			SetPos(m_nPos);//����m_nThumbPos
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
		//��������SetCapture�����Բ���Ҫ��y�������ж�
		if(m_bLButtonDown)
		{
			//������Ϊ��point.x��
			if(point.x<=m_nThumbHW/2+m_nMarginLeft)//λ��(~,������/2+��հ�]�����������������,~Ϊ��������߸������
				m_nThumbPos=m_nMarginLeft;
			else if( point.x>m_nThumbHW/2+m_nMarginLeft && point.x< rectClient.Width()-(m_nThumbHW/2)-m_nMarginRight)
			//����λ�ã�λ��(������/2+��հף��������-������/2-�ҿհ�)
				m_nThumbPos=point.x-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Width()-m_nThumbHW-m_nMarginRight;//λ��[�������-������/2-�ҿհ�,~]���������������ұ�
			//if(m_pParent) m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			m_nPos=GetPos();
			SetPos(m_nPos);//����m_nThumbPos
			UpdateWindow();
		}
	}
	else
	{
		if(m_bLButtonDown)
		{
			//��������SetCapture�����Բ���Ҫ��x�������ж�
			//������Ϊ��point.y��
			if(point.y<=m_nThumbHW/2+m_nMarginTop)//λ��(~,����߶�/2+�Ͽհ�]������������������
				m_nThumbPos=m_nMarginTop;
			else if( point.y>m_nThumbHW/2+m_nMarginTop && point.y< rectClient.Height()-(m_nThumbHW/2)-m_nMarginBottom)
			//����λ�ã�λ��(����߶�/2+�Ͽհף�����߶�-����߶�/2-�¿հ�)
				m_nThumbPos=point.y-(m_nThumbHW/2);
			else
				m_nThumbPos=rectClient.Height()-m_nThumbHW-m_nMarginBottom;//λ��[����߶�-����߶�/2-�¿հ�,~)���������������±�
			//if(m_pParent) m_pParent->PostMessage(UM_IMGSLIDER_SLIDED,(WPARAM)m_hWnd);
			m_nPos=GetPos();
			SetPos(m_nPos);//����m_nThumbPos
			UpdateWindow();
		}
	}
}

void CImgSlider::Resize(int w,int h)
{
	//����m_nPos�����µ�m_nThumbPos
	SetPos(m_nPos);
	CLWnd::Resize(w,h);
}
