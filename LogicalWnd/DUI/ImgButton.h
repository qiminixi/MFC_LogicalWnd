#ifndef PNGBUTTON_H
#define PNGBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif 

//@ 按钮大小会自动调整为图像大小
//@ 如果按钮为Disable态，将不处理任何鼠标消息

#define LUM_IMGBUTTON_LBUTTON_DOWN	100
#define LUM_IMGBUTTON_LBUTTON_UP	101

//@ 按钮状态
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
	
	//@ 重载
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
	
	//@ 属性
protected:
	Image		*m_pImage;
	ButtonState	m_buttonstate;
	CLWnd		*m_pParent;//@ 接收通知消息的父窗口
};
#endif

