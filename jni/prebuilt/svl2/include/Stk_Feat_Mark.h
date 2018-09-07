// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat_Mark.h
 *
 *	@brief	标记特征类
 *
 *	@par	历史:
 *		2016/02/15	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_FEAT_MARK_ENTITY_H_
#define _STK_FEAT_MARK_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_Enum.h"
#include "Stk_File.h"

/****************************************************************************
	 *	@class	Stk_Feat_Mark
	 *	@brief	标记特征类
	 *
	 *	创建用于存储标记特征的类库。<br>
	 *	继承自Stk_Entity 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_Mark :
	public Stk_Entity
{
private:
	STK_RGBA32				m_rgbaMark;			//!< 标记颜色
	StkMarkTypeEnum			m_eType;			//!< 标记类型
	STK_MTX32				m_mtxDefPlane;		//!< 定义平面
	STK_FLOAT32				m_fCurveWidth;		//!< 线宽
	StkCurveDispTypeEnum	m_eCurveType;		//!< 线型
protected:
	/*****************************************************************************
	 *	@brief		写标记特征数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteFile(Stk_File* i_stkFileP, StkSegmentHead* i_HeadP);
public:
	/****************************************************************************
	 *	@brief		标记特征构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Mark();
	
	/****************************************************************************
	 *	@brief		标记特征析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_Mark();
public:
	/****************************************************************************
	 *	@brief		设置标记颜色
	 *	@param[in]	i_rgbaMark		标记颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetColor(STK_RGBA32 i_rgbaMark);
	
	/****************************************************************************
	 *	@brief		获取标记颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		标记颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32				GetColor();

	/****************************************************************************
	 *	@brief		设置标记类型
	 *	@param[in]	i_eType			标记类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetType(StkMarkTypeEnum i_eType);
	
	/****************************************************************************
	 *	@brief		获取标记类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		标记类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkMarkTypeEnum			GetType();

	/****************************************************************************
	 *	@brief		设置定义平面
	 *	@param[in]	i_mtxDefPlane		定义平面
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDefPlane(const STK_MTX32& i_mtxDefPlane);

	/****************************************************************************
	 *	@brief		获取定义平面
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		定义平面
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const STK_MTX32&		GetDefPlane();

	/****************************************************************************
	 *	@brief		设置线的宽度
	 *	@param[in]	i_fCurveWidth	线宽
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCurveWidth(STK_FLOAT32 i_fCurveWidth);
	
	/****************************************************************************
	 *	@brief		获取线的宽度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		线宽
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetCurveWidth();

	/****************************************************************************
	 *	@brief		设置线的类型
	 *	@param[in]	i_eCurveType	线型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCurveType(StkCurveDispTypeEnum i_eCurveType);
	
	/****************************************************************************
	 *	@brief		获取线的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		线型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkCurveDispTypeEnum	GetCurveType();
};

/****************************************************************************
	 *	@class	Stk_Mark_RectAngle
	 *	@brief	矩形标记特征类
	 *
	 *	创建用于存储矩形标记特征的类库。<br>
	 *	继承自Stk_Feat_Mark 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Mark_RectAngle :
	public Stk_Feat_Mark
{
private:
	StkMarkFillStyleEnum		m_eFillStyle;			//!< 填充方式
	STK_PNT32					m_pntLeftTop;			//!< 左上角锚点
	STK_PNT32					m_pntRightBottom;		//!< 右下角锚点
public:
	/****************************************************************************
	 *	@brief		矩形标记特征构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mark_RectAngle();
	
	/****************************************************************************
	 *	@brief		矩形标记特征析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mark_RectAngle();
public:
	/****************************************************************************
	 *	@brief		设置填充方式
	 *	@param[in]	i_eFillStyle	填充方式
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFillStyle(StkMarkFillStyleEnum i_eFillStyle);
	
	/****************************************************************************
	 *	@brief		获取填充方式
	 *	@param[in]	无
	 *	@retval		填充方式
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkMarkFillStyleEnum	GetFillStyle();

	/****************************************************************************
	 *	@brief		设置矩形标记锚点
	 *	@param[in]	i_pntLeftTop		左上角锚点
	 *	@param[in]	i_pntRightBottom	右下角锚点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetAnchorPoint(STK_PNT32& i_pntLeftTop, STK_PNT32& i_pntRightBottom);
	
	/****************************************************************************
	 *	@brief		获取矩形标记锚点
	 *	@param[in]	无
	 *	@param[out]	o_pntLeftTop		左上角锚点
	 *	@param[out]	o_pntRightBottom	右下角锚点
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetAnchorPoint(STK_PNT32& o_pntLeftTop, STK_PNT32& o_pntRightBottom);

	/****************************************************************************
	 *	@brief		读取文件中的矩形标记特征数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_RectAngleHeadP	矩形标记记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_RectAngleHeadP);
	
	/*****************************************************************************
	 *	@brief		写矩形标记特征数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

/****************************************************************************
	 *	@class	Stk_Mark_Ellipse
	 *	@brief	椭圆标记特征类
	 *
	 *	创建用于存储椭圆标记特征的类库。<br>
	 *	继承自Stk_Feat_Mark 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Mark_Ellipse :
	public Stk_Feat_Mark
{
private:
	StkMarkFillStyleEnum		m_eFillStyle;			//!< 填充方式
	STK_PNT32					m_pntCenter;			//!< 中心点
	STK_FLOAT32					m_fMajorRadius;			//!< 长半径
	STK_FLOAT32					m_fMinorRadius;			//!< 短半径
	STK_DIR32					m_dirNormal;			//!< 法线方向
	STK_DIR32					m_dirOrigin;			//!< 原点方向
public:
	/****************************************************************************
	 *	@brief		椭圆标记特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mark_Ellipse();
	
	/****************************************************************************
	 *	@brief		椭圆标记特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mark_Ellipse();
public:
	/****************************************************************************
	 *	@brief		设置填充方式
	 *	@param[in]	i_eFillStyle	填充方式
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFillStyle(StkMarkFillStyleEnum i_eFillStyle);
	
	/****************************************************************************
	 *	@brief		获取填充方式
	 *	@param[in]	无
	 *	@retval		填充方式
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkMarkFillStyleEnum	GetFillStyle();

	/****************************************************************************
	 *	@brief		设置椭圆中心点
	 *	@param[in]	i_pntCenter		椭圆中心点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCenter(STK_PNT32 i_pntCenter);
	
	/****************************************************************************
	 *	@brief		获取椭圆中心点
	 *	@param[in]	无
	 *	@retval		椭圆中心点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetCenter();

	/****************************************************************************
	 *	@brief		设置椭圆长半径
	 *	@param[in]	i_fMajorRadius	椭圆长半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMajorRadius(STK_FLOAT32 i_fMajorRadius);
	
	/****************************************************************************
	 *	@brief		获取椭圆长半径
	 *	@param[in]	无
	 *	@retval		椭圆长半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetMajorRadius();

	/****************************************************************************
	 *	@brief		设置椭圆短半径
	 *	@param[in]	i_fMajorRadius	椭圆短半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMinorRadius(STK_FLOAT32 i_fMinorRadius);
	
	/****************************************************************************
	 *	@brief		获取椭圆短半径
	 *	@param[in]	无
	 *	@retval		椭圆短半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetMinorRadius();

	/****************************************************************************
	 *	@brief		设置椭圆法线方向
	 *	@param[in]	i_dirNormal		椭圆法线方向
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNormal(STK_DIR32 i_dirNormal);
	
	/****************************************************************************
	 *	@brief		获取椭圆法线方向
	 *	@param[in]	无
	 *	@retval		椭圆法线方向
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32				GetNormal();

	/****************************************************************************
	 *	@brief		设置椭圆原点方向
	 *	@param[in]	i_dirOrigin		椭圆原点方向
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetOriginDir(STK_DIR32 i_dirOrigin);
	
	/****************************************************************************
	 *	@brief		获取椭圆原点方向
	 *	@param[in]	无
	 *	@retval		椭圆原点方向
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32				GetOriginDir();

	/****************************************************************************
	 *	@brief		读取文件中的椭圆标记特征数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_MarkEllipseHeadP	椭圆标记记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MarkEllipseHeadP);
	
	/*****************************************************************************
	 *	@brief		写椭圆标记特征数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

/****************************************************************************
	 *	@class	Stk_Mark_FreeHand
	 *	@brief	徒手画标记特征类
	 *
	 *	创建用于存储徒手画标记特征的类库。<br>
	 *	继承自Stk_Feat_Mark 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Mark_FreeHand :
	public Stk_Feat_Mark
{
private:
	STK_FLOAT32				m_fUMin;			//!< 起始范围
	STK_FLOAT32				m_fUMax;			//!< 终止范围
	vector<STK_PNT32>		m_vcPoints;			//!< 点列数据
public:
	/****************************************************************************
	 *	@brief		徒手画标记特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mark_FreeHand();
	
	/****************************************************************************
	 *	@brief		徒手画标记特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mark_FreeHand();
public:
	/****************************************************************************
	 *	@brief		设置徒手画的起始终止范围
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
	 *	@brief		获取徒手画的起始终止范围
	 *	@param[in]	无
	 *	@param[out]	o_fUMin				起始范围
	 *	@param[out]	o_fUMax				终止范围
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

	/****************************************************************************
	 *	@brief		添加徒手画的点列数据
	 *	@param[in]	i_pntFreeHand		点列数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@retval		其它				添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddPoint(STK_PNT32 i_pntFreeHand);
	
	/****************************************************************************
	 *	@brief		获取徒手画的点列数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		徒手画的点列数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetPointsNum();

	/****************************************************************************
	 *	@brief		获取指定索引的点列数据
	 *	@param[in]	i_nuIndex			索引
	 *	@param[out]	无
	 *	@retval		指定索引的点列数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32				GetPointByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		读取文件中的徒手画标记特征数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_MarkFreeHandHeadP	徒手画标记记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MarkFreeHandHeadP);
	
	/*****************************************************************************
	 *	@brief		写徒手画标记特征数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

#endif