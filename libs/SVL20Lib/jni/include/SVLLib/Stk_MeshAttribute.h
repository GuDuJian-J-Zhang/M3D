// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MeshAttribute
*	@brief	����������
*	@par ��ʷ��¼
*				2017/07/14 ������qinyp(Hoteamsoft)
**************************************************************************/
#pragma once

#include "Stk_Object.h"

#ifndef _SVLLIB_MESHATTRIBUTE_H_
#define _SVLLIB_MESHATTRIBUTE_H_
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_MeshAttribute : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_MeshAttribute)
		public:
			Stk_MeshAttribute();
			virtual ~Stk_MeshAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_MESHATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ü�������
			*	@param[in]	i_geoAttrP		��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS	�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS                  SetGeometryAttribute(Stk_GeometryAttributePtr i_geoAttrP);
		};
	}
}

#endif