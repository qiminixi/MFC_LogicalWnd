#include "StdAfx.h"
#include "GifPlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CWinThread* g_pThread;

//如何精确控制时间
//将类中的成员定义为私有
UINT PlayThread(LPVOID pParam)
{
	CGifPlayer *pGifPlayer=(CGifPlayer*)pParam;
	int frameindex=0;
	while(1)
	{
		/*int size=pGifPlayer->m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
		PropertyItem *pItem=(PropertyItem *)malloc(size);
		pGifPlayer->m_pImage->GetPropertyItem(PropertyTagFrameDelay,size,pItem);
		for(int i=0;i<4;i++)
		{
			TRACE("%d\n",((long*)pItem->value)[i]);
		}*/

		pGifPlayer->m_pImage->SelectActiveFrame(&FrameDimensionTime,frameindex);
		frameindex++;
		if(frameindex>=pGifPlayer->m_nFrameCount)
			frameindex=0;
		pGifPlayer->UpdateWindow();
		Sleep(100);
	}
	
	return 0;
}

CGifPlayer::CGifPlayer()
{
	m_pImage=NULL;
	m_pImageCurFrame=NULL;
}
CGifPlayer::~CGifPlayer()
{
}

BOOL CGifPlayer::Create(int nX,int nY,int nZAxisLevel/*=LWND_ZAXIS_LEVEL_MIDDLE*/)
{
	return CLWnd::Create(nX,nY,1,1,TRUE,nZAxisLevel);
}

void CGifPlayer::Destroy()
{
	//@ 在这里关闭线程
	if(g_pThread){
		TerminateThread(g_pThread->m_hThread, 0);
		g_pThread = NULL;
	}
	CLWnd::Destroy();
}

void CGifPlayer::Draw()
{
	Graphics graphics(GetBitmap());
	graphics.DrawImage(m_pImage,0,0);
}

BOOL CGifPlayer::LoadGif(WCHAR *szFileName)
{
	if(m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	m_pImage = new Image(szFileName);

	Status stat=m_pImage->GetLastStatus();
	if(stat!=Ok) return FALSE;
	Resize(m_pImage->GetWidth(),m_pImage->GetHeight());
	
	m_nFrameCount=GetFrameCount();
	
	return TRUE;
}

int CGifPlayer::GetFrameCount()
{
	ASSERT(m_pImage!=NULL);
	UINT count=m_pImage->GetFrameDimensionsCount();
	GUID* pDimensionIDs = (GUID*)malloc(sizeof(GUID)*count);
	m_pImage->GetFrameDimensionsList(pDimensionIDs,count);
	WCHAR strGuid[39];
	StringFromGUID2(pDimensionIDs[0], strGuid, 39);
	TRACE((L"The first (and only) dimension ID is %s.\n", strGuid));
	UINT frameCount = m_pImage->GetFrameCount(&pDimensionIDs[0]);
	free(pDimensionIDs);
	return frameCount;
}

void CGifPlayer::Play()
{
	g_pThread = AfxBeginThread(PlayThread,this);
}
