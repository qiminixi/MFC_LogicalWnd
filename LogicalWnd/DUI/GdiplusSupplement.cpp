#include "stdafx.h"
#include "GdiplusSupplement.h"

//@ ����Դ����ͼ�񣬷���Image����
//@ �����Ԥ������Դ������Bitmap����ôszTypeΪRT_BITMAP��MSDN����˵��
BOOL ImageFromIDResource(UINT nID, LPCTSTR szType, Image* &pImg)
{
	if(szType==RT_BITMAP)//Ԥ������Դλͼ
	{
		pImg=Bitmap::FromHBITMAP(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(nID)),NULL);
		return pImg==NULL ? FALSE:TRUE;
	}

	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), szType);
	if (!hRsrc) return FALSE;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
	// load from stream

	pImg=Gdiplus::Image::FromStream(pstm); 

	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}