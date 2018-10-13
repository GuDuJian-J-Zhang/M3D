// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_Log.h
 *  @brief		»’÷æ¿‡
 *	@par ¿˙ ∑º«¬º
 *				2016/11/8 ¥¥Ω®°£MengYaohui(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_LOG_H_
#define _SVLLIB_LOG_H_
#include "Stk_Object.h"
#include <iostream>
namespace HoteamSoft {
	namespace SVLLib {

	
#if 0
/** @brief	»’÷æº∂±¿‡–Õ	*/
enum Stk_Log_Level
        {
            STK_LOG_LEVEL_NONE=0,					//!< ≤ª¥Ú”°»’÷æ
	STK_LOG_LEVEL_TRACE,					//!< ¡˜≥Ã£∫‘À––¡˜≥Ã¥Ú”° Õ®≥£∑≈÷√µΩ∫Ø ˝ø™ º
	STK_LOG_LEVEL_DEBUG,					//!< µ˜ ‘£∫µ˜ ‘–≈œ¢¥Ú”°
	STK_LOG_LEVEL_INFON,					//!< –≈œ¢£∫Õ®≥£”√”⁄≤Ÿ◊˜≥…π¶Ã· æ
	STK_LOG_LEVEL_NOTICE,					//!< Ã· æ£∫Õ®≥£”√”⁄≤Ÿ◊˜≥…π¶Ã· æ±»INFO…‘∏ﬂ“ªº∂±
	STK_LOG_LEVEL_WARNING,					//!< æØ∏Ê£∫ø…ƒ‹ª·‘Ï≥…≤ªø…‘§¡œµƒ”∞œÏ
	STK_LOG_LEVEL_ERROR,					//!< ¥ÌŒÛ£∫≤ª”∞œÏ’˚ÃÂ–‘ƒ‹µƒ–°¥ÌŒÛ
	STK_LOG_LEVEL_CRITICAL,					//!< Œ£º±¥ÌŒÛ£∫‘À––≤ª’˝»∑«Èøˆ
	STK_LOG_LEVEL_FATAL,					//!< ÷¬√¸¥ÌŒÛ£∫ µº÷¬π“ª˙
	STK_LOG_LEVEL_HIGHEST					//!< ◊Ó∏ﬂº∂±£¨√ª”–º∂±«∞◊∫
};

/** @brief	¥Ú”°»’÷æµΩŒƒº˛ªÚøÿ÷∆Ã®	*/
enum Stk_Log_Type
{
	STK_LOG_TO_FILE = 0,					//!< ¥Ú”°»’÷æµΩŒƒº˛
	STK_LOG_TO_CONSOLE,						//!< ¥Ú”°»’÷æµΩøÿ÷∆Ã®
};

/** @brief	»’÷æµƒ∂ÓÕ‚–≈œ¢¿‡±	*/
enum Stk_Log_Addition_info
{
	STK_LOG_ADDITION_NONE		= 0,		//!< ≤ª∏Ωº”–≈œ¢
	STK_LOG_ADDITION_TIME		= 0x0001	//!< ∏Ωº” ±º‰–≈œ¢
};
#endif
class STK_TOOLKIT_EXPORT Stk_Log : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Log)
public:
	Stk_Log(void);
	virtual ~Stk_Log(void);
private:
	FILE				*m_pFile;							//!< »’÷æ–¥»ÎŒƒº˛
	Stk_string			m_strPath;							//!< »’÷æÀ˘‘⁄¬∑æ∂
	Stk_Log_Level		m_level;							//!< »’÷æº∂±
	Stk_Log_Type		m_type;								//!< –¥»Î∑Ω Ω
private:
	/************************************************************************
	 *	@brief		≥ı ºªØ
	 *	@param[in]	Œﬁ
	 *	@param[out]	Œﬁ
	 *	@retval		Œﬁ
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ************************************************************************/
	void Initialize();
	/*************************************************************************
	 *	@brief		ªÒ»°∏Ωº”–≈œ¢
	 *	@param[in]	type	ªÒ»°∏Ωº”–≈œ¢µƒº∂±£¨”√Stk_Log_Addition_info√∂æŸ
	 *						Ω¯––"|"◊È∫œ
	 *	@retval		◊÷∑˚¥Æ£∫∏Ωº”–≈œ¢
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 *************************************************************************/
	Stk_string GetAddtionInfo(int type);
public:
	/**************************************************************************
	 *	@brief		…Ë÷√¥Ú”°»’÷æµƒ∑Ω Ω£¨»Áπ˚≤ª…Ë÷√ƒ¨»œ¥Ú”°µΩøÿ÷∆∆˜
	 *	@param[in]	Stk_Log_Type type	»’÷æ¿‡±	
	 *	@param[out]	Œﬁ
	 *	@retval		Œﬁ
	 *	@note		STK_LOG_TO_FILE£∫¥Ú”°µΩŒƒº˛	
					STK_LOG_TO_CONSOLE£∫¥Ú”°»’÷æµΩøÿ÷∆Ã®
	 *	@attention	Œﬁ
	**************************************************************************/
	void SetLogType(Stk_Log_Type type){m_type = type;}
	/**************************************************************************
	 *	@brief		…Ë÷√¥Ú”°»’÷æµƒº∂±£¨ƒ¨»œŒ™◊Ó∏ﬂΩÁ±
	 *	@param[in]	Stk_Log_Level level			»’÷æº∂±
	 *	@param[out]	
	 *	@retval		
	 *	@note		»Áπ˚µ±«∞º∂± «STK_LOG_LEVEL_ERROR£∫–°”⁄¥Àº∂±µƒ–≈œ¢≤ª¥Ú”°£¨
					±»»ÁSTK_LOG_LEVEL_WARNING£ª¥Û”⁄¥Àº∂±µƒ–≈œ¢¥Ú”°£¨±»»Á
					STK_LOG_LEVEL_CRITICAL°£
	 *	@attention	Œﬁ
	**************************************************************************/
	void SetLogLevel(Stk_Log_Level level){m_level = level;}
	/**************************************************************************
	 *	@brief		…Ë÷√¥Ú”°»’÷æµƒ¬∑æ∂£¨ƒ¨»œ¬∑æ∂ «µ±«∞ƒø¬ºµƒ"stkLog.log"
	 *	@param[in]	std::string strPath		»’÷æ¬∑æ∂
	 *	@param[out]	Œﬁ
	 *	@retval		Œﬁ
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	void SetLogPath(Stk_string strPath);
	/**************************************************************************
	 *	@brief		¥Ú”°»’÷æ
	 *	@param[in]	std:wstring strMsg		»’÷æ–≈œ¢
	 *	@param[in]	Stk_Log_Level level			»’÷æº∂±
	 *	@param[in]	type	ªÒ»°∏Ωº”–≈œ¢µƒº∂±£¨”√Stk_Log_Addition_info√∂æŸ
	 *						Ω¯––"|"◊È∫œ
	 *	@param[out]	≥…π¶:STK_SUCCESS		 ß∞‹:STK_ERROR
	 *	@retval		Œﬁ
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 **************************************************************************/
	STK_STATUS PrintLog(Stk_string strMsg, Stk_Log_Level level = STK_LOG_LEVEL_HIGHEST, int type = 0);
	/**************************************************************************
	 *	@brief		¥Ú”°ø’––
	 *	@param[in]	Œﬁ
	 *	@param[out]	Œﬁ
	 *	@retval		≥…π¶:STK_SUCCESS		 ß∞‹:STK_ERROR
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	STK_STATUS PrintEndl()
	{
		return PrintLog(" ");
	}
};
	}//svllib
}//hoteamsoft
#endif
