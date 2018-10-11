// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_GroupFeature.cpp
 *
 *	@brief	组特征类
 *
 *	@par	历史:
 *		2014/11/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_GroupFeature.h"
#include "Stk_PatternFeature.h"
#include "Stk_AssemblyFeature.h"
#include "Stk_Platform.h"

/*************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
 *	@param[in]	i_stkProtoP				ProtoType
 *	@param[out]	无
 *	@retval		无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_GroupFeature::Stk_GroupFeature(Stk_DocumentManager* i_stkDocManagerP, Stk_ProtoType* i_stkProtoP)
	: Stk_ComponentFeature(i_stkDocManagerP, i_stkProtoP)
{

}

/*************************************************************************//**
 *	@brief		析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_GroupFeature::~Stk_GroupFeature()
{

}

/************************************************************************//**
 *	@brief		设置当前特征相对上级的配置ID
 *	@param[in]	i_idPlacement		配置ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_STATUS Stk_GroupFeature::SetPlcID(STK_ID i_idPlacement)
//{
//	m_idPlacement = i_idPlacement;
//	return STK_SUCCESS;
//}

/************************************************************************//**
 *	@brief		获取当前特征相对上级的配置ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		配置ID号
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_ID Stk_GroupFeature::GetPlcID()
//{
//	return m_idPlacement;
//}

/************************************************************************//**
 *	@brief		往特征下添加包含的实例配置ID
 *	@param[in]	i_idPlacement		配置ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_GroupFeature::AddChildInsPlcID(STK_ID i_idPlacement)
{
	m_vcInstancePlcIDs.push_back(i_idPlacement);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特征下包含的实例数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_GroupFeature::GetChildInsNum()
{
	return m_vcInstancePlcIDs.size();
}

/************************************************************************//**
 *	@brief		获取特征下指定的实例配置ID
 *	@param[in]	i_nuIndex			索引
 *	@param[out]	无
 *	@retval		指定的实例配置ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_GroupFeature::GetChildInsPlcIDByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcInstancePlcIDs.at(i_nuIndex);
}

/*************************************************************************//**
 *	@brief		写组件特征文件
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
 *	@retval		STK_ERROR			保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_GroupFeature::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			ComponentFeatHead = {0};
	StkComponentFeatRec2	ComponentFeatRec2 = {0};
	StkVirtualFeatRec*		VirtualFeatRecP = NULL;
	STK_UINT32				nuChildInsNum = 0;
	STK_UINT32				nuFeatureNum = 0;
	StkComponentFeatureEnum	eComponentFeat = TYPE_COMPONENTFEATURE_UNKNOWN;
	Stk_ComponentFeature*	ComponentFeatP = NULL;
	wstring					wstrComponentName = L"";

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第一步】 写段头记录
	 */
	ComponentFeatHead.id = GetID();
	ComponentFeatHead.kind = SEG_TYPE_COMP_GROUPFEAT;
	eState = i_stkFileP->WriteSegmentHeadData(&ComponentFeatHead);

	/*
	 *	【第二步】 写相关的组件特征记录
	 */
	// [2.1] 组件特征记录
	ComponentFeatRec2.Control.type = REC_TYPE_COMPONENTFEATURE;
	ComponentFeatRec2.Control.length = sizeof(StkComponentFeatRec2) - sizeof(StkControl);
	ComponentFeatRec2.Control.version = _VER_COMPONENTFEATURE_RECORD;
	
	wstrComponentName = GetName();
