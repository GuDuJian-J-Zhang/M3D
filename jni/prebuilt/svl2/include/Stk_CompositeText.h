// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_CompositeText.h
 *
 *	@brief	文本、复合文本类的声明
 *
 *	@par	历史:
 *		2013/08/14	创建。WangY@HOTEAMSOFT
****************************************************************************/
#ifndef _Stk_CompositeText_H_
#define _Stk_CompositeText_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_File.h"
#include <string>
#include <vector>

using namespace std;

/**************************************************************************************
	 *	@class	Stk_Text
	 *	@brief	文本类
	 *
	 *	记录单个文本各属性信息。
***************************************************************************************/

class DATAMANAGER_API	Stk_Text
{
private:
	STK_PNT32						m_pntInnerLoc;		//!< 定位点
	STK_DIR32						m_dirAxis[2];		//!< X,Y旋转向量,[0]->X向量,[1]->Y向量
	STK_FLOAT32						m_fCharSize[2];		//!< 字符宽高,[0]->宽；[1]->高
	STK_FLOAT32						m_fCharDis;			//!< 字符间隔
	WCHAR							m_wchFont[64];		//!< 字体，建议用m_wstrFonts替换
	wstring							m_wstrFonts;		//!< 字体
	wstring							m_wstrText;			//!< 字符串内容
	StkTextTypeEnum					m_eUsageType;		//!< 文本的应用类型
	STK_BOX32						m_boxBounding;		//!< 复合文本框
	vector<Stk_OutFrame*>			m_vcOutFrames;		//!< 外框
	STK_DOUBLE64					m_dRotationAngle;	//!< 文本旋转角度
	StkFontStyleTypeEnum			m_eFontStyle;		//!< 字体风格
	StkFontTypeEnum					m_eFontType;		//!< 字体类型
	StkTextAttributeTypeEnum		m_eTextAttribute;	//!< 文本属性
public:
	/****************************************************************************
	 *	@brief		Stk_Text类构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Text(void);

	/****************************************************************************
	 *	@brief		Stk_Text类析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Text(void);

	/****************************************************************************
	 *	@brief		获取文本定位点
	 *	@param[in]	无
	 *	@retval		m_pntInnerLoc		文本定位点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32						GetInnerLoc();

	/****************************************************************************
	 *	@brief		设置文本定位点
	 *	@param[in]	i_pntLocation		定位点
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetInnerLoc(const STK_PNT32& i_pntLocation);

	/****************************************************************************
	 *	@brief		获取文本内容
	 *	@param[in]	无
	 *	@retval		m_wstrText			文本内容
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring							GetText();

	/****************************************************************************
	 *	@brief		设置文本内容
	 *	@param[in]	i_wstrText			文本内容
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetText(wstring i_wstrText);

	/****************************************************************************
	 *	@brief		获取旋转向量
	 *	@param[out]	o_dirXAxis			X方向向量
	 *	@param[out]	o_dirYAxis			Y方向向量
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void							GetInnerXYAxis(STK_DIR32& o_dirXAxis, STK_DIR32& o_dirYAxis);
	
	/****************************************************************************
	 *	@brief		设置旋转向量
	 *	@param[in]	i_dirXAxis			X方向向量
	 *	@param[in]	i_dirYAxis			Y方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetInnerXYAxis(const STK_DIR32& i_dirXAxis, const STK_DIR32& i_dirYAxis);

	/****************************************************************************
	 *	@brief		获取字符宽高
	 *	@param[in]	无
	 *	@param[out]	o_fWidth			字符宽
	 *	@param[out]	o_fHeight			字符高
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void							GetCharWidthHeight(STK_FLOAT32& o_fWidth, STK_FLOAT32& o_fHeight);
	
	/****************************************************************************
	 *	@brief		设置字符宽高
	 *	@param[in]	i_fWidth			字符宽
	 *	@param[in]	i_fHeight			字符高
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetCharWidthHeight(const STK_FLOAT32& i_fWidth, const STK_FLOAT32& i_fHeight);

	/****************************************************************************
	 *	@brief		获取字符间距
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fCharDis			字符间距
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32						GetCharSpacing();

	/****************************************************************************
	 *	@brief		设置字符间距
	 *	@param[in]	i_fCharDis			字符间距
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetCharSpacing(const STK_FLOAT32& i_fCharDis);

	/****************************************************************************
	 *	@brief		获取字体格式
	 *	@param[in]	无
	 *	@param[out]	o_wchFontA			字体
	 *	@retval		无
	 *	@note		建议用GetFonts替换
	 *	@attention	无
	 ****************************************************************************/
	void							GetFont(WCHAR o_wchFontA[STK_MAXNAME]);
	
