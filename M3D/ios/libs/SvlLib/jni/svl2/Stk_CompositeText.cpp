// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_CompositeText.cpp
 *
 *	@brief	复合文本类成员函数的实现
 *
 *	@par	历史:
 *		2013/08/14	创建。WangY@HOTEAMSOFT
 *		2014/04/24	更新。Gaoqq@HOTEAMSOFT
 *					支持Andriod平台
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Define.h"
#include "Stk_CompositeText.h"
#include "Stk_Platform.h"
#include "Stk_Enum.h"
#include "Stk_DocumentManager.h"
#ifndef __MOBILE__
#include "Stk_LogFile.h"
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_ComText构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ComText::Stk_ComText(void)
{
	//for (int ii = 0; ii < m_vcTexts.size(); ii++){
	//	m_vcTexts[ii] = NULL;
	//}
	//memset(&m_OuterBox,0,sizeof(STK_BOX32));
	m_stkEnvelopeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_ComText析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ComText::~Stk_ComText(void)
{
	for (int ii = 0; ii < m_vcTexts.size(); ii++)
	{
		delete m_vcTexts[ii];
		m_vcTexts[ii] = NULL;
	}
	if (m_stkEnvelopeP != NULL)
	{
		delete m_stkEnvelopeP;
		m_stkEnvelopeP = NULL;
	}
}

/************************************************************************//**
 *	@brief		获取文本对象数组的大小
 *	@param[in]	无
 *	@retval		m_vcTexts.size()		文本对象数组的大小
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_ComText::GetNumTexts()
{
	return m_vcTexts.size();
}

/************************************************************************//**
 *	@brief		获取文本对象数组
 *	@param[in]	无
 *	@retval		m_vcTexts			文本对象数组
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_Text*> Stk_ComText::GetTexts()
{
	return m_vcTexts;
}

/************************************************************************//**
 *	@brief		设置文本对象数组
 *	@param[in]	i_vcTexts			文本对象数组
 *	@retval		STK_SUCCESS			文本设置成功
 *	@retval		STK_ERROR			文本设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ComText::SetTexts(const vector<Stk_Text*>& i_vcTexts)
{
	m_vcTexts = i_vcTexts;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置复合文本包围框
 *	@param[in]	i_boxBounding		包围框
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ComText::SetOuterBox(const STK_BOX32& i_boxBounding)
{
	m_boxBounding = i_boxBounding;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取复合文本包围框
 *	@param[in]	无
 *	@retval		m_boxBounding		复合文本包围框
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOX32 Stk_ComText::GetOuterBox()
{
	return m_boxBounding;
}

/****************************************************************************
 *	@brief		获取Text中外框的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_UINT32		外框数组数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Text::GetOutFrameNum()
{
	return m_vcOutFrames.size();
}

/****************************************************************************
 *	@brief		通过Index获取外框
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Stk_OutFrame*		外框
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_OutFrame* Stk_Text::GetOutFrameByIndex(STK_UINT32 i_nuKeyPos)
{
	Stk_OutFrame* stkOutFrameP = NULL;

	if(i_nuKeyPos >= 0 && i_nuKeyPos < m_vcOutFrames.size())
	{
		stkOutFrameP = m_vcOutFrames[i_nuKeyPos];
	}

	return stkOutFrameP;
}

/****************************************************************************
 *	@brief		增加外框数据
 *	@param[in]	i_OutFrameP		指向外框的指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		STK_ERROR		设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::AddOutFrame(Stk_OutFrame* i_OutFrameP)
{
	// 设置定义类型
	i_OutFrameP->SetDefineType(STK_FRAME_DEF_TEXT);

	// 插入数组
	if (i_OutFrameP != NULL)
	{
		m_vcOutFrames.push_back(i_OutFrameP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置文本旋转角度
 *	@param[in]	i_dRotationAngle	旋转角度
 *	@param[out] 无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Text::SetRotationAngle(STK_DOUBLE64 i_dRotationAngle)
{
	m_dRotationAngle = i_dRotationAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本旋转角度
 *	@param[in]	无
 *	@param[out] 无
 *	@retval		文本旋转角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Text::GetRotationAngle()
{
	return m_dRotationAngle;
}

/************************************************************************//**
 *	@brief		设置文本字体风格
 *	@param[in]	i_eFontStyle		文本字体风格
 *	@param[out] 无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Text::SetFontStyle(StkFontStyleTypeEnum i_eFontStyle)
{
	m_eFontStyle = i_eFontStyle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本字体风格
 *	@param[in]	无
 *	@param[out] 无
 *	@retval		文本字体风格
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkFontStyleTypeEnum Stk_Text::GetFontStyle()
{
	return m_eFontStyle;
}

/************************************************************************//**
 *	@brief		设置文本字体类型
 *	@param[in]	i_eFontType			字体类型
 *	@param[out] 无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Text::SetFontType(StkFontTypeEnum i_eFontType)
{
	m_eFontType = i_eFontType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本字体类型
 *	@param[in]	无
 *	@param[out] 无
 *	@retval		文本字体类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkFontTypeEnum Stk_Text::GetFontType()
{
	return m_eFontType;
}

/************************************************************************//**
 *	@brief		设置文本属性类型
 *	@param[in]	i_eAttributeType	文本属性类型
 *	@param[out] 无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Text::SetAttributeType(StkTextAttributeTypeEnum i_eAttributeType)
{
	m_eTextAttribute = i_eAttributeType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本属性类型
 *	@param[in]	无
 *	@param[out] 无
 *	@retval		文本属性类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkTextAttributeTypeEnum Stk_Text::GetAttributeType()
{
	return m_eTextAttribute;
}

/************************************************************************//**
 *	@brief		设置外框类型
 *	@param[in]	i_eFrameType		外框类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ComText::SetFrameType(StkFrameTypeEnum i_eFrameType)
{
	m_eFrameType = i_eFrameType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取外框类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		外框类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkFrameTypeEnum Stk_ComText::GetFrameType()
{
	return m_eFrameType;
}

/************************************************************************//**
 *	@brief		设置包络框对象
 *	@param[in]	i_stkEnvelopeP		包络框对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ComText::SetEnvelope(Stk_Envelope* i_stkEnvelopeP)
{
	m_stkEnvelopeP = i_stkEnvelopeP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取包络框对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		包络框对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Envelope* Stk_ComText::GetEnvelope()
{
	return m_stkEnvelopeP;
}

/*************************************************************************//**
 *	@brief		写入复合文本数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ComText::WriteComText(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS		ret = STK_SUCCESS;
	StkComTextRec4*	ComTextRec4P = NULL;
#ifdef _WINDOWS_	
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_stkFileP == NULL || i_head == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = (i_stkFileP->GetDocumentManager())->GetSVLLogIni();
	stkSVLLogFile = (i_stkFileP->GetDocumentManager())->GetSVLLogFile();
#endif
	ComTextRec4P = (StkComTextRec4 *)calloc(1, sizeof(StkComTextRec4));
	if(ComTextRec4P == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	ComTextRec4P->Control.type = REC_TYPE_COMTEXT;
	ComTextRec4P->Control.length = sizeof(StkComTextRec4) - sizeof(StkControl);
	ComTextRec4P->Control.version = _VER_COMTEXT_RECORD;
	ComTextRec4P->NumText = m_vcTexts.size();
	memcpy(ComTextRec4P->outBox.BoundBox, &m_boxBounding, sizeof(STK_BOX32));
	ComTextRec4P->nuFrameType = m_eFrameType;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)ComTextRec4P);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T(" ■ERROR：ComText记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(ComTextRec4P != NULL)
	{
		free(ComTextRec4P);
		ComTextRec4P = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_Text类构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Text::Stk_Text(void)
{
	//memset(&m_InnerLoc, 0, sizeof(STK_PNT32));
	m_pntInnerLoc.X = 0.0;
	m_pntInnerLoc.Y = 0.0;
	m_pntInnerLoc.Z = 0.0;
	//memset(m_Axis, 0, sizeof(STK_DIR32)*2);
	m_dirAxis[0].X = 0.0;
	m_dirAxis[0].Y = 0.0;
	m_dirAxis[0].Z = 0.0;
	m_dirAxis[1].X = 0.0;
	m_dirAxis[1].Y = 0.0;
	m_dirAxis[1].Z = 0.0;
	//memset(m_CharSize, 0.0f, sizeof(STK_FLOAT32)*2);
	m_fCharSize[0] = 0.0;
	m_fCharSize[1] = 0.0;
	m_fCharDis = 0.0f;

	memset(m_wchFont, 0, sizeof(WCHAR)*STK_MAXNAME);
	m_wstrFonts = L"";
	m_eUsageType = text_type_value;
}

/************************************************************************//**
 *	@brief		Stk_Text类析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Text::~Stk_Text(void)
{
	//OutFrame
	for (int ii = 0; ii < m_vcOutFrames.size(); ii++)
	{
		if (m_vcOutFrames[ii] != NULL)
		{
			delete m_vcOutFrames[ii];
			m_vcOutFrames[ii] = NULL;
		}
	}

}

/************************************************************************//**
 *	@brief		获取文本定位点
 *	@param[in]	无
 *	@retval		m_pntInnerLoc		文本定位点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Text::GetInnerLoc()
{
	return m_pntInnerLoc;
}

/************************************************************************//**
 *	@brief		设置文本定位点
 *	@param[in]	i_pntLocation		定位点
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetInnerLoc(const STK_PNT32& i_pntLocation)
{
	m_pntInnerLoc = i_pntLocation;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本内容
 *	@param[in]	无
 *	@retval		m_wstrText			文本内容
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Text::GetText()
{
	return m_wstrText;
}

/************************************************************************//**
 *	@brief		设置文本内容
 *	@param[in]	i_wstrText			文本内容
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetText(wstring i_wstrText)
{
	m_wstrText = i_wstrText;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取旋转向量
 *	@param[out]	o_dirXAxis			X方向向量
 *	@param[out]	o_dirYAxis			Y方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Text::GetInnerXYAxis(STK_DIR32& o_dirXAxis, STK_DIR32& o_dirYAxis)
{
	o_dirXAxis = m_dirAxis[0];
	o_dirYAxis = m_dirAxis[1];
}

/************************************************************************//**
 *	@brief		设置旋转向量
 *	@param[in]	i_dirXAxis			X方向向量
 *	@param[in]	i_dirYAxis			Y方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetInnerXYAxis(const STK_DIR32& i_dirXAxis, const STK_DIR32& i_dirYAxis)
{
	m_dirAxis[0] = i_dirXAxis;
	m_dirAxis[1] = i_dirYAxis;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取字符宽高
 *	@param[in]	无
 *	@param[out]	o_fWidth			字符宽
 *	@param[out]	o_fHeight			字符高
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Text::GetCharWidthHeight(STK_FLOAT32& o_fWidth, STK_FLOAT32& o_fHeight)
{
	o_fWidth = m_fCharSize[0];
	o_fHeight = m_fCharSize[1];
}

/************************************************************************//**
 *	@brief		设置字符宽高
 *	@param[in]	i_fWidth			字符宽
 *	@param[in]	i_fHeight			字符高
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetCharWidthHeight(const STK_FLOAT32& i_fWidth, const STK_FLOAT32& i_fHeight)
{
	m_fCharSize[0] = i_fWidth;
	m_fCharSize[1] = i_fHeight;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取字符间距
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fCharDis			字符间距
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_FLOAT32 Stk_Text::GetCharSpacing()
{
	return m_fCharDis;
}

/************************************************************************//**
 *	@brief		设置字符间距
 *	@param[in]	i_fCharDis			字符间距
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetCharSpacing(const STK_FLOAT32& i_fCharDis)
{
	m_fCharDis = i_fCharDis;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取字体格式
 *	@param[in]	无
 *	@param[out]	o_wchFontA			字体
 *	@retval		无
 *	@note		建议用GetFonts替换
 *	@attention	无
 ****************************************************************************/

void Stk_Text::GetFont(WCHAR o_wchFontA[STK_MAXNAME])
{
	for (int ii = 0; m_wchFont[ii] != L'\0' && ii<STK_MAXNAME-1; ii++ )
	{
		*o_wchFontA = m_wchFont[ii];
		o_wchFontA++;
	}
	*o_wchFontA = L'\0';
}

/************************************************************************//**
 *	@brief		设置字符格式
 *	@param[in]	i_wchFontA			字体
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		建议用SetFonts替换
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetFont(WCHAR i_wchFontA[STK_MAXNAME])
{
	int ii = 0;
	while (*i_wchFontA != L'\0' && ii<STK_MAXNAME-1)
	{
		m_wchFont[ii] = *i_wchFontA;
		ii++;
		i_wchFontA++;
	}
	m_wchFont[ii] = L'\0';

	m_wstrFonts = m_wchFont; //同步m_wstrFonts与m_wchFont

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置字符应用类型
 *	@param[in]	i_Type			字符应用类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Text::SetUsageType(StkTextTypeEnum i_Type)
{
	m_eUsageType = i_Type;
}

/************************************************************************//**
 *	@brief		获取字符应用类型
  *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkTextTypeEnum Stk_Text::GetUsageType()
{
	return m_eUsageType;
}

/************************************************************************//**
 *	@brief		获取字体格式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrFonts			字体
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Text::GetFonts()
{
	return m_wstrFonts;
}

/************************************************************************//**
 *	@brief		设置字符格式
 *	@param[in]	i_wstrFonts			字体
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		不允许超过63字符
 *	@attention	无
 ****************************************************************************/

void Stk_Text::SetFonts(wstring i_wstrFonts)
{
	if(i_wstrFonts.size()>STK_MAXNAME-1)
	{
		i_wstrFonts.erase(STK_MAXNAME-1);
	}

	m_wstrFonts = i_wstrFonts;
#ifdef __MOBILE__
	wcscpy(m_wchFont, i_wstrFonts.c_str() ); //同步m_wstrFonts与m_wchFont
#else
	wcscpy_s(m_wchFont,STK_MAXNAME, i_wstrFonts.c_str() ); //同步m_wstrFonts与m_wchFont
#endif
}

/************************************************************************//**
 *	@brief		设置文本包围框
 *	@param[in]	i_boxBounding		包围框
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Text::SetOuterBox(const STK_BOX32& i_boxBounding)
{
	m_boxBounding = i_boxBounding;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文本包围框
 *	@param[in]	无
 *	@param[out] 无
 *	@retval		m_boxBounding		文本包围框
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOX32 Stk_Text::GetOuterBox()
{
	return m_boxBounding;
}

/*************************************************************************//**
 *	@brief		写入文本数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Text::WriteText(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS			ret = STK_SUCCESS;
	StkTextRec5*		TextRecP5 = NULL;
	STK_UINT32			nuNumChar = 0;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif	

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	// 【1.1】 输出Text Record
	nuNumChar = m_wstrText.length() + 1;
	TextRecP5 = (StkTextRec5 *)calloc(1, sizeof(StkTextRec5) - sizeof(STK_WCHAR) + nuNumChar * sizeof(STK_WCHAR));
	if(TextRecP5 == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	TextRecP5->Control.type = REC_TYPE_TEXT;
	TextRecP5->Control.length = sizeof(StkTextRec5) - sizeof(STK_WCHAR) + nuNumChar*sizeof(STK_WCHAR) - sizeof(StkControl);
	TextRecP5->Control.version = _VER_TEXT_RECORD /*5*/;

	TextRecP5->InnerLoc = m_pntInnerLoc;
	TextRecP5->Axis[0] = m_dirAxis[0];			//X旋转向量
	TextRecP5->Axis[1] = m_dirAxis[1];			//Y旋转向量
	TextRecP5->dAngle = m_dRotationAngle;		// 旋转角度
	TextRecP5->Size[0] = m_fCharSize[0];		//字符宽
	TextRecP5->Size[1] = m_fCharSize[1];		//字符高
	TextRecP5->CharDis = m_fCharDis;
	//wcscpy_s(TextRecP3->font, STK_MAXNAME, m_wchFont );
	TextRecP5->NumChar = nuNumChar;
	TextRecP5->usetype = m_eUsageType;
	TextRecP5->nuFontStyle = m_eFontStyle;
	TextRecP5->nuFontType = m_eFontType;
	TextRecP5->nuTextAttribute = m_eTextAttribute;

	memcpy(TextRecP5->outBox.BoundBox, &m_boxBounding, sizeof(STK_BOX32));

#ifdef __MOBILE__
	memcpy(TextRecP5->font,Stk_Platform::WStringToSTKWCHARStr(m_wstrFonts),(m_wstrFonts.size()+1)*sizeof(STK_WCHAR));
	memcpy(TextRecP5->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrText),(m_wstrText.size() +1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(TextRecP5->font,STK_MAXNAME, m_wstrFonts.c_str() );
	wcscpy_s(TextRecP5->atr, nuNumChar, m_wstrText.c_str());	//把结束符L'\0'同时加进去
#endif

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)TextRecP5);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Text记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	if(TextRecP5 != NULL)
	{
		free(TextRecP5);
		TextRecP5 = NULL;
	}


	// 【1.2】 输出OutFrame Record
	for(int ii = 0; ii < m_vcOutFrames.size(); ii++)
	{
		m_vcOutFrames[ii]->WriteFrame(i_head, i_stkFileP);
		vector<Stk_CurveEntity* > vcTmpCurves = m_vcOutFrames[ii]->GetFrameData();

		for(int jj = 0; jj < vcTmpCurves.size(); jj++)
		{
			switch(vcTmpCurves[jj]->GetCurveType())
			{
			case CURVE_TYPE_POLYLINE:
				((Stk_PolyLine*)vcTmpCurves[jj])->WritePolyline(i_head, i_stkFileP);
				break;
			case CURVE_TYPE_ELLIPSE:
				((Stk_Ellipse*)vcTmpCurves[jj])->WriteEllipse(i_head, i_stkFileP);
				break;
			case CURVE_TYPE_LINE:
				((Stk_Line*)vcTmpCurves[jj])->WriteLine(i_head, i_stkFileP);
				break;
			default:
				break;
			}
		}
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Envelope::Stk_Envelope()
{
	m_bHasFrame = true;
	m_rgbaBackground.Red = 0;
	m_rgbaBackground.Green = 0;
	m_rgbaBackground.Blue = 0;
	m_rgbaBackground.Alpha = 1;
	m_rgbaFrame.Red = 0;
	m_rgbaFrame.Green = 0;
	m_rgbaFrame.Blue = 0;
	m_rgbaFrame.Alpha = 1;
}
/*************************************************************************//**
 *	@brief		析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Envelope::~Stk_Envelope()
{

}

/************************************************************************//**
 *	@brief		设置包络框的背景色
 *	@param[in]	i_rgbaBackground	包络框背景色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Envelope::SetBackgroundColor(STK_RGBA32 i_rgbaBackground)
{
	m_rgbaBackground = i_rgbaBackground;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取包络框的背景色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		包络框的背景色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Envelope::GetBackgroundColor()
{
	return m_rgbaBackground;
}

/************************************************************************//**
 *	@brief		设置是否有包络框的边框的标识
 *	@param[in]	i_bHasFrame		是否有包络框的边框的标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Envelope::SetFrameFlag(STK_BOOL i_bHasFrame)
{
	m_bHasFrame = i_bHasFrame;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否有包络框的边框的标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		是否有包络框的边框的标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Envelope::IsHaveFrame()
{
	return m_bHasFrame;
}

/****************************************************************************
 *	@brief		设置包络框边框的颜色
 *	@param[in]	i_rgbaFrame			包络框边框的颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Envelope::SetFrameColor(STK_RGBA32 i_rgbaFrame)
{
	m_rgbaFrame = i_rgbaFrame;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取包络框边框的颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		包络框的背景色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Envelope::GetFrameColor()
{
	return m_rgbaFrame;
}

/************************************************************************//**
 *	@brief		设置锚点
 *	@param[in]	i_eType				锚点类型
 *	@param[in]	i_pntAnchor			锚点
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Envelope::SetAnchorPoint(StkAnchorTypeEnum i_eType, STK_PNT32 i_pntAnchor)
{
	if (i_eType != TYPE_ANCHOR_NUMBER)
	{
		map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = m_mapAnchorPoints.find(i_eType);
		if (itor != m_mapAnchorPoints.end())
		{
			m_mapAnchorPoints.erase(itor);
		}
		m_mapAnchorPoints.insert(pair<StkAnchorTypeEnum, STK_PNT32>(i_eType, i_pntAnchor));
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		设置存在的所有锚点类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		存在的所有锚点类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<StkAnchorTypeEnum>& Stk_Envelope::GetAllAnchorTypes()
{
	vector<StkAnchorTypeEnum>	vcAnchorTypes;
	map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = m_mapAnchorPoints.begin();
	while(itor != m_mapAnchorPoints.end())
	{
		vcAnchorTypes.push_back(itor->first);
		itor++;
	}
	return vcAnchorTypes;
}

/************************************************************************//**
 *	@brief		获取锚点
 *	@param[in]	i_eType				锚点类型
 *	@param[out]	o_pntAnchor			锚点
 *	@retval		STK_SUCCESS			获取成功
 *	@retval		其它				获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Envelope::GetAnchorPointByType(StkAnchorTypeEnum i_eType, STK_PNT32& o_pntAnchor)
{
	map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = m_mapAnchorPoints.begin();
	while(itor != m_mapAnchorPoints.end())
	{
		if (itor->first == i_eType)
		{
			o_pntAnchor = itor->second;
			return STK_SUCCESS;
		}
		itor++;
	}
	return STK_ERROR_ANCHOR_NOT_FOUND;
}

/*************************************************************************//**
 *	@brief		写包络框数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Envelope::WriteFile(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkEnvelopeRec*		EnvelopeRecP = NULL;
	STK_UINT32			nuAnchorPointNum = 0;
	map<StkAnchorTypeEnum, STK_PNT32>::iterator itor = m_mapAnchorPoints.begin();
	nuAnchorPointNum = m_mapAnchorPoints.size();

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	EnvelopeRecP = (StkEnvelopeRec*)calloc(1, sizeof(StkEnvelopeRec) - sizeof(StkAnchorPoint) + nuAnchorPointNum * sizeof(StkAnchorPoint));
	if(EnvelopeRecP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	EnvelopeRecP->Control.type = REC_TYPE_ENVELOPE;
	EnvelopeRecP->Control.length = sizeof(StkEnvelopeRec) - sizeof(StkAnchorPoint) + nuAnchorPointNum * sizeof(StkAnchorPoint) - sizeof(StkControl);
	EnvelopeRecP->Control.version = _VER_ENVELOPE_RECORD;

	if (m_bHasFrame != NULL)
	{
		EnvelopeRecP->IsItem |= STK_HAS_ENVELOPE_FRAME;
	}
	EnvelopeRecP->rgbaFrame = m_rgbaFrame;
	EnvelopeRecP->rgbaBackground = m_rgbaBackground;
	EnvelopeRecP->nuAnchorPointNum = nuAnchorPointNum;
	int ii = 0;
	while(itor != m_mapAnchorPoints.end())
	{
		EnvelopeRecP->atr[ii].nuAnchorType = itor->first;
		EnvelopeRecP->atr[ii].pntAnchor = itor->second;
		ii++;
		itor++;
	}

	eState = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)EnvelopeRecP);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if(EnvelopeRecP != NULL)
	{
		free(EnvelopeRecP);
		EnvelopeRecP = NULL;
	}
	return STK_SUCCESS;
}