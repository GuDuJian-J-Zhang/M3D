// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AssemblyFeature.cpp
 *
 *	@brief	装配特征类
 *
 *	@par	历史:
 *		2014/11/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
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
Stk_AssemblyFeature::Stk_AssemblyFeature(Stk_DocumentManager* i_stkDocManagerP, Stk_ProtoType* i_stkProtoP)
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
Stk_AssemblyFeature::~Stk_AssemblyFeature()
{

}

/****************************************************************************
 *	@brief		添加装配特征中所包含的要素Node ID
 *	@param[in]	i_idPlacement		配置ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_AssemblyFeature::AddNodeID(STK_ID i_idPlacement)
{
	m_vcNodeIds.push_back(i_idPlacement);
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取装配特征中所包含的要素Node数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Node数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_AssemblyFeature::GetNodeNum()
{
	return m_vcNodeIds.size();
}

/****************************************************************************
 *	@brief		获取指定的Node ID
 *	@param[in]	i_nuIndex			索引
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_AssemblyFeature::GetNodeIDByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcNodeIds.at(i_nuIndex);
}

/****************************************************************************
 *	@brief		设置装配特征关联的实例的配置路径
 *	@param[in]	i_wstrPlcPath		配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_AssemblyFeature::SetPlcPath(wstring i_wstrPlcPath)
{
	m_wstrPlcPath = i_wstrPlcPath;
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取装配特征关联的实例的配置路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		配置路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_AssemblyFeature::GetPlcPath()
{
	return m_wstrPlcPath;
}

/*****************************************************************************
 *	@brief		写组件特征文件
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
 *	@retval		STK_ERROR			保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_AssemblyFeature::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			ComponentFeatHead = {0};
	StkComponentFeatRec2	ComponentFeatRec2 = {0};
	StkAssemblyFeatRec*		AssemblyFeatRecP = NULL;
	STK_UINT32				nuNodeNum = 0;
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
	ComponentFeatHead.kind = SEG_TYPE_COMP_ASSEMBLYFEAT;
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

	// [2.2] 装配特征记录
	nuNodeNum = m_vcNodeIds.size();
	AssemblyFeatRecP = (StkAssemblyFeatRec*)calloc(1, sizeof(StkAssemblyFeatRec) - sizeof(STK_UINT32) + nuNodeNum * sizeof(STK_UINT32));
	if (AssemblyFeatRecP == NULL)
	{
		return STK_ERROR;
	}
	AssemblyFeatRecP->Control.type = REC_TYPE_ASSEMBLYFEATURE;
	AssemblyFeatRecP->Control.length = sizeof(StkAssemblyFeatRec) - sizeof(STK_UINT32) + nuNodeNum * sizeof(STK_UINT32) - sizeof(StkControl);
	AssemblyFeatRecP->Control.version = _VER_ASSEMBLYFEATURE_RECORD;

#ifdef __MOBILE__
	memcpy(AssemblyFeatRecP->wchPlcPath, Stk_Platform::WStringToSTKWCHARStr(m_wstrPlcPath), (m_wstrPlcPath.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(AssemblyFeatRecP->wchPlcPath, STK_MAXPATH, (WCHAR*)m_wstrPlcPath.c_str());
#endif
	AssemblyFeatRecP->nuNodeNum = nuNodeNum;
	for (int ii = 0; ii < nuNodeNum; ii++)
	{
		AssemblyFeatRecP->atr[ii] = m_vcNodeIds[ii];
	}

	eState = i_stkFileP->WriteSegmentRecordData(&ComponentFeatHead, (StkControl*)AssemblyFeatRecP);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (AssemblyFeatRecP != NULL)
	{
		delete AssemblyFeatRecP;
		AssemblyFeatRecP = NULL;
	}

	/*
	 *	【第三步】 完成组件特征段的输出
	 */
	eState = i_stkFileP->WriteSegmentFinishData(&ComponentFeatHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

#if 0
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
#endif
	return eState;
}

/****************************************************************************
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
STK_STATUS Stk_AssemblyFeature::ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_FeatHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head ={0};
	StkControl**			RecordPP = NULL;
	StkComponentFeatRec*	ComponentFeatRecP = NULL;
	StkComponentFeatRec2*	ComponentFeatRec2P = NULL;
	StkAssemblyFeatRec*		AssemblyFeatRecP = NULL;
	STK_UINT32				nuFeatureNum = 0;
	StkComponentFeatureEnum	eComponentFeat = TYPE_COMPONENTFEATURE_UNKNOWN;
	Stk_ComponentFeature*	ComponentFeatP = NULL;

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
			}
			break;
		case REC_TYPE_ASSEMBLYFEATURE:
			if (RecordPP[ii]->version == 1)
			{
				AssemblyFeatRecP = (StkAssemblyFeatRec*)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrPlcPath = Stk_Platform::STKWCHARStrToWString(AssemblyFeatRecP->wchPlcPath);
#else
				m_wstrPlcPath = AssemblyFeatRecP->wchPlcPath;
#endif
				for (int jj = 0; jj < AssemblyFeatRecP->nuNodeNum; jj++)
				{
					m_vcNodeIds.push_back(AssemblyFeatRecP->atr[jj]);
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
#if 0
	/*
	 *	读取下一个特征段
	 */
	nuFeatureNum = GetChildFeatNum();
	for(int ii = 0; ii < nuFeatureNum; ii++)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}

		ComponentFeatP = GetChildComponentFeatByIndex(ii);
		if (ComponentFeatP == NULL)
		{
			break;
		}
		eComponentFeat = ComponentFeatP->GetType();
		switch(eComponentFeat)
		{
		case TYPE_COMPONENTFEATURE_GROUP:
			eState = ((Stk_GroupFeature*)ComponentFeatP)->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;
		case TYPE_COMPONENTFEATURE_PATTERN:
			eState = ((Stk_PatternFeature*)ComponentFeatP)->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
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
#endif
	return eState;
}