//@ ���ļ�ʵ�ֶԻ���Ч��
//@ ��ֻ�ǽ����ɸ������������
//@ ����ؼ��ǲ��õĶ�̬��������ô�˳�����ʱ��ܻ��Զ�ɾ������
//@ �ؼ�Ҫ���Ƕ�̬�����ģ�������ڳ�Ա�����Ļ�������������
//����ɾ������ɴ��󣬵ö���Ϊ������Destroyɾ�����Ĵ���������
//���ܲ��У�������������ʱ���Եúܲ����㡣
//@ ����һ����ʱ����϶���ʹ����Ϻ��ȵ���DestroyWindows���ٴ��ڣ�
//Ȼ��DeleteLWndsɾ���ؼ��������delete����϶��󡣲�����϶����
//����������ɾ���ؼ���������Ϊ��Ӱ���ܵ��Զ�ɾ�����ܣ���ɾ�����
//����ʱ�Ͱѿؼ���ɾ�ˣ�ʹ����ɾ���ؼ��Ĳ�������


//@ ��Ϣ����
//@ ��ʱ��
//@ gif����
//@ ��ͼ��˸

#ifndef LDIALOG_H
#define LDIALOG_H

class CLWnd;
class CCombination : public CLWnd
{
//@ ���졢��������
public:
	CCombination();
	virtual ~CCombination();
	
//@ ����
public:
	UINT MessageProcessing(UINT nMsgId,WPARAM wParam=0,LPARAM lParam=0);
	BOOL Create(int nX,int nY,int nWidth,int nHeight);//@ ���������
	void Move(int nX,int nY,int nWidth=-1,int nHeight=-1);//@ �ƶ������
	void Show(BOOL bShow=TRUE);//@ ��ʾ/���������
	
//@ ����
public:
	void AddLWnd(CLWnd* pLWnd);
	void DeleteLWnds();
	
//@ ����
protected:
	CList<CLWnd*> m_listCombinedLWnd;
};

#endif
