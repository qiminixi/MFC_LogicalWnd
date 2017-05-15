/*
*@ 该文件中对所有逻辑子窗口进行管理
*/
#ifndef LWND_MANAGER_H
#define LWND_MANAGER_H


class CLWnd;

void DUIInit(HWND hwndContainer);	//@ 初始化
void DUIAdd(CLWnd *pLWnd);			//@ 添加一个逻辑窗口至列表
void DUIRemove(CLWnd *pLWnd);		//@ 从列表删除一个逻辑窗口
void DUIDraw(BOOL bRedrawAll = TRUE);			//@ 让所有子窗口进行绘制，并显示至设备
void DUIRedrawWindow(CLWnd *pLWnd);	//@ 重绘一个窗口
void DUISetCapture(CLWnd *pLWnd);	//@ 设置鼠标捕获
void DUIReleaseCapture();			//@ 取消鼠标捕获

void DUICleanup();						//@ 销毁所有窗口，并删除CLWnd对象
void DUIMouseMove(int x,int y);			//@ 鼠标移动，寻找鼠标所在窗口，并另其响应
void DUILButtonDown(int x,int y);		//@ 鼠标左键按下，寻找鼠标所在窗口，并另其响应
void DUILButtonUp(int x,int y);			//@ 鼠标左键弹起，寻找鼠标所在窗口，并另其响应
void DUILButtonDblClick(int x,int y);	//@ 鼠标左键双击，寻找鼠标所在窗口，并另其响应



extern CList<CLWnd*> g_listLogicalWnd;//记录所有逻辑子窗口

#endif LWND_MANAGER_H