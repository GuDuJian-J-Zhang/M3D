// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@class	Stk_Utility
 *	@brief	π´”√Ω”ø⁄…˘√˜
 *	@par ¿˙ ∑º«¬º
 *			2016/11/8 ¥¥Ω®°£MengYaohui(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_UTILITY_H_
#define _SVLLIB_UTILITY_H_

#include "Stk_Define.h"
#ifdef _WIN32
#include <windows.h>
#else

#endif
//#include "Stk_Object.h"

// class STK_TOOLKIT_EXPORT Stk_Utility : public Stk_Object
// {
// 	STK_DECLARE_CLASS(Stk_Utility)
// public:
// 	Stk_Utility(void);
// 	virtual ~Stk_Utility(void);
// };
namespace HoteamSoft {
	namespace SVLLib {

	
namespace Stk_Utility
{
	/**************************************************************************
	 *	@brief		≈–∂œŒƒº˛ «∑Ò¥Ê‘⁄
	 *	@param[in]	i_strPath			Œƒº˛¬∑æ∂
	 *	@param[out]	
	 *	@retval		Œƒº˛¥Ê‘⁄:true	Œƒº˛≤ª¥Ê‘⁄:flase
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	bool file_is_exist(Stk_string i_strPath);
	bool file_is_exist(Stk_wstring i_strPath);

	/****************************************************************************
	 *	@brief		ÃÊªª√˚≥∆÷–∫¨”––Œ»Á"<"°¢">"µƒ◊÷∑˚Œ™"_"µƒ–Œ Ω
	 *	@param[in]	i_wstrName				Œƒº˛/ProtoType√˚≥∆
	 *	@param[out]	Œﬁ
	 *	@retval		Œƒº˛/ProtoType√˚≥∆
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	Stk_wstring		ReplaceImproperCode(Stk_wstring& i_wstrName);

	/****************************************************************************
	 *	@brief		≈–∂œSVL∞Ê±æ «∑Ò «V2.2÷Æ«∞µƒ∞Ê±æ
	 *	@param[in]	i_wstrVersion		∞Ê±æ∫≈
	 *	@param[out]	Œﬁ
	 *	@retval		 = TRUE				V2.2÷Æ«∞∞Ê±æ
					 = FALSE			V2.2º∞“‘∫Û∞Ê±æ
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_BOOL		IsOldVersion(Stk_wstring i_wstrVersion);

	/****************************************************************************
	 *	@brief		◊™ªªæÿ’ÛµΩUV∑ΩœÚœÚ¡øµƒ–Œ Ω
	 *	@param[in]	i_dMatrixAA		–˝◊™∆Ω“∆æÿ’Û
	 *	@param[out]	o_dirUp			Up∑ΩœÚœÚ¡ø
	 *	@param[out]	o_dirUp			Target∑ΩœÚœÚ¡ø
	 *	@retval		STK_SUCCESS		’˝≥£Ω· ¯
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_STATUS	TransMatrixToUV(STK_MTX32 i_dMatrixAA, STK_DIR32& o_dirUp, STK_DIR32& o_dirTarget);

	/*****************************************************************************
	 *	@brief		—πÀıº«¬º
	 *	@param[in]	i_chRecordP			—πÀı∂‘œÛº«¬º
	 *	@param[in]	i_nuBuffSize		—πÀı∂‘œÛÀ˘’ºµƒBuff¥Û–°
	 *	@param[in]	i_nCompressionLevel	—πÀıµ»º∂=9£∫∏ﬂ—πÀı±»
	 *	@param[out]	o_chStreamP			—πÀı∫Ûµƒ¡˜ ˝æ›
	 *	@param[out]	o_StreamSize		—πÀı∫Ûµƒ¡˜ ˝æ›À˘’ºµƒ
	 *	@retval		—πÀı∫Ûµƒ¡˜ ˝æ›¥Û–°
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_INT32 LZ4_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP);

	/*****************************************************************************
	 *	@brief		Ω‚—πÀıº«¬º
	 *	@param[in]	i_chCompressStreamP			◊º±∏Ω‚—πµƒ¡˜ ˝æ›
	 *	@param[in]	i_nuCompressBuffSize		¥˝Ω‚—πµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@param[in]	i_nuDeCompressBuffSize		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@param[out]	o_chDeCompressStreamP		Ω‚—π∫Ûµƒ¡˜ ˝æ›
	 *	@retval		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_INT32 LZ4_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP);

	/*****************************************************************************
	 *	@brief		—πÀıº«¬º
	 *	@param[in]	i_chRecordP			—πÀı∂‘œÛº«¬º
	 *	@param[in]	i_nuBuffSize		—πÀı∂‘œÛÀ˘’ºµƒBuff¥Û–°
	 *	@param[in]	i_nCompressionLevel	—πÀıµ»º∂=5
	 *	@param[out]	o_chStreamP			—πÀı∫Ûµƒ¡˜ ˝æ›
	 *	@param[out]	o_StreamSize		—πÀı∫Ûµƒ¡˜ ˝æ›À˘’ºµƒ
	 *	@retval		—πÀı∫Ûµƒ¡˜ ˝æ›¥Û–°
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_INT32 Zip7_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP);

	/*****************************************************************************
	 *	@brief		Ω‚—πÀıº«¬º
	 *	@param[in]	i_chCompressStreamP			◊º±∏Ω‚—πµƒ¡˜ ˝æ›
	 *	@param[in]	i_nuCompressBuffSize		¥˝Ω‚—πµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@param[in]	i_nuDeCompressBuffSize		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@param[out]	o_chDeCompressStreamP		Ω‚—π∫Ûµƒ¡˜ ˝æ›
	 *	@retval		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_INT32 Zip7_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP);

	/*****************************************************************************
	 *	@brief		LicenseºÏ≤‚
	 *	@param[in]	Œﬁ
	 *	@param[out]	Œﬁ
	 *	@retval		STK_SUCCESS				¥Ê‘⁄ ⁄»®
	 *	@retval		STK_ERROR_NO_LICENSE	Œﬁ ⁄»®
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	 ****************************************************************************/
	STK_STATUS CheckLicense();
	
	std::wstring GetCurModulePath();
}

	}//svllib
}//hoteamsoft

#endif
