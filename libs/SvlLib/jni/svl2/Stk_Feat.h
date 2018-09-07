// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat.h
 *
 *	@brief	特征类
 *
 *	@par	历史:
 *		2014/05/28	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_FEATENTITY_H_
#define _STK_FEATENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_Enum.h"
#include "Stk_Feat_Pipe.h"
#include "Stk_Feat_Hole.h"
#include "Stk_Feat_MagneticLine.h"
#include "Stk_Feat_Arrow.h"
#include "Stk_Feat_Mark.h"

/****************************************************************************
	 *	@class	Stk_Feat
	 *	@brief	特征类
	 *
	 *	创建用于存储Node节点的特征的类库。<br>
	 *	继承自Stk_Entity 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat :
	public Stk_Entity
{
private:
	StkFeatTypeEnum			m_eType;				//!< 特征类型
	STK_BOOL				m_bActive;				//!< 是否活动状态
	wstring					m_wstrName;				//!< 特征名称
	Stk_Entity*				m_dataP;				//!< 特征数据
public:
	/****************************************************************************
	 *	@brief		Stk_Feat构造函数
	 *	@param[in]	i_stkPrototypeP		所属零件
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat(Stk_ProtoType* i_stkPrototypeP);
	//!< Stk_Feat(Stk_Feat& i_stkFeat);

	/****************************************************************************
	 *	@brief		Stk_Feat析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat();
	//!< const Stk_Feat& operator = (Stk_Feat& i_stkFeat);
public:
	/****************************************************************************
	 *	@brief		设置特征ID
	 *	@param[in]	i_Id			特征ID
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetID(STK_UINT32 i_Id);

	/****************************************************************************
	 *	@brief		注册特征ID
	 *	@param[in]	i_idFeat		特征ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RegisterID(STK_ID i_idFeat);

	/****************************************************************************
	 *	@brief		获取特征ID
	 *	@param[in]	无
	 *	@retval		m_ID		特征ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetID();

	/****************************************************************************
	 *	@brief		设置特征类型
	 *	@param[in]	i_eType			特征类型
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatType(StkFeatTypeEnum i_eType);

	/****************************************************************************
	 *	@brief		获取特征类型
	 *	@param[in]	无
	 *	@retval		m_eType			特征类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkFeatTypeEnum			GetFeatType();

	/****************************************************************************
	 *	@brief		是否活动状态
	 *	@param[in]	无
	 *	@retval		m_bActive		是否活动状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsActiveed();

	/****************************************************************************
	 *	@brief		设置活动状态
	 *	@param[in]	i_bActived		活动状态
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActiveed(STK_BOOL i_bActived);

	/****************************************************************************
	 *	@brief		设置特证名
	 *	@param[in]	i_wstrName		特证名
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatName(wstring i_wstrName);

	/****************************************************************************
	 *	@brief		获取特证名
	 *	@param[in]	无
	 *	@retval		m_wstrName		特证名
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetFeatName();

	/****************************************************************************
	 *	@brief		设置线缆特征
	 *	@param[in]	i_FeatPipe		线缆特征对象指针
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其他			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatPipe(Stk_Feat_Pipe* i_FeatPipe);

	/****************************************************************************
	 *	@brief		获取线缆特征
	 *	@param[in]	无
	 *	@retval		m_dataP			线缆特征
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Pipe*			GetFeatPipe();

	/****************************************************************************
	 *	@brief		设置磁力线特征
	 *	@param[in]	i_MagneticLineP	磁力线特征对象
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其他			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMagneticLine(Stk_Feat_MagneticLine* i_MagneticLineP);

	/****************************************************************************
	 *	@brief		获取磁力线特征
	 *	@param[in]	无
	 *	@retval		磁力线特征
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_MagneticLine*	GetMagneticLine();

	/****************************************************************************
	 *	@brief		设置箭头特征
	 *	@param[in]	i_FeatArrowP	箭头特征对象
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其他			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetArrow(Stk_Feat_Arrow* i_FeatArrowP);

	/****************************************************************************
	 *	@brief		获取箭头特征
	 *	@param[in]	无
	 *	@retval		箭头特征
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Arrow*			GetArrow();

	/****************************************************************************
	 *	@brief		设置标记特征
	 *	@param[in]	i_FeatMarkP		标记特征对象
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其他			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatMark(Stk_Feat_Mark* i_FeatMarkP);

	/****************************************************************************
	 *	@brief		获取标记特征
	 *	@param[in]	无
	 *	@retval		标记特征
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Mark*			GetFeatMark();

	/****************************************************************************
	 *	@brief		读取特征文件
	 *	@param[in]	i_stkFileP		文件指针
	 *	@param[in]	i_FeatHeadSP	特征头记录
	 *	@retval		STK_SUCCESS		读取成功
	 *	@retval		其他			读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_FeatHeadSP);
	
	/****************************************************************************
	 *	@brief		写特征文件
	 *	@param[in]	i_stkFileP		文件指针
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其他			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteFile(Stk_File *i_stkFileP);
};

#endif