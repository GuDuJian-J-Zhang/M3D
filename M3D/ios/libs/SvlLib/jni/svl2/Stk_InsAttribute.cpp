// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_InsAttribute.cpp
 *
 *	@brief	实例属性
 *
 *	@par	历史:
 *		2015/03/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_InsAttribute.h"

/************************************************************************//**
 *	@brief		Stk_InsAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_InsAttribute::Stk_InsAttribute()
{
	m_eDisplayState = STK_DISPLAY;
	m_bHasColor = false;
	m_bHasMaterial = false;
	m_bHasPlacement = false;
	m_rgbaIns.Red = 0;
	m_rgbaIns.Green = 0;
	m_rgbaIns.Blue = 0;
	m_rgbaIns.Alpha = 0;
	m_idPlacement = 0;
	memset(&m_mtxPlacement, 0, sizeof(STK_MTX32));
}

/************************************************************************//**
 *	@brief		Stk_InsAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_InsAttribute::~Stk_InsAttribute()
{

}

/*************************************************************************//**
 *	@brief		设置实例显隐状态
 *	@param[in]	i_eDisplayState		实例显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其他				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_InsAttribute::SetDisplayState(StkDisplayStateEnum i_eDisplayState)
{
	m_eDisplayState = i_eDisplayState;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取实例显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkDisplayStateEnum Stk_InsAttribute::GetDisplayState()
{
	return m_eDisplayState;
}

/*************************************************************************//**
 *	@brief		设置实例颜色
 *	@param[in]	i_rgbaIns			实例颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其他				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_InsAttribute::SetColor(STK_RGBA32 i_rgbaIns)
{
	m_rgbaIns = i_rgbaIns;
	m_bHasColor = true;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取实例颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32& Stk_InsAttribute::GetColor()
{
	return m_rgbaIns;
}

/*************************************************************************//**
 *	@brief		设置实例材质
 *	@param[in]	i_matIns			实例材质
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其他				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_InsAttribute::SetMaterial(STK_MAT32 i_matIns)
{
	m_matIns = i_matIns;
	m_bHasMaterial = true;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取实例材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_MAT32& Stk_InsAttribute::GetMaterial()
{
	return m_matIns;
}

/************************************************************************//**
 *	@brief		设置相对于父级的配置ID，配置矩阵
 *	@param[in]	i_idPlacement			配置ID
 *	@param[in]	i_mtxPlcMatrix			配置矩阵
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_InsAttribute::SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix)
{
	m_idPlacement = i_idPlacement;
	m_mtxPlacement = i_mtxPlcMatrix;
	m_bHasPlacement = true;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取相对于父级的配置ID，配置矩阵
 *	@param[out]	o_idPlacement			配置ID
 *	@param[out]	o_mtxPlcMatrix			配置矩阵
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_InsAttribute::GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix)
{
	o_idPlacement = m_idPlacement;
	o_mtxPlcMatrix = m_mtxPlacement;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		写实例属性记录
 *	@param[in]	i_stkInsAttrHeadP	自定义属性段头
 *	@param[in]	i_stkFileP			SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_InsAttribute::WriteFile(StkSegmentHead* i_stkInsAttrHeadP, Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkInstanceColor		InsColorRec = {0};
	StkInstanceDisplay		InsDisplayRec = {0};
	StkInstanceMater		InsMaterialRec = {0};
	StkInsPlacement			InsPlacementRec = {0};

	if (i_stkInsAttrHeadP == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	// 实例显隐记录
	if (m_eDisplayState == STK_NO_DISPLAY)
	{
		InsDisplayRec.Control.type = REC_TYPE_INS_DSP;
		InsDisplayRec.Control.version = _VER_INS_DSP_RECORD;
		InsDisplayRec.Control.length = sizeof(STK_UINT32);
		InsDisplayRec.disp = m_eDisplayState;

		eState = i_stkFileP->WriteSegmentRecordData(i_stkInsAttrHeadP, (StkControl *)&InsDisplayRec);
		if(eState < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 实例颜色记录
	if (m_bHasColor)
	{
		InsColorRec.Control.type = REC_TYPE_INS_COL;
		InsColorRec.Control.version = _VER_INS_COL_RECORD;
		InsColorRec.Control.length = sizeof(STK_FLOAT32) * 4;
		
		InsColorRec.color[0] = m_rgbaIns.Red;
		InsColorRec.color[1] = m_rgbaIns.Green;
		InsColorRec.color[2] = m_rgbaIns.Blue;
		InsColorRec.color[3] = m_rgbaIns.Alpha;

		eState = i_stkFileP->WriteSegmentRecordData(i_stkInsAttrHeadP, (StkControl *)&InsColorRec);
		if(eState < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 实例材质记录
	if (m_bHasMaterial)
	{
		InsMaterialRec.Control.type = REC_TYPE_INS_MAT;
		InsMaterialRec.Control.version = _VER_INS_MAT_RECORD;
		InsMaterialRec.Control.length = sizeof(STK_FLOAT32) * 3;
		
		InsMaterialRec.mat[0] = m_matIns.Ambient;
		InsMaterialRec.mat[1] = m_matIns.Diffuse;
		InsMaterialRec.mat[2] = m_matIns.Specular;

		eState = i_stkFileP->WriteSegmentRecordData(i_stkInsAttrHeadP, (StkControl *)&InsMaterialRec);
		if(eState < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 实例相对于父级的配置矩阵
	if (m_bHasPlacement)
	{
		InsPlacementRec.Control.type = REC_TYPE_INS_PLACEMENT;
		InsPlacementRec.Control.version = _VER_PLACEMENT_RECORD;
		InsPlacementRec.Control.length = sizeof(StkInsPlacement) - sizeof(StkControl);

		InsPlacementRec.plcId = m_idPlacement;
		memcpy(InsPlacementRec.Matrix, m_mtxPlacement.PlcMatrix, sizeof(STK_FLOAT32)*4*4);

		eState = i_stkFileP->WriteSegmentRecordData(i_stkInsAttrHeadP, (StkControl *)&InsPlacementRec);
		if(eState < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	return eState;
}