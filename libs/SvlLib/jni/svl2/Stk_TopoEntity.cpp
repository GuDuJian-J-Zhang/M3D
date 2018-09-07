// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_TopoEntity.cpp
 *
 *	@brief	Body类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_TopoEntity.h"
#include "Stk_Manager.h"
#include "Stk_LineSetEntity.h"
#include "Stk_Platform.h"
#include "math.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_Body构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Body::Stk_Body(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_bDispStat = TRUE;
	m_bHasMaterial = FALSE;
	m_bHasColor = FALSE;
	m_strBodyName = L"";
	m_NodeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Body析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_Body::~Stk_Body(void)
{
	for (int ii = 0; ii < m_vcMeshComs.size(); ii++)
	{
		if (m_vcMeshComs[ii] != NULL)
		{
			delete m_vcMeshComs[ii];
			m_vcMeshComs[ii] = NULL;
		}
	}
	for (int ii = 0; ii < m_vcLineSets.size(); ii++)
	{
		if (m_vcLineSets[ii] != NULL)
		{
			delete m_vcLineSets[ii];
			m_vcLineSets[ii] = NULL;
		}
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
	for (int ii = 0; ii < m_vcSpacialPoints.size(); ii++)
	{
		if (m_vcSpacialPoints[ii] != NULL)
		{
			delete m_vcSpacialPoints[ii];
			m_vcSpacialPoints[ii] = NULL;
		}
	}

	// 删除所从属的Node节点
	if (m_NodeP != NULL)
	{
		m_NodeP->SetBody(NULL); //防止析构Node时再次析构Body造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}
	
	m_stkProtoP->GetProtoIDManager()->DelBodyByID(m_ID);
}

/************************************************************************//**
 *	@brief		往Body中添加Mesh数据
 *	@param[in]	i_MeshComP		Mesh类对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::AddMeshCom(Stk_Mesh_Com* i_MeshComP)
{
	m_vcMeshComs.push_back(i_MeshComP);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Body中Mesh的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		szNum		Mesh数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_Body::GetMeshComNum()
{
	STK_SIZE_T szNum = m_vcMeshComs.size();
	return szNum;
}

/************************************************************************//**
 *	@brief		获取Body中指定的Mesh
 *	@param[in]	i_nuKeyPose		指定的Mesh索引
 *	@param[out]	无
 *	@retval		StkMeshComP		Mesh对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Mesh_Com* Stk_Body::GetMeshCom(STK_UINT32 i_nuKeyPose)
{
	Stk_Mesh_Com* StkMeshComP =NULL;

	if (i_nuKeyPose > m_vcMeshComs.size())
	{
		return NULL;
	}

	StkMeshComP = m_vcMeshComs.at(i_nuKeyPose);

	return StkMeshComP;
}

/************************************************************************//**
 *	@brief		通过ID删除指定的MeshCom
 *	@param[in]	i_idMeshCom		指定的MeshCom ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		其他			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Body::DeleteMeshComByID(STK_ID i_idMeshCom)
{
	STK_STATUS			eState = STK_SUCCESS;
	Stk_ProtoType*		stkProtoTypeP = NULL;
	Stk_Mesh_Com*		stkMeshComP = NULL;
	Stk_Node*			stkNodeP = NULL;
	Stk_ProtoIDManager*	stkProtoIDManagerP = NULL;
	vector<Stk_Mesh_Com*>::iterator itor;

	stkProtoTypeP = GetProtoType();
	if (stkProtoTypeP == NULL)
	{
		return STK_ERROR;
	}
	stkProtoIDManagerP = stkProtoTypeP->GetProtoIDManager();
	if (stkProtoIDManagerP == NULL)
	{
		return STK_ERROR;
	}
	eState = stkProtoIDManagerP->GetMeshComByID(i_idMeshCom, stkMeshComP);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	stkNodeP = stkMeshComP->GetNode();
	if (stkNodeP == NULL)
	{
		for (itor = m_vcMeshComs.begin(); itor != m_vcMeshComs.end(); itor++)
		{
			if ((*itor)->GetID() == i_idMeshCom)
			{
				delete (*itor);
				m_vcMeshComs.erase(itor);
				break;
			}
		}
	}
	else if (stkMeshComP && stkNodeP)
	{
		delete stkNodeP; //1.删除MeshCom 2.移除与父Node关联 3.删除子Node
		stkNodeP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Body中所有的MeshCom
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		其他			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Body::DeleteAllMeshComs()
{
	Stk_Node*	stkNodeP = NULL;
	for (int ii = 0; ii < m_vcMeshComs.size(); ii++)
	{
		stkNodeP = m_vcMeshComs[ii]->GetNode();
		if (stkNodeP == NULL)
		{
			for (int ii  = 0; ii < m_vcMeshComs.size(); ii++)
			{
				delete m_vcMeshComs[ii];
				m_vcMeshComs[ii] = NULL;
			}
			m_vcMeshComs.clear();
		}
		else if (m_vcMeshComs[ii] && stkNodeP)
		{
			delete stkNodeP; //1.删除MeshCom 2.移除与父Node关联 3.删除子Node
			stkNodeP = NULL;
		}

	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Body ID
 *	@param[in]	i_idBody		Body ID <br>
 *				== STK_NEW_ID	注册新ID <br>
 *				== 其它			注册指定ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::RegisterID(STK_ID i_idBody)
{
	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegBodyID(this, i_idBody);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idBody;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Body ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID		Body ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Body::GetID()
{
	return m_ID;
}

/*************************************************************************//**
 *	@brief		设置Body ID
 *	@param[in]	i_ID			Body ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Body::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		获取当前Body所属的ProtoType的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idProtoType	ProtoType的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Body::GetProtoTypeID()
{
	return m_stkProtoP->GetID();
}

/************************************************************************//**
 *	@brief		设置Body颜色
 *	@param[in]	i_rgbaBody		Body颜色
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Body::SetColor(const STK_RGBA32 &i_rgbaBody)
{
	m_rgbaBody = i_rgbaBody; 
	m_bHasColor = TRUE;
}

/************************************************************************//**
 *	@brief		获取Body颜色
 *	@param[in]	无		
 *	@param[out]	无
 *	@retval		m_rgbaBody		Body颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_Body::GetColor()
{
	return m_rgbaBody;
}

/************************************************************************//**
 *	@brief		判断Body是否有颜色
 *	@param[in]	无		
 *	@param[out]	无
 *	@retval		m_bHasColor		是否有Body颜色标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Body::HasColor()
{
	return m_bHasColor;
}

/************************************************************************//**
 *	@brief		设置Body名称
 *	@param[in]	i_strName		Body名称
 *	@param[out]	无
 *	@retval		无
 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
 *	@attention	无
 ****************************************************************************/
void Stk_Body::SetName(const wstring &i_strName)
{
	m_strBodyName = i_strName;
	if (m_NodeP != NULL)
	{
		m_NodeP->SetNodeName(i_strName);
	}
}

/************************************************************************//**
 *	@brief		获取Body名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_strBodyName		Body名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring	 Stk_Body::GetName()
{
	return m_strBodyName;
}

/************************************************************************//**
 *	@brief		改变Body名称
 *	@param[in]	i_strName		Body名称
 *	@param[out]	无
 *	@retval		无
 *	@note		保持从Node到要素方向的名字一致
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Body::ChangeName(const wstring &i_strName)
{
	m_strBodyName = i_strName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置Body材质
 *	@param[in]	i_matBody		Body材质	
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Body::SetMaterial(const STK_MAT32 &i_matBody)
{
	m_matBody = i_matBody; 
	m_bHasMaterial = TRUE;
}

/************************************************************************//**
 *	@brief		获取Body材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_matBody		Body材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_MAT32 Stk_Body::GetMaterial()
{
	return m_matBody;
}

/************************************************************************//**
 *	@brief		判断是否有Body材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasMaterial	是否有Body材质的标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Body::HasMaterial()
{
	return m_bHasMaterial;
}

/************************************************************************//**
 *	@brief		设置Body显隐状态
 *	@param[in]	i_bDisplayState	Body显隐状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Body::SetDisplayState(STK_BOOL i_bDisplayState)
{
	m_bDispStat = i_bDisplayState;
}

/************************************************************************//**
 *	@brief		获取Body显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bDispStat		Body显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Body::GetDisplayState()
{
	return m_bDispStat;
}

/*************************************************************************//**
 *	@brief		添加线集
 *	@param[in]	i_stkLineSet		线集
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		STK_ERROR			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::AddLineSet(Stk_LineSet* i_stkLineSet)
{
	if (i_stkLineSet == NULL)
	{
		return STK_ERROR;
	}
	m_vcLineSets.push_back(i_stkLineSet);
	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		通过index删除线集
 *	@param[in]	i_stkLineSet		线集
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		STK_ERROR			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::DelLinSetByIndex(STK_UINT32 i_nuIndex)
{
	vector<Stk_LineSet*>::iterator itor;
	STK_UINT32	i = 0;

	if (i_nuIndex >= m_vcLineSets.size())
	{
		return STK_ERROR;
	}

	for (itor = m_vcLineSets.begin(); itor != m_vcLineSets.end(); itor++)
	{
		if ( i== i_nuIndex)
		{
			delete (*itor);
			m_vcLineSets.erase(itor);
			break;
		}
		i ++;
	}

	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		获取所有线集
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_LineSet*>& Stk_Body::GetAllLineSets()
{
	return m_vcLineSets;
}

/************************************************************************//**
 *	@brief		读取文件中的Body数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_BodyHeadP			Body记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR			读取失败
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_BodyHeadP)
{
	StkSegmentHead		head={0};
	StkBodyRec			*BodyRecP = NULL;
	StkBodyRec2			*BodyRec2P = NULL;
	StkBodyRec3			*BodyRec3P = NULL;
	StkBodyRec4			*BodyRec4P = NULL;
	StkBodyRec5			*BodyRec5P = NULL;
	StkControl			**RecordPP = NULL;
	Stk_Mesh_Com		*StkCurMeshComP = NULL;
	//Stk_MetaData*		StkMetaDataP = NULL;
	//STK_UINT32		nuMetaDataBelongFlag = 0;
	STK_UINT32			nuUserDataBelongFlag = 0;
	STK_UINT32			ii;
	STK_STATUS			eState;
	StkEntityNameRec*	stkEntityNameRecP = NULL;

	/*
	 * 读取Body段的数据
	 */
	RecordPP = (StkControl **)calloc(i_BodyHeadP->recnum, sizeof(StkControl*));
	if (i_BodyHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_BodyHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	RegisterID(i_BodyHeadP->id); //Body的注册ID，放到ReadFile里面

	/*
	 * 读取Body段的各记录
	 */
	for (ii = 0; ii < i_BodyHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_BODY)
		{
			if (RecordPP[ii]->version == 1)
			{
				BodyRecP = (StkBodyRec *)RecordPP[ii];

				if (BodyRecP->hasItem & STK_HAS_COLOR)
				{
					STK_RGBA32 rgbaTmp;
					rgbaTmp.Red = BodyRecP->color[0];
					rgbaTmp.Green = BodyRecP->color[1];
					rgbaTmp.Blue = BodyRecP->color[2];
					rgbaTmp.Alpha = BodyRecP->color[3];
					SetColor(rgbaTmp);
				}
				if (BodyRecP->hasItem & STK_HAS_MAT)
				{
					STK_MAT32 matTmp;
					matTmp.Ambient = BodyRecP->mat[0];
					matTmp.Diffuse = BodyRecP->mat[1];
					matTmp.Specular = BodyRecP->mat[2];
					SetMaterial(matTmp);
				}
				m_bDispStat = BodyRecP->disp;
			}
			else if (RecordPP[ii]->version == 2)
			{
				BodyRec2P = (StkBodyRec2 *)RecordPP[ii];

				if (BodyRec2P->hasItem & STK_HAS_COLOR)
				{
					STK_RGBA32 rgbaTmp;
					rgbaTmp.Red = BodyRec2P->color[0];
					rgbaTmp.Green = BodyRec2P->color[1];
					rgbaTmp.Blue = BodyRec2P->color[2];
					rgbaTmp.Alpha = BodyRec2P->color[3];
					SetColor(rgbaTmp);
				}
				if (BodyRec2P->hasItem & STK_HAS_MAT)
				{
					STK_MAT32 matTmp;
					matTmp.Ambient = BodyRec2P->mat[0];
					matTmp.Diffuse = BodyRec2P->mat[1];
					matTmp.Specular = BodyRec2P->mat[2];
					SetMaterial(matTmp);
				}
				m_bDispStat = BodyRec2P->disp;
			}
			else if (RecordPP[ii]->version == 3)
			{
				BodyRec3P = (StkBodyRec3 *)RecordPP[ii];
				if (BodyRec3P->hasItem & STK_HAS_COLOR)
				{
					STK_RGBA32 rgbaTmp;
					rgbaTmp.Red = BodyRec3P->color[0];
					rgbaTmp.Green = BodyRec3P->color[1];
					rgbaTmp.Blue = BodyRec3P->color[2];
					rgbaTmp.Alpha = BodyRec3P->color[3];
					SetColor(rgbaTmp);
				}
				if (BodyRec3P->hasItem & STK_HAS_MAT)
				{
					STK_MAT32 matTmp;
					matTmp.Ambient = BodyRec3P->mat[0];
					matTmp.Diffuse = BodyRec3P->mat[1];
					matTmp.Specular = BodyRec3P->mat[2];
					SetMaterial(matTmp);
				}
				m_bDispStat = BodyRec3P->disp;
#ifdef __MOBILE__
				m_strBodyName = Stk_Platform::STKWCHARStrToWString(BodyRec3P->chName); 
#else
				m_strBodyName = BodyRec3P->chName; 
#endif
			}
			else if (RecordPP[ii]->version == 4)
			{
				BodyRec4P = (StkBodyRec4 *)RecordPP[ii];
				if (BodyRec4P->hasItem & STK_HAS_COLOR)
				{
					STK_RGBA32 rgbaTmp;
					rgbaTmp.Red = BodyRec4P->color[0];
					rgbaTmp.Green = BodyRec4P->color[1];
					rgbaTmp.Blue = BodyRec4P->color[2];
					rgbaTmp.Alpha = BodyRec4P->color[3];
					SetColor(rgbaTmp);
				}
				if (BodyRec4P->hasItem & STK_HAS_MAT)
				{
					STK_MAT32 matTmp;
					matTmp.Ambient = BodyRec4P->mat[0];
					matTmp.Diffuse = BodyRec4P->mat[1];
					matTmp.Specular = BodyRec4P->mat[2];
					SetMaterial(matTmp);
				}
				m_bDispStat = BodyRec4P->disp;
#ifdef __MOBILE__
				m_strBodyName = Stk_Platform::STKWCHARStrToWString(BodyRec4P->chName); 
#else
				m_strBodyName = BodyRec4P->chName; 
#endif
			}
			else if (RecordPP[ii]->version == 5)
			{
				BodyRec5P = (StkBodyRec5 *)RecordPP[ii];
				if (BodyRec5P->hasItem & STK_HAS_COLOR)
				{
					STK_RGBA32 rgbaTmp;
					rgbaTmp.Red = BodyRec5P->color[0];
					rgbaTmp.Green = BodyRec5P->color[1];
					rgbaTmp.Blue = BodyRec5P->color[2];
					rgbaTmp.Alpha = BodyRec5P->color[3];
					SetColor(rgbaTmp);
				}
				if (BodyRec5P->hasItem & STK_HAS_MAT)
				{
					STK_MAT32 matTmp;
					matTmp.Ambient = BodyRec5P->mat[0];
					matTmp.Diffuse = BodyRec5P->mat[1];
					matTmp.Specular = BodyRec5P->mat[2];
					SetMaterial(matTmp);
				}
				m_bDispStat = BodyRec5P->disp;
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
				m_strBodyName = Stk_Platform::STKWCHARStrToWString(stkEntityNameRecP->atr); 
#else
				m_strBodyName = stkEntityNameRecP->atr; 
#endif
			}
		}
	}

	i_FileP->ReadSegmentFinishData(i_BodyHeadP, (StkControl **)RecordPP);// 释放内存
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
		eState = i_FileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return STK_ERROR_HEADRD;
		}

		switch (head.kind)
		{
		// 当下一段是Mesh表时
		case SEG_TYPE_MESH:
			StkCurMeshComP = new Stk_Mesh_Com(m_stkProtoP);
			eState = StkCurMeshComP->ReadFile(i_FileP, &head );
			if (eState!=STK_SUCCESS)
			{
				delete StkCurMeshComP;
				return STK_ERROR_HEADRD;
			}
			AddMeshCom(StkCurMeshComP);
			//nuMetaDataBelongFlag = 1;
			nuUserDataBelongFlag = 1;
			break;

		case SEG_TYPE_SUBLINE:
			eState = ReadSubLines(i_FileP, &head); // 旧的SubLine方式
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			break;
			
		case SEG_TYPE_LINESET:
			eState = ReadLineSets(i_FileP, &head); // 新的LineSet方式
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			break;
#if 0
		case SEG_TYPE_METADATA:
			switch(nuMetaDataBelongFlag)
			{
			case 0:
				eState = ReadMetaDatas(i_FileP, &head);
				if (eState!=STK_SUCCESS)
				{
					return STK_ERROR_HEADRD;
				}
				break;
			case 1:
				eState = StkCurMeshComP->ReadMetaDatas(i_FileP, &head);
				if (eState!=STK_SUCCESS)
				{
					return STK_ERROR_HEADRD;
				}
				break;
			default:
				break;
			}
			break;
#endif
		// 由于Mesh段没有END标志，因此这里做了区分UserData所属关系的特殊处理。
		case SEG_TYPE_USERDEFDATA:
			switch(nuUserDataBelongFlag)
			{
			case 0:
				// 暂时Body没有UserData
				break;
			case 1:
				eState = StkCurMeshComP->ReadUserDatas(i_FileP, &head);
				if (eState!=STK_SUCCESS)
				{
					return STK_ERROR_HEADRD;
				}
				break;
			default:
				break;
			}
			break;
		case SEG_TYPE_SPACIALPOINT:
			ReadSpacialPoints(i_FileP, &head);
			break;
		// 当下一段是Body-END表时
		case SEG_TYPE_BODY_END:
			goto rtn;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
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
 *	@brief		写Body数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Body::WriteFile(Stk_File *i_stkFileP)
{
	STK_UINT32			nuMeshNum = 0;
	STK_SIZE_T			szMetaDataNum = 0;
	StkSegmentHead		bodyHead = {0};
	StkSegmentHead		endHead = {0};
	StkBodyRec5			bodyRec5 = {0};
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	STK_UINT32			nuEntityNameSize = 0;
	STK_STATUS			ret = STK_SUCCESS;

#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
	STK_BOOL		bLogOutFlag = false;
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
	 * 【第1步】 输出Body段
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 
		(1 == stkSVLLog.bBody || 
		1 == stkSVLLog.bMesh))
	{
		bLogOutFlag = true;
	}
	else
	{
		bLogOutFlag = false;
	}
	if (bLogOutFlag)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【Body段】BodyID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// 【第1.1步】 输出段头记录
	nuMeshNum = m_vcMeshComs.size();
	bodyHead.id = m_ID;
	bodyHead.kind = SEG_TYPE_BODY;
	ret = i_stkFileP->WriteSegmentHeadData(&bodyHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Body段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}
	// 【第1.2步】 输出Body记录
	bodyRec5.Control.type = REC_TYPE_BODY;
	bodyRec5.Control.length = sizeof(StkBodyRec5)-sizeof(StkControl);
	bodyRec5.Control.version = _VER_BODY_RECORD /* 5 */;
	bodyRec5.disp = m_bDispStat;			//显隐状态
	bodyRec5.meshnum = m_vcMeshComs.size();
	bodyRec5.hasItem = 0;

	if(m_bHasColor){
		bodyRec5.color[0] = m_rgbaBody.Red;
		bodyRec5.color[1] = m_rgbaBody.Green;
		bodyRec5.color[2] = m_rgbaBody.Blue;
		bodyRec5.color[3] = m_rgbaBody.Alpha;
		bodyRec5.hasItem |= STK_HAS_COLOR;
	}else{
		bodyRec5.color[0] = -1;
		bodyRec5.color[1] = -1;
		bodyRec5.color[2] = -1;
		bodyRec5.color[3] = -1;
	}
	if(m_bHasMaterial){
		bodyRec5.mat[0] = m_matBody.Ambient;
		bodyRec5.mat[1] = m_matBody.Diffuse;
		bodyRec5.mat[2] = m_matBody.Specular;
		bodyRec5.hasItem |= STK_HAS_MAT;
	}else{
		bodyRec5.mat[0] = -1;
		bodyRec5.mat[1] = -1;
		bodyRec5.mat[2] = -1;
	}

	// 预留
	bodyRec5.usetype = 0;
	bodyRec5.dmy1 = 0;
	bodyRec5.dmy2 = 0;

	ret = i_stkFileP->WriteSegmentRecordData(&bodyHead, (StkControl*)&bodyRec5);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Body段-Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【第1.3步】 输出要素名称记录
	nuEntityNameSize = m_strBodyName.size() + 1;
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
	memcpy(stkEntityNameRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_strBodyName),(m_strBodyName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkEntityNameRecP->atr, nuEntityNameSize, (WCHAR*)m_strBodyName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(&bodyHead, (StkControl*)stkEntityNameRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (stkEntityNameRecP != NULL)
	{
		free(stkEntityNameRecP);
		stkEntityNameRecP = NULL;
	}

	// 【第1.4步】 完成Body段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&bodyHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Body段-Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 * 【第2步】 输出Mesh段
	 */
	for(int ii = 0; ii < nuMeshNum; ii++)
	{
		m_vcMeshComs[ii]->WriteFile(i_stkFileP);
	}

	/*
	 * 【第3步】 输出辅助线段
	 */
	if( m_vcSubLines.size()>0)
	{
		WriteSubLine(i_stkFileP);
	}
	
	/*
	 * 【第4步】 输出线集
	 */
	if (m_vcLineSets.size() > 0)
	{
		WriteLineSets(i_stkFileP);
	}

#if 0
	/*
	 *	【第5步】 输出自定义属性段
	 */
	szMetaDataNum = m_vcMetaDatas.size();
	if (szMetaDataNum > 0)
	{
		WriteMetaData(i_stkFileP);
	}
#endif

	/*
	 *	【第6步】 输出空间点集段
	 */
	if (m_vcSpacialPoints.size() > 0)
	{
		WriteSpacialPoints(i_stkFileP);
	}
	
	/*
	 * 【第7步】 输出Body-END段
	 */
#ifdef _WINDOWS_
	if (bLogOutFlag)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("输出Body-END BodyID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	endHead.id = m_ID;
	endHead.kind = SEG_TYPE_BODY_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Body-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		添加辅助线
 *	@param[in]	i_stkSubLine		辅助线
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		STK_ERROR			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::AddSubLine(Stk_CurveEntity* i_stkSubLine)
{
	if (i_stkSubLine == NULL)
	{
		return STK_ERROR;
	}
	m_vcSubLines.push_back(i_stkSubLine);
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所有辅助线
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_CurveEntity*>& Stk_Body::GetAllSubLines()
{
	return m_vcSubLines;
}


/*************************************************************************//**
 *	@brief		读取所有线集
 *	@param[in]	i_FileP				文件指针
 *	@param[in]	i_LineSetHeadP		线集段记录头
 *	@param[out]	无
 *	@retval		stat				读取所有线集状态
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::ReadLineSets(Stk_File *i_FileP, StkSegmentHead *i_LineSetHeadP)
{
	STK_STATUS		stat = STK_ERROR;
	Stk_LineSet*	StkLineSetP = NULL;

	StkLineSetP = new Stk_LineSet(m_stkProtoP);
	if (StkLineSetP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	stat = StkLineSetP->ReadFile(i_FileP, i_LineSetHeadP);
	if (stat != STK_SUCCESS)
	{
		delete StkLineSetP;
		return stat;
	}
	AddLineSet(StkLineSetP);

	return stat;
}

/************************************************************************//**
 *	@brief		将所有线集写入SVL
 *	@param[in]	i_stkFileP		File对象指针
 *	@param[out]	无
 *	@retval		stat			将所有线集写入SVL后的返回状态
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::WriteLineSets(Stk_File *i_stkFileP)
{
	STK_STATUS stat = STK_ERROR;
	for (int ii = 0; ii < m_vcLineSets.size(); ii++)
	{
		if (m_vcLineSets[ii] != NULL)
		{
			stat = m_vcLineSets[ii]->WriteLineSet(i_stkFileP);
			if (stat != STK_SUCCESS)
			{
				return stat;
			}
		}
	}

	return stat;
}

/*************************************************************************//**
 *	@brief		读取所有辅助线
 *	@param[in]	i_FileP				文件指针
 *	@param[in]	i_SubLineHeadP		辅助线记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			获取成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::ReadSubLines(Stk_File *i_FileP, StkSegmentHead *i_SubLineHeadP)
{
	StkSubLineRec*		SubLineRecP = NULL;
	StkPolyLineRec3*	PolyLineRecP3 = NULL;
	StkEllipseRec4*		EllipseRecP4 = NULL;
	StkLineRec3*		LineRecP3 = NULL;
	StkNurbsCurveRec*	NurbsCurveRecP = NULL;
	StkHyperbolaRec*	HyperbolaRecP = NULL;
	StkParabolaRec*		ParabolaRecP = NULL;

	Stk_PolyLine*		PolyLineP = NULL;
	Stk_Ellipse	*		EllipseP = NULL;
	Stk_Line*			LineP = NULL;
	Stk_NurbsCurve*		NurbsCurveP = NULL;
	Stk_Hyperbola*		HyperbolaP = NULL;
	Stk_Parabola*		ParabolaP = NULL;

	StkControl**		recordPP = NULL;
	STK_UINT32			ii, jj;

	STK_UINT32			nuNumKnotPlicity = 0;
	STK_UINT32			nuNumKnot = 0;
	STK_UINT32			nuNumPoint = 0;;
	STK_UINT32			nuNumWeight = 0;
	STK_PNT32			pntTmp;

	STK_UINT32			nuNumLines;
	STK_STATUS			eState;

	/*
	 * 读取辅助线段的数据
	 */
	recordPP = (StkControl **)calloc(1,sizeof(StkControl *) * i_SubLineHeadP->recnum);
	if (i_SubLineHeadP->recnum != 0 && recordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_SubLineHeadP, (StkControl **)recordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取辅助线的各记录
	 */
	for (ii = 0; ii < i_SubLineHeadP->recnum; ii++)
	{
		switch (recordPP[ii]->type)
		{
		case REC_TYPE_DMYLINE:
			// 预留
			break;
		case REC_TYPE_SUBLINE:
			// 预留
			break;
		case REC_TYPE_POLYLINE:
			if (recordPP[ii]->version == 3)
			{
				PolyLineRecP3 = (StkPolyLineRec3 *)recordPP[ii];
				PolyLineP = new Stk_PolyLine();
				/*
				 *	填充PolyLine类
				 */
				//Curve基类
				PolyLineP->SetID(PolyLineRecP3->ID);	//仅用于单独线条时
				PolyLineP->SetColor(PolyLineRecP3->Color);
				PolyLineP->SetUsageType((StkCurveUsageEnum)PolyLineRecP3->DefType);
				PolyLineP->SetCurveDispStat(PolyLineRecP3->nuDisp);
				PolyLineP->SetCurveDispType((StkCurveDispTypeEnum)PolyLineRecP3->nuDispType);

				vector<STK_PNT32> LinearPoints;
				STK_PNT32 TmpPoint;
				for (jj = 0; jj < PolyLineRecP3->NumPoints; jj++)
				{
					TmpPoint.X = PolyLineRecP3->atr[jj*3 + 0];
					TmpPoint.Y = PolyLineRecP3->atr[jj*3 + 1];
					TmpPoint.Z = PolyLineRecP3->atr[jj*3 + 2];
					LinearPoints.push_back(TmpPoint);
				}
				PolyLineP->SetPoints(LinearPoints);

				m_vcSubLines.push_back((Stk_CurveEntity *)PolyLineP);
			}
			break;
		case REC_TYPE_ELLIPSE:
			if (recordPP[ii]->version == 4)
			{
				EllipseRecP4 = (StkEllipseRec4 *)recordPP[ii];
				EllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				//Curve基类
				EllipseP->SetID(EllipseRecP4->ID);	//仅用于单独线条时
				EllipseP->SetColor(EllipseRecP4->Color);
				EllipseP->SetDomain(EllipseRecP4->CMin, EllipseRecP4->CMax);
				EllipseP->SetCenterPoint(EllipseRecP4->Center);
				EllipseP->SetXYZDir(EllipseRecP4->XDir, EllipseRecP4->YDir, EllipseRecP4->ZDir);
				EllipseP->SetRadius(EllipseRecP4->MajorRadius, EllipseRecP4->MinorRadius);
				EllipseP->SetCoordiantePnt(EllipseRecP4->BeginPnt, EllipseRecP4->EndPnt);
				EllipseP->SetParameter(EllipseRecP4->BeginPar, EllipseRecP4->EndPar);
				//版本2
				EllipseP->SetNormal(EllipseRecP4->Normal);
				EllipseP->SetOriginDir(EllipseRecP4->OriginDir);
				//版本3
				EllipseP->SetUsageType((StkCurveUsageEnum)EllipseRecP4->DefType);
				//版本4
				EllipseP->SetCurveDispStat(EllipseRecP4->nuDisp);
				EllipseP->SetCurveDispType((StkCurveDispTypeEnum)EllipseRecP4->nuDispType);

				m_vcSubLines.push_back((Stk_CurveEntity *)EllipseP);
			}
			break;
		case REC_TYPE_LINE:
			if (recordPP[ii]->version == 3)
			{
				LineRecP3 = (StkLineRec3 *)recordPP[ii];
				LineP = new Stk_Line();

				LineP->SetID( LineRecP3->ID);
				LineP->SetColor( LineRecP3->Color);
				LineP->SetStartPoint( LineRecP3->StartPnt);
				LineP->SetEndPoint( LineRecP3->EndPnt);
				// 版本2
				LineP->SetUsageType((StkCurveUsageEnum)LineRecP3->DefType);
				//版本3
				LineP->SetCurveDispStat(LineRecP3->nuDisp);
				LineP->SetCurveDispType((StkCurveDispTypeEnum)LineRecP3->nuDispType);

				m_vcSubLines.push_back((Stk_CurveEntity *)LineP);
			}
			break;
		case REC_TYPE_NURBSCURVE:
			if (recordPP[ii]->version == 1)
			{
				NurbsCurveRecP = (StkNurbsCurveRec *)recordPP[ii];
				NurbsCurveP = new Stk_NurbsCurve();

				NurbsCurveP->SetID(NurbsCurveRecP->ID);
				NurbsCurveP->SetColor(NurbsCurveRecP->Color);
				NurbsCurveP->SetUsageType((StkCurveUsageEnum)NurbsCurveRecP->nuDefType);
				NurbsCurveP->SetCurveDispStat(NurbsCurveRecP->nuDisp);
				NurbsCurveP->SetCurveDispType((StkCurveDispTypeEnum)NurbsCurveRecP->nuDispType);
				NurbsCurveP->SetDomain(NurbsCurveRecP->fUMin, NurbsCurveRecP->fUMax);
				NurbsCurveP->SetDegree(NurbsCurveRecP->nuDegree);

				nuNumKnotPlicity = NurbsCurveRecP->nuNumKnotPlicity;
				nuNumKnot = NurbsCurveRecP->nuNumKnot;
				nuNumPoint = NurbsCurveRecP->nuNumPoint;
				nuNumWeight = NurbsCurveRecP->nuNumWeight;

				for (jj = 0; jj < nuNumKnotPlicity; jj++)
				{
					NurbsCurveP->AddKnotMultiplicity(NurbsCurveRecP->atr[jj]); //FLOAT到UINT截断警告，已测试没有问题
				}

				for (jj = 0; jj < nuNumKnot; jj++)
				{
					NurbsCurveP->AddKnot(NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + jj]);
				}

				for (jj = 0; jj < nuNumPoint; jj++)
				{
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 0];
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 1];
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 2];
					NurbsCurveP->AddPoint(pntTmp);
				}

				for (jj = 0; jj < nuNumWeight; jj++)
				{
					NurbsCurveP->AddWeight(NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + nuNumPoint * 3 * sizeof(STK_FLOAT32) + jj]);
				}

				m_vcSubLines.push_back((Stk_CurveEntity *)NurbsCurveP);
			}
			break;
		case REC_TYPE_HYPERBOLA:
			if (recordPP[ii]->version == 1)
			{
				HyperbolaRecP = (StkHyperbolaRec *)recordPP[ii];
				HyperbolaP = new Stk_Hyperbola();

				HyperbolaP->SetID(HyperbolaRecP->ID);
				HyperbolaP->SetColor(HyperbolaRecP->Color);
				HyperbolaP->SetUsageType((StkCurveUsageEnum)HyperbolaRecP->nuDefType);
				HyperbolaP->SetCurveDispStat(HyperbolaRecP->nuDisp);
				HyperbolaP->SetCurveDispType((StkCurveDispTypeEnum)HyperbolaRecP->nuDispType);
				HyperbolaP->SetDomain(HyperbolaRecP->fUMin, HyperbolaRecP->fUMax);
				HyperbolaP->SetCenterPoint(HyperbolaRecP->pntCenter);
				HyperbolaP->SetStartPoint(HyperbolaRecP->pntStart);
				HyperbolaP->SetEndPoint(HyperbolaRecP->pntEnd);

				m_vcSubLines.push_back((Stk_CurveEntity *)HyperbolaP);
			}
			break;
		case REC_TYPE_PARABOLA:
			if (recordPP[ii]->version == 1)
			{
				ParabolaRecP = (StkParabolaRec *)recordPP[ii];
				ParabolaP = new Stk_Parabola();

				ParabolaP->SetID(ParabolaRecP->ID);
				ParabolaP->SetColor(ParabolaRecP->Color);
				ParabolaP->SetUsageType((StkCurveUsageEnum)ParabolaRecP->nuDefType);
				ParabolaP->SetCurveDispStat(ParabolaRecP->nuDisp);
				ParabolaP->SetCurveDispType((StkCurveDispTypeEnum)ParabolaRecP->nuDispType);
				ParabolaP->SetDomain(ParabolaRecP->fUMin, ParabolaRecP->fUMax);
				ParabolaP->SetStartPoint(ParabolaRecP->pntStart);
				ParabolaP->SetEndPoint(ParabolaRecP->pntEnd);
				ParabolaP->SetXYZDir(ParabolaRecP->dirX, ParabolaRecP->dirY, ParabolaRecP->dirZ);
				ParabolaP->SetFocalDistance(ParabolaRecP->fFocalDistance);

				m_vcSubLines.push_back((Stk_CurveEntity *)ParabolaP);
			}
			break;
		default:
			break;
		}
	}
	i_FileP->ReadSegmentFinishData(i_SubLineHeadP,(StkControl **)recordPP);// 释放内存
	if (recordPP != NULL)
	{
		free(recordPP);
		recordPP = NULL;
	}

	// SubLine 转 LineSet
	STK_BOOL bflg = FALSE;
	STK_FLOAT32 ftol = 0.001;
	i_FileP->GetLineSetConvertFlag(bflg,ftol);
	if(bflg){
		Stk_LineSet *stkCurLinesetP = NULL;
		Stk_CurveEntity *stkCurCurve = NULL; 
		STK_PNT32 StartPnt,EndPnt,LastPnt;
		nuNumLines = m_vcSubLines.size();
		for(ii=0; ii<nuNumLines; ii++)
		{
			stkCurCurve = m_vcSubLines[ii];
			if(stkCurCurve->GetCurveType() == CURVE_TYPE_POLYLINE)
			{
				const vector<STK_PNT32>& pnts=((Stk_PolyLine*)stkCurCurve)->GetPoints();
				StartPnt = pnts[0];
				EndPnt = pnts[pnts.size()-1];
			}
			else if(stkCurCurve->GetCurveType() == CURVE_TYPE_LINE)
			{
				StartPnt = ((Stk_Line*)stkCurCurve)->GetStartPoint();
				EndPnt =  ((Stk_Line*)stkCurCurve)->GetEndPoint();
			}

			if(ii==0||(fabs(StartPnt.X-LastPnt.X)> ftol
				|| fabs(StartPnt.Y-LastPnt.Y)> ftol
				|| fabs(StartPnt.Z-LastPnt.Z)> ftol))
			{
				stkCurLinesetP = new Stk_LineSet(m_stkProtoP);
				AddLineSet(stkCurLinesetP);
				stkCurLinesetP->RegisterID(STK_NEW_ID);
				stkCurLinesetP->SetName(L"");
				stkCurLinesetP->SetDispStat(TRUE);
			}

			LastPnt = EndPnt;
			stkCurLinesetP->AddLine(stkCurCurve);
		}
		m_vcSubLines.clear();
	}


	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		将所有辅助线写入SVL
 *	@param[in]	i_stkFileP			File对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			获取成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::WriteSubLine(Stk_File *i_stkFileP)
{
	StkSegmentHead		SubLineHead ={0};
	StkSubLineRec		SubLineRec = {0};
	INT					ret = 0;
	STK_UINT32			nuNumSubLine;
	STK_UINT32			nuCurveType;
	STK_UINT32			ii;

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

	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【辅助线段】输出 辅助线"));
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	nuNumSubLine = (STK_UINT32)m_vcSubLines.size();
	if(nuNumSubLine<=0)
	{
		return STK_SUCCESS;
	}

	// [第1步] 输出段头记录
	SubLineHead.id = 0;
	SubLineHead.kind = SEG_TYPE_SUBLINE;
	ret = i_stkFileP->WriteSegmentHeadData(&SubLineHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：辅助线段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// [第2步] 输出辅助线记录
	SubLineRec.Control.type = REC_TYPE_SUBLINE;
	SubLineRec.Control.length = sizeof(StkSubLineRec)-sizeof(StkControl);
	SubLineRec.Control.version = _VER_SUBLINE_RECORD;
	SubLineRec.nuNumSubLine = nuNumSubLine;

	ret = i_stkFileP->WriteSegmentRecordData(&SubLineHead, (StkControl *)&SubLineRec);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：辅助线段 Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}


	for (ii = 0; ii < nuNumSubLine; ii++)
	{
		nuCurveType = m_vcSubLines[ii]->GetCurveType();
		switch (nuCurveType)
		{
		case CURVE_TYPE_POLYLINE:
			((Stk_PolyLine *)m_vcSubLines[ii])->WritePolyline(&SubLineHead, i_stkFileP);
			break;
		case CURVE_TYPE_ELLIPSE:
			((Stk_Ellipse *)m_vcSubLines[ii])->WriteEllipse(&SubLineHead, i_stkFileP);
			break;
		case CURVE_TYPE_LINE:
			((Stk_Line *)m_vcSubLines[ii])->WriteLine(&SubLineHead, i_stkFileP);
			break;
		case CURVE_TYPE_NURBSCURVE:
			((Stk_NurbsCurve *)m_vcSubLines[ii])->WriteNurbsCurve(&SubLineHead, i_stkFileP);
			break;
		case CURVE_TYPE_HYPERBOLA:
			((Stk_Hyperbola *)m_vcSubLines[ii])->WriteHyperbola(&SubLineHead, i_stkFileP);
			break;
		case CURVE_TYPE_PARABOLA:
			((Stk_Parabola *)m_vcSubLines[ii])->WriteParabola(&SubLineHead, i_stkFileP);
			break;
		default:
			break;
		}
	}

	ret = i_stkFileP->WriteSegmentRecordData(&SubLineHead,(StkControl *)&SubLineRec);

	// [第3步] 完成辅助线段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&SubLineHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：辅助线段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

#if 0
/*****************************************************************************
 *	@brief		往当前Body中添加自定义属性数据
 *	@param[in]	i_MetaDataP		自定义属性数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	if (i_MetaDataP != NULL)
	{
		m_vcMetaDatas.push_back(i_MetaDataP);
	}
	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		获取当前Body中所有的自定义属性数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas	自定义属性数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_MetaData*>& Stk_Body::GetAllMetaDatas()
{
	return m_vcMetaDatas;
}

/*****************************************************************************
 *	@brief		获取当前Body中所有的自定义属性数据数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_Body::GetMetaDataNum()
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

STK_STATUS Stk_Body::ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP)
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
				// 属性名称
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
				//属性类型
				stkMetaDataP->SetType((StkMetaDataValueTypeEnum)MetaDataRecP->nuValueType);
				// 属性值
				nuNumValue = MetaDataRecP->nuNumValue;
				STK_WCHAR* wchValueP = new STK_WCHAR[nuNumValue]; //在Andriod下与结构体成员STK_WCHAR赋值，因此这里用STK_WCHAR而不是WCHAR
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
				// 属性单位
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

STK_STATUS Stk_Body::WriteMetaData(Stk_File* i_stkFileP)
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
 *	@brief		向当前Body中添加空间点
 *	@param[in]	i_stkSpacialPoint		空间点数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Body::AddSpacialPoints(Stk_SpacialPointEntity* i_stkSpacialPoint)
{
	if (i_stkSpacialPoint != NULL)
	{
		m_vcSpacialPoints.push_back(i_stkSpacialPoint);
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取当前Body中空间点的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		空间点的数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Body::GetSpacialPointNum()
{
	return m_vcSpacialPoints.size();
}

/*************************************************************************//**
 *	@brief		通过指定索引获取当前Body中指定的空间点
 *	@param[in]	i_nuIndex				指定索引
 *	@param[out]	无
 *	@retval		指定的空间点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpacialPointEntity* Stk_Body::GetSpacialPointByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcSpacialPoints.at(i_nuIndex);
}

/*************************************************************************//**
 *	@brief		从SVL文件中读取空间点数据
 *	@param[in]	i_FileP					SVL文件指针
 *	@param[in]	i_SpacialPointHeadP		空间点记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS				读取成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::ReadSpacialPoints(Stk_File* i_FileP, StkSegmentHead* i_SpacialPointHeadP)
{
	Stk_SpacialPointEntity*	stkSpacialPointP = NULL;
	StkSpacialPointRec*		SpacialPointRecP = NULL;
	StkControl**			RecordPP = NULL;
	STK_UINT32				nuCharNum = 0;
	STK_UINT32				ii, jj;
	STK_STATUS				eState = STK_SUCCESS;

	/*
	 *	读取空间点集段的数据
	 */
	RecordPP = (StkControl**)calloc(i_SpacialPointHeadP->recnum, sizeof(StkControl *));
	if (i_SpacialPointHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	i_FileP->ReadSegmentAllRecordData(i_SpacialPointHeadP, (StkControl **)RecordPP);
	/*
	 *	读取空间点集的各记录
	 */
	for (ii = 0; ii < i_SpacialPointHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_SPACIALPOINT)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				stkSpacialPointP = new Stk_SpacialPointEntity(m_stkProtoP);
				if (stkSpacialPointP == NULL)
				{
					return STK_ERROR_MEMORY;
				}

				SpacialPointRecP = (StkSpacialPointRec*)RecordPP[ii];
				nuCharNum = SpacialPointRecP->nuCharNum;
				STK_WCHAR* wchNameP = new STK_WCHAR[nuCharNum];
				if (nuCharNum > 0 && wchNameP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				stkSpacialPointP->SetID(SpacialPointRecP->idPoint);
				stkSpacialPointP->SetType((StkSpacialPointTypeEnum)SpacialPointRecP->nuType);
				stkSpacialPointP->SetDispStat(SpacialPointRecP->nuDispStat);
				stkSpacialPointP->SetColor(SpacialPointRecP->rgbaPoint);
				stkSpacialPointP->SetPntPosition(SpacialPointRecP->pntPos);
				for (jj = 0; jj < nuCharNum; jj++)
				{
					wchNameP[jj] = SpacialPointRecP->atr[jj];
				}
#ifdef __MOBILE__
				stkSpacialPointP->SetName(Stk_Platform::STKWCHARStrToWString(wchNameP));
#else
				stkSpacialPointP->SetName(wchNameP);
#endif
				m_vcSpacialPoints.push_back(stkSpacialPointP);
				if (wchNameP != NULL)
				{
					delete [] wchNameP;
					wchNameP = NULL;
				}
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_SpacialPointHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		向SVL文件中填写空间点数据
 *	@param[in]	i_stkFileP		File对象指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Body::WriteSpacialPoints(Stk_File *i_stkFileP)
{
	StkSegmentHead		SpacialPointHead = {0};
	StkSpacialPointRec	SpacialPointRec = {0};
	STK_INT32			ret = 0;
	STK_UINT32			nuSpacialPointNum = 0;
	STK_UINT32			ii;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	nuSpacialPointNum = m_vcSpacialPoints.size();
	if (nuSpacialPointNum <= 0)
	{
		return STK_SUCCESS;
	}

	// 【第1步】 输出段头记录
	SpacialPointHead.id = 0;
	SpacialPointHead.kind = SEG_TYPE_SPACIALPOINT;
	ret = i_stkFileP->WriteSegmentHeadData(&SpacialPointHead);
	if (ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第2步】 输出空间点集记录
	for (ii = 0; ii < nuSpacialPointNum; ii++)
	{
		m_vcSpacialPoints[ii]->WriteSpacialPoint(&SpacialPointHead, i_stkFileP);
	}

	// 【第3步】 完成空间点集段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&SpacialPointHead);
	if (ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

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
STK_STATUS Stk_Body::SetNode(Stk_Node* i_NodeP)
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
Stk_Node* Stk_Body::GetNode()
{
	return m_NodeP;
}