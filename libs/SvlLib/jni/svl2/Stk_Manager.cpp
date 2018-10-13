// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Manager.cpp
 *
 *	@brief	SVLToolKit命令管理器
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_File.h"
#include "Stk_Define.h"
#include "Stk_Manager.h"
#include "Stk_MeshEntity.h"

/************************************************************************//**
 *	@brief		Stk_GlobalIDManager构造函数
 *	@param[in]	无		
 *	@param[out]	无		
 *	@retval		无		
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

Stk_GlobalIDManager::Stk_GlobalIDManager(void)
{
	m_nuMaxLinkID = 0;
	//m_nuMaxFileLinkID = 0;
	m_nuMaxCADFileID = 0;
}

/************************************************************************//**
 *	@brief		Stk_GlobalIDManager析构函数
 *	@param[in]	无		
 *	@param[out]	无		
 *	@retval		无		
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

Stk_GlobalIDManager::~Stk_GlobalIDManager(void)
{
#if 0
	STK_ID IDInfo;
	if (m_mapFileID.size() > 0)
	{
		Stk_File_Map::iterator itor = m_mapFileID.begin();
		while (m_mapFileID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelFileID(&IDInfo);
		}
	}
	if (m_mapInstanceID.size() > 0)
	{
		Stk_Instance_Map::iterator itor = m_mapInstanceID.begin();
		while (m_mapInstanceID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelInstanceID(&IDInfo);
		}
	}
	if (m_mapProtoTypeID.size() > 0)
	{
		Stk_ProtoType_Map::iterator itor = m_mapProtoTypeID.begin();
		while (m_mapProtoTypeID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelProtoTypeID(&IDInfo);
		}
	}
#endif
}


/************************************************************************//**
 *	@brief		通过ID获取文件管理器
 *	@param[in]	i_idFile					文件类对象ID
 *	@param[out]	o_FileP						文件类对象指针
 *	@retval		STK_SUCCESS					获取成功	
 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetFileByID(STK_ID i_idFile, Stk_File* &o_FileP)
{
	Stk_File_Map::iterator itor;

	itor = m_mapFileID.find(i_idFile);

	if (itor != m_mapFileID.end())
	{
		o_FileP = itor->second;
		return STK_SUCCESS;
	}
	else{
		o_FileP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取实例
 *	@param[in]	i_idInstance				实例ID
 *	@param[out]	o_InstanceP					实例类对象指针
 *	@retval		STK_SUCCESS					获取成功	
 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetInstanceByID(STK_ID i_idInstance, Stk_Instance* &o_InstanceP)
{
	Stk_Instance_Map::iterator itor;

	itor = m_mapInstanceID.find(i_idInstance);

	if (itor != m_mapInstanceID.end())
	{
		o_InstanceP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_InstanceP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取ProtoType
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[out]	o_ProtoTypeP				ProtoType类对象指针
 *	@retval		STK_SUCCESS					获取成功	
 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetProtoTypeByID(STK_ID i_idProtoType, Stk_ProtoType* &o_ProtoTypeP)
{
	Stk_ProtoType_Map::iterator itor;

	itor = m_mapProtoTypeID.find(i_idProtoType);

	if (itor != m_mapProtoTypeID.end())
	{
		o_ProtoTypeP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_ProtoTypeP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		注册文件类对象ID
 *	@param[in]	i_FileP						文件类对象指针
 *	@param[out]	o_idFile					文件类对象ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_ERROR					注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegFileID(Stk_File* i_FileP, STK_ID &o_idFile)
{

	//Stk_FileLinkString_Map::iterator itor;

	// 如果是NEWID，则从LinkString中获取ID号
	// 如果是指定ID，则从输入参数中获取ID号
	if(o_idFile == STK_NEW_ID )
	{
		if (i_FileP == NULL || i_FileP->GetCADFilePath().empty())
		{
			return STK_ERROR;
		}
		if (GetCADFileIDByPath(i_FileP->GetCADFilePath(), o_idFile) != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		// 第一步： 在“全局ID存储：FileIDMap”中
		m_mapFileID.insert(pair<STK_ID, Stk_File*>(o_idFile, i_FileP));
	}
	else
	{
		// 注意！无论是转换器、浏览器，File都是由ProtoType->RegCADFile()来创建的。
		// 并且调用是都是STK_NEW_ID。因此当形参不等于STK_NEW_ID时错误处理。

		return STK_ERROR;
	}

	return STK_SUCCESS;
}

#if 0
/************************************************************************//**
 *	@brief		注册文件类对象ID
 *	@param[in]	i_stkFileP					文件类对象指针
 *	@param[out]	o_ID						文件类对象ID
 *	@retval		STK_SUCCESS					注册成功
  *	@retval		其它						注册失败
 *	@note		特殊情况，拥有注册Link与RegisterID的能力
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::RegFileLink(Stk_File* i_stkFileP, STK_ID &o_ID)
{
	Stk_FileLinkString_Map::iterator itor;
	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	// 为空，文件还没有注册
	if(m_mapFileLinkString.size() == 0)
	{
		o_ID = 1;
		m_mapFileLinkString.insert(pair<wstring, STK_ID>(i_stkFileP->GetSVLPath(), o_ID));
		m_nuMaxFileLinkID ++;
		//m_mapFileID.insert(pair<STK_ID, Stk_File*>(o_ID, i_stkFileP));
		return STK_SUCCESS;
	}
	// 非空，分两种情况，已注册和未注册
	else
	{
		itor = m_mapFileLinkString.find(i_stkFileP->GetSVLPath());
		if(itor != m_mapFileLinkString.end())
		{
			return STK_SUCCESS_EXIST;			// 文件已注册，返回
		}
		else
		{
			o_ID = ++ m_nuMaxFileLinkID;
			m_mapFileLinkString.insert(pair<wstring, STK_ID>(i_stkFileP->GetSVLPath(), o_ID));
			//m_mapFileID.insert(pair<STK_ID, Stk_File*>(o_ID, i_stkFileP));
			return STK_SUCCESS;
		}
	}
}
#endif

/************************************************************************//**
 *	@brief			注册实例ID
 *	@param[in]		i_InstanceP					实例指针
 *	@param[out]	io_idInstance					实例ID
 *	@retval			STK_SUCCESS					注册成功
 *	@retval			STK_ERROR					注册失败
 *	@note			无
 *	@attention		内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegInstanceID(
	Stk_Instance* i_InstanceP,
	STK_ID &io_idInstance
)
{
	pair<map<STK_ID, Stk_Instance*>::iterator, bool> ret;
	Stk_Instance_Map::iterator itor;
	if (i_InstanceP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if(io_idInstance==STK_NEW_ID || io_idInstance==STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapInstanceID.size() == 0)
		{
			io_idInstance = 1;
			m_mapInstanceID.insert(pair<STK_ID, Stk_Instance*>(io_idInstance, i_InstanceP));
			return STK_SUCCESS;
		}

		itor = m_mapInstanceID.end();
		itor --;
		io_idInstance = itor->first+1;

		// 插入InsID管理器
		m_mapInstanceID.insert(pair<STK_ID, Stk_Instance*>(io_idInstance, i_InstanceP));
		return STK_SUCCESS;
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else{
		ret = m_mapInstanceID.insert(pair<STK_ID, Stk_Instance*>(io_idInstance, i_InstanceP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册ProtoType ID
 *	@param[in]	i_ProtoTypeP				ProtoType指针
 *	@param[out]	io_idProtoType				ProtoType ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_ERROR					注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegProtoTypeID(
	Stk_ProtoType* i_ProtoTypeP,
	STK_ID &io_idProtoType
)
{
	pair<map<STK_ID, Stk_ProtoType*>::iterator, bool> ret;
	Stk_ProtoLinkString_Map::iterator itor;

	// 如果是NEWID，则从LinkString中获取ID号
	// 如果是指定ID，则从输入参数中获取ID号
	if(io_idProtoType == STK_NEW_ID || io_idProtoType == STK_DMY_ID)
	{
		if (i_ProtoTypeP == NULL || i_ProtoTypeP->GetLinkString().empty())
		{
			return STK_ERROR;
		}
		if (GetProtoIDByLinkString(i_ProtoTypeP->GetLinkString(), io_idProtoType) != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		// 第一步： 在“全局ID存储：ProtoTypeIDMap”中
		m_mapProtoTypeID.insert(pair<STK_ID, Stk_ProtoType*>(io_idProtoType, i_ProtoTypeP));
	}
	else
	{
		// 注意！从其它CAD直接转换来的ProtoType，因为不调用RegProtoTypeLink，因此这里要同步。
		// 例如：1.SView直接读取ProE文件（要与打开SVL文件区分对待）
		wstring wstrLink;
		WCHAR wchIDA[32];
		swprintf(wchIDA, 32, L"%d", io_idProtoType); //支持Andriod
		wstrLink = wchIDA;
		wstrLink += L"|New";

		m_mapProtoLinkString.insert(pair<wstring, STK_ID>( wstrLink, io_idProtoType));
		/*
		 *	为避免ID冲突，找寻最大ID
		 */
		if (m_nuMaxLinkID ++ < io_idProtoType)
		{
			m_nuMaxLinkID = io_idProtoType;
		}

		// 第一步： 在“全局ID存储：ProtoTypeIDMap”中
		ret = m_mapProtoTypeID.insert(pair<STK_ID, Stk_ProtoType*>(io_idProtoType, i_ProtoTypeP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过ID获取Body信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idBody					Body ID
 *	@param[out]	o_BodyP						Body类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetBodyByID(
	STK_ID i_idProtoType,
	STK_ID i_idBody,
	Stk_Body* &o_BodyP
	)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager = NULL;
	STK_STATUS eState;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_BodyP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取Body的ID
	eState = pProtoManager->GetBodyByID(i_idBody, o_BodyP);

	if (eState != STK_SUCCESS)
	{
		o_BodyP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取Mesh信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idMeshCom					Mesh ID
 *	@param[out]	o_MeshComP					Mesh类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetMeshComByID(
	STK_ID i_idProtoType,
	STK_ID i_idMeshCom,
	Stk_Mesh_Com* &o_MeshComP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_MeshComP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取MeshCom的ID
	eState = pProtoManager->GetMeshComByID( i_idMeshCom, o_MeshComP);

	if (eState != STK_SUCCESS)
	{
		o_MeshComP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取PMI信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idPMI						PMI ID
 *	@param[out]	o_stkPMIP					PMI类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetPMIByID(
	STK_ID i_idProtoType,
	STK_ID i_idPMI,
	Stk_PMI* &o_stkPMIP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if( itor != m_mapProtoTypeID.end()){
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else{
		o_stkPMIP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取PMI的ID
	state = pProtoManager->GetPMIByID( i_idPMI, o_stkPMIP);

	if( state != STK_SUCCESS){
		o_stkPMIP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取视图信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idView					视图 ID
 *	@param[out]	o_stkViewP					视图类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetViewByID(
	STK_ID i_idProtoType,
	STK_ID i_idView,
	Stk_View* &o_stkViewP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_stkViewP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取PMI的ID
	state = pProtoManager->GetViewByID(i_idView, o_stkViewP);

	if (state != STK_SUCCESS)
	{
		o_stkViewP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取视图与PMI的关联信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idNodeConnector			视图与PMI的关联ID
 *	@param[out]	o_NodeConnectorP			视图与PMI的类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetNodeConnectorByID(
	STK_ID i_idProtoType,
	STK_ID i_idNodeConnector,
	Stk_NodeConnector* &o_NodeConnectorP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_NodeConnectorP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取视图与PMI关联的ID
	state = pProtoManager->GetNodeConnectorByID(i_idNodeConnector, o_NodeConnectorP);

	if (state != STK_SUCCESS)
	{
		o_NodeConnectorP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过ID获取线集信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idLineSet					线集 ID
 *	@param[out]	o_LineSetP					线集对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetLineSetByID(
	STK_ID i_idProtoType,
	STK_ID i_idLineSet,
	Stk_LineSet* &o_LineSetP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_LineSetP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取视图与PMI关联的ID
	state = pProtoManager->GetLineSetByID(i_idLineSet, o_LineSetP);

	if (state != STK_SUCCESS)
	{
		o_LineSetP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过ID获取Face信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idMeshFace				Face ID
 *	@param[out]	o_MeshFaceP					Face类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetMeshFaceByID(
	STK_ID i_idProtoType,
	STK_ID i_idMeshFace,
	Stk_Mesh_Face* &o_MeshFaceP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_MeshFaceP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取MeshCom的ID
	eState = pProtoManager->GetMeshFaceByID(i_idMeshFace, o_MeshFaceP);

	if (eState != STK_SUCCESS)
	{
		o_MeshFaceP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取Edge信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idMeshEdge				Edge ID
 *	@param[out]	o_MeshEdgeP					Edge类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetMeshEdgeByID(
	STK_ID i_idProtoType,
	STK_ID i_idMeshEdge,
	Stk_Mesh_Edge* &o_MeshEdgeP
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_MeshEdgeP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取MeshCom的ID
	eState = pProtoManager->GetMeshEdgeByID(i_idMeshEdge, o_MeshEdgeP);
	if (eState != STK_SUCCESS)
	{
		o_MeshEdgeP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		通过ID获取Render信息
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_idRender					Render ID
 *	@param[out]	o_stkRenderP				Render类对象指针
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::GetRenderByID(STK_ID i_idProtoType,STK_ID i_idRender, Stk_Render* &o_stkRenderP)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_stkRenderP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	// 获取MeshCom的ID
	eState = pProtoManager->GetRenderByID(i_idRender, o_stkRenderP);
	if (eState != STK_SUCCESS)
	{
		o_stkRenderP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Body的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_BodyP						Body类对象指针
 *	@param[out]	o_idBody					Body ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegBodyID(
	STK_ID i_idProtoType,
	Stk_Body* i_BodyP,
	STK_ID &o_idBody
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_BodyP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idBody = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}

	eState = pProtoManager->RegBodyID(i_BodyP, o_idBody);
	if (eState != STK_SUCCESS)
	{
		o_idBody = -1;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Mesh的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_MeshComP					Mesh类对象指针
 *	@param[out]	o_idMeshCom					Mesh ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegMeshComID(
	STK_ID i_idProtoType,
	Stk_Mesh_Com* i_MeshComP,
	STK_ID &o_idMeshCom
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_MeshComP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idMeshCom = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}

	eState = pProtoManager->RegMeshComID(i_MeshComP, o_idMeshCom);
	if (eState != STK_SUCCESS)
	{
		o_idMeshCom = -1;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册PMI的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_stkPMIP					PMI类对象指针
 *	@param[out]	io_idPMI					PMI ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::RegPMIID(
	STK_ID		i_idProtoType,
	Stk_PMI		*i_stkPMIP,
	STK_ID		&io_idPMI
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if( i_stkPMIP == NULL){
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if( itor != m_mapProtoTypeID.end()){
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else{
		io_idPMI = 0;
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->RegPMIID( i_stkPMIP, io_idPMI);
	if( state != STK_SUCCESS){
		io_idPMI = 0;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册View的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_stkViewP					视图类对象指针
 *	@param[out]	io_idView					视图 ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegViewID(STK_ID i_idProtoType, Stk_View* i_stkViewP, STK_ID& io_idView)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_stkViewP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		io_idView = 0;
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->RegViewID(i_stkViewP, io_idView);
	if (state != STK_SUCCESS)
	{
		io_idView = 0;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册视图与PMI关联的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_stkNodeConnectorP			视图与PMI关联类对象指针
 *	@param[out]	o_idNodeConnector			视图与PMI关联 ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegNodeConnectorID(
	STK_ID i_idProtoType,
	Stk_NodeConnector* i_stkNodeConnectorP,
	STK_ID& o_idNodeConnector
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_stkNodeConnectorP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idNodeConnector = 0;
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->RegNodeConnectorID(i_stkNodeConnectorP, o_idNodeConnector);
	if (state != STK_SUCCESS)
	{
		o_idNodeConnector = 0;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册线集的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_stkLineSetP				线集对象指针
 *	@param[out]	o_idLineSet					线集 ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegLineSetID(
	STK_ID i_idProtoType,
	Stk_LineSet* i_stkLineSetP,
	STK_ID& o_idLineSet
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_stkLineSetP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idLineSet = 0;
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->RegLineSetID(i_stkLineSetP, o_idLineSet);
	if (state != STK_SUCCESS)
	{
		o_idLineSet = 0;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Face的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_MeshFaceP					Face类对象指针
 *	@param[out]	o_idMeshFace				Face ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegMeshFaceID(
	STK_ID i_idProtoType,
	Stk_Mesh_Face* i_MeshFaceP,
	STK_ID &o_idMeshFace
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_MeshFaceP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idMeshFace = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}

	eState = pProtoManager->RegMeshFaceID(i_MeshFaceP, o_idMeshFace);
	if (eState != STK_SUCCESS)
	{
		o_idMeshFace = -1;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Edge的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_MeshEdgeP					Edge类对象指针
 *	@param[out]	o_idMeshEdge				Edge ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegMeshEdgeID(
	STK_ID i_idProtoType,
	Stk_Mesh_Edge* i_MeshEdgeP,
	STK_ID &o_idMeshEdge
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_MeshEdgeP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		o_idMeshEdge = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}

	eState = pProtoManager->RegMeshEdgeID(i_MeshEdgeP, o_idMeshEdge);
	if (eState != STK_SUCCESS)
	{
		o_idMeshEdge = -1;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Render的ID
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[in]	i_stkRenderP				Render类对象指针
 *	@param[out]	io_idRender					Render ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::RegRenderID(STK_ID i_idProtoType, Stk_Render* i_stkRenderP, STK_ID &io_idRender)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_stkRenderP == NULL)
	{
		return STK_ERROR;
	}

	// 获取该零件的ProtoIDManager
	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		io_idRender = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}

	eState = pProtoManager->RegRenderID(i_stkRenderP, io_idRender);
	if (eState != STK_SUCCESS)
	{
		io_idRender = -1;
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册ProtoType LinkString与ID的映射表
 *	@param[in]	i_wstrLink					ProtoType LinkString
 *	@param[out]	o_idProtoType				ProtoType ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			Link已注册
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegProtoLinkString( 
	const wstring &i_wstrLink,
	STK_ID &o_idProtoType
)
{
	Stk_ProtoLinkString_Map::iterator itor;

	/*
	 *  【CASE1】：如果Link已注册，则返回ID
	 */
	itor = m_mapProtoLinkString.find(i_wstrLink);
	if (itor != m_mapProtoLinkString.end())
	{
		o_idProtoType = itor->second;
		return STK_SUCCESS_EXIST;
	}

	/*
	 *  【CASE2】：如果Link未注册，则发行新ID
	 */
	if (m_mapProtoLinkString.size() == 0)
	{
		o_idProtoType = 1;
		m_mapProtoLinkString.insert(pair<wstring,STK_ID>(i_wstrLink, o_idProtoType) );
		m_nuMaxLinkID ++;
		return STK_SUCCESS;
	}
	else
	{
		itor = m_mapProtoLinkString.end();
// 		itor --;
// 		outProtoTypeID = itor->second+1;
		o_idProtoType = ++m_nuMaxLinkID; //此处最大的LinkID应该是++m_nuMaxLinkID，而不是itor->second+1;
		m_mapProtoLinkString.insert( pair<wstring, STK_ID>(i_wstrLink, o_idProtoType));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType LinkString与ID的映射表
 *	@param[in]	i_wstrLink					ProtoType LinkString
 *	@param[out]	o_idProtoType				ProtoType ID
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetProtoIDByLinkString( 
	const wstring &i_wstrLink, 
	STK_ID &o_idProtoType 
)
{
	Stk_ProtoLinkString_Map::iterator itor;
	itor = m_mapProtoLinkString.find(i_wstrLink);

	if (itor != m_mapProtoLinkString.end())
	{
		o_idProtoType = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_idProtoType = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		删除ProtoType LinkString与ID的映射
 *	@param[in]	i_wstrLink					ProtoType LinkString
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelProtoLinkString(const wstring &i_wstrLink)
{
	m_mapProtoLinkString.erase(i_wstrLink);

	// add on 2013.6.29 by Gaoqq
	// ObjectID 回收 ###
	if (m_mapProtoLinkString.size() == 0)
	{
		m_nuMaxLinkID = 0;
	}

	return STK_SUCCESS;
}

#if 0
/************************************************************************//**
 *	@brief		删除FileLinkString与ID的映射
 *	@param[in]	i_wstrFileLink				File LinkString
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelFileLinkString(const wstring &i_wstrFileLink)
{
	m_mapFileLinkString.erase(i_wstrFileLink);
	if(m_mapFileLinkString.size() == 0)
	{
		m_nuMaxFileLinkID = 0;
	}
	return STK_SUCCESS;
}
#endif

/************************************************************************//**
 *	@brief		删除文件类对象与ID的映射
 *	@param[in]	i_idFile					文件类对象ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		这里暂时确定一个原则：IDMANAGER中的Del方法，只删除ID，不删除对象
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelFileID(STK_ID i_idFile)
{
	m_mapFileID.erase(i_idFile);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除实例类对象与ID的映射
 *	@param[in]	i_idInstance				实例类对象ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelInstanceID(STK_ID i_idInstance)
{
	m_mapInstanceID.erase(i_idInstance);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除ProtoType与ID的映射
 *	@param[in]	i_idProtoType				ProtoType ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelProtoTypeID(STK_ID i_idProtoType)
{

	// 第一步： 在“零件ID存储：ProtoIDManagerMap”中
	Stk_ProtoType_Map::iterator itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor == m_mapProtoTypeID.end())
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	//delete itor->second; //ProtoType*的释放在File的析构时执行
	m_mapProtoTypeID.erase(i_idProtoType);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_ProtoIDManager构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

Stk_ProtoIDManager::Stk_ProtoIDManager(void)
{
	//m_nuMaxNodeLinkID = 0;
}

/************************************************************************//**
 *	@brief		Stk_ProtoIDManager析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

Stk_ProtoIDManager::~Stk_ProtoIDManager(void)
{
	STK_ID IDInfo;
	//Body
	if (m_mapBodyID.size() != 0)
	{
		Stk_Body_Map::iterator itor = m_mapBodyID.begin();
		while (m_mapBodyID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelBodyByID( IDInfo);
		}
	}
	//Mesh
	if (m_mapMeshComID.size() != 0)
	{
		Stk_MeshCom_Map::iterator itor = m_mapMeshComID.begin();
		while (m_mapMeshComID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelMeshComByID( IDInfo);
		}
	}
	//Face
	if (m_mapMeshFaceID.size() != 0)
	{
		Stk_MeshFace_Map::iterator itor = m_mapMeshFaceID.begin();
		while (m_mapMeshFaceID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelMeshFaceByID( IDInfo);
		}
	}
	//Edge
	if (m_mapMeshEdgeID.size() != 0)
	{
		Stk_MeshEdge_Map::iterator itor = m_mapMeshEdgeID.begin();
		while (m_mapMeshEdgeID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelMeshEdgeByID( IDInfo);
		}
	}
	//PMI
	if (m_mapPMIID.size() != 0)
	{
		Stk_PMI_Map::iterator itor = m_mapPMIID.begin();
		while (m_mapPMIID.size() != 0)
		{
			IDInfo = itor->first;
			itor++;
			DelPMIByID(IDInfo);
		}
	}
}

/************************************************************************//**
 *	@brief		通过ID获取Body信息
 *	@param[in]	i_idBody				Body ID
 *	@param[out]	o_BodyP					Body类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetBodyByID(
	STK_ID i_idBody,
	Stk_Body* &o_BodyP
)
{
	Stk_Body_Map::iterator itor;

	itor = m_mapBodyID.find(i_idBody);
	if (itor != m_mapBodyID.end())
	{
		o_BodyP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_BodyP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取Mesh信息
 *	@param[in]	i_idMeshCom				Mesh ID
 *	@param[out]	o_MeshComP				Mesh类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetMeshComByID(
	STK_ID i_idMeshCom,
	Stk_Mesh_Com* &o_MeshComP
)
{
	Stk_MeshCom_Map::iterator itor;

	itor = m_mapMeshComID.find(i_idMeshCom);
	if (itor != m_mapMeshComID.end())
	{
		o_MeshComP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_MeshComP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取Face信息
 *	@param[in]	i_idMeshFace			Face ID
 *	@param[out]	o_MeshFaceP				Face类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetMeshFaceByID(
	STK_ID i_idMeshFace,
	Stk_Mesh_Face* &o_MeshFaceP
)
{
	Stk_MeshFace_Map::iterator itor;

	itor = m_mapMeshFaceID.find(i_idMeshFace);
	if (itor != m_mapMeshFaceID.end())
	{
		o_MeshFaceP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_MeshFaceP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取Edge信息
 *	@param[in]	i_idMeshEdge			Edge ID
 *	@param[out]	o_MeshEdgeP				Edge类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetMeshEdgeByID(
	STK_ID i_idMeshEdge,
	Stk_Mesh_Edge* &o_MeshEdgeP
)
{
	Stk_MeshEdge_Map::iterator itor;

	itor = m_mapMeshEdgeID.find(i_idMeshEdge);
	if (itor != m_mapMeshEdgeID.end())
	{
		o_MeshEdgeP = itor->second;
		return STK_SUCCESS;
	}
	else{
		o_MeshEdgeP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取PMI信息
 *	@param[in]	i_idPMI					PMI ID
 *	@param[out]	o_stkPMIP				PMI类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetPMIByID(
	STK_ID i_idPMI,
	Stk_PMI* &o_stkPMIP
)
{
	Stk_PMI_Map::iterator itor;

	itor = m_mapPMIID.find(i_idPMI);
	if( itor != m_mapPMIID.end()){
		o_stkPMIP = itor->second;
		return STK_SUCCESS;
	}
	else{
		o_stkPMIP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取视图信息
 *	@param[in]	i_idView				视图 ID
 *	@param[out]	o_stkViewP				视图类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetViewByID(
	STK_ID i_idView,
	Stk_View* &o_stkViewP
)
{
	Stk_View_Map::iterator itor;

	itor = m_mapViewID.find(i_idView);
	if (itor != m_mapViewID.end())
	{
		o_stkViewP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_stkViewP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取视图与PMI的关联信息
 *	@param[in]	i_idNodeConnector		视图与PMI的关联 ID
 *	@param[out]	o_stkNodeConnectorP		视图与PMI的关联类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetNodeConnectorByID(
	STK_ID i_idNodeConnector,
	Stk_NodeConnector* &o_stkNodeConnectorP
)
{
	Stk_NodeConnector_Map::iterator itor;

	itor = m_mapNodeConnectorID.find(i_idNodeConnector);
	if (itor != m_mapNodeConnectorID.end())
	{
		o_stkNodeConnectorP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_stkNodeConnectorP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取线集
 *	@param[in]	i_idLineSet				线集 ID
 *	@param[out]	o_stkLineSetP			线集对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetLineSetByID(
	STK_ID i_idLineSet,
	Stk_LineSet* &o_stkLineSetP
)
{
	Stk_LineSet_Map::iterator itor;

	itor = m_mapLineSetID.find(i_idLineSet);
	if (itor != m_mapLineSetID.end())
	{
		o_stkLineSetP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_stkLineSetP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取特征
 *	@param[in]	i_idFeat				特征 ID
 *	@param[out]	o_stkFeatP				特征对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetFeatByID(STK_ID i_idFeat, Stk_Feat* &o_stkFeatP)
{
	Stk_Feat_Map::iterator itor;
	itor = m_mapFeatID.find(i_idFeat);
	if (itor != m_mapFeatID.end())
	{
		o_stkFeatP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_stkFeatP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		通过ID获取Render信息
 *	@param[in]	i_idRender				Render ID
 *	@param[out]	o_stkRenderP			Render类对象指针
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP)
{
	Stk_Render_Map::iterator itor;
	itor = m_mapRenderID.find(i_idRender);
	if (itor != m_mapRenderID.end())
	{
		o_stkRenderP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_stkRenderP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		注册Body ID
 *	@param[in]	i_BodyP				Body类对象指针
 *	@param[out]	io_idBody			Body ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegBodyID(
	Stk_Body* i_BodyP,
	STK_ID& io_idBody
)
{
	Stk_Body_Map::iterator itor;
	pair<map<STK_ID, Stk_Body*>::iterator, bool> ret;

	/*
	 *  【CASE1】：发行新ID
	 */
	if(io_idBody==STK_NEW_ID || io_idBody == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapBodyID.size() == 0)
		{
			io_idBody = 1;
			m_mapBodyID.insert(pair<STK_ID, Stk_Body*>(io_idBody, i_BodyP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapBodyID.end();
			itor --;
			io_idBody = itor->first+1;
			m_mapBodyID.insert(pair<STK_ID, Stk_Body*>(io_idBody, i_BodyP));
			return STK_SUCCESS;
		}
	}
	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapBodyID.insert(pair<STK_ID, Stk_Body*>(io_idBody, i_BodyP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册PMI ID
 *	@param[in]	i_stkPMIP				PMI类对象指针
 *	@param[out]	io_idPMI				PMI ID
 *	@retval		STK_SUCCESS				注册成功
 *	@retval		STK_ERROR				注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::RegPMIID(
	Stk_PMI*	i_stkPMIP,
	STK_ID		&io_idPMI
)
{
	Stk_PMI_Map::iterator itor;
	pair<map<STK_ID, Stk_PMI*>::iterator, bool> ret;

	/*
	 *  【CASE1】：发行新ID
	 */
	if( io_idPMI == STK_NEW_ID || io_idPMI == STK_DMY_ID){
		// 判定出最大ID来发行
		if(m_mapPMIID.size() == 0){
			io_idPMI = 1;
			m_mapPMIID.insert(pair<STK_ID, Stk_PMI*>(io_idPMI,i_stkPMIP));
			return STK_SUCCESS;
		}
		else{
			itor = m_mapPMIID.end();
			itor --;
			io_idPMI = itor->first+1;
			m_mapPMIID.insert(pair<STK_ID, Stk_PMI*>(io_idPMI,i_stkPMIP));
			return STK_SUCCESS;
		}
	}
	/*
	 *  【CASE2】：使用指定ID
	 */
	else {
		ret = m_mapPMIID.insert(pair<STK_ID, Stk_PMI*>(io_idPMI,i_stkPMIP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册View ID
 *	@param[in]	i_stkViewP			视图类对象指针
 *	@param[out]	io_idView			视图 ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegViewID(Stk_View* i_stkViewP, STK_ID& io_idView)
{
	Stk_View_Map::iterator itor;
	pair<map<STK_ID, Stk_View*>::iterator, bool> ret;

	/*
	 *	【CASE1】：发行新ID
	 */
	if (io_idView == STK_NEW_ID || io_idView == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapViewID.size() == 0)
		{
			io_idView = 1;
			m_mapViewID.insert(pair<STK_ID, Stk_View*>(io_idView, i_stkViewP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapViewID.end();
			itor --;
			io_idView = itor->first+1;
			m_mapViewID.insert(pair<STK_ID, Stk_View*>(io_idView, i_stkViewP));
			return STK_SUCCESS;
		}
	}
	/*
	 *	【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapViewID.insert(pair<STK_ID, Stk_View*>(io_idView, i_stkViewP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册视图与PMI关联 ID
 *	@param[in]	i_stkNodeConnectorP			视图与PMI关联类对象指针
 *	@param[out]	o_idNodeConnector			视图与PMI关联 ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_ERROR					注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegNodeConnectorID(
	Stk_NodeConnector* i_stkNodeConnectorP,
	STK_ID& o_idNodeConnector
)
{
	Stk_NodeConnector_Map::iterator itor;
	pair<map<STK_ID, Stk_NodeConnector*>::iterator, bool> ret;

	/*
	 *	【CASE1】：发行新ID
	 */
	if (o_idNodeConnector == STK_NEW_ID || o_idNodeConnector == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapNodeConnectorID.size() == 0)
		{
			o_idNodeConnector = 1;
			m_mapNodeConnectorID.insert(pair<STK_ID, Stk_NodeConnector*>(o_idNodeConnector, i_stkNodeConnectorP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapNodeConnectorID.end();
			itor --;
			o_idNodeConnector = itor->first+1;
			m_mapNodeConnectorID.insert(pair<STK_ID, Stk_NodeConnector*>(o_idNodeConnector, i_stkNodeConnectorP));
			return STK_SUCCESS;
		}
	}
	/*
	 *	【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapNodeConnectorID.insert(pair<STK_ID, Stk_NodeConnector*>(o_idNodeConnector, i_stkNodeConnectorP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册线集 ID
 *	@param[in]	i_stkLineSetP		线集对象指针
 *	@param[out]	o_idLineSet			线集 ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegLineSetID(
	Stk_LineSet* i_stkLineSetP,
	STK_ID& o_idLineSet
)
{
	Stk_LineSet_Map::iterator itor;
	pair<map<STK_ID, Stk_LineSet*>::iterator, bool> ret;

	/*
	 *	【CASE1】：发行新ID
	 */
	if (o_idLineSet == STK_NEW_ID || o_idLineSet == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapLineSetID.size() == 0)
		{
			o_idLineSet = 1;
			m_mapLineSetID.insert(pair<STK_ID, Stk_LineSet*>(o_idLineSet, i_stkLineSetP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapLineSetID.end();
			itor --;
			o_idLineSet = itor->first+1;
			m_mapLineSetID.insert(pair<STK_ID, Stk_LineSet*>(o_idLineSet, i_stkLineSetP));
			return STK_SUCCESS;
		}
	}
	/*
	 *	【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapLineSetID.insert(pair<STK_ID, Stk_LineSet*>(o_idLineSet, i_stkLineSetP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Mesh ID
 *	@param[in]	i_MeshComP			Mesh类对象指针
 *	@param[out]	o_idMeshCom			Mesh ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegMeshComID(
	Stk_Mesh_Com* i_MeshComP,
	STK_ID &o_idMeshCom
)
{
	Stk_MeshCom_Map::iterator itor;
	pair<map<STK_ID, Stk_Mesh_Com*>::iterator, bool> ret;

	/*
	 *  【CASE1】：发行新ID
	 */
	if (o_idMeshCom == STK_NEW_ID || o_idMeshCom == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapMeshComID.size() == 0)
		{
			o_idMeshCom = 1;
			m_mapMeshComID.insert(pair<STK_ID, Stk_Mesh_Com*>(o_idMeshCom, i_MeshComP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapMeshComID.end();
			itor --;
			o_idMeshCom = itor->first+1;
			m_mapMeshComID.insert(pair<STK_ID, Stk_Mesh_Com*>(o_idMeshCom, i_MeshComP));
			return STK_SUCCESS;
		}
	}
	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapMeshComID.insert(pair<STK_ID, Stk_Mesh_Com*>(o_idMeshCom, i_MeshComP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Face ID
 *	@param[in]	i_MeshFaceP			Face类对象指针
 *	@param[out]	o_idMeshFace		Face ID
 *	@retval		STK_SUCCESS			注册成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegMeshFaceID(
	Stk_Mesh_Face* i_MeshFaceP,
	STK_ID &o_idMeshFace
)
{
	pair<map<STK_ID, Stk_Mesh_Face*>::iterator, bool> ret;
	Stk_MeshFace_Map::iterator itor;
	if (i_MeshFaceP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if (o_idMeshFace == STK_NEW_ID || o_idMeshFace == STK_DMY_ID)
	{

		// 判定出最大ID来发行
		if (m_mapMeshFaceID.size() == 0)
		{
			o_idMeshFace = 1;
			m_mapMeshFaceID.insert(pair<STK_ID, Stk_Mesh_Face*>(o_idMeshFace, i_MeshFaceP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapMeshFaceID.end();
			itor --;
			o_idMeshFace = itor->first+1;
			m_mapMeshFaceID.insert(pair<STK_ID, Stk_Mesh_Face*>(o_idMeshFace, i_MeshFaceP));
			return STK_SUCCESS;
		}
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapMeshFaceID.insert(pair<STK_ID, Stk_Mesh_Face*>(o_idMeshFace, i_MeshFaceP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Edge ID
 *	@param[in]	i_MeshEdgeP			Edge类对象指针
 *	@param[out]	o_idMeshEdge		Edge ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegMeshEdgeID(
	Stk_Mesh_Edge* i_MeshEdgeP,
	STK_ID &o_idMeshEdge
)
{
	Stk_MeshEdge_Map::iterator itor;
	pair<map<STK_ID, Stk_Mesh_Edge*>::iterator, bool> ret;
	if (i_MeshEdgeP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if (o_idMeshEdge == STK_NEW_ID ||o_idMeshEdge == STK_DMY_ID )
	{
		// 判定出最大ID来发行
		if (m_mapMeshEdgeID.size() == 0)
		{
			o_idMeshEdge = 1;
			m_mapMeshEdgeID.insert(pair<STK_ID, Stk_Mesh_Edge*>(o_idMeshEdge, i_MeshEdgeP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapMeshEdgeID.end();
			itor --;
			o_idMeshEdge = itor->first+1;
			m_mapMeshEdgeID.insert(pair<STK_ID, Stk_Mesh_Edge*>(o_idMeshEdge, i_MeshEdgeP));
			return STK_SUCCESS;
		}
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapMeshEdgeID.insert(pair<STK_ID, Stk_Mesh_Edge*>(o_idMeshEdge, i_MeshEdgeP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			//return STK_ERROR;
			
			// 由于杨洪鹏在转换器中已将边界线去重的逻辑给屏蔽掉。
			// 因此，为了对应，该处需要增加重复注册时的边界线ID
			return STK_SUCCESS_EXIST;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册特征ID
 *	@param[in]	i_stkFeatP			特征类对象指针
 *	@param[out]	o_idFeat			特征ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		其他				注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegFeatID(Stk_Feat* i_stkFeatP, STK_ID& o_idFeat)
{
	Stk_Feat_Map::iterator itor;
	pair<map<STK_ID, Stk_Feat*>::iterator, bool> ret;
	if (i_stkFeatP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if (o_idFeat == STK_NEW_ID || o_idFeat == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapFeatID.size() == 0)
		{
			o_idFeat = 1;
			m_mapFeatID.insert(pair<STK_ID, Stk_Feat*>(o_idFeat, i_stkFeatP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapFeatID.end();
			itor --;
			o_idFeat = itor->first+1;
			m_mapFeatID.insert(pair<STK_ID, Stk_Feat*>(o_idFeat, i_stkFeatP));
		}
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapFeatID.insert(pair<STK_ID, Stk_Feat*>(o_idFeat, i_stkFeatP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Render ID
 *	@param[in]	i_stkRenderP		Render类对象指针
 *	@param[out]	io_idRender			Render ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::RegRenderID(Stk_Render* i_stkRenderP, STK_ID &io_idRender)
{
	Stk_Render_Map::iterator itor;
	pair<map<STK_ID, Stk_Render*>::iterator, bool> ret;
	if (i_stkRenderP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if (io_idRender == STK_NEW_ID || io_idRender == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapRenderID.size() == 0)
		{
			io_idRender = 1;
			m_mapRenderID.insert(pair<STK_ID, Stk_Render*>(io_idRender, i_stkRenderP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapRenderID.end();
			itor --;
			io_idRender = itor->first+1;
			m_mapRenderID.insert(pair<STK_ID, Stk_Render*>(io_idRender, i_stkRenderP));
		}
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapRenderID.insert(pair<STK_ID, Stk_Render*>(io_idRender, i_stkRenderP));
		if (ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册Curve ID
 *	@param[in]	i_stkCurveP			Curve对象指针
 *	@param[out]	io_idCurve			Curve ID
 *	@retval		STK_SUCCESS			注册成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::RegCurveID(Stk_CurveEntity* i_stkCurveP, STK_ID& io_idCurve)
{
	Stk_Curve_Map::iterator itor;
	pair<map<STK_ID, Stk_CurveEntity*>::iterator, bool> ret;
	if (i_stkCurveP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  【CASE1】：发行新ID
	 */
	if (io_idCurve == STK_NEW_ID || io_idCurve == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapCurveID.size() == 0)
		{
			io_idCurve = 1;
			m_mapCurveID.insert(pair<STK_ID, Stk_CurveEntity*>(io_idCurve, i_stkCurveP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapCurveID.end();
			itor --;
			io_idCurve = itor->first+1;
			m_mapCurveID.insert(pair<STK_ID, Stk_CurveEntity*>(io_idCurve, i_stkCurveP));
		}
	}

	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapCurveID.insert(pair<STK_ID, Stk_CurveEntity*>(io_idCurve, i_stkCurveP));
		if (ret.second)
		{
			return STK_SUCCESS;
		}else
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Body与ID之间的映射
 *	@param[in]	i_idBody			Body ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelBodyByID(STK_ID i_idBody)
{
	m_mapBodyID.erase(i_idBody);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Mesh与ID之间的映射
 *	@param[in]	i_idMeshCom			Mesh ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelMeshComByID(STK_ID i_idMeshCom)
{
	m_mapMeshComID.erase(i_idMeshCom);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Face与ID之间的映射
 *	@param[in]	i_idMeshFace		Face ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelMeshFaceByID(STK_ID i_idMeshFace)
{
	m_mapMeshFaceID.erase(i_idMeshFace);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Edge与ID之间的映射
 *	@param[in]	i_idMeshEdge		Edge ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelMeshEdgeByID(STK_ID i_idMeshEdge)
{
	m_mapMeshEdgeID.erase(i_idMeshEdge);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除PMI与ID之间的映射
 *	@param[in]	i_idPMI				PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::DelPMIByID(STK_ID i_idPMI)
{
	m_mapPMIID.erase(i_idPMI);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除View与ID之间的映射
 *	@param[in]	i_idView			View ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelViewByID(STK_ID i_idView)
{
	m_mapViewID.erase(i_idView);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除视图和PMI之间的关联与ID之间的映射
 *	@param[in]	i_idNodeConnector	视图和PMI之间的关联 ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelNodeConnectorByID(STK_ID i_idNodeConnector)
{
	m_mapNodeConnectorID.erase(i_idNodeConnector);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除线集与ID之间的映射
 *	@param[in]	i_idLineSet			线集 ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelLineSetByID(STK_ID i_idLineSet)
{
	m_mapLineSetID.erase(i_idLineSet);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过制定ID删除特征
 *	@param[in]	i_idFeat					特征ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelFeatByID(STK_ID i_idFeat)
{
	m_mapFeatID.erase(i_idFeat);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Render与ID之间的映射
 *	@param[in]	i_idRender			Render ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::DelRenderByID(STK_ID i_idRender)
{
	m_mapRenderID.erase(i_idRender);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册组件特征 ID
 *	@param[in]	i_ComponentFeatP			组件特征指针
 *	@param[out]	io_idComponentFeat			组件特征 ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_ERROR					注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::RegComponentFeatID(Stk_ComponentFeature* i_ComponentFeatP, STK_ID& io_idComponentFeat)
{
	Stk_ComponentFeat_Map::iterator itor;
	pair<map<STK_ID, Stk_ComponentFeature*>::iterator, bool> ret;

	/*
	 *  【CASE1】：发行新ID
	 */
	if(io_idComponentFeat==STK_NEW_ID || io_idComponentFeat == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapComponentFeatID.size() == 0)
		{
			io_idComponentFeat = 1;
			m_mapComponentFeatID.insert(pair<STK_ID, Stk_ComponentFeature*>(io_idComponentFeat, i_ComponentFeatP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapComponentFeatID.end();
			itor --;
			io_idComponentFeat = itor->first+1;
			m_mapComponentFeatID.insert(pair<STK_ID, Stk_ComponentFeature*>(io_idComponentFeat, i_ComponentFeatP));
			return STK_SUCCESS;
		}
	}
	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapComponentFeatID.insert(pair<STK_ID, Stk_ComponentFeature*>(io_idComponentFeat, i_ComponentFeatP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}
		else
		{
			return STK_SUCCESS_EXIST; //指定ID已被注册，退出
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过ID获取组件特征
 *	@param[in]	i_idComponentFeat			组件特征 ID
 *	@param[out]	o_ComponentFeatP			组件特征类对象指针
 *	@retval		STK_SUCCESS					获取成功	
 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::GetComponentFeatByID(STK_ID i_idComponentFeat, Stk_ComponentFeature* &o_ComponentFeatP)
{
	Stk_ComponentFeat_Map::iterator itor;

	itor = m_mapComponentFeatID.find(i_idComponentFeat);

	if (itor != m_mapComponentFeatID.end())
	{
		o_ComponentFeatP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_ComponentFeatP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/************************************************************************//**
 *	@brief		删除组件特征与ID的映射
 *	@param[in]	i_idComponentFeat			组件特征 ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoIDManager::DelComponentFeatID(STK_ID i_idComponentFeat)
{
	Stk_ComponentFeat_Map::iterator itor = m_mapComponentFeatID.find(i_idComponentFeat);
	if (itor == m_mapComponentFeatID.end())
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	
	m_mapComponentFeatID.erase(i_idComponentFeat);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Body与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idBody				Body ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelBodyID(STK_ID i_idProtoType, STK_ID i_idBody)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_idProtoType <= 0 || i_idBody <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	eState = pProtoManager->DelBodyByID(i_idBody);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;

}

/************************************************************************//**
 *	@brief		删除Mesh与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idMeshCom				Mesh ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelMeshComID(STK_ID i_idProtoType, STK_ID i_idMeshCom)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_idProtoType <= 0 || i_idMeshCom <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	eState = pProtoManager->DelMeshComByID(i_idMeshCom);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Face与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idMeshFace			Face ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelMeshFaceID(STK_ID i_idProtoType, STK_ID i_idMeshFace)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_idProtoType <= 0 || i_idMeshFace <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return	STK_ERROR_ID_NOT_FOUND;
	}
	eState = pProtoManager->DelMeshFaceByID( i_idMeshFace);
	if (eState != STK_SUCCESS)
	{
		return	STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Edge与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idMeshEdge			Edge ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelMeshEdgeID(STK_ID i_idProtoType,STK_ID i_idMeshEdge)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS eState;

	if (i_idProtoType <= 0 || i_idMeshEdge <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return	STK_ERROR_ID_NOT_FOUND;
	}
	eState = pProtoManager->DelMeshEdgeByID( i_idMeshEdge);
	if (eState != STK_SUCCESS)
	{
		return	STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除PMI与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idPMI					PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::DelPMIID(STK_ID i_idProtoType,STK_ID i_idPMI)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if(i_idProtoType <= 0 || i_idPMI <= 0){
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if(itor != m_mapProtoTypeID.end()){
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else{
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->DelPMIByID( i_idPMI);
	if(state != STK_SUCCESS){
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除View与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idView				View ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelViewID(STK_ID i_idProtoType, STK_ID i_idView)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_idProtoType <= 0 || i_idView <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->DelViewByID(i_idView);
	if (state != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除视图和PMI的关联与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idNodeConnector		视图和PMI的关联 ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelNodeConnectorID(
	STK_ID i_idProtoType,
	STK_ID i_idNodeConnector
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_idProtoType <= 0 || i_idNodeConnector <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->DelNodeConnectorByID(i_idNodeConnector);
	if (state != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除线集与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idLineSet				线集 ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelLineSetID(
	STK_ID i_idProtoType,
	STK_ID i_idLineSet
)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_idProtoType <= 0 || i_idLineSet <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->DelLineSetByID(i_idLineSet);
	if (state != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		删除Render与ID之间的映射
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[in]	i_idRender				Render ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::DelRenderID(STK_ID i_idProtoType,STK_ID i_idRender)
{
	Stk_ProtoType_Map::iterator itor;
	Stk_ProtoIDManager *pProtoManager;
	STK_STATUS state;

	if (i_idProtoType <= 0 || i_idRender <= 0)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	itor = m_mapProtoTypeID.find(i_idProtoType);
	if (itor != m_mapProtoTypeID.end())
	{
		pProtoManager = itor->second->GetProtoIDManager();
	}
	else
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	state = pProtoManager->DelRenderByID(i_idRender);
	if (state != STK_SUCCESS)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置ProtoType的修改状态
 *	@param[in]	i_bModified				修改状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS				修改成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_GlobalIDManager::SetModifiedFlag(STK_BOOL i_bModified)
{
	Stk_ProtoType_Map::iterator itor = m_mapProtoTypeID.begin();

	while(itor != m_mapProtoTypeID.end())
	{
		itor->second->SetDataStatus(i_bModified);
		itor++;
	}
	return STK_SUCCESS;
}

#if 0
/************************************************************************//**
 *	@brief		注册Node LinkString与ID的映射表
 *	@param[in]	i_wstrLink					Node LinkString
 *	@param[out]	o_idNode					Node ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			Link已注册
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegNodeLinkString( 
	const wstring &i_wstrLink,
	STK_ID &o_idNode
)
{
	Stk_NodeLinkString_Map::iterator itor;

	/*
	 *  【CASE1】：如果Link已注册，则返回ID
	 */
	itor = m_mapNodeLinkString.find(i_wstrLink);
	if (itor != m_mapNodeLinkString.end())
	{
		o_idNode = itor->second;
		return STK_SUCCESS_EXIST;
	}

	/*
	 *  【CASE2】：如果Link未注册，则发行新ID
	 */
	if (m_mapNodeLinkString.size() == 0)
	{
		o_idNode = 1;
		m_mapNodeLinkString.insert(pair<wstring,STK_ID>(i_wstrLink, o_idNode) );
		m_nuMaxNodeLinkID ++;
		return STK_SUCCESS;
	}
	else
	{
		itor = m_mapNodeLinkString.end();
// 		itor --;
// 		outNodeID = itor->second+1;
		o_idNode = ++m_nuMaxNodeLinkID; //此处最大的LinkID应该是++m_nuMaxNodeLinkID，而不是itor->second+1;
		m_mapNodeLinkString.insert( pair<wstring, STK_ID>(i_wstrLink, o_idNode));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node LinkString与ID的映射表
 *	@param[in]	i_wstrLink					Node LinkString
 *	@param[out]	o_idNode					Node ID
 *	@retval		STK_SUCCESS					获取成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetNodeIDByLinkString( 
	const wstring &i_wstrLink, 
	STK_ID &o_idNode 
)
{
	Stk_NodeLinkString_Map::iterator itor;
	itor = m_mapNodeLinkString.find(i_wstrLink);

	if (itor != m_mapNodeLinkString.end())
	{
		o_idNode = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_idNode = -1;
		return STK_ERROR_ID_NOT_FOUND;
	}
}
#endif

/************************************************************************//**
 *	@brief		注册Node ID
 *	@param[in]	i_NodeP						Node指针
 *	@param[out]	io_idNode					Node ID
 *				== STK_NEW_ID				注册新ID <br>
 *				== 其它						注册指定ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			指定ID已被注册，退出
 *	@retval		STK_ERROR					注册失败
 *	@note		
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::RegNodeID(
	Stk_Node* i_NodeP,
	STK_ID &io_idNode
)
{
#if 0
	//Stk_NodeLinkString_Map::iterator itor;

	// 如果是NEWID，则从LinkString中获取ID号
	// 如果是指定ID，则从输入参数中获取ID号
	if(io_idNode == STK_NEW_ID || io_idNode == STK_DMY_ID)
	{
		if (i_NodeP == NULL && i_NodeP->GetLinkString().empty())
		{
			return STK_ERROR;
		}
		if (GetNodeIDByLinkString(i_NodeP->GetLinkString(), io_idNode) != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		// 第一步： 在“全局ID存储：NodeIDMap”中
		m_mapNodeID.insert(pair<STK_ID, Stk_Node*>(io_idNode, i_NodeP));
	}
	else
	{
		// 注意！从其它CAD直接转换来的Node，因为不调用RegNodeLink，因此这里要同步。
		// 例如：1.SView直接读取ProE文件（要与打开SVL文件区分对待）
		if (i_NodeP->GetLinkString().empty())
		{
			wstring wstrLink;
			WCHAR wchIDA[32];
			swprintf(wchIDA, 32, L"%d", io_idNode); //支持Andriod
			wstrLink = wchIDA;
			wstrLink += L"|NewNode";

			m_mapNodeLinkString.insert(pair<wstring, STK_ID>( wstrLink, io_idNode));
		}

		/*
		 *	为避免ID冲突，找寻最大ID
		 */
		if (m_nuMaxNodeLinkID ++ < io_idNode)
		{
			m_nuMaxNodeLinkID = io_idNode;
		}
		// 第一步： 在“全局ID存储：NodeIDMap”中
		m_mapNodeID.insert(pair<STK_ID, Stk_Node*>(io_idNode, i_NodeP));
	}
#else
	Stk_Node_Map::iterator itor;
	pair<map<STK_ID, Stk_Node*>::iterator, bool> ret;

	/*
	 *  【CASE1】：发行新ID
	 */
	if(io_idNode==STK_NEW_ID || io_idNode == STK_DMY_ID)
	{
		// 判定出最大ID来发行
		if (m_mapNodeID.size() == 0)
		{
			io_idNode = 1;
			m_mapNodeID.insert(pair<STK_ID, Stk_Node*>(io_idNode, i_NodeP));
			return STK_SUCCESS;
		}
		else
		{
			itor = m_mapNodeID.end();
			itor --;
			io_idNode = itor->first+1;
			m_mapNodeID.insert(pair<STK_ID, Stk_Node*>(io_idNode, i_NodeP));
			return STK_SUCCESS;
		}
	}
	/*
	 *  【CASE2】：使用指定ID
	 */
	else
	{
		ret = m_mapNodeID.insert(pair<STK_ID, Stk_Node*>(io_idNode, i_NodeP));
		if ( ret.second)
		{
			return STK_SUCCESS;
		}
		else
		{
			return STK_SUCCESS_EXIST; //指定ID已被注册，退出
		}
	}
#endif

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过ID获取Node
 *	@param[in]	i_idNode					Node ID
 *	@param[out]	o_NodeP						Node类对象指针
 *	@retval		STK_SUCCESS					获取成功	
 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::GetNodeByID(STK_ID i_idNode, Stk_Node* &o_NodeP)
{
	Stk_Node_Map::iterator itor;

	itor = m_mapNodeID.find(i_idNode);

	if (itor != m_mapNodeID.end())
	{
		o_NodeP = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_NodeP = NULL;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

#if 0
/************************************************************************//**
 *	@brief		删除Node LinkString与ID的映射
 *	@param[in]	i_wstrLink					Node LinkString
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelNodeLinkString(const wstring &i_wstrLink)
{
	m_mapNodeLinkString.erase(i_wstrLink);

	// NodeID 回收 ###
	if (m_mapNodeLinkString.size() == 0)
	{
		m_nuMaxNodeLinkID = 0;
	}

	return STK_SUCCESS;
}
#endif

/************************************************************************//**
 *	@brief		删除Node与ID的映射
 *	@param[in]	i_idNode				Node ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					删除成功
 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoIDManager::DelNodeByID(STK_ID i_idNode)
{

	// 第一步： 在“零件ID存储：NodeIDManagerMap”中
	Stk_Node_Map::iterator itor = m_mapNodeID.find(i_idNode);
	if (itor == m_mapNodeID.end())
	{
		return STK_ERROR_ID_NOT_FOUND;
	}
	//delete itor->second; //Node*的释放在File的析构时执行
	m_mapNodeID.erase(i_idNode);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所有的File
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapFileID			CAD物理文件与注册ID之间的映射
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//const Stk_File_Map& Stk_GlobalIDManager::GetAllFiles()
//{
//	// 注意！从IDManager中获取所有File是有风险的，因为DocManager下的File列与IDManager存在不一致的风险。
//	// 因为IDManager的本意只是管理ID，并不是管理DocManager下的所有File。
//	// 现在这样使用，务必保证Stk_File_Map中的File等价于DocManager正在使用File。
//	// 例如，从DocManager中删除File对象时，一定要同时删除m_mapFileID、m_mapCADFilePath中的File单元。
//
//	return m_mapFileID;
//}

/*************************************************************************//**
 *	@brief		注册CAD物理文件与路径之间的映射
 *	@param[in]	i_wstrCADFilePath		CAD物理文件路径
 *	@param[in]	o_ID					CAD File ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				注册成功
 *	@retval		其他					注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::RegCADFilePath(wstring i_wstrCADFilePath, STK_ID &o_ID)
{
	Stk_CADFilePath_Map::iterator itor;

	// 为空，文件还没有注册
	if(m_mapCADFilePath.size() == 0)
	{
		o_ID = 1;
		m_mapCADFilePath.insert(pair<wstring, STK_ID>(i_wstrCADFilePath, o_ID));
		m_nuMaxCADFileID ++;
		//m_mapFileID.insert(pair<STK_ID, Stk_File*>(o_ID, i_stkFileP));
		return STK_SUCCESS;
	}
	// 非空，分两种情况，已注册和未注册
	else
	{
		itor = m_mapCADFilePath.find(i_wstrCADFilePath);
		if(itor != m_mapCADFilePath.end())
		{
			o_ID = itor->second;
			return STK_SUCCESS_EXIST;			// 文件已注册，返回
		}
		else
		{
			o_ID = ++ m_nuMaxCADFileID;
			m_mapCADFilePath.insert(pair<wstring, STK_ID>(i_wstrCADFilePath, o_ID));
			//m_mapFileID.insert(pair<STK_ID, Stk_File*>(o_ID, i_stkFileP));
			return STK_SUCCESS;
		}
	}
	
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		通过CAD物理文件路径查找指定的File的ID
 *	@param[in]	i_wstrCADPath			CAD物理文件路径
 *	@param[out]	o_ID					CAD File ID
 *	@retval		STK_SUCCESS				查找成功
 *	@retval		其他					查找失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::GetCADFileIDByPath(wstring i_wstrCADPath, STK_ID &o_ID)
{
	map<wstring, STK_ID>::iterator itor = m_mapCADFilePath.find(i_wstrCADPath);
	if (itor != m_mapCADFilePath.end())
	{
		o_ID = itor->second;
		return STK_SUCCESS;
	}
	else
	{
		o_ID = 0;
		return STK_ERROR_ID_NOT_FOUND;
	}
}

/*************************************************************************//**
 *	@brief		删除指定CAD物理文件路径的File
 *	@param[in]	i_wstrCADPath			CAD物理文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		其他					删除失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_GlobalIDManager::DelCADFileIDByPath(wstring i_wstrCADPath)
{
	m_mapCADFilePath.erase(i_wstrCADPath);
	if(m_mapCADFilePath.size() == 0)
	{
		m_nuMaxCADFileID = 0;
	}

	return STK_SUCCESS;
}
