// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_MeshEntity.cpp
 *
 *	@brief	Mesh 基类、复合图形类和构成面类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_MeshEntity.h"
#include "Stk_Manager.h"
#include "Stk_Enum.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_MeshEntity构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_MeshEntity::Stk_MeshEntity(void)
{
	//memset( &m_boxBounding,0,sizeof(STK_BOX32));
	m_boxBounding.BoundBox[0][0] = 0.0;
	m_boxBounding.BoundBox[0][1] = 0.0;
	m_boxBounding.BoundBox[0][2] = 0.0;
	m_boxBounding.BoundBox[1][0] = 0.0;
	m_boxBounding.BoundBox[1][1] = 0.0;
	m_boxBounding.BoundBox[1][2] = 0.0;
	m_bHasBox = FALSE;
	m_stkGeoAttrP = NULL;
	m_stkProtoP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_MeshEntity析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_MeshEntity::~Stk_MeshEntity(void)
{
	if(m_stkGeoAttrP != NULL)
	{
		delete m_stkGeoAttrP;
		m_stkGeoAttrP = NULL;
	}
#if 0
	for (int ii = 0; ii < m_vcMetaDatas.size(); ii++)
	{
		if (m_vcMetaDatas[ii] != NULL)
		{
			delete m_vcMetaDatas[ii];
			m_vcMetaDatas[ii] = NULL;
		}
	}
#endif
	m_stkProtoP->GetProtoIDManager()->DelMeshComByID(m_ID);
}

/************************************************************************//**
 *	@brief		设置包围盒
 *	@param[in]	i_boxBounding	包围盒
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS	Stk_MeshEntity::SetBoundingBox(const STK_BOX32 &i_boxBounding)
{
	m_boxBounding = i_boxBounding;
	m_bHasBox = TRUE;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取包围盒
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_boxBounding	包围盒
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOX32 Stk_MeshEntity::GetBoundingBox()
{
	return m_boxBounding;
}

/*************************************************************************//**
 *	@brief		判断是否有包围盒
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasBox	是否有包围盒
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_MeshEntity::HasBoundingBox()
{
	return m_bHasBox;
}

/*************************************************************************//**
 *	@brief		获取ProtoType的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idProtoType	ProtoType的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_MeshEntity::GetProtoTypeID()
{
	return m_stkProtoP->GetID();
}

/*************************************************************************//**
 *	@brief		获取Mesh的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	Mesh的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_MeshEntity::GetID()
{
	return m_ID;
}

/*************************************************************************//**
 *	@brief		设置ID
 *	@param[in]	i_ID		ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_MeshEntity::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		获取基本属性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_stkGeoAttrP	基本属性对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_GeometryAttribute* Stk_MeshEntity::GetGeoAttr()
{
	return m_stkGeoAttrP;
}

/************************************************************************//**
 *	@brief		设置基本属性
 *	@param[in]	*i_stkGeoAttrP	基本属性对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
  *	@retval		STK_ERROR	设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_MeshEntity::SetGeoAttr(Stk_GeometryAttribute *i_stkGeoAttrP)
{
	if(i_stkGeoAttrP == NULL)
	{
		return STK_ERROR;
	}
	else
	{
		m_stkGeoAttrP = i_stkGeoAttrP;
		return STK_SUCCESS;
	}
}

#if 0
/*****************************************************************************
 *	@brief		往当前Mesh中添加自定义属性数据
 *	@param[in]	i_MetaDataP		自定义属性数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_MeshEntity::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	if (i_MetaDataP != NULL)
	{
		m_vcMetaDatas.push_back(i_MetaDataP);
	}
	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		获取当前Mesh中所有的自定义属性数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas	自定义属性数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_MetaData*>& Stk_MeshEntity::GetAllMetaDatas()
{
	return m_vcMetaDatas;
}

/*****************************************************************************
 *	@brief		获取当前Mesh中所有的自定义属性数据数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_MeshEntity::GetMetaDataNum()
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

STK_STATUS Stk_MeshEntity::ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP)
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
				STK_WCHAR* wchTitleP = new STK_WCHAR[nuNumTitle];
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
				STK_WCHAR* wchValueP = new STK_WCHAR[nuNumValue];
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
				STK_WCHAR* wchUnitsP = new STK_WCHAR[nuNumUnits];
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

STK_STATUS Stk_MeshEntity::WriteMetaData(Stk_File* i_stkFileP)
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

/*************************************************************************//**
 *	@brief		Stk_Mesh_Edge构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Edge::Stk_Mesh_Edge(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	memset(&m_boxBounding, 0, sizeof(STK_BOX32));
	m_bHasBox = FALSE;
	m_bHasColor = FALSE;
	m_rgbaEdge.Red = -1.0;
	m_rgbaEdge.Green = -1.0;
	m_rgbaEdge.Blue = -1.0;
	m_rgbaEdge.Alpha = -1.0;
	m_stkGeoAttrP = NULL;
	m_fWidth = 6.0f;

	map<STK_UINT32, vcEdgeInfo*>::iterator itor = m_mapEdgeLODInfo.begin();
	while(itor != m_mapEdgeLODInfo.end())
	{
		itor->second = NULL;
		itor++;
	}
}

/*************************************************************************//**
 *	@brief		Stk_Mesh_Edge析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Edge::~Stk_Mesh_Edge()
{
	map<STK_UINT32, vcEdgeInfo*>::iterator itor = m_mapEdgeLODInfo.begin();
	while(itor != m_mapEdgeLODInfo.end())
	{
		if(itor->second != NULL)
		{
			delete itor->second;
			itor->second = NULL;
		}
		itor++;
	}
	m_stkProtoP->GetProtoIDManager()->DelMeshEdgeByID( m_ID);
}

/*************************************************************************//**
 *	@brief		设置Edge指定LOD等级下的Edge信息
 *	@param[in]	i_nuLODLevel		LOD等级
 *	@param[in]	i_EdgeInfoP			对应LOD等级的Edge数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::SetEdgeLODInfo(STK_UINT32 i_nuLODLevel, vcEdgeInfo *i_EdgeInfoP)
{
	pair<map<STK_UINT32, vcEdgeInfo*>::iterator, bool> ret;
	if (i_nuLODLevel > 8 || i_EdgeInfoP == NULL)
	{
		return STK_ERROR;
	}
	ret = m_mapEdgeLODInfo.insert(pair<STK_UINT32, vcEdgeInfo*>(i_nuLODLevel, i_EdgeInfoP));
	if(ret.second)
	{
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
	
}

/*************************************************************************//**
 *	@brief		获取所有LOD等级对应的Edge数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapEdgeLODInfo	所有LOD等级对应的Edge数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

map<STK_UINT32, vcEdgeInfo*> Stk_Mesh_Edge::GetAllEdgeLODInfo()
{
	return m_mapEdgeLODInfo;
}

/************************************************************************//**
 *	@brief		清空所有的EdgeLOD信息
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::ClearAllEdgeLOD()
{
	m_mapEdgeLODInfo.clear();
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		判断边界线是否有颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasColor	边界线是否有颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Mesh_Edge::IsHasColor()
{
	return m_bHasColor;
}

/*************************************************************************//**
 *	@brief		获取边界线的颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaEdge	边界线的颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Mesh_Edge::GetColor()
{
	return m_rgbaEdge;
}

/*************************************************************************//**
 *	@brief		设置边界线的颜色值
 *	@param[in]	i_rgbaEdge	边界线的颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::SetColor(STK_RGBA32 &i_rgbaEdge)
{
	m_rgbaEdge = i_rgbaEdge;
	m_bHasColor = TRUE;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		注册ID
 *	@param[in]	i_IDEdge		Edge ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		其他			ID重复注册失败
 *	@note		由于V2.2版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::RegisterID(STK_ID i_IDEdge)
{
	STK_STATUS	ret = STK_ERROR;
	Stk_File*	stkFileP = NULL;

	ret = m_stkProtoP->GetProtoIDManager()->RegMeshEdgeID( this, i_IDEdge);
	
	// 由于杨洪鹏在转换器中已将边界线去重的逻辑给屏蔽掉。
	// 因此，为了对应，该处需要增加重复注册时的边界线ID
#if 0
	if(ret != STK_SUCCESS)
	{
		stkFileP = m_stkProtoP->GetFile();
		if (stkFileP == NULL)
		{
			return STK_ERROR;
		}
		wstring wstrVer = stkFileP->GetSVLVersion();
		double dVer = wcstod(wstrVer.c_str(),NULL);
		if (dVer >= 2.33)
		{
			return STK_ERROR;
		}
		else
		{
			m_ID = i_IDEdge;
			return STK_SUCCESS;
		}
	}
	else
	{
		m_ID = i_IDEdge;
		return STK_SUCCESS;
	}
#else
	if (ret != STK_SUCCESS && ret != STK_SUCCESS_EXIST)
	{
		return STK_ERROR;
	}
	else
	{
		m_ID = i_IDEdge;
		return ret;
	}
#endif

}

/*************************************************************************//**
 *	@brief		设置边界线的宽度
 *	@param[in]	i_fWidth	边界线的宽度
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::SetWidth(STK_FLOAT32 i_fWidth)
{
	m_fWidth = i_fWidth;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线的宽度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		边界线的宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Mesh_Edge::GetWidth()
{
	return m_fWidth;
}

/*************************************************************************//**
 *	@brief		写MeshEdge数据
 *	@param[in]	i_head				数据段头指针
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Edge::WriteFile(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	StkMeshEdgeRec2		edgeRec = {0};
	StkLineAttrRec		lineAttrRec = {0};
	StkEllipseAttrRec	ellipseAttrRec = {0};
	STK_STATUS			ret = STK_SUCCESS;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第1步】 输出MeshEdge记录
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("MeshEdge ID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	edgeRec.Control.type = REC_TYPE_MESHEDGE;
	edgeRec.Control.length = sizeof(StkMeshEdgeRec2) - sizeof(StkControl);
	edgeRec.Control.version = _VER_MESHEDGE_RECORD;

	edgeRec.EdgeId = m_ID;
	edgeRec.hasItem = 0;
	// 颜色
	if( m_bHasColor){
		edgeRec.color[0] = m_rgbaEdge.Red;
		edgeRec.color[1] = m_rgbaEdge.Green;
		edgeRec.color[2] = m_rgbaEdge.Blue;
		edgeRec.color[3] = m_rgbaEdge.Alpha;
		edgeRec.hasItem |= STK_HAS_COLOR;
	}else{
		edgeRec.color[0] = -1;
		edgeRec.color[1] = -1;
		edgeRec.color[2] = -1;
		edgeRec.color[3] = -1;
	}
	// 包围盒
	if( m_bHasBox){
		edgeRec.box[0][0] = m_boxBounding.BoundBox[0][0];
		edgeRec.box[0][1] = m_boxBounding.BoundBox[0][1];
		edgeRec.box[0][2] = m_boxBounding.BoundBox[0][2];
		edgeRec.box[1][0] = m_boxBounding.BoundBox[1][0];
		edgeRec.box[1][1] = m_boxBounding.BoundBox[1][1];
		edgeRec.box[1][2] = m_boxBounding.BoundBox[1][2];
		edgeRec.hasItem |= STK_HAS_BOX;
	}else{
		edgeRec.box[0][0] = 0;
		edgeRec.box[0][1] = 0;
		edgeRec.box[0][2] = 0;
		edgeRec.box[1][0] = 0;
		edgeRec.box[1][1] = 0;
		edgeRec.box[1][2] = 0;
	}
	edgeRec.fWidth = m_fWidth;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl*)&edgeRec);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshEdge Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	【第2步】 输出MeshEdge的LOD记录:0-8
	 */
	map<STK_UINT32, vcEdgeInfo*>::iterator itor;
	for(itor = m_mapEdgeLODInfo.begin(); itor != m_mapEdgeLODInfo.end(); itor++)
	{
		StkEdgeLODRec* EdgeLODRecP = NULL;
		STK_UINT32 nuIndexNumLOD = 0;

		nuIndexNumLOD = itor->second->size();
		EdgeLODRecP = (StkEdgeLODRec *)calloc(1, sizeof(StkEdgeLODRec) - sizeof(UINT) + nuIndexNumLOD * sizeof(UINT)); //### 当=0时
		if(EdgeLODRecP == NULL){
			continue;
		}

		EdgeLODRecP->Control.type = REC_TYPE_EDGE_LOD;
		EdgeLODRecP->Control.length = sizeof(StkEdgeLODRec) - sizeof(STK_UINT32) + nuIndexNumLOD * sizeof(STK_UINT32) - sizeof(StkControl);
		EdgeLODRecP->Control.version = _VER_EDGE_LOD_RECORD;

		EdgeLODRecP->EdgeId = m_ID;
		EdgeLODRecP->LODLevel = itor->first;
		EdgeLODRecP->uintnum = nuIndexNumLOD;

		for(int ii = 0; ii < nuIndexNumLOD; ii++)
		{
			EdgeLODRecP->atr[ii] = (itor->second)->at(ii);
		}

		ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl*)EdgeLODRecP);
		if( ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshEdge LOD记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(EdgeLODRecP != NULL)
		{
			free(EdgeLODRecP);
			EdgeLODRecP = NULL;
		}
	}

	/*
	 *	【第3步】 输出MeshEdge的基本属性记录
	 */
	if (m_stkGeoAttrP != NULL)
	{
		switch(m_stkGeoAttrP->GetGeoAttrType())
		{
		case GEOATTR_TYPE_LINE:
			lineAttrRec.Control.type = REC_TYPE_EDGE_ATTR_LINE;
			lineAttrRec.Control.length = sizeof(StkLineAttrRec) - sizeof(StkControl);
			lineAttrRec.Control.version = _VER_EDGEATTR_RECORD;

			lineAttrRec.AttrType = GEOATTR_TYPE_LINE;
			lineAttrRec.ID = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetID();
			lineAttrRec.fLength = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetLength();
			lineAttrRec.dirDirect = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetDirection();
			lineAttrRec.pntCenter = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetCenterPoint();
			lineAttrRec.pntStart = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetStartPoint();
			lineAttrRec.pntEnd = ((Stk_LineAttribute*)m_stkGeoAttrP)->GetEndPoint();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl*)&lineAttrRec);
			if( ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshEdge LINE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;
		case GEOATTR_TYPE_ELLIPSE:
			ellipseAttrRec.Control.type = REC_TYPE_EDGE_ATTR_ELLIPSE;
			ellipseAttrRec.Control.length = sizeof(StkEllipseAttrRec) - sizeof(StkControl);
			ellipseAttrRec.Control.version = _VER_EDGEATTR_RECORD;

			ellipseAttrRec.AttrType = GEOATTR_TYPE_ELLIPSE;
			ellipseAttrRec.ID = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetID();
		
			ellipseAttrRec.fMajorRadius = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetMajorRadius();
			ellipseAttrRec.fMinorRadius = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetMinorRadius();
			ellipseAttrRec.pntStart = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetStartPoint();
			ellipseAttrRec.pntEnd = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetEndPoint();
			ellipseAttrRec.pntCenter = ((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetCenterPoint();

			((Stk_EllipseAttribute*)m_stkGeoAttrP)->GetXYZDir(ellipseAttrRec.XDir, ellipseAttrRec.YDir, ellipseAttrRec.ZDir);
			
			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl*)&ellipseAttrRec);
			if( ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshEdge ELLIPSE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;
		default:
			break;
		}
	}
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		Stk_Mesh_Com构造函数
 *	@param[in]	i_stkProtoTypeP		当前Mesh所属的ProtoType对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Mesh_Com::Stk_Mesh_Com(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_bHasColor = FALSE;
	m_rgbaMesh.Red = -1.0;
	m_rgbaMesh.Green = -1.0;
	m_rgbaMesh.Blue = -1.0;
	m_rgbaMesh.Alpha = -1.0;
	m_bIsClosed = FALSE;
	m_bHasMaterial = FALSE;
	m_bIsUniColor = FALSE;
	m_bIsOnTop = FALSE;
	m_bIsDisplay = TRUE;
	m_eMeshType = MESH_STORE_LOD_UNSHARE_VERTEX;
	m_stkGeoAttrP = NULL;
	m_eMeshEgdeType = MESH_EDGE_STORE_IN_FACE; // 未来如果用户可以新建MeshCom，需要注意m_eMeshEgdeType的初始值 ###
	m_NodeP = NULL;
	m_bHasRender = false;
	m_idRender = 0;
	m_szUserDataCnt = 0;
}

