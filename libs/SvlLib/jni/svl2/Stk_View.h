// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_View.h
 *
 *	@brief	视图类
 *
 *	@par	历史:
 *		2013/08/10	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_VIEW_ENTITY_H_
#define _Stk_VIEW_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Entity.h"
#include "Stk_File.h"
#include "Stk_Camera.h"
#include "Stk_SurfaceEntity.h"
#include "Stk_NodeConnector.h"
#include "Stk_Node.h"
#include "Stk_InsAttribute.h"

/**************************************************************************************
	 *	@class	Stk_View
	 *	@brief	View视图类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_View :
	public Stk_Entity
{
private:
	Stk_Node*					m_NodeP;					//!< 所属Node
	StkViewUsageEnum			m_eUsageType;				//!< 应用类型
	wstring						m_wstrName;					//!< 视图名称
	STK_BOOL					m_bIsActivated;				//!< 是否活动
	Stk_Camera					*m_CameraP;					//!< 照相机
	STK_BOOL					m_bHasClipPlan;				//!< 是否有剖面
	vector<Stk_NodeConnector*>	m_vcNodeConnectors;			//!< View与PMI的关联
	vector<Stk_SurfaceEntity*>	m_vcClippingPlans;			//!< 多个剖面,add by wy in 2013/9/29
	STK_FLOAT32					m_fTransparency;			//!< 透明度
	map<wstring, Stk_InsAttribute*>	m_mapInsAttribute;		//!< 子实例属性
private:
	/****************************************************************************
	 *	@brief		写ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
	 *	@param[in]	i_wstrInsPlcPath	实例配置路径
	 *	@param[in]	i_stkInsAttrP		实例属性
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention
	 ****************************************************************************/
	STK_STATUS					WriteInsAttribute(wstring i_wstrInsPlcPath, Stk_InsAttribute* i_stkInsAttrP, Stk_File* i_stkFileP);

	/****************************************************************************
	 *	@brief		读ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_InsAttrHeadP		实例属性段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention
	 ****************************************************************************/
	STK_STATUS					ReadInsAttribute(Stk_File* i_stkFileP, StkSegmentHead *i_InsAttrHeadP);
