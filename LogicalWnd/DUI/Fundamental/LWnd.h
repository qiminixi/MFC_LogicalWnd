/**
 *@ 该文件中对CLWnd类进行定义，CLWnd是所有逻辑窗口的基类
 *@ 所有窗口都应该采用动态创建的方式创建 
 *@ 注意如果窗口创建了并且没有销毁，框架会在程序退出前自动销毁窗口并删除对象
 *对于动态创建窗口，如果调用了Destory进行了销毁，那么对象的销毁操作也得手动进行
 *@ 这是因为记录窗口的位图必须在GDI+关闭之前进行，静态对象是程序退出时销毁的，
 *这时GDI+已经关闭，销毁操作会失败，导致内存泄露。为了避免用户手动进行窗口销毁
 *和删除操作（这在窗口非常多的时候会变得很麻烦），要求窗口全部进行动态创建，
 *这样善后工作交给框架就可以了。
**/

#ifndef LWND_H
#define LWND_H

//Z轴层次
#define LWND_ZAXIS_LEVEL_BOTTOM		0	//底部
#define LWND_ZAXIS_LEVEL_MIDDLE		1	//中间
#define LWND_ZAXIS_LEVEL_TOP		2	//最上面
#define LWND_ZAXIS_LEVEL_TOPMOST	3	//最最上面

class CLWnd
{
//构造函数
public:
	CLWnd();
	virtual ~CLWnd();
	
//方法
public:
	BOOL Create(int nX,int nY,int nWidth,int nHeight,
	BOOL bIsVisible=TRUE, int nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE);//创建窗口
	virtual void Destroy();//销毁窗口
	
	//操作响应
	virtual void Draw();//绘制窗口
	virtual void MouseMove(int x,int y);
	virtual void MouseLeave();
	virtual void LButtonDown(int x,int y);
	virtual void LButtonUp(int x,int y);
	virtual void LButtonDblClick(int x,int y);
	
	//事件处理
	//到目前，事件由其他窗口发送以便通信，系统不会发送消息
	virtual UINT MessageProcessing(UINT nMsgId,WPARAM wParam=0,LPARAM lParam=0);
	
	//操作
	virtual void Move(int nX,int nY,int nWidth=-1,int nHeight=-1);//移动窗口
	virtual void Show(BOOL bShow=TRUE);//显示/隐藏窗口
	virtual void Resize(int nWidth,int nHeight);//调整窗口大小
	virtual void Resized(int nWidth,int nHeight){}//窗口大小改变了会调用它
	void UpdateWindow();//更新窗口，不适用Redraw，是因为Draw由框架调用
	void SetCapture();//设置鼠标捕获
	void ReleaseCapture();//取消鼠标捕获
	void ResetBitmap();//将位图重置为全透明状态
	void Enable(BOOL bEnable){m_bIsEnable=bEnable;}//设为启用/不可用窗口
	
	//属性访问
	const Rect & GetWindowRect(){return m_rcWnd;}					//获取窗口区域
	BOOL IsVisible(){return m_bIsVisible;}							//窗口是否可见
	BOOL IsEnable(){return m_bIsEnable;}							//窗口是否可用
	Bitmap * GetBitmap(void){return m_pWndBitmap;}					//获取位图
	UINT GetZAxisLevel(){return m_nZAxisLevel;}						//获取窗口Z轴层次
	void SetZAxisLevel(UINT nZAxisLevel);							//设置窗口Z轴层次
	const CString & GetWindowText(){return m_strText;};				//获取窗口文本
	void SetWindowText(LPCTSTR lpszText){m_strText=lpszText;}		//设置窗口文本
	BOOL IsWindowCreated(){return m_pWndBitmap!=NULL?TRUE:FALSE;}	//窗口是否创建
	
	
//属性
private:
	Rect m_rcWnd;//窗口区域（通过矩形记录位置和大小），坐标是相对父窗口（即容器）的
	UINT m_nZAxisLevel;//Z轴层次，在Z轴上的显示顺序为窗口列表中的记录顺序
	BOOL m_bIsVisible;//窗口是否可见
	BOOL m_bIsEnable;//窗口是否可用
	Bitmap *m_pWndBitmap;//窗口位图
	CString m_strText;//文字
};

#endif