/************************************************************************//**
 *	@brief		Stk_Mesh_Com析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_Mesh_Com::~Stk_Mesh_Com(void)
{
	for (int jj = 0; jj < m_vcSubFaces.size(); jj++)
	{
		if (m_vcSubFaces[jj] != NULL)
		{
			delete m_vcSubFaces[jj];
			m_vcSubFaces[jj] = NULL;
		}
	}
	map<STK_ID, Stk_Mesh_Edge*>::iterator itorEdge = m_mapMeshEdges.begin();
	while(itorEdge != m_mapMeshEdges.end())
	{
		if(itorEdge->second != NULL)
		{
			delete itorEdge->second;
			itorEdge->second = NULL;
		}
		itorEdge ++;
	}
	//LOD
	map<STK_UINT32,MeshLODInfo*>::iterator itor = m_mapMeshLODInfo.begin();
	while (itor != m_mapMeshLODInfo.end())
	{
		if (itor->second != NULL)
		{
			delete itor->second;
			itor->second = NULL;
		}
		itor++;
	}
	// 删除所从属的Node节点
	if (m_NodeP!=NULL)
	{
		m_NodeP->SetMesh(NULL); //防止析构Node时再次析构Mesh造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}

	m_stkProtoP->GetProtoIDManager()->DelMeshComByID(m_ID);
}

/************************************************************************//**
 *	@brief		向Mesh中添加Face数据
 *	@param[in]	i_FaceP		Face类对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS	添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Com::AddMeshFace(Stk_Mesh_Face *i_FaceP)
{
	m_vcSubFaces.push_back(i_FaceP);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Mesh中Face的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		szNum	Face数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_Mesh_Com::GetMeshFaceNum()
{
	STK_SIZE_T szNum = m_vcSubFaces.size();
	return szNum;
}

/************************************************************************//**
 *	@brief		获取Mesh中指定的Face
 *	@param[in]	i_nuKeyPose		指定的Face的索引
 *	@param[out]	无
 *	@retval		outMeshFaceP	指定的Face对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Mesh_Face* Stk_Mesh_Com::GetMeshFace(STK_UINT32 i_nuKeyPose)
{
	Stk_Mesh_Face* outMeshFaceP =NULL;

	if (i_nuKeyPose > m_vcSubFaces.size())
	{
		return NULL;
	}

	outMeshFaceP = m_vcSubFaces.at(i_nuKeyPose);

	return outMeshFaceP;
}

/****************************************************************************
 *	@brief		从Mesh中删除指定的Face
 *	@param[in]	i_stkFaceP		指定的Face对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		其它			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::DeleteMeshFace(Stk_Mesh_Face* i_stkFaceP)
{
	STK_STATUS		eState = STK_SUCCESS;
	vector<Stk_Mesh_Face*>::iterator itor = m_vcSubFaces.begin();
	while(itor != m_vcSubFaces.end())
	{
		if ((*itor) == i_stkFaceP)
		{
			m_stkProtoP->GetProtoIDManager()->DelMeshFaceByID(i_stkFaceP->GetID());
			delete (*itor);
			m_vcSubFaces.erase(itor);
			return STK_SUCCESS;
		}
		itor ++;
	}
	return STK_ERROR_MEMORY;
}

/************************************************************************//**
 *	@brief		清空MeshCom里的点列及索引数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		清空成功
 *	@retval		其他			清空失败
 *	@note		无
 *	@attention	清空MeshCom中的点列及Face和Edge索引
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::ClearMeshVertexAndIndex()
{
	for (int jj = 0; jj < m_vcSubFaces.size(); jj++)
	{
		if (m_vcSubFaces[jj] != NULL)
		{
			map<STK_UINT32, FaceLODInfo*> mapFaceLODInfoP = m_vcSubFaces[jj]->GetAllFaceLOD();
			map<STK_UINT32, FaceLODInfo*>::iterator itor = mapFaceLODInfoP.begin();
			while(itor != mapFaceLODInfoP.end())
			{
				if (itor->second != NULL)
				{
					delete itor->second;
					itor->second = NULL;
				}
				itor++;
			}
			m_stkProtoP->GetProtoIDManager()->DelMeshFaceByID(m_vcSubFaces[jj]->GetID());
			m_vcSubFaces[jj]->ClearAllFaceLOD();
		}
	}
	m_vcSubFaces.clear();

	map<STK_ID, Stk_Mesh_Edge*>::iterator itorEdge = m_mapMeshEdges.begin();
	while(itorEdge != m_mapMeshEdges.end())
	{
		if(itorEdge->second != NULL)
		{
			map<STK_UINT32, vcEdgeInfo*> mapEdgeLODInfoP = itorEdge->second->GetAllEdgeLODInfo();
			map<STK_UINT32, vcEdgeInfo*>::iterator itor = mapEdgeLODInfoP.begin();
			while(itor != mapEdgeLODInfoP.end())
			{
				if(itor->second != NULL)
				{
					delete itor->second;
					itor->second = NULL;
				}
				itor++;
			}
			m_stkProtoP->GetProtoIDManager()->DelMeshEdgeByID(itorEdge->second->GetID());
			itorEdge->second->ClearAllEdgeLOD();
		}
		itorEdge ++;
	}
	m_mapMeshEdges.clear();

	//LOD
	map<STK_UINT32,MeshLODInfo*>::iterator itor = m_mapMeshLODInfo.begin();
	while (itor != m_mapMeshLODInfo.end())
	{
		if (itor->second != NULL)
		{
			delete itor->second;
			itor->second = NULL;
		}
		itor++;
	}
	m_mapMeshLODInfo.clear();

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置Mesh的颜色
 *	@param[in]	i_rgbaMesh		Mesh颜色
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Com::SetColor(const STK_RGBA32 &i_rgbaMesh)
{
	m_rgbaMesh = i_rgbaMesh; 
	m_bHasColor = TRUE;
}

/************************************************************************//**
 *	@brief		获取Mesh的颜色
 *	@param[in]	无		
 *	@param[out]	无
 *	@retval		m_rgbaMesh	Mesh颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_Mesh_Com::GetColor()
{ 
	return m_rgbaMesh;
}

/************************************************************************//**
 *	@brief		判断Mesh是否有颜色
 *	@param[in]	无		
 *	@param[out]	无
 *	@retval		m_bHasColor	Mesh颜色状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Com::HasColor()
{
	return m_bHasColor;
}

/************************************************************************//**
 *	@brief		读取文件中的Mesh数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_MeshHeadP			Mesh记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		STK_ERROR			读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Mesh_Com::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MeshHeadP)
{
	Stk_Mesh_Face	*CurMeshFaceP = NULL;
	StkMeshRec2		*MeshRec2P = NULL;
	StkMeshRec3		*MeshRec3P = NULL;
	StkMeshRec4		*MeshRec4P = NULL;
	StkMeshRec5		*MeshRec5P = NULL;
	StkFaceRec2		*FaceRec2P = NULL;
	StkFaceRec3		*FaceRec3P = NULL;
	StkFaceRec4		*FaceRec4P = NULL;
	StkMeshEdgeRec	*MeshEdgeRecP = NULL;
	StkMeshEdgeRec2	*MeshEdgeRec2P = NULL;
	StkMeshLODRec	*MeshLODRecP = NULL;
	StkFaceLODRec	*FaceLODRecP = NULL;
	StkEdgeLODRec	*EdgeLODRecP = NULL;
	StkUVRec		*stkUVRecP = NULL;
	FaceLODInfo		*FaceLODTmpP = NULL;
	vcEdgeInfo		*EdgeIdxP = NULL;
	Stk_Mesh_Edge	*stkMeshEdgeP = NULL;
	StkControl		**RecordPP = NULL;
	UINT			nuElemcount;
	UINT			ii,jj;
	STK_STATUS		ret;
	STK_STATUS		eState;
	StkPlnFaceAttrRec			*plnFaceAttrRecP = NULL;
	Stk_PlaneFaceAttribute		*stkPlnFaceAttrP = NULL;
	StkCylinFaceAttrRec			*cyFaceAttrRecP = NULL;
	Stk_CylinderFaceAttribute	*stkCylinFaceAttrP = NULL;
	StkConicalFaceAttrRec		*conFaceAttrRecP = NULL;
	Stk_ConicalFaceAttribute	*stkConFaceAttrP = NULL;
	StkSphereFaceAttrRec		*spFaceAttrRecP =NULL;
	Stk_SphereFaceAttribute		*stkSpFaceAttrP = NULL;
	Stk_RevolutionFaceAttribute	*stkRevoFaceAttrP = NULL;
	StkRevoFaceAttrRec			*revoFaceAttrRecP = NULL;
	Stk_ToroidalFaceAttribute	*stkTorFaceAttrP = NULL;
	StkTorFaceAttrRec			*torFaceAttrRecP = NULL;
	StkLineAttrRec				*lineAttrRecP = NULL;
	Stk_LineAttribute			*stkLineAttrP = NULL;
	StkEllipseAttrRec			*ellipseAttrRecP = NULL;
	Stk_EllipseAttribute		*stkEllipseAttrP = NULL;
	STK_UINT32		nuLevel = 0;	// 用于保存Render时用

	RegisterID(i_MeshHeadP->id); //### Mesh、Face、Edge 的注册ID，都放到ReadFile里面

	/*
	 * 读取Mesh段的数据
	 */
	RecordPP = (StkControl **)calloc(i_MeshHeadP->recnum, sizeof(StkControl*));
	if (i_MeshHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_MeshHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取Mesh段的各记录
	 */
	for (ii = 0; ii < i_MeshHeadP->recnum; ii++)
	{

		if (RecordPP[ii]->type == REC_TYPE_MESH)
		{
			/*
			 *  填充Mesh_Com
			 */
			//版本1已废弃
			if (RecordPP[ii]->version == 2)
			{
				/*
				 *  填充Mesh_Com(version 2)
				 */
				MeshRec2P = (StkMeshRec2*)RecordPP[ii];
				if (MeshRec2P->hasItem & STK_HAS_COLOR)
				{
					SetColor( *(STK_RGBA32*)MeshRec2P->color);
				}
				if (MeshRec2P->hasItem & STK_HAS_BOX)
				{
					SetBoundingBox(*(STK_BOX32*)MeshRec2P->box);
				}
				if (MeshRec2P->hasItem & STK_HAS_MAT)
				{
					SetMaterial(*(STK_MAT32*)MeshRec2P->mat);
				}
				if (MeshRec2P->IsItem & STK_IS_CLOSED)
				{
					SetClosed(TRUE);
				}
				if (MeshRec2P->IsItem & STK_IS_UNICOLOR)
				{ 
					SetUniColor(TRUE);
				}
				SetMeshType(MESH_STORE_LOD_UNSHARE_VERTEX); //版本2 默认是“LOD方式二”
			}
			else if( RecordPP[ii]->version == 3){
				/*
				 *  填充Mesh_Com(version 3)
				 */
				MeshRec3P = (StkMeshRec3*)RecordPP[ii];
				if (MeshRec3P->hasItem&STK_HAS_COLOR)
				{
					SetColor(*(STK_RGBA32*)MeshRec3P->color);
				}
				if (MeshRec3P->hasItem & STK_HAS_BOX)
				{
					SetBoundingBox(*(STK_BOX32*)MeshRec3P->box);
				}
				if (MeshRec3P->IsItem & STK_IS_CLOSED)
				{
					SetClosed(TRUE);
				}
				if (MeshRec3P->IsItem & STK_IS_UNICOLOR)
				{ 
					SetUniColor(TRUE);
				}
				if (MeshRec3P->IsItem & STK_IS_ONTOP)
				{ 
					SetDisplayOnTop(TRUE);
				}
// 				if (MeshRec3P->IsItem & STK_IS_DISPLAY)
// 				{
// 					SetDisplay(TRUE);
// 				}

				SetMeshType((StkMeshStoreTypeEnum)MeshRec3P->meshType);
			}
			else if( RecordPP[ii]->version == 4)
			{
				/*
				 *  填充Mesh_Com(version 4)
				 */
				MeshRec4P = (StkMeshRec4*)RecordPP[ii];
				if (MeshRec4P->hasItem&STK_HAS_COLOR)
				{
					SetColor(*(STK_RGBA32*)MeshRec4P->color);
				}
				if (MeshRec4P->hasItem & STK_HAS_BOX)
				{
					SetBoundingBox(*(STK_BOX32*)MeshRec4P->box);
				}
				if (MeshRec4P->IsItem & STK_IS_CLOSED)
				{
					SetClosed(TRUE);
				}
				if (MeshRec4P->IsItem & STK_IS_UNICOLOR)
				{ 
					SetUniColor(TRUE);
				}
				if (MeshRec4P->IsItem & STK_IS_ONTOP)
				{ 
					SetDisplayOnTop(TRUE);
				}
				if (MeshRec4P->IsItem & STK_IS_NO_DISPLAY)
				{
					SetDisplay(FALSE);
				}

				SetMeshType((StkMeshStoreTypeEnum)MeshRec4P->meshType);
			}
			else if( RecordPP[ii]->version == 5)
			{
				/*
				 *  填充Mesh_Com(version 5)
				 */
				MeshRec5P = (StkMeshRec5*)RecordPP[ii];
				if (MeshRec5P->hasItem&STK_HAS_COLOR)
				{
					SetColor(*(STK_RGBA32*)MeshRec5P->color);
				}
				if (MeshRec5P->hasItem & STK_HAS_BOX)
				{
					SetBoundingBox(*(STK_BOX32*)MeshRec5P->box);
				}
				if (MeshRec5P->hasItem & STK_HAS_MAT)
				{
					SetMaterial(*(STK_MAT32*)MeshRec5P->mat);
				}
				if (MeshRec5P->hasItem & STK_HAS_RENDER)
				{
					SetRenderID(MeshRec5P->idRender);
				}
				if (MeshRec5P->IsItem & STK_IS_CLOSED)
				{
					SetClosed(TRUE);
				}
				if (MeshRec5P->IsItem & STK_IS_UNICOLOR)
				{ 
					SetUniColor(TRUE);
				}
				if (MeshRec5P->IsItem & STK_IS_ONTOP)
				{ 
					SetDisplayOnTop(TRUE);
				}
				if (MeshRec5P->IsItem & STK_IS_NO_DISPLAY)
				{
					SetDisplay(FALSE);
				}

				SetMeshType((StkMeshStoreTypeEnum)MeshRec5P->meshType);
			}
			else
			{
				//ERROR
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_MESH_LOD)
		{
			/*
			 *  填充Mesh_Com_LOD
			 */
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				MeshLODRecP = (StkMeshLODRec*)RecordPP[ii];
				MeshLODInfo* MeshLODTmpP = NULL;
				MeshLODTmpP = new MeshLODInfo();
				if (MeshLODTmpP == NULL)
				{
					return STK_ERROR;
				}

				MeshLODTmpP->PloygonNum = MeshLODRecP->PolygonNum;
				MeshLODTmpP->AngularTol = MeshLODRecP->AngularTol;
				MeshLODTmpP->ChordError = MeshLODRecP->ChordError;
				nuElemcount = MeshLODRecP->FloatNum/3;	// 顶点+法线的个数
				MeshLODTmpP->Vertices.reserve(nuElemcount);  // 性能优化
				for (jj = 0; jj < nuElemcount; jj++)
				{
					MeshLODTmpP->Vertices.push_back(*(STK_PNT32*)(MeshLODRecP->atr + jj*3));
				}

				//建立LOD等级与MeshLODInfo的对应关系
				SetMeshLODInfo(MeshLODRecP->LODLevel, MeshLODTmpP);
				nuLevel = MeshLODRecP->LODLevel;
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_UV)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkUVRecP = (StkUVRec *)RecordPP[ii];
				STK_UINT32		nuUVNum = 0;
				MeshLODInfo*	MeshLODUVP = NULL;
				MeshLODUVP = GetMeshLODInfo(nuLevel);
				if (MeshLODUVP != NULL)
				{
					nuUVNum = stkUVRecP->FloatNum/3;
					for (int jj = 0; jj < nuUVNum; jj++)
					{
						MeshLODUVP->UVs.push_back(*(STK_PNT32*)(stkUVRecP->atr + jj*3));
					}
				}
			}
		}
		else if(RecordPP[ii]->type == REC_TYPE_FACE)
		{
			// 版本1已废弃
			if (RecordPP[ii]->version == 2)
			{
				FaceRec2P = (StkFaceRec2*)RecordPP[ii];
				CurMeshFaceP = new Stk_Mesh_Face(m_stkProtoP);
				CurMeshFaceP->RegisterID(FaceRec2P->faceId ); // 要改到Face ReadFile()里面

				/*
				 *  填充Mesh_Face
				 */
				if (FaceRec2P->hasItem & STK_HAS_COLOR)
				{
					CurMeshFaceP->SetColor( *(STK_RGBA32*)FaceRec2P->color);
				}
				if (FaceRec2P->hasItem & STK_HAS_BOX)
				{
					CurMeshFaceP->SetBoundingBox(*(STK_BOX32*)FaceRec2P->box);
				}
				if (FaceRec2P->hasItem & STK_HAS_MAT)
				{
					CurMeshFaceP->SetMaterial(*(STK_MAT32*)FaceRec2P->mat);
				}
				// 建立Face与Com的关联
				AddMeshFace(CurMeshFaceP);
			}

			else if (RecordPP[ii]->version == 3)	// 增加与面关联的边的ID，added by Shaoqy 2013/11/29
			{
				FaceRec3P = (StkFaceRec3*)RecordPP[ii];
				CurMeshFaceP = new Stk_Mesh_Face(m_stkProtoP);
				CurMeshFaceP->RegisterID(FaceRec3P->faceId ); // 要改到Face ReadFile()里面

				/*
				 *  填充Mesh_Face
				 */
				if (FaceRec3P->hasItem & STK_HAS_COLOR)
				{
					CurMeshFaceP->SetColor( *(STK_RGBA32*)FaceRec3P->color);
				}
				if (FaceRec3P->hasItem & STK_HAS_BOX)
				{
					CurMeshFaceP->SetBoundingBox(*(STK_BOX32*)FaceRec3P->box);
				}
				if (FaceRec3P->hasItem & STK_HAS_MAT)
				{
					CurMeshFaceP->SetMaterial(*(STK_MAT32*)FaceRec3P->mat);
				}
				for(int jj = 0; jj < FaceRec3P->MeshEdgeNum; jj++)
				{
					CurMeshFaceP->AddMeshEdgeID(FaceRec3P->atr[jj]); //可效率优化
				}

				// 建立Face与Com的关联
				AddMeshFace(CurMeshFaceP);
			}
			else if (RecordPP[ii]->version == 4)
			{
				FaceRec4P = (StkFaceRec4*)RecordPP[ii];
				CurMeshFaceP = new Stk_Mesh_Face(m_stkProtoP);
				CurMeshFaceP->RegisterID(FaceRec4P->faceId ); // 要改到Face ReadFile()里面

				/*
				 *  填充Mesh_Face
				 */
				if (FaceRec4P->hasItem & STK_HAS_COLOR)
				{
					CurMeshFaceP->SetColor( *(STK_RGBA32*)FaceRec4P->color);
				}
				if (FaceRec4P->hasItem & STK_HAS_BOX)
				{
					CurMeshFaceP->SetBoundingBox(*(STK_BOX32*)FaceRec4P->box);
				}
				if (FaceRec4P->hasItem & STK_HAS_MAT)
				{
					CurMeshFaceP->SetMaterial(*(STK_MAT32*)FaceRec4P->mat);
				}
				if (FaceRec4P->hasItem & STK_HAS_RENDER)
				{
					CurMeshFaceP->SetRenderID(FaceRec4P->idRender);
				}
				for(int jj = 0; jj < FaceRec4P->MeshEdgeNum; jj++)
				{
					CurMeshFaceP->AddMeshEdgeID(FaceRec4P->atr[jj]); //可效率优化
				}

				// 建立Face与Com的关联
				AddMeshFace(CurMeshFaceP);
			}
			else
			{
				//ERROR
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_FACE_LOD)
		{

			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				FaceLODRecP = (StkFaceLODRec*)RecordPP[ii];
				FaceLODTmpP = new FaceLODInfo();
				
				if (FaceLODTmpP == NULL)
				{
					return STK_ERROR;
				}
				if (CurMeshFaceP == NULL)
				{
					continue;
				}
				CurMeshFaceP->RegisterID(FaceLODRecP->FaceId); // 要改到Face ReadFile()里面 : WY_2013-8-27

				vector<STK_UINT32> vcPolyIndeies;
				UINT nuNum = FaceLODRecP->UintNum;
				vcPolyIndeies.reserve(nuNum);  // 性能优化
				for (jj = 0; jj < nuNum; jj++)
				{
					vcPolyIndeies.push_back(FaceLODRecP->atr[jj]);
				}

				FaceLODTmpP->TrianglesLOD = vcPolyIndeies;
				CurMeshFaceP->SetFaceLODInfo(FaceLODRecP->LODLevel, FaceLODTmpP);
			}
		}

		/*
		 * 平面基本属性
		 */
		else if (RecordPP[ii]->type == REC_TYPE_FACE_ATTR_PLN)
		{
			if(RecordPP[ii]->version == 1)
			{
				plnFaceAttrRecP = (StkPlnFaceAttrRec *)RecordPP[ii];
				stkPlnFaceAttrP = new Stk_PlaneFaceAttribute();
				if(stkPlnFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}
				stkPlnFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)plnFaceAttrRecP->AttrType);
				stkPlnFaceAttrP->SetID(plnFaceAttrRecP->ID);
				stkPlnFaceAttrP->SetNormal(plnFaceAttrRecP->Normal);
				stkPlnFaceAttrP->SetOrigin(plnFaceAttrRecP->Origin);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkPlnFaceAttrP);
			}
		}
		/*
		 * 圆柱面基本属性
		 */
		else if (RecordPP[ii]->type == REC_TYPE_FACE_ATTR_CYL){
			if(RecordPP[ii]->version == 1)
			{
				cyFaceAttrRecP = (StkCylinFaceAttrRec *)RecordPP[ii];
				stkCylinFaceAttrP = new Stk_CylinderFaceAttribute();
				if(stkCylinFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}
				stkCylinFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)cyFaceAttrRecP->AttrType);
				stkCylinFaceAttrP->SetID(cyFaceAttrRecP->ID);
				stkCylinFaceAttrP->SetRadius(cyFaceAttrRecP->Radius);
				stkCylinFaceAttrP->SetRevoAxis(cyFaceAttrRecP->RevoAxis);
				stkCylinFaceAttrP->SetAxisOrigin(cyFaceAttrRecP->AxisOrigin);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkCylinFaceAttrP);
			}
		}

		/*
		 * 圆锥面基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_FACE_ATTR_CON){
			if(RecordPP[ii]->version == 1)
			{
				conFaceAttrRecP = (StkConicalFaceAttrRec *)RecordPP[ii];
				stkConFaceAttrP = new Stk_ConicalFaceAttribute();
				if(stkConFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}
				stkConFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)conFaceAttrRecP->AttrType);
				stkConFaceAttrP->SetID(conFaceAttrRecP->ID);
				stkConFaceAttrP->SetRadius(conFaceAttrRecP->Radius);
				stkConFaceAttrP->SetAxisOrigin(conFaceAttrRecP->AxisOrigin);
				stkConFaceAttrP->SetRevoAxis(conFaceAttrRecP->RevoAxis);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkConFaceAttrP);
			}
		}

		/*
		 * 球面基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_FACE_ATTR_SPH){
			if(RecordPP[ii]->version == 1)
			{
				spFaceAttrRecP = (StkSphereFaceAttrRec *)RecordPP[ii];
				stkSpFaceAttrP = new Stk_SphereFaceAttribute();
				if(stkSpFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}
				stkSpFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)spFaceAttrRecP->AttrType);
				stkSpFaceAttrP->SetID(spFaceAttrRecP->ID);
				stkSpFaceAttrP->SetRadius(spFaceAttrRecP->Radius);
				stkSpFaceAttrP->SetAxisOrigin(spFaceAttrRecP->AxisOrigin);
				stkSpFaceAttrP->SetRevoAxis(spFaceAttrRecP->RevoAxis);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkSpFaceAttrP);
			}
		}

		/*
		 * 旋转面基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_FACE_ATTR_REV)
		{
			if(RecordPP[ii]->version == 1)
			{
				revoFaceAttrRecP = (StkRevoFaceAttrRec *)RecordPP[ii];
				stkRevoFaceAttrP = new Stk_RevolutionFaceAttribute();
				if(stkRevoFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}

				stkRevoFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)revoFaceAttrRecP->AttrType);
				stkRevoFaceAttrP->SetID(revoFaceAttrRecP->ID);
				stkRevoFaceAttrP->SetRadius(revoFaceAttrRecP->Radius);
				stkRevoFaceAttrP->SetAxisOrigin(revoFaceAttrRecP->AxisOrigin);
				stkRevoFaceAttrP->SetRevoAxis(revoFaceAttrRecP->RevoAxis);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkRevoFaceAttrP);
			}
		}

		/*
		 * 轮胎面基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_FACE_ATTR_TOR)
		{
			if(RecordPP[ii]->version == 1)
			{
				torFaceAttrRecP = (StkTorFaceAttrRec *)RecordPP[ii];
				stkTorFaceAttrP = new Stk_ToroidalFaceAttribute();
				if(stkTorFaceAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(CurMeshFaceP == NULL)
				{
					continue;
				}

				stkTorFaceAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)torFaceAttrRecP->AttrType);
				stkTorFaceAttrP->SetID(torFaceAttrRecP->ID);
				stkTorFaceAttrP->SetRadius(torFaceAttrRecP->Radius);
				stkTorFaceAttrP->SetAxisOrigin(torFaceAttrRecP->AxisOrigin);
				stkTorFaceAttrP->SetRevoAxis(torFaceAttrRecP->RevoAxis);
				stkTorFaceAttrP->SetMajorRaius(torFaceAttrRecP->MajorRadius);
				stkTorFaceAttrP->SetMinorRadius(torFaceAttrRecP->MinorRadius);
				CurMeshFaceP->SetGeoAttr((Stk_GeometryAttribute *)stkTorFaceAttrP);
			}
		}
		/*
		 * 边界线MeshEdge
		 */
		else if (RecordPP[ii]->type == REC_TYPE_MESHEDGE)
		{
			if(RecordPP[ii]->version == 1)
			{
				m_eMeshEgdeType = MESH_EDGE_STORE_IN_EDGE;

				MeshEdgeRecP = (StkMeshEdgeRec*)RecordPP[ii];
				stkMeshEdgeP = new Stk_Mesh_Edge(m_stkProtoP);
				if(stkMeshEdgeP == NULL)
				{
					return STK_ERROR;
				}
				ret = stkMeshEdgeP->RegisterID(MeshEdgeRecP->EdgeId);
				if(ret != STK_SUCCESS && ret != STK_SUCCESS_EXIST)
				{
					return STK_ERROR;
				}
				if(MeshEdgeRecP->hasItem & STK_HAS_BOX)
				{
					stkMeshEdgeP->SetBoundingBox(*(STK_BOX32*)MeshEdgeRecP->box);
				}
				if(MeshEdgeRecP->hasItem & STK_HAS_COLOR)
				{
					stkMeshEdgeP->SetColor(*(STK_RGBA32*)MeshEdgeRecP->color);
				}
				ret = AddMeshEdge(stkMeshEdgeP);
				if (ret != STK_SUCCESS)
				{
					delete stkMeshEdgeP;
					stkMeshEdgeP = NULL;
				}
			}
			else if (RecordPP[ii]->version == 2)
			{
				m_eMeshEgdeType = MESH_EDGE_STORE_IN_EDGE;

				MeshEdgeRec2P = (StkMeshEdgeRec2*)RecordPP[ii];
				stkMeshEdgeP = new Stk_Mesh_Edge(m_stkProtoP);
				if(stkMeshEdgeP == NULL)
				{
					return STK_ERROR;
				}
				ret = stkMeshEdgeP->RegisterID(MeshEdgeRec2P->EdgeId);
				if(ret != STK_SUCCESS && ret != STK_SUCCESS_EXIST)
				{
					return STK_ERROR;
				}
				if(MeshEdgeRec2P->hasItem & STK_HAS_BOX)
				{
					stkMeshEdgeP->SetBoundingBox(*(STK_BOX32*)MeshEdgeRec2P->box);
				}
				if(MeshEdgeRec2P->hasItem & STK_HAS_COLOR)
				{
					stkMeshEdgeP->SetColor(*(STK_RGBA32*)MeshEdgeRec2P->color);
				}
				stkMeshEdgeP->SetWidth(MeshEdgeRec2P->fWidth);

				ret = AddMeshEdge(stkMeshEdgeP);
				if (ret != STK_SUCCESS)
				{
					delete stkMeshEdgeP;
					stkMeshEdgeP = NULL;
				}
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_EDGE_LOD )
		{
			//注：没有添加Edge ID信息 : WY_2013-8-27
			// 暂时屏蔽边界线读取
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				if(m_eMeshEgdeType == MESH_EDGE_STORE_IN_FACE) 
				{
					EdgeLODRecP = (StkEdgeLODRec*)RecordPP[ii];
					vcPolylineIndices	EdgeIdx;
					for (int jj = 0; jj < EdgeLODRecP->uintnum; jj++)
					{
						EdgeIdx.push_back(EdgeLODRecP->atr[jj]);
					}
					FaceLODTmpP->EdgeLODIdx.push_back(EdgeIdx);
				}
				else if(m_eMeshEgdeType == MESH_EDGE_STORE_IN_EDGE)
				{
					EdgeLODRecP = (StkEdgeLODRec*)RecordPP[ii];
					EdgeIdxP = new vcEdgeInfo();
					for (int jj = 0; jj < EdgeLODRecP->uintnum; jj++)
					{
						EdgeIdxP->push_back(EdgeLODRecP->atr[jj]);
					}
					stkMeshEdgeP->SetEdgeLODInfo(EdgeLODRecP->LODLevel, EdgeIdxP);
				}
			}

#if 0 //最初的想法是将旧边界线转换为MeshEdge类
			if (RecordPP[ii]->version == 1)		
			{
				EdgeLODRecP = (StkEdgeLODRec*)RecordPP[ii];
				if(m_eMeshEgdeType == MESH_EDGE_STORE_IN_EDGE)
				{
					EdgeIdxP = new vcEdgeInfo();
					for (int jj = 0; jj < EdgeLODRecP->uintnum; jj++)
					{
						EdgeIdxP->push_back(EdgeLODRecP->atr[jj]);
					}
					stkMeshEdgeP->SetEdgeLODInfo(EdgeLODRecP->LODLevel, EdgeIdxP);
				}
				else if(m_eMeshEgdeType == MESH_EDGE_STORE_IN_FACE) // add by gaoqq on 2014.2.28 对旧边界线的兼容
				{
					if( EdgeLODRecP->LODLevel == 0)
					{
						stkMeshEdgeP = new Stk_Mesh_Edge(m_idProtoType);
						if(stkMeshEdgeP == NULL)
						{
							return STK_ERROR;
						}
						ret = stkMeshEdgeP->RegisterID(STK_NEW_ID);
						if(ret != STK_SUCCESS)
						{
							return STK_ERROR;
						}
						AddMeshEdge(stkMeshEdgeP);
						CurMeshFaceP->AddMeshEdgeID(stkMeshEdgeP->GetID());
					}

					EdgeIdxP = new vcEdgeInfo();
					for (int jj = 0; jj < EdgeLODRecP->uintnum; jj++)
					{
						EdgeIdxP->push_back(EdgeLODRecP->atr[jj]);
					}
					stkMeshEdgeP->SetEdgeLODInfo(EdgeLODRecP->LODLevel, EdgeIdxP);
				}
			}
#endif
		}
		/*
		 * 直线基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_EDGE_ATTR_LINE)
		{
			if(RecordPP[ii]->version == 1)
			{
				lineAttrRecP = (StkLineAttrRec *)RecordPP[ii];
				stkLineAttrP = new Stk_LineAttribute();
				if(stkLineAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(stkMeshEdgeP == NULL)
				{
					continue;
				}
				stkLineAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)(lineAttrRecP->AttrType));
				stkLineAttrP->SetID(lineAttrRecP->ID);
				stkLineAttrP->SetStartPoint(lineAttrRecP->pntStart);
				stkLineAttrP->SetEndPoint(lineAttrRecP->pntEnd);
				stkLineAttrP->SetCenterPoint(lineAttrRecP->pntCenter);
				stkLineAttrP->SetDirection(lineAttrRecP->dirDirect);
				stkLineAttrP->SetLength(lineAttrRecP->fLength);
				stkMeshEdgeP->SetGeoAttr((Stk_GeometryAttribute*)stkLineAttrP);
			}
		}
		/*
		 * Ellipse基本属性
		 */
		else if(RecordPP[ii]->type == REC_TYPE_EDGE_ATTR_ELLIPSE || RecordPP[ii]->type == REC_TYPE_EDGE_ATTR_CIRCLE)
		{
			if(RecordPP[ii]->version == 1)
			{
				ellipseAttrRecP = (StkEllipseAttrRec *)RecordPP[ii];
				stkEllipseAttrP = new Stk_EllipseAttribute();
				if(stkEllipseAttrP == NULL)
				{
					return STK_ERROR;
				}
				if(stkMeshEdgeP == NULL)
				{
					continue;
				}
				stkEllipseAttrP->SetGeoAttrType((StkGeoAttrTypeEnum)(ellipseAttrRecP->AttrType));
				stkEllipseAttrP->SetID(ellipseAttrRecP->ID);
				stkEllipseAttrP->SetStartPoint(ellipseAttrRecP->pntStart);
				stkEllipseAttrP->SetEndPoint(ellipseAttrRecP->pntEnd);
				stkEllipseAttrP->SetCenterPoint(ellipseAttrRecP->pntCenter);
				stkEllipseAttrP->SetMajorRadius(ellipseAttrRecP->fMajorRadius);
				stkEllipseAttrP->SetMinorRadius(ellipseAttrRecP->fMinorRadius);
				stkEllipseAttrP->SetXYZDir(ellipseAttrRecP->XDir,ellipseAttrRecP->YDir,ellipseAttrRecP->ZDir);
				stkMeshEdgeP->SetGeoAttr((Stk_GeometryAttribute*)stkEllipseAttrP);
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_MeshHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;

}

/*************************************************************************//**
 *	@brief		写Mesh数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		meshHead = {0};
	STK_STATUS			ret = STK_SUCCESS;
	STK_SIZE_T			szMetaDataNum = 0;

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

	m_eMeshEgdeType = MESH_EDGE_STORE_IN_EDGE;

	/*
	 *	【1.1】输出段头记录
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【Mesh段】MeshID：%d 构成面数：%d"), m_ID, m_vcSubFaces.size());
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	meshHead.id = m_ID;
	meshHead.kind = SEG_TYPE_MESH;
	ret = i_stkFileP->WriteSegmentHeadData(&meshHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Mesh段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	【1.2】输出Mesh记录
	 */
	StkMeshRec5 *meshrec5P = NULL;
	meshrec5P = (StkMeshRec5 *)calloc(1, sizeof(StkMeshRec5));
	if(meshrec5P == NULL){
		return STK_ERROR;
	}

	meshrec5P->hasItem = 0;
	// 颜色
	if(m_bHasColor){
		meshrec5P->color[0] = m_rgbaMesh.Red;		//R
		meshrec5P->color[1] = m_rgbaMesh.Green;		//G
		meshrec5P->color[2] = m_rgbaMesh.Blue;		//B
		meshrec5P->color[3] = m_rgbaMesh.Alpha;		//A
		meshrec5P->hasItem |= STK_HAS_COLOR;
	}else{
		meshrec5P->color[0] = -1;
		meshrec5P->color[1] = -1;
		meshrec5P->color[2] = -1;
		meshrec5P->color[3] = -1;
	}
	// 包围盒
	if(m_bHasBox){
		meshrec5P->box[0][0] = m_boxBounding.BoundBox[0][0];
		meshrec5P->box[0][1] = m_boxBounding.BoundBox[0][1];
		meshrec5P->box[0][2] = m_boxBounding.BoundBox[0][2];
		meshrec5P->box[1][0] = m_boxBounding.BoundBox[1][0];
		meshrec5P->box[1][1] = m_boxBounding.BoundBox[1][1];
		meshrec5P->box[1][2] = m_boxBounding.BoundBox[1][2];
		meshrec5P->hasItem |= STK_HAS_BOX;
	}else{
		meshrec5P->box[0][0] = 0;
		meshrec5P->box[0][1] = 0;
		meshrec5P->box[0][2] = 0;
		meshrec5P->box[1][0] = 0;
		meshrec5P->box[1][1] = 0;
		meshrec5P->box[1][2] = 0;
	}
	// 材质
	if(m_bHasMaterial){
		meshrec5P->mat[0] = m_matMesh.Ambient;
		meshrec5P->mat[1] = m_matMesh.Diffuse;
		meshrec5P->mat[2] = m_matMesh.Specular;
		meshrec5P->hasItem |= STK_HAS_MAT;
	}else{
		meshrec5P->mat[0] = -1;
		meshrec5P->mat[1] = -1;
		meshrec5P->mat[2] = -1;
	}

	// 渲染(Render)
	if(m_bHasRender)
	{
		meshrec5P->idRender = m_idRender;
		meshrec5P->hasItem |= STK_HAS_RENDER;
	}else{
		meshrec5P->idRender = 0;
	}

	// 是否闭合/统一颜色
	meshrec5P->IsItem  = 0;
	if(m_bIsClosed) meshrec5P->IsItem|=STK_IS_CLOSED; 
	if(m_bIsUniColor) meshrec5P->IsItem|=STK_IS_UNICOLOR; 
	if(m_bIsOnTop) meshrec5P->IsItem|=STK_IS_ONTOP; 
	if (!m_bIsDisplay)
	{
		meshrec5P->IsItem |= STK_IS_NO_DISPLAY;
	}

	meshrec5P->facenum = (UINT)m_vcSubFaces.size();
	meshrec5P->Control.type = REC_TYPE_MESH;
	meshrec5P->Control.length = sizeof(StkMeshRec5) - sizeof(StkControl);
	meshrec5P->Control.version = _VER_MESH_RECORD/*5*/;
	meshrec5P->meshType = m_eMeshType;		//Mesh存储类型

	//预留
	meshrec5P->usetype = 0;
	meshrec5P->product = 0;
	meshrec5P->dmy1 = m_eMeshEgdeType; // 边界线的存储类型
	meshrec5P->dmy2 = 0;
	meshrec5P->dmy3 = 0;

	ret = i_stkFileP->WriteSegmentRecordData(&meshHead, (StkControl *)meshrec5P);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Mesh段-Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(meshrec5P != NULL){
		free(meshrec5P);
		meshrec5P = NULL;
	}

	/*
	 *	【1.3】输出Mesh LOD记录
	 *	（LOD等级:0-8）
	 */
	map<STK_UINT32,MeshLODInfo*>::iterator itor;
	for( itor = m_mapMeshLODInfo.begin(); itor != m_mapMeshLODInfo.end(); itor++)
	{
		StkMeshLODRec*	meshLODRecP = NULL;
		StkUVRec*		UVRecP = NULL;

		MeshLODInfo *LODInfo = itor->second;
		meshLODRecP = (StkMeshLODRec *)calloc(1, sizeof(StkMeshLODRec) - sizeof(FLOAT) + 3*LODInfo->Vertices.size()*sizeof(FLOAT));
		if(meshLODRecP == NULL)
		{
			return STK_ERROR;
		}
		meshLODRecP->LODLevel = itor->first;
		meshLODRecP->AngularTol = LODInfo->AngularTol;
		meshLODRecP->ChordError = LODInfo->ChordError;
		meshLODRecP->PolygonNum = LODInfo->PloygonNum;
		meshLODRecP->FloatNum = (UINT)LODInfo->Vertices.size() * 3;

#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			_stprintf_s(strConvertLog,  _countof(strConvertLog), 
				_T("输出MeshLOD LOD等级：%d 三角形数：%d 弦高：%f 角度：%f"), meshLODRecP->LODLevel, meshLODRecP->PolygonNum, meshLODRecP->ChordError, meshLODRecP->AngularTol);
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		int jj = 0;
		for(int ii = 0; ii < LODInfo->Vertices.size(); ii++)
		{
			meshLODRecP->atr[jj++] = (LODInfo->Vertices)[ii].X;
			meshLODRecP->atr[jj++] = (LODInfo->Vertices)[ii].Y;
			meshLODRecP->atr[jj++] = (LODInfo->Vertices)[ii].Z;
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog,  _countof(strConvertLog), 
					_T("输出MeshLOD 面片顶点%d：%f %f %f"), ii, (LODInfo->Vertices)[ii].X, (LODInfo->Vertices)[ii].Y, (LODInfo->Vertices)[ii].Z);
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
		}

		meshLODRecP->Control.type = REC_TYPE_MESH_LOD;
		meshLODRecP->Control.length = sizeof(StkMeshLODRec) - sizeof(FLOAT) 
			+ (UINT)LODInfo->Vertices.size()*sizeof(FLOAT)*3 - sizeof(StkControl);
		meshLODRecP->Control.version = _VER_MESH_LOD_RECORD;

		ret = i_stkFileP->WriteSegmentRecordData(&meshHead, (StkControl *)meshLODRecP);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Mesh段-LOD记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(meshLODRecP != NULL)
		{
			free(meshLODRecP); 
			meshLODRecP = NULL;
		}

		// UV Record输出
		UVRecP = (StkUVRec *)calloc(1, sizeof(StkUVRec) - sizeof(FLOAT) + 3*LODInfo->UVs.size()*sizeof(FLOAT));
		if(UVRecP == NULL)
		{
			return STK_ERROR;
		}
		UVRecP->FloatNum = (UINT)LODInfo->UVs.size() * 3;

		int kk = 0;
		for(int ii = 0; ii < LODInfo->UVs.size(); ii++)
		{
			UVRecP->atr[kk++] = (LODInfo->UVs)[ii].X;
			UVRecP->atr[kk++] = (LODInfo->UVs)[ii].Y;
			UVRecP->atr[kk++] = (LODInfo->UVs)[ii].Z;
		}

		UVRecP->Control.type = REC_TYPE_UV;
		UVRecP->Control.length = sizeof(StkUVRec) - sizeof(FLOAT) 
			+ (UINT)LODInfo->UVs.size()*sizeof(FLOAT)*3 - sizeof(StkControl);
		UVRecP->Control.version = _VER_UV_RECORD;

		ret = i_stkFileP->WriteSegmentRecordData(&meshHead, (StkControl *)UVRecP);
		if(ret < STK_SUCCESS)
		{
			return STK_ERROR;
		}

		if(UVRecP != NULL)
		{
			free(UVRecP); 
			UVRecP = NULL;
		}
	}
	
	/*
	 *	【1.4】输出Face,Edge Record
	 */
	for(int ii = 0; ii < m_vcSubFaces.size(); ii++)
	{
		m_vcSubFaces[ii]->WriteFile(&meshHead, i_stkFileP, m_eMeshEgdeType);
	}
	
	/*
	 *	【1.5】输出Edge Record
	 */
	if( m_eMeshEgdeType == MESH_EDGE_STORE_IN_EDGE)
	{
		map<STK_ID, Stk_Mesh_Edge *>::iterator itorEdge;		//2013/11/29  Added by Shaoqy
		for(itorEdge = m_mapMeshEdges.begin(); itorEdge != m_mapMeshEdges.end(); itorEdge ++)
		{
			itorEdge->second->WriteFile(&meshHead, i_stkFileP);
		}
	}

	/*
	 *	完成Mesh Table输出
	 */
	ret = i_stkFileP->WriteSegmentFinishData(&meshHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Mesh段-Finish失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

#if 0
	/*
	 *	【第2步】 输出自定义属性段
	 */
	szMetaDataNum = m_vcMetaDatas.size();
	if (szMetaDataNum > 0)
	{
		WriteMetaData(i_stkFileP);
	}
#endif

	/*
	 *	【第2步】 输出用户数据段
	 */
	if(m_szUserDataCnt > 0)
	{
		WriteUserData(i_stkFileP);
	}


	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		注册ID
 *	@param[in]	i_idMeshCom			Mesh ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			ID重复注册失败
 *	@note		由于V2.2版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Com::RegisterID(STK_ID i_idMeshCom)
{
	Stk_File*	stkFileP = NULL;

	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegMeshComID(this, i_idMeshCom);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idMeshCom;
		return STK_SUCCESS;
	} 
	else
	{ 
		stkFileP = m_stkProtoP->GetFile();
		if (stkFileP == NULL)
		{
			return STK_ERROR;
		}
		wstring  wstrVer = stkFileP->GetSVLVersion();
		double dVer = wcstod(wstrVer.c_str(),NULL);
		if (dVer >= 2.33)
		{
			return STK_ERROR;
		}
		else
		{
			m_ID = i_idMeshCom;
			return STK_SUCCESS;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置Mesh的LOD等级
 *	@param[in]	i_nuLODLevel		LOD等级
 *	@param[in]	i_MeshLODInfoP		对应LOD等级的Mesh数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::SetMeshLODInfo(STK_UINT32 i_nuLODLevel, MeshLODInfo* i_MeshLODInfoP)
{
	if (i_nuLODLevel > 8 || i_MeshLODInfoP == NULL)
	{
		return STK_ERROR;
	}
	m_mapMeshLODInfo.insert(pair<STK_UINT32,MeshLODInfo*>(i_nuLODLevel, i_MeshLODInfoP));
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取MeshEdge的存储类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eMeshEgdeType
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkMeshEdgeStoreTypeEnum Stk_Mesh_Com::GetMeshEdgeType()
{
	return m_eMeshEgdeType;
}


/************************************************************************//**
 *	@brief		获取指定LOD等级下的Mesh数据
 *	@param[in]	i_nuLODLevel		LOD等级
 *	@param[out]	无
 *	@retval		指定等级的Mesh数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

MeshLODInfo* Stk_Mesh_Com::GetMeshLODInfo(STK_UINT32 i_nuLODLevel)
{
	if (i_nuLODLevel > 8)
	{
		return NULL;
	}
	map<STK_UINT32,MeshLODInfo*>::iterator itor = m_mapMeshLODInfo.find(i_nuLODLevel);
	if (itor == m_mapMeshLODInfo.end())
	{
		return NULL;
	}
	return itor->second;
}

/************************************************************************//**
 *	@brief		获取所有LOD对应的Mesh数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapMeshLODInfo	LOD与Mesh数据对应的映射
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const map<STK_UINT32,MeshLODInfo*>& Stk_Mesh_Com::GetAllMeshLOD()
{
	return m_mapMeshLODInfo;
}

/************************************************************************//**
 *	@brief		设置Mesh材质
 *	@param[in]	i_matMesh		Mesh材质
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Com::SetMaterial(const STK_MAT32 &i_matMesh)
{
	m_matMesh = i_matMesh; 
	m_bHasMaterial = TRUE;
}

/************************************************************************//**
 *	@brief		获取Mesh材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_matMesh		Mesh材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_MAT32 Stk_Mesh_Com::GetMaterial()
{
	return m_matMesh;
}

/************************************************************************//**
 *	@brief		判断Mesh是否有材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasMaterial		是否有Mesh材质标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Com::HasMaterial()
{
	return m_bHasMaterial;
}

/************************************************************************//**
 *	@brief		判断Mesh是否闭合
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsClosed		Mesh闭合状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Com::IsClosed()
{
	return m_bIsClosed;
}

/************************************************************************//**
 *	@brief		设置Mesh闭合状态
 *	@param[in]	inIsClosed		Mesh闭合状态
 *	@param[out]	无
 *	@retval		无	
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Com::SetClosed(STK_BOOL inIsClosed)
{
	m_bIsClosed = inIsClosed;
}

/****************************************************************************
 *	@brief		判断Mesh是否最前端显示
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsOnTop		是否最前端显示
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Mesh_Com::IsDisplayOnTop()
{
	return m_bIsOnTop;
}

/****************************************************************************
 *	@brief		设置Mesh是否最前端显示
 *	@param[in]	inIsOnTop		是否最前端显示
 *	@param[out]	无
 *	@retval		无	
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Mesh_Com::SetDisplayOnTop(STK_BOOL inIsOnTop)
{
	m_bIsOnTop = inIsOnTop;
}

/************************************************************************//**
 *	@brief		判断Mesh是否统一色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsUniColor	Mesh统一色状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Com::IsUniColor()
{
	return m_bIsUniColor;
}

/************************************************************************//**
 *	@brief		设置Mesh是否统一色
 *	@param[in]	i_bUniColor		Mesh统一色状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Com::SetUniColor(STK_BOOL i_bUniColor)
{
	m_bIsUniColor = i_bUniColor;
}

/************************************************************************//**
 *	@brief		获取Mesh存储类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuMeshType	Mesh存储类型
 *	@note		=1 不支持LOD方式 <br>
 *				=2 LOD方式一：不共享点列 <br>
 *				=3 LOD方式二：共享点列
 *	@attention	无
 ****************************************************************************/

StkMeshStoreTypeEnum Stk_Mesh_Com::GetMeshType()
{
	return m_eMeshType;
}

/************************************************************************//**
 *	@brief		设置Mesh存储类型
 *	@param[in]	i_eMeshType		Mesh存储类型
 *	@param[out]	无
 *	@retval		无
 *	@note		=1 不支持LOD方式 <br>
 *				=2 LOD方式一：不共享点列 <br>
 *				=3 LOD方式二：共享点列
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Com::SetMeshType(StkMeshStoreTypeEnum i_eMeshType)
{
	m_eMeshType = i_eMeshType;
}

/*************************************************************************//**
 *	@brief		增加Mesh下的边界线
 *	@param[in]	i_MeshEdgeP		边界线对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS		获取成功
  *	@retval		STK_ERROR		获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::AddMeshEdge(Stk_Mesh_Edge *i_MeshEdgeP)
{
	pair<map<STK_ID, Stk_Mesh_Edge*>::iterator, bool> ret;
	m_eMeshEgdeType = MESH_EDGE_STORE_IN_EDGE;

	if(i_MeshEdgeP == NULL)
	{
		return STK_ERROR;
	}
	else
	{
		ret = m_mapMeshEdges.insert(pair<STK_ID, Stk_Mesh_Edge*>(i_MeshEdgeP->GetID(), i_MeshEdgeP));
		if(ret.second)
		{
			return STK_SUCCESS;
		}
		else
		{
			return STK_SUCCESS_EXIST;
		}
	}
}

/*************************************************************************//**
 *	@brief		获取该MeshCom下所有MeshEdge的ID列
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		边界线ID列
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<STK_ID> Stk_Mesh_Com::GetMeshEdgeIDs()
{
	vector<STK_ID> vcEgdeIDs;
	map<STK_ID, Stk_Mesh_Edge*>::iterator itorEdge = m_mapMeshEdges.begin();

	while(itorEdge != m_mapMeshEdges.end())
	{
		vcEgdeIDs.push_back(itorEdge->first);
		itorEdge ++;
	}

	return vcEgdeIDs;
}

/*************************************************************************//**
 *	@brief		通过ID来获取MeshEdge
 *	@param[in]	i_idEdge		ID号
 *	@param[out]	无
 *	@retval		MeshEdge指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Edge* Stk_Mesh_Com::GetMeshEdgeByID(STK_ID i_idEdge)
{
	map<STK_ID, Stk_Mesh_Edge*>::iterator itor;

	itor = m_mapMeshEdges.find(i_idEdge);
	if (itor==m_mapMeshEdges.end())
	{
		return NULL;
	}

	return itor->second;
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::SetNode(Stk_Node* i_NodeP)
{
	m_NodeP = i_NodeP;
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
Stk_Node* Stk_Mesh_Com::GetNode()
{
	return m_NodeP;
}

/************************************************************************//**
 *	@brief		Stk_Mesh_Face构造函数
 *	@param[in]	i_stkProtoTypeP		Face所属的ProtoType对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Face::Stk_Mesh_Face(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;;
	m_ID = 0;
	m_bHasColor = FALSE;
	m_rgbaFace.Red = -1.0;
	m_rgbaFace.Green = -1.0;
	m_rgbaFace.Blue = -1.0;
	m_rgbaFace.Alpha = -1.0;
	m_bHasMaterial = FALSE;
	m_stkGeoAttrP = NULL;
	m_bHasRender = false;
	m_idRender = 0;
	//LOD
// 	map<STK_UINT32, FaceLODInfo*>::iterator itor = m_mapFaceLODInfo.begin();
// 	while (itor != m_mapFaceLODInfo.end())
// 	{
// 		itor->second = NULL;
// 		itor++;
// 	}
}

/****************************************************************************
 *	@brief		插入用户数据
 *	@param[in]	i_stkUserDataP		用户数据指针
 *	@retval		STK_SUCCESS			成功
 *	@retval		其它				失败
 *	@note		1.请new Stk_UserData对象并将用户数据流填入，然后使用ProtoType::AddUserData()插入
 *				2.通过“用户数据名称”+“用户数据ID”的方式来作为唯一标识
 *				  例如“类型”+“1” 表示第一个数据， “类型”+“2” 表示第二个数据
 *				  当两次设置的唯一标识相同时，会自动覆盖掉前次的数据
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::AddUserData(Stk_UserData* i_stkUserDataP)
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

/****************************************************************************
 *	@brief		通过名称获取用户数据列表
 *	@param[in]	用户数据名称
 *	@param[out]	无
 *	@retval		用户数据列表
 *	@note		无
 *	@attention	无
 ****************************************************************************/
map<STK_ID,Stk_UserData*>* Stk_Mesh_Com::GetUserDataByName(wstring i_wstrDataName)
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

/****************************************************************************
 *	@brief		删除指定的用户数据
 *	@param[in]	i_wstrDataName		用户数据名称
 *	@param[in]	i_id				用户数据ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			找到数据，删除成功
 *	@retval		STK_ERROR			没有找到数据，删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS  Stk_Mesh_Com::DelUserData( wstring i_wstrDataName, STK_ID i_id)
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

/*****************************************************************************
 *	@brief		从SVL文件中读取用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[in]	i_UserHeadP		自定义属性记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Com::ReadUserDatas(Stk_File *i_FileP, StkSegmentHead *i_UserHeadP)
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
				stkUserDataP = new Stk_UserData(m_stkProtoP);
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
				stkUserDataP = new Stk_UserData(m_stkProtoP);
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

/*****************************************************************************
 *	@brief		往SVL文件中填写用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Com::WriteUserData(Stk_File* i_stkFileP)
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

/************************************************************************//**
 *	@brief		判断Mesh是否显示/隐藏
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		显示/隐藏状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Mesh_Com::IsDisplay()
{
	return m_bIsDisplay;
}

/************************************************************************//**
 *	@brief		设置Mesh是否显示/隐藏
 *	@param[in]	i_bDisplay		显示/隐藏状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::SetDisplay(STK_BOOL i_bDisplay)
{
	m_bIsDisplay = i_bDisplay;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_Mesh_Face析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Face::~Stk_Mesh_Face(void)
{
	//LOD
	map<STK_UINT32, FaceLODInfo*>::iterator itor = m_mapFaceLODInfo.begin();
	while (itor != m_mapFaceLODInfo.end())
	{
		if (itor->second != NULL)
		{
			delete itor->second;
			itor->second = NULL;
		}
		itor++;
	}

	// 这里预留了删除Edge
	m_stkProtoP->GetProtoIDManager()->DelMeshFaceByID(m_ID);

}

/************************************************************************//**
 *	@brief		注册ID
 *	@param[in]	i_idMeshFace	Face ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		ID重复注册失败
 *	@note		由于V2.2版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Face::RegisterID(STK_ID i_idMeshFace)
{
	Stk_File*	stkFileP = NULL;

	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegMeshFaceID(this, i_idMeshFace);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idMeshFace;
		return STK_SUCCESS;
	} 
	else
	{
		stkFileP = m_stkProtoP->GetFile();
		if (stkFileP == NULL)
		{
			return STK_ERROR;
		}
		wstring  wstrVer = stkFileP->GetSVLVersion();
		double dVer = wcstod(wstrVer.c_str(),NULL);
		if (dVer >= 2.33)
		{
			return STK_ERROR;
		}
		else
		{
			m_ID = i_idMeshFace;
			return STK_SUCCESS;
		}
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写MeshFace数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[in]	i_head				数据段头指针
 *	@param[in]	i_eEdgeStroeType	MeshEdge的存储方式
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Face::WriteFile(StkSegmentHead *i_head, Stk_File *i_stkFileP, StkMeshEdgeStoreTypeEnum i_eEdgeStroeType)
{
	STK_STATUS				ret = STK_SUCCESS;
	StkFaceRec2				FaceRec2 = {0};
	StkFaceRec4				*FaceRecP4 = NULL;
	StkPlnFaceAttrRec		plnFaceAttrRec = {0};
	StkCylinFaceAttrRec		cyFaceAttrRec = {0};
	StkConicalFaceAttrRec	conFaceAttrRec = {0};
	StkSphereFaceAttrRec	spFaceAttrRec = {0};
	StkRevoFaceAttrRec		revoFaceAttrRec = {0};
	StkTorFaceAttrRec		torFaceAttrRec = {0};
	STK_UINT32				nuMeshEdgeNum = 0;
#ifdef _WINDOWS_
	WCHAR					strConvertLog[MAX_PATH + 512];
	Stk_SVLLog				stkSVLLog;
	Stk_LogFileEx*			stkSVLLogFile;
#endif
	
	nuMeshEdgeNum = m_vcMeshEdgeID.size();	

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第1步】 输出MeshFace记录
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("MeshFace ID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	if(i_eEdgeStroeType == MESH_EDGE_STORE_IN_FACE)
	{
		FaceRec2.Control.type = REC_TYPE_FACE;
		FaceRec2.Control.length = sizeof(StkFaceRec2) - sizeof(StkControl);
		FaceRec2.Control.version = /*_VER_FACE_RECORD*/ 2; //←这里比较特殊，同时支持两种格式的存储

		FaceRec2.faceId = m_ID;
		FaceRec2.hasItem = 0;
		// 颜色
		if( m_bHasColor){
			FaceRec2.color[0] = m_rgbaFace.Red;	//R
			FaceRec2.color[1] = m_rgbaFace.Green;	//G
			FaceRec2.color[2] = m_rgbaFace.Blue;	//B
			FaceRec2.color[3] = m_rgbaFace.Alpha;	//A
			FaceRec2.hasItem |= STK_HAS_COLOR;
		}else{
			FaceRec2.color[0] = -1;
			FaceRec2.color[1] = -1;
			FaceRec2.color[2] = -1;
			FaceRec2.color[3] = -1;
		}
		// 包围盒
		if( m_bHasBox){
			FaceRec2.box[0][0] = m_boxBounding.BoundBox[0][0];
			FaceRec2.box[0][1] = m_boxBounding.BoundBox[0][1];
			FaceRec2.box[0][2] = m_boxBounding.BoundBox[0][2];
			FaceRec2.box[1][0] = m_boxBounding.BoundBox[1][0];
			FaceRec2.box[1][1] = m_boxBounding.BoundBox[1][1];
			FaceRec2.box[1][2] = m_boxBounding.BoundBox[1][2];
			FaceRec2.hasItem |= STK_HAS_BOX;
		}else{
			FaceRec2.box[0][0] = 0;
			FaceRec2.box[0][1] = 0;
			FaceRec2.box[0][2] = 0;
			FaceRec2.box[1][0] = 0;
			FaceRec2.box[1][1] = 0;
			FaceRec2.box[1][2] = 0;
		}
		// 材质
		if(m_bHasMaterial){
			FaceRec2.mat[0] = m_matFace.Ambient;
			FaceRec2.mat[1] = m_matFace.Diffuse;
			FaceRec2.mat[2] = m_matFace.Specular;
			FaceRec2.hasItem |= STK_HAS_MAT;
		}else{
			FaceRec2.mat[0] = -1;
			FaceRec2.mat[1] = -1;
			FaceRec2.mat[2] = -1;
		}

		ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&FaceRec2);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshFace Base记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}
	}
	else if(i_eEdgeStroeType == MESH_EDGE_STORE_IN_EDGE)
	{
		FaceRecP4 = (StkFaceRec4 *)calloc(1, sizeof(StkFaceRec4)- sizeof(STK_ID) + nuMeshEdgeNum * sizeof(STK_ID));
		if(FaceRecP4 == NULL)
		{
			return STK_ERROR;
		}
		FaceRecP4->Control.type = REC_TYPE_FACE;
		FaceRecP4->Control.length = sizeof(StkFaceRec4) - sizeof(STK_ID) + nuMeshEdgeNum * sizeof(STK_ID)- sizeof(StkControl);
		FaceRecP4->Control.version = _VER_FACE_RECORD/*4*/;

		FaceRecP4->faceId = m_ID;
		FaceRecP4->hasItem = 0;
		// 颜色
		if( m_bHasColor){
			FaceRecP4->color[0] = m_rgbaFace.Red;	//R
			FaceRecP4->color[1] = m_rgbaFace.Green;	//G
			FaceRecP4->color[2] = m_rgbaFace.Blue;	//B
			FaceRecP4->color[3] = m_rgbaFace.Alpha;	//A
			FaceRecP4->hasItem |= STK_HAS_COLOR;
		}else{
			FaceRecP4->color[0] = -1;
			FaceRecP4->color[1] = -1;
			FaceRecP4->color[2] = -1;
			FaceRecP4->color[3] = -1;
		}
		// 包围盒
		if( m_bHasBox){
			FaceRecP4->box[0][0] = m_boxBounding.BoundBox[0][0];
			FaceRecP4->box[0][1] = m_boxBounding.BoundBox[0][1];
			FaceRecP4->box[0][2] = m_boxBounding.BoundBox[0][2];
			FaceRecP4->box[1][0] = m_boxBounding.BoundBox[1][0];
			FaceRecP4->box[1][1] = m_boxBounding.BoundBox[1][1];
			FaceRecP4->box[1][2] = m_boxBounding.BoundBox[1][2];
			FaceRecP4->hasItem |= STK_HAS_BOX;
		}else{
			FaceRecP4->box[0][0] = 0;
			FaceRecP4->box[0][1] = 0;
			FaceRecP4->box[0][2] = 0;
			FaceRecP4->box[1][0] = 0;
			FaceRecP4->box[1][1] = 0;
			FaceRecP4->box[1][2] = 0;
		}
		// 材质
		if(m_bHasMaterial){
			FaceRecP4->mat[0] = m_matFace.Ambient;
			FaceRecP4->mat[1] = m_matFace.Diffuse;
			FaceRecP4->mat[2] = m_matFace.Specular;
			FaceRecP4->hasItem |= STK_HAS_MAT;
		}else{
			FaceRecP4->mat[0] = -1;
			FaceRecP4->mat[1] = -1;
			FaceRecP4->mat[2] = -1;
		}
		// 纹理ID
		if(m_bHasRender){
			FaceRecP4->idRender = m_idRender;
			FaceRecP4->hasItem |= STK_HAS_RENDER;
		}else{
			FaceRecP4->idRender = 0;
		}

		// 关联的边的ID			added by Shaoqy on 2013/12/31
		FaceRecP4->MeshEdgeNum = nuMeshEdgeNum;
		for(int ii = 0; ii < nuMeshEdgeNum; ii ++)
		{
			FaceRecP4->atr[ii] = m_vcMeshEdgeID[ii];
		}

		ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)FaceRecP4);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshFace Base记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(FaceRecP4 != NULL){
			free(FaceRecP4);
			FaceRecP4 = NULL;
		}
	}

	/*
	 *	【第2步】 输出Face的LOD记录:0-8
	 */
	STK_UINT32 nuIndexNum = 0;
	STK_UINT32 nuCurLODEdgeNum = 0;
	map<STK_UINT32, FaceLODInfo *>::iterator itor;
	for(itor = m_mapFaceLODInfo.begin(); itor != m_mapFaceLODInfo.end(); itor++)
	{
		StkFaceLODRec *FaceLODRecP = NULL;
		FaceLODInfo * LODInfo = itor->second;
		nuIndexNum = LODInfo->TrianglesLOD.size();
		nuCurLODEdgeNum = LODInfo->EdgeLODIdx.size();

		FaceLODRecP = (StkFaceLODRec *)calloc(1, sizeof(StkFaceLODRec) - sizeof(UINT) + nuIndexNum * sizeof(UINT));
		if(FaceLODRecP == NULL){
			return STK_ERROR;
		}
		/*
		 *	属性填充
		 */
		FaceLODRecP->Control.type = REC_TYPE_FACE_LOD;
		FaceLODRecP->Control.length = sizeof(StkFaceLODRec) - sizeof(UINT) + nuIndexNum * sizeof(UINT) - sizeof(StkControl);
		FaceLODRecP->Control.version = _VER_FACE_LOD_RECORD;

		FaceLODRecP->FaceId = m_ID;
		FaceLODRecP->LODLevel = itor->first;
		FaceLODRecP->Edgenum = nuCurLODEdgeNum;
		FaceLODRecP->UintNum = nuIndexNum;
		for(int jj = 0 ; jj < nuIndexNum; jj++)
		{
			FaceLODRecP->atr[jj] = (LODInfo->TrianglesLOD)[jj];
		}
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
		{
			for (int jj = 0; jj < nuIndexNum/3; jj++)
			{
				_stprintf_s(strConvertLog,  _countof(strConvertLog), 
					_T("输出MeshLOD 面片顶点索引：%d %d %d"), (LODInfo->TrianglesLOD)[jj], (LODInfo->TrianglesLOD)[jj+1], (LODInfo->TrianglesLOD)[jj+2]);
				stkSVLLogFile->Log(strConvertLog);
			}
		}
#endif

		ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)FaceLODRecP);
		if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshFace LOD记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(FaceLODRecP != NULL){
			free(FaceLODRecP);
			FaceLODRecP = NULL;
		}
		

		//相应LOD下的Edge信息
		for(int ii = 0; ii < nuCurLODEdgeNum; ii++){

			StkEdgeLODRec*		EdgeLODRecP = NULL;
			STK_UINT32			EdgeIndexNum = 0;
			vcPolylineIndices	pCurPolylineIndices;

			EdgeIndexNum = (LODInfo->EdgeLODIdx[ii]).size(); 
			EdgeLODRecP = (StkEdgeLODRec *)calloc(1, sizeof(StkEdgeLODRec) - sizeof(UINT) + EdgeIndexNum * sizeof(UINT)); //### 当=0时
			if(EdgeLODRecP == NULL){
				continue;
			}

			EdgeLODRecP->Control.type = REC_TYPE_EDGE_LOD;
			EdgeLODRecP->Control.length = sizeof(StkEdgeLODRec) - sizeof(UINT) + EdgeIndexNum*sizeof(UINT) - sizeof(StkControl);
			EdgeLODRecP->Control.version = _VER_EDGE_RECORD;
			EdgeLODRecP->EdgeId = 0;	//### 暂时没有边界线ID
			EdgeLODRecP->LODLevel = itor->first;
			EdgeLODRecP->uintnum = EdgeIndexNum;
			pCurPolylineIndices =LODInfo->EdgeLODIdx[ii];
			if(pCurPolylineIndices.size() == 0){
				if(EdgeLODRecP != NULL){
					free(EdgeLODRecP);
					EdgeLODRecP = NULL;
				}
				continue;
			}

			for(int kk = 0; kk < EdgeIndexNum; kk++){
				EdgeLODRecP->atr[kk] = pCurPolylineIndices[kk];
			}

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)EdgeLODRecP);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MeshEdge LOD记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}

			if(EdgeLODRecP != NULL){
				free(EdgeLODRecP);
				EdgeLODRecP = NULL;
			}
		}

	}
	/*
	 *	【第3步】 输出Face的基本属性记录
	 */
	if (m_stkGeoAttrP != NULL)
	{
		switch (m_stkGeoAttrP->GetGeoAttrType()){
		// 
		case GEOATTR_TYPE_PLANEFACE:
			plnFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_PLN;
			plnFaceAttrRec.Control.length = sizeof(StkPlnFaceAttrRec) - sizeof(StkControl);
			plnFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			plnFaceAttrRec.AttrType = GEOATTR_TYPE_PLANEFACE;
			plnFaceAttrRec.ID = ((Stk_PlaneFaceAttribute *)m_stkGeoAttrP)->GetID();
			plnFaceAttrRec.Normal = ((Stk_PlaneFaceAttribute *)m_stkGeoAttrP)->GetNormal();
			plnFaceAttrRec.Origin = ((Stk_PlaneFaceAttribute *)m_stkGeoAttrP)->GetOrigin();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&plnFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PLANEFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}

			break;
		case GEOATTR_TYPE_CYLINDERFACE:
			cyFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_CYL;
			cyFaceAttrRec.Control.length = sizeof(StkCylinFaceAttrRec) - sizeof(StkControl);
			cyFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			cyFaceAttrRec.AttrType = GEOATTR_TYPE_CYLINDERFACE;
			cyFaceAttrRec.ID = ((Stk_CylinderFaceAttribute *)m_stkGeoAttrP)->GetID();
			cyFaceAttrRec.Radius = ((Stk_CylinderFaceAttribute *)m_stkGeoAttrP)->GetRadius();
			cyFaceAttrRec.AxisOrigin = ((Stk_CylinderFaceAttribute *)m_stkGeoAttrP)->GetAxisOrigin();
			cyFaceAttrRec.RevoAxis = ((Stk_CylinderFaceAttribute *)m_stkGeoAttrP)->GetRevoAxis();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&cyFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：CYLINDERFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;
		case GEOATTR_TYPE_CONICALFACE:
			conFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_CON;
			conFaceAttrRec.Control.length = sizeof(StkConicalFaceAttrRec) - sizeof(StkControl);
			conFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			conFaceAttrRec.AttrType = GEOATTR_TYPE_CONICALFACE;
			conFaceAttrRec.ID = ((Stk_ConicalFaceAttribute *)m_stkGeoAttrP)->GetID();
			conFaceAttrRec.AxisOrigin = ((Stk_ConicalFaceAttribute *)m_stkGeoAttrP)->GetAxisOrigin();
			conFaceAttrRec.Radius = ((Stk_ConicalFaceAttribute *)m_stkGeoAttrP)->GetRadius();
			conFaceAttrRec.RevoAxis = ((Stk_ConicalFaceAttribute *)m_stkGeoAttrP)->GetRevoAxis();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&conFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：CONICALFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;
		case GEOATTR_TYPE_SPHEREFACE:
			spFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_SPH;
			spFaceAttrRec.Control.length = sizeof(StkSphereFaceAttrRec) - sizeof(StkControl);
			spFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			spFaceAttrRec.AttrType = GEOATTR_TYPE_SPHEREFACE;
			spFaceAttrRec.ID = ((Stk_SphereFaceAttribute *)m_stkGeoAttrP)->GetID();
			spFaceAttrRec.AxisOrigin = ((Stk_SphereFaceAttribute *)m_stkGeoAttrP)->GetAxisOrigin();
			spFaceAttrRec.Radius = ((Stk_SphereFaceAttribute *)m_stkGeoAttrP)->GetRadius();
			spFaceAttrRec.RevoAxis = ((Stk_SphereFaceAttribute *)m_stkGeoAttrP)->GetRevoAxis();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&spFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：SPHEREFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;

		case GEOATTR_TYPE_REVOLUTIONFACE:
			revoFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_REV;
			revoFaceAttrRec.Control.length = sizeof(StkRevoFaceAttrRec) - sizeof(StkControl);
			revoFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			revoFaceAttrRec.AttrType = GEOATTR_TYPE_REVOLUTIONFACE;
			revoFaceAttrRec.ID = ((Stk_RevolutionFaceAttribute *)m_stkGeoAttrP)->GetID();
			revoFaceAttrRec.AxisOrigin = ((Stk_RevolutionFaceAttribute *)m_stkGeoAttrP)->GetAxisOrigin();
			revoFaceAttrRec.Radius = ((Stk_RevolutionFaceAttribute *)m_stkGeoAttrP)->GetRadius();
			revoFaceAttrRec.RevoAxis = ((Stk_RevolutionFaceAttribute *)m_stkGeoAttrP)->GetRevoAxis();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&revoFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：REVOLUTIONFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;

		case GEOATTR_TYPE_TOROIDALFACE:
			torFaceAttrRec.Control.type = REC_TYPE_FACE_ATTR_TOR;
			torFaceAttrRec.Control.length = sizeof(StkTorFaceAttrRec) - sizeof(StkControl);
			torFaceAttrRec.Control.version = _VER_FACEATTR_RECORD;
			torFaceAttrRec.AttrType = GEOATTR_TYPE_TOROIDALFACE;
			torFaceAttrRec.ID = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetID();
			torFaceAttrRec.AxisOrigin = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetAxisOrigin();
			torFaceAttrRec.Radius = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetRadius();
			torFaceAttrRec.RevoAxis = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetRevoAxis();
			torFaceAttrRec.MajorRadius = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetMajorRadius();
			torFaceAttrRec.MinorRadius = ((Stk_ToroidalFaceAttribute *)m_stkGeoAttrP)->GetMinorRadius();

			ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&torFaceAttrRec);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMesh)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：TOROIDALFACE ATTRIBUTE记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
			break;
		}
	}

	return STK_SUCCESS;
}

