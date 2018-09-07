// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_CurveEntity.h
 *
 *	@brief	Stk_CurveEntity类、Stk_PolyLine类、Stk_Ellipse类和Stk_Line类的声明
 *
 *	@par	历史:
 *		2013/08/14	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_CurveEntity_H_
#define _Stk_CurveEntity_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_File.h"
#include "Stk_Entity.h"
#include <string>
#include <vector>

/**************************************************************************************
	 *	@class	Stk_Text
	 *	@brief	线条基类
	 *
	 *	PMI各线条类型的基础类。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_CurveEntity :
	public Stk_Entity
{
private:
	STK_RGBA32				m_rgbaCurve;		//!< 颜色
protected:
	StkCurveTypeEnum		m_eCurveType;		//!< 线条类型
	StkCurveUsageEnum		m_eUsageType;		//!< 线条的应用类型
	STK_BOOL				m_bDispStat;		//!< 线条是否隐藏
	StkCurveDispTypeEnum	m_eDispType;		//!< 线条的显示类型（点线、连线等）
public:
	/****************************************************************************
	 *	@brief		Stk_CurveEntity构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_CurveEntity(void);

	/****************************************************************************
	 *	@brief		Stk_CurveEntity析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	virtual ~Stk_CurveEntity(void);

	/****************************************************************************
	 *	@brief		注册Curve的ID
	 *	@param[in]	i_ID				Curve的ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_ERROR			注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取Curve的ID
	 *	@param[in]	无
	 *	@retval		m_ID			Curve的ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetID();

	/****************************************************************************
	 *	@brief		设置Curve的ID
	 *	@param[in]	i_ID				Curve的ID
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取Curve的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eCurveType	Curve的类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkCurveTypeEnum			GetCurveType();
	//STK_STATUS				SetCurveType(STK_UINT32 inType); // NOTE：在子类的构造函数中自动设置

	/****************************************************************************
	 *	@brief		获取Curve的应用类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eUsageType		Curve的定义场景类型
	 *	@note		用于区分Curve的定义场景类型
	 *	@attention	无
	 ****************************************************************************/
	StkCurveUsageEnum			GetUsageType();

	/****************************************************************************
	 *	@brief		设置Curve的定义场景类型
	 *	@param[in]	i_nuDefType		Curve的定义场景类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetUsageType(StkCurveUsageEnum i_eUsageType); 

	/****************************************************************************
	 *	@brief		获取Curve的颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaCurve			颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		设置Curve的颜色
	 *	@param[in]	i_rgbaCurve			Curve的颜色
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetColor(const STK_RGBA32& i_rgbaCurve);

	/*****************************************************************************
	 *	@brief		设置线条是否隐藏
	 *	@param[in]	i_bDispStat			线条是否隐藏
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCurveDispStat(STK_BOOL i_bDispStat);

	/*****************************************************************************
	 *	@brief		获取线条的显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bDispStat
	 *				= TRUE		显示
	 *				= FALSE		隐藏
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					GetCurveDispStat();

	/*****************************************************************************
	 *	@brief		设置线条的显示类型
	 *	@param[in]	i_eDispType				线条显示类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCurveDispType(StkCurveDispTypeEnum i_eDispType);

	/*****************************************************************************
	 *	@brief		获取线条的显示类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuDispType			线条显示类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkCurveDispTypeEnum		GetCurveDispType();
};

/**************************************************************************************
	 *	@class	Stk_Text
	 *	@brief	折线类
	 *
	 *	记录PMI中类型为折线的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/

class DATAMANAGER_API Stk_PolyLine :
	public Stk_CurveEntity
{
private:
	STK_FLOAT32					m_fUMin;		//!< 起始范围
	STK_FLOAT32					m_fUMax;		//!< 终止范围
	vector<STK_PNT32>			m_pntPoints;	//!< 点列
public:
	/****************************************************************************
	 *	@brief		Stk_PolyLine构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PolyLine(void);

	/****************************************************************************
	 *	@brief		Stk_PolyLine构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PolyLine(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_PolyLine析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_PolyLine(void);

	/****************************************************************************
	 *	@brief		获取折线点列信息
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntPoints			折线点列
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<STK_PNT32>&	GetPoints();

	/****************************************************************************
	 *	@brief		设置折线点列信息
	 *	@param[in]	i_pntPoints			折线点列
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetPoints(const vector<STK_PNT32>& i_pntPoints);

	/****************************************************************************
	 *	@brief		获取折线起始终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fUMin				起始范围
	 *	@param[out]	o_fUMax				终止范围
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

	/****************************************************************************
	 *	@brief		设置折线起始终止范围
	 *	@param[in]	i_fUMin				起始范围
	 *	@param[in]	i_fUMax				终止范围
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDomain(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);

	/*****************************************************************************
	 *	@brief		写入折线数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WritePolyline(StkSegmentHead* i_head, Stk_File* i_stkFileP);
	
	/****************************************************************************
	 *	@brief		写入顶点数据
	 *	@param[in]	fatrP			顶点存储地址
	 *	@param[in]	pnt				顶点数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteVertex(STK_FLOAT32* fatrP, const STK_PNT32& pnt);
};

/**************************************************************************************
	 *	@class	Stk_Text
	 *	@brief	弧线类
	 *
	 *	记录PMI中类型为弧线的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/

class DATAMANAGER_API Stk_Ellipse :
	public Stk_CurveEntity
{
private:
	STK_FLOAT32				m_fUMin;			//!< 起始范围
	STK_FLOAT32				m_fUMax;			//!< 终止范围
	STK_PNT32				m_pntCenter;		//!< 中心点
	STK_DIR32				m_dirX;				//!< X方向向量
	STK_DIR32				m_dirY;				//!< Y方向向量
	STK_DIR32				m_dirZ;				//!< Z方向向量
	STK_FLOAT32				m_fMajorRadius;		//!< 长半径
	STK_FLOAT32				m_fMinorRadius;		//!< 短半径
	STK_PNT32				m_pntStart;			//!< 起点坐标
	STK_PNT32				m_pntEnd;			//!< 终点坐标
	STK_FLOAT32				m_fStartPar;		//!< 起点参数位置
	STK_FLOAT32				m_fEndPar;			//!< 终点参数位置
	STK_DIR32				m_dirNormal;		//!< 圆弧方向
	STK_DIR32				m_dirOrigin;		//!< 原点方向

public:
	/****************************************************************************
	 *	@brief		Stk_Ellipse构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Ellipse(void);

	/****************************************************************************
	 *	@brief		Stk_Ellipse构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Ellipse(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_Ellipse析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Ellipse(void);

	/****************************************************************************
	 *	@brief		获取弧线中心点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntCenter			中心点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetCenterPoint();

	/****************************************************************************
	 *	@brief		设置弧线中心点
	 *	@param[in]	i_pntCenter			中心点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCenterPoint(const STK_PNT32& i_pntCenter);

	/****************************************************************************
	 *	@brief		获取弧线方向向量
	 *	@param[in]	无
	 *	@param[out]	o_dirX				X方向向量
	 *	@param[out]	o_dirY				Y方向向量
	 *	@param[out]	o_dirZ				Z方向向量
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);
	
	/****************************************************************************
	 *	@brief		设置弧线向量
	 *	@param[in]	i_dirX				X方向向量
	 *	@param[in]	i_dirY				Y方向向量
	 *	@param[in]	i_dirZ				Z方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);

	/****************************************************************************
	 *	@brief		获取弧线半径
	 *	@param[in]	无
	 *	@param[out]	o_fMajorR			长半径
	 *	@param[out]	o_fMinorR			短半径
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					GetRadius(STK_FLOAT32& o_fMajorR, STK_FLOAT32& o_fMinorR);
	
	/****************************************************************************
	 *	@brief		设置弧线半径
	 *	@param[in]	i_fMajorR			长半径
	 *	@param[in]	i_fMinorR			短半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetRadius(STK_FLOAT32 i_fMajorR, STK_FLOAT32 i_fMinorR);

	/****************************************************************************
	 *	@brief		设置圆弧方向
	 *	@param[in]	i_dirNormal	圆弧方向
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetNormal(const STK_DIR32& i_dirNormal);
	
	/****************************************************************************
	 *	@brief		获取圆弧方向
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirNormal	圆弧方向
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32				GetNormal();

	/****************************************************************************
	 *	@brief		设置原点方向
	 *	@param[in]	i_dirOrigin	原点方向
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetOriginDir(const STK_DIR32& i_dirOrigin);
	
	/****************************************************************************
	 *	@brief		获取原点方向
	 *	@param[in]	无	
	 *	@param[out]	无
	 *	@retval		m_dirOrigin	原点方向
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32				GetOriginDir();

	/****************************************************************************
	 *	@brief		获取弧线起始终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fUMin				起始范围
	 *	@param[out]	o_fUMax				终止范围
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);
	
	/****************************************************************************
	 *	@brief		设置弧线起始终止范围
	 *	@param[in]	i_fUMin				起始范围
	 *	@param[in]	i_fUMax				终止范围
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDomain(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);

	/****************************************************************************
	 *	@brief		获取弧线点坐标
	 *	@param[in]	无
	 *	@param[out]	o_pntStart			起点坐标
	 *	@param[out]	o_pntEnd			终点坐标
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					GetCoordinatePnt(STK_PNT32& o_pntStart, STK_PNT32& o_pntEnd);
	
	/****************************************************************************
	 *	@brief		设置弧线点坐标
	 *	@param[in]	i_pntStart			起点坐标
	 *	@param[in]	i_pntEnd			终点坐标
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCoordiantePnt(const STK_PNT32& i_pntStart, const STK_PNT32& i_pntEnd);

	/****************************************************************************
	 *	@brief		获取弧线点参数位置
	 *	@param[in]	无
	 *	@param[out]	o_fStartPar			起点参数位置
	 *	@param[out]	o_fEndPar			终点参数位置
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					GetParameter(STK_FLOAT32& o_fStartPar, STK_FLOAT32& o_fEndPar);
	
	/****************************************************************************
	 *	@brief		设置弧线点参数位置
	 *	@param[in]	i_fStartPar			起点参数位置
	 *	@param[in]	i_fEndPar			终点参数位置
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetParameter(STK_FLOAT32 i_fStartPar, STK_FLOAT32 i_fEndPar);

	/*****************************************************************************
	 *	@brief		写入椭圆弧数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteEllipse(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_Text
	 *	@brief	两点线类
	 *
	 *	记录PMI中由两点确定一条线的类的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/

class DATAMANAGER_API Stk_Line : public Stk_CurveEntity
{
private:
	STK_PNT32				m_pntStart;		//!< 起始点
	STK_PNT32				m_pntEnd;		//!< 终止点

public:
	/****************************************************************************
	 *	@brief		Stk_Line构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Line(void);

	/****************************************************************************
	 *	@brief		Stk_Line构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Line(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_Line析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Line(void);

	/****************************************************************************
	 *	@brief		获取起始点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntStart	起始点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetStartPoint();
	
	/****************************************************************************
	 *	@brief		获取终止点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntEnd	终止点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetEndPoint();

	/****************************************************************************
	 *	@brief		设置起始点
	 *	@param[in]	i_pntStart	起始点
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetStartPoint(const STK_PNT32& i_pntStart);

	/****************************************************************************
	 *	@brief		设置终止点
	 *	@param[in]	i_pntEnd	终止点
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetEndPoint(const STK_PNT32& i_pntEnd);

	/*****************************************************************************
	 *	@brief		写入直线线数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteLine(StkSegmentHead* i_head, Stk_File* i_stkFileP);

};

/****************************************************************************************
	 *	@class	Stk_NurbsCurve
	 *	@brief	NURBS曲线类
	 *
	 *	记录NURBS曲线的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/
class DATAMANAGER_API Stk_NurbsCurve :
	public Stk_CurveEntity
{
private:
	STK_FLOAT32				m_fUMin;			//!< 起始范围
	STK_FLOAT32				m_fUMax;			//!< 终止范围
	STK_UINT32				m_nuDegree;			//!< degree值
	vector<STK_UINT32>		m_vcKnotPlicity;	//!< 多样性
	vector<STK_FLOAT32>		m_vcKnots;			//!< Knots节点
	vector<STK_PNT32>		m_vcPoints;			//!< 控制点
	vector<STK_FLOAT32>		m_vcWeights;		//!< 权重
public:
	/*****************************************************************************
	 *	@brief		Stk_NurbsCurve构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_NurbsCurve(void);

	/****************************************************************************
	 *	@brief		Stk_NurbsCurve构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_NurbsCurve(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		Stk_NurbsCurve析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_NurbsCurve(void);

	/*****************************************************************************
	 *	@brief		获取起始和终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fMin					起始范围
	 *	@param[out]	o_fMax					终止范围
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);
	
	/*****************************************************************************
	 *	@brief		设置起始和终止范围
	 *	@param[in]	i_fMin					起始范围
	 *	@param[in]	i_fMax					终止范围
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

	/*****************************************************************************
	 *	@brief		获取Degree
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuDegree				Degree值
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetDegree();

	/*****************************************************************************
	 *	@brief		设置Degree
	 *	@param[in]	i_nuDegree				Degree参数
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDegree(STK_UINT32 i_nuDegree);

	/*****************************************************************************
	 *	@brief		获取Knot的多样性
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vecKnotPlicity		Knot的多样性
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<STK_UINT32>		GetKnotMultiplicity();

	/*****************************************************************************
	 *	@brief		添加Knot的多样性
	 *	@param[in]	i_nuKnotPlicity			Knot的多样性
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddKnotMultiplicity(STK_UINT32 i_nuKnotPlicity);

	/*****************************************************************************
	 *	@brief		获取Knot节点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vecKnots				Knots节点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<STK_FLOAT32>		GetKnots();

	/*****************************************************************************
	 *	@brief		添加Knot节点
	 *	@param[in]	i_fKnot					Knot节点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddKnot(STK_FLOAT32 i_fKnot);

	/*****************************************************************************
	 *	@brief		获取控制点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vecPoints				Nurbs曲线的控制点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<STK_PNT32>		GetPoints();

	/*****************************************************************************
	 *	@brief		添加Nurbs曲线控制点
	 *	@param[in]	i_pntCtrl				Nurbs曲线控制点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddPoint(STK_PNT32 i_pntCtrl);

	/*****************************************************************************
	 *	@brief		获取Nurbs曲线控制点的权重
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vecWeights				Nurbs曲线控制点的权重
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<STK_FLOAT32>		GetWeights();

	/*****************************************************************************
	 *	@brief		添加Nurbs曲线控制点的权重
	 *	@param[in]	i_fWeight					Nurbs曲线控制点的权重
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddWeight(STK_FLOAT32 i_fWeight);

	/*****************************************************************************
	 *	@brief		填写Nurbs曲线控制点
	 *	@param[in]	i_Point					控制点
	 *	@param[out]	o_fAtr					占位符位置
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteVertex(STK_FLOAT32* o_fAtr, STK_PNT32 i_Point);

	/*****************************************************************************
	 *	@brief		将Nurbs曲线数据写入SVL文件
	 *	@param[in]	i_head					记录头
	 *	@param[in]	i_stkFileP				文件对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteNurbsCurve(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

/****************************************************************************************
	 *	@class	Stk_Hyperbola
	 *	@brief	双曲线类
	 *
	 *	记录双曲线的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/
class DATAMANAGER_API Stk_Hyperbola :
	public Stk_CurveEntity
{
private:
	STK_FLOAT32				m_fUMin;			//!< 起始范围
	STK_FLOAT32				m_fUMax;			//!< 终止范围
	STK_PNT32				m_pntCenter;		//!< 中心点
	STK_DIR32				m_dirX;				//!< X方向向量
	STK_DIR32				m_dirY;				//!< Y方向向量
	STK_DIR32				m_dirZ;				//!< Z方向向量
	STK_FLOAT32				m_fSemiAxis;		//!< 半轴
	STK_FLOAT32				m_fSemiImgAxis;
	STK_PNT32				m_pntStart;			//!< 起点坐标
	STK_PNT32				m_pntEnd;			//!< 终点坐标
public:
	/*****************************************************************************
	 *	@brief		Stk_Hyperbola构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Hyperbola(void);

	/****************************************************************************
	 *	@brief		Stk_Hyperbola构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Hyperbola(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		Stk_Hyperbola析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Hyperbola(void);

	/*****************************************************************************
	 *	@brief		获取起始和终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fMin					起始范围
	 *	@param[out]	o_fMax					终止范围
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

	/*****************************************************************************
	 *	@brief		设置起始和终止范围
	 *	@param[in]	i_fMin					起始范围
	 *	@param[in]	i_fMax					终止范围
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

	/*****************************************************************************
	 *	@brief		获取双曲线的中心点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntCenter				双曲线中心点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetCenterPoint();

	/*****************************************************************************
	 *	@brief		设置双曲线的中心点
	 *	@param[in]	i_pntCenter				双曲线的中心点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

	/*****************************************************************************
	 *	@brief		获取双曲线的XYZ方向向量
	 *	@param[in]	无
	 *	@param[out]	o_dirX					X方向向量
	 *	@param[out]	o_dirY					Y方向向量
	 *	@param[out]	o_dirZ					Z方向向量
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);
	
	/*****************************************************************************
	 *	@brief		设置双曲线的XYZ方向向量
	 *	@param[in]	i_dirX					X方向向量
	 *	@param[in]	i_dirY					Y方向向量
	 *	@param[in]	i_dirZ					Z方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

	/*****************************************************************************
	 *	@brief		获取双曲线的半轴
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fSemiAxis				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetSemiAxis();

	/*****************************************************************************
	 *	@brief		设置双曲线的半轴
	 *	@param[in]	i_fSemiAxis				双曲线的半轴
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetSemiAxis(STK_FLOAT32 i_fSemiAxis);

	/*****************************************************************************
	 *	@brief		获取双曲线的SemiImgAxis
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fSemiAxis				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetSemiImgAxis();

	/*****************************************************************************
	 *	@brief		设置双曲线的SemiImgAxis
	 *	@param[in]	i_fSemiImgAxis			双曲线的SemiImgAxis
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetSemiImgAxis(STK_FLOAT32 i_fSemiImgAxis);

	/*****************************************************************************
	 *	@brief		获取起点坐标
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntStart				起点坐标
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetStartPoint();

	/*****************************************************************************
	 *	@brief		设置起点坐标
	 *	@param[in]	i_pntStart				起点坐标
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);
	
	/*****************************************************************************
	 *	@brief		获取终点坐标
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntEnd				终点坐标
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetEndPoint();

	/*****************************************************************************
	 *	@brief		设置终点坐标
	 *	@param[in]	i_pntEnd				终点坐标
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

	/*****************************************************************************
	 *	@brief		将双曲线数据写入SVL文件
	 *	@param[in]	i_head					记录头
	 *	@param[in]	i_stkFileP				文件指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteHyperbola(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

/****************************************************************************************
	 *	@class	Stk_Parabola
	 *	@brief	抛物线类
	 *
	 *	记录抛物线的数据信息。<br>
	 *	继承自Stk_CurveEntity类。
***************************************************************************************/
class DATAMANAGER_API Stk_Parabola :
	public Stk_CurveEntity
{
private:
	STK_FLOAT32				m_fUMin;			//!< 起始范围
	STK_FLOAT32				m_fUMax;			//!< 终止范围
	STK_PNT32				m_pntStart;			//!< 起点坐标
	STK_PNT32				m_pntEnd;			//!< 终点坐标
	STK_PNT32				m_pntCenter;		//!< 中心点
	STK_DIR32				m_dirX;				//!< X方向向量
	STK_DIR32				m_dirY;				//!< Y方向向量
	STK_DIR32				m_dirZ;				//!< Z方向向量
	STK_FLOAT32				m_fFocalDis;		//!< 焦点距离
public:
	/*****************************************************************************
	 *	@brief		Stk_Parabola构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Parabola(void);

	/****************************************************************************
	 *	@brief		Stk_Parabola构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Parabola(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		Stk_Parabola析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Parabola(void);

	/*****************************************************************************
	 *	@brief		获取起始和终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fMin					起始范围
	 *	@param[out]	o_fMax					终止范围
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

	/*****************************************************************************
	 *	@brief		设置起始和终止范围
	 *	@param[in]	i_fMin					起始范围
	 *	@param[in]	i_fMax					终止范围
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

	/*****************************************************************************
	 *	@brief		获取起点坐标
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntStart				起点坐标
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetStartPoint();

	/*****************************************************************************
	 *	@brief		设置起点坐标
	 *	@param[in]	i_pntStart				起点坐标
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);

	/*****************************************************************************
	 *	@brief		获取终点坐标
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntEnd				终点坐标
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetEndPoint();

	/*****************************************************************************
	 *	@brief		设置终点坐标
	 *	@param[in]	i_pntEnd				终点坐标
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

	/*****************************************************************************
	 *	@brief		获取中心点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntCenter				中心点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetCenterPoint();

	/*****************************************************************************
	 *	@brief		设置中心点
	 *	@param[in]	i_pntCenter				中心点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

	/*****************************************************************************
	 *	@brief		获取抛物线的XYZ方向向量
	 *	@param[in]	无
	 *	@param[out]	o_dirX					X方向向量
	 *	@param[out]	o_dirY					Y方向向量
	 *	@param[out]	o_dirZ					Z方向向量
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);
	
	/*****************************************************************************
	 *	@brief		设置抛物线的XYZ方向向量
	 *	@param[in]	i_dirX					X方向向量
	 *	@param[in]	i_dirY					Y方向向量
	 *	@param[in]	i_dirZ					Z方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

	/*****************************************************************************
	 *	@brief		获取Focal距离
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fFocalDis				Focal距离
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetFocalDistance();

	/*****************************************************************************
	 *	@brief		设置Focal距离
	 *	@param[in]	i_fDistance				Focal距离
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFocalDistance(STK_FLOAT32 i_fDistance);

	/*****************************************************************************
	 *	@brief		将抛物线数据写入SVL文件
	 *	@param[in]	i_head					记录头
	 *	@param[in]	i_stkFileP				文件指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteParabola(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

#endif //_Stk_CurveEntity_H_
