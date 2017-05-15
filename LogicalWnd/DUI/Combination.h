//@ 该文件实现对话框效果
//@ 它只是将若干个窗口组合起来
//@ 如果控件是采用的动态创建，那么退出程序时框架会自动删除它们
//@ 控件要求都是动态创建的，如果存在成员变量的话，框架如果对它
//进行删除会造成错误，得额外为它调用Destroy删除它的窗口让它脱
//离框架才行，当窗口数量多时这显得很不方便。
//@ 这样一个临时的组合对象使用完毕后，先调用DestroyWindows销毁窗口，
//然后DeleteLWnds删除控件对象，最后delete掉组合对象。不在组合对象的
//析构函数中删除控件对象，是因为会影响框架的自动删除功能，在删除组合
//对象时就把控件都删了，使后面删除控件的操作错误。


//@ 消息处理
//@ 计时器
//@ gif动画
//@ 绘图闪烁

#ifndef LDIALOG_H
#define LDIALOG_H

class CLWnd;
class CCombination : public CLWnd
{
//@ 构造、析构函数
public:
	CCombination();
	virtual ~CCombination();
	
//@ 重载
public:
	UINT MessageProcessing(UINT nMsgId,WPARAM wParam=0,LPARAM lParam=0);
	BOOL Create(int nX,int nY,int nWidth,int nHeight);//@ 创建组合体
	void Move(int nX,int nY,int nWidth=-1,int nHeight=-1);//@ 移动组合体
	void Show(BOOL bShow=TRUE);//@ 显示/隐藏组合体
	
//@ 操作
public:
	void AddLWnd(CLWnd* pLWnd);
	void DeleteLWnds();
	
//@ 属性
protected:
	CList<CLWnd*> m_listCombinedLWnd;
};

#endif
