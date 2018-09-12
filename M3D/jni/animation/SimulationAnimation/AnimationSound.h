// PlaySound.h : process sound header file 
// Add By YL 2014.11.12
#ifndef _H_ANIMATIONSOUND_H
#define _H_ANIMATIONSOUND_H

#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/Animation_def.h"

class SA_API CAnimationSound
{
	public:
		CAnimationSound(void);
		~CAnimationSound(void);

	public:
		//某个音频文件的总时间长度
		int cdlen;
		//设备型号
		int m_count;
		//歌曲的起点、终点
		int cdfrom,cdto;
		bool m_bErrFlag;
	public:		
		bool TransferSound(const char cSoundFilePath[SA_BUFFER_SIZE_SMALL],const char plcIdPath[SA_BUFFER_SIZE_SMALL],bool bPlayFlg,float fTime);	//插值传递声音
		void* GetInfo(int item);				//获得指定声音信息
		int GetLength(const wchar_t* strSoundPath);	//获得歌曲长度
		bool PlaySound(const __wchar_t* strSoundPath,bool bPlayFlg,int dwFrom);	//播放|停止声音
};
#endif