//STK_STATUS Stk_Mesh_Face::SetPolyIndeiesLOD(STK_UINT32 inLODLevel,vector<STK_UINT32>& inPolyIndeies)
//{
//	if (inLODLevel >8)
//	{
//		return STK_ERROR;
//	}
//	m_PolyIndeiesLOD.insert(pair<STK_UINT32,vector<STK_UINT32>>(inLODLevel,inPolyIndeies));
//	return STK_SUCCESS;
//}
//
//STK_STATUS Stk_Mesh_Face::GetPolyIndeiesLOD(STK_UINT32 inLODLevel,vector<STK_UINT32>& outPolyIndeies)
//{
//	if (inLODLevel >8)
//	{
//		return STK_ERROR;
//	}
//	map<STK_UINT32,vector<STK_UINT32>>::iterator itor = m_PolyIndeiesLOD.find(inLODLevel);
//	if (itor == m_PolyIndeiesLOD.end())
//	{
//		return STK_ERROR;
//	}
//	outPolyIndeies = itor->second;
//	return STK_SUCCESS;
//}

/************************************************************************//**
 *	@brief		设置Face的LOD等级
 *	@param[in]	i_nuLODLevel		LOD等级
 *	@param[in]	i_LODInfoP			对应LOD等级的Face数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Mesh_Face::SetFaceLODInfo(STK_UINT32 i_nuLODLevel, FaceLODInfo* i_LODInfoP)
{
	if (i_nuLODLevel > 8 || i_LODInfoP == NULL)
	{
		return STK_ERROR;
	}
	m_mapFaceLODInfo.insert(pair<STK_UINT32,FaceLODInfo*>(i_nuLODLevel,i_LODInfoP));

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定LOD等级下的Face数据
 *	@param[in]	i_nuLODLevel		LOD等级
 *	@param[out]	无
 *	@retval		itor->second		Face数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

FaceLODInfo* Stk_Mesh_Face::GetFaceLODInfo(STK_UINT32 i_nuLODLevel)
{
	if (i_nuLODLevel > 8)
	{
		return NULL;
	}

	map<STK_UINT32,FaceLODInfo*>::iterator itor = m_mapFaceLODInfo.find(i_nuLODLevel);
	if (itor == m_mapFaceLODInfo.end())
	{
		return NULL;
	}
	return itor->second;
}

/************************************************************************//**
 *	@brief		获取所有LOD等级对应的Face数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapFaceLODInfo	LOD与Face数据的映射
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const map<STK_UINT32, FaceLODInfo*>& Stk_Mesh_Face::GetAllFaceLOD()
{
	return m_mapFaceLODInfo;
}

/************************************************************************//**
 *	@brief		清空所有的LOD信息
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mesh_Face::ClearAllFaceLOD()
{
	m_mapFaceLODInfo.clear();
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置面的颜色
 *	@param[in]	i_rgbaFace		面颜色
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Face::SetColor(STK_RGBA32 i_rgbaFace)
{
	m_rgbaFace = i_rgbaFace; 
	m_bHasColor = TRUE;
}

/************************************************************************//**
 *	@brief		获取面的颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaFace	面颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_Mesh_Face::GetColor()
{
	return m_rgbaFace;
}

/************************************************************************//**
 *	@brief		判断面是否有颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasColor	面颜色状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Face::HasColor()
{
	return m_bHasColor;
}

/************************************************************************//**
 *	@brief		设置面的材质
 *	@param[in]	i_matFace		面材质
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Mesh_Face::SetMaterial(const STK_MAT32 &i_matFace)
{
	m_matFace = i_matFace; 
	m_bHasMaterial = TRUE;
}

/************************************************************************//**
 *	@brief		获取面的材质
 *	@param[in]	无	
 *	@param[out]	无
 *	@retval		m_matFace	面材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_MAT32 Stk_Mesh_Face::GetMaterial()
{
	return m_matFace;
}

/************************************************************************//**
 *	@brief		判断面是否有材质
 *	@param[in]	无	
 *	@param[out]	无
 *	@retval		m_bHasMaterial		是否有面材质的标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Mesh_Face::HasMaterial()
{
	return m_bHasMaterial;
}

/*************************************************************************//**
 *	@brief		增加与此面关联的MeshEdge的ID
 *	@param[in]	i_IDEdge	关联的边的ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Face::AddMeshEdgeID(STK_ID i_IDEdge)
{
	m_vcMeshEdgeID.push_back(i_IDEdge);
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取与此面关联的MeshEdge的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		与面相关联的边界线ID列
 *	@note		无
 *	@attention	无
 ****************************************************************************/
