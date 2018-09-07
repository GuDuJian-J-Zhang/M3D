// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/****************************************************************************
*	@file	Stk_Render.h
*
*	@brief	Stk_Render渲染类
*
*	@par	历史:
*		2015/09/07	创建。WangY@HOTEAMSOFT
 ****************************************************************************/

#ifndef _Stk_RENDER_H_
#define _Stk_RENDER_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"
#include "Stk_Texture.h"

/**************************************************************************************
	 *	@class	Stk_Render
	 *	@brief	Stk_Render 渲染类
	 *
	 *	继承自Stk_Object类。
***************************************************************************************/

class DATAMANAGER_API  Stk_Render :
	public Stk_Object
{
private:
	Stk_ProtoType*		m_stkProtoTypeP;		//!< 所属的ProtoType对象，升级到V2.355后，不再使用
	STK_ID				m_ID;					//!< 渲染的ID标识
	wstring				m_wstrName;				//!< 纹理名称
	Stk_Texture*		m_stkTextureP;			//!< 纹理对象
public:
	/****************************************************************************
	 *	@brief		Stk_Render构造函数
	 *	@param[in]	i_stkProtoP			所属ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	当Render存储于File中时，不使用此接口
	 ****************************************************************************/
	Stk_Render(Stk_ProtoType* i_stkProtoP);

	/****************************************************************************
	 *	@brief		Stk_Render构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	当Render存储于File中时，使用此接口
	 ****************************************************************************/
	Stk_Render(void);

	/****************************************************************************
	 *	@brief		Stk_Render析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Render();
public:
	/****************************************************************************
	 *	@brief		注册Render ID
	 *	@param[in]	i_idRender		Render ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	由于Render数据不再放置在ProtoType中，而放置于File，因此，ID不再管理
	 ****************************************************************************/
	STK_STATUS			RegisterID(STK_ID i_idRender);

	/****************************************************************************
	 *	@brief		设置Render ID
	 *	@param[in]	i_idRender		Render ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	Render数据存储于File中时，使用SetID，不适用RegisterID
	 ****************************************************************************/
	STK_STATUS			SetID(STK_ID i_idRender);

	/****************************************************************************
	 *	@brief		获取Render ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Render ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID				GetID();

	/****************************************************************************
	 *	@brief		设置纹理名称
	 *	@param[in]	i_wstrName			纹理名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetTextureName(wstring i_wstrName);
	
	/****************************************************************************
	 *	@brief		获取纹理名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		纹理名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring				GetTextureName();

	/****************************************************************************
	 *	@brief		设置纹理
	 *	@param[in]	i_stkTextureP		纹理对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetTexture(Stk_Texture* i_stkTextureP);
	
	/****************************************************************************
	 *	@brief		获取纹理
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		纹理对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Texture*		GetTexture();

	/****************************************************************************
	 *	@brief		读取渲染(纹理)文件
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_RenderHeadSP		渲染段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_SUCCESS_EXIST	已读取跳出
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_RenderHeadSP);
	
	/****************************************************************************
	 *	@brief		写渲染(纹理)文件
	 *	@param[in]	i_stkFileP		文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其他			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(Stk_File *i_stkFileP);
};

#endif