// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_PMIEntity.cpp
 *
 *	@brief	Node节点类
 *
 *	@par	历史:
 *		2014/04/16	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Manager.h" 
#include "Stk_Node.h"
#include "Stk_Platform.h"

/************************************************************************//**
 *	@brief		Stk_Node构造函数
  *	@param[in]	i_DocManagerP		DocumentManager
 *	@param[in]	i_stkProtoP			ProtoType
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Node::Stk_Node( Stk_DocumentManager *i_DocManagerP, Stk_ProtoType* i_stkProtoP)
{
	m_DocManager = i_DocManagerP;
	m_stkProtoP = i_stkProtoP;
	m_nuID = 0;
	m_bDispStat = true;
	m_idRefCount = 0;
	//m_wstrLink = L"";
	m_dataP = NULL;
	m_idMaxPlc = 0;
	m_stkFeatP = NULL;
	m_eType = NODE_TYPE_UNKNOWN;
	m_wstrName = L"";
}

/************************************************************************//**
 *	@brief		Stk_Node析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		1. 会删除Node所包含的数据	<br>
				2. 会移除与父Node的关联		<br>
				3. 会删除子Node
 *	@attention	无
 ****************************************************************************/

Stk_Node::~Stk_Node()
{
	// 第一步，删除Node包含的数据
	if (m_dataP != NULL)
	{
		switch(m_eType) //防止析构m_dataP时再次析构Node造成死循环
		{
		case NODE_TYPE_BODY:
			((Stk_Body*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_MESH:
			((Stk_Mesh_Com*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_PMI:
			((Stk_PMI*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_VIEW:
			((Stk_View*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_META:
			((Stk_MetaData*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_CAMERA:
			((Stk_Camera*)m_dataP)->SetNode(NULL);
			break;
		case NODE_TYPE_LAYERINFOSSET:
			((Stk_LayerInfosSet*)m_dataP)->SetNode(NULL);
			break;
		}
		delete m_dataP;
		m_dataP = NULL;
	}
	for (int ii = 0; ii < m_vcInfos.size(); ii++)
	{
		if (m_vcInfos[ii] != NULL)
		{
			delete m_vcInfos[ii];
			m_vcInfos[ii] = NULL;
		}
	}
	if (m_stkFeatP != NULL)
	{
		delete m_stkFeatP;
		m_stkFeatP = NULL;
	}

	for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
	{
		if (m_vcNodeConnectors[ii] != NULL)
		{
			delete m_vcNodeConnectors[ii];
			m_vcNodeConnectors[ii] = NULL;
		}
	}

#if 0 //提高性能
	// 第二步，解除与父Node的关系
	RemoveFromAllParents();

	// 第三步，删除所有的下级Node
	STK_UINT32 i;
	STK_SIZE_T szChild = m_vcChildNodeIDs.size();
	for (i=0; i<szChild; i++)
	{
		DelChildNodeByPlc(m_vcChildNodeIDs[i].plcid);
	}
#else
	// 第二步，断开父Node的子关联；清空该Node的m_vcPaNodes
	if (m_stkProtoP && m_stkProtoP->IsDeleting()==false) //优化性能
	{
		RemoveFromAllParents();
	}
	
	// 第三步，断开该Node的子关联
	DelAllChildNodes();
#endif

	m_stkProtoP->GetProtoIDManager()->DelNodeByID(m_nuID);
}

/************************************************************************//**
 *	@brief		引用计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

void Stk_Node::AddRef()
{
	m_idRefCount++;
}

/************************************************************************//**
 *	@brief		获取引用计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		引用计数器
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_INT32 Stk_Node::GetRef()
{
	return m_idRefCount;
}

/************************************************************************//**
 *	@brief		Node内存释放
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			释放
 *				STK_SUCCESS_EXIST	跳过
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Node::Release()
{
	m_idRefCount--;
	if (m_idRefCount <= 0)
	{
		delete this;
		return STK_SUCCESS;
	}
	else
	{
		return STK_SUCCESS_EXIST;
	}
}

#if 0
/************************************************************************//**
 *	@brief		获取Node Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrLink	Node Link
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

wstring Stk_Node::GetLinkString()
{
	return m_wstrLink;
}

/************************************************************************//**
 *	@brief		设置Node Link
 *	@param[in]	i_wstrLink		Node Link
 *	@param[out]	无
 *	@retval		STK_SUCCESS		成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Node::SetLinkString(wstring i_wstrLink)
{
	m_wstrLink = i_wstrLink;

	return STK_SUCCESS;
}


/************************************************************************//**
 *	@brief		注册Node路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_SUCCESS_EXIST	已注册
 *	@retval		其他				注册失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Node::RegNodeLink()
{
	STK_STATUS	eState;
	STK_ID		idNode;

	if(m_wstrLink == L"")
	{
		return STK_ERROR;
	}

	eState = m_stkProtoP->GetProtoIDManager()->RegNodeLinkString(m_wstrLink, idNode);

	return eState;
}
#endif

/****************************************************************************
 *	@brief		注册Node ID
 *	@param[in]	i_idNode		Node ID <br>
 *				== STK_NEW_ID				注册新ID <br>
 *				== 其它						注册指定ID
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			指定ID已被注册，退出
 *	@retval		STK_ERROR					注册失败
 *	@note		指定ID需大于等于1的整数
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::RegisterID(STK_ID i_idNode)
{
	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegNodeID(this, i_idNode );
	if (ret==STK_SUCCESS)
	{
		m_nuID = i_idNode;
		return STK_SUCCESS;
	}
	else
	{
		return ret;
	}
}

/************************************************************************//**
 *	@brief		移除子Node，不释放内存
 *	@param[in]	i_idPlc			Node配置ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		移除成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Node::RemoveChild(STK_ID i_idPlc)
{
	Stk_Node* stkNodeP = NULL;
	for (vector<PlcNode>::iterator itor = m_vcChildNodeIDs.begin(); itor != m_vcChildNodeIDs.end(); itor ++)
	{
		if ((*itor).plcid == i_idPlc)
		{
			m_vcChildNodeIDs.erase(itor);
// 			m_stkProtoP->GetProtoIDManager()->GetNodeByID(i_idPlc, stkNodeP);
// 			if( stkNodeP)
// 			{
// 				stkNodeP->Release();
// 				stkNodeP = NULL;
// 			}
			return STK_SUCCESS;
		}
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		删除子Node并释放内存
 *	@param[in]	i_idNode		Node ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@note		无
 *	@attention	对外接口
 ****************************************************************************/
STK_STATUS Stk_Node::DeleteChildNodeByID(STK_ID i_idNode)
{
	STK_INT32	nRefCount = 0;
	Stk_Node*	ChildNodeP = NULL;
	vector<PlcNode>::iterator itor = m_vcChildNodeIDs.begin();
	while (itor != m_vcChildNodeIDs.end())
	{
		if (itor->nodeid == i_idNode)
		{
			m_stkProtoP->GetProtoIDManager()->GetNodeByID(i_idNode, ChildNodeP);
			if (ChildNodeP)
			{
				//delete ChildNodeP;
				ChildNodeP->Release();
				nRefCount = ChildNodeP->GetRef();
				if (nRefCount > 0)
				{
					m_vcChildNodeIDs.erase(itor);
				}
			}
			return STK_SUCCESS;
		}
		itor++;
	}

	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		设置显隐状态
 *	@param[in]	i_bDispStat		显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetDispStat(STK_BOOL i_bDispStat)
{
	m_bDispStat = i_bDispStat;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bDispStat		显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Node::GetDispStat()
{
	return m_bDispStat;
}

/************************************************************************//**
 *	@brief		设置Node名称
 *	@param[in]	i_wstrNodeName	Node名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		最大63字符
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetNodeName(wstring i_wstrNodeName)
{
	m_wstrName = i_wstrNodeName;
	switch(m_eType)
	{
	case NODE_TYPE_VIRTUAL:
		break;
	case NODE_TYPE_BODY:
		if (m_dataP != NULL)
		{
			((Stk_Body*)m_dataP)->ChangeName(i_wstrNodeName);
		}
		break;
	case NODE_TYPE_MESH:
		break;
	case NODE_TYPE_PMI:
		if (m_dataP != NULL)
		{
			((Stk_PMI*)m_dataP)->ChangeName(i_wstrNodeName);
		}
		break;
	case NODE_TYPE_VIEW:
		if (m_dataP != NULL)
		{
			((Stk_View*)m_dataP)->ChangeName(i_wstrNodeName);
		}
		break;
	case NODE_TYPE_CAMERA:
		break;
	case NODE_TYPE_META:
		break;
	case NODE_TYPE_LAYERINFOSSET:
		break;
	default:
		break;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName		Node名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Node::GetNodeName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置Node类型
 *	@param[in]	i_eType			Node类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetNodeType(StkNodeTypeEnum i_eType)
{
	m_eType = i_eType;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eType			Node类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkNodeTypeEnum Stk_Node::GetNodeType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		获取当前Node节点的子节点的数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		子节点的数目
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_Node::GetChildNodeNum()
{
	return m_vcChildNodeIDs.size();
}

/************************************************************************//**
 *	@brief		获取当前Node节点的指定索引的子节点
 *	@param[in]	i_nuKeyPose		指定索引
 *	@param[out]	o_nuPlcID		制定Node的配置ID
 *	@retval		stkNodeP		指定Node的指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Node* Stk_Node::GetChildNode(STK_UINT32 i_nuKeyPose, STK_ID &o_nuPlcID)
{
	Stk_Node* stkNodeP = NULL;
	if (i_nuKeyPose>=m_vcChildNodeIDs.size())
	{
		return NULL;
	}

	PlcNode vcPlcNode = m_vcChildNodeIDs[i_nuKeyPose];
	o_nuPlcID = vcPlcNode.plcid; 

	m_stkProtoP->GetProtoIDManager()->GetNodeByID(vcPlcNode.nodeid, stkNodeP);
	return stkNodeP;
}

/************************************************************************//**
 *	@brief		向当前Node节点中添加子节点
 *	@param[in]	i_idNode		子节点ID
 *	@param[out]	o_idPlc			配置ID
 *	@retval		STK_SUCCESS		添加成功
 *	@note		子Node会自动AddRef
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::AddChildNode(STK_ID i_idNode, STK_ID& o_idPlc)
{
	PlcNode plcInfo;
	m_idMaxPlc++;
	o_idPlc = m_idMaxPlc;
	plcInfo.plcid = m_idMaxPlc;
	plcInfo.nodeid = i_idNode;
	m_vcChildNodeIDs.push_back(plcInfo);

	// 建立子Node与父Node的关系
	Stk_Node* stkCldNodeP = NULL;
	m_stkProtoP->GetProtoIDManager()->GetNodeByID(i_idNode, stkCldNodeP);
	if (stkCldNodeP)
	{
		stkCldNodeP->AddParentNode(this);
		stkCldNodeP->AddRef();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/****************************************************************************
 *	@brief		向当前Node节点中添加父节点
 *	@param[in]	i_stkNodeP		父Node指针
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::AddParentNode(Stk_Node* i_stkNodeP)
{
	m_vcPaNodes.push_back(i_stkNodeP);

	return STK_SUCCESS;
}


/****************************************************************************
 *	@brief		获取当前Node节点的父节点的数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		父节点的数目
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_SIZE_T Stk_Node::GetParentNodeNum()
{
	return m_vcPaNodes.size();
}

/****************************************************************************
 *	@brief		获取当前Node节点的指定索引的父节点
 *	@param[in]	i_nuKeyPose		指定索引
 *	@param[out]	无
 *	@retval		stkNodeP		指定的Node节点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Node* Stk_Node::GetParentNodeByIndex(STK_UINT32 i_nuIndex)
{
	if ( i_nuIndex >= m_vcPaNodes.size())
	{
		return NULL;
	}
	else
	{
		return m_vcPaNodes.at(i_nuIndex);
	}
}

/************************************************************************//**
 *	@brief		通过配置ID删除子Node，释放子Node内存
 *	@param[in]	i_idPlc			配置ID
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		STK_ERROR		删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::DelChildNodeByPlc(STK_ID i_idPlc)
{
	STK_INT32	nRefCount = 0;
	Stk_Node*	ChildNodeP = NULL;
	vector<PlcNode>::iterator itor = m_vcChildNodeIDs.begin();
	while (itor != m_vcChildNodeIDs.end())
	{
		if (itor->plcid == i_idPlc)
		{
			m_stkProtoP->GetProtoIDManager()->GetNodeByID(itor->nodeid, ChildNodeP);
			if (ChildNodeP)
			{
				//delete ChildNodeP;
				ChildNodeP->Release();
				nRefCount = ChildNodeP->GetRef();
				if (nRefCount > 0)
				{
					m_vcChildNodeIDs.erase(itor);
				}
			}
			return STK_SUCCESS;
		}
		itor++;
	}

	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		删除所有子Node，释放子Node内存
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		STK_ERROR		删除失败
 *	@note		delete node时会自动调用DelAllChildNodes，请避免重复调用
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::DelAllChildNodes()
{
	Stk_Node* ChildNodeP = NULL;
	while(m_vcChildNodeIDs.size()>0){
		m_stkProtoP->GetProtoIDManager()->GetNodeByID( (*(m_vcChildNodeIDs.rbegin())).nodeid, ChildNodeP);
		if (ChildNodeP)
		{
			m_vcChildNodeIDs.pop_back();  //先移除m_vcChildNodeIDs，再Release性能更高
			ChildNodeP->Release();
		}
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		断开与所有父Node的关联
 *	@param[in]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@retval		STK_ERROR		删除失败
 *	@note		仅从所有的父Node中移除关联，不释放Node的内存
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::RemoveFromAllParents()
{
	STK_UINT32 i,j;
	STK_SIZE_T szPa = m_vcPaNodes.size();//找到所有的父Node
	STK_ID plcid = 0;
	for (i=0; i<szPa; i++)
	{
		Stk_Node* stkPaNodeP = m_vcPaNodes[i];
		STK_SIZE_T szCld = stkPaNodeP->GetChildNodeNum();
		for (j=0; j<szCld; j++)
		{
			if (stkPaNodeP->GetChildNode(j,plcid) == this) //从父Node解除当前Node
			{
				stkPaNodeP->RemoveChild(plcid);
			}
		}
	}
	m_vcPaNodes.clear();

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取配置ID
 *	@param[in]	无
 *	@retval		配置ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_ID Stk_Node::GetPlcID()
//{
//	return m_idMaxPlc;
//}

/************************************************************************//**
 *	@brief		向当前Node节点中添加基本信息
 *	@param[in]	stkInfoP		添加基本信息
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::AddInfo(Stk_Information* stkInfoP)
{
	if (stkInfoP != NULL)
	{
		m_vcInfos.push_back(stkInfoP);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取基本信息大小
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		基本信息大小
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_Node::GetInfoNum()
{
	return m_vcInfos.size();
}

/************************************************************************//**
 *	@brief		获取指定索引的基本信息
 *	@param[in]	i_nuIndex			指定的索引
 *	@param[out]	无
 *	@retval		Stk_Information		基本信息
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Information* Stk_Node::GetInfoByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcInfos.at(i_nuIndex);
}

/************************************************************************//**
 *	@brief		设置Node ID
 *	@param[in]	i_Id			Node ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetID(STK_UINT32 i_Id)
{
	m_nuID = i_Id;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuID			Node ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Node::GetID()
{
	return m_nuID;
}

/************************************************************************//**
 *	@brief		获取Body指针
 *	@param[in]	i_stkBodyP		Body对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetBody(Stk_Body* i_stkBodyP)
{
	if (m_eType == NODE_TYPE_BODY)
	{
		m_dataP = i_stkBodyP;
		if (i_stkBodyP!=NULL)
		{
			i_stkBodyP->SetNode(this);
			m_wstrName = i_stkBodyP->GetName();
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取Body指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			Body对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Body* Stk_Node::GetBody()
{
	if (m_eType == NODE_TYPE_BODY)
	{
		return (Stk_Body*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		获取Camera指针
 *	@param[in]	i_stkCameraP		Camera对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetCamera(Stk_Camera* i_stkCameraP)
{
	if (m_eType == NODE_TYPE_CAMERA)
	{
		m_dataP = i_stkCameraP;
		if (i_stkCameraP!=NULL)
		{
			i_stkCameraP->SetNode(this);
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取Camera指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			Camera对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Camera* Stk_Node::GetCamera()
{
	if (m_eType == NODE_TYPE_CAMERA)
	{
		return (Stk_Camera*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置Mesh指针
 *	@param[in]	i_stkMeshP		Mesh指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetMesh(Stk_Mesh_Com* i_stkMeshP)
{
	if (m_eType == NODE_TYPE_MESH)
	{
		m_dataP = i_stkMeshP;
		if (i_stkMeshP!=NULL)
		{
			i_stkMeshP->SetNode(this);
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取Mesh指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			Mesh对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Mesh_Com* Stk_Node::GetMesh()
{
	if (m_eType == NODE_TYPE_MESH)
	{
		return (Stk_Mesh_Com*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		获取PMI指针
 *	@param[in]	i_stkPMIP		PMI对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetPMI(Stk_PMI* i_stkPMIP)
{
	if (m_eType == NODE_TYPE_PMI)
	{
		m_dataP = (Stk_PMI*)i_stkPMIP;
		if (i_stkPMIP!=NULL)
		{
			i_stkPMIP->SetNode(this);
			m_wstrName = i_stkPMIP->GetName();
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取PMI指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			PMI对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_PMI* Stk_Node::GetPMI()
{
	if (m_eType == NODE_TYPE_PMI)
	{
		return (Stk_PMI*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置View指针
 *	@param[in]	i_stkViewP		视图对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetView(Stk_View* i_stkViewP)
{
	if (m_eType == NODE_TYPE_VIEW)
	{
		m_dataP = (Stk_View*)i_stkViewP;
		if (i_stkViewP!=NULL)
		{
			i_stkViewP->SetNode(this);
			m_wstrName = i_stkViewP->GetName();
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取View指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			PMI对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_View* Stk_Node::GetView()
{
	if (m_eType == NODE_TYPE_VIEW)
	{
		return (Stk_View*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置MetaData指针
 *	@param[in]	i_stkMetaDataP	自定义对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetMetaData(Stk_MetaData* i_stkMetaDataP)
{
	if (m_eType == NODE_TYPE_META)
	{
		m_dataP = (Stk_MetaData*)i_stkMetaDataP;
		if (i_stkMetaDataP!=NULL)
		{
			i_stkMetaDataP->SetNode(this);
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取自定义对象指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			自定义属性对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_MetaData* Stk_Node::GetMetaData()
{
	if (m_eType == NODE_TYPE_META)
	{
		return (Stk_MetaData*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置图层信息集
 *	@param[in]	i_LayerInfosSet	图层信息集
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Node::SetLayerInfosSet(Stk_LayerInfosSet* i_LayerInfosSet)
{
	if (m_eType == NODE_TYPE_LAYERINFOSSET)
	{
		m_dataP = (Stk_LayerInfosSet*)i_LayerInfosSet;
		if (i_LayerInfosSet!=NULL)
		{
			i_LayerInfosSet->SetNode(this);
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取图层信息集
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dataP			图层信息集对象指针
 *	@retval		其它			获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LayerInfosSet* Stk_Node::GetLayerInfosSet()
{
	if (m_eType == NODE_TYPE_LAYERINFOSSET)
	{
		return (Stk_LayerInfosSet*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		读取Node文件
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_NodeHeadSP		Node表头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_SUCCESS_EXIST	已读取跳出
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Node::ReadFile(Stk_File* i_stkFileP, StkSegmentHead* i_NodeHeadSP)
{
	/*
	 *	变量定义
	 */
	STK_UINT32			ii, jj;
	StkControl**		recordPP = NULL;
	Stk_Node*			stkNodeP = NULL;
	StkNodeRec*			stkNodeRecP = NULL;
	StkNodeRec2*		stkNodeRec2P = NULL;
	StkNodeRec3*		stkNodeRec3P = NULL;
	StkNodeRec4*		stkNodeRec4P = NULL;
	StkNodeRec5*		stkNodeRec5P = NULL;
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	STK_STATUS			eState;
	Stk_Body			*StkBodyP = NULL; 
	Stk_Mesh_Com		*StkMeshComP = NULL;
	Stk_PMI				*StkPMIP = NULL;
	Stk_View			*StkViewP = NULL;
	Stk_MetaData*		stkMetaDataP = NULL;
	Stk_LayerInfosSet*	stkLayerInfosSetP = NULL;
	Stk_Information*	stkInfoP = NULL;
	Stk_Feat*			stkFeatP = NULL;
	STK_UINT32			nuElemCount = 0;
	STK_ID				idChildNode = 0;
	Stk_Node*			CurNodeP = NULL;
	StkSegmentHead		head ={0};
	PlcNode				plcnode = {0};
	Stk_NodeConnector*	stkNodeConnectorP = NULL;

	/*
	 *	读取Node段的数据
	 */
	recordPP = (StkControl **)calloc(i_NodeHeadSP->recnum, sizeof(StkControl*));
	if (i_NodeHeadSP->recnum != 0 && recordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_NodeHeadSP, (StkControl **)recordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取Node段的各记录
	 */
	for (ii = 0; ii < i_NodeHeadSP->recnum; ii++)
	{
		if (recordPP[ii]->type == REC_TYPE_Node)
		{
			if (recordPP[ii]->version == 1)
			{
				stkNodeRecP = (StkNodeRec *)recordPP[ii];
				m_eType = (StkNodeTypeEnum)stkNodeRecP->type;

				if (!((stkNodeRecP->IsItem) & STK_Node_IS_DISPLAY))
				{
					SetDispStat(false);
				}
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkNodeRecP->name);
#else
				m_wstrName = stkNodeRecP->name;
#endif
				nuElemCount = stkNodeRecP->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					plcnode.plcid = stkNodeRecP->childdat[jj].PlcId;
					plcnode.nodeid = stkNodeRecP->childdat[jj].NodeId;
					idChildNode = plcnode.nodeid;

#if 0				//注意！ Node不需要通过LinkString的方式发行ID
					// 创建Node
					wstrCurLink.erase();
					swprintf(wchIDA, 32, L"%d", plcnode.nodeid); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					swprintf(wchIDA, 32, L"%d", m_stkProtoP->GetID()); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					wstrCurLink += i_stkFileP->GetSVLPath();//### 多Document时需要改
					
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->SetLinkString(wstrCurLink);

					// 注意！由于可能存在重复引用的情况，这里直接new Stk_Node就不可取了
					// 正确的处理方案是参照Instance与ProtoType的关联方式，使用id关联
					eState = CurNodeP->RegNodeLink();
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					CurNodeP->RegisterID(idChildNode);
					m_vcChildNodeIDs.push_back(plcnode);
#else
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->RegisterID(idChildNode);
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					m_vcChildNodeIDs.push_back(plcnode); //建立子关联
					CurNodeP->AddParentNode(this); // 建立父关联
#endif
				}
			}
			else if (recordPP[ii]->version == 2)
			{
				stkNodeRec2P = (StkNodeRec2 *)recordPP[ii];
				m_eType = (StkNodeTypeEnum)stkNodeRec2P->type;

				if (!((stkNodeRec2P->IsItem) & STK_Node_IS_DISPLAY))
				{
					SetDispStat(false);
				}
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkNodeRec2P->name);
#else
				m_wstrName = stkNodeRec2P->name;
#endif
				nuElemCount = stkNodeRec2P->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					plcnode.plcid = stkNodeRec2P->childdat[jj].PlcId;
					plcnode.nodeid = stkNodeRec2P->childdat[jj].NodeId;
					idChildNode = plcnode.nodeid;

#if 0				//注意！ Node不需要通过LinkString的方式发行ID
					// 创建Node
					wstrCurLink.erase();
					swprintf(wchIDA, 32, L"%d", plcnode.nodeid); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					swprintf(wchIDA, 32, L"%d", m_stkProtoP->GetID()); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					wstrCurLink += i_stkFileP->GetSVLPath();//### 多Document时需要改

					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->SetLinkString(wstrCurLink);

					// 注意！由于可能存在重复引用的情况，这里直接new Stk_Node就不可取了
					// 正确的处理方案是参照Instance与ProtoType的关联方式，使用id关联
					eState = CurNodeP->RegNodeLink();
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					CurNodeP->RegisterID(idChildNode);
					m_vcChildNodeIDs.push_back(plcnode);
#else
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->RegisterID(idChildNode);
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					m_vcChildNodeIDs.push_back(plcnode); //建立子关联
					CurNodeP->AddParentNode(this); // 建立父关联
#endif
				}
			}
			else if (recordPP[ii]->version == 3)
			{
				stkNodeRec3P = (StkNodeRec3 *)recordPP[ii];
				m_eType = (StkNodeTypeEnum)stkNodeRec3P->type;

				if (!((stkNodeRec3P->IsItem) & STK_Node_IS_DISPLAY))
				{
					SetDispStat(false);
				}
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkNodeRec3P->name);
#else
				m_wstrName = stkNodeRec3P->name;
#endif
				nuElemCount = stkNodeRec3P->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					plcnode.plcid = stkNodeRec3P->childdat[jj].PlcId;
					plcnode.nodeid = stkNodeRec3P->childdat[jj].NodeId;
					idChildNode = plcnode.nodeid;

#if 0				//注意！ Node不需要通过LinkString的方式发行ID
					// 创建Node
					wstrCurLink.erase();
					swprintf(wchIDA, 32, L"%d", plcnode.nodeid); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					swprintf(wchIDA, 32, L"%d", m_stkProtoP->GetID()); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					wstrCurLink += i_stkFileP->GetSVLPath();//### 多Document时需要改

					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->SetLinkString(wstrCurLink);

					// 注意！由于可能存在重复引用的情况，这里直接new Stk_Node就不可取了
					// 正确的处理方案是参照Instance与ProtoType的关联方式，使用id关联
					eState = CurNodeP->RegNodeLink();
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					CurNodeP->RegisterID(idChildNode);
					m_vcChildNodeIDs.push_back(plcnode);
#else
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->RegisterID(idChildNode);
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					m_vcChildNodeIDs.push_back(plcnode); //建立子关联
					CurNodeP->AddParentNode(this); // 建立父关联
#endif
				}
			}
			else if (recordPP[ii]->version == 4)
			{
				stkNodeRec4P = (StkNodeRec4 *)recordPP[ii];
				m_eType = (StkNodeTypeEnum)stkNodeRec4P->type;

				if (!((stkNodeRec4P->IsItem) & STK_Node_IS_DISPLAY))
				{
					SetDispStat(false);
				}
				nuElemCount = stkNodeRec4P->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					plcnode.plcid = stkNodeRec4P->childdat[jj].PlcId;
					plcnode.nodeid = stkNodeRec4P->childdat[jj].NodeId;
					idChildNode = plcnode.nodeid;

#if 0				//注意！ Node不需要通过LinkString的方式发行ID
					// 创建Node
					wstrCurLink.erase();
					swprintf(wchIDA, 32, L"%d", plcnode.nodeid); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					swprintf(wchIDA, 32, L"%d", m_stkProtoP->GetID()); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					wstrCurLink += i_stkFileP->GetSVLPath();//### 多Document时需要改

					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->SetLinkString(wstrCurLink);

					// 注意！由于可能存在重复引用的情况，这里直接new Stk_Node就不可取了
					// 正确的处理方案是参照Instance与ProtoType的关联方式，使用id关联
					eState = CurNodeP->RegNodeLink();
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					CurNodeP->RegisterID(idChildNode);
					m_vcChildNodeIDs.push_back(plcnode);
#else
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->RegisterID(idChildNode);
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					m_vcChildNodeIDs.push_back(plcnode); //建立子关联
					CurNodeP->AddParentNode(this); // 建立父关联
#endif
				}
			}
			else if (recordPP[ii]->version == 5)
			{
				stkNodeRec5P = (StkNodeRec5 *)recordPP[ii];
				m_eType = (StkNodeTypeEnum)stkNodeRec5P->type;
				m_eSubType = (StkNodeSubTypeEnum)stkNodeRec5P->subType;

				if (!((stkNodeRec5P->IsItem) & STK_Node_IS_DISPLAY))
				{
					SetDispStat(false);
				}
				nuElemCount = stkNodeRec5P->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					plcnode.plcid = stkNodeRec5P->childdat[jj].PlcId;
					plcnode.nodeid = stkNodeRec5P->childdat[jj].NodeId;
					idChildNode = plcnode.nodeid;

#if 0				//注意！ Node不需要通过LinkString的方式发行ID
					// 创建Node
					wstrCurLink.erase();
					swprintf(wchIDA, 32, L"%d", plcnode.nodeid); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					swprintf(wchIDA, 32, L"%d", m_stkProtoP->GetID()); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					wstrCurLink += i_stkFileP->GetSVLPath();//### 多Document时需要改

					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->SetLinkString(wstrCurLink);

					// 注意！由于可能存在重复引用的情况，这里直接new Stk_Node就不可取了
					// 正确的处理方案是参照Instance与ProtoType的关联方式，使用id关联
					eState = CurNodeP->RegNodeLink();
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					CurNodeP->RegisterID(idChildNode);
					m_vcChildNodeIDs.push_back(plcnode);
#else
					CurNodeP = new Stk_Node(m_DocManager, m_stkProtoP);
					CurNodeP->RegisterID(idChildNode);
					if (eState == STK_SUCCESS_EXIST || eState == STK_ERROR)
					{
						if (CurNodeP != NULL)
						{
							delete CurNodeP;
							CurNodeP = NULL;
						}
						continue;
					}
					m_vcChildNodeIDs.push_back(plcnode); //建立子关联
					CurNodeP->AddParentNode(this); // 建立父关联
#endif
				}
			}
		}
		else if (recordPP[ii]->type == REC_TYPE_ENTITY_NAME)
		{
			if (recordPP[ii]->version == 1)
			{
				stkEntityNameRecP = (StkEntityNameRec*)recordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkEntityNameRecP->atr);
#else
				m_wstrName = stkEntityNameRecP->atr;
#endif
			}
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_NodeHeadSP, (StkControl **)recordPP);
	if (recordPP != NULL)
	{
		free(recordPP);
		recordPP = NULL;
	}
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取Node段中的子段
	 */
	while (1)
	{
		eState = i_stkFileP->ReadSegmentHeadData( &head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		switch (head.kind) 
		{
		case SEG_TYPE_INFO:
			stkInfoP = new Stk_Information(m_stkProtoP);
			eState = stkInfoP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkInfoP;
				return eState;
			}
			AddInfo(stkInfoP);
			break;

		case SEG_TYPE_BODY:
			StkBodyP = new Stk_Body(m_stkProtoP);
			eState = StkBodyP->ReadFile(i_stkFileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete StkBodyP;
				return eState;
			}
			SetBody(StkBodyP);

			//m_nuID = StkBodyP->GetID();
			m_dataP = StkBodyP;
			break;

		case SEG_TYPE_MESH:
			StkMeshComP = new Stk_Mesh_Com(m_stkProtoP);
			eState = StkMeshComP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete StkMeshComP;
				StkMeshComP = NULL;
				return eState;
			}
			SetMesh(StkMeshComP);

			m_dataP = StkMeshComP;
			break;

		case SEG_TYPE_PMI:
			StkPMIP = new Stk_PMI(m_stkProtoP);
			eState = StkPMIP->ReadFile(i_stkFileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete StkPMIP;
				return eState;
			}
			SetPMI(StkPMIP); // 添加到ProtoType类的vector成员中

			//m_nuID = StkPMIP->GetID();
			m_dataP = StkPMIP;
			break;

		case SEG_TYPE_VIEW:
			StkViewP = new Stk_View(m_stkProtoP);
			eState = StkViewP->ReadFile(i_stkFileP, &head );
			if (eState!=STK_SUCCESS)
			{
				delete StkViewP;
				return eState;
			}
			SetView(StkViewP);

			//m_nuID = StkViewP->GetID();
			m_dataP = StkViewP;
			break;

		case SEG_TYPE_METADATA:
			stkMetaDataP = new Stk_MetaData(m_stkProtoP);
			eState = stkMetaDataP->ReadMetaData(i_stkFileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete stkMetaDataP;
				stkMetaDataP = NULL;
				return eState;
			}
			SetMetaData(stkMetaDataP);

			//m_nuID = stkMetaDataP->GetID();
			m_dataP = stkMetaDataP;
			break;

		case SEG_TYPE_LAYERSET:
			stkLayerInfosSetP = new Stk_LayerInfosSet(m_stkProtoP);
			eState = stkLayerInfosSetP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkLayerInfosSetP;
				stkLayerInfosSetP = NULL;
				return eState;
			}
			SetLayerInfosSet(stkLayerInfosSetP);
			m_dataP = stkLayerInfosSetP;
			break;
		case SEG_TYPE_FEAT:
			stkFeatP = new Stk_Feat(m_stkProtoP);
			eState = stkFeatP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatP;
				stkFeatP = NULL;
				return eState;
			}
			SetFeat(stkFeatP);
			break;
		case SEG_TYPE_CONNECTOR:
			stkNodeConnectorP = new Stk_NodeConnector(m_stkProtoP);
			eState = stkNodeConnectorP->ReadFile(i_stkFileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete stkNodeConnectorP;
				return eState;
			}

			AddNodeConnector(stkNodeConnectorP);
			break;
		// 当下一段是Node-END表时
		case SEG_TYPE_Node_END:
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
	/*
	 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
	 *	正式版本方案：参考NX，装配和模型数据分开存储。
	 */
	// begin
	//if (m_DocManager != NULL && m_DocManager->GetLoadingCancel() == true)
	//{
	//	return STK_LOADING_CANCEL;
	//}
	// end
	/*
	 *  读取下一个Node
	 */
	STK_SIZE_T idxChild = 0;
	STK_ID idNode = 0;
	PlcNode splcnode = {0};
	while(idxChild < m_vcChildNodeIDs.size() )
	{
		eState = i_stkFileP->ReadSegmentHeadData( &head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		splcnode = m_vcChildNodeIDs[idxChild];
		m_stkProtoP->GetProtoIDManager()->GetNodeByID(splcnode.nodeid, stkNodeP);

		eState = stkNodeP->ReadFile(i_stkFileP, &head );
		if (eState != STK_SUCCESS)
		{
			return eState;
		}
		stkNodeP->AddRef();

		// 如果不从二进制中读取PMI
		if(m_DocManager->GetPMIViewReadFlag()== false &&
			( stkNodeP->GetNodeType() == NODE_TYPE_PMI ||  stkNodeP->GetNodeType() == NODE_TYPE_VIEW))
		{
			DelChildNodeByPlc(splcnode.plcid); // DelChildNodeByPlc删除并释放内存。RemoveChild不释放内存 @@@有问题，暂不影响使用
		}
		else
		{
			idxChild++;
		}
	}

	for (int ii = 0; ii < m_vcChildNodeIDs.size(); ii ++)
	{
		STK_ID idChildPlc = m_vcChildNodeIDs[ii].plcid;
		if (m_idMaxPlc < idChildPlc)
		{
			m_idMaxPlc = idChildPlc;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		写Node文件
 *	@param[in]	i_stkFileP		文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Node::WriteFile(Stk_File *i_stkFileP)
{
	/*
	 *	变量定义
	 */
	STK_STATUS				ret;
	StkSegmentHead			nodeHead = {0};
	StkSegmentHead			endHead = {0};
	StkNodeRec5*			stkNodeRec5P = NULL;
	StkEntityNameRec*		stkEntityNameRecP = NULL;
	STK_UINT32				nuChildNum = 0;
	STK_UINT32				nuEntityNameSize = 0;
	Stk_Node*				stkNodeP = NULL;
	StkSegmentHead			metaDataHead = {0};

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 * 【第1步】 输出Node段
	 */

	// 【1.1 输出段头记录】
	nodeHead.id = m_nuID;
	nodeHead.kind = SEG_TYPE_Node;
	ret = i_stkFileP->WriteSegmentHeadData(&nodeHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	//	【1.2 输出Node记录】
	nuChildNum = m_vcChildNodeIDs.size();
	stkNodeRec5P = (StkNodeRec5 *)calloc(1, sizeof(StkNodeRec5) - sizeof(StkNodeChild) + nuChildNum * sizeof(StkNodeChild));
	if(stkNodeRec5P == NULL){
		return STK_ERROR;
	}

	stkNodeRec5P->Control.type = REC_TYPE_Node;
	stkNodeRec5P->Control.version = _VER_Node_RECORD /*5*/;

	stkNodeRec5P->childnum = nuChildNum;
	stkNodeRec5P->Control.length = sizeof(StkNodeRec5) - sizeof(StkNodeChild) + nuChildNum * sizeof(StkNodeChild) - sizeof(StkControl);
	stkNodeRec5P->IsItem = 0;
	if (m_bDispStat)
	{
		stkNodeRec5P->IsItem |= STK_Node_IS_DISPLAY;
	}
	// Node类型
	stkNodeRec5P->type = m_eType;

	// Node Sub类型
	stkNodeRec5P->subType = m_eSubType;

	//预留
	stkNodeRec5P->usetype = 0;
	stkNodeRec5P->dmy1 = 0;
	stkNodeRec5P->dmy2 = 0;

	/*
	 *	输出子Node
	 */
	for(int ii=0; ii < nuChildNum; ii++)
	{
		stkNodeRec5P->childdat[ii].PlcId = m_vcChildNodeIDs[ii].plcid;
		stkNodeRec5P->childdat[ii].NodeId = m_vcChildNodeIDs[ii].nodeid;
	}

	ret = i_stkFileP->WriteSegmentRecordData(&nodeHead, (StkControl*)stkNodeRec5P);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.3 输出Node Name记录】
	nuEntityNameSize = m_wstrName.size() + 1;
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
	memcpy(stkEntityNameRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkEntityNameRecP->atr, nuEntityNameSize, (WCHAR*)m_wstrName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(&nodeHead, (StkControl*)stkEntityNameRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.4 完成Node段的输出】
	ret = i_stkFileP->WriteSegmentFinishData(&nodeHead);
	if( ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if(stkNodeRec5P != NULL)
	{
		free(stkNodeRec5P);
		stkNodeRec5P = NULL;
	}
	if (stkEntityNameRecP != NULL)
	{
		free(stkEntityNameRecP);
		stkEntityNameRecP = NULL;
	}

	// 【1.5 Info段的输出】
	for (int ii = 0; ii < m_vcInfos.size(); ii++)
	{
		if (m_vcInfos[ii] != NULL)
		{
			ret = m_vcInfos[ii]->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
	}

	// 【1.6 输出Node段的子段】
	switch (m_eType)
	{
	case NODE_TYPE_VIRTUAL:
		// 输出虚节点
		// 注意！虚节点下没有数据，因此这里没有调用任何输出接口
		break;

	case NODE_TYPE_BODY:
		// 输出Body
		if (m_dataP != NULL)
		{
			ret = ((Stk_Body*)m_dataP)->WriteFile(i_stkFileP);
			if (ret!=STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;

	case NODE_TYPE_MESH:
		// 输出Mesh
		if (m_dataP != NULL)
		{
			ret = ((Stk_Mesh_Com*)m_dataP)->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;

	case NODE_TYPE_PMI:
		// 输出PMI
		if (m_dataP != NULL)
		{
			ret = ((Stk_PMI*)m_dataP)->WriteFile(i_stkFileP);
			if (ret!=STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;

	case NODE_TYPE_VIEW:
		// 输出VIEW
		// 注意！Stk_View中可以包含Camera，这个Camera不作为Node存储
		//（参照依据：Dtk的Dtk_ModelDisplay下也是可以包含Camera）
		if (m_dataP != NULL)
		{
			ret = ((Stk_View*)m_dataP)->WriteFile(i_stkFileP);
			if (ret!=STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;

	case NODE_TYPE_META:
		if (m_dataP != NULL)
		{
			// 【第1步】 输出段头记录
			metaDataHead.id = 0;
			metaDataHead.kind = SEG_TYPE_METADATA;
			ret = i_stkFileP->WriteSegmentHeadData(&metaDataHead);
			if (ret < STK_SUCCESS)
			{
				return STK_ERROR;
			}

			// 【第2步】 输出自定义属性记录
			ret = ((Stk_MetaData*)m_dataP)->WriteMetaData(&metaDataHead, i_stkFileP);
			if (ret!=STK_SUCCESS)
			{
				return STK_ERROR;
			}

			// 【第3步】 完成自定义属性段的输出
			ret = i_stkFileP->WriteSegmentFinishData(&metaDataHead);
			if (ret < STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;

	case NODE_TYPE_CAMERA:


		break;

	case NODE_TYPE_LAYERINFOSSET:
		if (m_dataP != NULL)
		{
			ret = ((Stk_LayerInfosSet*)m_dataP)->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;
		
	default:
		break;
	}

	/*
	 * 【第7步】 输出Feat段
	 */
	if (m_stkFeatP != NULL)
	{
		m_stkFeatP->WriteFile(i_stkFileP);
	}

	/*
	 *	【第8步】 输出NodeConnector段
	 */
	for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
	{
		m_vcNodeConnectors[ii]->WriteFile(i_stkFileP);
	}

	/*
	 * 【第8步】 输出Node-END段
	 */
	endHead.id = m_nuID;
	endHead.kind = SEG_TYPE_Node_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if( ret < STK_SUCCESS){
		return STK_ERROR;
	}

	/*
	 *	当前Node的子Node的输出
	 */
	for (int ii = 0; ii < nuChildNum; ii++)
	{
		m_stkProtoP->GetProtoIDManager()->GetNodeByID(m_vcChildNodeIDs[ii].nodeid, stkNodeP);
		stkNodeP->WriteFile(i_stkFileP);
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		填写特征数据
 *	@param[in]	i_stkFeatP		特征数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetFeat(Stk_Feat* i_stkFeatP)
{
	if (i_stkFeatP != NULL)
	{
		m_stkFeatP = i_stkFeatP;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特征数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		特征数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat* Stk_Node::GetFeat()
{
	return m_stkFeatP;
}

/************************************************************************//**
 *	@brief		获取DocManager
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		DocManager
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_DocumentManager* Stk_Node::GetDocManager()
{
	return m_DocManager;
}

/************************************************************************//**
 *	@brief		添加Node与要素之间的关联
 *	@param[in]	i_NodeConnectorP	Node与要素之间的关联
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		STK_ERROR			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::AddNodeConnector(Stk_NodeConnector* i_NodeConnectorP)
{
	if (i_NodeConnectorP != NULL)
	{
		m_vcNodeConnectors.push_back(i_NodeConnectorP);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node与要素之间的关联的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Node与要素之间的关联的数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_Node::GetNodeConnectorNum()
{
	return m_vcNodeConnectors.size();
}

/************************************************************************//**
 *	@brief		获取指定的Node与要素之间的关联
 *	@param[in]	i_nuIndex		指定的关联的索引号
 *	@param[out]	无
 *	@retval		指定的Node与要素之间的关联
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_NodeConnector* Stk_Node::GetNodeConnectorByIndex(STK_UINT32 i_nuIndex)
{
	if (i_nuIndex >= m_vcNodeConnectors.size())
	{
		return NULL;
	}
	return m_vcNodeConnectors[i_nuIndex];
}

/************************************************************************//**
 *	@brief		删除Node与要素之间的关联
 *	@param[in]	i_idNode			被关联的Node ID号
 *	@param[in]	i_PointedEnt		被指向要素类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::DelNodeConnector(STK_ID i_idNode, STK_UINT32 i_PointedEnt)
{
	STK_ID				idNode;
	StkEntityTypeEnum	nuPointingEnt, nuPointedEnt;
	for (int i=0; i < m_vcNodeConnectors.size(); i++)
	{
		Stk_NodeConnector *pConnector = m_vcNodeConnectors.at(i);
		pConnector->GetEntityType(nuPointingEnt,nuPointedEnt);
		idNode = pConnector->GetLinkNode();
		if (idNode == i_idNode && nuPointedEnt == i_PointedEnt)
		{
			m_vcNodeConnectors.erase(m_vcNodeConnectors.begin() + i);
			return STK_SUCCESS;
		}
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		删除Node与实例之间的关联
 *	@param[in]	i_wstrPlcPath		被关联的实例配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::DelNodeConnector(wstring i_wstrPlcPath)
{
	wstring wstrPlcPath;
	for (int i=0; i < m_vcNodeConnectors.size(); i++)
	{
		Stk_NodeConnector *pConnector = m_vcNodeConnectors.at(i);
		wstrPlcPath = pConnector->GetLinkProto();
		if (wstrPlcPath == i_wstrPlcPath)
		{
			m_vcNodeConnectors.erase(m_vcNodeConnectors.begin() + i);
			return STK_SUCCESS;
		}
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		设置Node Sub类型
 *	@param[in]	i_eSubType		Node Sub类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Node::SetNodeSubType(StkNodeSubTypeEnum i_eSubType)
{
	m_eSubType = i_eSubType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Node Sub类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Node Sub类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkNodeSubTypeEnum Stk_Node::GetNodeSubType()
{
	return m_eSubType;
}
