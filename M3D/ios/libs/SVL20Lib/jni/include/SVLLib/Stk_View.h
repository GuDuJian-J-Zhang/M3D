// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_View.h
 *
 *	@brief	视图类
 *
 *	@par	历史:
 *		2013/08/10	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_VIEW_ENTITY_H_
#define _SVLLIB_VIEW_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif


#include "Stk_Camera.h"
//#include "Stk_SurfaceEntity.h"
//#include "Stk_NodeConnector.h"
#include "Stk_InsAttribute.h"
#include "Stk_Object.h"

namespace HoteamSoft {
	namespace SVLLib {
		//class Stk_ViewDocument;
		/**************************************************************************************
			 *	@class	Stk_View
			 *	@brief	View视图类
			 *
			 *	继承自Stk_Entity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT  Stk_View :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_View)
		protected:
			enum { _typeID = _SVLLIB_TYPE_VIEW };	//!< 类别
		
		public:
			/****************************************************************************
			 *	@brief		Stk_View构造函数
			 *	@param[in]	i_ViewDocP		Stk_ViewDocument指针
			 *	@param[in]	i_stkPartP		Stk_Part对象指针
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_View();

			/****************************************************************************
			 *	@brief		Stk_View析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
			 *	@attention	无
			 ****************************************************************************/
			virtual ~Stk_View(void);

			void Init(Stk_ViewDocument* i_ViewDocP, int i_iPartID);

		public:
			/****************************************************************************
			*	@brief		设置视图唯一标识
			*	@param[in]	i_Identy	视图唯一标识
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//void						SetIdenty(STK_ID i_Identy);

			/****************************************************************************
			*	@brief		获取视图唯一标识
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		视图的唯一标识
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//STK_ID			GetIdenty();

			/****************************************************************************
			*	@brief		设置视图ID
			*	@param[in]	i_ID	视图ID
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetViewID(STK_ID i_ID);

			/****************************************************************************
			*	@brief		获取视图ID
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		视图的ID
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_ID			GetViewID();

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
			void						SetCamera(Stk_CameraPtr i_CameraP);

			/****************************************************************************
			 *	@brief		获取照相机
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_CameraP		照相机对象
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_CameraPtr					GetCamera(void);

			/****************************************************************************
			 *	@brief		获取所有的剖面
			 *	@param[in]	无
			 *	@param[out]	vcPlaneEntities 剖面集合
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			const vector<Stk_PlaneEntityPtr>&  GetAllPlaneEntity();

			/****************************************************************************
			*	@brief		添加面到数组中
			*	@param[in]	i_PlaneEntityP		面
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						AddPlaneEntity(Stk_PlaneEntityPtr i_PlaneEntityP);

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
			*	@brief		设置是否是默认视图
			*	@param[in]	i_bDefault	是否是默认视图状态
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetDefault(STK_BOOL i_bDefault);

			/****************************************************************************
			*	@brief		判断是否是默认视图
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsDefaulted	相机是否活动
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsDefault();

			/****************************************************************************
			*	@brief		添加视图与PMI之间的关联类对象数据
			*	@param[in]	inConnector		视图与PMI之间的关联
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@retval		STK_ERROR			添加失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddConnector(StkEntityTypeEnum eEntityType, STK_ID entityID);

			/****************************************************************************
			*	@brief		获取所有视图与PMI之间的关联类对象数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcConnectors	所有的视图与PMI之间的关联
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const map<StkEntityTypeEnum, vector<STK_ID>>& GetConnector();

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
			 *	@param[out]	vcInsPlcPaths	所有配置路径
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			void GetAllInsAttrPlcPath(vector<wstring>& vcInsPlcPaths);

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
		};
	}//SVLLib
}//HoteamSoft

#endif