	/****************************************************************************
	 *	@brief		设置字符格式
	 *	@param[in]	i_wchFontA			字体
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		建议用SetFonts替换
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFont(WCHAR i_wchFontA[STK_MAXNAME]);

	/****************************************************************************
	 *	@brief		获取字体格式
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrFonts			字体
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring							GetFonts();

	/****************************************************************************
	 *	@brief		设置字符格式
	 *	@param[in]	i_wstrFonts			字体
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		不允许超过63字符
	 *	@attention	无
	 ****************************************************************************/
	void							SetFonts(wstring i_wstrFonts);
	
	/****************************************************************************
	 *	@brief		设置字符应用类型
	 *	@param[in]	i_Type			字符应用类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void							SetUsageType(StkTextTypeEnum i_Type);

	/****************************************************************************
	 *	@brief		获取字符应用类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkTextTypeEnum					GetUsageType();

	/****************************************************************************
	 *	@brief		设置文本包围框
	 *	@param[in]	i_boxBounding		包围框
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetOuterBox(const STK_BOX32 &i_boxBounding);

	/****************************************************************************
	 *	@brief		获取文本包围框
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		m_boxBounding		文本包围框
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOX32						GetOuterBox();

	/****************************************************************************
	 *	@brief		获取外框数量
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		外框数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32						GetOutFrameNum();

	/****************************************************************************
	 *	@brief		通过索引获取外框指针
	 *	@param[in]	索引
	 *	@param[out] 无
	 *	@retval		外框数量
	 *	@note		通过GetOutFrameNum来获取数量，索引号为"0"至"数量-1"
	 *	@attention	无
	 ****************************************************************************/
	Stk_OutFrame*					GetOutFrameByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		增加外框
	 *	@param[in]	i_OutFrameP		外框指针
	 *	@param[out] 无
	 *	@retval		状态
	 *	@note		
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						AddOutFrame(Stk_OutFrame* i_OutFrameP);
	
