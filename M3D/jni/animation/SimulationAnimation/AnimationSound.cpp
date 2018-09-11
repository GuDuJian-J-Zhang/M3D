// PlaySound.cpp : implementation file

#include "stdafx.h"
#include "AnimationSound.h"
//#include "AnimationAPI.h"

CAnimationSound::CAnimationSound(void)
{
	m_bErrFlag = false;
	m_count = NULL;
}

CAnimationSound::~CAnimationSound(void)
{
}

//回调主工程声音处理(为获取SVL的绝对Path而采用回调到主工程获得) Add By YL 2014.11.18
bool CAnimationSound::TransferSound(const char cSoundFilePath[SA_BUFFER_SIZE_SMALL],const char plcIdPath[SA_BUFFER_SIZE_SMALL],bool bPlayFlg,float fTime)
{

	return true;
}

void* CAnimationSound::GetInfo(int item)
{
	return 0;
}

int CAnimationSound::GetLength(const wchar_t* strSoundPath)
{

	return 0;
}

//播放声音...
bool CAnimationSound::PlaySound(const __wchar_t* strSoundPath,bool bPlayFlg,int dwFrom)
{

	return true;
}
