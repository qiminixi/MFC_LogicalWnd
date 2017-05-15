/**
 *@ ���ļ��ж�CLWnd����ж��壬CLWnd�������߼����ڵĻ���
 *@ ���д��ڶ�Ӧ�ò��ö�̬�����ķ�ʽ���� 
 *@ ע��������ڴ����˲���û�����٣���ܻ��ڳ����˳�ǰ�Զ����ٴ��ڲ�ɾ������
 *���ڶ�̬�������ڣ����������Destory���������٣���ô��������ٲ���Ҳ���ֶ�����
 *@ ������Ϊ��¼���ڵ�λͼ������GDI+�ر�֮ǰ���У���̬�����ǳ����˳�ʱ���ٵģ�
 *��ʱGDI+�Ѿ��رգ����ٲ�����ʧ�ܣ������ڴ�й¶��Ϊ�˱����û��ֶ����д�������
 *��ɾ�����������ڴ��ڷǳ����ʱ����ú��鷳����Ҫ�󴰿�ȫ�����ж�̬������
 *�����ƺ���������ܾͿ����ˡ�
**/

#ifndef LWND_H
#define LWND_H

//Z����
#define LWND_ZAXIS_LEVEL_BOTTOM		0	//�ײ�
#define LWND_ZAXIS_LEVEL_MIDDLE		1	//�м�
#define LWND_ZAXIS_LEVEL_TOP		2	//������
#define LWND_ZAXIS_LEVEL_TOPMOST	3	//��������

class CLWnd
{
//���캯��
public:
	CLWnd();
	virtual ~CLWnd();
	
//����
public:
	BOOL Create(int nX,int nY,int nWidth,int nHeight,
	BOOL bIsVisible=TRUE, int nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE);//��������
	virtual void Destroy();//���ٴ���
	
	//������Ӧ
	virtual void Draw();//���ƴ���
	virtual void MouseMove(int x,int y);
	virtual void MouseLeave();
	virtual void LButtonDown(int x,int y);
	virtual void LButtonUp(int x,int y);
	virtual void LButtonDblClick(int x,int y);
	
	//�¼�����
	//��Ŀǰ���¼����������ڷ����Ա�ͨ�ţ�ϵͳ���ᷢ����Ϣ
	virtual UINT MessageProcessing(UINT nMsgId,WPARAM wParam=0,LPARAM lParam=0);
	
	//����
	virtual void Move(int nX,int nY,int nWidth=-1,int nHeight=-1);//�ƶ�����
	virtual void Show(BOOL bShow=TRUE);//��ʾ/���ش���
	virtual void Resize(int nWidth,int nHeight);//�������ڴ�С
	virtual void Resized(int nWidth,int nHeight){}//���ڴ�С�ı��˻������
	void UpdateWindow();//���´��ڣ�������Redraw������ΪDraw�ɿ�ܵ���
	void SetCapture();//������겶��
	void ReleaseCapture();//ȡ����겶��
	void ResetBitmap();//��λͼ����Ϊȫ͸��״̬
	void Enable(BOOL bEnable){m_bIsEnable=bEnable;}//��Ϊ����/�����ô���
	
	//���Է���
	const Rect & GetWindowRect(){return m_rcWnd;}					//��ȡ��������
	BOOL IsVisible(){return m_bIsVisible;}							//�����Ƿ�ɼ�
	BOOL IsEnable(){return m_bIsEnable;}							//�����Ƿ����
	Bitmap * GetBitmap(void){return m_pWndBitmap;}					//��ȡλͼ
	UINT GetZAxisLevel(){return m_nZAxisLevel;}						//��ȡ����Z����
	void SetZAxisLevel(UINT nZAxisLevel);							//���ô���Z����
	const CString & GetWindowText(){return m_strText;};				//��ȡ�����ı�
	void SetWindowText(LPCTSTR lpszText){m_strText=lpszText;}		//���ô����ı�
	BOOL IsWindowCreated(){return m_pWndBitmap!=NULL?TRUE:FALSE;}	//�����Ƿ񴴽�
	
	
//����
private:
	Rect m_rcWnd;//��������ͨ�����μ�¼λ�úʹ�С������������Ը����ڣ�����������
	UINT m_nZAxisLevel;//Z���Σ���Z���ϵ���ʾ˳��Ϊ�����б��еļ�¼˳��
	BOOL m_bIsVisible;//�����Ƿ�ɼ�
	BOOL m_bIsEnable;//�����Ƿ����
	Bitmap *m_pWndBitmap;//����λͼ
	CString m_strText;//����
};

#endif