#ifdef __MOBILE__
	memcpy(ComponentFeatRec2.wchName, Stk_Platform::WStringToSTKWCHARStr(wstrComponentName), (wstrComponentName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(ComponentFeatRec2.wchName, STK_ULTRANAME, (WCHAR*)wstrComponentName.c_str());
#endif
	ComponentFeatRec2.nNumber = GetNumber();
	ComponentFeatRec2.nuType = GetType();
	ComponentFeatRec2.nuChildFeatNum = GetChildFeatNum();

	eState = i_stkFileP->WriteSegmentRecordData(&ComponentFeatHead, (StkControl*)&ComponentFeatRec2);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [2.2] 当前组件特征下的子实例配置ID记录
	nuChildInsNum = m_vcInstancePlcIDs.size();
	VirtualFeatRecP = (StkVirtualFeatRec*)calloc(1, sizeof(StkVirtualFeatRec) - sizeof(STK_UINT32) + nuChildInsNum * sizeof(STK_UINT32));
	if (VirtualFeatRecP == NULL)
	{
		return STK_ERROR;
	}
	VirtualFeatRecP->Control.type = REC_TYPE_VIRTUALFEATURE;
	VirtualFeatRecP->Control.length = sizeof(StkVirtualFeatRec) - sizeof(STK_UINT32) + nuChildInsNum * sizeof(STK_UINT32) - sizeof(StkControl);
	VirtualFeatRecP->Control.version = _VER_VIRTUALFEATURE_RECORD;

	//VirtualFeatRecP->nuPlcID = m_idPlacement;
	VirtualFeatRecP->nuChildInsNum = nuChildInsNum;
	for (int ii = 0; ii < nuChildInsNum; ii++)
	{
		VirtualFeatRecP->atr[ii] = m_vcInstancePlcIDs[ii];
	}

	eState = i_stkFileP->WriteSegmentRecordData(&ComponentFeatHead, (StkControl*)VirtualFeatRecP);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (VirtualFeatRecP != NULL)
	{
		delete VirtualFeatRecP;
		VirtualFeatRecP = NULL;
	}

	/*
	 *	【第三步】 完成组件特征段的输出
	 */
	eState = i_stkFileP->WriteSegmentFinishData(&ComponentFeatHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第四步】 输出特征段的子段
	 */
	nuFeatureNum = GetChildFeatNum();
	for (int ii = 0; ii < nuFeatureNum; ii++)
	{
		ComponentFeatP = GetChildComponentFeatByIndex(ii);
		if (ComponentFeatP == NULL)
		{
			break;
		}
		switch(ComponentFeatP->GetType())
		{
		case TYPE_COMPONENTFEATURE_GROUP:
			eState = ((Stk_GroupFeature*)ComponentFeatP)->WriteFile(i_stkFileP);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;
		case TYPE_COMPONENTFEATURE_PATTERN:
			eState = ((Stk_PatternFeature*)ComponentFeatP)->WriteFile(i_stkFileP);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;
		case TYPE_COMPONENTFEATURE_ASSEMBLY:
			eState = ((Stk_AssemblyFeature*)ComponentFeatP)->WriteFile(i_stkFileP);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;
		default:
			break;
		}
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取文件中的ProtoType数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[in]	i_FeatHeadP			组件特征记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR			读取失败
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GroupFeature::ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_FeatHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head ={0};
	StkControl**			RecordPP = NULL;
	StkComponentFeatRec*	ComponentFeatRecP = NULL;
	StkComponentFeatRec2*	ComponentFeatRec2P = NULL;
	StkVirtualFeatRec*		VirtualFeatRecP = NULL;
	StkComponentFeatureEnum	eComponentFeat = TYPE_COMPONENTFEATURE_UNKNOWN;
	
	STK_UINT32				nuChildFeatNum = 0;
	Stk_GroupFeature*		stkGroupFeatP = NULL;
	Stk_PatternFeature*		stkPatternFeatP = NULL;
	Stk_DocumentManager*	stkDocumentManagerP = NULL;
	Stk_ProtoType*			stkProtoTypeP = NULL;

	RegisterID(i_FeatHeadP->id);

	/*
	 * 读取组件特征段的数据
	 */
	RecordPP = (StkControl **)calloc(i_FeatHeadP->recnum, sizeof(StkControl*));
	if (i_FeatHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_stkFileP->ReadSegmentAllRecordData(i_FeatHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取组件特征段各记录
	 */
	for (int ii = 0; ii < i_FeatHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_COMPONENTFEATURE:
			if (RecordPP[ii]->version == 1)
			{
				ComponentFeatRecP = (StkComponentFeatRec*)RecordPP[ii];
				SetNumber(ComponentFeatRecP->nNumber);
				SetType((StkComponentFeatureEnum)ComponentFeatRecP->nuType);
			}
			else if (RecordPP[ii]->version == 2)
			{
				ComponentFeatRec2P = (StkComponentFeatRec2*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(ComponentFeatRec2P->wchName));
#else
				SetName(ComponentFeatRec2P->wchName);
#endif
				SetNumber(ComponentFeatRec2P->nNumber);
				SetType((StkComponentFeatureEnum)ComponentFeatRec2P->nuType);
				nuChildFeatNum = ComponentFeatRec2P->nuChildFeatNum;
			}
			break;
		case REC_TYPE_VIRTUALFEATURE:
			if (RecordPP[ii]->version == 1)
			{
				VirtualFeatRecP = (StkVirtualFeatRec*)RecordPP[ii];
				//m_idPlacement = VirtualFeatRecP->nuPlcID;
				for (int jj = 0; jj < VirtualFeatRecP->nuChildInsNum; jj++)
				{
					m_vcInstancePlcIDs.push_back(VirtualFeatRecP->atr[jj]);
				}
			}
			break;
		default:
			break;
		}
	}
	
	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_FeatHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取下一个特征段
	 */
	stkDocumentManagerP = GetDocumentManager();
	stkProtoTypeP = GetProtoType();
	if (stkDocumentManagerP == NULL || stkProtoTypeP == NULL)
	{
		return STK_ERROR;
	}
	for(int ii = 0; ii < nuChildFeatNum; ii++)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}

		switch (head.kind) 
		{
		case SEG_TYPE_COMP_GROUPFEAT:
			stkGroupFeatP = new Stk_GroupFeature(stkDocumentManagerP, stkProtoTypeP);
			eState = stkGroupFeatP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			AddChildComponentFeat(stkGroupFeatP);
			break;
		case SEG_TYPE_COMP_PATTERN:
			stkPatternFeatP = new Stk_PatternFeature(stkDocumentManagerP, stkProtoTypeP);
			eState = stkPatternFeatP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			AddChildComponentFeat(stkPatternFeatP);
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
	return eState;
}