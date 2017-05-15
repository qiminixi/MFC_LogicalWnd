#ifndef PNGBUTTON_H
#define PNGBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif 

//@ ��ť��С���Զ�����Ϊͼ���С
//@ �����ťΪDisable̬�����������κ������Ϣ

#define LUM_IMGBUTTON_LBUTTON_DOWN	100
#define LUM_IMGBUTTON_LBUTTON_UP	101

//@ ��ť״̬
enum ButtonState
{
	ButtonStateNormal,
	ButtonStateHover,
	ButtonStateActive,
	ButtonStateDisable 
};

class CImgButton : public CLWnd
{
public:
	CImgButton();
	virtual ~CImgButton();
	
	//@ ����
public:
	BOOL Create(int x,int y,CLWnd *pParent,int nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE);
	void Destroy();
	void Draw();
	void MouseMove(int x,int y);
	void MouseLeave();
	void LButtonDown(int x,int y);
	void LButtonUp(int x,int y);
	void LButtonDblClick(int x,int y);
	
	// @...
public:
	BOOL LoadImage(WCHAR *szFileName);	
	BOOL LoadImage(UINT nIDResource,LPCTSTR szType);
	void Enable(BOOL bEnable);
	
	//@ ����
protected:
	Image		*m_pImage;
	ButtonState	m_buttonstate;
	CLWnd		*m_pParent;//@ ����֪ͨ��Ϣ�ĸ�����
};
#endif

