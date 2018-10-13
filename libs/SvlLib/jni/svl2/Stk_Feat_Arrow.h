// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat_Arrow.h
 *
 *	@brief	箭头特征类
 *
 *	@par	历史:
 *		2015/07/31	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_FEAT_ARROW_ENTITY_H_
#define _STK_FEAT_ARROW_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_Enum.h"
#include "Stk_File.h"

/****************************************************************************
	 *	@class	Stk_Feat_Arrow
	 *	@brief	箭头特征类
	 *
	 *	创建用于存储箭头特征的类库。<br>
	 *	继承自Stk_Entity 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_Arrow :
	public Stk_Entity
{
private:
	wstring					m_wstrName;						//!< 箭头名称
	StkArrowTypeEnum		m_eType;						//!< 箭头类型：直线、直角、圆形
	STK_FLOAT32				m_fAlpha;						//!< 箭头透明度
	STK_BOOL				m_bIsOnTop;						//!< 箭头是否总在最前端显示
	StkActorPositionEnum	m_eActorPosition;				//!< 灯头位置
	STK_FLOAT32				m_fActorForm;					//!< 灯头形状
	STK_FLOAT32				m_fActorLength;					//!< 灯头长度
	STK_FLOAT32				m_fActorWidth;					//!< 灯头宽度
	STK_FLOAT32				m_fWidth;						//!< 箭头宽度
	STK_FLOAT32				m_fGradient;					//!< 箭头倾斜度
	STK_RGBA32				m_rgbaBackground;				//!< 箭头背景色
	STK_BOOL				m_bIsGradualChange;				//!< 箭头背景色是否渐变
	STK_BOOL				m_bIsDisplayBorder;				//!< 是否显示边界
	STK_FLOAT32				m_fBorderWidth;					//!< 边界宽度
	STK_RGBA32				m_rgbaBorder;					//!< 边界颜色
	StkCurveDispTypeEnum	m_eLineType;					//!< 边界线类型
	STK_MTX32				m_mtxDefPlane;					//!< 定义平面
protected:
	/*****************************************************************************
	 *	@brief		写箭头特征数据
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
	 *	@brief		箭头特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Arrow();
	
	/****************************************************************************
	 *	@brief		箭头特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_Arrow();
public:
	/****************************************************************************
	 *	@brief		设置箭头名称
	 *	@param[in]	i_wstrName		箭头名称
	 *	@retval		无
	 *	@note		STK_SUCCESS		设置成功
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetName(wstring i_wstrName);
	
	/****************************************************************************
	 *	@brief		获取箭头名称
	 *	@param[in]	无
	 *	@retval		箭头名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetName();

	/****************************************************************************
	 *	@brief		设置箭头类型
	 *	@param[in]	i_eType			箭头类型
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetType(StkArrowTypeEnum i_eType);
	
	/****************************************************************************
	 *	@brief		获取箭头类型
	 *	@param[in]	无
	 *	@retval		箭头类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkArrowTypeEnum		GetType();

	/****************************************************************************
	 *	@brief		设置箭头的不透明性
	 *	@param[in]	i_fAlpha		不透明性
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetOpacity(STK_FLOAT32 i_fAlpha);
	
	/****************************************************************************
	 *	@brief		获取箭头的不透明性
	 *	@param[in]	i_fAlpha		不透明性
	 *	@retval		不透明性
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetOpacity();

	/****************************************************************************
	 *	@brief		设置箭头是否总在最前端
	 *	@param[in]	i_bIsOnTop		是否总在最前端
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetOnTop(STK_BOOL i_bIsOnTop);
	
	/****************************************************************************
	 *	@brief		箭头是否总在最前端
	 *	@param[in]	无
	 *	@retval		是否总在最前端
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsOnTop();

	/****************************************************************************
	 *	@brief		设置灯头位置
	 *	@param[in]	i_eActorPosition	灯头位置
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActorPosition(StkActorPositionEnum i_eActorPosition);
	
	/****************************************************************************
	 *	@brief		获取灯头位置
	 *	@param[in]	无
	 *	@retval		灯头位置
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkActorPositionEnum	GetActorPosition();

	/****************************************************************************
	 *	@brief		设置灯头形状
	 *	@param[in]	i_fActorForm		灯头形状
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActorForm(STK_FLOAT32 i_fActorForm);

	/****************************************************************************
	 *	@brief		获取灯头形状
	 *	@param[in]	无
	 *	@retval		灯头形状
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetActorForm();

	/****************************************************************************
	 *	@brief		设置灯头长度
	 *	@param[in]	i_fActorLength		灯头长度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActorLength(STK_FLOAT32 i_fActorLength);

	/****************************************************************************
	 *	@brief		获取灯头长度
	 *	@param[in]	无
	 *	@retval		灯头长度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetActorLength();

	/****************************************************************************
	 *	@brief		设置灯头宽度
	 *	@param[in]	i_fActorWidth		灯头宽度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActorWidth(STK_FLOAT32 i_fActorWidth);

	/****************************************************************************
	 *	@brief		获取灯头宽度
	 *	@param[in]	无
	 *	@retval		灯头宽度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetActorWidth();

	/****************************************************************************
	 *	@brief		设置箭头宽度
	 *	@param[in]	i_fArrowWidth		箭头宽度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetArrowWidth(STK_FLOAT32 i_fArrowWidth);

	/****************************************************************************
	 *	@brief		获取箭头宽度
	 *	@param[in]	无
	 *	@retval		箭头宽度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetArrowWidth();

	/****************************************************************************
	 *	@brief		设置箭头倾斜度
	 *	@param[in]	i_fArrowGradient	箭头倾斜度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetArrowGradient(STK_FLOAT32 i_fArrowGradient);

	/****************************************************************************
	 *	@brief		获取箭头倾斜度
	 *	@param[in]	无
	 *	@retval		箭头倾斜度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetArrowGradient();

	/****************************************************************************
	 *	@brief		设置箭头背景色
	 *	@param[in]	i_rgbaBackground	箭头背景色
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBackgroundColor(STK_RGBA32 i_rgbaBackground);

	/****************************************************************************
	 *	@brief		获取箭头背景色
	 *	@param[in]	无
	 *	@retval		箭头背景色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32				GetBackgroundColor();

	/****************************************************************************
	 *	@brief		设置箭头背景色是否渐变
	 *	@param[in]	i_bBackgroundColorChange	箭头背景色是否渐变
	 *	@retval		STK_SUCCESS					设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBackgroundColorChange(STK_BOOL i_bBackgroundColorChange);

	/****************************************************************************
	 *	@brief		箭头背景色是否渐变
	 *	@param[in]	无
	 *	@retval		箭头背景色是否渐变
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsBackgroundColorChange();

	/****************************************************************************
	 *	@brief		设置边框是否显示
	 *	@param[in]	i_bIsBorderDisplay	边框是否显示
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBorderDisplay(STK_BOOL i_bIsBorderDisplay);

	/****************************************************************************
	 *	@brief		边框是否显示
	 *	@param[in]	无
	 *	@retval		边框是否显示
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsBorderDisplay();

	/****************************************************************************
	 *	@brief		设置边框宽度
	 *	@param[in]	i_fBorderWidth		边框宽度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBorderWidth(STK_FLOAT32 i_fBorderWidth);

	/****************************************************************************
	 *	@brief		获取边框宽度
	 *	@param[in]	无
	 *	@retval		边框宽度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32				GetBorderWidth();

	/****************************************************************************
	 *	@brief		设置边框颜色
	 *	@param[in]	i_rgbaBorder		边框颜色
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBorderColor(STK_RGBA32 i_rgbaBorder);

	/****************************************************************************
	 *	@brief		获取边框颜色
	 *	@param[in]	无
	 *	@retval		边框颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32				GetBorderColor();

	/****************************************************************************
	 *	@brief		设置边框线类型
	 *	@param[in]	i_eBorderType		边框线类型
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBorderType(StkCurveDispTypeEnum i_eBorderType);

	/****************************************************************************
	 *	@brief		获取边框线类型
	 *	@param[in]	无
	 *	@retval		边框线类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkCurveDispTypeEnum	GetBorderType();

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
	 *	@brief		设置定义平面
	 *	@param[in]	i_mtxDefPlane		定义平面
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDefPlane(const STK_MTX32& i_mtxDefPlane);
};

/****************************************************************************
	 *	@class	Stk_Feat_LineArrow
	 *	@brief	直线箭头特征类
	 *
	 *	创建用于存储箭头特征的类库。<br>
	 *	继承自Stk_Feat_Arrow 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_LineArrow :
	public Stk_Feat_Arrow
{
private:
	STK_PNT32			m_pntAnchor1;			//!< 锚点1
	STK_PNT32			m_pntAnchor2;			//!< 锚点2
	STK_DIR32			m_dirX1;				//!< 锚点1的X方向向量
	STK_DIR32			m_dirY1;				//!< 锚点1的Y方向向量
	STK_DIR32			m_dirX2;				//!< 锚点2的X方向向量
	STK_DIR32			m_dirY2;				//!< 锚点2的Y方向向量
public:
	/****************************************************************************
	 *	@brief		直线箭头特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_LineArrow();
	
	/****************************************************************************
	 *	@brief		直线箭头特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_LineArrow();
public:
	/****************************************************************************
	 *	@brief		设置直线箭头的第一个锚点
	 *	@param[in]	i_pntAnchor1		锚点1
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetFirstAnchorPoint(STK_PNT32 i_pntAnchor1);

	/****************************************************************************
	 *	@brief		获取直线箭头的第一个锚点
	 *	@param[in]	无
	 *	@retval		锚点1
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetFirstAnchorPoint();

	/****************************************************************************
	 *	@brief		设置直线箭头的第二个锚点
	 *	@param[in]	i_pntAnchor2		锚点2
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetSecondAnchorPoint(STK_PNT32 i_pntAnchor2);

	/****************************************************************************
	 *	@brief		获取直线箭头的第二个锚点
	 *	@param[in]	无
	 *	@retval		锚点2
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetSecondAnchorPoint();

	/****************************************************************************
	 *	@brief		设置锚点1的X方向向量
	 *	@param[in]	i_dirX				X方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetFirstAnchorPointDirX(STK_DIR32 i_dirX);

	/****************************************************************************
	 *	@brief		获取锚点1的X方向向量
	 *	@param[in]	无
	 *	@retval		锚点1的X方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetFirstAnchorPointDirX();

	/****************************************************************************
	 *	@brief		设置锚点1的Y方向向量
	 *	@param[in]	i_dirY				Y方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetFirstAnchorPointDirY(STK_DIR32 i_dirY);

	/****************************************************************************
	 *	@brief		获取锚点1的Y方向向量
	 *	@param[in]	无
	 *	@retval		锚点1的Y方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetFirstAnchorPointDirY();

	/****************************************************************************
	 *	@brief		设置锚点2的X方向向量
	 *	@param[in]	i_dirX				X方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetSecondAnchorPointDirX(STK_DIR32 i_dirX);

	/****************************************************************************
	 *	@brief		获取锚点2的X方向向量
	 *	@param[in]	无
	 *	@retval		锚点2的X方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetSecondAnchorPointDirX();

	/****************************************************************************
	 *	@brief		设置锚点2的Y方向向量
	 *	@param[in]	i_dirY				Y方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetSecondAnchorPointDirY(STK_DIR32 i_dirY);

	/****************************************************************************
	 *	@brief		获取锚点2的Y方向向量
	 *	@param[in]	无
	 *	@retval		锚点1的Y方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetSecondAnchorPointDirY();

	/****************************************************************************
	 *	@brief		读取文件中的直线箭头特征数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_LineArrowHeadP	直线箭头记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_LineArrowHeadP);
	
	/*****************************************************************************
	 *	@brief		写直线箭头特征数据
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
	 *	@class	Stk_Feat_CircularArrow
	 *	@brief	圆形箭头特征类
	 *
	 *	创建用于存储箭头特征的类库。<br>
	 *	继承自Stk_Feat_Arrow 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_CircularArrow :
	public Stk_Feat_Arrow
{
private:
	STK_BOOL			m_bIsDisplayAxis;		//!< 是否显示轴
	STK_BOOL			m_bIsPlaneRender;		//!< 平面是否渲染
	STK_FLOAT32			m_fRadius;				//!< 半径
	STK_PNT32			m_pntOrigin;			//!< 圆心
	STK_DIR32			m_dirX;					//!< X方向向量
	STK_DIR32			m_dirY;					//!< Y方向向量
	STK_UINT32			m_nuStep;				//!< 进程
	STK_FLOAT32			m_fAngle;				//!< 圆角开口角度
public:
	/****************************************************************************
	 *	@brief		圆形箭头特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_CircularArrow();
	
	/****************************************************************************
	 *	@brief		圆形箭头特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_CircularArrow();
public:
	/****************************************************************************
	 *	@brief		设置是否显示圆形箭头的轴
	 *	@param[in]	i_bIsDisplayAxis	是否显示轴
	 *	@retval		无
	 *	@note		STK_SUCCESS			设置成功
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDisplayAxis(STK_BOOL i_bIsDisplayAxis);

	/****************************************************************************
	 *	@brief		是否显示圆形箭头的轴
	 *	@param[in]	无
	 *	@retval		是否显示轴
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL			IsDisplayAxis();

	/****************************************************************************
	 *	@brief		设置是否显示平面渲染
	 *	@param[in]	i_bIsPlaneRender	是否显示平面渲染
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetPlaneRender(STK_BOOL i_bIsPlaneRender);

	/****************************************************************************
	 *	@brief		是否显示平面渲染
	 *	@param[in]	无
	 *	@retval		是否显示平面渲染
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL			IsPlaneRender();

	/****************************************************************************
	 *	@brief		设置半径
	 *	@param[in]	i_fRadius			半径
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetRadius(STK_FLOAT32 i_fRadius);

	/****************************************************************************
	 *	@brief		获取半径
	 *	@param[in]	无
	 *	@retval		半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32			GetRadius();

	/****************************************************************************
	 *	@brief		设置圆心
	 *	@param[in]	i_pntOrigin			圆心
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetOrigin(STK_PNT32 i_pntOrigin);

	/****************************************************************************
	 *	@brief		获取圆心
	 *	@param[in]	无
	 *	@retval		圆心
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetOrigin();

	/****************************************************************************
	 *	@brief		设置圆心的X方向向量
	 *	@param[in]	i_dirX				圆心的X方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDirX(STK_DIR32 i_dirX);

	/****************************************************************************
	 *	@brief		获取圆心的X方向向量
	 *	@param[in]	无
	 *	@retval		圆心的X方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetDirX();

	/****************************************************************************
	 *	@brief		设置圆心的Y方向向量
	 *	@param[in]	i_dirY				圆心的Y方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDirY(STK_DIR32 i_dirY);

	/****************************************************************************
	 *	@brief		获取圆心的Y方向向量
	 *	@param[in]	无
	 *	@retval		圆心的Y方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetDirY();

	/****************************************************************************
	 *	@brief		设置进程
	 *	@param[in]	i_nuStep			进程
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetStep(STK_UINT32 i_nuStep);

	/****************************************************************************
	 *	@brief		获取进程
	 *	@param[in]	无
	 *	@retval		进程
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32			GetStep();

	/****************************************************************************
	 *	@brief		设置角度
	 *	@param[in]	i_fAngle			角度
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetAngle(STK_FLOAT32 i_fAngle);

	/****************************************************************************
	 *	@brief		获取角度
	 *	@param[in]	无
	 *	@retval		角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32			GetAngle();

	/****************************************************************************
	 *	@brief		读取文件中的圆形箭头特征数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_CircularArrowHeadP直线箭头记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_CircularArrowHeadP);
	
	/*****************************************************************************
	 *	@brief		写圆形箭头特征数据
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
	 *	@class	Stk_Feat_RightAngleArrow
	 *	@brief	直角箭头特征类
	 *
	 *	创建用于存储箭头特征的类库。<br>
	 *	继承自Stk_Feat_Arrow 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_RightAngleArrow :
	public Stk_Feat_Arrow
{
private:
	STK_FLOAT32			m_fFirstLength;			//!< 直角箭头的长度1
	STK_FLOAT32			m_fSecondLength;		//!< 直角箭头的长度2
	STK_PNT32			m_pntOrigin;			//!< 直角箭头的原点
	STK_DIR32			m_dirX;					//!< 直角箭头的X方向向量
	STK_DIR32			m_dirY;					//!< 直角箭头的Y方向向量
public:
	/****************************************************************************
	 *	@brief		直角箭头特征构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_RightAngleArrow();
	
	/****************************************************************************
	 *	@brief		直角箭头特征析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_RightAngleArrow();
public:
	/****************************************************************************
	 *	@brief		设置直角箭头的长度1
	 *	@param[in]	i_fFirstLength		直角箭头长度1
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetFirstLength(STK_FLOAT32 i_fFirstLength);

	/****************************************************************************
	 *	@brief		获取直角箭头的长度1
	 *	@param[in]	i_fFirstLength		直角箭头长度1
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32			GetFirstLength();

	/****************************************************************************
	 *	@brief		设置直角箭头的长度2
	 *	@param[in]	i_fSecondLength		直角箭头长度2
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetSecondLength(STK_FLOAT32 i_fSecondLength);

	/****************************************************************************
	 *	@brief		获取直角箭头的长度2
	 *	@param[in]	i_fSecondLength		直角箭头长度2
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32			GetSecondLength();

	/****************************************************************************
	 *	@brief		设置直角箭头的原点
	 *	@param[in]	i_pntOrigin			直角箭头原点
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetOrigin(STK_PNT32 i_pntOrigin);

	/****************************************************************************
	 *	@brief		获取直角箭头的原点
	 *	@param[in]	i_fSecondLength		直角箭头原点
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetOrigin();

	/****************************************************************************
	 *	@brief		设置直角的X方向向量
	 *	@param[in]	i_dirX				圆心的X方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDirX(STK_DIR32 i_dirX);

	/****************************************************************************
	 *	@brief		获取直角的X方向向量
	 *	@param[in]	无
	 *	@retval		圆心的X方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetDirX();

	/****************************************************************************
	 *	@brief		设置直角的Y方向向量
	 *	@param[in]	i_dirY				圆心的Y方向向量
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetDirY(STK_DIR32 i_dirY);

	/****************************************************************************
	 *	@brief		获取直角的Y方向向量
	 *	@param[in]	无
	 *	@retval		圆心的Y方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetDirY();

	/****************************************************************************
	 *	@brief		读取文件中的直角箭头特征数据
	 *	@param[in]	i_FileP					文件对象指针
	 *	@param[in]	i_RightAngleArrowHeadP	直角箭头记录指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_RightAngleArrowHeadP);
	
	/*****************************************************************************
	 *	@brief		写直角箭头特征数据
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