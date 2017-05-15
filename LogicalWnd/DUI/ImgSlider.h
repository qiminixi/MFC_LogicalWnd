/**********************************************************************
*	简介：使用图像作为背景的slider控件
**********************************************************************/

#ifndef IMGSLIDER_H
#define IMGSLIDER_H

// ImgSlider.h : header file

#define _USE_GDIPLUS_  1   //如果不需要使用GDI进行绘图，注释掉该宏

#define UM_IMGSLIDER_RELEASED_CAPTURE	WM_USER+57 //滑动完成
#define UM_IMGSLIDER_SLIDED				WM_USER+58 //滑动中
#define UM_IMGSLIDER_SET_TIPTEXT		WM_USER+59 //设置提示文本

/////////////////////////////////////////////////////////////////////////////
// CImgSlider window

class CImgSlider : public CLWnd
{
//@ 构造函数
public:
	CImgSlider();
	CImgSlider(CLWnd *pParent);
	virtual~CImgSlider();
public:
	void SetParent(CLWnd *pParent);
	BOOL Create(BOOL bHorizontal,int x,int y,int w,int h,CLWnd *pParent);//创建滑竿条,如果bVert为true,为竖直轨道条，否则为水平轨道条
	void SetSizes(int nMarginLeft,int nMarginTop,int nMarginRight,int nMarginBottom,
		int nThumbMarginLT,int nThumbMarginRB,int nThumbHW);//通过设置与控件四边的空白设置轨道尺寸，设置滑块的尺寸
	
	BOOL LoadImagesFromResource(UINT nIDChannel,UINT nIDActive,UINT nIDThumb,LPCTSTR lpszType);//从资源载入图像
	BOOL LoadImagesFromFile(WCHAR *lpszChannel,WCHAR *lpszActive,WCHAR *lpszThumb);//从文件载入图像
	
	void EnableOutline(BOOL bEnable=TRUE);//启用轮廓线绘制功能，帮助调整控件中的位图
	void SetOutLineColor(COLORREF clr);//设置轮廓线颜色
	void EnableBg(BOOL bEnable=TRUE);//启用背景绘制功能
	void SetBgColor(COLORREF clr);//设置背景颜色
	void SetPos(int n);//设置当前位置
	int GetPos();//获取当前位置
	
	void SetRange(int nMin,int nMax,BOOL bRedraw=TRUE);//设置滚动范围，必须大于0.由于精度丢失
//（整数进行除法后会增大，负数会减少，只能处理一种情况）,bRedraw指明是否将滑块置于0位置重绘
	void GetRange(int &nMin,int &nMax);//获取滚动范围
private:
	void Init();//使用默认值进行初始化
	void DrawVert(Graphics &graphics);//绘制水平方向的控件
	void DrawHorizontal(Graphics &graphics);//绘制竖直方向的控件
	void GetClientRect(CRect &rect);//获取窗口客户区，为了兼容以前基于CWnd的代码

//@ 属性
protected:
	CLWnd *m_pParent;
	BOOL m_bHorizontal;//是否为水平轨道条，默认TRUE
	int m_nMax;//滑动范围的最大值，比如滑动范围[0,100]，100为最大值，默认100
	int m_nMin;//滑动范围的最小值，比如滑动范围[0,100]，0为最小值，默认0
	int m_nPos;//当前位置，记住当前位置，应付窗口大小更改
	CRect rectWnd;//窗口矩形，因为要在create之前设置窗口大小，从而让SetPos
	int m_nMarginLeft;//轨道左边距控件左边的距离
	int m_nMarginTop;//轨道上边距控件上边的距离
	int m_nMarginRight;//轨道右边距控件右边的距离
	int m_nMarginBottom;////轨道下边距控件下边的距离
	
	int m_nThumbMarginLT;//如果是水平滑杆，为滑块上边相对控件上边的距离，如果是竖直滑杆，为滑块左边相对控件左边的距离（LeftTop）
	int m_nThumbMarginRB;//如果是水平滑杆，为滑块下边相对控件下边的距离，如果是竖直滑杆，为滑块右边相对控件右边的距离（RightBottom）
	//使滑块能够随轨道位置移动
	int m_nThumbHW;//如果是水平滑杆，为滑块宽度，如果是竖直滑竿，为滑块高度
	
	COLORREF m_clrBg;//背景颜色
	BOOL m_bEnableBg;//是否使用背景颜色绘制背景
	BOOL m_bEnableOutline;//是否在控件上绘制轮廓线
	COLORREF m_clrOutline;//轮廓线颜色
	int m_nThumbPos;//滑块的位置（绘图用），是距控件左边（上边）的距离，而不是滑块所处位置对应的值
	//比如滑块处在中间，轨道长度300，滚动范围0~100，那么m_nCurPos的值为150
	//滑块的有效位置是中心位置，中心对应的值才是滑块对应的对应的值
	//滑块的位置取值范围是：[0, 轨道长度 - 滑块长度]（[0, 轨道高度 - 滑块高度]）
		
	Image *m_pImgChannel;//轨道图像
	Image *m_pImgActive;//轨道图像
	Image *m_pImgThumb;//滑块图像
	
	BOOL m_bLButtonDown;//鼠标是否按下


	//@ 重载
protected:
	void Draw();
	void MouseMove(int x,int y);
	void LButtonDown(int x,int y);
	void LButtonUp(int x,int y);
	void Resize(int w,int h);
};

/////////////////////////////////////////////////////////////////////////////

#endif
