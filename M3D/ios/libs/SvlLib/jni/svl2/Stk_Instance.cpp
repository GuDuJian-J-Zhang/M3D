// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Instance.cpp
 *
 *	@brief	为获取ProtoType时用
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Define.h"
#include "Stk_Instance.h"
#include "Stk_Manager.h"
#include "Stk_DocumentManager.h"

#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_Instance构造函数
 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		浏览器中打开Instance时专用构造函数
 *	@attention	无
 ****************************************************************************/

Stk_Instance::Stk_Instance(Stk_DocumentManager* i_stkDocManagerP)
{
	m_idProto = 0;
	m_bHasColor = FALSE;
	m_eDispStat = STK_DISPLAY;
	m_bHasMaterial = FALSE;
	m_wstrFilePath = L"";
	m_DocManager = i_stkDocManagerP;
	m_nNumber = 0;
}

/************************************************************************//**
 *	@brief		Stk_Instance析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Instance::~Stk_Instance(void)
{
	if (0 != m_idProto)
	{
		Stk_ProtoType* PrototypeP = GetProtoType();
		if (PrototypeP)
		{
			PrototypeP->Release();	
		}
		m_idProto = 0;
	}

	m_DocManager->GetGlobalIDManger()->DelInstanceID(m_ID);
}

/************************************************************************//**
 *	@brief		获取实例ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID		实例ID号
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Instance::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		获取配置ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idPlacement		配置ID号
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Instance::GetPlcID()
{
	return m_idPlacement;
}

/************************************************************************//**
 *	@brief		设置配置ID
 *	@param[in]	i_idPlacement		配置ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Instance::SetPlcID(STK_ID i_idPlacement)
{
	m_idPlacement = i_idPlacement;
}

/************************************************************************//**
 *	@brief		获取ProtoType ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idProto		ProtoType ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Instance::GetProtoID()
{
	return m_idProto;
}

/************************************************************************//**
 *	@brief		设置ProtoType ID
 *	@param[in]	i_idProto		ProtoType ID
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Instance::SetProtoID(STK_ID i_idProto)
{
	m_idProto = i_idProto;
}

/************************************************************************//**
 *	@brief		获取实例名
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrInsName		实例名
 *	@note		如果没有实例名，则返回ProtoType名
 *	@attention	无
 ****************************************************************************/

wstring Stk_Instance::GetInsName()
{
	wstring wstrName = m_wstrInsName;

	if (m_wstrInsName == L"" && GetProtoType()!=NULL)
	{
		wstrName = GetProtoType()->GetProtoName();
	}

	return wstrName;
}

