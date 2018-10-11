// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Utility.cpp
 *
 *	@brief	π´”√Ω”ø⁄∫Ø ˝ µœ÷
 *
 *	@par	¿˙ ∑:
 *		2014/2/27	¥¥Ω®°£WangY@HOTEAMSOFT
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_Utility.h"
#include "Stk_Platform.h"
#include "LzmaLib.h"
#include "LZ4.h"
#include "LZ4hc.h"
//#include "LicMan/LicVerify.h"
#include <fstream>
#include "Stk_File_struct.h"
#include "ChineseCode.h"
#ifdef _WIN32
#include <tchar.h>
#endif

#include "Stk_API.h"

namespace HoteamSoft {
	namespace SVLLib {

	

// STK_IMPLEMENT_CLASS(Stk_Utility, Stk_Object)
// 
// Stk_Utility::Stk_Utility(void)
// {
// }
// 
// Stk_Utility::~Stk_Utility(void)
// {
// }


bool Stk_Utility::file_is_exist(Stk_string i_strPath)
{
	std::fstream _file(i_strPath.c_str());
	if (!_file.is_open())
		return false;
	_file.close();
	return true;
}

bool Stk_Utility::file_is_exist(Stk_wstring i_strPath)
{
#ifdef __MOBILE__
	std::wfstream _file;
	string strFilePath = ChineseCode::WStringToString(i_strPath);
	_file.open(strFilePath.c_str());
#else
	std::wfstream _file(i_strPath.c_str());
#endif
	if (!_file.is_open())
		return false;
	_file.close();
	return true;
}

/************************************************************************//**
 *	@brief		ÃÊªª√˚≥∆÷–∫¨”––Œ»Á"<"°¢">"µƒ◊÷∑˚Œ™"_"µƒ–Œ Ω
 *	@param[in]	i_wstrName				Œƒº˛/ProtoType√˚≥∆
 *	@param[out]	Œﬁ
 *	@retval		Œƒº˛/ProtoType√˚≥∆
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/

Stk_wstring Stk_Utility::ReplaceImproperCode(Stk_wstring& i_wstrName)
{
	STK_STATUS	stat = STK_SUCCESS;
	STK_UINT32	nuIndex = -1;
	Stk_wstring	wstrName = L"";
	int	nuSize = i_wstrName.length();
	for (int ii = 0; ii < nuSize; ii++)
	{
		if (i_wstrName.at(ii) == L'<' || i_wstrName.at(ii) == L'>'
			|| i_wstrName.at(ii) == L':' || i_wstrName.at(ii) == L'*'
			|| i_wstrName.at(ii) == L'?' || i_wstrName.at(ii) == L'\"'
			|| i_wstrName.at(ii) == L'/' || i_wstrName.at(ii) == L'\\'
			|| i_wstrName.at(ii) == L'|'
			)
		{
			i_wstrName.at(ii) = L'_';
		}
	}
	wstrName = i_wstrName;

	return wstrName;
}


/************************************************************************//**
 *	@brief		≈–∂œSVL∞Ê±æ «∑Ò «V2.2÷Æ«∞µƒ∞Ê±æ
 *	@param[in]	i_wstrVersion		∞Ê±æ∫≈
 *	@param[out]	Œﬁ
 *	@retval		 = TRUE				V2.2÷Æ«∞∞Ê±æ
				 = FALSE			V2.2º∞“‘∫Û∞Ê±æ
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/
STK_BOOL Stk_Utility::IsOldVersion(Stk_wstring i_wstrVersion)
{
	STK_FLOAT32	fVersion = 0;
	string		strVersion = "";

	strVersion = ChineseCode::WStringToString(i_wstrVersion);
	fVersion = (STK_FLOAT32)atof(strVersion.c_str());
	if (fVersion >= 2.20f)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/************************************************************************//**
 *	@brief		◊™ªªæÿ’ÛµΩUV∑ΩœÚœÚ¡øµƒ–Œ Ω
 *	@param[in]	i_dMatrixAA		–˝◊™∆Ω“∆æÿ’Û
 *	@param[out]	o_dirUp			Up∑ΩœÚœÚ¡ø
 *	@param[out]	o_dirUp			Target∑ΩœÚœÚ¡ø
 *	@retval		STK_SUCCESS		’˝≥£Ω· ¯
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/

STK_STATUS Stk_Utility::TransMatrixToUV(STK_MTX32 i_dMatrixAA, STK_DIR32& o_dirUp, STK_DIR32& o_dirTarget)
{
	/*
	 *	±‰¡ø∂®“Â
	 */
	STK_STATUS	stat;
	/*
	 *	≥ı ºªØ
	 */
	stat = STK_SUCCESS;

