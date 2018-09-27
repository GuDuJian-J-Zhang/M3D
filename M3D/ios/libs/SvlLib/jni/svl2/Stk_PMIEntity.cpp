// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_PMIEntity.cpp
 *
 *	@brief	PMI类
 *
 *	@par	历史:
 *		2013/08/19	创建。WangY@HOTEAMSOFT
 *		2013/10/10	更新。Gaoqq@HOTEAMSOFT
 *					Curve与Leader等从属对应的改善
 *		2014/04/24	更新。Gaoqq@HOTEAMSOFT
 *					支持Andriod平台
****************************************************************************/

#include "StdAfx.h"
#include "Stk_TopoEntity.h"
#include "Stk_Manager.h"
#include "Stk_Enum.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_PMI构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_PMI::Stk_PMI(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_idNode = 0;
	m_eType = PMI_TYPE_UNKNOWN;
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		{
			if (ii == jj)
			{
				m_mtxDefPlane.PlcMatrix[ii][jj] = 1.0;
			}
			else
			{
				m_mtxDefPlane.PlcMatrix[ii][jj] = 0.0;
			}
		}
	}
	m_rgbaPMI.Red = -1.0f;
	m_rgbaPMI.Green = -1.0f;
	m_rgbaPMI.Blue = -1.0f;
	m_rgbaPMI.Alpha = -1.0f;
	m_bIsDisplay = true;
	m_nuDim = 3; // 默认三维
	m_bIsParallelScreen = false;
	m_eMode = PMI_MODE_UNKNOWN;
	m_strPMIName = L"";
	m_bIsFix = false;
	m_ePMISource = PMI_SOURCE_CONVERT;
	m_NodeP = NULL;
	m_bIsOldVersion = true;
	m_szUserDataCnt = 0;
	m_bIsAdjusted = false;
	m_bIsFront = false;
}

/************************************************************************//**
 *	@brief		Stk_PMI析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		1.析构PMI时自动删除与所有View的关联 <br>
 *				2.析构PMI时自动删除Node节点，但不解除Node与父Node的关联
 *	@attention	无
 ****************************************************************************/