/************************************************************************//**
 *	@brief		设置实例名
 *	@param[in]	i_wstrInsName		实例名
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Instance::SetInsName(const wstring &i_wstrInsName)
{
	m_wstrInsName = i_wstrInsName;
}

/************************************************************************//**
 *	@brief		注册ID
 *	@param[in]	i_idInstance	实例ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Instance::RegisterID(STK_ID i_idInstance)
{
	STK_STATUS ret = m_DocManager->GetGlobalIDManger()->RegInstanceID(this, i_idInstance);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idInstance;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		设置组件路径
 *	@param[in]	i_wstrLink		组件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Instance::SetProtoLink(const wstring &i_wstrLink)
{
	m_wstrProtoLink = i_wstrLink;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取组件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrProtoLink		组件路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Instance::GetProtoLink()
{
	return m_wstrProtoLink;
}

/************************************************************************//**
 *	@brief		注册组件路径
 *	@param[in]	i_wstrLink			组件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_SUCCESS_EXIST	已注册
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Instance::RegProtoLink(const wstring &i_wstrLink)
{
	STK_STATUS	eState;
	eState = m_DocManager->GetGlobalIDManger()->RegProtoLinkString(i_wstrLink, m_idProto);

	return eState;
}

/************************************************************************//**
 *	@brief		读SVL文件
 *	@param[in]	i_FileP					文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS				读取成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Instance::ReadFile(Stk_File *i_FileP)
{
	// 注意这个ReadFile方法，不能加入StkSegmentHead *inHead
	// 原因是该实例不一定需要从文件中读取ProtoType

	STK_STATUS		eState;
	StkSegmentHead	head ={0};

	/*
	 *  注册ProtoLink。如果已注册则跳出
	 */
	eState = RegProtoLink(m_wstrProtoLink);
	if (eState == STK_SUCCESS_EXIST)
	{
		eState = STK_SUCCESS;
		return eState;
	}

	/*
	 *  从SVL中读取Component，创建对象
	 */
	Stk_ProtoType *ProtoTypeP = new Stk_ProtoType(m_wstrProtoLink,m_DocManager);
	ProtoTypeP->RegisterID(STK_NEW_ID);
	ProtoTypeP->SetFile(i_FileP);

	eState = i_FileP->ReadSegmentHeadData(&head);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	eState = ProtoTypeP->ReadCADFile(i_FileP, &head); //必须在RegProtoLink判断之后
	if (eState!=STK_SUCCESS)
	{
		delete ProtoTypeP;
		return eState;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写实例数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[in]	i_eSaveType			文件保存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Instance::WriteFile(Stk_File *i_stkFileP, StkFileSaveTypeEnum i_eSaveType)
{
	Stk_ProtoType *stkProtoP = NULL;

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	stkProtoP = GetProtoType();
	if(stkProtoP != NULL)
	{
		stkProtoP->WriteCADFile( i_stkFileP, i_eSaveType);
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}


/************************************************************************//**
 *	@brief		获取ProtoType
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		ProtoType对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType*  Stk_Instance::GetProtoType()
{

	Stk_ProtoType* ProtoTypeP = NULL;

	if (m_idProto == STK_NEW_ID)
	{
		return NULL;
	}

	m_DocManager->GetGlobalIDManger()->GetProtoTypeByID(m_idProto, ProtoTypeP);

	return ProtoTypeP;

}

/************************************************************************//**
 *	@brief		获取配置ID，配置矩阵
 *	@param[out]	o_idPlacement			配置ID
 *	@param[out]	o_mtxPlcMatrix			配置矩阵
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Instance::GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix)
{
	o_idPlacement = m_idPlacement;
	o_mtxPlcMatrix = m_mtxPlacement;

	return STK_SUCCESS;
};

/************************************************************************//**
 *	@brief		设置配置ID，配置矩阵
 *	@param[in]	i_idPlacement			配置ID
 *	@param[in]	i_mtxPlcMatrix			配置矩阵
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Instance::SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix)
{
	m_idPlacement = i_idPlacement;
	m_mtxPlacement = i_mtxPlcMatrix;
}

/************************************************************************//**
 *	@brief		实例是否有颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasColor		是否有颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Instance::HasColor()
{
	return m_bHasColor;
}

/************************************************************************//**
 *	@brief		设置实例颜色
 *	@param[in]	i_fRed			红色
 *	@param[in]	i_fGreen		绿色
 *	@param[in]	i_fBlue			蓝色
 *	@param[in]	i_fAlpha		透明度
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Instance::SetColor(STK_FLOAT32 i_fRed, STK_FLOAT32 i_fGreen, STK_FLOAT32 i_fBlue, STK_FLOAT32 i_fAlpha)
{
	m_rgbaInstance.Red = i_fRed;
	m_rgbaInstance.Green = i_fGreen;
	m_rgbaInstance.Blue = i_fBlue;
	m_rgbaInstance.Alpha = i_fAlpha;

	m_bHasColor = TRUE;

}

/************************************************************************//**
 *	@brief		获取实例颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaInstance	实例颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_Instance::GetColor()
{
	return m_rgbaInstance;
}

/************************************************************************//**
 *	@brief		设置实例的显隐
 *	@param[in]	i_eDispState	实例的显隐状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Instance::SetDisplayState(StkDisplayStateEnum i_eDispState)
{
	m_eDispStat = i_eDispState;
}

/************************************************************************//**
 *	@brief		获取实例的显隐
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eDispStat	实例的显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkDisplayStateEnum Stk_Instance::GetDisplayState()
{
	return m_eDispStat;
}

/************************************************************************//**
 *	@brief		设置实例材质
 *	@param[in]	i_matInstance	实例材质
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Instance::SetMaterial(const STK_MAT32 &i_matInstance)
{
	m_matInstance = i_matInstance;
	m_bHasMaterial = TRUE;
}

/************************************************************************//**
 *	@brief		获取实例材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_matInstance	实例材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_MAT32 Stk_Instance::GetMaterial()
{
	return m_matInstance;
}

/************************************************************************//**
 *	@brief		判断是否有材质
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasMaterial	是否有材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Instance::HasMaterial()
{
	return m_bHasMaterial;
}

/************************************************************************//**
 *	@brief		获取LOD级数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		LOD等级数
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Instance::GetLODCount()
{
	return GetProtoType()->GetLODCount();
}
/************************************************************************//**
 *	@brief		获取SVL文件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrFilePath		文件路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Instance::GetFilePath()
{
	return m_wstrFilePath;
}

/************************************************************************//**
 *	@brief		设置SVL文件路径
 *	@param[in]	i_wstrfilePath	文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::SetFilePath(wstring i_wstrfilePath)
{
	m_wstrFilePath = i_wstrfilePath;
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		设置特征编号
 *	@param[in]	i_nNumber			特征编号
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::SetNumber(STK_INT32 i_nNumber)
{
	m_nNumber = i_nNumber;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特征编号
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		特征编号
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_INT32 Stk_Instance::GetNumber()
{
	return m_nNumber;
}

/************************************************************************//**
 *	@brief		设置实例标识GUID
 *	@param[in]	i_wstrGUID			实例标识GUID
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::SetGUID(wstring i_wstrGUID)
{
	m_wstrGUID = i_wstrGUID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取实例标识GUID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例标识GUID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Instance::GetGUID()
{
	return m_wstrGUID;
}

/*************************************************************************//**
 *	@brief		往当前实例中添加自定义属性数据
 *	@param[in]	i_MetaDataP		自定义属性数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	if (i_MetaDataP != NULL)
	{
		m_vcMetaDatas.push_back(i_MetaDataP);
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取当前实例中所有的自定义属性数据数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_SIZE_T Stk_Instance::GetMetaDataNum()
{
	return m_vcMetaDatas.size();
}

/*****************************************************************************
 *	@brief		获取当前实例中指定的自定义属性数据
 *	@param[in]	i_nuIndex		自定义数据索引
 *	@param[out]	无
 *	@retval		指定的自定义属性数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_MetaData* Stk_Instance::GetMetaDataByIndex(STK_UINT32 i_nuIndex)
{
	if (i_nuIndex >= m_vcMetaDatas.size())
	{
		return NULL;
	}
	return m_vcMetaDatas.at(i_nuIndex);
}

/************************************************************************//**
 *	@brief		通过自定义属性名删除指定的对象
 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@retval		STK_ERROR			删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::DelMetaDataByTitle(wstring i_wstrMetaDataTitle)
{
	wstring	wstrTitle = L"";
	vector<Stk_MetaData*>::iterator itor = m_vcMetaDatas.begin();
	while(itor != m_vcMetaDatas.end())
	{
		wstrTitle = (*itor)->GetTitle();
		if (wstrTitle == i_wstrMetaDataTitle)
		{
			delete (*itor);
			m_vcMetaDatas.erase(itor);
			return STK_SUCCESS;
		}
		itor++;
	}

	return STK_ERROR; //没有找到，删除失败
}

/*************************************************************************//**
 *	@brief		设置工艺BOM的序号
 *	@param[in]	i_wstrBOMID			工艺BOM的序号
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Instance::SetVirtualBOMID(wstring i_wstrBOMID)
{
	m_wstrBOMID = i_wstrBOMID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取工艺BOM的序号
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		工艺BOM的序号
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Instance::GetVirtualBOMID()
{
	return m_wstrBOMID;
}