// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_View.h
*
*	@brief	视图类
*
*	@par	历史:
*		2017/06/10	创建。qinyp@HOTEAMSOFT
****************************************************************************/


#ifndef _SVLLIB_PMI_H_
#define _SVLLIB_PMI_H_

#pragma once
#include "Stk_Object.h"
#include "Stk_PMIEntity.h"
#include "Stk_CompositeText.h"
#include "Stk_UserData.h"
#include "Stk_Image.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_PMI : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_PMI)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI };	//!< 类别
		public:
			Stk_PMI();
			virtual ~Stk_PMI();
		public:
			void Init(Stk_PMIDocument* i_PMIDocP, int i_iModelID);
		public:
			/****************************************************************************
			*	@brief		设置PMI唯一标识
			*	@param[in]	i_Identity	视图唯一标识
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//void						SetID(STK_ID i_ID);

			/****************************************************************************
			*	@brief		获取PMI唯一标识
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		视图的唯一标识
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//STK_ID			GetID();

			/****************************************************************************
			*	@brief		设置PMI名称
			*	@param[in]	i_strName		PMI名称
			*	@param[out]	无
			*	@retval		无
			*	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
			*	@attention	无
			****************************************************************************/
			void						SetName(const wstring &i_strName);

			/****************************************************************************
			*	@brief		获取PMI名称
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrPMIName		PMI名称
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring						GetName();
			
			/****************************************************************************
			*	@brief		获取定义平面
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_mtxDefPlane		定义平面
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const STK_MTX32&			GetDefPlane();

			/****************************************************************************
			*	@brief		设置定义平面
			*	@param[in]	i_mtxDefPlane		定义平面
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetDefPlane(const STK_MTX32& i_mtxDefPlane);
			
			/****************************************************************************
			*	@brief		获取PMI颜色
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_rgbaPMI		PMI颜色
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const STK_RGBA32&			GetColor();

			/****************************************************************************
			*	@brief		设置PMI颜色
			*	@param[in]	i_rgbaPMI		PMI颜色
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetColor(const STK_RGBA32& i_rgbaPMI);

			/****************************************************************************
			*	@brief		获取PMI类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eType			PMI类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkPMITypeEnum				GetType();

			/****************************************************************************
			*	@brief		设置PMI类型
			*	@param[in]	i_eType			PMI类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetType(StkPMITypeEnum i_nuType);

			/****************************************************************************
			*	@brief		获取PMI样式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_uiStyle			PMI类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const STK_UINT32&           GetStyle();

			/****************************************************************************
			*	@brief		设置PMI样式
			*	@param[in]	i_uiStyle			PMI类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS                 SetStyle(const STK_UINT32& i_uiStyle);

			/****************************************************************************
			*	@brief		获取使用方式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_uiUserType			PMI类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const STK_INT32&           GetUseType();

			/****************************************************************************
			*	@brief		设置PMI样式
			*	@param[in]	i_uiUserType			PMI类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS                 SetUseType(const STK_UINT32& i_uiUseType);

			/****************************************************************************
			*	@brief		设置PMI来源
			*	@param[in]	i_ePMISource		PMI来源
			*	@param[out]	无
			*	@retval		STK_SUCCESS
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetPMISource(StkPMISourceEnum i_ePMISource);

			/****************************************************************************
			*	@brief		获取PMI来源
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_ePMISource	PMI来源
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkPMISourceEnum			GetPMISource();

			/****************************************************************************
			*	@brief		设置PMI的X方向向量
			*	@param[in]	i_dirX			X方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetAxisX(const STK_DIR32& i_dirX);

			/****************************************************************************
			*	@brief		获取PMI的X方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		X方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32&					GetAxisX();

			/****************************************************************************
			*	@brief		设置PMI的Y方向向量
			*	@param[in]	i_dirY			Y方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetAxisY(const STK_DIR32& i_dirY);

			/****************************************************************************
			*	@brief		获取PMI的Y方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Y方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32&					GetAxisY();

			/****************************************************************************
			*	@brief		设置符号基准点位置
			*	@param[in]	i_pntSymBase		符号基准点位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetSymBasePoint(const STK_PNT32& i_pntSymBase);

			/****************************************************************************
			*	@brief		获取符号基准点位置
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		符号基准点位置
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32&					GetSymBasePoint();

			/****************************************************************************
			*	@brief		判断PMI显隐状态
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		显隐状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsDisplay();

			/****************************************************************************
			*	@brief		设置PMI显隐状态
			*	@param[in]	i_bIsDisplay	显隐状态
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetDisplay(STK_BOOL i_bIsDisplay);

			/****************************************************************************
			*	@brief		获取引出线数组数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcLeaders		引出线数组
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<Stk_LeaderPtr>&	GetLeaders();

			/****************************************************************************
			*	@brief		设置引出线数组数据
			*	@param[in]	i_vcLeaders		引出线数组
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetLeaders(const vector<Stk_LeaderPtr>& i_vcLeaders);

			/****************************************************************************
			*	@brief		添加引出线
			*	@param[in]	i_leaderP		引出线
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddLeader(Stk_LeaderPtr i_leaderP);
			
			/****************************************************************************
			*	@brief		获取延长线数组数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcExLines		延长线数组
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<Stk_ExtensionlinePtr>& GetExLines();

			/****************************************************************************
			*	@brief		添加延长线
			*	@param[in]	i_exlineP 延长线对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS             AddExline(Stk_ExtensionlinePtr i_exlineP);

			/****************************************************************************
			*	@brief		设置延长线数组数据
			*	@param[in]	i_vcExLines		延长线数组
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetExLines(const vector<Stk_ExtensionlinePtr>& i_vcExLines);

			/****************************************************************************
			*	@brief		获取PMI中复合文本数组数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcComTexts	复合文本数组数据
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<Stk_ComTextPtr>& GetComText();

			/****************************************************************************
			*	@brief		往PMI中添加复合文本数据
			*	@param[in]	i_ComTextP		复合文本
			*	@param[out]	无
			*	@retval		STK_SUCCESS		添加成功
			*	@retval		STK_ERROR		添加失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddComText(Stk_ComTextPtr i_ComTextP);

			/****************************************************************************
			*	@brief		获取PMI中外框数组数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcOutFrames	外框数组数据
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<Stk_OutFramePtr>& GetOutFrame();

			/****************************************************************************
			*	@brief		PMI中设置外框数据
			*	@param[in]	i_OutFrameP		指向外框的指针
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@retval		STK_ERROR		设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetOutFrame(Stk_OutFramePtr i_OutFrameP);

			/****************************************************************************
			*	@brief		获取PMI中特殊线数组数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcSpecialLines	特殊线数组数据
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<Stk_SpecialLinePtr>& GetSpecialLines();

			/****************************************************************************
			*	@brief		往PMI中添加特殊线数据
			*	@param[in]	i_SpecLineP		特殊线数据
			*	@param[out]	无
			*	@retval		STK_SUCCESS		添加成功
			*	@retval		STK_ERROR		添加失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddSpecialLine(Stk_SpecialLinePtr i_SpecLineP);

			/****************************************************************************
			*	@brief		添加图片(图章)
			*	@param[in]	i_stkPictureP		图片(图章)
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddImage(Stk_ImagePtr i_stkImageP);

			/****************************************************************************
			*	@brief		获取图片(图章)的数量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片(图章)的数量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32					GetImageNum();

			/****************************************************************************
			*	@brief		获取指定的图片(图章)
			*	@param[in]	i_nuIndex			索引
			*	@param[out]	无
			*	@retval		指定的图片(图章)
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_ImagePtr				GetImageByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		插入用户数据
			*	@param[in]	i_stkUserDataP		用户数据指针
			*	@retval		STK_SUCCESS			成功
			*	@retval		其它				失败
			*	@note		
			*				通过“用户数据名称”+“用户数据ID”的方式来作为唯一标识
			*				  例如“动画”+“1” 表示第一个动画 “动画”+“2” 表示第二个动画
			*				  当两次设置的唯一标识相同时，会自动覆盖掉前次的数据
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddUserData(Stk_UserDataPtr i_stkUserDataP);

			/****************************************************************************
			*	@brief		通过名称获取用户数据列表
			*	@param[in]	用户数据名称
			*	@param[out]	无
			*	@retval		用户数据列表
			*	@note		无
			*	@attention	无
			****************************************************************************/
			map<STK_ID, Stk_UserDataPtr>* GetUserDataByName(wstring i_wstrDataName);

			/****************************************************************************
			*	@brief		删除指定的用户数据
			*	@param[in]	i_wstrDataName		用户数据名称
			*	@param[in]	i_id				用户数据ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			找到数据，删除成功
			*	@retval		STK_ERROR			没有找到数据，删除失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					DelUserData(wstring i_wstrDataName, STK_ID i_id);

			/****************************************************************************
			*	@brief		判断PMI文字是否采用平行屏幕显示
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsParallelScreen	是否平行于屏幕
			*	@note		== true 平行屏幕显示 <br>
			*				== false 不平行屏幕显示
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsParallelScreen();

			/****************************************************************************
			*	@brief		设置PMI文字是否采用平行屏幕显示
			*	@param[in]	i_bIsParallelScreen	是否平行于屏幕
			*	@param[out]	无
			*	@retval		无
			*	@note		== true 平行屏幕显示 <br>
			*				== false 不平行屏幕显示
			*	@attention	无
			****************************************************************************/
			void						SetParallelScreen(STK_BOOL i_bIsParallelScreen);

			/****************************************************************************
			*	@brief		判断PMI的文字是否固定位置
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsFix		是否固定
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsFix();

			/****************************************************************************
			*	@brief		设置PMI的文字是否固定位置
			*	@param[in]	i_bIsFix		是否固定
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetFix(STK_BOOL i_bIsFix);

			/****************************************************************************
			*	@brief		是否是旧版本的PMI
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		新旧版本的PMI标志
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsOldVersion();

			/****************************************************************************
			*	@brief		设置PMI新旧版本标志
			*	@param[in]	i_bOldVersion		PMI新旧版本标志
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetOldVersion(STK_BOOL i_bOldVersion);

			/****************************************************************************
			*	@brief		是否被调整过
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsAdjusted	调整状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsAdjusted();

			/****************************************************************************
			*	@brief		设置调整标志
			*	@param[in]	i_bIsAdjusted	调整状态
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetAdjusted(STK_BOOL i_bIsAdjusted);

			/****************************************************************************
			*	@brief		是否是最前端显示的PMI
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		最前端显示PMI标识
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsFront();

			/****************************************************************************
			*	@brief		设置是否是最前端显示PMI的标识
			*	@param[in]	i_bFront			是否是最前端显示PMI的标识
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetFront(STK_BOOL i_bFront);
		};
	}
}
#endif
