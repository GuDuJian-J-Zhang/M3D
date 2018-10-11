// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_InsAttribute.h
 *
 *	@brief	实例属性：颜色、显隐、透明度等
 *
 *	@par	历史:
 *		2015/03/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_InsAttribute_H_
#define _Stk_InsAttribute_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Define.h"
#include "Stk_File.h"

/**************************************************************************************
	 *	@class	Stk_InsAttribute
	 *	@brief	实例基本属性类
	 *
	 *	实例属性：颜色、显隐、透明度等
***************************************************************************************/
class DATAMANAGER_API Stk_InsAttribute
{
private:
	StkDisplayStateEnum	m_eDisplayState;		// 实例显隐
	STK_BOOL			m_bHasColor;			// 是否有颜色
	STK_RGBA32			m_rgbaIns;				// 实例颜色
	STK_BOOL			m_bHasMaterial;			// 是否有材质
	STK_MAT32			m_matIns;				// 实例材质
	STK_BOOL			m_bHasPlacement;		// 是否有实例配置矩阵
	STK_ID				m_idPlacement;			// 实例配置矩阵ID
	STK_MTX32			m_mtxPlacement;			// 实例相对于父级的配置矩阵
public:
	/****************************************************************************
	 *	@brief		Stk_InsAttribute构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_InsAttribute();

	/****************************************************************************
	 *	@brief		Stk_InsAttribute析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_InsAttribute();
public:
	/*****************************************************************************
	 *	@brief		设置实例显隐状态
	 *	@param[in]	i_eDisplayState		实例显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		其他				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDisplayState(StkDisplayStateEnum i_eDisplayState);
	
	/*****************************************************************************
	 *	@brief		获取实例显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkDisplayStateEnum	GetDisplayState();

	/*****************************************************************************
	 *	@brief		设置实例颜色
	 *	@param[in]	i_rgbaIns			实例颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		其他				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetColor(STK_RGBA32 i_rgbaIns);
	
	/*****************************************************************************
	 *	@brief		获取实例颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32&			GetColor();

	/*****************************************************************************
	 *	@brief		设置实例材质
	 *	@param[in]	i_matIns			实例材质
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		其他				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetMaterial(STK_MAT32 i_matIns);
	
	/*****************************************************************************
	 *	@brief		获取实例材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MAT32&			GetMaterial();

	/****************************************************************************
	 *	@brief		设置相对于父级的配置ID，配置矩阵
	 *	@param[in]	i_idPlacement			配置ID
	 *	@param[in]	i_mtxPlcMatrix			配置矩阵
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix);		// 设置配置信息
	
	/****************************************************************************
	 *	@brief		获取相对于父级的配置ID，配置矩阵
	 *	@param[out]	o_idPlacement			配置ID
	 *	@param[out]	o_mtxPlcMatrix			配置矩阵
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix);	// 获取配置信息
	
	/****************************************************************************
	 *	@brief		写实例属性记录
	 *	@param[in]	i_stkInsAttrHeadP	自定义属性段头
	 *	@param[in]	i_stkFileP			SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(StkSegmentHead* i_stkInsAttrHeadP, Stk_File *i_stkFileP);
};

#endif