#include "StdAfx.h"
#include "Stk_SurfaceEntity.h"

/************************************************************************//**
 *	@brief		Stk_SurfaceEntity构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SurfaceEntity::Stk_SurfaceEntity(void)
{
	m_ID = 0;
	m_rgbaSurface.Red = -1.0;
	m_rgbaSurface.Green = -1.0;
	m_rgbaSurface.Blue = -1.0;
	m_rgbaSurface.Alpha = -1.0;
	m_eFaceType = SURFACE_TYPE_UNKNOWN;
}

/************************************************************************//**
 *	@brief		Stk_SurfaceEntity析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SurfaceEntity::~Stk_SurfaceEntity(void)
{

}

/************************************************************************//**
 *	@brief		获取剖面ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID			剖面ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_SurfaceEntity::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置剖面ID
 *	@param[in]	i_ID			剖面ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SurfaceEntity::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖面类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuType		剖面类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkSurfaceTypeEnum Stk_SurfaceEntity::GetType()
{
	return m_eFaceType;
}

/****************************************************************************
 *	@brief		设置剖面类型
 *	@param[in]	i_eType			剖面类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//void Stk_SurfaceEntity::SetFaceType(StkSurfaceTypeEnum i_eType)
//{
//	m_eFaceType = i_eType;
//}

/************************************************************************//**
 *	@brief		获取剖面颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaSurface	剖面颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_SurfaceEntity::GetColor()
{
	return m_rgbaSurface;
}

/************************************************************************//**
 *	@brief		设置剖面颜色
 *	@param[in]	i_rgbaSurface	剖面颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SurfaceEntity::SetColor(const STK_RGBA32& i_rgbaSurface)
{
	m_rgbaSurface = i_rgbaSurface;
	return STK_SUCCESS;
}

//void Stk_SurfaceEntity::GetDomain(STK_FLOAT32& oMin, STK_FLOAT32& oMax)
//{
//
//}


/************************************************************************//**
 *	@brief		Stk_PlaneSurface构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_PlaneSurface::Stk_PlaneSurface(void)
{
	m_pntOrigin.X = 0.0;
	m_pntOrigin.Y = 0.0;
	m_pntOrigin.Z = 0.0;
	m_dirNormal.X = 0.0;
	m_dirNormal.Y = 0.0;
	m_dirNormal.Z = 0.0;
	m_dirU.X = 0.0;
	m_dirU.Y = 0.0;
	m_dirU.Z = 0.0;
	m_dirV.X = 0.0;
	m_dirV.Y = 0.0;
	m_dirV.Z = 0.0;
	m_eFaceType = SURFACE_TYPE_PLANE;
	m_fDistance = 0.0;
	m_stkClippingCurveP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_PlaneSurface析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_PlaneSurface::~Stk_PlaneSurface(void)
{
	if (m_stkClippingCurveP != NULL)
	{
		delete m_stkClippingCurveP;
		m_stkClippingCurveP = NULL;
	}
}

/************************************************************************//**
 *	@brief		获取剖面的某点位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntOrigin		剖面上某点位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_PlaneSurface::GetOrigin()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		设置剖面的某点位置
 *	@param[in]	i_pntOrigin		剖面上某点位置
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PlaneSurface::SetOrigin(const STK_PNT32 &i_pntOrigin )
{
	m_pntOrigin = i_pntOrigin;
}

/************************************************************************//**
 *	@brief		获取剖面法向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirNormal		剖面法向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_DIR32 Stk_PlaneSurface::GetNormal()
{
	return m_dirNormal;
}

/************************************************************************//**
 *	@brief		设置剖面法向量
 *	@param[in]	i_dirNormal		剖面法向量
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PlaneSurface::SetNormal(const STK_DIR32 &i_dirNormal)
{
	m_dirNormal = i_dirNormal;
}

/************************************************************************//**
 *	@brief		设置剖面UV向量
 *	@param[in]	i_dirU		剖面U向量
 *	@param[in]	i_dirV		剖面V向量
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PlaneSurface::SetUVDir(const STK_DIR32 &i_dirU, const STK_DIR32 &i_dirV)
{
	m_dirU = i_dirU;
	m_dirU = i_dirV;
}

/************************************************************************//**
 *	@brief		获取剖面UV向量
 *	@param[in]	o_dirU		剖面U向量
 *	@param[in]	o_dirV		剖面V向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS	获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PlaneSurface::GetUVDir(STK_DIR32 &o_dirU, STK_DIR32 &o_dirV)
{
	o_dirU = m_dirU;
	o_dirV = m_dirU;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切深度:原点与剖切面深度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		深度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_PlaneSurface::GetDistance()
{
	return m_fDistance;
}

/************************************************************************//**
 *	@brief		设置剖切深度:原点与剖切面深度
 *	@param[in]	i_fDistance		深度
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PlaneSurface::SetDistance(STK_FLOAT32 i_fDistance)
{
	m_fDistance = i_fDistance;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖切线对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ClippingCurve* Stk_PlaneSurface::GetClippingCurve()
{
	return m_stkClippingCurveP;
}

/************************************************************************//**
 *	@brief		设置剖切线对象
 *	@param[in]	i_stkClippingCurveP	剖切线对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PlaneSurface::SetClippingCurve(Stk_ClippingCurve* i_stkClippingCurveP)
{
	if (i_stkClippingCurveP == NULL)
	{
		return STK_ERROR;
	}
	m_stkClippingCurveP = i_stkClippingCurveP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_ClippingCurve构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ClippingCurve::Stk_ClippingCurve()
{
	m_bDisplay = false;
	m_eDrawStyle = CLIPPINGCURVE_STYLE_DATABASE;
	m_eDrawType = CLIPPINGCURVE_DRAW_TYPE_ANSI31;
	m_fAngle = 0;
	m_fRatio = 1;
	m_rgbaCurve.Red = 0;
	m_rgbaCurve.Green = 0;
	m_rgbaCurve.Blue = 1;
	m_rgbaCurve.Alpha = -1;
}

/************************************************************************//**
 *	@brief		Stk_ClippingCurve析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ClippingCurve::~Stk_ClippingCurve()
{

}

/************************************************************************//**
 *	@brief		设置剖切线显隐状态
 *	@param[in]	i_bDisplay		显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetDisplay(STK_BOOL i_bDisplay)
{
	m_bDisplay = i_bDisplay;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ClippingCurve::GetDisplay()
{
	return m_bDisplay;
}

/************************************************************************//**
 *	@brief		设置剖切线绘制方式
 *	@param[in]	i_eDrawStyle	绘制方式
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetDrawStyle(StkClippingCurveDrawStyleEnum i_eDrawStyle)
{
	m_eDrawStyle = i_eDrawStyle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线绘制方式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		绘制方式
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkClippingCurveDrawStyleEnum Stk_ClippingCurve::GetDrawStyle()
{
	return m_eDrawStyle;
}

/************************************************************************//**
 *	@brief		设置剖切线绘制显示类型
 *	@param[in]	i_eDrawType		剖切线绘制显示类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetDrawType(StkClippingCurveDrawTypeEnum i_eDrawType)
{
	m_eDrawType = i_eDrawType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线绘制显示类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖切线绘制显示类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkClippingCurveDrawTypeEnum Stk_ClippingCurve::GetDrawType()
{
	return m_eDrawType;
}

/************************************************************************//**
 *	@brief		设置剖切线角度
 *	@param[in]	i_fAngle		剖切线角度
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetAngle(STK_FLOAT32 i_fAngle)
{
	m_fAngle = i_fAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线角度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖切线角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_ClippingCurve::GetAngle()
{
	return m_fAngle;
}

/************************************************************************//**
 *	@brief		设置剖切线显示比例
 *	@param[in]	i_fRatio		剖切线显示比例
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetRatio(STK_FLOAT32 i_fRatio)
{
	m_fRatio = i_fRatio;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线显示比例
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖切线显示比例
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_ClippingCurve::GetRatio()
{
	return m_fRatio;
}

/************************************************************************//**
 *	@brief		设置剖切线显示颜色
 *	@param[in]	i_rgbaCurve		剖切线显示颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ClippingCurve::SetColor(STK_RGBA32 i_rgbaCurve)
{
	m_rgbaCurve = i_rgbaCurve;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取剖切线显示颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		剖切线显示颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_ClippingCurve::GetColor()
{
	return m_rgbaCurve;
}