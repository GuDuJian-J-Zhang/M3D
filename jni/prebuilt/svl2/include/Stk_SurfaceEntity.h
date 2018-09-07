#ifndef _Stk_SURFACE_ENTITY_H_
#define _Stk_SURFACE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"

class Stk_ClippingCurve;
/**************************************************************************************
	 *	@class	Stk_SurfaceEntity
	 *	@brief	Stk_SurfaceEntity 基类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_SurfaceEntity :
	public Stk_Entity
{
protected:
	STK_RGBA32			m_rgbaSurface;	//!< 颜色
	StkSurfaceTypeEnum	m_eFaceType;	//!< 面类型

public:
	/****************************************************************************
	 *	@brief		Stk_SurfaceEntity构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SurfaceEntity(void);
	
	/****************************************************************************
	 *	@brief		Stk_SurfaceEntity析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	virtual ~Stk_SurfaceEntity(void);

	/****************************************************************************
	 *	@brief		获取剖面ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID			剖面ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32			GetID();
	
	/****************************************************************************
	 *	@brief		设置剖面ID
	 *	@param[in]	i_ID			剖面ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取剖面类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuType		剖面类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkSurfaceTypeEnum	GetType();
	//void				SetFaceType(StkSurfaceTypeEnum i_eType); //NOTE：在子类的构造函数中自动设置

	/****************************************************************************
	 *	@brief		获取剖面颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaSurface	剖面颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32			GetColor();
	
	/****************************************************************************
	 *	@brief		设置剖面颜色
	 *	@param[in]	i_rgbaSurface	剖面颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetColor(const STK_RGBA32& i_rgbaSurface);

};


/**************************************************************************************
	 *	@class	Stk_PlaneSurface
	 *	@brief	Stk_PlaneSurface 类
	 *
	 *	继承自Stk_SurfaceEntity类。
***************************************************************************************/

class DATAMANAGER_API Stk_PlaneSurface :
	public Stk_SurfaceEntity
{
private:
	STK_PNT32			m_pntOrigin;		//!< 起始范围
	STK_DIR32			m_dirNormal;		//!< 法线
	STK_DIR32			m_dirU;				//!< U方向
	STK_DIR32			m_dirV;				//!< V方向
	STK_FLOAT32			m_fDistance;		//!< 剖切深度：原点与剖切面的距离深度
	Stk_ClippingCurve*	m_stkClippingCurveP;//!< 剖切线
public:
	/****************************************************************************
	 *	@brief		Stk_PlaneSurface构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PlaneSurface(void);
	
	/****************************************************************************
	 *	@brief		Stk_PlaneSurface析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_PlaneSurface(void);

	/****************************************************************************
	 *	@brief		获取剖面的某点位置
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntOrigin		剖面上某点位置
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetOrigin();
	
	/****************************************************************************
	 *	@brief		设置剖面的某点位置
	 *	@param[in]	i_pntOrigin		剖面上某点位置
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void				SetOrigin(const STK_PNT32 &i_pntOrigin);

	/****************************************************************************
	 *	@brief		获取剖面法向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirNormal		剖面法向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetNormal();

	/****************************************************************************
	 *	@brief		设置剖面法向量
	 *	@param[in]	i_dirNormal		剖面法向量
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void				SetNormal(const STK_DIR32 &i_dirNormal);

	/****************************************************************************
	 *	@brief		获取剖面UV向量
	 *	@param[in]	o_dirU		剖面U向量
	 *	@param[in]	o_dirV		剖面V向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			GetUVDir(STK_DIR32 &o_dirU, STK_DIR32 &o_dirV);
	
	/****************************************************************************
	 *	@brief		设置剖面UV向量
	 *	@param[in]	i_dirU		剖面U向量
	 *	@param[in]	i_dirV		剖面V向量
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void				SetUVDir(const STK_DIR32 &i_dirU, const STK_DIR32 &i_dirV);

	/****************************************************************************
	 *	@brief		获取剖切深度:原点与剖切面深度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		深度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32			GetDistance();
	
	/****************************************************************************
	 *	@brief		设置剖切深度:原点与剖切面深度
	 *	@param[in]	i_fDistance		深度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDistance(STK_FLOAT32 i_fDistance);

	/****************************************************************************
	 *	@brief		获取剖切线对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖切线对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ClippingCurve*	GetClippingCurve();
	
	/****************************************************************************
	 *	@brief		设置剖切线对象
	 *	@param[in]	i_stkClippingCurveP	剖切线对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetClippingCurve(Stk_ClippingCurve* i_stkClippingCurveP);
};

/**************************************************************************************
	 *	@class	Stk_ClippingCurve
	 *	@brief	剖切阴影线类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/
class DATAMANAGER_API Stk_ClippingCurve :
	public Stk_Entity
{
private:
	STK_BOOL						m_bDisplay;		//!< 是否显示剖切线
	StkClippingCurveDrawStyleEnum	m_eDrawStyle;	//!< 绘制显示方式
	StkClippingCurveDrawTypeEnum	m_eDrawType;	//!< 绘制显示类型
	STK_FLOAT32						m_fAngle;		//!< 剖切线角度
	STK_FLOAT32						m_fRatio;		//!< 剖切线显示比例
	STK_RGBA32						m_rgbaCurve;	//!< 剖切线颜色
public:
	/****************************************************************************
	 *	@brief		Stk_ClippingCurve构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ClippingCurve(void);

	/****************************************************************************
	 *	@brief		Stk_ClippingCurve析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ClippingCurve(void);
public:
	/****************************************************************************
	 *	@brief		设置剖切线显隐状态
	 *	@param[in]	i_bDisplay		显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetDisplay(STK_BOOL i_bDisplay);

	/****************************************************************************
	 *	@brief		获取剖切线显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL		GetDisplay();

	/****************************************************************************
	 *	@brief		设置剖切线绘制方式
	 *	@param[in]	i_eDrawStyle	绘制方式
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetDrawStyle(StkClippingCurveDrawStyleEnum i_eDrawStyle);

	/****************************************************************************
	 *	@brief		获取剖切线绘制方式
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		绘制方式
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkClippingCurveDrawStyleEnum	GetDrawStyle();

	/****************************************************************************
	 *	@brief		设置剖切线绘制显示类型
	 *	@param[in]	i_eDrawType		剖切线绘制显示类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetDrawType(StkClippingCurveDrawTypeEnum i_eDrawType);

	/****************************************************************************
	 *	@brief		获取剖切线绘制显示类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖切线绘制显示类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkClippingCurveDrawTypeEnum	GetDrawType();

	/****************************************************************************
	 *	@brief		设置剖切线角度
	 *	@param[in]	i_fAngle		剖切线角度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetAngle(STK_FLOAT32 i_fAngle);

	/****************************************************************************
	 *	@brief		获取剖切线角度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖切线角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetAngle();

	/****************************************************************************
	 *	@brief		设置剖切线显示比例
	 *	@param[in]	i_fRatio		剖切线显示比例
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetRatio(STK_FLOAT32 i_fRatio);

	/****************************************************************************
	 *	@brief		获取剖切线显示比例
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖切线显示比例
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetRatio();

	/****************************************************************************
	 *	@brief		设置剖切线显示颜色
	 *	@param[in]	i_rgbaCurve		剖切线显示颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetColor(STK_RGBA32 i_rgbaCurve);

	/****************************************************************************
	 *	@brief		获取剖切线显示颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖切线显示颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32		GetColor();
};

#endif