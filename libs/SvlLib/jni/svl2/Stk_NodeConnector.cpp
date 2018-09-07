// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_NodeConnector.cpp
 *
 *	@brief	Stk_NodeConnector关联类
 *
 *	@par	历史:
 *		2013/08/19	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_NodeConnector.h"
#include "Stk_Manager.h"
#include "Stk_Platform.h"
#include "Stk_Utility.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_NodeConnector构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_NodeConnector::Stk_NodeConnector(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_bIsLinkProto = FALSE;
	m_bIsLinkNode = FALSE;
	m_bIsLinkEntities = FALSE;
	m_wstrPlcPath = L"";
	m_idNode = 0;

}

/************************************************************************//**
 *	@brief		Stk_NodeConnector析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_NodeConnector::~Stk_NodeConnector(void)
{

}

/************************************************************************//**
 *	@brief		读取文件中的关联类数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_ViewHeadP			View记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_NodeConnector::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ViewHeadP)
{
	StkControl			**RecordPP = NULL;
	UINT ii, jj;

	StkNodeLinkBaseRec	*LinkBaseRecP = NULL;
	StkNodeLinkBaseRec2	*LinkBaseRec2P = NULL;
	StkProtoLinkRec		*ProtoLinkRecP = NULL;
	StkNodeIDRec		*NodeIDRecP = NULL;
	StkSubEntityRec		*SubEntityRecP = NULL;
	StkSubEntityRec2	*SubEntityRec2P = NULL;
	STK_STATUS			eState;


	// 注册ID
	//RegisterID(i_ViewHeadP->id );	//NodeConnector的注册ID，放到ReadFile里面
	
	/*
	 *	读取NodeConnector段的数据
	 */
	RecordPP = (StkControl **)calloc(i_ViewHeadP->recnum, sizeof(StkControl*));
	if (i_ViewHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_ViewHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取NodeConnector段的各记录
	 */
	for (ii = 0; ii < i_ViewHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_NODELINKBASE)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	填充NodeLink基础部分
				 */
				LinkBaseRecP = (StkNodeLinkBaseRec *)RecordPP[ii];
				m_ePointingEntityType = (StkEntityTypeEnum)LinkBaseRecP->PointingEntityType;
				m_ePointedEntityType = (StkEntityTypeEnum)LinkBaseRecP->PointedEntityType;
			}
			else if (RecordPP[ii]->version == 2)
			{
				/*
				 *	填充NodeLink基础部分
				 */
				LinkBaseRec2P = (StkNodeLinkBaseRec2 *)RecordPP[ii];
				m_ePointingEntityType = (StkEntityTypeEnum)LinkBaseRec2P->PointingEntityType;
				m_ePointedEntityType = (StkEntityTypeEnum)LinkBaseRec2P->PointedEntityType;
			}
			else
			{
				// ERROR
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_PROTOLINK)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	ProtoLink部分
				 */
				ProtoLinkRecP = (StkProtoLinkRec *)RecordPP[ii];
				m_bIsLinkProto = TRUE;
#ifdef __MOBILE__
				m_wstrPlcPath = Stk_Platform::STKWCHARStrToWString(ProtoLinkRecP->atr);
#else
				m_wstrPlcPath = ProtoLinkRecP->atr;
