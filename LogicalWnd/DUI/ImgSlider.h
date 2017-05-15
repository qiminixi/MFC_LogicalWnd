/**********************************************************************
*	��飺ʹ��ͼ����Ϊ������slider�ؼ�
**********************************************************************/

#ifndef IMGSLIDER_H
#define IMGSLIDER_H

// ImgSlider.h : header file

#define _USE_GDIPLUS_  1   //�������Ҫʹ��GDI���л�ͼ��ע�͵��ú�

#define UM_IMGSLIDER_RELEASED_CAPTURE	WM_USER+57 //�������
#define UM_IMGSLIDER_SLIDED				WM_USER+58 //������
#define UM_IMGSLIDER_SET_TIPTEXT		WM_USER+59 //������ʾ�ı�

/////////////////////////////////////////////////////////////////////////////
// CImgSlider window

class CImgSlider : public CLWnd
{
//@ ���캯��
public:
	CImgSlider();
	CImgSlider(CLWnd *pParent);
	virtual~CImgSlider();
public:
	void SetParent(CLWnd *pParent);
	BOOL Create(BOOL bHorizontal,int x,int y,int w,int h,CLWnd *pParent);//����������,���bVertΪtrue,Ϊ��ֱ�����������Ϊˮƽ�����
	void SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW);//ͨ��������ؼ��ıߵĿհ����ù���ߴ磬���û���ĳߴ�
	
	BOOL LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType);//����Դ����ͼ��
	BOOL LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb);//���ļ�����ͼ��
	
	void EnableOutline(BOOL bEnable=TRUE);//���������߻��ƹ��ܣ����������ؼ��е�λͼ
	void SetOutLineColor(COLORREF clr);//������������ɫ
	void EnableBg(BOOL bEnable=TRUE);//���ñ������ƹ���
	void SetBgColor(COLORREF clr);//���ñ�����ɫ
	void SetPos(int n);//���õ�ǰλ��
	int GetPos();//��ȡ��ǰλ��
	
	void SetRange(int nMin,int nMax,BOOL bRedraw=TRUE);//���ù�����Χ���������0.���ھ��ȶ�ʧ
//���������г���������󣬸�������٣�ֻ�ܴ���һ�������,bRedrawָ���Ƿ񽫻�������0λ���ػ�
	void GetRange(int &nMin,int &nMax);//��ȡ������Χ
private:
	void Init();//ʹ��Ĭ��ֵ���г�ʼ��
	void DrawVert(Graphics &graphics);//����ˮƽ����Ŀؼ�
	void DrawHorizontal(Graphics &graphics);//������ֱ����Ŀؼ�
	void GetClientRect(CRect &rect);//��ȡ���ڿͻ�����Ϊ�˼�����ǰ����CWnd�Ĵ���

//@ ����
protected:
	CLWnd *m_pParent;
	BOOL m_bHorizontal;//�Ƿ�Ϊˮƽ�������Ĭ��TRUE
	int m_nMax;//������Χ�����ֵ�����绬����Χ[0,100]��100Ϊ���ֵ��Ĭ��100
	int m_nMin;//������Χ����Сֵ�����绬����Χ[0,100]��0Ϊ��Сֵ��Ĭ��0
	int m_nPos;//��ǰλ�ã���ס��ǰλ�ã�Ӧ�����ڴ�С����
	CRect rectWnd;//���ھ��Σ���ΪҪ��create֮ǰ���ô��ڴ�С���Ӷ���SetPos
	int m_nMarginLeft;//�����߾�ؼ���ߵľ���
	int m_nMarginTop;//����ϱ߾�ؼ��ϱߵľ���
	int m_nMarginRight;//����ұ߾�ؼ��ұߵľ���
	int m_nMarginBottom;////����±߾�ؼ��±ߵľ���
	
	int m_nThumbMarginLT;//�����ˮƽ���ˣ�Ϊ�����ϱ���Կؼ��ϱߵľ��룬�������ֱ���ˣ�Ϊ���������Կؼ���ߵľ��루LeftTop��
	int m_nThumbMarginRB;//�����ˮƽ���ˣ�Ϊ�����±���Կؼ��±ߵľ��룬�������ֱ���ˣ�Ϊ�����ұ���Կؼ��ұߵľ��루RightBottom��
	//ʹ�����ܹ�����λ���ƶ�
	int m_nThumbHW;//�����ˮƽ���ˣ�Ϊ�����ȣ��������ֱ���ͣ�Ϊ����߶�
	
	COLORREF m_clrBg;//������ɫ
	BOOL m_bEnableBg;//�Ƿ�ʹ�ñ�����ɫ���Ʊ���
	BOOL m_bEnableOutline;//�Ƿ��ڿؼ��ϻ���������
	COLORREF m_clrOutline;//��������ɫ
	int m_nThumbPos;//�����λ�ã���ͼ�ã����Ǿ�ؼ���ߣ��ϱߣ��ľ��룬�����ǻ�������λ�ö�Ӧ��ֵ
	//���绬�鴦���м䣬�������300��������Χ0~100����ôm_nCurPos��ֵΪ150
	//�������Чλ��������λ�ã����Ķ�Ӧ��ֵ���ǻ����Ӧ�Ķ�Ӧ��ֵ
	//�����λ��ȡֵ��Χ�ǣ�[0, ������� - ���鳤��]��[0, ����߶� - ����߶�]��
		
	Image *m_pImgChannel;//���ͼ��
	Image *m_pImgActive;//���ͼ��
	Image *m_pImgThumb;//����ͼ��
	
	BOOL m_bLButtonDown;//����Ƿ���


	//@ ����
protected:
	void Draw();
	void MouseMove(int x,int y);
	void LButtonDown(int x,int y);
	void LButtonUp(int x,int y);
	void Resize(int w,int h);
};

/////////////////////////////////////////////////////////////////////////////

#endif