	o_dirUp.X = (STK_FLOAT32)i_dMatrixAA.PlcMatrix[1][0];
	o_dirUp.Y = (STK_FLOAT32)i_dMatrixAA.PlcMatrix[1][1];
	o_dirUp.Z = (STK_FLOAT32)i_dMatrixAA.PlcMatrix[1][2];

	o_dirTarget.X = -(STK_FLOAT32)i_dMatrixAA.PlcMatrix[2][0];
	o_dirTarget.Y = -(STK_FLOAT32)i_dMatrixAA.PlcMatrix[2][1];
	o_dirTarget.Z = -(STK_FLOAT32)i_dMatrixAA.PlcMatrix[2][2];

	return stat;
}
/*************************************************************************//**
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
STK_INT32 Stk_Utility::LZ4_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP)
{
	char*			out_buff = NULL;
	unsigned int	outSize = 0;

	out_buff = (char*)calloc(1, LZ4_compressBound(i_nuBuffSize));
	if (out_buff == NULL)
	{
		return -1;
	}

	if (i_nCompressionLevel <= 3)
	{
		outSize = LZ4_compress(i_chRecordP, out_buff, i_nuBuffSize);
	}
	else if (i_nCompressionLevel > 4 && i_nCompressionLevel <= 16)
	{
		outSize = LZ4_compressHC2(i_chRecordP, out_buff, i_nuBuffSize, i_nCompressionLevel);
	}
	else
	{
		if (out_buff == NULL)
		{
			free(out_buff);
			out_buff = NULL;
		}
		return outSize;
	}
	o_chStreamP = out_buff;

	return outSize;
}


/*************************************************************************//**
 *	@brief		Ω‚—πÀıº«¬º
 *	@param[in]	i_chCompressStreamP			◊º±∏Ω‚—πµƒ¡˜ ˝æ›
 *	@param[in]	i_nuCompressBuffSize		¥˝Ω‚—πµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@param[in]	i_nuDeCompressBuffSize		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@param[out]	o_chDeCompressStreamP		Ω‚—π∫Ûµƒ¡˜ ˝æ›
 *	@retval		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/
STK_INT32 Stk_Utility::LZ4_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP)
{
	char*			out_buff = NULL;
	int				decodeSize = 0;

	out_buff = (char*)calloc(1, LZ4_compressBound(i_nuDeCompressBuffSize));
	if (out_buff == NULL)
	{
		return -1;
	}

	decodeSize = LZ4_decompress_safe(i_chCompressStreamP, out_buff, i_nuCompressBuffSize, i_nuDeCompressBuffSize + 1024);
	if (decodeSize > 0)
	{
		o_chDeCompressStreamP = out_buff;
	}

	return decodeSize;
}

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
STK_INT32 Stk_Utility::Zip7_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP)
{
	int				state = 0;
 	unsigned char*	out_buff = NULL;
 	unsigned int	outSize = 0;
 	size_t			destLen = 0;					// (io)
 	size_t			unPropsSize = LZMA_PROPS_SIZE;
 	unsigned		dictSize = 1 << 24;
 	int				lc = 3;
 	int				lp = 0;
 	int				pb = 2;
 	int				fb = 32;
 	int				numThreads = 2;
 	Byte			outProps[LZMA_PROPS_SIZE];

 	out_buff = (unsigned char*)calloc(1, i_nuBuffSize);
 	if (out_buff == NULL)
 	{
 		return -1;
 	}
 #ifdef __MOBILE__
 	destLen = 1*i_nuBuffSize;
 #else
 	destLen = _msize(out_buff);
 #endif
 
 	state = LzmaCompress(out_buff, &destLen, (unsigned char *)i_chRecordP, i_nuBuffSize, outProps, &unPropsSize, i_nCompressionLevel, dictSize, lc, lp, pb, fb, numThreads);
 	if (state != 0)
 	{
 		return STK_ERROR;
 	}
 	if (destLen > 0 && (STK_INT32)destLen < i_nuBuffSize)
 	{
 		o_chStreamP = (char *)out_buff;
 	}
	
	return destLen;
}

/*************************************************************************//**
 *	@brief		Ω‚—πÀıº«¬º
 *	@param[in]	i_chCompressStreamP			◊º±∏Ω‚—πµƒ¡˜ ˝æ›
 *	@param[in]	i_nuCompressBuffSize		¥˝Ω‚—πµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@param[in]	i_nuDeCompressBuffSize		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@param[out]	o_chDeCompressStreamP		Ω‚—π∫Ûµƒ¡˜ ˝æ›
 *	@retval		Ω‚—π∫Ûµƒ¡˜ ˝æ›À˘’ºƒ⁄¥Ê¥Û–°
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/
STK_INT32 Stk_Utility::Zip7_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP)
{
	int				state = 0;
	unsigned char*	out_buff = NULL;
	size_t			destLen = 0;
	size_t			unPropsSize = LZMA_PROPS_SIZE;
	Byte			outProps[LZMA_PROPS_SIZE];
	
 	out_buff = (unsigned char*)calloc(1, i_nuDeCompressBuffSize);
 	if (out_buff == NULL)
 	{
 		return -1;
 	}
 	outProps[0] = 0x5d;
 	outProps[1] = 0x00;
 	outProps[2] = 0x00;
 	outProps[3] = 0x00;
 	outProps[4] = 0x01;
 
 #ifdef __MOBILE__
 	destLen = 1* i_nuDeCompressBuffSize;
 #else
 	destLen = _msize(out_buff);
 #endif
 
 	state = LzmaUncompress(out_buff, &destLen, (unsigned char *)i_chCompressStreamP, (size_t *)&i_nuCompressBuffSize, outProps, unPropsSize);
 	if (state != 0)
 	{
 		return STK_ERROR;
 	}
 	o_chDeCompressStreamP = (char *)out_buff;

	return destLen;
}

/*************************************************************************//**
 *	@brief		LicenseºÏ≤‚
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		STK_SUCCESS				¥Ê‘⁄ ⁄»®
 *	@retval		STK_ERROR_NO_LICENSE	Œﬁ ⁄»®
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/
STK_STATUS Stk_Utility::CheckLicense()
{
	STK_STATUS		eState = STK_SUCCESS;
	char			chRegCode[STK_MAXPATH] = {};// ◊¢≤·¬Î
	STK_INT32		nRet = 0;					// ◊¥Ã¨
	STK_INT32		nEdition = 0;				// ≤˙∆∑∞Ê±æ
	STK_INT32		nSpan = 0;					//  £”‡ ±º‰

	//!< ¥˝ÕÍ…∆
	//// ªÒ»°◊¢≤·¬Î
	//nRet = GetRegCode(chRegCode, "", "");
	//if (nRet == 0 && strcmp(chRegCode, "") != 0)
	//{
	//	// ◊¢≤·¬Î–£—È
	//	int nRet = VerifyRegCode(chRegCode, 2, "3.0");
	//	if (nRet == -1)
	//	{
	//		eState = STK_ERROR_DECODE_LICENSE;
	//		return eState;
	//	}
	//	else if (nRet == -2)
	//	{
	//		eState = STK_ERROR_PRODUCT_VER_LICENSE;
	//		return eState;
	//	}
	//	else if (nRet == -3)
	//	{
	//		eState = STK_ERROR_MAC_LICENSE;
	//		return eState;
	//	}

	//	// ªÒ»°∞Ê±æ¿‡–Õ
	//	nEdition = GetEdition(chRegCode);
	//	// ∞Ê±æ¿‡–Õ≤ªŒ™Lite∞Ê±æ ±
	//	if (nEdition != 0)
	//	{
	//		// ªÒ»° £”‡ ±º‰
	//		nSpan = GetSpareDate(chRegCode);

	//		// π˝∆⁄Ã· æ
	//		if (nSpan <= 0)
	//		{
	//			eState = STK_ERROR_TIME_LICENSE;
	//			return eState;
	//		}
	//	}
	//}
	//else
	//{
	//	eState = STK_ERROR_NO_LICENSE;
	//	return eState;
	//}
	
	return eState;
}

std::wstring  Stk_Utility::GetCurModulePath()
{
	wchar_t modulePath[256];
#ifdef _WIN32
    
    wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	wchar_t modFile[_MAX_PATH];
	//HMODULE module = GetModuleHandle(0);
	HMODULE module = Stk_API::GetAPI()->GetDllHandle();
	::GetModuleFileNameW(module, modFile, _MAX_PATH);
	_wsplitpath(modFile, drive, dir, fname, ext);
	_swprintf(modulePath,  L"%s%s", drive, dir);
#else
	//__MOBILE_TODO

#endif

	return modulePath;
}

	}//svllib
}//hoteamsoft