const vector<STK_ID>& Stk_Mesh_Face::GetMeshEdgeIDs()
{
	return m_vcMeshEdgeID;
}

/*************************************************************************//**
 *	@brief		获取与此面关联的MeshEdge
 *	@param[in]	i_idEdge		边界线ID
 *	@param[out]	无
 *	@retval		stkMeshEdgeP	MeshEdge对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mesh_Edge* Stk_Mesh_Face::GetMeshEdgeByID(STK_ID i_idEdge)
{
	Stk_Mesh_Edge *stkMeshEdgeP = NULL;

	m_stkProtoP->GetProtoIDManager()->GetMeshEdgeByID(i_idEdge, stkMeshEdgeP);

	return stkMeshEdgeP;
}

/*************************************************************************//**
 *	@brief		获取与此面关联的MeshEdge的条数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		nuEdgeNum		边界线条数
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_Mesh_Face::GetMeshEdgeNum()
{
	STK_UINT32 nuEdgeNum = 0;

	nuEdgeNum = m_vcMeshEdgeID.size();

	return nuEdgeNum;
}

/************************************************************************//**
 *	@brief		设置面的Render ID
 *	@param[in]	i_idRender		面的Render ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Face::SetRenderID(STK_ID i_idRender)
{
	m_idRender = i_idRender;
	m_bHasRender = true;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取面的Render ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		面的Render ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_Mesh_Face::GetRenderID()
{
	return m_idRender;
}

/************************************************************************//**
 *	@brief		通过ID获取指定的Render(渲染)信息
 *	@param[in]	i_idRender		Render ID
 *	@param[out]	o_stkRenderP	检索到的渲染对象
 *	@retval		STK_SUCCESS		检索成功
 *	@retval		其它			检索失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Face::GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP)
{
	STK_STATUS				eState = STK_SUCCESS;
	Stk_DocumentManager*	stkDocManagerP = NULL;
	Stk_File*				stkFileP = NULL;
	Stk_Render*				stkRenderP = NULL;
	wstring					wstrVersion = L"";
	string					strVersion = "";
	STK_FLOAT32				fVersion = 0;
	STK_UINT32				nuRenderNum = 0;

	stkFileP = m_stkProtoP->GetFile();
	if (stkFileP == NULL)
	{
		return STK_ERROR;
	}

	wstrVersion = stkFileP->GetSVLVersion();
	strVersion = Stk_Platform::WStringToString(wstrVersion);
	fVersion = atof(strVersion.c_str());
	if (fVersion > 2.354f)
	{
		nuRenderNum = stkFileP->GetRenderNum();
		for (int ii = 0; ii < nuRenderNum; ii++)
		{
			stkRenderP = stkFileP->GetRenderByIndex(ii);
			if (i_idRender == stkRenderP->GetID())
			{
				break;
			}
		}
	}
	else
	{
		stkDocManagerP = stkFileP->GetDocumentManager();
		if (stkDocManagerP == NULL)
		{
			return STK_ERROR;
		}
		eState = stkDocManagerP->GetGlobalIDManger()->GetRenderByID(m_stkProtoP->GetID(), i_idRender, stkRenderP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		if (stkRenderP == NULL)
		{
			return STK_ERROR;
		}
	}

	o_stkRenderP = stkRenderP;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置面的Render ID
 *	@param[in]	i_idRender		面的Render ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::SetRenderID(STK_ID i_idRender)
{
	m_idRender = i_idRender;
	m_bHasRender = true;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取面的Render ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		面的Render ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_Mesh_Com::GetRenderID()
{
	return m_idRender;
}

/************************************************************************//**
 *	@brief		通过ID获取指定的Render(渲染)信息
 *	@param[in]	i_idRender		Render ID
 *	@param[out]	o_stkRenderP	检索到的渲染对象
 *	@retval		STK_SUCCESS		检索成功
 *	@retval		其它			检索失败
 *	@note		无
 *	@attention	根据文件版本号区分纹理在ProtoType中还是File中
 ****************************************************************************/
