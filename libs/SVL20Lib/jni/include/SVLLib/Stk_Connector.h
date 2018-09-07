// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Connector.h
*
*	@brief	关联类
*
*	@par	历史:
*
****************************************************************************/
#ifndef _SVLLIB_CONNECTOR_H_
#define _SVLLIB_CONNECTOR_H_
#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {

		typedef struct {
			STK_ID					idEntity;
			StkEntityTypeEnum		nuEntityType;
			STK_PNT32				pntPick;		// 为攻长义应用层提供，Pick坐标
			STK_DIR32				dirPick;		// 为攻长义应用层提供，Pick方向向量
		}SubEntity;

		class STK_TOOLKIT_EXPORT Stk_Connector : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Connector)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CONNECTOR };	//!< 类别
		public:
			Stk_Connector();
			virtual ~Stk_Connector();
		public:
			/****************************************************************************
			*	@brief		设置Entity类型
			*	@param[in]	i_ePointingEnt			指向类型
			*	@param[in]	i_ePointedEnt			被指向类型
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetEntityType(StkEntityTypeEnum i_ePointingEnt, StkEntityTypeEnum i_ePointedEnt);

			/****************************************************************************
			*	@brief		获取Entity类型
			*	@param[in]	无
			*	@param[out]	o_ePointingEnt			指向类型
			*	@param[out]	o_ePointedEnt			被指向类型
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						GetEntityType(StkEntityTypeEnum& o_ePointingEnt, StkEntityTypeEnum& o_ePointedEnt);

			/****************************************************************************
			*	@brief		设置Proto Link
			*	@param[in]	i_wstrPlcPath			配置路径
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetLinkProto(const wstring &i_wstrPlcPath);

			/****************************************************************************
			*	@brief		获取Proto Link
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrPlcPath	配置路径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring						GetLinkProto();

			/****************************************************************************
			*	@brief		判断是否有Proto Link
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsLinkProto	配置路径状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsLinkProto();

			/****************************************************************************
			*	@brief		设置是否是否指向其它ProtoType
			*	@param[in]	i_bLinkProto 
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void                       SetIsLinkProto(STK_BOOL i_bLinkProto);

			/****************************************************************************
			*	@brief		设置NodeConnector Link
			*	@param[in]	i_idNode		Node ID
			*	@param[out]	无
			*	@retval		无
			*	@note		例如PMI、图层信息集等Node级
			*	@attention	无
			****************************************************************************/
			void						SetLinkNode(STK_ID i_idNode);

			/****************************************************************************
			*	@brief		设置是否Connector Link
			*	@param[in]	i_bLinkNode		是否设置
			*	@param[out]	无
			*	@retval		无
			*	@note		
			*	@attention	无
			****************************************************************************/
			void                        SetIsLinkNode(STK_BOOL i_bLinkNode);

			/****************************************************************************
			*	@brief		获取NodeConnector Link
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_idNode		Node ID
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_ID						GetLinkNode();

			/****************************************************************************
			*	@brief		判断是否有Node Link
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsLinkNode	Node Link状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsLinkNode();

			/****************************************************************************
			*	@brief		设置子Entity
			*	@param[in]	i_SubEntity		子Entity
			*	@param[out]	无
			*	@retval		无
			*	@note		例如边、面、图层等
			*	@attention	无
			****************************************************************************/
			void						AddSubEntity(SubEntity i_SubEntity);

			/****************************************************************************
			*	@brief		获取子Entity
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcEntities	所有子Entity
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<SubEntity>			GetSubEntity();

			/****************************************************************************
			*	@brief		判断是否有子Entity Link
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bIsLinkEntities	子Entity Link状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					IsLinkEntities();

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
	}
}
#endif //_SVLLIB_CONNECTOR_H_