// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Render.cpp
 *
 *	@brief	渲染类成员函数的实现
 *
 *	@par	历史:
 *		2015/09/08	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Render.h"
#include "Stk_Platform.h"
#include "Stk_ProtoType.h"

/************************************************************************//**
 *	@brief		Stk_Render构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Render::Stk_Render(Stk_ProtoType* i_stkProtoP)
{
	m_stkProtoTypeP = i_stkProtoP;
	m_ID = 0;
	m_wstrName = L"";
	m_stkTextureP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Render构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	当Render存储于File中时，使用此接口
 ****************************************************************************/
Stk_Render::Stk_Render(void)
{
	m_stkProtoTypeP = NULL;
	m_ID = 0;
	m_wstrName = L"";
	m_stkTextureP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Render析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Render::~Stk_Render()
{
	if (m_stkTextureP != NULL)
	{
		delete m_stkTextureP;
		m_stkTextureP = NULL;
	}
}

/************************************************************************//**
 *	@brief		注册Render ID
 *	@param[in]	i_idRender		Render ID <br>
 *				== STK_NEW_ID	注册新ID <br>
 *				== 其它			注册指定ID
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		指定ID需大于等于1的整数
 *	@attention	由于Render数据不再放置在ProtoType中，而放置于File，因此，ID不再管理
 ****************************************************************************/
STK_STATUS Stk_Render::RegisterID(STK_ID i_idRender)
{
	STK_STATUS eState = STK_SUCCESS;
	if (m_stkProtoTypeP == NULL)
	{
		return STK_ERROR;
	}
	eState = m_stkProtoTypeP->GetProtoIDManager()->RegRenderID(this, i_idRender);
	if (eState == STK_SUCCESS)
	{
		m_ID = i_idRender;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		设置Render ID
 *	@param[in]	i_idRender		Render ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	Render数据存储于File中时，使用SetID，不适用RegisterID
 ****************************************************************************/
STK_STATUS Stk_Render::SetID(STK_ID i_idRender)
{
	m_ID = i_idRender;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Render ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Render ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_Render::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置纹理名称
 *	@param[in]	i_wstrName			纹理名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Render::SetTextureName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取纹理名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		纹理名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Render::GetTextureName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置纹理
 *	@param[in]	i_stkTextureP		纹理对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Render::SetTexture(Stk_Texture* i_stkTextureP)
{
	m_stkTextureP = i_stkTextureP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取纹理
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		纹理对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Texture* Stk_Render::GetTexture()
{
	return m_stkTextureP;
}

/************************************************************************//**
 *	@brief		读取渲染(纹理)文件
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_RenderHeadSP		渲染段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_SUCCESS_EXIST	已读取跳出
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Render::ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_RenderHeadSP)
{
	STK_STATUS		eState = STK_SUCCESS;
	StkControl**	RecordPP = NULL;
	StkRenderRec*	stkRenderRecP = NULL;
	StkSegmentHead	head = {0};
	Stk_Texture*	stkTextureP = NULL;
	wstring			wstrVersion = L"";
	string			strVersion = "";
	STK_FLOAT32		fVersion = 0;
	STK_UINT32		nuRenderNum = 0;

	wstrVersion = i_stkFileP->GetSVLVersion();
	strVersion = Stk_Platform::WStringToString(wstrVersion);
	fVersion = atof(strVersion.c_str());
	if (fVersion > 2.354f)
	{
		SetID(i_RenderHeadSP->id);
	}
	else
	{
		RegisterID(i_RenderHeadSP->id);
	}

	/*
	 *	读取渲染段的数据
	 */
	RecordPP = (StkControl **)calloc(i_RenderHeadSP->recnum, sizeof(StkControl*));
	if (i_RenderHeadSP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_RenderHeadSP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取渲染段的各记录
	 */
	for (int ii = 0; ii < i_RenderHeadSP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_RENDER)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkRenderRecP = (StkRenderRec*)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkRenderRecP->name);
#else
				m_wstrName = stkRenderRecP->name;
#endif
			}
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_RenderHeadSP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 * 读取渲染段中的子段
	 */
	while (1)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}
		switch (head.kind) 
		{
		case SEG_TYPE_TEXTURE:
			stkTextureP = new Stk_Texture();
			eState = stkTextureP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkTextureP;
				stkTextureP = NULL;
				return eState;
			}
			SetTexture(stkTextureP);
			break;
		case SEG_TYPE_RENDER_END:
			goto pt1;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = i_stkFileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}
pt1:
	return eState;
}

/*************************************************************************//**
 *	@brief		写渲染(纹理)文件
 *	@param[in]	i_stkFileP		文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Render::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS		eState = STK_SUCCESS;
	StkSegmentHead	RenderHead = {0};
	StkSegmentHead	endHead = {0};
	StkRenderRec	stkRenderRec = {0};

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 * 【第1步】 输出渲染段
	 */

	// 【1.1 输出段头记录】
	RenderHead.id = m_ID;
	RenderHead.kind = SEG_TYPE_RENDER;
	eState = i_stkFileP->WriteSegmentHeadData(&RenderHead);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// 【1.2 输出渲染记录】
	stkRenderRec.Control.type = REC_TYPE_RENDER;
	stkRenderRec.Control.length = sizeof(StkRenderRec) - sizeof(StkControl);
	stkRenderRec.Control.version = _VER_RENDER_RECORD;
#ifdef __MOBILE__
	memcpy(stkRenderRec.name,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkRenderRec.name, STK_MAXNAME, (WCHAR*)m_wstrName.c_str());
#endif
	
	eState = i_stkFileP->WriteSegmentRecordData(&RenderHead, (StkControl*)&stkRenderRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.3 完成渲染段的输出】
	eState = i_stkFileP->WriteSegmentFinishData(&RenderHead);
	if( eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 * 【第2步】 输出纹理段
	 */
	if(m_stkTextureP != NULL)
	{
		m_stkTextureP->WriteFile(i_stkFileP);
	}
	
	/*
	 * 【第3步】 输出渲染-END段
	 */
	endHead.id = 0;
	endHead.kind = SEG_TYPE_RENDER_END;
	eState = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}