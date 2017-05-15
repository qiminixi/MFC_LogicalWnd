#ifndef GIFPLAYER_H
#define GIFPLAYER_H

class CGifPlayer:public CLWnd
{
//构造、析构函数
public:
	CGifPlayer();
	virtual ~CGifPlayer();
	
	//@ 操作
public:
	BOOL Create(int nX,int nY,int nZAxisLevel=LWND_ZAXIS_LEVEL_MIDDLE);
	void Destroy();
	void Draw();
	BOOL LoadGif(WCHAR *szFileName);
	void Play();
	
private:
	int GetFrameCount();
	
	//@ 属性
public:
	Image *m_pImage;//图像
	Image *m_pImageCurFrame;//当前帧
	int m_nFrameCount;//是否是动画
};


#endif