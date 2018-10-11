// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_CompositeText.h
*
*	@brief	文本、复合文本类的声明
*
*	@par	历史:
*		
****************************************************************************/
#ifndef _SVLLIB_COMPOSITETEXT_H_
#define _SVLLIB_COMPOSITETEXT_H_

#include "Stk_Object.h"
#include "Stk_PMIEntity.h"

namespace HoteamSoft {
	namespace SVLLib {
		
		/**************************************************************************************
		*	@class	Stk_ComText
		*	@brief	复合文本类
		*
		*	对多个文本类对象进行组合。
		***************************************************************************************/
		class Stk_Text;
		class STK_TOOLKIT_EXPORT Stk_ComText : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_ComText)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_COMTEXT };	//!< 类别
		public:
			Stk_ComText();
			virtual ~Stk_ComText();
		public:
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
			vector<Stk_TextPtr>				GetTexts();

			/****************************************************************************
			*	@brief		设置文本对象数组
			*	@param[in]	i_vcTexts			文本对象数组
			*	@retval		STK_SUCCESS			文本设置成功
			*	@retval		STK_ERROR			文本设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetTexts(const vector<Stk_TextPtr>& i_vcTexts);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		设置包络框对象
			*	@param[in]	i_stkEnvelopeP		包络框对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetEnvelope(Stk_EnvelopePtr i_stkEnvelopeP);

			/****************************************************************************
			*	@brief		获取包络框对象
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		包络框对象
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_EnvelopePtr					GetEnvelope();
		};

		/**************************************************************************************
		*	@class	Stk_Text
		*	@brief	文本类
		*
		*	记录单个文本各属性信息。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Text : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Text)
		public:
			Stk_Text();
			virtual ~Stk_Text();
		public:
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
			*	@brief		设置文本旋转角度
			*	@param[in]	i_fRotationAngle	旋转角度
			*	@param[out] 无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetRotationAngle(STK_DOUBLE64 i_fRotationAngle);

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
			*	@brief		获取字符宽高
			*	@param[in]	无
			*	@param[out]	o_fWidth			字符宽
			*	@param[out]	o_fHeight			字符高
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void							GetCharWidthAndHeight(STK_FLOAT32& o_fWidth, STK_FLOAT32& o_fHeight);

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
			STK_STATUS						SetCharWidthAndHeight(const STK_FLOAT32& i_fWidth, const STK_FLOAT32& i_fHeight);
			
			/****************************************************************************
			*	@brief		获取字符间距
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fCharDis			字符间距
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32						GetCharSpace();

			/****************************************************************************
			*	@brief		设置字符间距
			*	@param[in]	i_fCharDis			字符间距
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetCharSpace(const STK_FLOAT32& i_fCharSpace);

			/****************************************************************************
			*	@brief		获取字体格式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrFontName			字体
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring							GetFontName();

			/****************************************************************************
			*	@brief		设置字符格式
			*	@param[in]	i_wstrFontName			字体
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		不允许超过63字符
			*	@attention	无
			****************************************************************************/
			void							SetFontName(wstring i_wstrFontName);

			/****************************************************************************
			*	@brief		获取文本内容
			*	@param[in]	无
			*	@retval		m_wstrText			文本内容
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring							GetTextContent();

			/****************************************************************************
			*	@brief		设置文本内容
			*	@param[in]	i_wstrText			文本内容
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetTextContent(wstring i_wstrText);

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
			Stk_OutFramePtr					GetOutFrameByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		增加外框
			*	@param[in]	i_OutFrameP		外框指针
			*	@param[out] 无
			*	@retval		状态
			*	@note
			*	@attention	无
			****************************************************************************/
			STK_STATUS						AddOutFrame(Stk_OutFramePtr i_OutFrameP);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Envelope
		*	@brief	包络框类
		*
		*
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Envelope : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Envelope)
		public:
			Stk_Envelope();
			virtual ~Stk_Envelope();
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
			*	@brief		设置是否有包络框的边框的标识
			*	@param[in]	i_bHasFrame		是否有包络框的边框的标识
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetStyle(STK_INT32 i_iStyle);

			/****************************************************************************
			*	@brief		获取包络框的边框的标识
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		是否有包络框的边框的标识
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL						GetStyle();

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
			*	@brief		获取存在的所有锚点类型
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
			
			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif // !_STK_COMPOSITETEXT_H