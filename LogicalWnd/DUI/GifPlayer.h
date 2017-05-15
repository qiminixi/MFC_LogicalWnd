#ifndef GIFPLAYER_H
#define GIFPLAYER_H

class CGifPlayer:public CLWnd
{
//���졢��������
public:
	CGifPlayer();
	virtual ~CGifPlayer();
	
	//@ ����
public:
	BOOL Create(int nX,int nY,int nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE);
	void Destroy();
	void Draw();
	BOOL LoadGif(WCHAR *szFileName);
	void Play();
	
private:
	int GetFrameCount();
	
	//@ ����
public:
	Image *m_pImage;//ͼ��
	Image *m_pImageCurFrame;//��ǰ֡
	int m_nFrameCount;//�Ƿ��Ƕ���
};


#endif