	/****************************************************************************
	 *	@brief		设置文本旋转角度
	 *	@param[in]	i_dRotationAngle	旋转角度
	 *	@param[out] 无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetRotationAngle(STK_DOUBLE64 i_dRotationAngle);

	/****************************************************************************
	 *	@brief		获取文本旋转角度
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		文本旋转角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64					GetRotationAngle();

	/****************************************************************************
	 *	@brief		设置文本字体风格
	 *	@param[in]	i_eFontStyle		文本字体风格
	 *	@param[out] 无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFontStyle(StkFontStyleTypeEnum i_eFontStyle);

	/****************************************************************************
	 *	@brief		获取文本字体风格
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		文本字体风格
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkFontStyleTypeEnum			GetFontStyle();

	/****************************************************************************
	 *	@brief		设置文本字体类型
	 *	@param[in]	i_eFontType			字体类型
	 *	@param[out] 无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFontType(StkFontTypeEnum i_eFontType);

	/****************************************************************************
	 *	@brief		获取文本字体类型
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		文本字体类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkFontTypeEnum					GetFontType();

	/****************************************************************************
	 *	@brief		设置文本属性类型
	 *	@param[in]	i_eAttributeType	文本属性类型
	 *	@param[out] 无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetAttributeType(StkTextAttributeTypeEnum i_eAttributeType);
	
	/****************************************************************************
	 *	@brief		获取文本属性类型
	 *	@param[in]	无
	 *	@param[out] 无
	 *	@retval		文本属性类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkTextAttributeTypeEnum		GetAttributeType();
	
	/*****************************************************************************
	 *	@brief		写入文本数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteText(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_Envelope
	 *	@brief	包络框类
	 *
	 *	
***************************************************************************************/

class DATAMANAGER_API Stk_Envelope
{
private:
	STK_RGBA32							m_rgbaBackground;	//!< 包络框背景色
	STK_BOOL							m_bHasFrame;		//!< 是否有包络框的边框
	STK_RGBA32							m_rgbaFrame;		//!< 包络框边框的颜色
	map<StkAnchorTypeEnum, STK_PNT32>	m_mapAnchorPoints;	//!< 锚点
public:
	/*****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Envelope();
	
	/*****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Envelope();
public:
	/****************************************************************************
	 *	@brief		设置包络框的背景色
	 *	@param[in]	i_rgbaBackground	包络框背景色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetBackgroundColor(STK_RGBA32 i_rgbaBackground);

	/****************************************************************************
	 *	@brief		获取包络框的背景色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		包络框的背景色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32						GetBackgroundColor();

	/****************************************************************************
	 *	@brief		设置是否有包络框的边框的标识
	 *	@param[in]	i_bHasFrame		是否有包络框的边框的标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFrameFlag(STK_BOOL i_bHasFrame);

	/****************************************************************************
	 *	@brief		是否有包络框的边框的标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		是否有包络框的边框的标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL						IsHaveFrame();

	/****************************************************************************
	 *	@brief		设置包络框边框的颜色
	 *	@param[in]	i_rgbaFrame			包络框边框的颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFrameColor(STK_RGBA32 i_rgbaFrame);

	/****************************************************************************
	 *	@brief		获取包络框边框的颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		包络框的背景色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32						GetFrameColor();

	/****************************************************************************
	 *	@brief		设置锚点
	 *	@param[in]	i_eType				锚点类型
	 *	@param[in]	i_pntAnchor			锚点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		其它				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetAnchorPoint(StkAnchorTypeEnum i_eType, STK_PNT32 i_pntAnchor);
	
	/****************************************************************************
	 *	@brief		设置存在的所有锚点类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		存在的所有锚点类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<StkAnchorTypeEnum>&		GetAllAnchorTypes();
	
	/****************************************************************************
	 *	@brief		获取锚点
	 *	@param[in]	i_eType				锚点类型
	 *	@param[out]	o_pntAnchor			锚点
	 *	@retval		STK_SUCCESS			获取成功
	 *	@retval		其它				获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						GetAnchorPointByType(StkAnchorTypeEnum i_eType, STK_PNT32& o_pntAnchor);

	/*****************************************************************************
	 *	@brief		写包络框数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteFile(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_ComText
	 *	@brief	复合文本类
	 *
	 *	对多个文本类对象进行组合。
***************************************************************************************/

class DATAMANAGER_API Stk_ComText
{
private:
	vector<Stk_Text*>				m_vcTexts;			//!< 文本
	STK_BOX32						m_boxBounding;		//!< 复合文本包围盒
	StkFrameTypeEnum				m_eFrameType;		//!< 外框类型
	Stk_Envelope*					m_stkEnvelopeP;		//!< 包络框对象
public:
	/****************************************************************************
	 *	@brief		Stk_ComText构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ComText(void);

	/****************************************************************************
	 *	@brief		Stk_ComText析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ComText(void);

	/****************************************************************************
	 *	@brief		获取文本对象数组的大小
	 *	@param[in]	无
	 *	@retval		m_vcTexts.size()		文本对象数组的大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T						GetNumTexts();

	/****************************************************************************
	 *	@brief		获取文本对象数组
	 *	@param[in]	无
	 *	@retval		m_vcTexts			文本对象数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_Text*>				GetTexts();

	/****************************************************************************
	 *	@brief		设置文本对象数组
	 *	@param[in]	i_vcTexts			文本对象数组
	 *	@retval		STK_SUCCESS			文本设置成功
	 *	@retval		STK_ERROR			文本设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetTexts(const vector<Stk_Text*>& i_vcTexts);

	/****************************************************************************
	 *	@brief		设置复合文本包围框
	 *	@param[in]	i_boxBounding		包围框
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetOuterBox(const STK_BOX32 &i_boxBounding);

	/****************************************************************************
	 *	@brief		获取复合文本包围框
	 *	@param[in]	无
	 *	@retval		m_boxBounding		复合文本包围框
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOX32						GetOuterBox();

	/****************************************************************************
	 *	@brief		设置外框类型
	 *	@param[in]	i_eFrameType		外框类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFrameType(StkFrameTypeEnum i_eFrameType);

	/****************************************************************************
	 *	@brief		获取外框类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		外框类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkFrameTypeEnum				GetFrameType();

	/*****************************************************************************
	 *	@brief		写入复合文本数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteComText(StkSegmentHead* i_head, Stk_File* i_stkFileP);

	/****************************************************************************
	 *	@brief		设置包络框对象
	 *	@param[in]	i_stkEnvelopeP		包络框对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetEnvelope(Stk_Envelope* i_stkEnvelopeP);

	/****************************************************************************
	 *	@brief		获取包络框对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		包络框对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Envelope*					GetEnvelope();
};

#endif //_Stk_CompositeText_H_