Stk_PMI::~Stk_PMI(void)
{
	//Leader
	for (int ii = 0; ii < m_vcLeaders.size(); ii++)
	{
		if (m_vcLeaders[ii] != NULL)
		{
			delete m_vcLeaders[ii];
			m_vcLeaders[ii] = NULL;	
		}
	}
	//ExtensionLine
	for (int ii = 0; ii < m_vcExLines.size(); ii++)
	{
		if(m_vcExLines[ii] != NULL){
			delete m_vcExLines[ii];
			m_vcExLines[ii] = NULL;
		}
	}
	//CompositeText
	for (int ii = 0; ii < m_vcComTexts.size(); ii++)
	{
		if (m_vcComTexts[ii] != NULL)
		{
			delete m_vcComTexts[ii];
			m_vcComTexts[ii] = NULL;
		}
	}
	//OutFrame
	for (int ii = 0; ii < m_vcOutFrames.size(); ii++)
	{
		if (m_vcOutFrames[ii] != NULL)
		{
			delete m_vcOutFrames[ii];
			m_vcOutFrames[ii] = NULL;
		}
	}
	//SpecialLine
	for (int ii = 0; ii < m_vcSpecialLines.size(); ii++)
	{
		if (m_vcSpecialLines[ii] != NULL)
		{
			delete m_vcSpecialLines[ii];
			m_vcSpecialLines[ii] = NULL;
		}
	}
#if 0
	//自定义属性
	for (int ii = 0; ii < m_vcMetaDatas.size(); ii++)
	{
		if (m_vcMetaDatas[ii] != NULL)
		{
			delete m_vcMetaDatas[ii];
			m_vcMetaDatas[ii] = NULL;
		}
	}
#endif
	//用户数据释放
	map<wstring,map<STK_ID,Stk_UserData*>*>::iterator itor;
	map<STK_ID,Stk_UserData*>::iterator UDitor;
	map<STK_ID,Stk_UserData*> *UDmap;

	itor = m_mapUserDatas.begin();
	while (itor != m_mapUserDatas.end()){
		UDmap = (itor->second);
		UDitor = UDmap->begin();
		while(UDitor != UDmap->end()){
			delete UDitor->second;
			UDitor++;
		}
		delete itor->second;
		itor++;
	}
	m_szUserDataCnt =0;

	// 删除View与PMI的关联
	if (m_stkProtoP && m_stkProtoP->GetTopNode())
	{
		vector<Stk_Node*>	vcNodes;
		m_stkProtoP->PreOrderNode(m_stkProtoP->GetTopNode(),NODE_TYPE_VIEW,vcNodes);
		for(STK_UINT32 i=0; i< vcNodes.size(); i++)
		{
			vcNodes[i]->GetView()->DelPMIConnector(m_idNode);
		}
	}

	// 图片(图章)
	for (int ii = 0; ii < m_vcPictures.size(); ii++)
	{
		if (m_vcPictures[ii] != NULL)
		{
			delete m_vcPictures[ii];
			m_vcPictures[ii] = NULL;
		}
	}

	// 删除所从属的Node节点
	if (m_NodeP!=NULL)
	{
		m_NodeP->SetPMI(NULL); //防止析构Node时再次析构PMI造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}

	m_stkProtoP->GetProtoIDManager()->DelPMIByID(m_ID);
}

/************************************************************************//**
 *	@brief		获取PMI ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID			PMI ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_PMI::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置PMI ID
 *	@param[in]	i_ID			PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置PMI名称
 *	@param[in]	i_strName		PMI名称
 *	@param[out]	无
 *	@retval		无
 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
 *	@attention	无
 ****************************************************************************/
void Stk_PMI::SetName(const wstring &i_strName)
{
	m_strPMIName = i_strName;
	if (m_NodeP != NULL)
	{
		m_NodeP->SetNodeName(i_strName);
	}
}

/************************************************************************//**
 *	@brief		获取PMI名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_strPMIName		PMI名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring	 Stk_PMI::GetName()
{
	return m_strPMIName;
}

/************************************************************************//**
 *	@brief		改变PMI名称
 *	@param[in]	i_strName		PMI名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		保持从Node到要素方向的名字一致
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PMI::ChangeName(const wstring &i_strName)
{
	m_strPMIName = i_strName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取组件 ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idProtoType	组件 ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_PMI::GetProtoTypeID()
{
	return m_stkProtoP->GetID();
}

/************************************************************************//**
 *	@brief		注册PMI ID
 *	@param[in]	i_idPMI			PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::RegisterID(STK_ID i_idPMI)
{
	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegPMIID(this, i_idPMI);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idPMI;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读取文件中的PMI数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_PMIHeadP			PMI记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PMI::ReadFile( Stk_File *i_FileP, StkSegmentHead * i_PMIHeadP)
{
	StkControl		**RecordPP = NULL;
	UINT			ii,jj;
	StkSegmentHead	Head={0};
	Stk_Leader		*StkLeaderP = NULL;
	Stk_Extensionline *StkExLineP = NULL;
	Stk_PolyLine	*StkPolyLineP = NULL;
	Stk_Ellipse		*StkEllipseP = NULL;
	Stk_Line		*StkLineP = NULL;
	Stk_ComText		*StkComTextP = NULL;
	Stk_Text		*StkTextP = NULL;
	Stk_OutFrame	*StkOutFrameP = NULL;
	Stk_SpecialLine	*StkSpecLineP = NULL;
	StkPMIBaseRec	*PMIBaseRecP = NULL;
	StkPMIBaseRec2	*PMIBaseRec2P = NULL;
	StkPMIBaseRec3	*PMIBaseRec3P = NULL;
	StkPMIBaseRec4	*PMIBaseRec4P = NULL;
	StkPMIBaseRec5	*PMIBaseRec5P = NULL;
	StkPMIBaseRec6	*PMIBaseRec6P = NULL;
	StkPMIBaseRec7	*PMIBaseRec7P = NULL;
	StkPMIBaseRec8	*PMIBaseRec8P = NULL;
	StkLeaderRec	*LeaderRecP = NULL;
	StkLeaderRec2	*LeaderRec2P = NULL;
	StkExLineRec	*ExLineRecP = NULL;
	StkExLineRec2	*ExLineRecP2 = NULL;
	StkPolyLineRec	*PolyLineRecP = NULL;
	StkPolyLineRec2	*PolyLineRec2P = NULL;
	StkPolyLineRec3	*PolyLineRec3P = NULL;
	StkEllipseRec	*EllipseRecP = NULL;
	StkEllipseRec2	*EllipseRec2P = NULL;
	StkEllipseRec3	*EllipseRec3P = NULL;
	StkEllipseRec4	*EllipseRec4P = NULL;
	StkComTextRec	*ComTextRecP = NULL;
	StkComTextRec2	*ComTextRec2P = NULL;
	StkComTextRec3	*ComTextRec3P = NULL;
	StkComTextRec4	*ComTextRec4P = NULL;
	StkTextRec		*TextRecP = NULL;
	StkTextRec2		*TextRecP2 = NULL;
	StkTextRec3		*TextRecP3 = NULL;
	StkTextRec4		*TextRecP4 = NULL;
	StkTextRec5		*TextRecP5 = NULL;
	StkFrameRec		*OutFrameRecP = NULL;
	StkFrameRec2	*OutFrameRecP2 = NULL;
	StkSpecLinRec	*SpecLineRecP = NULL;
	StkLineRec		*LineRecP = NULL;
	StkLineRec2		*LineRec2P = NULL;
	StkLineRec3		*LineRec3P = NULL;
	STK_UINT32		nuCurveBelongFlag = 0;
	STK_STATUS		eState;
	Stk_MetaData*	stkMetaDataP = NULL;
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	StkEnvelopeRec*	stkEnvelopeRecP = NULL;
	Stk_Envelope*	stkEnvelopeP = NULL;
	StkPictureRec*	stkPictureRecP = NULL;
	StkPictureRec2*	stkPictureRec2P = NULL;
	StkPictureRec3*	stkPictureRec3P = NULL;
	Stk_Picture*	stkPictureP = NULL;

	// 注册ID
	RegisterID( i_PMIHeadP->id );	//PMI的注册ID，放到ReadFile里面

	m_eMode = i_FileP->GetOptionOfPmiMode();

	/*
	 * 读取PMI段的数据
	 */
	RecordPP = (StkControl **)calloc(i_PMIHeadP->recnum, sizeof(StkControl*));
	if(i_PMIHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_PMIHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取PMI段的各记录
	 */
	for (ii = 0; ii < i_PMIHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_PMI)
		{
			/*
			 *	填充PMI基础部分
			 */
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				PMIBaseRecP = (StkPMIBaseRec *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRecP->Type;
				m_mtxDefPlane = PMIBaseRecP->DefPlane;
				m_rgbaPMI = PMIBaseRecP->Color;
				SetDisplay(true);
			}
			// 版本2
			else if (RecordPP[ii]->version == 2)
			{
				PMIBaseRec2P = (StkPMIBaseRec2 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec2P->Type;
				m_mtxDefPlane = PMIBaseRec2P->DefPlane;
				m_rgbaPMI = PMIBaseRec2P->Color;
				// 版本2
				if (!PMIBaseRec2P->IsItem & STK_PMI_IS_DISPLAY )
				{
					SetDisplay(false);
				}
			}
			// 版本3
			else if (RecordPP[ii]->version == 3)
			{
				PMIBaseRec3P = (StkPMIBaseRec3 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec3P->Type;
				m_mtxDefPlane = PMIBaseRec3P->DefPlane;
				m_rgbaPMI = PMIBaseRec3P->Color;
				// 版本2
				if (!((PMIBaseRec3P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec3P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec3P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
			}
			// 版本4
			else if (RecordPP[ii]->version == 4)
			{
				PMIBaseRec4P = (StkPMIBaseRec4 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec4P->Type;
				m_mtxDefPlane = PMIBaseRec4P->DefPlane;
				m_rgbaPMI = PMIBaseRec4P->Color;
				// 版本2
				if (!((PMIBaseRec4P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec4P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec4P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
				if ((PMIBaseRec4P->IsItem) & STK_PMI_IS_FIX)
				{
					SetFix(true);
				}
#ifdef __MOBILE__
				m_strPMIName = Stk_Platform::STKWCHARStrToWString(PMIBaseRec4P->name);
#else
				m_strPMIName = PMIBaseRec4P->name;
#endif
			}
			// 版本5
			else if (RecordPP[ii]->version == 5)
			{
				PMIBaseRec5P = (StkPMIBaseRec5 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec5P->Type;
				m_mtxDefPlane = PMIBaseRec5P->DefPlane;
				m_rgbaPMI = PMIBaseRec5P->Color;
				// 版本2
				if (!((PMIBaseRec5P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec5P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec5P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
				if ((PMIBaseRec5P->IsItem) & STK_PMI_IS_FIX)
				{
					SetFix(true);
				}
#ifdef __MOBILE__
				m_strPMIName = Stk_Platform::STKWCHARStrToWString(PMIBaseRec5P->name);
#else
				m_strPMIName = PMIBaseRec5P->name;
#endif
				m_ePMISource = (StkPMISourceEnum)PMIBaseRec5P->nuSource;
			}
			// 版本6
			else if (RecordPP[ii]->version == 6)
			{
				PMIBaseRec6P = (StkPMIBaseRec6 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec6P->Type;
				m_mtxDefPlane = PMIBaseRec6P->DefPlane;
				m_rgbaPMI = PMIBaseRec6P->Color;
				// 版本2
				if (!((PMIBaseRec6P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec6P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec6P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
				if ((PMIBaseRec6P->IsItem) & STK_PMI_IS_FIX)
				{
					SetFix(true);
				}
				if (!((PMIBaseRec6P->IsItem) & STK_PMI_IS_OLDVERSION))
				{
					SetOldVersion(false);
				}
#ifdef __MOBILE__
				m_strPMIName = Stk_Platform::STKWCHARStrToWString(PMIBaseRec6P->name);
#else
				m_strPMIName = PMIBaseRec6P->name;
#endif
				m_ePMISource = (StkPMISourceEnum)PMIBaseRec6P->nuSource;
				m_dirX = PMIBaseRec6P->dirX;
				m_dirY = PMIBaseRec6P->dirY;
				m_pntSymBase = PMIBaseRec6P->pntSymBase;
			}
			// 版本7
			else if (RecordPP[ii]->version == 7)
			{
				PMIBaseRec7P = (StkPMIBaseRec7 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec7P->Type;
				m_mtxDefPlane = PMIBaseRec7P->DefPlane;
				m_rgbaPMI = PMIBaseRec7P->Color;
				// 版本2
				if (!((PMIBaseRec7P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec7P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec7P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
				if ((PMIBaseRec7P->IsItem) & STK_PMI_IS_FIX)
				{
					SetFix(true);
				}
				if (!((PMIBaseRec7P->IsItem) & STK_PMI_IS_OLDVERSION))
				{
					SetOldVersion(false);
				}
				if ((PMIBaseRec7P->IsItem) & STK_PMI_IS_ADJUSTED)
				{
					SetAdjusted(true);
				}
#ifdef __MOBILE__
				m_strPMIName = Stk_Platform::STKWCHARStrToWString(PMIBaseRec7P->name);
#else
				m_strPMIName = PMIBaseRec7P->name;
#endif
				m_ePMISource = (StkPMISourceEnum)PMIBaseRec7P->nuSource;
				m_dirX = PMIBaseRec7P->dirX;
				m_dirY = PMIBaseRec7P->dirY;
				m_pntSymBase = PMIBaseRec7P->pntSymBase;
			}
			// 版本8
			else if (RecordPP[ii]->version == 8)
			{
				PMIBaseRec8P = (StkPMIBaseRec8 *)RecordPP[ii];
				m_eType = (StkPMITypeEnum)PMIBaseRec8P->Type;
				m_mtxDefPlane = PMIBaseRec8P->DefPlane;
				m_rgbaPMI = PMIBaseRec8P->Color;
				// 版本2
				if (!((PMIBaseRec8P->IsItem) & STK_PMI_IS_DISPLAY))
				{
					SetDisplay(false);
				}
				// 版本3
				if (!((PMIBaseRec8P->IsItem) & STK_PMI_IS_DIM3D))
				{
					SetDim(2);
				}
				if ((PMIBaseRec8P->IsItem) & STK_PMI_IS_PARALLEL)
				{
					SetParallelScreen(true);
				}
				if ((PMIBaseRec8P->IsItem) & STK_PMI_IS_FIX)
				{
					SetFix(true);
				}
				if (!((PMIBaseRec8P->IsItem) & STK_PMI_IS_OLDVERSION))
				{
					SetOldVersion(false);
				}
				if ((PMIBaseRec8P->IsItem) & STK_PMI_IS_ADJUSTED)
				{
					SetAdjusted(true);
				}
				if ((PMIBaseRec8P->IsItem) & STK_PMI_IS_FRONT)
				{
					SetFront(true);
				}
				m_ePMISource = (StkPMISourceEnum)PMIBaseRec8P->nuSource;
				m_dirX = PMIBaseRec8P->dirX;
				m_dirY = PMIBaseRec8P->dirY;
				m_pntSymBase = PMIBaseRec8P->pntSymBase;
			}
			else
			{
				//错误
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_ENTITY_NAME)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkEntityNameRecP = (StkEntityNameRec*)RecordPP[ii];
#ifdef __MOBILE__
				m_strPMIName = Stk_Platform::STKWCHARStrToWString(stkEntityNameRecP->atr); 
#else
				m_strPMIName = stkEntityNameRecP->atr; 
#endif
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_LEADER)
		{
			nuCurveBelongFlag = 1;
			if (RecordPP[ii]->version == 1)
			{
				LeaderRecP = (StkLeaderRec *)RecordPP[ii];
				StkLeaderP = new Stk_Leader();
				/*
				 *	填充Leader类
				 */
				StkLeaderP->SetLeaderType((StkLeaderTypeEnum)LeaderRecP->LeaderType);
				StkLeaderP->SetTerminator((StkTermTypeEnum)LeaderRecP->TermType,LeaderRecP->Size,LeaderRecP->TermLoc,LeaderRecP->TermDir);
			
				//建立Leader与PMI之间的关联
				m_vcLeaders.push_back(StkLeaderP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				LeaderRec2P = (StkLeaderRec2 *)RecordPP[ii];
				StkLeaderP = new Stk_Leader();
				/*
				 *	填充Leader类
				 */
				StkLeaderP->SetLeaderType((StkLeaderTypeEnum)LeaderRec2P->LeaderType);
				StkLeaderP->SetTerminator((StkTermTypeEnum)LeaderRec2P->TermType,LeaderRec2P->Size,LeaderRec2P->TermLoc,LeaderRec2P->TermDir);
				StkLeaderP->SetStubDelta(LeaderRec2P->nuStub);

				//建立Leader与PMI之间的关联
				m_vcLeaders.push_back(StkLeaderP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_EXLINE)
		{
			nuCurveBelongFlag = 2;
			if (RecordPP[ii]->version == 1)
			{
				ExLineRecP = (StkExLineRec *)RecordPP[ii];
				StkExLineP = new Stk_Extensionline();
				/*
				 *	填充ExtensionLine类
				 */
				StkExLineP->SetExLineType((StkExtensTypeEnum)ExLineRecP->ExLineType);

				//建立ExtensionLine与PMI之间的关联
				m_vcExLines.push_back(StkExLineP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				ExLineRecP2 = (StkExLineRec2 *)RecordPP[ii];
				StkExLineP = new Stk_Extensionline();

				if (!(ExLineRecP2->IsItem & STK_PMI_IS_DISPLAY) )
				{
					StkExLineP->SetDisplay(false);
				}
				
				/*
				 *	填充ExtensionLine类
				 */
				StkExLineP->SetExLineType((StkExtensTypeEnum)ExLineRecP2->ExLineType);

				//建立ExtensionLine与PMI之间的关联
				m_vcExLines.push_back(StkExLineP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_COMTEXT)
		{
			if (RecordPP[ii]->version == 1)
			{
				ComTextRecP = (StkComTextRec *)RecordPP[ii];
				StkComTextP = new Stk_ComText();
				/*
				 *	填充CompositeText类
				 */
				StkComTextP->SetOuterBox(ComTextRecP->outBox);

				//建立CompositeText与PMI之间的关联
				m_vcComTexts.push_back(StkComTextP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				ComTextRec2P = (StkComTextRec2 *)RecordPP[ii];
				StkComTextP = new Stk_ComText();
				/*
				 *	填充CompositeText类
				 */
				StkComTextP->SetOuterBox(ComTextRec2P->outBox);
				StkComTextP->SetFrameType((StkFrameTypeEnum)ComTextRec2P->nuFrameType);

				//建立CompositeText与PMI之间的关联
				m_vcComTexts.push_back(StkComTextP);
			}
			else if (RecordPP[ii]->version == 3)
			{
				ComTextRec3P = (StkComTextRec3 *)RecordPP[ii];
				StkComTextP = new Stk_ComText();
				/*
				 *	填充CompositeText类
				 */
				StkComTextP->SetOuterBox(ComTextRec3P->outBox);
				StkComTextP->SetFrameType((StkFrameTypeEnum)ComTextRec3P->nuFrameType);
				
				// 包络框记录不再使用此处
				//if (ComTextRec3P->IsItem & STK_HAS_ENVELOPE)
				//{
				//	StkComTextP->SetEnvelopeFlag(true);
				//}
				//StkComTextP->SetBackgroundColor(ComTextRec3P->rgbaBackground);

				//for(int jj = 0; jj < ComTextRec3P->nuAnchorPointNum; jj++)
				//{
				//	StkComTextP->SetAnchorPoint((StkAnchorTypeEnum)ComTextRec3P->atr[jj].nuAnchorType, ComTextRec3P->atr[jj].pntAnchor);
				//}

				//建立CompositeText与PMI之间的关联
				m_vcComTexts.push_back(StkComTextP);
			}
			else if (RecordPP[ii]->version == 4)
			{
				ComTextRec4P = (StkComTextRec4 *)RecordPP[ii];
				StkComTextP = new Stk_ComText();
				/*
				 *	填充CompositeText类
				 */
				StkComTextP->SetOuterBox(ComTextRec4P->outBox);
				StkComTextP->SetFrameType((StkFrameTypeEnum)ComTextRec4P->nuFrameType);

				//建立CompositeText与PMI之间的关联
				m_vcComTexts.push_back(StkComTextP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_TEXT)
		{
			if (RecordPP[ii]->version == 1)
			{
				TextRecP = (StkTextRec *)RecordPP[ii];
				StkTextP = new Stk_Text();
				/*
				 *	填充Text类
				 */
				StkTextP->SetInnerLoc(TextRecP->InnerLoc);
				StkTextP->SetInnerXYAxis(TextRecP->Axis[0], TextRecP->Axis[1]);
				StkTextP->SetCharWidthHeight(TextRecP->Size[0], TextRecP->Size[1]);
				StkTextP->SetCharSpacing(TextRecP->CharDis);
#ifdef __MOBILE__
				StkTextP->SetFonts(Stk_Platform::STKWCHARStrToWString(TextRecP->font));
				StkTextP->SetText(Stk_Platform::STKWCHARStrToWString(TextRecP->atr));
#else
				//StkTextP->SetFont(TextRecP->font);
				StkTextP->SetFonts(TextRecP->font);
				StkTextP->SetText(TextRecP->atr);
#endif
				//建立Text与CompositeText之间的关联
				vector<Stk_Text*> TmpTexts = StkComTextP->GetTexts();
				TmpTexts.push_back(StkTextP);
				StkComTextP->SetTexts(TmpTexts);
			}
			else if (RecordPP[ii]->version == 2)
			{
				TextRecP2 = (StkTextRec2 *)RecordPP[ii];
				StkTextP = new Stk_Text();
				/*
				 *	填充Text类
				 */
				StkTextP->SetInnerLoc(TextRecP2->InnerLoc);
				StkTextP->SetInnerXYAxis(TextRecP2->Axis[0], TextRecP2->Axis[1]);
				StkTextP->SetCharWidthHeight(TextRecP2->Size[0], TextRecP2->Size[1]);
				StkTextP->SetCharSpacing(TextRecP2->CharDis);
				StkTextP->SetUsageType((StkTextTypeEnum)TextRecP2->usetype);
#ifdef __MOBILE__
				StkTextP->SetFonts(Stk_Platform::STKWCHARStrToWString(TextRecP2->font));
				StkTextP->SetText(Stk_Platform::STKWCHARStrToWString(TextRecP2->atr));
#else
				//StkTextP->SetFont(TextRecP2->font);
				StkTextP->SetFonts(TextRecP2->font);
				StkTextP->SetText(TextRecP2->atr);
#endif
				//建立Text与CompositeText之间的关联
				vector<Stk_Text*> TmpTexts = StkComTextP->GetTexts();
				TmpTexts.push_back(StkTextP);
				StkComTextP->SetTexts(TmpTexts);
			}
			else if (RecordPP[ii]->version == 3)
			{
				TextRecP3 = (StkTextRec3 *)RecordPP[ii];
				StkTextP = new Stk_Text();
				/*
				 *	填充Text类
				 */
				StkTextP->SetInnerLoc(TextRecP3->InnerLoc);
				StkTextP->SetInnerXYAxis(TextRecP3->Axis[0], TextRecP3->Axis[1]);
				StkTextP->SetCharWidthHeight(TextRecP3->Size[0], TextRecP3->Size[1]);
				StkTextP->SetCharSpacing(TextRecP3->CharDis);
				StkTextP->SetUsageType((StkTextTypeEnum)TextRecP3->usetype);
#ifdef __MOBILE__
				StkTextP->SetFonts(Stk_Platform::STKWCHARStrToWString(TextRecP3->font));
				StkTextP->SetText(Stk_Platform::STKWCHARStrToWString(TextRecP3->atr));
#else
				//StkTextP->SetFont(TextRecP3->font);
				StkTextP->SetFonts(TextRecP3->font);
				StkTextP->SetText(TextRecP3->atr);
#endif
				StkTextP->SetOuterBox(TextRecP3->outBox);

				//建立Text与CompositeText之间的关联
				vector<Stk_Text*> TmpTexts = StkComTextP->GetTexts();
				TmpTexts.push_back(StkTextP);
				StkComTextP->SetTexts(TmpTexts);
			}
			else if (RecordPP[ii]->version == 4)
			{
				TextRecP4 = (StkTextRec4 *)RecordPP[ii];
				StkTextP = new Stk_Text();
				/*
				 *	填充Text类
				 */
				StkTextP->SetInnerLoc(TextRecP4->InnerLoc);
				StkTextP->SetInnerXYAxis(TextRecP4->Axis[0], TextRecP4->Axis[1]);
				StkTextP->SetCharWidthHeight(TextRecP4->Size[0], TextRecP4->Size[1]);
				StkTextP->SetCharSpacing(TextRecP4->CharDis);
				StkTextP->SetUsageType((StkTextTypeEnum)TextRecP4->usetype);
#ifdef __MOBILE__
				StkTextP->SetFonts(Stk_Platform::STKWCHARStrToWString(TextRecP4->font));
				StkTextP->SetText(Stk_Platform::STKWCHARStrToWString(TextRecP4->atr));
#else
				//StkTextP->SetFont(TextRecP4->font);
				StkTextP->SetFonts(TextRecP4->font);
				StkTextP->SetText(TextRecP4->atr);
#endif
				StkTextP->SetOuterBox(TextRecP4->outBox);
				StkTextP->SetRotationAngle(TextRecP4->dAngle);
				
				//建立Text与CompositeText之间的关联
				vector<Stk_Text*> TmpTexts = StkComTextP->GetTexts();
				TmpTexts.push_back(StkTextP);
				StkComTextP->SetTexts(TmpTexts);
			}
			else if (RecordPP[ii]->version == 5)
			{
				TextRecP5 = (StkTextRec5 *)RecordPP[ii];
				StkTextP = new Stk_Text();
				/*
				 *	填充Text类
				 */
				StkTextP->SetInnerLoc(TextRecP5->InnerLoc);
				StkTextP->SetInnerXYAxis(TextRecP5->Axis[0], TextRecP5->Axis[1]);
				StkTextP->SetCharWidthHeight(TextRecP5->Size[0], TextRecP5->Size[1]);
				StkTextP->SetCharSpacing(TextRecP5->CharDis);
				StkTextP->SetUsageType((StkTextTypeEnum)TextRecP5->usetype);
#ifdef __MOBILE__
				StkTextP->SetFonts(Stk_Platform::STKWCHARStrToWString(TextRecP5->font));
				StkTextP->SetText(Stk_Platform::STKWCHARStrToWString(TextRecP5->atr));
#else
				//StkTextP->SetFont(TextRecP5->font);
				StkTextP->SetFonts(TextRecP5->font);
				StkTextP->SetText(TextRecP5->atr);
#endif
				StkTextP->SetOuterBox(TextRecP5->outBox);
				StkTextP->SetRotationAngle(TextRecP5->dAngle);
				StkTextP->SetFontStyle((StkFontStyleTypeEnum)TextRecP5->nuFontStyle);
				StkTextP->SetFontType((StkFontTypeEnum)TextRecP5->nuFontType);
				StkTextP->SetAttributeType((StkTextAttributeTypeEnum)TextRecP5->nuTextAttribute);
				
				//建立Text与CompositeText之间的关联
				vector<Stk_Text*> TmpTexts = StkComTextP->GetTexts();
				TmpTexts.push_back(StkTextP);
				StkComTextP->SetTexts(TmpTexts);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_ENVELOPE)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkEnvelopeRecP = (StkEnvelopeRec*)RecordPP[ii];
				stkEnvelopeP = new Stk_Envelope();
				/*
				 *	填充包络框类
				 */
				if (stkEnvelopeRecP->IsItem & STK_HAS_ENVELOPE_FRAME)
				{
					stkEnvelopeP->SetFrameFlag(true);
				}
				stkEnvelopeP->SetBackgroundColor(stkEnvelopeRecP->rgbaBackground);
				stkEnvelopeP->SetFrameColor(stkEnvelopeRecP->rgbaFrame);
				for(int jj = 0; jj < stkEnvelopeRecP->nuAnchorPointNum; jj++)
				{
					stkEnvelopeP->SetAnchorPoint((StkAnchorTypeEnum)stkEnvelopeRecP->atr[jj].nuAnchorType, stkEnvelopeRecP->atr[jj].pntAnchor);
				}
				StkComTextP->SetEnvelope(stkEnvelopeP);
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_OUTFRAME)
		{
			nuCurveBelongFlag = 3;
			if (RecordPP[ii]->version == 1)
			{
				OutFrameRecP = (StkFrameRec *)RecordPP[ii];
				StkOutFrameP = new Stk_OutFrame();
				/*
				 *	填充OutFrame类
				 */
				StkOutFrameP->SetFrameType((StkOutFrameUsageEnum)OutFrameRecP->FrameType);
				//建立OutFrame与PMI之间的关联
				m_vcOutFrames.push_back(StkOutFrameP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				OutFrameRecP2 = (StkFrameRec2 *)RecordPP[ii];
				StkOutFrameP = new Stk_OutFrame();
				/*
				 *	填充OutFrame类
				 */
				StkOutFrameP->SetFrameType((StkOutFrameUsageEnum)OutFrameRecP2->FrameType);
				StkOutFrameP->SetDefineType((StkOutFrameDefEnum)OutFrameRecP2->defType);
				if (StkOutFrameP->GetDefineType() == STK_FRAME_DEF_PMI)
				{
					//建立OutFrame与PMI之间的关联
					m_vcOutFrames.push_back(StkOutFrameP);
				}
				else if (StkOutFrameP->GetDefineType() == STK_FRAME_DEF_TEXT)
				{
					//建立OutFrame与Stk_Text之间的关联
					StkTextP->AddOutFrame(StkOutFrameP);
				}
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_ANCHIOR)
		{
			// 预留包围框


		}
		else if(RecordPP[ii]->type == REC_TYPE_SPECIALLINE)
		{
			nuCurveBelongFlag = 4;
			if (RecordPP[ii]->version == 1)
			{
				SpecLineRecP = (StkSpecLinRec *)RecordPP[ii];
				StkSpecLineP = new Stk_SpecialLine();

				/*
				 *	填充SpecialLine类
				 */
				StkSpecLineP->SetSpecLinUse((StkSpecLineUsageEnum)SpecLineRecP->SpecLineUse);
				//建立SpecialLine与PMI之间的关联
				m_vcSpecialLines.push_back(StkSpecLineP);
			}
		}
		// 折线
		else if (RecordPP[ii]->type == REC_TYPE_POLYLINE)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				PolyLineRecP = (StkPolyLineRec *)RecordPP[ii];
				StkPolyLineP = new Stk_PolyLine();
				/*
				 *	填充PolyLine类
				 */
				StkPolyLineP->SetID(PolyLineRecP->ID);	//仅用于单独线条时
				StkPolyLineP->SetColor(PolyLineRecP->Color);

				vector<STK_PNT32> vcLinearPoints;
				STK_PNT32 pntTmp;
				for (jj = 0; jj < PolyLineRecP->NumPoints; jj++)
				{
					pntTmp.X = PolyLineRecP->atr[jj*3 + 0];
					pntTmp.Y = PolyLineRecP->atr[jj*3 + 1];
					pntTmp.Z = PolyLineRecP->atr[jj*3 + 2];
					vcLinearPoints.push_back(pntTmp);
				}
				StkPolyLineP->SetPoints(vcLinearPoints);
			}
			// 版本2
			else if (RecordPP[ii]->version == 2)
			{
				PolyLineRec2P = (StkPolyLineRec2 *)RecordPP[ii];
				StkPolyLineP = new Stk_PolyLine();
				/*
				 *	填充PolyLine类
				 */
				StkPolyLineP->SetID(PolyLineRec2P->ID);	//仅用于单独线条时
				StkPolyLineP->SetColor(PolyLineRec2P->Color);
				StkPolyLineP->SetUsageType((StkCurveUsageEnum)PolyLineRec2P->DefType);

				vector<STK_PNT32> vcLinearPoints;
				STK_PNT32 pntTmp;
				for (jj = 0; jj < PolyLineRec2P->NumPoints; jj++)
				{
					pntTmp.X = PolyLineRec2P->atr[jj*3 + 0];
					pntTmp.Y = PolyLineRec2P->atr[jj*3 + 1];
					pntTmp.Z = PolyLineRec2P->atr[jj*3 + 2];
					vcLinearPoints.push_back(pntTmp);
				}
				StkPolyLineP->SetPoints(vcLinearPoints);
			}
			// 版本3
			else if (RecordPP[ii]->version == 3)
			{
				PolyLineRec3P = (StkPolyLineRec3 *)RecordPP[ii];
				StkPolyLineP = new Stk_PolyLine();
				/*
				 *	填充PolyLine类
				 */
				StkPolyLineP->SetID(PolyLineRec3P->ID);	//仅用于单独线条时
				StkPolyLineP->SetColor(PolyLineRec3P->Color);
				StkPolyLineP->SetUsageType((StkCurveUsageEnum)PolyLineRec3P->DefType);

				vector<STK_PNT32> vcLinearPoints;
				STK_PNT32 pntTmp;
				for (jj = 0; jj < PolyLineRec3P->NumPoints; jj++)
				{
					pntTmp.X = PolyLineRec3P->atr[jj*3 + 0];
					pntTmp.Y = PolyLineRec3P->atr[jj*3 + 1];
					pntTmp.Z = PolyLineRec3P->atr[jj*3 + 2];
					vcLinearPoints.push_back(pntTmp);
				}
				StkPolyLineP->SetPoints(vcLinearPoints);
			}

			switch (nuCurveBelongFlag)
			{
			case 1:
				StkLeaderP->AddCurve((Stk_CurveEntity *)StkPolyLineP);
				break;
			case 2:
				StkExLineP->SetCurve((Stk_CurveEntity *)StkPolyLineP);
				break;
			case 3:
				StkOutFrameP->SetFrameData((Stk_CurveEntity *)StkPolyLineP);
				break;
			case 4:
				StkSpecLineP->AddCurve((Stk_CurveEntity *)StkPolyLineP);
				break;
			default:
				// 错误
				break;
			}

		}
		// 弧线
		else if (RecordPP[ii]->type == REC_TYPE_ELLIPSE)
		{
			if (RecordPP[ii]->version == 1)
			{
				EllipseRecP = (StkEllipseRec *)RecordPP[ii];
				StkEllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				StkEllipseP->SetID(EllipseRecP->ID);	//仅用于单独线条时
				StkEllipseP->SetColor(EllipseRecP->Color);
				StkEllipseP->SetDomain(EllipseRecP->CMin, EllipseRecP->CMax);
				StkEllipseP->SetCenterPoint(EllipseRecP->Center);
				StkEllipseP->SetXYZDir(EllipseRecP->XDir, EllipseRecP->YDir, EllipseRecP->ZDir);
				StkEllipseP->SetRadius(EllipseRecP->MajorRadius, EllipseRecP->MinorRadius);
				StkEllipseP->SetCoordiantePnt(EllipseRecP->BeginPnt, EllipseRecP->EndPnt);
				StkEllipseP->SetParameter(EllipseRecP->BeginPar, EllipseRecP->EndPar);

			}
			else if (RecordPP[ii]->version == 2)
			{
				EllipseRec2P = (StkEllipseRec2 *)RecordPP[ii];
				StkEllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				StkEllipseP->SetID(EllipseRec2P->ID);	//仅用于单独线条时
				StkEllipseP->SetColor(EllipseRec2P->Color);
				StkEllipseP->SetDomain(EllipseRec2P->CMin, EllipseRec2P->CMax);
				StkEllipseP->SetCenterPoint(EllipseRec2P->Center);
				StkEllipseP->SetXYZDir(EllipseRec2P->XDir, EllipseRec2P->YDir, EllipseRec2P->ZDir);
				StkEllipseP->SetRadius(EllipseRec2P->MajorRadius, EllipseRec2P->MinorRadius);
				StkEllipseP->SetCoordiantePnt(EllipseRec2P->BeginPnt, EllipseRec2P->EndPnt);
				StkEllipseP->SetParameter(EllipseRec2P->BeginPar, EllipseRec2P->EndPar);
				//版本2
				StkEllipseP->SetNormal(EllipseRec2P->Normal);
				StkEllipseP->SetOriginDir(EllipseRec2P->OriginDir);
			}
			else if (RecordPP[ii]->version == 3)
			{
				EllipseRec3P = (StkEllipseRec3 *)RecordPP[ii];
				StkEllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				StkEllipseP->SetID(EllipseRec3P->ID);	//仅用于单独线条时
				StkEllipseP->SetColor(EllipseRec3P->Color);
				StkEllipseP->SetDomain(EllipseRec3P->CMin, EllipseRec3P->CMax);
				StkEllipseP->SetCenterPoint(EllipseRec3P->Center);
				StkEllipseP->SetXYZDir(EllipseRec3P->XDir, EllipseRec3P->YDir, EllipseRec3P->ZDir);
				StkEllipseP->SetRadius(EllipseRec3P->MajorRadius, EllipseRec3P->MinorRadius);
				StkEllipseP->SetCoordiantePnt(EllipseRec3P->BeginPnt, EllipseRec3P->EndPnt);
				StkEllipseP->SetParameter(EllipseRec3P->BeginPar, EllipseRec3P->EndPar);
				//版本2
				StkEllipseP->SetNormal(EllipseRec3P->Normal);
				StkEllipseP->SetOriginDir(EllipseRec3P->OriginDir);
				//版本3
				StkEllipseP->SetUsageType((StkCurveUsageEnum)EllipseRec3P->DefType);
			}
			else if (RecordPP[ii]->version == 4)
			{
				EllipseRec4P = (StkEllipseRec4 *)RecordPP[ii];
				StkEllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				StkEllipseP->SetID(EllipseRec4P->ID);	//仅用于单独线条时
				StkEllipseP->SetColor(EllipseRec4P->Color);
				StkEllipseP->SetDomain(EllipseRec4P->CMin, EllipseRec4P->CMax);
				StkEllipseP->SetCenterPoint(EllipseRec4P->Center);
				StkEllipseP->SetXYZDir(EllipseRec4P->XDir, EllipseRec4P->YDir, EllipseRec4P->ZDir);
				StkEllipseP->SetRadius(EllipseRec4P->MajorRadius, EllipseRec4P->MinorRadius);
				StkEllipseP->SetCoordiantePnt(EllipseRec4P->BeginPnt, EllipseRec4P->EndPnt);
				StkEllipseP->SetParameter(EllipseRec4P->BeginPar, EllipseRec4P->EndPar);
				//版本2
				StkEllipseP->SetNormal(EllipseRec4P->Normal);
				StkEllipseP->SetOriginDir(EllipseRec4P->OriginDir);
				//版本3
				StkEllipseP->SetUsageType((StkCurveUsageEnum)EllipseRec4P->DefType);
			}
			else
			{
				//###错误
			}

			switch (nuCurveBelongFlag)
			{
			case 1:
				StkLeaderP->AddCurve((Stk_CurveEntity *)StkEllipseP);
				break;
			case 2:
				StkExLineP->SetCurve((Stk_CurveEntity *)StkEllipseP);
				break;
			case 3:
				StkOutFrameP->SetFrameData((Stk_CurveEntity *)StkEllipseP);
				break;
			case 4:
				StkSpecLineP->AddCurve((Stk_CurveEntity *)StkEllipseP);
				break;
			default:
				// 错误
				break;
			}

		}
		// 直线
		else if (RecordPP[ii]->type == REC_TYPE_LINE)
		{

			if (RecordPP[ii]->version == 1)
			{
				LineRecP = (StkLineRec *)RecordPP[ii];
				StkLineP = new Stk_Line();

				StkLineP->SetID( LineRecP->ID);
				StkLineP->SetColor( LineRecP->Color);
				StkLineP->SetStartPoint( LineRecP->StartPnt);
				StkLineP->SetEndPoint( LineRecP->EndPnt);
			}
			else if (RecordPP[ii]->version == 2)
			{
				LineRec2P = (StkLineRec2 *)RecordPP[ii];
				StkLineP = new Stk_Line();

				StkLineP->SetID( LineRec2P->ID);
				StkLineP->SetColor( LineRec2P->Color);
				StkLineP->SetStartPoint( LineRec2P->StartPnt);
				StkLineP->SetEndPoint( LineRec2P->EndPnt);
				// 版本2
				StkLineP->SetUsageType((StkCurveUsageEnum)LineRec2P->DefType);
			}
			else if (RecordPP[ii]->version == 3)
			{
				LineRec3P = (StkLineRec3 *)RecordPP[ii];
				StkLineP = new Stk_Line();

				StkLineP->SetID( LineRec3P->ID);
				StkLineP->SetColor( LineRec3P->Color);
				StkLineP->SetStartPoint( LineRec3P->StartPnt);
				StkLineP->SetEndPoint( LineRec3P->EndPnt);
				// 版本2
				StkLineP->SetUsageType((StkCurveUsageEnum)LineRec3P->DefType);
			}


			switch (nuCurveBelongFlag)
			{
			case 1:
				StkLeaderP->AddCurve((Stk_CurveEntity *)StkLineP);
				break;
			case 2:
				StkExLineP->SetCurve((Stk_CurveEntity *)StkLineP);
				break;
			case 3:
				StkOutFrameP->SetFrameData((Stk_CurveEntity *)StkLineP);
				break;
			case 4:
				StkSpecLineP->AddCurve((Stk_CurveEntity *)StkLineP);
				break;
			default:
				// 错误
				break;
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_PICTURE)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkPictureRecP = (StkPictureRec*)RecordPP[ii];
				stkPictureP = new Stk_Picture();
				stkPictureP->SetType((StkPictureTypeEnum)stkPictureRecP->nuType);
				stkPictureP->SetLocation(stkPictureRecP->pntLocation);
				stkPictureP->SetDisplayWidth(stkPictureRecP->fWidth);
				stkPictureP->SetDisplayHeight(stkPictureRecP->fHeight);
				stkPictureP->SetPictureSaveStyle((StkPicSaveStyleEnum)stkPictureRecP->nuSaveStyle);
#ifdef __MOBILE__
				stkPictureP->SetPictureSavePath(Stk_Platform::STKWCHARStrToWString(stkPictureRecP->wchPicturePath));
#else
				stkPictureP->SetPictureSavePath(stkPictureRecP->wchPicturePath);
#endif
				stkPictureP->SetData(stkPictureRecP->nudataSize, stkPictureRecP->atr);
				m_vcPictures.push_back(stkPictureP);
			}
			else if (RecordPP[ii]->version == 2)
			{
				stkPictureRec2P = (StkPictureRec2*)RecordPP[ii];
				stkPictureP = new Stk_Picture();
				stkPictureP->SetType((StkPictureTypeEnum)stkPictureRec2P->nuType);
				stkPictureP->SetLocation(stkPictureRec2P->pntLocation);
				stkPictureP->SetDisplayWidth(stkPictureRec2P->fDispalyWidth);
				stkPictureP->SetDisplayHeight(stkPictureRec2P->fDispalyHeight);
				stkPictureP->SetActualWidth(stkPictureRec2P->fActualWidth);
				stkPictureP->SetActualHeight(stkPictureRec2P->fActualHeight);
				stkPictureP->SetPictureSaveStyle((StkPicSaveStyleEnum)stkPictureRec2P->nuSaveStyle);
				stkPictureP->SetNC(stkPictureRec2P->nNC);
#ifdef __MOBILE__
				stkPictureP->SetPictureSavePath(Stk_Platform::STKWCHARStrToWString(stkPictureRec2P->wchPicturePath));
#else
				stkPictureP->SetPictureSavePath(stkPictureRec2P->wchPicturePath);
#endif
				stkPictureP->SetData(stkPictureRec2P->nudataSize, stkPictureRec2P->atr);
				m_vcPictures.push_back(stkPictureP);
			}
			else if (RecordPP[ii]->version == 3)
			{
				stkPictureRec3P = (StkPictureRec3*)RecordPP[ii];
				stkPictureP = new Stk_Picture();
				stkPictureP->SetType((StkPictureTypeEnum)stkPictureRec3P->nuType);
				stkPictureP->SetLocation(stkPictureRec3P->pntLocation);
				stkPictureP->SetDisplayWidth(stkPictureRec3P->fDispalyWidth);
				stkPictureP->SetDisplayHeight(stkPictureRec3P->fDispalyHeight);
				stkPictureP->SetActualWidth(stkPictureRec3P->fActualWidth);
				stkPictureP->SetActualHeight(stkPictureRec3P->fActualHeight);
				stkPictureP->SetPictureSaveStyle((StkPicSaveStyleEnum)stkPictureRec3P->nuSaveStyle);
				stkPictureP->SetNC(stkPictureRec3P->nNC);
				stkPictureP->SetScaleX(stkPictureRec3P->fScaleX);
				stkPictureP->SetScaleY(stkPictureRec3P->fScaleY);
#ifdef __MOBILE__
				stkPictureP->SetPictureSavePath(Stk_Platform::STKWCHARStrToWString(stkPictureRec3P->wchPicturePath));
#else
				stkPictureP->SetPictureSavePath(stkPictureRec3P->wchPicturePath);
#endif
				stkPictureP->SetData(stkPictureRec3P->nudataSize, stkPictureRec3P->atr);
				m_vcPictures.push_back(stkPictureP);
			}
		}
		else
		{
			
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_PMIHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 * 读取下一段的数据
	 */
	while (1)
	{
		eState = i_FileP->ReadSegmentHeadData( &Head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		switch (Head.kind)
		{
#if 0
		case SEG_TYPE_METADATA:
			ReadMetaDatas(i_FileP, &Head);
			break;
#endif
		case SEG_TYPE_USERDEFDATA:
			eState = ReadUserDatas(i_FileP, &Head);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			break;
		// 当下一段是PMI-END表时
		case SEG_TYPE_PMI_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(Head.recnum, sizeof(StkControl*));
			if (Head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_FileP->ReadSegmentAllRecordData(&Head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = i_FileP->ReadSegmentFinishData(&Head,(StkControl **)TmpRecordPP);// 释放内存
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

rtn:
	return STK_SUCCESS;

}

/*************************************************************************//**
 *	@brief		写PMI数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PMI::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		PMIHead = {0};
	StkSegmentHead		endHead = {0};
	STK_STATUS			ret = STK_SUCCESS;
	STK_SIZE_T			szMetaDataNum = 0;
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	STK_UINT32			nuEntityNameSize = 0;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
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
#endif

	/*
	 *	【第一步】 输出PMI段
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【PMI段】输出 PMI ID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);

	}
#endif

	// 【1.1】 输出PMI的Table HEAD
	PMIHead.id = m_ID;
	PMIHead.kind = SEG_TYPE_PMI;

	ret = i_stkFileP->WriteSegmentHeadData(&PMIHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PMI段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}
	// 【1.2】 输出PMI的PMI Record
	StkPMIBaseRec8 *PMIBaseRec8P = NULL;
	PMIBaseRec8P = (StkPMIBaseRec8 *)calloc(1, sizeof(StkPMIBaseRec8));
	if(PMIBaseRec8P == NULL)
	{
		return STK_ERROR;
	}
	PMIBaseRec8P->DefPlane = m_mtxDefPlane;
	PMIBaseRec8P->Type = (STK_UINT32)m_eType;
	PMIBaseRec8P->Color = m_rgbaPMI;
	PMIBaseRec8P->NumLeader = (STK_UINT32)m_vcLeaders.size();
	PMIBaseRec8P->NumExLine = (STK_UINT32)m_vcExLines.size();
	PMIBaseRec8P->Control.type = REC_TYPE_PMI;
	PMIBaseRec8P->Control.length = sizeof(StkPMIBaseRec8) - sizeof(StkControl);
	PMIBaseRec8P->Control.version = _VER_PMI_RECORD /*8*/;

	PMIBaseRec8P->IsItem = 0;
	if( m_bIsDisplay)
		PMIBaseRec8P->IsItem |= STK_PMI_IS_DISPLAY; 
	if( m_nuDim == 3)
		PMIBaseRec8P->IsItem |= STK_PMI_IS_DIM3D;
	if( m_bIsParallelScreen)
		PMIBaseRec8P->IsItem |= STK_PMI_IS_PARALLEL;
	if( m_bIsFix)
		PMIBaseRec8P->IsItem |= STK_PMI_IS_FIX;
	if (m_bIsOldVersion)
	{
		PMIBaseRec8P->IsItem |= STK_PMI_IS_OLDVERSION;
	}
	if (m_bIsAdjusted)
	{
		PMIBaseRec8P->IsItem |= STK_PMI_IS_ADJUSTED;
	}
	if (m_bIsFront)
	{
		PMIBaseRec8P->IsItem |= STK_PMI_IS_FRONT;
	}
	// 预留
	PMIBaseRec8P->usetype = 0;
	PMIBaseRec8P->dmy1 = 0;
	PMIBaseRec8P->dmy2 = 0;
	PMIBaseRec8P->nuSource = m_ePMISource;


	ret = i_stkFileP->WriteSegmentRecordData(&PMIHead, (StkControl *)PMIBaseRec8P);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PMI段 Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(PMIBaseRec8P != NULL)
	{
		free(PMIBaseRec8P);
		PMIBaseRec8P = NULL;
	}
	// 【1.3】 输出PMI的名字
	nuEntityNameSize = m_strPMIName.size() + 1;
	stkEntityNameRecP = (StkEntityNameRec*)calloc(1, sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR));
	if(stkEntityNameRecP == NULL)
	{
		return STK_ERROR;
	}
	stkEntityNameRecP->Control.type = REC_TYPE_ENTITY_NAME;
	stkEntityNameRecP->Control.length = sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR) - sizeof(StkControl);
	stkEntityNameRecP->Control.version = _VER_ENTITY_NAME_RECORD;

	stkEntityNameRecP->nuNameSize = nuEntityNameSize;
#ifdef __MOBILE__
	memcpy(stkEntityNameRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_strPMIName),(m_strPMIName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkEntityNameRecP->atr, nuEntityNameSize, (WCHAR*)m_strPMIName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(&PMIHead, (StkControl*)stkEntityNameRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (stkEntityNameRecP != NULL)
	{
		free(stkEntityNameRecP);
		stkEntityNameRecP = NULL;
	}

	// 【1.4】 输出Leader Record
	for (int ii = 0; ii < m_vcLeaders.size(); ii++)
	{
		m_vcLeaders[ii]->WriteLeader(&PMIHead, i_stkFileP);

		vector<Stk_CurveEntity*> tmpCurves = m_vcLeaders[ii]->GetAllCurve();
		for(int jj =0 ; jj < tmpCurves.size(); jj++){
			switch(tmpCurves[jj]->GetCurveType())
			{
			case CURVE_TYPE_POLYLINE:
				((Stk_PolyLine *)tmpCurves[jj])->WritePolyline(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_ELLIPSE:
				((Stk_Ellipse *)tmpCurves[jj])->WriteEllipse(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_LINE:
				((Stk_Line *)tmpCurves[jj])->WriteLine(&PMIHead, i_stkFileP);
				break;
			default:
				break;
			}
		}
	}
	// 【1.5】 输出ExtensionLine Record
	for(int ii = 0; ii < m_vcExLines.size(); ii++)
	{
		m_vcExLines[ii]->WriteExLine(&PMIHead, i_stkFileP);
		/*
		 *	输出Linear Record或者Angular Record
		 */
		switch(m_vcExLines[ii]->GetExLineType())
		{
		case EXTENS_TYPE_LENGTH:
			((Stk_PolyLine *)m_vcExLines[ii]->GetCurve())->WritePolyline(&PMIHead, i_stkFileP);
			break;
		case EXTENS_TYPE_ANGULAR:
			((Stk_Ellipse *)m_vcExLines[ii]->GetCurve())->WriteEllipse(&PMIHead, i_stkFileP);
			break;
		default:
			break;
		}
	}
	// 【1.6】 输出CompositeText Record
	for(int ii = 0; ii < m_vcComTexts.size(); ii++)
	{
		m_vcComTexts[ii]->WriteComText(&PMIHead, i_stkFileP);

		vector<Stk_Text*> vcTmpTexts = m_vcComTexts[ii]->GetTexts();
		for(int jj = 0; jj < m_vcComTexts[ii]->GetNumTexts(); jj++){
			vcTmpTexts[jj]->WriteText(&PMIHead, i_stkFileP);
		}
		Stk_Envelope* stkEnvelopeP = NULL;
		stkEnvelopeP = m_vcComTexts[ii]->GetEnvelope();
		if (stkEnvelopeP != NULL)
		{
			stkEnvelopeP->WriteFile(&PMIHead, i_stkFileP);
		}
	}

	// 【1.7】 输出OutFrame Record
	for(int ii = 0; ii < m_vcOutFrames.size(); ii++)
	{
		m_vcOutFrames[ii]->WriteFrame(&PMIHead, i_stkFileP);
		vector<Stk_CurveEntity* > vcTmpCurves = m_vcOutFrames[ii]->GetFrameData();

		for(int jj = 0; jj < vcTmpCurves.size(); jj++){
			switch(vcTmpCurves[jj]->GetCurveType())
			{
			case CURVE_TYPE_POLYLINE:
				((Stk_PolyLine*)vcTmpCurves[jj])->WritePolyline(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_ELLIPSE:
				((Stk_Ellipse*)vcTmpCurves[jj])->WriteEllipse(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_LINE:
				((Stk_Line*)vcTmpCurves[jj])->WriteLine(&PMIHead, i_stkFileP);
				break;
			default:
				break;
			}
		}
	}

	// 【1.8】 输出Special Linear/Angular Record
	for(int ii = 0; ii < m_vcSpecialLines.size(); ii++)
	{
		m_vcSpecialLines[ii]->WriteSpecialLine(&PMIHead, i_stkFileP);
		vector<Stk_CurveEntity*> vcTmpCurves = m_vcSpecialLines[ii]->GetCurves();
		for(int jj = 0; jj < vcTmpCurves.size(); jj++){
			switch(vcTmpCurves[jj]->GetCurveType())
			{
			case CURVE_TYPE_POLYLINE:
				((Stk_PolyLine*)vcTmpCurves[jj])->WritePolyline(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_ELLIPSE:
				((Stk_Ellipse*)vcTmpCurves[jj])->WriteEllipse(&PMIHead, i_stkFileP);
				break;
			case CURVE_TYPE_LINE:
				((Stk_Line*)vcTmpCurves[jj])->WriteLine(&PMIHead, i_stkFileP);
				break;
			default:
				break;
			}
		}
	}

	// 【1.9】 输出图片(图章) Record
	for (int ii = 0; ii < m_vcPictures.size(); ii++)
	{
		m_vcPictures[ii]->WriteFile(&PMIHead, i_stkFileP);
	}

	// 【1.7】 完成PMI Table的输出
	ret = i_stkFileP->WriteSegmentFinishData(&PMIHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PMI段-Finish写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

#if 0
	/*
	 *	【第二步】 输出自定义属性段
	 */
	szMetaDataNum = m_vcMetaDatas.size();
	if (szMetaDataNum > 0)
	{
		WriteMetaData(i_stkFileP);
	}
#endif

	/*
	 *	【第二步】 输出用户数据段
	 */
	if(m_szUserDataCnt > 0)
	{
		WriteUserData(i_stkFileP);
	}

	/*
	 *	【第三步】 输出PMI-END段
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("输出PMI-END PMI ID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	endHead.id = m_ID;
	endHead.kind = SEG_TYPE_PMI_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PMI-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		获取PMI类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eType			PMI类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPMITypeEnum Stk_PMI::GetType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		设置PMI类型
 *	@param[in]	i_eType			PMI类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetType(StkPMITypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取定义平面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mtxDefPlane		定义平面
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const STK_MTX32&  Stk_PMI::GetDefPlane()
{
	return m_mtxDefPlane;
}

/************************************************************************//**
 *	@brief		设置定义平面
 *	@param[in]	i_mtxDefPlane		定义平面
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetDefPlane(const STK_MTX32& i_mtxDefPlane)
{
	m_mtxDefPlane = i_mtxDefPlane;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaPMI		PMI颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const STK_RGBA32& Stk_PMI::GetColor()
{
	return m_rgbaPMI;
}

/************************************************************************//**
 *	@brief		设置PMI颜色
 *	@param[in]	i_rgbaPMI		PMI颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetColor(const STK_RGBA32& i_rgbaPMI)
{
	m_rgbaPMI = i_rgbaPMI;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		判断PMI的文字是否固定位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsFix		是否固定
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_PMI::IsFix()
{
	return m_bIsFix;
}

/************************************************************************//**
 *	@brief		设置PMI的文字是否固定位置
 *	@param[in]	i_bIsFix		是否固定
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PMI::SetFix(STK_BOOL i_bIsFix)
{
	m_bIsFix = i_bIsFix;
}

/************************************************************************//**
 *	@brief		判断PMI显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsDisplay	显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_PMI::IsDisplay()
{
	return m_bIsDisplay;
}

/************************************************************************//**
 *	@brief		设置PMI显隐状态
 *	@param[in]	i_bIsDisplay	显隐状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PMI::SetDisplay(STK_BOOL i_bIsDisplay)
{
	m_bIsDisplay = i_bIsDisplay;
}

/****************************************************************************
 *	@brief		是否被调整过
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsAdjusted	调整状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_PMI::IsAdjusted()
{
	return m_bIsAdjusted;
}

/****************************************************************************
 *	@brief		设置调整标志
 *	@param[in]	i_bIsAdjusted	调整状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_PMI::SetAdjusted(STK_BOOL i_bIsAdjusted)
{
	m_bIsAdjusted = i_bIsAdjusted;
}

/************************************************************************//**
 *	@brief		判断PMI文字是否采用平行屏幕显示
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsParallelScreen	是否平行于屏幕
 *	@note		== true 平行屏幕显示 <br>
 *				== false 不平行屏幕显示
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_PMI::IsParallelScreen()
{
	return m_bIsParallelScreen;
}

/************************************************************************//**
 *	@brief		设置PMI文字是否采用平行屏幕显示
 *	@param[in]	i_bIsParallelScreen	是否平行于屏幕
 *	@param[out]	无
 *	@retval		无
 *	@note		== true 平行屏幕显示 <br>
 *				== false 不平行屏幕显示
 *	@attention	无
 ****************************************************************************/

void Stk_PMI::SetParallelScreen(STK_BOOL i_bIsParallelScreen)
{
	m_bIsParallelScreen = i_bIsParallelScreen;
}

/************************************************************************//**
 *	@brief		获取PMI存储模式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eMode	PMI存储模式
 *	@note		== PMI_MODE_PARA 参数存储 <br>
 *				== PMI_MODE_POLY 折线存储
 *	@attention	无
 ****************************************************************************/

StkPMIModeEnum Stk_PMI::GetMode()
{
	return m_eMode;
}

/************************************************************************//**
 *	@brief		设置PMI存储模式
 *	@param[in]	i_eMode	PMI存储模式
 *	@param[out]	无
 *	@retval		无
 *	@note		== PMI_MODE_PARA 参数存储 <br>
 *				== PMI_MODE_POLY 折线存储
 *	@attention	无
 ****************************************************************************/

void Stk_PMI::SetMode(StkPMIModeEnum i_eMode)
{
	m_eMode = i_eMode;
}

/************************************************************************//**
 *	@brief		获取引出线数组数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcLeaders		引出线数组
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_Leader*>& Stk_PMI::GetLeaders()
{
	return m_vcLeaders;
}

/************************************************************************//**
 *	@brief		设置引出线数组数据
 *	@param[in]	i_vcLeaders		引出线数组
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetLeaders(const vector<Stk_Leader*>& i_vcLeaders)
{
	m_vcLeaders = i_vcLeaders;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取延长线数组数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcExLines		延长线数组
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_Extensionline*>& Stk_PMI::GetExLines()
{
	return m_vcExLines;
}

/************************************************************************//**
 *	@brief		设置延长线数组数据
 *	@param[in]	i_vcExLines		延长线数组
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetExLines(const vector<Stk_Extensionline*>& i_vcExLines)
{
	m_vcExLines = i_vcExLines;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI中复合文本数组数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcComTexts	复合文本数组数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_ComText*>& Stk_PMI::GetComText()
{
	return m_vcComTexts;
}

/************************************************************************//**
 *	@brief		往PMI中添加复合文本数据
 *	@param[in]	i_ComTextP		复合文本
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@retval		STK_ERROR		添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::AddComText(Stk_ComText* i_ComTextP)
{
	if (i_ComTextP != NULL)
	{
		m_vcComTexts.push_back(i_ComTextP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI中外框数组数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcOutFrames	外框数组数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_OutFrame*>& Stk_PMI::GetOutFrame()
{
	return m_vcOutFrames;
}

/************************************************************************//**
 *	@brief		PMI中设置外框数据
 *	@param[in]	i_OutFrameP		指向外框的指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		STK_ERROR		设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetOutFrame(Stk_OutFrame* i_OutFrameP)
{
	// 设置定义类型
	i_OutFrameP->SetDefineType(STK_FRAME_DEF_PMI);

	// 插入数组
	if (i_OutFrameP != NULL)
	{
		m_vcOutFrames.push_back(i_OutFrameP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI中特殊线数组数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcSpecialLines	特殊线数组数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_SpecialLine*>& Stk_PMI::GetSpecialLines()
{
	return m_vcSpecialLines;
}

/************************************************************************//**
 *	@brief		往PMI中添加特殊线数据
 *	@param[in]	i_SpecLineP		特殊线数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@retval		STK_ERROR		添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::AddSpecialLine(Stk_SpecialLine* i_SpecLineP)
{
	if (i_SpecLineP != NULL)
	{
		m_vcSpecialLines.push_back(i_SpecLineP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		添加自定义属性
 *	@param[in]	i_MetaDataP		自定义属性
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@retval		STK_ERROR		添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	STK_STATUS		eState = STK_SUCCESS;
	Stk_Node*		stkNodeP = NULL;
	Stk_Node*		ChildNodeP = NULL;
	Stk_Node*		MetaDataNodeP = NULL;
	Stk_MetaData*	MetaDataP = NULL;
	StkNodeTypeEnum	NodeType = NODE_TYPE_UNKNOWN;
	STK_UINT32		nuNodeNum = 0;
	wstring			wstrTitle = L"";
	wstring			wstrTitlePara = L"";
	STK_ID			idNode = 0;
	STK_ID			idPlc = 0;

	stkNodeP = GetNode();
	nuNodeNum = stkNodeP->GetChildNodeNum();
	
	MetaDataNodeP = new Stk_Node(stkNodeP->GetDocManager(), GetProtoType());
	eState = MetaDataNodeP->RegisterID(STK_NEW_ID);
	if (eState != STK_SUCCESS)
	{
		if (MetaDataNodeP != NULL)
		{
			delete MetaDataNodeP;
			MetaDataNodeP = NULL;
		}
		return STK_ERROR;
	}
	MetaDataNodeP->SetNodeType(NODE_TYPE_META);
	MetaDataNodeP->SetMetaData((Stk_MetaData*)i_MetaDataP);

	idNode = MetaDataNodeP->GetID();

	if (nuNodeNum == 0)
	{
		stkNodeP->AddChildNode(idNode, idPlc);
	}
	else
	{
		for (int ii = 0; ii < nuNodeNum; ii++)
		{
			ChildNodeP = stkNodeP->GetChildNode(ii, idPlc);
			NodeType = ChildNodeP->GetNodeType();
			if (NodeType == StkNodeTypeEnum::NODE_TYPE_META)
			{
				MetaDataP = ChildNodeP->GetMetaData();
				wstrTitle = MetaDataP->GetTitle();
				wstrTitlePara = i_MetaDataP->GetTitle();
				if (wstrTitle == wstrTitlePara)
				{
					if (MetaDataNodeP != NULL)
					{
						delete MetaDataNodeP;
						MetaDataNodeP = NULL;
					}
					return STK_ERROR;
				}
				stkNodeP->AddChildNode(idNode, idPlc);
			}
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定的自定义属性
 *	@param[in]	i_nuIndex		自定义属性索引
 *	@param[out]	无
 *	@retval		指定的自定义属性
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_MetaData* Stk_PMI::GetMetaDataByIndex(STK_UINT32 i_nuIndex)
{
	Stk_Node*				stkNodeP = NULL;
	Stk_Node*				ChildNodeP = NULL;
	Stk_MetaData*			MetaDataP = NULL;
	vector<Stk_MetaData*>	vcMetaDatas;
	StkNodeTypeEnum			NodeType = NODE_TYPE_UNKNOWN;
	STK_UINT32				nuNodeNum = 0;
	STK_ID					nuPlcID;

	stkNodeP = GetNode();
	nuNodeNum = stkNodeP->GetChildNodeNum();
	for (int ii = 0; ii < nuNodeNum; ii++)
	{
		ChildNodeP = stkNodeP->GetChildNode(ii,nuPlcID);
		NodeType = ChildNodeP->GetNodeType();
		if (NodeType == StkNodeTypeEnum::NODE_TYPE_META)
		{
			MetaDataP = ChildNodeP->GetMetaData();
			vcMetaDatas.push_back(MetaDataP);
		}
	}
	return vcMetaDatas.at(i_nuIndex);
}

/************************************************************************//**
 *	@brief		获取当前节点自定义属性数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		自定义属性数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_SIZE_T Stk_PMI::GetMetaDataNum()
{
	Stk_Node*				stkNodeP = NULL;
	Stk_Node*				ChildNodeP = NULL;
	Stk_MetaData*			MetaDataP = NULL;
	StkNodeTypeEnum			NodeType = NODE_TYPE_UNKNOWN;
	STK_UINT32				nuNodeNum = 0;
	STK_UINT32				nuMetaDataNum = 0;
	STK_ID					nuPlcID;

	stkNodeP = GetNode();
	nuNodeNum = stkNodeP->GetChildNodeNum();
	for (int ii = 0; ii < nuNodeNum; ii++)
	{
		ChildNodeP = stkNodeP->GetChildNode(ii,nuPlcID);
		NodeType = ChildNodeP->GetNodeType();
		if (NodeType == StkNodeTypeEnum::NODE_TYPE_META)
		{
			MetaDataP = stkNodeP->GetMetaData();
			nuMetaDataNum++;
		}
	}
	return nuMetaDataNum;
}

/************************************************************************//**
 *	@brief		通过自定义属性名删除
 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::DelMetaDataByTitle(wstring i_wstrMetaDataTitle)
{
	Stk_Node*		stkNodeP = NULL;
	Stk_Node*		ChildNodeP = NULL;
	Stk_MetaData*	MetaDataP = NULL;
	StkNodeTypeEnum	NodeType = NODE_TYPE_UNKNOWN;
	STK_UINT32		nuNodeNum = 0;
	wstring			wstrTitle = L"";
	STK_ID			idNode = 0;
	STK_ID			idPlc = 0;

	stkNodeP = GetNode();
	nuNodeNum = stkNodeP->GetChildNodeNum();
	for (int ii = 0; ii < nuNodeNum; ii++)
	{
		ChildNodeP = stkNodeP->GetChildNode(ii,idPlc);
		NodeType = ChildNodeP->GetNodeType();
		if (NodeType == StkNodeTypeEnum::NODE_TYPE_META)
		{
			MetaDataP = ChildNodeP->GetMetaData();
			wstrTitle = MetaDataP->GetTitle();
			if (wstrTitle == i_wstrMetaDataTitle)
			{
				//idPlc = ChildNodeP->GetPlcID(); //注意！ Node的PlcID从GetChildNode方法获得
				stkNodeP->DelChildNodeByPlc(idPlc);
				
				return STK_SUCCESS;
			}
		}
	}
	
	return STK_ERROR; //没有找到，删除失败
}

/************************************************************************//**
 *	@brief		获取PMI的维度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuDim		维度
 *	@note		== 2 二维 <br>
 *				== 3 三维
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_PMI::GetDim()
{
	return m_nuDim;
}

/************************************************************************//**
 *	@brief		设置PMI的维度
 *	@param[in]	i_nuDim			维度
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		STK_ERROR		设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetDim(STK_UINT32 i_nuDim)
{
	if (i_nuDim <2 || i_nuDim >3)
	{
		return STK_ERROR;
	}
	m_nuDim = i_nuDim;
	return STK_SUCCESS;
}

#if 0
/*****************************************************************************
 *	@brief		往当前PMI中添加自定义属性数据
 *	@param[in]	i_MetaDataP		自定义属性数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	if (i_MetaDataP != NULL)
	{
		m_vcMetaDatas.push_back(i_MetaDataP);
	}
	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		获取当前PMI中所有的自定义属性数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas	自定义属性数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_MetaData*>& Stk_PMI::GetAllMetaDatas()
{
	return m_vcMetaDatas;
}

/*****************************************************************************
 *	@brief		获取当前PMI中所有的自定义属性数据数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_PMI::GetMetaDataNum()
{
	return m_vcMetaDatas.size();
}

/*****************************************************************************
 *	@brief		从SVL文件中读取自定义属性数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[in]	i_PropHeadP		自定义属性记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP)
{
	Stk_MetaData*		stkMetaDataP = NULL;
	StkMetaDataRec*		MetaDataRecP = NULL;
	StkControl**		RecordPP = NULL;
	STK_UINT32			nuNumTitle = 0;
	STK_UINT32			nuNumValue = 0;
	STK_UINT32			nuNumUnits = 0;
	STK_UINT32			ii, jj;
	STK_STATUS			eState;

	/*
	 *	读取自定义属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_PropHeadP->recnum);
	if (i_PropHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_PropHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取自定义属性的各记录
	 */
	for (ii = 0; ii < i_PropHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_METADATA)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				stkMetaDataP = new Stk_MetaData(m_stkProtoP);
				if (stkMetaDataP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				MetaDataRecP = (StkMetaDataRec*)RecordPP[ii];
				//m_eValueType = (StkMetaDataValueTypeEnum)MetaDataRecP->nuValueType;

				// 一、属性名称
				nuNumTitle = MetaDataRecP->nuNumTitle;
				STK_WCHAR* wchTitleP = new STK_WCHAR[nuNumTitle+1];
				if (nuNumTitle > 0 && wchTitleP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumTitle; jj++)
				{
					wchTitleP[jj] = MetaDataRecP->atr[jj];
				}
#ifdef __MOBILE__
				stkMetaDataP->SetTitle(Stk_Platform::STKWCHARStrToWString(wchTitleP));
#else
				stkMetaDataP->SetTitle(wchTitleP);
#endif

				//二、属性类型
				stkMetaDataP->SetType((StkMetaDataValueTypeEnum)MetaDataRecP->nuValueType);
				// 属性值
				nuNumValue = MetaDataRecP->nuNumValue;
				STK_WCHAR* wchValueP = new STK_WCHAR[nuNumValue+1];
				if (nuNumValue > 0 && wchValueP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumValue; jj++)
				{
					wchValueP[jj] = MetaDataRecP->atr[nuNumTitle + jj];
				}
#ifdef __MOBILE__
				stkMetaDataP->SetValue(Stk_Platform::STKWCHARStrToWString(wchValueP));
#else
				stkMetaDataP->SetValue(wchValueP);
#endif

				// 三、属性单位
				nuNumUnits = MetaDataRecP->nuNumUnits;
				STK_WCHAR* wchUnitsP = new STK_WCHAR[nuNumUnits+1];
				if (nuNumUnits > 0 && wchUnitsP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumUnits; jj++)
				{
					wchUnitsP[jj] = MetaDataRecP->atr[nuNumTitle + nuNumValue + jj];
				}
#ifdef __MOBILE__
				stkMetaDataP->SetUnits(Stk_Platform::STKWCHARStrToWString(wchUnitsP));
#else
				stkMetaDataP->SetUnits(wchUnitsP);
#endif
				if (stkMetaDataP != NULL)
				{
					m_vcMetaDatas.push_back(stkMetaDataP);
				}

				//删除临时分配的内存
				if (wchTitleP != NULL)
				{
					delete [] wchTitleP;
					wchTitleP = NULL;
				}
				if (wchValueP != NULL)
				{
					delete [] wchValueP;
					wchValueP = NULL;
				}
				if (wchUnitsP != NULL)
				{
					delete [] wchUnitsP;
					wchUnitsP = NULL;
				}
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_PropHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		往SVL文件中填写自定义属性数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::WriteMetaData(Stk_File* i_stkFileP)
{
	StkSegmentHead		metaDataHead = {0};
	StkMetaDataRec		metaDataRec = {0};
	STK_INT32			ret = 0;
	STK_UINT32			nuMetaDataNum = 0;
	STK_UINT32			ii, jj;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
	nuMetaDataNum = GetMetaDataNum();
	if (nuMetaDataNum <= 0)
	{
		return STK_SUCCESS;
	}

	// 【第1步】 输出段头记录
	metaDataHead.id = 0;
	metaDataHead.kind = SEG_TYPE_METADATA;
	ret = i_stkFileP->WriteSegmentHead(&metaDataHead);
	if (ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第2步】 输出自定义属性记录
	for (ii = 0; ii < nuMetaDataNum; ii++)
	{
		m_vcMetaDatas[ii]->WriteMetaData(&metaDataHead, i_stkFileP);
	}

	// 【第3步】 完成自定义属性段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&metaDataHead);
	if (ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}
#endif

/************************************************************************//**
 *	@brief		获取PMI来源
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ePMISource	PMI来源
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPMISourceEnum Stk_PMI::GetPMISource()
{
	return m_ePMISource;
}

/************************************************************************//**
 *	@brief		设置PMI来源
 *	@param[in]	i_ePMISource		PMI来源
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::SetPMISource(StkPMISourceEnum i_ePMISource)
{
	m_ePMISource = i_ePMISource;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置PMI的X方向向量
 *	@param[in]	i_dirX			X方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::SetAxisX(const STK_DIR32& i_dirX)
{
	m_dirX = i_dirX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI的X方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		X方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32& Stk_PMI::GetAxisX()
{
	return m_dirX;
}

/************************************************************************//**
 *	@brief		设置PMI的Y方向向量
 *	@param[in]	i_dirY			Y方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::SetAxisY(const STK_DIR32& i_dirY)
{
	m_dirY = i_dirY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI的Y方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Y方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32& Stk_PMI::GetAxisY()
{
	return m_dirY;
}

/************************************************************************//**
 *	@brief		设置符号基准点位置
 *	@param[in]	i_pntSymBase		符号基准点位置
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::SetSymBasePoint(const STK_PNT32& i_pntSymBase)
{
	m_pntSymBase = i_pntSymBase;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取符号基准点位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		符号基准点位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32& Stk_PMI::GetSymBasePoint()
{
	return m_pntSymBase;
}
/************************************************************************//**
 *	@brief		是否是旧版本的PMI
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		新旧版本的PMI标志
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_PMI::IsOldVersion()
{
	return m_bIsOldVersion;
}

/************************************************************************//**
 *	@brief		设置PMI新旧版本标志
 *	@param[in]	i_bOldVersion		PMI新旧版本标志
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::SetOldVersion(STK_BOOL i_bOldVersion)
{
	m_bIsOldVersion = i_bOldVersion;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PMI::SetNode(Stk_Node* i_NodeP)
{
	m_NodeP = i_NodeP;
	if (i_NodeP != NULL)
	{
		m_idNode = i_NodeP->GetID();
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所对应的Node
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所对应的Node
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_Node* Stk_PMI::GetNode()
{
	return m_NodeP;
}

/************************************************************************//**
 *	@brief		插入用户数据
 *	@param[in]	i_stkUserDataP		用户数据指针
 *	@retval		STK_SUCCESS			成功
 *	@retval		其它				失败
 *	@note		1.请new Stk_UserData对象并将用户数据流填入，然后使用Stk_PMI::AddUserData()插入
 *				2.通过“用户数据名称”+“用户数据ID”的方式来作为唯一标识
 *				  例如“类型”+“1” 表示第一个数据， “类型”+“2” 表示第二个数据
 *				  当两次设置的唯一标识相同时，会自动覆盖掉前次的数据
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::AddUserData(Stk_UserData* i_stkUserDataP)
{
	map<wstring,map<STK_ID,Stk_UserData*>*>::iterator  itor;
	map<STK_ID,Stk_UserData*>::iterator UDitor;
	STK_STATUS stat = STK_SUCCESS;

	if (i_stkUserDataP == NULL)
	{
		return STK_ERROR;
	}

	wstring		wstrDataName=L"";
	wstring		wstrFileName=L"";
	STK_ID		UserDataID=0;
	STK_CHAR	*dataP=NULL;
	STK_SIZE_T	dataSize=0;

	stat = i_stkUserDataP->GetUserData(wstrDataName,UserDataID,dataP,dataSize,wstrFileName);
	if (stat!=STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 查询唯一标识
	itor = m_mapUserDatas.find(wstrDataName);
	if( itor != m_mapUserDatas.end())
	{
		UDitor = (itor->second)->find(UserDataID);
		if ( UDitor!= (itor->second)->end())
		{
			// 如果已存在，则先释放
			delete (UDitor->second);
			// 插入UserData
			(itor->second)->erase(UDitor);
			(itor->second)->insert(pair<STK_ID,Stk_UserData*>(UserDataID,i_stkUserDataP));
		}else
		{
			// 插入UserData
			(itor->second)->insert(pair<STK_ID,Stk_UserData*>(UserDataID,i_stkUserDataP));
			m_szUserDataCnt ++;
		}
	}
	else{
		// 插入UserData
		map<STK_ID,Stk_UserData*> *UDmap = new map<STK_ID,Stk_UserData*>();
		UDmap->insert(pair<STK_ID,Stk_UserData*>(UserDataID,i_stkUserDataP));
		m_mapUserDatas.insert(pair<wstring,map<STK_ID,Stk_UserData*>*>(wstrDataName,UDmap));
		m_szUserDataCnt ++;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过名称获取用户数据列表
 *	@param[in]	用户数据名称
 *	@param[out]	无
 *	@retval		用户数据列表
 *	@note		无
 *	@attention	无
 ****************************************************************************/
map<STK_ID,Stk_UserData*>* Stk_PMI::GetUserDataByName(wstring i_wstrDataName)
{
	map<wstring,map<STK_ID,Stk_UserData*>*>::iterator  itor;
	STK_STATUS stat = STK_SUCCESS;

	// 查询唯一标识
	itor = m_mapUserDatas.find(i_wstrDataName);
	if( itor == m_mapUserDatas.end())
	{
		return NULL;
	}else
	{
		return itor->second;
	}
}

/************************************************************************//**
 *	@brief		删除指定的用户数据
 *	@param[in]	i_wstrDataName		用户数据名称
 *	@param[in]	i_id				用户数据ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			找到数据，删除成功
 *	@retval		STK_ERROR			没有找到数据，删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::DelUserData( wstring i_wstrDataName, STK_ID i_id)
{
	map<wstring,map<STK_ID,Stk_UserData*>*>::iterator  itor;
	map<STK_ID,Stk_UserData*>::iterator UDitor;
	STK_STATUS	stat = STK_SUCCESS;

	// 查询唯一标识
	itor = m_mapUserDatas.find(i_wstrDataName);
	if( itor != m_mapUserDatas.end())
	{
		UDitor = (itor->second)->find(i_id);
		if ( UDitor!= (itor->second)->end())
		{
			// 释放Stk_UserData*
			delete (UDitor->second);
			(itor->second)->erase(UDitor);
			if (m_szUserDataCnt>0)
			{
				m_szUserDataCnt--;
			}
			return STK_SUCCESS;
		}else
		{
			// 不存在
			return STK_ERROR;
		}
	}
	else{
		// 不存在
		return STK_ERROR;
	}

}

/*************************************************************************//**
 *	@brief		往SVL文件中填写用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::WriteUserData(Stk_File* i_stkFileP)
{
	StkSegmentHead		userDataHead = {0};
	STK_INT32			ret = 0;
	STK_UINT32			nuUserDataNum = 0;
	map<wstring,map<STK_ID,Stk_UserData*>*>::iterator itor;
	map<STK_ID,Stk_UserData*>::iterator UDitor;
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPrototype && 1 == stkSVLLog.bMetaData)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【UserData段】输出 UserData"));
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// 【第1步】 输出段头记录
	userDataHead.id = 0;
	userDataHead.kind = SEG_TYPE_USERDEFDATA;
	ret = i_stkFileP->WriteSegmentHeadData(&userDataHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPrototype && 1 == stkSVLLog.bMetaData)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：UserData段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【第2步】 输出用户数据记录
	map<STK_ID,Stk_UserData*> *UDmap;
	itor = m_mapUserDatas.begin();
	while (itor != m_mapUserDatas.end()){
		UDmap = itor->second;
		UDitor = UDmap->begin();
		while(UDitor != UDmap->end()){
			UDitor->second->WriteUserData(i_stkFileP, &userDataHead);
			UDitor++;
		}

		itor++;
	}

	// 【第3步】 完成用户数据的输出
	ret = i_stkFileP->WriteSegmentFinishData(&userDataHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPrototype && 1 == stkSVLLog.bMetaData)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：UserData段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		从SVL文件中读取用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[in]	i_UserHeadP		自定义属性记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PMI::ReadUserDatas(Stk_File *i_FileP, StkSegmentHead *i_UserHeadP)
{
	Stk_UserData*		stkUserDataP = NULL;
	StkUserDataRec*		UserDataRecP = NULL;
	StkUserDataRec2*	UserDataRec2P = NULL;
	StkControl**		RecordPP = NULL;
	STK_UINT32			nuNumTitle = 0;
	STK_UINT32			nuNumValue = 0;
	STK_UINT32			nuNumUnits = 0;
	STK_UINT32			ii;
	STK_STATUS			eState;
	wstring				wstrDataName=L"";
	wstring				wstrFileName=L"";
	STK_ID				UserDataID=0;

	/*
	 *	读取自定义属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_UserHeadP->recnum);
	if (i_UserHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_UserHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取自定义属性的各记录
	 */
	for (ii = 0; ii < i_UserHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_USERDEFDATA)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				stkUserDataP = new Stk_UserData(GetProtoType());
				if (stkUserDataP == NULL)
				{
					return STK_ERROR_MEMORY;
				}

				UserDataRecP = (StkUserDataRec*)RecordPP[ii];
				
#ifdef __MOBILE__
				wstrDataName = Stk_Platform::STKWCHARStrToWString(UserDataRecP->dataName);
				wstrFileName = Stk_Platform::STKWCHARStrToWString(UserDataRecP->FileName);
#else
				wstrDataName = UserDataRecP->dataName;
				wstrFileName = UserDataRecP->FileName;
#endif
				UserDataID = UserDataRecP->dataID;
				stkUserDataP->SetUserData(wstrDataName,UserDataID,UserDataRecP->atr,UserDataRecP->nuCharNum,wstrFileName); // UserDataRecP->atr 会被拷贝一份内存副本到stkUserDataP中


				// 写入ProtoType
				eState = AddUserData(stkUserDataP);
				if (eState!= STK_SUCCESS)
				{
					return STK_ERROR;
				}
			}
			// 版本2
			else if (RecordPP[ii]->version == 2)
			{
				stkUserDataP = new Stk_UserData(GetProtoType());
				if (stkUserDataP == NULL)
				{
					return STK_ERROR_MEMORY;
				}

				UserDataRec2P = (StkUserDataRec2*)RecordPP[ii];

#ifdef __MOBILE__
				wstrDataName = Stk_Platform::STKWCHARStrToWString(UserDataRec2P->dataName);
				wstrFileName = Stk_Platform::STKWCHARStrToWString(UserDataRec2P->FileName);
#else
				wstrDataName = UserDataRec2P->dataName;
				wstrFileName = UserDataRec2P->FileName;
#endif
				UserDataID = UserDataRec2P->dataID;
				stkUserDataP->SetUserData(wstrDataName,UserDataID,UserDataRec2P->atr,UserDataRec2P->nuCharNum,wstrFileName); // UserDataRecP->atr 会被拷贝一份内存副本到stkUserDataP中


				// 写入ProtoType
				eState = AddUserData(stkUserDataP);
				if (eState!= STK_SUCCESS)
				{
					return STK_ERROR;
				}
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_UserHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否是最前端显示的PMI
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		最前端显示PMI标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_PMI::IsFront()
{
	return m_bIsFront;
}

/************************************************************************//**
 *	@brief		设置是否是最前端显示PMI的标识
 *	@param[in]	i_bFront			是否是最前端显示PMI的标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::SetFront(STK_BOOL i_bFront)
{
	m_bIsFront = i_bFront;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_Leader构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Leader::Stk_Leader(void)
{
	m_rgbaLeader.Red = -1.0;
	m_rgbaLeader.Green = -1.0;
	m_rgbaLeader.Blue = -1.0;
	m_rgbaLeader.Alpha = -1.0;
	m_eLeaderType = LEADER_TYPE_UNKNOWN;
	m_eTermType = TERM_PATSMNONE;
	m_fTermSizeA[0] = 0;
	m_fTermSizeA[1] = 0;
	m_pntTermLoc.X = 0;
	m_pntTermLoc.Y = 0;
	m_pntTermLoc.Z = 0;
	m_dirTerm.X = 0;
	m_dirTerm.Y = 0;
	m_dirTerm.Z = 0;
	//m_CurveP = NULL;
	m_bIsStub = false;
}

/************************************************************************//**
 *	@brief		Stk_Leader析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Leader::~Stk_Leader(void)
{
	for (int ii = 0; ii < m_vcCurves.size(); ii++)
	{
		if (m_vcCurves[ii] != NULL)
		{
			delete m_vcCurves[ii];
			m_vcCurves[ii] = NULL;
		}
	}
}

/************************************************************************//**
 *	@brief		获取引出线颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaLeader	引出线颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_Leader::GetColor()
{
	return m_rgbaLeader;
}

/************************************************************************//**
 *	@brief		设置引出线颜色
 *	@param[in]	i_rgbaLeader	引出线颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Leader::SetColor(STK_RGBA32 i_rgbaLeader)
{
	m_rgbaLeader = i_rgbaLeader;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取引出线类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eLeaderType	引出线类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkLeaderTypeEnum Stk_Leader::GetLeaderType()
{
	return m_eLeaderType;
}

/************************************************************************//**
 *	@brief		设置引出线类型
 *	@param[in]	i_eType			引出线类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Leader::SetLeaderType(StkLeaderTypeEnum i_eType)
{
	m_eLeaderType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取引出线末端类型、宽高、起始位置和方向
 *	@param[in]	无
 *	@param[out]	o_eTermType		末端类型
 *	@param[out]	o_SizeA			末端宽高
 *	@param[out]	o_pntLoc		末端起始位置
 *	@param[out]	o_dirTerm		末端方向
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Leader::GetTerminator(StkTermTypeEnum& o_eTermType, STK_FLOAT32 o_SizeA[2], STK_PNT32& o_pntLoc, STK_DIR32& o_dirTerm)
{
	o_eTermType = m_eTermType;
	memcpy(o_SizeA,m_fTermSizeA,sizeof(STK_FLOAT32)*2);
	o_pntLoc = m_pntTermLoc;
	o_dirTerm = m_dirTerm;
}

/************************************************************************//**
 *	@brief		设置引出线末端类型、宽高、起始位置和方向
 *	@param[in]	i_eTermType		末端类型
 *	@param[in]	i_SizeA			末端宽高
 *	@param[in]	i_pntLoc		末端起始位置
 *	@param[in]	i_dirTerm		末端方向
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Leader::SetTerminator(StkTermTypeEnum i_eTermType,STK_FLOAT32 i_SizeA[2], const STK_PNT32& i_pntLoc, const STK_DIR32& i_dirTerm)
{
	m_eTermType = i_eTermType;
	memcpy(m_fTermSizeA, i_SizeA, sizeof(STK_FLOAT32)*2);
	m_pntTermLoc = i_pntLoc;
	m_dirTerm = i_dirTerm;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取引出线数组中最后一条线
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		最后一条引出线
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity* Stk_Leader::GetCurve()
{
	if (m_vcCurves.size() !=0)
	{
		vector<Stk_CurveEntity*>::iterator itor;
		itor = m_vcCurves.end()-1;
		return *itor;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置引出线数据
 *	@param[in]	i_CurveP	Curve数据指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		STK_ERROR	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Leader::SetCurve(Stk_CurveEntity* i_CurveP)
{
	if (i_CurveP != NULL)
	{
		m_vcCurves.push_back(i_CurveP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取所有的引出线
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcCurves	所有引出线
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_CurveEntity*> Stk_Leader::GetAllCurve()
{
	return m_vcCurves;
}

/************************************************************************//**
 *	@brief		添加引出线数据
 *	@param[in]	i_CurveP	Curve数据指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS	添加成功
 *	@retval		STK_ERROR	添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Leader::AddCurve(Stk_CurveEntity* i_CurveP)
{
	if (i_CurveP != NULL)
	{
		m_vcCurves.push_back(i_CurveP);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置二维折弯线标识
 *	@param[in]	i_bStub		二维折弯线标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		STK_ERROR	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Leader::SetStubDelta(STK_BOOL i_bStub)
{
	m_bIsStub = i_bStub;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否是二维折弯线
 *	@param[in]	i_bStub		二维折弯线标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		STK_ERROR	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Leader::IsStubDelta()
{
	return m_bIsStub;
}

/*************************************************************************//**
 *	@brief		写入引出线数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Leader::WriteLeader(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	STK_STATUS	ret = STK_SUCCESS;

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	StkLeaderRec2 *LeaderRec2P = NULL;
	LeaderRec2P = (StkLeaderRec2 *)calloc(1, sizeof(StkLeaderRec2));
	if(LeaderRec2P == NULL){
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	LeaderRec2P->Control.type = REC_TYPE_LEADER;
	LeaderRec2P->Control.length = sizeof(StkLeaderRec2) - sizeof(StkControl);
	LeaderRec2P->Control.version = _VER_LEADER_RECORD;/*2*/

	LeaderRec2P->LeaderType = m_eLeaderType;
	LeaderRec2P->TermType = m_eTermType;
	memcpy(LeaderRec2P->Size, m_fTermSizeA, sizeof(STK_UINT32)*2);
	LeaderRec2P->TermLoc = m_pntTermLoc;
	LeaderRec2P->TermDir = m_dirTerm;
	LeaderRec2P->nuStub = m_bIsStub;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)LeaderRec2P);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if( LeaderRec2P != NULL)
	{
		free(LeaderRec2P);
		LeaderRec2P = NULL;
	}

	return ret;
}
/************************************************************************//**
 *	@brief		Stk_Extensionline构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Extensionline::Stk_Extensionline(void)
{
	//m_nuExLineType = -1;
	m_CurveP = NULL;
	m_bIsDisplay = true;
}

/************************************************************************//**
 *	@brief		Stk_Extensionline析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Extensionline::~Stk_Extensionline(void)
{
	if (m_CurveP != NULL)
	{
		delete m_CurveP;
		m_CurveP = NULL;
	}
}

/************************************************************************//**
 *	@brief		获取延长线的定义类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eExLineType	延长线的定义类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkExtensTypeEnum Stk_Extensionline::GetExLineType()
{
	return m_eExLineType;
}

/************************************************************************//**
 *	@brief		设置延长线的定义类型
 *	@param[in]	i_eType		延长线的定义类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		STK_SUCCESS	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Extensionline::SetExLineType(StkExtensTypeEnum i_eType)
{
	m_eExLineType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Curve数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_CurveP	Curve对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity* Stk_Extensionline::GetCurve()
{
	return m_CurveP;
}

/************************************************************************//**
 *	@brief		设置Curve数据
 *	@param[in]	i_CurveP	Curve数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		STK_ERROR	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Extensionline::SetCurve(Stk_CurveEntity* i_CurveP)
{
	if (i_CurveP != NULL)
	{
		m_CurveP = i_CurveP;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		判断延长线显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsDisplay	显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Extensionline::IsDisplay()
{
	return m_bIsDisplay;
}

/************************************************************************//**
 *	@brief		设置延长线显隐状态
 *	@param[in]	i_bIsDisplay	显隐状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Extensionline::SetDisplay(STK_BOOL i_bIsDisplay)
{
	m_bIsDisplay = i_bIsDisplay;
}


/*************************************************************************//**
 *	@brief		写入延长线数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Extensionline::WriteExLine(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	STK_STATUS	ret = STK_SUCCESS;

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	StkExLineRec2 *ExLineRecP2 = NULL;
	ExLineRecP2 = (StkExLineRec2 *)calloc(1, sizeof(StkExLineRec2));
	if(ExLineRecP2 == NULL){
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	ExLineRecP2->Control.type = REC_TYPE_EXLINE;
	ExLineRecP2->Control.length = sizeof(StkExLineRec2) - sizeof(StkControl);
	ExLineRecP2->Control.version = _VER_EXLINE_RECORD /*2*/;
	ExLineRecP2->ExLineType = m_eExLineType;

	ExLineRecP2->IsItem = 0;
	if( m_bIsDisplay)
		ExLineRecP2->IsItem |= STK_EXTLINE_IS_DISPLAY; 

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)ExLineRecP2);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if(ExLineRecP2 != NULL){
		free(ExLineRecP2);
		ExLineRecP2 = NULL;
	}
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		Stk_SpecialLine构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpecialLine::Stk_SpecialLine(void)
{
	m_eLineUsage = SPEC_LINE_USAGE_UNKNOWN;
}

/************************************************************************//**
 *	@brief		Stk_SpecialLine析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpecialLine::~Stk_SpecialLine(void)
{
	for (int ii = 0; ii < m_vcCurves.size(); ii++)
	{
		if (m_vcCurves[ii] != NULL)
		{
			delete m_vcCurves[ii];
			m_vcCurves[ii] = NULL;
		}
	}
}

/************************************************************************//**
 *	@brief		获取特殊线用途
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eLineUsage	特殊线用途
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkSpecLineUsageEnum Stk_SpecialLine::GetSpecLinUse()
{
	return m_eLineUsage;
}

/************************************************************************//**
 *	@brief		设置特殊线用途
 *	@param[in]	i_eLineType	特殊线用途
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功	
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpecialLine::SetSpecLinUse(StkSpecLineUsageEnum i_eLineType)
{
	m_eLineUsage = i_eLineType; 
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取所有特殊线
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcCurves		所有特殊线
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_CurveEntity*> Stk_SpecialLine::GetCurves()
{
	return m_vcCurves;
}

/************************************************************************//**
 *	@brief		添加特殊线到特殊线数组中
 *	@param[in]	i_CurveP		Curve数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@retval		STK_ERROR
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpecialLine::AddCurve(Stk_CurveEntity* i_CurveP)
{
	if (i_CurveP == NULL)
	{
		return STK_ERROR;
	}
	m_vcCurves.push_back(i_CurveP);
	return STK_SUCCESS;
}


/*************************************************************************//**
 *	@brief		写入特殊线数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_SpecialLine::WriteSpecialLine(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	STK_STATUS	ret = STK_SUCCESS;

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	StkSpecLinRec *SpecLineRecP = NULL;
	SpecLineRecP = (StkSpecLinRec*)calloc(1, sizeof(StkSpecLinRec));
	if(SpecLineRecP == NULL){
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	SpecLineRecP->Control.type = REC_TYPE_SPECIALLINE;
	SpecLineRecP->Control.length = sizeof(StkSpecLinRec) - sizeof(StkControl);
	SpecLineRecP->Control.version = _VER_SPECIALLINE_RECORD;
	SpecLineRecP->SpecLineUse = m_eLineUsage;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)SpecLineRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if(SpecLineRecP != NULL){
		free(SpecLineRecP);
		SpecLineRecP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		添加图片(图章)
 *	@param[in]	i_stkPictureP		图片(图章)
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PMI::AddPicture(Stk_Picture* i_stkPictureP)
{
	if (i_stkPictureP != NULL)
	{
		m_vcPictures.push_back(i_stkPictureP);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片(图章)的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片(图章)的数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_PMI::GetPictureNum()
{
	return m_vcPictures.size();
}

/************************************************************************//**
 *	@brief		获取指定的图片(图章)
 *	@param[in]	i_nuIndex			索引
 *	@param[out]	无
 *	@retval		指定的图片(图章)
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Picture* Stk_PMI::GetPictureByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcPictures.at(i_nuIndex);
}