#endif
			}
			else
			{
				//ERROR
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_NODEID)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	NodeID部分
				 */
				NodeIDRecP = (StkNodeIDRec *)RecordPP[ii];
				m_idNode = NodeIDRecP->NodeID;
				m_bIsLinkNode = TRUE;
			}
			else
			{
				//ERROR
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_SUBENTITY)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	SubEntity部分
				 */
				SubEntityRecP = (StkSubEntityRec *)RecordPP[ii];
				STK_UINT32	NumSubEntity = SubEntityRecP->NumSubEntity;
				for (jj = 0; jj < NumSubEntity; jj++)
				{
					SubEntity SubEntityTmp;
					SubEntityTmp.idEntity = SubEntityRecP->atr[jj].EntityID;
					SubEntityTmp.nuEntityType = (StkEntityTypeEnum )SubEntityRecP->atr[jj].EntityType;
					m_vcEntities.push_back(SubEntityTmp);
				}
				m_bIsLinkEntities = TRUE;
			}
			else if (RecordPP[ii]->version == 2)
			{
				/*
				 *	SubEntity部分
				 */
				SubEntityRec2P = (StkSubEntityRec2 *)RecordPP[ii];
				STK_UINT32	NumSubEntity = SubEntityRec2P->NumSubEntity;
				for (jj = 0; jj < NumSubEntity; jj++)
				{
					SubEntity SubEntityTmp;
					SubEntityTmp.idEntity = SubEntityRec2P->atr[jj].EntityID;
					SubEntityTmp.nuEntityType = (StkEntityTypeEnum )SubEntityRec2P->atr[jj].EntityType;
					SubEntityTmp.pntPick = SubEntityRec2P->atr[jj].pntPick;
					SubEntityTmp.dirPick = SubEntityRec2P->atr[jj].dirPick;
					m_vcEntities.push_back(SubEntityTmp);
				}
				m_bIsLinkEntities = TRUE;
			}
			else
			{

			}
		}
		else
		{
			//不做任何操作
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_ViewHeadP,(StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写关联类数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		STK_ERROR			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_NodeConnector::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		head = {0};
	STK_STATUS			ret = STK_SUCCESS;

#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	string			plcPath;
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
	plcPath = Stk_Platform::WStringToString(m_wstrPlcPath);
#endif
	
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【NodeConnector段】输出 ProtoLink:%s NodeID:%d"), m_wstrPlcPath.c_str(), m_idNode);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif
	head.id = 0;
	head.kind = SEG_TYPE_CONNECTOR;
	ret = i_stkFileP->WriteSegmentHeadData(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：NodeConnector段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	NodeConnector基础属性填充
	 */
	StkNodeLinkBaseRec2 *LinkBaseRec2P = NULL;
	LinkBaseRec2P = (StkNodeLinkBaseRec2 *)calloc(1, sizeof(StkNodeLinkBaseRec2));
	if(LinkBaseRec2P == NULL){
		return STK_ERROR;
	}
	LinkBaseRec2P->Control.type = REC_TYPE_NODELINKBASE;
	LinkBaseRec2P->Control.length = sizeof(StkNodeLinkBaseRec2) - sizeof(StkControl);
	LinkBaseRec2P->Control.version = _VER_NODELINKBASE_RECORD /*2*/;
	LinkBaseRec2P->PointingEntityType = m_ePointingEntityType;
	LinkBaseRec2P->PointedEntityType = m_ePointedEntityType;
	//预留
	LinkBaseRec2P->usetype = 0;
	LinkBaseRec2P->dmy1 = 0;
	LinkBaseRec2P->dmy2 = 0;

	ret = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)LinkBaseRec2P);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：NodeConnector Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(LinkBaseRec2P != NULL){
		free(LinkBaseRec2P);
		LinkBaseRec2P = NULL;
	}

	/*
	 *	输出ProtoLink记录
	 */
	if(m_bIsLinkProto){
		STK_UINT32 nuNumChar = (UINT)m_wstrPlcPath.length() + 1;
		StkProtoLinkRec *ProtoLinkRecP = NULL;
		ProtoLinkRecP = (StkProtoLinkRec *)calloc(1, sizeof(StkProtoLinkRec) - sizeof(STK_WCHAR) + nuNumChar * sizeof(STK_WCHAR));
		if(ProtoLinkRecP == NULL){
			return STK_ERROR;
		}
		ProtoLinkRecP->Control.type = REC_TYPE_PROTOLINK;
		ProtoLinkRecP->Control.length = sizeof(StkProtoLinkRec) - sizeof(STK_WCHAR) + nuNumChar * sizeof(STK_WCHAR) - sizeof(StkControl);
		ProtoLinkRecP->Control.version = _VER_PROTOLINK_RECORD;
		ProtoLinkRecP->NumChar = nuNumChar;
		for(UINT ii = 0; ii < nuNumChar - 1; ii++){
			ProtoLinkRecP->atr[ii] = m_wstrPlcPath[ii];
		}
		ProtoLinkRecP->atr[nuNumChar - 1] = L'\0';

		ret = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)ProtoLinkRecP);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：LinkProto记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(ProtoLinkRecP != NULL){
			free(ProtoLinkRecP);
			ProtoLinkRecP = NULL;
		}
	}

	/*
	 *	输出NodeID记录
	 */
	if(m_bIsLinkNode){
		StkNodeIDRec *NodeIDRecP = NULL;
		NodeIDRecP = (StkNodeIDRec *)calloc(1, sizeof(StkControl) + sizeof(STK_UINT32));
		if(NodeIDRecP == NULL){
			return STK_ERROR;
		}

		NodeIDRecP->Control.type = REC_TYPE_NODEID;
		NodeIDRecP->Control.length = sizeof(STK_UINT32);
		NodeIDRecP->Control.version = _VER_NODEID_RECORD;
		NodeIDRecP->NodeID = m_idNode;

		ret = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)NodeIDRecP);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：LinkNode记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(NodeIDRecP != NULL){
			free(NodeIDRecP);
			NodeIDRecP = NULL;
		}
	}

	/*
	 *	输出SubEntity记录
	 */
	if(m_bIsLinkEntities){
		STK_UINT32 nuEntityNum = (UINT)m_vcEntities.size();
		StkSubEntityRec2 *SubEntityRec2P = NULL;
		SubEntityRec2P = (StkSubEntityRec2 *)calloc(1, sizeof(StkSubEntityRec2) - sizeof(StkOneEntity2) + nuEntityNum * sizeof(StkOneEntity2));
		if(SubEntityRec2P == NULL){
			return STK_ERROR;
		}

		SubEntityRec2P->Control.type = REC_TYPE_SUBENTITY;
		SubEntityRec2P->Control.length = sizeof(StkSubEntityRec2) - sizeof(StkOneEntity2) + nuEntityNum * sizeof(StkOneEntity2) - sizeof(StkControl);
		SubEntityRec2P->Control.version = _VER_SUBENTITY_RECOR;
		SubEntityRec2P->NumSubEntity = nuEntityNum;
		for(UINT ii = 0; ii < nuEntityNum; ii++){
			SubEntityRec2P->atr[ii].EntityID = m_vcEntities[ii].idEntity;
			SubEntityRec2P->atr[ii].EntityType = m_vcEntities[ii].nuEntityType;
			SubEntityRec2P->atr[ii].pntPick = m_vcEntities[ii].pntPick;
			SubEntityRec2P->atr[ii].dirPick = m_vcEntities[ii].dirPick;
		}

		ret = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)SubEntityRec2P);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：LinkEntity记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(SubEntityRec2P != NULL){
			free(SubEntityRec2P);
			SubEntityRec2P = NULL;
		}
	}

	/*
	 *	完成NodeConnector段输出
	 */
	ret = i_stkFileP->WriteSegmentFinishData(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：NodeConnector段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		设置Entity类型
 *	@param[in]	i_ePointingEnt			指向类型
 *	@param[in]	i_ePointedEnt			被指向类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_NodeConnector::SetEntityType(StkEntityTypeEnum i_ePointingEnt, StkEntityTypeEnum i_ePointedEnt)
{
	m_ePointingEntityType = i_ePointingEnt;
	m_ePointedEntityType = i_ePointedEnt;
}

/************************************************************************//**
 *	@brief		获取Entity类型
 *	@param[in]	无
 *	@param[out]	o_ePointingEnt			指向类型
 *	@param[out]	o_ePointedEnt			被指向类型
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_NodeConnector::GetEntityType(StkEntityTypeEnum& o_ePointingEnt, StkEntityTypeEnum& o_ePointedEnt)
{
	o_ePointingEnt = m_ePointingEntityType;
	o_ePointedEnt = m_ePointedEntityType;
}

/************************************************************************//**
 *	@brief		设置Proto Link
 *	@param[in]	i_wstrPlcPath			配置路径
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_NodeConnector::SetLinkProto(const wstring &i_wstrPlcPath)
{
	m_wstrPlcPath = i_wstrPlcPath;
	m_bIsLinkProto = TRUE;
}

/************************************************************************//**
 *	@brief		获取Proto Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrPlcPath	配置路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_NodeConnector::GetLinkProto()
{
	return m_wstrPlcPath;
}

/************************************************************************//**
 *	@brief		判断是否有Proto Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsLinkProto	配置路径状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_NodeConnector::IsLinkProto()
{
	return m_bIsLinkProto;
}

/************************************************************************//**
 *	@brief		设置Node Link
 *	@param[in]	i_idNode		Node ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_NodeConnector::SetLinkNode(STK_ID i_idNode)
{
	m_idNode = i_idNode;
	m_bIsLinkNode = TRUE;
}

/************************************************************************//**
 *	@brief		获取Node Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idNode		Node ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_NodeConnector::GetLinkNode()
{
	return m_idNode;
}

/************************************************************************//**
 *	@brief		判断是否有Node Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsLinkNode	Node Link状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_NodeConnector::IsLinkNode()
{
	return m_bIsLinkNode;
}

/************************************************************************//**
 *	@brief		设置子Entity
 *	@param[in]	i_SubEntity		子Entity
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_NodeConnector::AddSubEntity(SubEntity i_SubEntity)
{
	m_vcEntities.push_back(i_SubEntity);
	m_bIsLinkEntities = TRUE;
}

/************************************************************************//**
 *	@brief		获取子Entity
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcEntities	所有子Entity
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<SubEntity> Stk_NodeConnector::GetSubEntity()
{
	return m_vcEntities;
}

/************************************************************************//**
 *	@brief		判断是否有子Entity Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsLinkEntities	子Entity Link状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_NodeConnector::IsLinkEntities()
{
	return m_bIsLinkEntities;
}