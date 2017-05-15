/*
*@ ���ļ��ж������߼��Ӵ��ڽ��й���
*/
#ifndef LWND_MANAGER_H
#define LWND_MANAGER_H


class CLWnd;

void DUIInit(HWND hwndContainer);	//@ ��ʼ��
void DUIAdd(CLWnd *pLWnd);			//@ ���һ���߼��������б�
void DUIRemove(CLWnd *pLWnd);		//@ ���б�ɾ��һ���߼�����
void DUIDraw(BOOL bRedrawAll = TRUE);			//@ �������Ӵ��ڽ��л��ƣ�����ʾ���豸
void DUIRedrawWindow(CLWnd *pLWnd);	//@ �ػ�һ������
void DUISetCapture(CLWnd *pLWnd);	//@ ������겶��
void DUIReleaseCapture();			//@ ȡ����겶��

void DUICleanup();						//@ �������д��ڣ���ɾ��CLWnd����
void DUIMouseMove(int x,int y);			//@ ����ƶ���Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonDown(int x,int y);		//@ ���������£�Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonUp(int x,int y);			//@ ����������Ѱ��������ڴ��ڣ���������Ӧ
void DUILButtonDblClick(int x,int y);	//@ ������˫����Ѱ��������ڴ��ڣ���������Ӧ



extern CList<CLWnd*> g_listLogicalWnd;//��¼�����߼��Ӵ���

#endif LWND_MANAGER_H