STK_STATUS Stk_Mesh_Com::GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP)
{
	STK_STATUS				eState = STK_SUCCESS;
	Stk_DocumentManager*	stkDocManagerP = NULL;
	Stk_File*				stkFileP = NULL;
	Stk_Render*				stkRenderP = NULL;
	wstring					wstrVersion = L"";
	string					strVersion = "";
	STK_FLOAT32				fVersion = 0;
	STK_UINT32				nuRenderNum = 0;

	stkFileP = m_stkProtoP->GetFile();
	if (stkFileP == NULL)
	{
		return STK_ERROR;
	}

	wstrVersion = stkFileP->GetSVLVersion();
	strVersion = Stk_Platform::WStringToString(wstrVersion);
	fVersion = atof(strVersion.c_str());
	if (fVersion > 2.354f)
	{
		nuRenderNum = stkFileP->GetRenderNum();
		for (int ii = 0; ii < nuRenderNum; ii++)
		{
			stkRenderP = stkFileP->GetRenderByIndex(ii);
			if (i_idRender == stkRenderP->GetID())
			{
				break;
			}
		}
	}
	else
	{
		stkDocManagerP = stkFileP->GetDocumentManager();
		if (stkDocManagerP == NULL)
		{
			return STK_ERROR;
		}
		eState = stkDocManagerP->GetGlobalIDManger()->GetRenderByID(m_stkProtoP->GetID(), i_idRender, stkRenderP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		if (stkRenderP == NULL)
		{
			return STK_ERROR;
		}
	}
	
	o_stkRenderP = stkRenderP;

	return STK_SUCCESS;
}