public:
	/****************************************************************************
	 *	@brief		Stk_View构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_View(Stk_ProtoType* i_stkProtoTypeP);
	
	/****************************************************************************
	 *	@brief		Stk_View析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_View(void);

public:
	/****************************************************************************
	 *	@brief		设置视图ID
	 *	@param[in]	i_ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取视图ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID	视图ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();

	/****************************************************************************
	 *	@brief		注册View ID
	 *	@param[in]	i_idView		视图ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idView);
	
	/****************************************************************************
	 *	@brief		读取文件中的Body数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_ViewHeadP			View记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR			读取失败
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ViewHeadP);
	
	/*****************************************************************************
	 *	@brief		写View数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
	
	/****************************************************************************
	 *	@brief		设置视图定义类型
	 *	@param[in]	i_eUsageType	视图定义类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetUsageType(StkViewUsageEnum i_eUsageType);
	
	/****************************************************************************
	 *	@brief		获取视图定义类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eUsageType	视图的应用类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkViewUsageEnum			GetUsageType();

	/****************************************************************************
	 *	@brief		设置视图名
	 *	@param[in]	inName			视图名
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
	 *	@attention	无
	 ****************************************************************************/
	void						SetName(const wstring &inName);
	
	/****************************************************************************
	 *	@brief		获取视图名
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrName		视图名
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetName(void);

	/****************************************************************************
	 *	@brief		改变View名称
	 *	@param[in]	i_strName		View名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		保持从Node到要素方向的名字一致
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ChangeName(const wstring &i_strName);

	/****************************************************************************
	 *	@brief		设置透明度
	 *	@param[in]	i_fTrans		透明度
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetTransparency(STK_FLOAT32 i_fTrans);
	
	/****************************************************************************
	 *	@brief		获取透明度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fTransparency	透明度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32					GetTransparency(void);

	/****************************************************************************
	 *	@brief		设置照相机
	 *	@param[in]	i_CameraP		照相机对象
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetCamera(Stk_Camera* i_CameraP);
	
	/****************************************************************************
	 *	@brief		获取照相机
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_CameraP		照相机对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Camera*					GetCamera(void);

	/****************************************************************************
	 *	@brief		设置剖面
	 *	@param[in]	i_ClippingPlanP	剖面类对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		因为很多CAD的视图中可以有多个剖面，因此建议使用AddClippingPlan替代
	 *	@attention	无
	 ****************************************************************************/
	void						SetClippingPlan(Stk_SurfaceEntity* i_ClippingPlanP);
	
	/****************************************************************************
	 *	@brief		获取剖面对象数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		剖面对象数据指针
	 *	@note		由GetAllClippingPlan替代
	 *	@attention	无
	 ****************************************************************************/
	//Stk_SurfaceEntity*		GetClippingPlan(void);
	
	/****************************************************************************
	 *	@brief		判断是否有剖面
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasClipPlan	是否有剖面标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//STK_BOOL					HasClippingPlan();

	/****************************************************************************
	 *	@brief		添加剖面到数组中
	 *	@param[in]	i_ClippingPlanP		剖面
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						AddClippingPlan(Stk_SurfaceEntity* i_ClippingPlanP);
	
	/****************************************************************************
	 *	@brief		获取所有的剖面
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcClippingPlans	剖面数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_SurfaceEntity*>& GetAllClippingPlan();

	/****************************************************************************
	 *	@brief		设置相机活动状态
	 *	@param[in]	i_bActivated	相机活动状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetActivated(STK_BOOL i_bActivated);
	
	/****************************************************************************
	 *	@brief		判断相机是否活动
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsActivated	相机是否活动
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsActivated();

	/****************************************************************************
	 *	@brief		添加视图与PMI之间的关联类对象数据
	 *	@param[in]	inNodeConnector		视图与PMI之间的关联
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@retval		STK_ERROR			添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddNodeConnector(Stk_NodeConnector* inNodeConnector);
	
	/****************************************************************************
	 *	@brief		获取所有视图与PMI之间的关联类对象数据
	 *	@param[in]	无		
	 *	@param[out]	无
	 *	@retval		m_vcNodeConnectors	所有的视图与PMI之间的关联
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_NodeConnector*>& GetNodeConnector();
	
	/****************************************************************************
	 *	@brief		删除视图与PMI之间的关联
	 *	@param[in]	i_idNode			节点ID号
	 *	@param[in]	i_PointedEnt		被指向要素类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelNodeConnector(STK_UINT32 i_idNode, STK_UINT32& i_PointedEnt);

	/****************************************************************************
	 *	@brief		创建视图与PMI之间的关联
	 *	@param[in]	i_idNode			PMI所属Node的ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		注意是NodeID，而不是PMI ID
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddPMIConnector(STK_ID i_idNode);
	
	/****************************************************************************
	 *	@brief		删除视图与PMI之间的关联
	 *	@param[in]	i_idNode			PMI所属Node的ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败
	 *	@note		注意是NodeID，而不是PMI ID
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelPMIConnector(STK_ID i_idNode);
	
	/*****************************************************************************
	 *	@brief		设置所对应的Node
	 *	@param[in]	i_NodeP				所对应的Node
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetNode(Stk_Node* i_NodeP);

	/*****************************************************************************
	 *	@brief		获取所对应的Node
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所对应的Node
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Node*					GetNode();

	/****************************************************************************
	 *	@brief		添加子实例的显隐状态
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_eDisplayState		显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState);

	/****************************************************************************
	 *	@brief		获取指定配置路径下子实例的显隐状态
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_eDisplayState		子实例的显隐状态
	 *	@retval		子实例的显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState);

	/****************************************************************************
	 *	@brief		添加子实例的颜色
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_rgbaIns			子实例的颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns);

	/****************************************************************************
	 *	@brief		获取指定配置路径下的子实例的颜色
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_rgbaIns			子实例的颜色
	 *	@retval		指定的子实例的颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns);

	/****************************************************************************
	 *	@brief		添加配置路径下子实例的材质
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_matIns			子实例的材质
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns);

	/****************************************************************************
	 *	@brief		获取指定的子实例的材质
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_matIns			子实例的材质
	 *	@retval		指定的子实例的材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns);

	/****************************************************************************
	 *	@brief		添加配置路径下子实例相对于父级的配置矩阵
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_idPlacement		子实例的配置ID
	 *	@param[in]	i_mtxInsPlc			子实例的配置矩阵
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsPlacement(wstring i_wstrInsPlcPath, STK_ID i_idPlacement, STK_MTX32 i_mtxInsPlc);

	/****************************************************************************
	 *	@brief		获取指定的子实例相对于父级的配置矩阵
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_idPlacement		子实例的配置ID
	 *	@param[out]	o_mtxInsPlc			子实例的配置矩阵
	 *	@retval		指定的子实例的配置矩阵
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsPlacementByPlcPath(wstring i_wstrInsPlcPath, STK_ID& o_idPlacement, STK_MTX32& o_mtxInsPlc);

	/****************************************************************************
	 *	@brief		获取包含实例属性的所有配置路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所有配置路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<wstring>				GetAllInsAttrPlcPath();

	/****************************************************************************
	 *	@brief		删除指定配置路径的实例属性
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	删除成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath);

	/****************************************************************************
	 *	@brief		删除当前视图中所有的实例属性
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	删除成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteAllInsAttribute();
};

#endif