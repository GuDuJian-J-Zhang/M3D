// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_ProtoType.cpp
 *
 *	@brief	ProtoType
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
 *		2014/04/24	更新。Gaoqq@HOTEAMSOFT
 *					支持Andriod平台
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Define.h"
#include "Stk_ProtoType.h"
#include "Stk_Manager.h"
#include "Stk_DocumentManager.h"
#include "Stk_Platform.h"
#include "Stk_Utility.h"
#include "Stk_DocumentManager.h"
#include "Stk_GroupFeature.h"
#include "Stk_PatternFeature.h"
#include "Stk_AssemblyFeature.h"
#include "Utility.h"

#ifndef __MOBILE__
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/****************************************************************************
 *	@brief		Stk_ProtoType构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		浏览器中，打开ProtoType时专用构造函数
 *	@attention	无
 ****************************************************************************/
#if 1
// @@@
//Stk_ProtoType::Stk_ProtoType()
//{
//	m_wstrLink = L"";
//	m_wstrName = L"";
//	m_idRefCount = 0;
//	m_nuLODCount = 1;
//	m_bIsDirtyData = TRUE;
//// 	m_wstrCADFileName = L"";
//// 	m_wstrFilePathName = L"";
//// 	m_wstrDataSource = L"";
//// 	m_nuPolygonNum = 0;
//	m_eProtoType = PROTO_TYPE_UNKNOWN;
//	m_DocManager = NULL;
//	m_ProtoIDManager = NULL;
//	m_isCatalog = false;
//	m_bIsNodeMode = false;
//	m_isCADFileTop = true;
//	m_stkFileP = NULL;
//	m_idPlcMax = 0;
//
//}
#endif

/************************************************************************//**
 *	@brief		Stk_ProtoType构造函数
 *	@param[in]	i_wstrLink			LinkString
 *	@param[in]	i_stkDocManagerP	DocManager
 *	@param[out]	无
 *	@retval		无
 *	@note		浏览器中，打开ProtoType时专用构造函数
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType::Stk_ProtoType(const wstring &i_wstrLink, Stk_DocumentManager* i_stkDocManagerP)
{
	//m_typeID = STK_TYPE_DOCUMENT;
	m_wstrLink = i_wstrLink;
	m_wstrName = L"";
	m_idRefCount = 0;
	m_nuLODCount = 1;
	m_bIsDirtyData = TRUE;
// 	m_wstrCADFileName = L"";
// 	m_wstrFilePathName = L"";
// 	m_wstrDataSource = L"";
// 	m_nuPolygonNum = 0;
	m_eProtoType = PROTO_TYPE_UNKNOWN;
	m_DocManager = i_stkDocManagerP;
	m_ProtoIDManager = new Stk_ProtoIDManager();
	m_TopNodeP = NULL;
	m_isCatalog = false;
	m_bIsNodeMode = false;
	m_isCADFileTop = false;
	m_stkFileP = NULL;
	m_idPlcMax = 0;
	m_bPipe = false;
	m_dUnitFactor = 1.0;
	m_szUserDataCnt = 0;
	m_ID = 0;
	m_bIsDeleting = false;
	m_bIsUseNumber = false;
	m_bHasColor = false;
	m_bDisplay = true;
	m_rgbaProtoType.Alpha = 0;
	m_rgbaProtoType.Blue = 0;
	m_rgbaProtoType.Green = 0;
	m_rgbaProtoType.Red = 0;
}

/****************************************************************************
 *	@brief		Stk_ProtoType拷贝构造函数
 *	@param[in]	i_stkProtoType	ProtoType
 *	@param[out]	无
 *	@retval		无
 *	@note		浏览器中，打开ProtoType时专用构造函数
 *	@attention	无
 ****************************************************************************/

//Stk_ProtoType::Stk_ProtoType(const Stk_ProtoType& i_stkProtoType)
//{
	//vector<Stk_Instance*> vcChildInss = i_stkProtoType.m_vcChildInss;
	//vector<Stk_Body*> vcBodys = i_stkProtoType.m_vcBodys;
	//vector<Stk_PMI*> vcPMIs = i_stkProtoType.m_vcPMIs;
	//vector<Stk_View*> vcViews = i_stkProtoType.m_vcViews;
	//vector<Stk_Note*> vcNotes = i_stkProtoType.m_vcNotes;
	//vector<Stk_MetaData*> vcMetaDatas = i_stkProtoType.m_vcMetaDatas;

	//m_ID = i_stkProtoType.m_ID;
	//m_wstrName = i_stkProtoType.m_wstrName;
	//m_eProtoType = i_stkProtoType.m_eProtoType;
	//m_wstrLink = i_stkProtoType.m_wstrLink;
	//m_wstrPath = i_stkProtoType.m_wstrPath;
	//m_mapChildInss = i_stkProtoType.m_mapChildInss;
	//m_idRefCount= i_stkProtoType.m_idRefCount;
	//m_nuLODCount = i_stkProtoType.m_nuLODCount;
	//m_bIsDirtyData = i_stkProtoType.m_bIsDirtyData;
	//m_wstrProtokey = i_stkProtoType.m_wstrProtokey;
	//m_wstrCADFileName = i_stkProtoType.m_wstrCADFileName;
	//m_wstrFilePathName = i_stkProtoType.m_wstrFilePathName;
	//m_wstrDataSource = i_stkProtoType.m_wstrDataSource;
	//m_nuPolygonNum = i_stkProtoType.m_nuPolygonNum;
	//m_DocManager = new Stk_DocumentManager();
	//memcpy(m_DocManager, i_stkProtoType.m_DocManager, sizeof(Stk_DocumentManager));
	//m_ProtoIDManager = new Stk_ProtoIDManager();
	//memcpy(m_ProtoIDManager, i_stkProtoType.m_ProtoIDManager, sizeof(Stk_ProtoIDManager));
	//for (int ii = 0; ii < vcChildInss.size(); ii++)
	//{
	//	Stk_Instance* InstanceP = NULL;
	//	InstanceP = new Stk_Instance();
	//	if (InstanceP != NULL)
	//	{
	//		memcpy(InstanceP, vcChildInss[ii], sizeof(*(vcChildInss[ii])));
	//		m_vcChildInss.push_back(InstanceP);
	//	}
	//}
	//for (int ii = 0; ii < vcBodys.size(); ii++)
	//{
	//	Stk_Body* BodyP = NULL;
	//	BodyP = new Stk_Body();
	//	if (BodyP != NULL)
	//	{
	//		memcpy(BodyP, vcBodys[ii], sizeof(*(vcBodys[ii])));
	//		m_vcBodys.push_back(BodyP);
	//	}
	//}
	//for (int ii = 0; ii < vcPMIs.size(); ii++)
	//{
	//	Stk_PMI* stkPMIP = NULL;
	//	stkPMIP = new Stk_PMI();
	//	if (stkPMIP != NULL)
	//	{
	//		memcpy(stkPMIP, vcPMIs[ii], sizeof(*(vcPMIs[ii])));
	//		m_vcPMIs.push_back(stkPMIP);
	//	}
	//}
	//for (int ii = 0; ii < vcViews.size(); ii++)
	//{
	//	Stk_View* ViewP = NULL;
	//	ViewP = new Stk_View();
	//	if (ViewP != NULL)
	//	{
	//		memcpy(ViewP, vcViews[ii], sizeof(*(vcViews[ii])));
	//		m_vcViews.push_back(ViewP);
	//	}
	//}
	//for (int ii = 0; ii < vcNotes.size(); ii++)
	//{
	//	Stk_Note* NoteP = NULL;
	//	NoteP = new Stk_Note();
	//	if (NoteP != NULL)
	//	{
	//		memcpy(NoteP, vcNotes[ii], sizeof(*(vcNotes[ii])));
	//		m_vcNotes.push_back(NoteP);
	//	}
	//}
	//for (int ii = 0; ii < vcMetaDatas.size(); ii++)
	//{
	//	Stk_MetaData* MetaDataP = NULL;
	//	MetaDataP = new Stk_MetaData(vcMetaDatas[ii]->GetProtoType());
	//	if (MetaDataP != NULL)
	//	{
	//		memcpy(MetaDataP, vcMetaDatas[ii], sizeof(*(vcMetaDatas[ii])));
	//		m_vcMetaDatas.push_back(MetaDataP);
	//	}
	//}
//}

/************************************************************************//**
 *	@brief		Stk_ProtoType析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType::~Stk_ProtoType(void)
{
	m_bIsDeleting = true;

#if 0 // V2.2后数据在Node中存储和释放
	for (int ii = 0; ii < m_vcBodys.size(); ii++)
	{
		if (m_vcBodys[ii] != NULL)
		{
			delete m_vcBodys[ii];
			m_vcBodys[ii] = NULL;
		}
	}
	//PMI释放
	for (int ii = 0; ii < m_vcPMIs.size(); ii++)
	{
		if (m_vcPMIs[ii] != NULL)
		{
			delete m_vcPMIs[ii];
			m_vcPMIs[ii] = NULL;
		}
	}
	//View释放
	for (int ii = 0; ii < m_vcViews.size(); ii++)
	{
		if (m_vcViews[ii] != NULL)
		{
			delete m_vcViews[ii];
			m_vcViews[ii] = NULL;
		}
	}
	//3D Note释放
	for (int ii = 0; ii < m_vcNotes.size(); ii++)
	{
		if (m_vcNotes[ii] != NULL)
		{
			delete m_vcNotes[ii];
			m_vcNotes[ii] = NULL;
		}
	}
#endif
	//自定义属性释放
	for (int ii = 0; ii < m_vcMetaDatas.size(); ii++)
	{
		if (m_vcMetaDatas[ii] != NULL)
		{
			delete m_vcMetaDatas[ii];
			m_vcMetaDatas[ii] = NULL;
		}
	}

	// 实例属性释放
	map<wstring, Stk_InsAttribute*>::iterator itorInsAttr = m_mapInsAttribute.begin();
	while(itorInsAttr != m_mapInsAttribute.end())
	{
		if (itorInsAttr->second != NULL)
		{
			delete itorInsAttr->second;
			itorInsAttr->second = NULL;
		}
		itorInsAttr++;
	}

	if(m_TopNodeP)
	{
		m_TopNodeP->Release();
	}

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

	//渲染数据释放
	for (int ii = 0; ii < m_vcRenders.size(); ii++)
	{
		if (m_vcRenders[ii] != NULL)
		{
			delete m_vcRenders[ii];
			m_vcRenders[ii] = NULL;
		}
	}

	DelAllChildren();

	if(m_ProtoIDManager){
		delete m_ProtoIDManager;
		m_ProtoIDManager = NULL;
	}

	m_DocManager->GetGlobalIDManger()->DelProtoTypeID(m_ID);
	m_DocManager->GetGlobalIDManger()->DelProtoLinkString(m_wstrLink);

	m_bIsDeleting = false;
}

/****************************************************************************
 *	@brief		Stk_ProtoType赋值构造函数
 *	@param[in]	i_stkProtoType	Proto
 *	@param[out]	无
 *	@retval		当前Proto
 *	@note		浏览器中，打开ProtoType时专用构造函数
 *	@attention	无
 ****************************************************************************/

//Stk_ProtoType& Stk_ProtoType::operator = (const Stk_ProtoType& i_stkProtoType)
//{
	//if (this == &i_stkProtoType)
	//{
	//	return *this;
	//}
	//vector<Stk_Instance*> vcChildInss = i_stkProtoType.m_vcChildInss;
	//vector<Stk_Body*> vcBodys = i_stkProtoType.m_vcBodys;
	//vector<Stk_PMI*> vcPMIs = i_stkProtoType.m_vcPMIs;
	//vector<Stk_View*> vcViews = i_stkProtoType.m_vcViews;
	//vector<Stk_Note*> vcNotes = i_stkProtoType.m_vcNotes;
	//vector<Stk_MetaData*> vcMetaDatas = i_stkProtoType.m_vcMetaDatas;

	//m_ID = i_stkProtoType.m_ID;
	//m_wstrName = i_stkProtoType.m_wstrName;
	//m_eProtoType = i_stkProtoType.m_eProtoType;
	//m_wstrLink = i_stkProtoType.m_wstrLink;
	//m_wstrPath = i_stkProtoType.m_wstrPath;
	//m_mapChildInss = i_stkProtoType.m_mapChildInss;
	//m_idRefCount= i_stkProtoType.m_idRefCount;
	//m_nuLODCount = i_stkProtoType.m_nuLODCount;
	//m_bIsDirtyData = i_stkProtoType.m_bIsDirtyData;
	//m_wstrProtokey = i_stkProtoType.m_wstrProtokey;
	//m_wstrCADFileName = i_stkProtoType.m_wstrCADFileName;
	//m_wstrFilePathName = i_stkProtoType.m_wstrFilePathName;
	//m_wstrDataSource = i_stkProtoType.m_wstrDataSource;
	//m_nuPolygonNum = i_stkProtoType.m_nuPolygonNum;
	//m_DocManager = new Stk_DocumentManager();
	//memcpy(m_DocManager, i_stkProtoType.m_DocManager, sizeof(Stk_DocumentManager));
	//m_ProtoIDManager = new Stk_ProtoIDManager();
	//memcpy(m_ProtoIDManager, i_stkProtoType.m_ProtoIDManager, sizeof(Stk_ProtoIDManager));
	//for (int ii = 0; ii < vcChildInss.size(); ii++)
	//{
	//	Stk_Instance* InstanceP = NULL;
	//	InstanceP = new Stk_Instance();
	//	if (InstanceP != NULL)
	//	{
	//		memcpy(InstanceP, vcChildInss[ii], sizeof(*(vcChildInss[ii])));
	//		m_vcChildInss.push_back(InstanceP);
	//	}
	//}
	//for (int ii = 0; ii < vcBodys.size(); ii++)
	//{
	//	Stk_Body* BodyP = NULL;
	//	BodyP = new Stk_Body();
	//	if (BodyP != NULL)
	//	{
	//		memcpy(BodyP, vcBodys[ii], sizeof(*(vcBodys[ii])));
	//		m_vcBodys.push_back(BodyP);
	//	}
	//}
	//for (int ii = 0; ii < vcPMIs.size(); ii++)
	//{
	//	Stk_PMI* stkPMIP = NULL;
	//	stkPMIP = new Stk_PMI();
	//	if (stkPMIP != NULL)
	//	{
	//		memcpy(stkPMIP, vcPMIs[ii], sizeof(*(vcPMIs[ii])));
	//		m_vcPMIs.push_back(stkPMIP);
	//	}
	//}
	//for (int ii = 0; ii < vcViews.size(); ii++)
	//{
	//	Stk_View* ViewP = NULL;
	//	ViewP = new Stk_View();
	//	if (ViewP != NULL)
	//	{
	//		memcpy(ViewP, vcViews[ii], sizeof(*(vcViews[ii])));
	//		m_vcViews.push_back(ViewP);
	//	}
	//}
	//for (int ii = 0; ii < vcNotes.size(); ii++)
	//{
	//	Stk_Note* NoteP = NULL;
	//	NoteP = new Stk_Note();
	//	if (NoteP != NULL)
	//	{
	//		memcpy(NoteP, vcNotes[ii], sizeof(*(vcNotes[ii])));
	//		m_vcNotes.push_back(NoteP);
	//	}
	//}
	//for (int ii = 0; ii < vcMetaDatas.size(); ii++)
	//{
	//	Stk_MetaData* MetaDataP = NULL;
	//	MetaDataP = new Stk_MetaData(vcMetaDatas[ii]->GetProtoType());
	//	if (MetaDataP != NULL)
	//	{
	//		memcpy(MetaDataP, vcMetaDatas[ii], sizeof(*(vcMetaDatas[ii])));
	//		m_vcMetaDatas.push_back(MetaDataP);
	//	}
	//}

	//return *this;
//}

/************************************************************************//**
 *	@brief		获取ProtoType ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	ProtoType ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_ProtoType::GetID()
{
	return m_ID;
}

/*************************************************************************//**
 *	@brief		设置ProtoType ID
 *	@param[in]	i_ID			ProtoType ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置ProtoType所属的File
 *	@param[in]	i_stkFileP		文件对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

VOID  Stk_ProtoType::SetFile(Stk_File* i_stkFileP)
{
	m_stkFileP = i_stkFileP;
}

/*************************************************************************//**
 *	@brief		获取ProtoType所属的File
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_stkFileP		ProtoType所属的File
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_File*   Stk_ProtoType::GetFile()
{
	return m_stkFileP;
}


/*************************************************************************//**
 *	@brief		是否为CGR类型文件
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_isCatalog		是否为CGR标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsCatalog()
{
	return m_isCatalog;
}

/*************************************************************************//**
 *	@brief		设置该ProtoType为CADFile的Top标志
 *	@param[in]	i_isCADFileTop		是否为TopProtoType
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
VOID Stk_ProtoType::SetCADFileTop(STK_BOOL i_isCADFileTop)
{
	m_isCADFileTop = i_isCADFileTop;
}


/*************************************************************************//**
 *	@brief		是否为CADFile的TopProtoType
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_isCADFileTop		是否为TopProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsCADFileTop()
{
	return m_isCADFileTop;
}

/*************************************************************************//**
 *	@brief		设置是否为Catalog型的标识
 *	@param[in]	i_bCatalog		是否为Catalog
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
VOID Stk_ProtoType::SetCatalog(STK_BOOL i_bCatalog)
{
	m_isCatalog = i_bCatalog;
}

/************************************************************************//**
 *	@brief		获取ProtoType的名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName	ProtoType的名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_ProtoType::GetProtoName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		获取ProtoType的Key
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrProtokey		ProtoType的Key
 *	@note		将PMI存储在外部XML文件的临时方案专用
 *	@attention	无
 ****************************************************************************/

wstring Stk_ProtoType::GetProtoKey()
{
	return m_wstrProtokey;
}

/*************************************************************************//**
 *	@brief		设置ProtoType的名称
 *	@param[in]	i_wstrName			ProtoType的名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		ProtoType名字不允许出现'<'、'>'等特殊字符，该字符被转换为'_'
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetProtoName(const wstring& i_wstrName)
{
	wstring		wstrNameTmp = i_wstrName;
	m_wstrName = Stk_Utility::ReplaceImproperCode(wstrNameTmp);

	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		获取ProtoType Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrLink		ProtoType Link
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_ProtoType::GetLinkString()
{
	return m_wstrLink;
}

/************************************************************************//**
 *	@brief		设置ProtoType Link
 *	@param[in]	i_wstrLink		ProtoType Link
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_ProtoType::SetLinkString(wstring i_wstrLink)
{
	m_wstrLink = i_wstrLink;
}

/*************************************************************************//**
 *	@brief		获取CAD文件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrCADPath	CAD文件路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::GetCADFilePath()
{
	return m_wstrCADPath;
}

/*************************************************************************//**
 *	@brief		设置CAD物理路径路径
 *	@param[in]	i_wstrCADFilePath		CAD物理文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		通过SetCADFilePath时会自动匹配并填写CADFileName
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetCADFilePath(const wstring& i_wstrCADFilePath)
{
	Stk_File*	stkFileP = NULL;
	m_wstrCADPath = i_wstrCADFilePath;
	m_bIsDirtyData = TRUE;
	stkFileP = GetFile();
	if (stkFileP == NULL)
	{
		m_wstrCADFileName = ComputeCADFileName(i_wstrCADFilePath);
		return STK_ERROR;
	}
	m_wstrCADFileName = stkFileP->ComputeCADFileName(i_wstrCADFilePath);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		注册ProtoType ID
 *	@param[in]	i_idProtoType	ProtoType ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::RegisterID(STK_ID i_idProtoType)
{
	STK_STATUS ret=m_DocManager->GetGlobalIDManger()->RegProtoTypeID(this, i_idProtoType );
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idProtoType;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}
	
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoIDManager
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ProtoIDManager		ProtoType内的ID管理器
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_ProtoIDManager* Stk_ProtoType::GetProtoIDManager()
{
	return m_ProtoIDManager;
}

#if 0
/************************************************************************//**
 *	@brief		读取文件中的ProtoType数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_ProtoHeadP		Proto记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR			读取失败
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		V2.2之前使用此接口，如果已存在将V2.2之前文件转换到V2.2以后的处理，则此接口废弃
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ProtoHeadP)
{
	StkSegmentHead	head ={0};
	StkControl		**RecordPP = NULL;
	STK_UINT32		nuElemCount = 0;
	STK_UINT32		ii,jj;
	STK_ID			idFileProto;
	STK_MTX32		mtxPlacement;
	STK_ID			idPlacement;
	Stk_Instance	*CurInsP = NULL;
	wstring			wstrCurLink;
	wstring			wstrInsName;
	WCHAR			wchIDA[32];
	wstring			wstrPlcPath;
	STK_UINT32		nuStrIdx;
	STK_FLOAT32		fInsColorA[4]={0};
	STK_UINT32		nuHasItem = 0;
	Stk_Body		*StkBodyP = NULL; 
	Stk_PMI			*StkPMIP = NULL;
	Stk_View		*StkViewP = NULL;
	Stk_MetaData*	stkMetaDataP = NULL;
	STK_STATUS		state;
	wstring			wstrLoadPath = L"";
	wstring			wstrChildInsSavePath = L"";
	wstring			wstrPaProtoLoadPath = L"";
	STK_BOOL		flag = false;
	STK_UINT32		nuDisplayState = 0;
	STK_STATUS		eState;
	STK_ID			idTopNode = 1;

	/*
	 * 读取ProtoType段的数据
	 */
	RecordPP = (StkControl **)calloc(i_ProtoHeadP->recnum, sizeof(StkControl*));
	if (i_ProtoHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_ProtoHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取ProtoType段的各记录
	 */
	while (1)
	{
		eState = i_FileP->ReadSegmentHeadData( &head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		switch (head.kind) 
		{
		case SEG_TYPE_Node:
			wstrCurLink.erase();
			//_itow_s( idFileProto, wchIDA, 10);
			swprintf(wchIDA, 32, L"%d", idTopNode); //支持Andriod
			wstrCurLink += wchIDA;
			wstrCurLink += L"|";
			swprintf(wchIDA, 32, L"%d", m_ID); //支持Andriod
			wstrCurLink += wchIDA;
			wstrCurLink += L"|";
			wstrCurLink += i_FileP->GetCADFilePath();

			// 设置顶级Node
			m_TopNodeP = new Stk_Node(m_DocManager,this);
			m_TopNodeP->SetLinkString(wstrCurLink);
			
			eState = m_TopNodeP->RegNodeLink();
			if (eState == STK_SUCCESS_EXIST)
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				break;
			}
			if (eState == STK_ERROR  )
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				return eState;
			}
			m_TopNodeP->RegisterID(head.id);
			
			eState = m_TopNodeP->ReadFile(i_FileP,&head);
			if (eState!=STK_SUCCESS)
			{
				delete m_TopNodeP;
				return eState;
			}
			SetNodeMode(true);

			// 如果不从二进制中读取PMI
			if(m_DocManager->GetPMIViewReadFlag() && m_TopNodeP->GetNodeType() == NODE_TYPE_PMI)
			{
				eState = m_TopNodeP->Release();
				if ( eState == STK_SUCCESS)
				{
					m_TopNodeP = NULL;
				}
			}
			break;

		case SEG_TYPE_BODY:
			StkBodyP = new Stk_Body(this);
			eState = StkBodyP->ReadFile(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete StkBodyP;
				return eState;
			}
			AddBody(StkBodyP);
			break;

		case SEG_TYPE_PMI:
			if(m_DocManager->GetPMIViewReadFlag())
			{
				StkPMIP = new Stk_PMI(this);
				eState = StkPMIP->ReadFile(i_FileP, &head);
				if (eState!=STK_SUCCESS)
				{
					delete StkPMIP;
					return eState;
				}
				AddPMI(StkPMIP); // 添加到ProtoType类的vector成员中
				break;
			}
			else
			{
				StkControl **TmpRecordPP = NULL;
				TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
				if (head.recnum !=0 && TmpRecordPP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
				if (eState!=STK_SUCCESS)
				{
					return eState;
				}
				eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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

		case SEG_TYPE_VIEW:
			if(m_DocManager->GetPMIViewReadFlag())
			{
				StkViewP = new Stk_View(this);
				eState = StkViewP->ReadFile(i_FileP, &head );
				if (eState!=STK_SUCCESS)
				{
					delete StkViewP;
					return eState;
				}
				AddView(StkViewP);
				break;
			}
			else
			{
				StkControl **TmpRecordPP = NULL;
				TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
				if (head.recnum !=0 && TmpRecordPP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
				if (eState!=STK_SUCCESS)
				{
					return eState;
				}
				eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
		case SEG_TYPE_NOTE:
			stkNoteP = new Stk_Note(this);
			eState = stkNoteP->ReadNote(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			AddNote(stkNoteP);
			break;

		case SEG_TYPE_METADATA:
			eState = ReadMetaDatas(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			break;

		// 当下一段是ProtoType-END表时
		case SEG_TYPE_PROTO_END:
			goto pt1;
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
				return eState;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *  填写ProtoType类的子实例
	 */
#if 0
	_itow_s(i_ProtoHeadP->id, wchIDA, 10);
	wstrCurLink += wchIDA;
	wstrCurLink += L'|';
	//wstrCurLink += curFilePath; //### 多Document时需要改
	wstrCurLink += i_FileP->GetCADFilePath(); //### 多Document时需要改

	if (wstrCurLink != m_wstrLink)
	{
		return STK_ERROR;
	}
#endif
	
	//  根据ProtoType段的内容，
	//  创建子实例并设定实例状态
	for (ii = 0; ii < i_ProtoHeadP->recnum; ii++)
	{

		// 创建子实例
		if (RecordPP[ii]->type == REC_TYPE_PROTO)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType*)RecordPP[ii])->name);
#else
				m_wstrName = ((StkProtoType*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType*)RecordPP[ii])->childnum;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType*)RecordPP[ii])->childdat[jj].childId;
					memcpy(&mtxPlacement, ((StkProtoType*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16 );
					idPlacement = ((StkProtoType*)RecordPP[ii])->childdat[jj].plcId;

#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					//wstrCurLink += curFilePath; //### 多Document时需要改
					wstrCurLink += i_FileP->GetCADFilePath();

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetProtoLink(wstrCurLink);
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName(wstrInsName); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement(idPlacement, mtxPlacement);
					m_vcChildInss.push_back(CurInsP);
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement, m_vcChildInss.size() - 1));
				}
			}
			else if ( RecordPP[ii]->version == 2)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName =Stk_Platform::STKWCHARStrToWString(((StkProtoType2*)RecordPP[ii])->name);
#else
				m_wstrName = ((StkProtoType2*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType2*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType2*)RecordPP[ii])->childnum;

				for (jj=0; jj<nuElemCount; jj++)
				{

					idFileProto = ((StkProtoType2*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType2*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType2*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType2*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType2*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					//wstrCurLink += curFilePath; //### 多Document时需要改
					wstrCurLink += i_FileP->GetCADFilePath();

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本3	added by Shao in 2013/12/25
			else if ( RecordPP[ii]->version == 3)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->name);				
#else
				m_wstrName = ((StkProtoType3*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType3*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType3*)RecordPP[ii])->childnum;
				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType3*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType3*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType3*)RecordPP[ii])->childdat[jj].plcId;
					
#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType3*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetCADFilePath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType3*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetCADFilePath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath;
					}

					if (wstrLoadPath == wstrPaProtoLoadPath)		// 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					}
					else
						CurInsP->SetFilePath(L"Local");
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本4
			else if ( RecordPP[ii]->version == 4)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->name);	
#else
				m_wstrName = ((StkProtoType4*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType4*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType4*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType4*)RecordPP[ii])->types;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType4*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType4*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType4*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType4*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetCADFilePath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType4*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetCADFilePath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
						wstrLoadPath = wstrPaProtoLoadPath;

					if (wstrLoadPath == wstrPaProtoLoadPath)		// 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//_itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					}
					else
						CurInsP->SetFilePath(L"Local");
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size()-1));
				}
			}
			// 版本5
			else if ( RecordPP[ii]->version == 5)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->name);	
#else
				m_wstrName = ((StkProtoType5*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType5*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType5*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType5*)RecordPP[ii])->types;
				
				if( ((StkProtoType5*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType5*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType5*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType5*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType5*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetCADFilePath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType5*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetCADFilePath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath;
					}

					if (wstrLoadPath == wstrPaProtoLoadPath)		// 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//_itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					}
					else
					{
						CurInsP->SetFilePath(L"Local");
					}
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			else
			{
				//错误
			}
		}

		// 读取PROTO TITLE信息	Added by Shaoqy on 2014/1/6
		else if (RecordPP[ii]->type == REC_TYPE_PROTO_TITLE)
		{
			if(RecordPP[ii]->version == 1)
			{
// #ifdef __MOBILE__
// 				m_wstrCADFileName = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->FileName);
// 				Stk_Utility::ReplaceImproperCode(m_wstrCADFileName);
// 				m_wstrFilePathName = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->FilePathName);
// 				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->DataSource);
// 				m_nuPolygonNum = ((StkProtoTitleRec*)RecordPP[ii])->PolygonNum;
// #else
// 				m_wstrCADFileName = ((StkProtoTitleRec*)RecordPP[ii])->FileName;
// 				Stk_Utility::ReplaceImproperCode(m_wstrCADFileName);  //ProtoType名字不允许出现'<'、'>'字符，该字符被转换为'_'
// 				m_wstrFilePathName = ((StkProtoTitleRec*)RecordPP[ii])->FilePathName;
// 				m_wstrDataSource = ((StkProtoTitleRec*)RecordPP[ii])->DataSource;
// 				m_nuPolygonNum = ((StkProtoTitleRec*)RecordPP[ii])->PolygonNum;
// #endif
			}
		}

		// 获取ProtoType的Key
		else if( RecordPP[ii]->type == REC_TYPE_PROTO_KEY){
			// 版本1
			if( RecordPP[ii]->version == 1){
#ifdef __MOBILE__
				m_wstrProtokey = Stk_Platform::STKWCHARStrToWString(((StkProtoKey*)RecordPP[ii])->atr);
#else
				m_wstrProtokey = ((StkProtoKey*)RecordPP[ii])->atr;
#endif
				state = i_FileP->RegProtoTypeKey(m_wstrProtokey,this);
				if( state!= STK_SUCCESS){
					return state;
				}
			}
		}
		// 获取子实例的状态信息
		else if(RecordPP[ii]->type == REC_TYPE_INS_ATR)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
#ifdef __MOBILE__
				wstrPlcPath = Stk_Platform::STKWCHARStrToWString(((StkInstanceAtr*)RecordPP[ii])->atr);
#else
				wstrPlcPath = ((StkInstanceAtr*)RecordPP[ii])->atr;
#endif
				nuStrIdx = wstrPlcPath.find( L"|",0 );
				nuStrIdx = wstrPlcPath.find( L"|",nuStrIdx+1 );
				idPlacement = wcstol(wstrPlcPath.substr(nuStrIdx+1).c_str(),NULL,10);

#if 0 // 提高读取大规模实例的SVL文件打开速度 add by Gaoqq on 2013.10.21
				nuElemCount = m_vcChildInss.size();
				for (jj=0; jj<nuElemCount; jj++)
				{
					CurInsP = m_vcChildInss[jj];
					if (CurInsP->GetPlcID() == idPlacement)
						break;
				}
#else
				CurInsP = GetChildByPlcID(idPlacement);
			}
#endif
		}

		// 设定子实例的颜色
		else if (RecordPP[ii]->type == REC_TYPE_INS_COL)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				memcpy( fInsColorA, ((StkInstanceColor*)RecordPP[ii])->color, sizeof(STK_FLOAT32)*4 );
				CurInsP->SetColor( fInsColorA[0], fInsColorA[1], fInsColorA[2], fInsColorA[3] );
			}
		}

		// 设定子实例的显隐状态
		else if (RecordPP[ii]->type == REC_TYPE_INS_DSP)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				nuHasItem = ((StkInstanceDisplay*)RecordPP[ii])->disp;
				if (nuHasItem == STK_INS_NODISP)
				{
					CurInsP->SetDisplayState(STK_NO_DISPLAY);
				}
			}
		}

		// 设定子实例的材质
		else if (RecordPP[ii]->type == REC_TYPE_INS_MAT)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				STK_MAT32 insMat;
				insMat.Ambient  = ((StkInstanceMater*)RecordPP[ii])->mat[0];
				insMat.Diffuse  = ((StkInstanceMater*)RecordPP[ii])->mat[1];
				insMat.Specular = ((StkInstanceMater*)RecordPP[ii])->mat[2];

				if (nuHasItem == STK_INS_NODISP)
				{
					CurInsP->SetMaterial(insMat);
				}
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_PLCDISPLAY)
		{
			if (RecordPP[ii]->version == 1)
			{
				wstrPlcPath = ((StkPlcPathDisplayRec*)RecordPP[ii])->plcPath;
				nuDisplayState = ((StkPlcPathDisplayRec*)RecordPP[ii])->nuDisplayState;
				AddDisplayState(wstrPlcPath, (StkDisplayStateEnum)nuDisplayState);
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_ProtoHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *  读取下一个ProtoType
	 */
	nuElemCount = m_vcChildInss.size();
	for (ii = 0; ii < nuElemCount; ii++)
	{
		CurInsP = m_vcChildInss[ii];
		if (CurInsP->GetFilePath() != L"" && CurInsP->GetFilePath() != L"Local"/*i_FileP->GetCADFilePath()*/)	// 判断是否有分散路径，若有则跳出，返回StkDocManager。add by Shaoqy on 2014/01/09
		{
			STK_ID idProto = 0;
			//CurInsP->RegProtoLink(CurInsP->GetProtoLink());
			m_DocManager->GetGlobalIDManger()->RegProtoLinkString(CurInsP->GetProtoLink(),idProto);
			continue;
		}
		eState = CurInsP->ReadFile(i_FileP);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		CurInsP->GetProtoType()->AddRef(); //add by Gaoqq on 2013.6.29
	}

	// 文件类型
	if( flag){
		if ( nuElemCount >0)
		{
			m_eProtoType = PROTO_TYPE_ASSY; //### 下一步在ProtoType表中直接读取
		}
		else
		{
			m_eProtoType = PROTO_TYPE_PART;
		}
	}

	for (int ii = 0; ii < m_vcChildInss.size(); ii ++)
	{
		STK_ID idChildPlc = m_vcChildInss[ii]->GetPlcID();
		if (m_idPlcMax < idChildPlc)
			m_idPlcMax = idChildPlc;
	}
	return STK_SUCCESS;
}
#endif

/*************************************************************************//**
 *	@brief		保存CAD文件中的ProtoType
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[in]	i_eSaveType			保存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
 *	@retval		STK_ERROR			保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::WriteCADFile(Stk_File *i_stkFileP, StkFileSaveTypeEnum i_eSaveType)
{
	StkSegmentHead compHead = {0};
	StkSegmentHead endHead = {0};
	STK_UINT32 nuChildNum = 0;
	STK_MTX32 mtxMatrix = {0};
	STK_UINT32 nPlcID = 0;
	vector<Stk_Instance *> vcChildInss;
	StkProtoType12 *protoRec12SP = NULL;
	StkInstanceAtr *insAtrSP = NULL;
	STK_SIZE_T szBodyNum = 0;
	STK_SIZE_T szPMINum = 0;
	STK_SIZE_T szViewNum = 0;
	STK_SIZE_T szNoteNum = 0;
	STK_SIZE_T szMetaDataNum = 0;
	Stk_Body *stkBodyP = NULL;
	Stk_Mesh_Com *stkMeshP = NULL;
	STK_STATUS ret = STK_SUCCESS;
	STK_RGBA32 rgbaInsColor;
	UINT charsize;
	StkProtoKey *ProtoKey = NULL;
	StkProtoTitleRec stkTitleRec = {0};
	wstring wstrSaveDir = L"";
	wstring wstrSVLPath = L"";
	wstring wstrProtoName = L"";
	StkPlcPathDisplayRec	plcPathDisplayRec;
	//map<wstring, StkDisplayStateEnum>::iterator itor = m_mapDisplayState.begin();
	Stk_ProtoType* stkChildPrototypeP = NULL;
	wstring wstrChildCADFilePath = L"";
	STK_ID idProtoType = 0;
	WCHAR wchProtoTypeID[STK_MAXNAME];
	StkComponentFeatureEnum	eComponentType = TYPE_COMPONENTFEATURE_UNKNOWN;
	map<wstring, Stk_InsAttribute*>::iterator itorInsAttr = m_mapInsAttribute.begin();
	wstring			wstrPassword = L"";
	Stk_File*		stkFileTmpP = NULL;

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
	if(!m_bIsDirtyData)
	{
		return STK_SUCCESS_EXIST;
	}
	/*
	 * 【第1步】 输出ProtoType段
	 */
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
	if (1 == stkSVLLog.bLogOutput && 
		(1 == stkSVLLog.bPrototype || 
		1 == stkSVLLog.bBody || 
		1 == stkSVLLog.bPMIAndView || 
		1 == stkSVLLog.bMetaData || 
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
			_T("【ProtoType段】输出 名称：%s"), m_wstrName.c_str());
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// 【1.1 输出段头记录】
	compHead.id = m_ID;
	compHead.kind = SEG_TYPE_PROTO;
	ret = i_stkFileP->WriteSegmentHeadData(&compHead);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	//	【1.2 输出ProtoTye记录】
	nuChildNum = m_vcChildInss.size();
	protoRec12SP = (StkProtoType12 *)calloc(1, sizeof(StkProtoType12) - sizeof(StkProtChild8) + nuChildNum * sizeof(StkProtChild8));
	if(protoRec12SP == NULL){
		return STK_ERROR;
	}

 	protoRec12SP->Control.type = REC_TYPE_PROTO;
	protoRec12SP->Control.version = _VER_PROTO_RECORD /*12*/;
#ifdef __MOBILE__
	memcpy(protoRec12SP->name,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
	memcpy(protoRec12SP->CADFilePath, Stk_Platform::WStringToSTKWCHARStr(m_wstrCADPath), (m_wstrCADPath.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(protoRec12SP->name,STK_ULTRANAME,(WCHAR*)m_wstrName.c_str());
	wcscpy_s(protoRec12SP->CADFilePath, STK_MAXPATH, (WCHAR*)m_wstrCADPath.c_str());
#endif
	protoRec12SP->LODnum = m_nuLODCount;
	protoRec12SP->childnum = nuChildNum;
	protoRec12SP->Control.length = sizeof(StkProtoType12) - sizeof(StkProtChild8) + nuChildNum * sizeof(StkProtChild8)  - sizeof(StkControl);

	//预留
	protoRec12SP->usetype = 0;
	protoRec12SP->dmy1 = 0;
	protoRec12SP->dmy2 = 0;
	protoRec12SP->types = m_eProtoType;
	protoRec12SP->UnitFactor = m_dUnitFactor; //单位

	protoRec12SP->IsItem = 0;
	if( m_bIsNodeMode)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_NODE_MODE; 
	}
	if( m_TopNodeP)
	{
		protoRec12SP->topNode = m_TopNodeP->GetID();
	}
	if( m_isCatalog)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_CATALOG;
	}
	if (m_isCADFileTop)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_TOP;
	}
	if (m_bPipe)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_PIPE;
	}
	if (m_bIsUseNumber)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_USE_NUMBER;
	}
	if (m_bHasColor)
	{
		protoRec12SP->IsItem |= STK_PROTO_HAS_COLOR;
		protoRec12SP->Color = m_rgbaProtoType;
	}
	if (m_bDisplay)
	{
		protoRec12SP->IsItem |= STK_PROTO_IS_DISPLAY;
	}

	/*
	 *	输出子实例
	 */
#ifdef _WINDOWS_
	if (bLogOutFlag)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("输出ProtoType Base 长度：%d 文件类型：%d 子实例数：%d 自定义属性数： %d"), protoRec12SP->Control.length, protoRec12SP->types, protoRec12SP->childnum, m_vcMetaDatas.size());
		stkSVLLogFile->Log(strConvertLog);

	}
#endif

	// 当为分散存储且存在子实例时，需根据顶级Proto保存的路径获取子实例保存的相对路径
	if (/*m_DocManager->GetFileSaveType() == FILESAVE_TYPE_DISTRIBUTION &&*/ nuChildNum > 0)	
	{
		stkFileTmpP = this->GetFile();
		if (stkFileTmpP == NULL)
		{
			return STK_ERROR;
		}
		wstrSVLPath = stkFileTmpP->GetSVLPath();
		wstrSaveDir = ConvertPathToDir(wstrSVLPath);
	}

	for(int ii=0; ii < nuChildNum; ii++)
	{
		Stk_ProtoType *stkProtoP  = m_vcChildInss[ii]->GetProtoType();
		if ( stkProtoP == NULL)
		{
			memset(protoRec12SP->childdat[ii].filePath, 0, STK_MAXPATH);
			continue;
		}
		idProtoType = stkProtoP->GetID();
		protoRec12SP->childdat[ii].childId = idProtoType;
		protoRec12SP->childdat[ii].nuType = stkProtoP->GetType();
		m_vcChildInss[ii]->GetPlacement(nPlcID, mtxMatrix);
		protoRec12SP->childdat[ii].plcId = nPlcID;		// 保证装配数显示正确
		protoRec12SP->childdat[ii].nNumber = m_vcChildInss[ii]->GetNumber();
		memcpy(protoRec12SP->childdat[ii].Matrix, mtxMatrix.PlcMatrix, sizeof(STK_FLOAT32) * 16 );
		wstrPassword = L"";
		stkFileTmpP = NULL;
		stkFileTmpP = stkProtoP->GetFile();
		/*
		 *	由于转换器存在直转当前实例场景，在这种模式下，子实例的Proto并没有注册File。
		 *	而直转下要建立BOM结构，需要后续的文件名、Proto名等，因此，就算File为空，也需要继续向下执行。
		 *	不过这种方式存在风险：后面如果存在需要用File的数据该怎么办呢？所以继续向下执行，也是无奈之举吧。
		 *	本身直转当前实例这种场景的转换就是不正确的。
		 */
		//if (stkFileTmpP == NULL)
		//{
		//	continue;
		//}
		if (stkFileTmpP != NULL)
		{
			wstrPassword = stkFileTmpP->GetPassword();
		}
		
#ifdef __MOBILE__
		memcpy(protoRec12SP->childdat[ii].InsName,Stk_Platform::WStringToSTKWCHARStr(m_vcChildInss[ii]->GetInsName()),
			(m_vcChildInss[ii]->GetInsName().size()+1)*sizeof(STK_WCHAR));
		memcpy(protoRec12SP->childdat[ii].ProtoName, Stk_Platform::WStringToSTKWCHARStr(stkProtoP->GetProtoName()),
			(stkProtoP->GetProtoName().size()+1)*sizeof(STK_WCHAR));
		memcpy(protoRec12SP->childdat[ii].wchGUID, Stk_Platform::WStringToSTKWCHARStr(m_vcChildInss[ii]->GetGUID()),
			(m_vcChildInss[ii]->GetGUID().size()+1)*sizeof(STK_WCHAR));
		memcpy(protoRec12SP->childdat[ii].Password, Stk_Platform::WStringToSTKWCHARStr(wstrPassword),
			(wstrPassword.size()+1)*sizeof(STK_WCHAR));
		memcpy(protoRec12SP->childdat[ii].wchBomID, Stk_Platform::WStringToSTKWCHARStr(m_vcChildInss[ii]->GetVirtualBOMID()),
			(m_vcChildInss[ii]->GetVirtualBOMID().size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(protoRec12SP->childdat[ii].InsName, STK_ULTRANAME, (WCHAR*)m_vcChildInss[ii]->GetInsName().c_str());
		wcscpy_s(protoRec12SP->childdat[ii].ProtoName, STK_ULTRANAME, (WCHAR*)stkProtoP->GetProtoName().c_str());
		wcscpy_s(protoRec12SP->childdat[ii].wchGUID, STK_MAXNAME, (WCHAR*)m_vcChildInss[ii]->GetGUID().c_str());
		wcscpy_s(protoRec12SP->childdat[ii].Password, STK_MAXPATH, (WCHAR*)wstrPassword.c_str());
		wcscpy_s(protoRec12SP->childdat[ii].wchBomID, STK_MAXNAME, (WCHAR*)m_vcChildInss[ii]->GetVirtualBOMID().c_str());
#endif

		wstring wstrChildInsPath = L"";
		swprintf(wchProtoTypeID, STK_MAXNAME, L"%d", idProtoType);
		wstrProtoName = stkProtoP->GetProtoName();
		//wstrChildInsPath += wchProtoTypeID;
		wstrChildInsPath += wstrProtoName;		// 路径更改为ProtoName|物理文件路径+.svl
		wstrChildInsPath += L"|";

		// 设置子实例路径
		// = 单一	CADFileName|Local
		// = 分散	CADFileName|子Proto所在SVL文件路径
		// = 混合	
		if (i_eSaveType == FILESAVE_TYPE_UNIQUE)
		{
			wstrChildInsPath += L"Local";
		}
		else if (i_eSaveType == FILESAVE_TYPE_DISTRIBUTION)
		{
			wstrChildInsPath += wstrSaveDir;
			wstrChildInsPath += stkProtoP->GetCADFileName();
			wstrChildInsPath += L".svl";
		}
		else if (i_eSaveType == FILESAVE_TYPE_BEFOR)
		{
			//### 待扩展
			wstring wstrPath = L"";
			DetectPathFromBeforeSave(this, stkProtoP, wstrPath);
			wstrChildInsPath += wstrPath;
			//wstrChildInsPath += stkFileTmpP->GetSVLPath();
		}

#ifdef __MOBILE__
		memcpy(protoRec12SP->childdat[ii].filePath,Stk_Platform::WStringToSTKWCHARStr(wstrChildInsPath),(wstrChildInsPath.size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(protoRec12SP->childdat[ii].filePath, STK_MAXPATH, wstrChildInsPath.c_str());	// add by Shaoqy on 2014/01/09
#endif

#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog,  _countof(strConvertLog), 
				_T("-子实例 名称：%s"), protoRec12SP->childdat[ii].InsName);
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
	}

	ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl*)protoRec12SP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	【1.3 输出ProtoType的Key】
	 */
	if(!m_wstrProtokey.empty())
	{
		charsize = m_wstrProtokey.size() + 1;

		ProtoKey = (StkProtoKey *)calloc(1,sizeof(StkControl) + sizeof(UINT)+ charsize*sizeof(STK_WCHAR) );
		ProtoKey->Control.type = REC_TYPE_PROTO_KEY;
		ProtoKey->Control.version = _VER_PROTO_KEY_RECORD;
		ProtoKey->charnum = charsize;
#ifdef __MOBILE__
		memcpy(ProtoKey->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrProtokey),(m_wstrProtokey.size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(ProtoKey->atr,charsize,(WCHAR*)m_wstrProtokey.c_str());
#endif
		ProtoKey->Control.length = sizeof(UINT)+ charsize*sizeof(STK_WCHAR);

		ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)ProtoKey);
		if( ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (bLogOutFlag)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-Key记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}
	}

	
	// 【1.4 输出实例状态记录】
	Stk_Instance *stkChildIns = NULL;
	for(int ii=0; ii < nuChildNum; ii++){
		stkChildIns = m_vcChildInss[ii];
		// 仅当该实例有状态时，才输出状态记录
		// 目前包括颜色、显隐、材质状态
		if(!stkChildIns->HasColor() && stkChildIns->GetDisplayState() != 1 && !stkChildIns->HasMaterial()){
			continue;
		}

		// 实例属性记录
		WCHAR strID[32];
		wstring strPlcPath = L"PATH|0";		// 因为Datakit目前只能获取一级子实例的状态
		STK_ID idPlc = stkChildIns->GetPlcID();
		//_itow_s(idPlc, strID, 10);
		swprintf(strID, 32, L"%d", idPlc); //支持Andriod
		strPlcPath += L"|";
		strPlcPath += strID;
		STK_UINT32 nuCharSize = strPlcPath.size() + 1;
		insAtrSP = (StkInstanceAtr*)calloc(1, sizeof(StkControl) + sizeof(UINT)+ nuCharSize * sizeof(STK_WCHAR));
		insAtrSP->Control.type = REC_TYPE_INS_ATR;
		insAtrSP->Control.version = _VER_INS_ATR_RECORD;
		insAtrSP->charnum = nuCharSize;
		insAtrSP->Control.length = sizeof(UINT) + nuCharSize * sizeof(STK_WCHAR);
#ifdef __MOBILE__
		memcpy(insAtrSP->atr,Stk_Platform::WStringToSTKWCHARStr(strPlcPath),(strPlcPath.size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(insAtrSP->atr, nuCharSize, (WCHAR*)strPlcPath.c_str());
#endif

		ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)insAtrSP);
		if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
			if (bLogOutFlag)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-实例属性记录写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		// 实例颜色记录
		if(stkChildIns->HasColor())
		{
			StkInstanceColor InsColor;
			rgbaInsColor = stkChildIns->GetColor();
			InsColor.Control.type = REC_TYPE_INS_COL;
			InsColor.Control.version = _VER_INS_COL_RECORD;
			InsColor.color[0] = rgbaInsColor.Red;
			InsColor.color[1] = rgbaInsColor.Green;
			InsColor.color[2] = rgbaInsColor.Blue;
			InsColor.color[3] = rgbaInsColor.Alpha;
			InsColor.Control.length = sizeof(float) * 4;
			
			ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)&InsColor);
			if( ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (bLogOutFlag)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-实例颜色记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
		}

		// 实例显隐状态
		if( stkChildIns->GetDisplayState() == 1)
		{
			StkInstanceDisplay insDisp;
			insDisp.Control.type = REC_TYPE_INS_DSP;
			insDisp.Control.version = _VER_INS_DSP_RECORD;
			insDisp.disp = STK_INS_NODISP;
			insDisp.Control.length = sizeof(UINT);

			ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)&insDisp);
			if( ret < STK_SUCCESS)
			{
#ifdef _WINDOWS_
				if (bLogOutFlag)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-显隐状态记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);
				}
#endif
				return STK_ERROR;
			}
		}

		// 实例材质
		if( stkChildIns->HasMaterial())
		{
			STK_MAT32 mater = stkChildIns->GetMaterial();
			StkInstanceMater insMat;
			insMat.Control.type = REC_TYPE_INS_MAT;
			insMat.Control.version = _VER_INS_MAT_RECORD;
			insMat.mat[0] = mater.Ambient;
			insMat.mat[1] = mater.Diffuse;
			insMat.mat[2] = mater.Specular;
			insMat.Control.length = sizeof(STK_FLOAT32) * 3;

			ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)&insMat);
			if( ret < STK_SUCCESS){
#ifdef _WINDOWS_
				if (bLogOutFlag)
				{
					_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段-材质记录写入失败！"));
					stkSVLLogFile->Log(strConvertLog);	
				}
#endif
				return STK_ERROR;
			}
		}
		// 实例自定义属性
		for (int ii = 0; ii < stkChildIns->GetMetaDataNum(); ii++)
		{
			Stk_MetaData* stkmetaDataP = NULL;
			stkmetaDataP = stkChildIns->GetMetaDataByIndex(ii);
			if (stkmetaDataP != NULL)
			{
				stkmetaDataP->WriteMetaData(&compHead, i_stkFileP);
			}
		}
	}

#if 0
	// 【1.5 输出Prototype的配置路径与实例显隐映射的记录】
	while(itor != m_mapDisplayState.end())
	{
		wstring		wstrPlcPath = itor->first;
		plcPathDisplayRec.Control.type = REC_TYPE_PLCDISPLAY;
		plcPathDisplayRec.Control.length = sizeof(StkPlcPathDisplayRec) - sizeof(StkControl);
		plcPathDisplayRec.Control.version = _VER_PLCPATHDISPLAY_RECORD;
#ifdef __MOBILE__
		memcpy(plcPathDisplayRec.plcPath,Stk_Platform::WStringToSTKWCHARStr(wstrPlcPath),(wstrPlcPath.size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(plcPathDisplayRec.plcPath, STK_MAXPATH, wstrPlcPath.c_str());
#endif
		plcPathDisplayRec.nuDisplayState = itor->second;
		ret = i_stkFileP->WriteSegmentRecordData(&compHead, (StkControl *)&plcPathDisplayRec);
		if(ret < STK_SUCCESS)
		{
			return STK_ERROR;
		}
		itor++;
	}
#endif

	// 【1.6 完成ProtoType段的输出】
	ret = i_stkFileP->WriteSegmentFinishData(&compHead);
	if( ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}
	if(protoRec12SP != NULL)
	{
		free(protoRec12SP);
		protoRec12SP = NULL;
	}
	if (ProtoKey != NULL)
	{
		free(ProtoKey);
		ProtoKey = NULL;
	}
	if(insAtrSP != NULL)
	{
		free(insAtrSP);
		insAtrSP = NULL;
	}

	/*
	 * 【第2步】 输出Node段、Node-END段
	 */
	if (m_TopNodeP != NULL)
	{
		m_TopNodeP->WriteFile(i_stkFileP);
	}

#if 0 //@@@ 正式使用时关闭
	// 注意！对于V2.156前的SVL文件，打开时Body、PMI、View等都自动转换为Node方式
	
	/* 
	 * 【第2步】 输出Body段、Body-END段
	 *          （内部含有Mesh段）
	 */
	szBodyNum = m_vcBodys.size();
	for(int ii = 0; ii < szBodyNum; ii++){
		// 输出Body
		stkBodyP = m_vcBodys[ii];
		stkBodyP->WriteFile(i_stkFileP);
	}

	/*
	 * 【第3步】 输出PMI段
	 */
	szPMINum =  m_vcPMIs.size();
	for(int ic = 0; ic < szPMINum; ic++){
		m_vcPMIs[ic]->WriteFile(i_stkFileP);
	}

	/*
	 * 【第4步】 输出View段
	 */
	szViewNum = m_vcViews.size();
	for(int ic = 0; ic < szViewNum; ic++){
		m_vcViews[ic]->WriteFile(i_stkFileP);
	}
	
	/*
	 *	【第5步】 输出Note段
	 */
	szNoteNum = m_vcNotes.size();
	for (int ic = 0; ic < szNoteNum; ic++)
	{
		m_vcNotes[ic]->WriteNote(i_stkFileP);
	}
#endif

	/*
	 *	【第6步】 输出自定义属性段
	 */
	szMetaDataNum = m_vcMetaDatas.size();
	if (szMetaDataNum > 0)
	{
		WriteMetaData(i_stkFileP);
	}

	/*
	 *	【第7步】 输出用户数据段
	 */
	if(m_szUserDataCnt>0){
		WriteUserData(i_stkFileP);
	}

	/*
	 *	【第8步】 输出组件特征段
	 */
	for (int ii = 0; ii < m_vcComponentFeats.size(); ii++)
	{
		if (m_vcComponentFeats[ii] != NULL)
		{
			eComponentType = m_vcComponentFeats[ii]->GetType();
			switch(eComponentType)
			{
			case TYPE_COMPONENTFEATURE_GROUP:
				((Stk_GroupFeature*)m_vcComponentFeats[ii])->WriteFile(i_stkFileP);
				break;
			case TYPE_COMPONENTFEATURE_PATTERN:
				((Stk_PatternFeature*)m_vcComponentFeats[ii])->WriteFile(i_stkFileP);
				break;
			case TYPE_COMPONENTFEATURE_ASSEMBLY:
				((Stk_AssemblyFeature*)m_vcComponentFeats[ii])->WriteFile(i_stkFileP);
				break;
			default:
				break;
			}
		}
	}

	/*
	 *	【第9步】 输出ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
	 */
	while(itorInsAttr != m_mapInsAttribute.end())
	{
		ret = WriteInsAttribute(itorInsAttr->first, itorInsAttr->second, i_stkFileP);
		if (ret < STK_SUCCESS)
		{
			return STK_ERROR;
		}
		itorInsAttr++;
	}

	/*
	 *	【第10步】 输出渲染(纹理)段
	 */
	for (int ii = 0; ii < m_vcRenders.size(); ii++)
	{
		ret = m_vcRenders[ii]->WriteFile(i_stkFileP);
		if (ret < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	/*
	 *	【第11步】 输出ProtoType-END段
	 */
#ifdef _WINDOWS_
	if (bLogOutFlag)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("输出ProtoType-END 名称：%s"), m_wstrName.c_str());
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	endHead.id = m_ID;
	endHead.kind = SEG_TYPE_PROTO_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if( ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (bLogOutFlag)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：ProtoType-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	/*
	 *	下级实例的填充
	 */
	for(int ii = 0; ii < nuChildNum; ii++)
	{
		// 注意！判断当前Prototype对应的CAD文件路径与父级CAD文件路径是否一致
		// 如果路径不一致，说明是不同的CAD File，需要写新的SVL文件
		stkChildPrototypeP = m_vcChildInss[ii]->GetProtoType();
		if (stkChildPrototypeP == NULL)
		{
			continue;
		}
		wstrChildCADFilePath = stkChildPrototypeP->GetCADFilePath();
		if (wstrChildCADFilePath != m_wstrCADPath)
		{
			continue;
		}
		m_vcChildInss[ii]->WriteFile( i_stkFileP, i_eSaveType);
	}

	m_bIsDirtyData = FALSE;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType的子实例数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcChildInss.size()	子实例数目
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_ProtoType::GetChildInsNum()
{
	return m_vcChildInss.size();
}

/************************************************************************//**
 *	@brief		获取ProtoType中指定的子实例
 *	@param[in]	i_nuKeyPose		要获取的子实例索引
 *	@param[out]	无
 *	@retval		pInstance		子实例
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Instance* Stk_ProtoType::GetChildIns(STK_UINT32 i_nuKeyPose)
{
	Stk_Instance* pInstance = NULL;

	if( i_nuKeyPose > m_vcChildInss.size()){
		return NULL;
	}

	pInstance = m_vcChildInss.at(i_nuKeyPose);

	return pInstance;
}

/************************************************************************//**
 *	@brief		通过配置ID获取子实例
 *	@param[in]	i_idPlacement	配置ID
 *	@param[out]	无
 *	@retval		pInstance		子实例
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Instance* Stk_ProtoType::GetChildByPlcID(STK_ID i_idPlacement)
{
	Stk_Instance* pInstance = NULL;
	map<STK_ID,STK_UINT32>::iterator itor;

	itor = m_mapChildInss.find(i_idPlacement);
	if (itor == m_mapChildInss.end())
	{
		return NULL;
	}

	pInstance = GetChildIns(itor->second);
	return pInstance;
}

/*****************************************************************************
 *	@brief		插入实例
 *	@param[in]	stkInsP			实例对象
 *	@param[in]	i_IDPlc			配置ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@retval		其它			添加失败
 *	@note		1. 由用户指定配置ID <br>
 *				2. 需要调用者为子实例的ProtoType进行AddRef<br>
				3. 返回STK_ERROR_LOOP时，<br>
				   表示插入的子Proto与当前Proto存在嵌套，<br>
				   需由转换端在日志中说明此错误。
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddChild(STK_ID i_IDPlc, Stk_Instance* stkInsP)
{
	STK_ID				idChildProtoType = 0;
	Stk_ProtoType*		stkProtoTypeP = NULL;
	//注意！
	//AddChild()是为ProtoType增加子实例的底层公共函数。事实上不建议由调用侧填写PlcID
	//而是由ProtoType通过m_idPlcMax的方式自动为其发行。考虑到已有用户（转换器转换、浏览器直接打开ProE）使用，暂不修改。
	stkProtoTypeP = stkInsP->GetProtoType();
	if(stkInsP == NULL || stkProtoTypeP == NULL)
	{
		return STK_ERROR;
	}

	idChildProtoType = stkProtoTypeP->GetID();
	if (m_ID == idChildProtoType)
	{
		return STK_ERROR_LOOP;
	}

	map<STK_ID, STK_UINT32>::iterator itor = m_mapChildInss.find(i_IDPlc);
	if(itor == m_mapChildInss.end())
	{
		m_mapChildInss.insert(pair<STK_ID, STK_UINT32>(i_IDPlc, m_mapChildInss.size()));
		m_vcChildInss.push_back(stkInsP);
		m_bIsDirtyData = TRUE;

		//m_eProtoType = PROTO_TYPE_ASSY; //转换成ASSY类型 //按照规划要求，参考Creo View不改变类型

		if (m_idPlcMax < i_IDPlc)
		{
			m_idPlcMax = i_IDPlc;	
		}
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/*************************************************************************//**
 *	@brief		插入实例
 *	@param[in]	i_stkInsP		实例对象
 *	@param[out]	无
 *	@retval		idPlc			配置ID
 *	@note		是对AddChild的封装 <br>
 *				1. 自动发行配置ID <br>
 *				2. 配置矩阵被置成单位阵 <br>
 *				3. 需要调用者为子实例的ProtoType进行AddRef
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_ProtoType::InsertChild(Stk_Instance* i_stkInsP)
{
	//STK_ID idPlc = 0;
	STK_MTX32 mtxUnit = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

	for (int ii = 0; ii < m_vcChildInss.size(); ii ++)
	{
		STK_ID idChildPlc = m_vcChildInss[ii]->GetPlcID();
		if (m_idPlcMax < idChildPlc)
			m_idPlcMax = idChildPlc;
	}
	m_idPlcMax++;
	i_stkInsP->SetPlacement(m_idPlcMax, mtxUnit);

	// 插入实例
	AddChild(m_idPlcMax, i_stkInsP);

	return m_idPlcMax;
}
/************************************************************************//**
 *	@brief		获取Body数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcBodys.size()	Body数目
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_ProtoType::GetBodyNum()
{
	vector<Stk_Node*>	vcNodes;

	PreOrderNode(m_TopNodeP, NODE_TYPE_BODY, vcNodes);
	return vcNodes.size();
}

/************************************************************************//**
 *	@brief		添加Body信息
 *	@param[in]	i_StkBodyP		Body对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入Body型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	无
 ****************************************************************************/

STK_STATUS  Stk_ProtoType::AddBody(Stk_Body* i_StkBodyP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = AddEntityToTopNode(NODE_TYPE_BODY, (Stk_Entity*)i_StkBodyP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	m_bIsDirtyData = TRUE;
	return eState;
}

/************************************************************************//**
 *	@brief		获取指定的Body信息
 *	@param[in]	i_nuKeyPose		指定的Body索引
 *	@param[out]	无
 *	@retval		StkBodyP		Body
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Body* Stk_ProtoType::GetBody(STK_UINT32 i_nuKeyPose)
{
	Stk_Body*			stkBodyP = NULL;
	vector<Stk_Node*>	vcNodes;
	
	PreOrderNode(m_TopNodeP, NODE_TYPE_BODY, vcNodes);
	stkBodyP = (Stk_Body *)((vcNodes.at(i_nuKeyPose))->GetBody());

	return stkBodyP;
}

/****************************************************************************
 *	@brief		通过ID获取Body
 *	@param[in]	i_idBody		指定的BodyID
 *	@param[out]	无
 *	@retval		StkBodyP		Body
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::GetBodyByID(STK_ID i_idBody, Stk_Body* &o_BodyP)
{
	m_ProtoIDManager->GetBodyByID(i_idBody, o_BodyP);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eProtoType	ProtoType类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkProtoTypeEnum Stk_ProtoType::GetType()
{
	return m_eProtoType;
}

/************************************************************************//**
 *	@brief		设置ProtoType类型
 *	@param[in]	i_eType		ProtoType类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
VOID Stk_ProtoType::SetType( StkProtoTypeEnum i_eType)	
{
	m_eProtoType = i_eType;
}

/************************************************************************//**
 *	@brief		获取View视图数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcViews.size()	View视图数目
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_ProtoType::GetViewNum()
{
	vector<Stk_Node*>	vcNodes;

	PreOrderNode(m_TopNodeP, NODE_TYPE_VIEW, vcNodes);

	return vcNodes.size();
}

/************************************************************************//**
 *	@brief		添加View信息
 *	@param[in]	i_StkViewP		View对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入View型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	无
 ****************************************************************************/

STK_STATUS  Stk_ProtoType::AddView(Stk_View* i_StkViewP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = AddEntityToTopNode(NODE_TYPE_VIEW, (Stk_View*)i_StkViewP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	return eState;
}

/************************************************************************//**
 *	@brief		获取指定的View信息
 *	@param[in]	i_nuKeyPose		指定的View索引
 *	@param[out]	无
 *	@retval		StkViewP		View
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_View* Stk_ProtoType::GetView(STK_UINT32 i_nuKeyPose)
{
	Stk_View*			stkViewP = NULL;
	vector<Stk_Node*>	vcNodes;

	PreOrderNode(m_TopNodeP, NODE_TYPE_VIEW, vcNodes);
	stkViewP = (Stk_View*)(vcNodes.at(i_nuKeyPose)->GetView());

	return stkViewP;
}

/************************************************************************//**
 *	@brief		对象计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

void Stk_ProtoType::AddRef()
{
	m_idRefCount++;
}

/************************************************************************//**
 *	@brief		设置对象计数器
 *	@param[in]	i_nuRef		对象计数器
 *	@param[out]	无
 *	@retval		无
 *	@note		在读取ReadCADFile时，下级Proto没有创建，因此只能先记录下数目，在LoadFile时将计数器设置给ProtoType
 *	@attention	内部接口
 ****************************************************************************/

void Stk_ProtoType::SetRef(STK_UINT32 i_nuRef)
{
	m_idRefCount = i_nuRef;
}

/************************************************************************//**
 *	@brief		ProtoType内存释放
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

void Stk_ProtoType::Release()
{
	STK_STATUS				eState = STK_SUCCESS;
	Stk_File				*stkFileP = NULL;
	STK_UINT32				nuTopProtoTypeNum = 0;

	m_idRefCount--;
	if (m_idRefCount <= 0)
	{
		// 在删除ProtoType之前，需要判断是否删除File.
		// 如果File中还存在其他的ProtoType，则不删除File，而只删除ProtoType
		stkFileP = GetFile();
		if (stkFileP != NULL) //只转当前实例时，对于转换器创建的临时子ProtoType是没有Stk_File的
		{
			nuTopProtoTypeNum = stkFileP->GetTopProtoTypeNum();
			if (nuTopProtoTypeNum == 1)
			{
				//m_DocManager->DelSVLFileFromLoadList(stkFileP->GetSVLPath());
				m_DocManager->DelSVLFileFromLoadList(stkFileP->GetSVLSaveTmpPath());
				m_DocManager->RemoveFile(stkFileP);
			}
			// 增加状态判断，只有当一个File中的所有子ProtoType释放完后，才可释放TopProtpType。
			// 且顶级ProtoType可能是零件族的情况，所有还要增加nuTopProtoTypeNum == 1的判断
			eState = stkFileP->DelTopProtoTypeFromFile(this);
			if (eState == STK_SUCCESS)
			{
				nuTopProtoTypeNum = stkFileP->GetTopProtoTypeNum();
				if (nuTopProtoTypeNum == 0)
				{
					delete stkFileP;
					stkFileP = NULL;
				}
			}
			else
			{
				delete this;
			}
		}
		else
		{
			delete this;
		}
	}
}

/************************************************************************//**
 *	@brief		ProtoType子实例内存释放
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		1. 删除所有的子实例
 *				2. 释放掉子实例
 *				3. 子ProtoType引用计算器减1
 *	@attention	内部接口
 ****************************************************************************/

void Stk_ProtoType::DelAllChildren()
{
	STK_SIZE_T szInsCount;
	Stk_Instance* stkInsP = NULL;
	szInsCount = m_vcChildInss.size();

	for (int ii = 0; ii < szInsCount; ii++)
	{
		//InsID = m_vcChildInss[ii]->GetID();
		//PrototypeID = m_vcChildInss[ii]->GetProtoID();
		//InsCount = m_vcChildInss[ii]->GetProtoType()->GetRefCount() -1;
		//protoLink = m_vcChildInss[ii]->GetProtoLink();
		stkInsP = m_vcChildInss[ii];
		//if (stkInsP->GetFilePath() == L"" || stkInsP->GetFilePath() == L"Local")	// 暂时注释掉，至于之前为什么有此条件，原因不明
		//{
			delete m_vcChildInss[ii];
			m_bIsDirtyData = TRUE;
		//}
		
		// Updata By Gaoqq on 2013.6.28
		//STK_IDMANAGER->DelInstanceID(&InsID); 改到Instance析构中
		//当Instance:ProtoType = 1:1时，删除ProtoType
		//if(InsCount == 0){
			//STK_IDMANAGER->DelProtoTypeID(&PrototypeID); 改到ProtoType析构中
			//STK_IDMANAGER->DelProtoLinkString(protoLink); 改到ProtoType析构中
		//}
	}
}

/************************************************************************//**
 *	@brief		获取对象计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_idRefCount	对象计数器
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_ID Stk_ProtoType::GetRefCount()
{
	return m_idRefCount;
}

/************************************************************************//**
 *	@brief		添加PMI数据信息
 *	@param[in]	i_StkPMIP		PMI对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入PMI型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::AddPMI(Stk_PMI* i_StkPMIP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = AddEntityToTopNode(NODE_TYPE_PMI, (Stk_Entity*)i_StkPMIP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	m_bIsDirtyData = TRUE;
	return eState;
}

/************************************************************************//**
 *	@brief		获取PMI数据数组
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcPMIs		PMI数据数组
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_PMI*> Stk_ProtoType::GetPMI()
{
	vector<Stk_PMI*>	vcPMIs;
	vector<Stk_Node*>	vcNodes;

	PreOrderNode(m_TopNodeP, NODE_TYPE_PMI, vcNodes);
	for (int ii = 0; ii < vcNodes.size(); ii++)
	{
		vcPMIs.push_back((Stk_PMI*)((vcNodes.at(ii))->GetPMI()) );
	}

	return vcPMIs;
}

/************************************************************************//**
 *	@brief		根据ID删除指定PMI
 *	@param[in]	i_idPMI			指定的PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		删除成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::DelPMIByID(STK_ID i_idPMI)
{
	Stk_PMI *stkPMI = NULL;
	Stk_Node *stkNode = NULL;
	STK_STATUS ret;
	
	ret = m_ProtoIDManager->GetPMIByID(i_idPMI, stkPMI);
	if (ret!=STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (stkPMI && stkPMI->GetNode())
	{
		stkNode = stkPMI->GetNode();
		delete stkNode; //1.删除PMI 2.移除与父Node关联 3.删除子Node
		stkNode = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		释放所有的视图对象内存
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		释放成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS	Stk_ProtoType::RemoveAllView()
{
	Stk_View*			stkViewP = NULL;
	STK_UINT32			nuChildNodeNum = 0;
	Stk_Node*			stkNodeP = NULL;
	StkNodeTypeEnum		eNodeType = NODE_TYPE_UNKNOWN;
	STK_ID				nuPlcID;

	if (m_TopNodeP == NULL)
	{
		return STK_ERROR_NOMORE_NODE;
	}

	nuChildNodeNum = m_TopNodeP->GetChildNodeNum();
	for (int ii = 0; ii < nuChildNodeNum; ii++)
	{
		stkNodeP = m_TopNodeP->GetChildNode(ii,nuPlcID);
		if (stkNodeP == NULL)
		{
			return STK_ERROR;
		}
		eNodeType = stkNodeP->GetNodeType();
		if (eNodeType == NODE_TYPE_VIEW)
		{
			delete stkNodeP;
			stkNodeP = NULL;
		}
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		释放指定的视图对象内存
 *	@param[in]	i_idView		视图ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		释放成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS	Stk_ProtoType::DeleteView(STK_ID i_idView )
{
	Stk_View*			stkViewP = NULL;
	STK_ID				idView = 0;
	vector<Stk_Node*>	vcNodes;

	PreOrderNode(m_TopNodeP, NODE_TYPE_VIEW, vcNodes);
	for (int ii = 0; ii < vcNodes.size(); ii++)
	{
		stkViewP = (Stk_View*)(vcNodes[ii]->GetView());
		idView = stkViewP->GetID();
		if (idView == i_idView)
		{
			delete stkViewP;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置LOD等级数
 *	@param[in]	i_nuLODCount	LOD等级数
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_ProtoType::SetLODCount(STK_UINT32 i_nuLODCount)
{
	m_nuLODCount = i_nuLODCount;
}

/************************************************************************//**
 *	@brief		获取LOD等级数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuLODCount	LOD等级数
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_ProtoType::GetLODCount(void)
{
	return m_nuLODCount;
}

/*************************************************************************//**
 *	@brief		设置数据状态(是否是脏数据)
 *	@param[in]	i_bIsDirty	数据状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetDataStatus(STK_BOOL i_bIsDirty)
{
	m_bIsDirtyData = i_bIsDirty;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取当前ProtoType下的所有线集
 *	@param[in]	无
 *	@param[out]	o_vcLineSets	线集
 *	@retval		STK_SUCCESS		获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 
STK_STATUS Stk_ProtoType::GetAllLineSets(vector<Stk_LineSet *>& o_vcLineSets)
{
	Stk_Body*			stkBodyP = NULL;
	vector<STK_ID>		vcIDs;
	STK_ID				idNote = 0;
	STK_BOOL			bDelFlag = FALSE;
	vector<Stk_Node*>	vcNodes;
	vector<Stk_LineSet*> vcLineSets;

	o_vcLineSets.clear();

	PreOrderNode(m_TopNodeP, NODE_TYPE_BODY, vcNodes);
	for (int ii = 0; ii < vcNodes.size(); ii++)
	{
		stkBodyP = vcNodes[ii]->GetBody();
		if (stkBodyP == NULL)
		{
			return STK_ERROR;
		}
		vcLineSets = stkBodyP->GetAllLineSets();
		for (int jj = 0; jj < vcLineSets.size(); jj++)
		{
			o_vcLineSets.push_back(vcLineSets[jj]);
		}
	}

	return STK_SUCCESS;
}
/*************************************************************************//**
 *	@brief		获取当前ProtoType下的所有辅助线
 *	@param[in]	无
 *	@param[out]	o_vcSubLines	辅助线
 *	@retval		STK_SUCCESS		获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::GetAllSubLines(vector<Stk_CurveEntity *>& o_vcSubLines)
{
	Stk_Body*			stkBodyP = NULL;
	vector<STK_ID>		vcIDs;
	STK_ID				idNote = 0;
	STK_BOOL			bDelFlag = FALSE;
	vector<Stk_Node*>	vcNodes;
	vector<Stk_CurveEntity *> vcCurves;

	o_vcSubLines.clear();
	PreOrderNode(m_TopNodeP, NODE_TYPE_BODY, vcNodes);
	for (int ii = 0; ii < vcNodes.size(); ii++)
	{
		stkBodyP = vcNodes[ii]->GetBody();
		if (stkBodyP == NULL)
		{
			return STK_ERROR;
		}
		vcCurves = stkBodyP->GetAllSubLines();
		for (int jj = 0; jj < vcCurves.size(); jj++)
		{
			o_vcSubLines.push_back(vcCurves[jj]);
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		根据顶级ProtoType的存放路径获取ChildIns保存的路径
 *	@param[in]	i_wstrPath			顶级ProtoType的存放路径
 *	@param[out]	无
 *	@retval		wstrPath			ChildIns保存的路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::ConvertPathToDir(const wstring& i_wstrPath)
{
	wstring wstrPath = L"";
	wstring::size_type index;
	index = i_wstrPath.find_last_of (L"\\");
	if(index != -1)
	{
		wstrPath = i_wstrPath.substr(0, index + 1);
	}
	return wstrPath;
}

/************************************************************************//**
 *	@brief		根据上级ProtoType的加载路径和Instance的保存路径获取ChildIns的加载路径
 *	@param[in]	i_wstrInsSavePath			Ins的保存路径
 *	@param[in]	i_wstrPaProtoLoadPath		上级ProtoType的加载路径
 *	@param[out]	无
 *	@retval		wstrPath					ChildIns的加载路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::ConvertChildInsLoadPath(const wstring& i_wstrInsSavePath, const wstring& i_wstrPaProtoLoadPath)
{
	wstring wstrInsName = L"";
	wstring wstrPath = L"";
	wstring::size_type indexSave, indexLoad;
	indexSave = i_wstrInsSavePath.find_last_of(L"\\");

	///查找linux路径
	if(indexSave == -1)
	{
		indexSave = i_wstrPaProtoLoadPath.find_last_of(L"\/");
	}

	if (indexSave != -1)
	{
		wstrInsName = i_wstrInsSavePath.substr(indexSave + 1, i_wstrInsSavePath.size() - indexSave -1);
	}

	indexLoad = i_wstrPaProtoLoadPath.find_last_of(L"\\");

	///查找linux路径
	if(indexLoad == -1)
	{
		indexLoad = i_wstrPaProtoLoadPath.find_last_of(L"\/");
	}

	if(indexLoad != -1)
	{
		wstrPath = i_wstrPaProtoLoadPath.substr(0, indexLoad + 1);
	}
	wstrPath += wstrInsName;
	return wstrPath;
}

/************************************************************************//**
 *	@brief		判断是否是脏数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsDirtyData				是否脏数据
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsDirtyData()
{
	return m_bIsDirtyData;
}

/************************************************************************//**
 *	@brief		判断是否是Node模式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsNodeMode				Node模式标识
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsNodeMode()
{
	return m_bIsNodeMode;
}

/************************************************************************//**
 *	@brief		设置是否是Node模式
 *	@param[in]	i_bIsNodeMode				是否是Node模式标识
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
void Stk_ProtoType::SetNodeMode(STK_BOOL i_bIsNodeMode)
{
	m_bIsNodeMode = i_bIsNodeMode;
}

/************************************************************************//**
 *	@brief		设置顶级的Node节点
 *	@param[in]	i_stkNodeP				Node节点
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		STK_ERROR				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::SetTopNode(Stk_Node* i_stkNodeP)
{
	if (i_stkNodeP != NULL)
	{
		m_TopNodeP = i_stkNodeP;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取顶级Node节点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_TopNodeP				顶级Node节点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Node* Stk_ProtoType::GetTopNode()
{
	return m_TopNodeP;
}

/************************************************************************//**
 *	@brief		移除指定实例的装配关系
 *	@param[in]	i_stkInsP		实例对象
 *	@param[out]	无
 *	@retval		STK_SUCCESS		移除成功
 *	@note		解除配置关系，不释放Instance内存，不减引用计数
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::RemoveChild(Stk_Instance* i_stkInsP)
{
	STK_ID idPlc = i_stkInsP->GetPlcID();
	for (vector<Stk_Instance*>::iterator itor = m_vcChildInss.begin(); itor != m_vcChildInss.end(); itor ++)
	{
		if (*itor == i_stkInsP)
		{
			m_vcChildInss.erase(itor);
			break;
		}
	}
	m_mapChildInss.erase(idPlc);
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		往当前ProtoType中添加自定义属性数据
 *	@param[in]	i_MetaDataP		自定义属性数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::AddMetaData(Stk_MetaData* i_MetaDataP)
{
	if (i_MetaDataP != NULL)
	{
		m_vcMetaDatas.push_back(i_MetaDataP);
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取当前ProtoType中所有的自定义属性数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas	自定义属性数据数组
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_MetaData*>& Stk_ProtoType::GetAllMetaDatas()
{
	return m_vcMetaDatas;
}

/*************************************************************************//**
 *	@brief		获取当前ProtoType中所有的自定义属性数据数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_ProtoType::GetMetaDataNum()
{
	return m_vcMetaDatas.size();
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
STK_STATUS Stk_ProtoType::DelMetaDataByTitle(wstring i_wstrMetaDataTitle)
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

/************************************************************************//**
 *	@brief		通过自定义属性名获取指定对象
 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
 *	@param[out]	无
 *	@retval		自定义属性对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_MetaData* Stk_ProtoType::GetMetaDataByTitle(wstring i_wstrMetaDataTitle)
{
	wstring			wstrTitle = L"";
	Stk_MetaData*	stkMetaDataP = NULL;
	vector<Stk_MetaData*>::iterator itor = m_vcMetaDatas.begin();
	while(itor != m_vcMetaDatas.end())
	{
		wstrTitle = (*itor)->GetTitle();
		if (wstrTitle == i_wstrMetaDataTitle)
		{
			stkMetaDataP = *itor;
			break;
		}
		itor++;
	}

	return stkMetaDataP;
}

/*************************************************************************//**
 *	@brief		从SVL文件中读取自定义属性数据
 *	@param[in]	i_FileP			SVL文件指针
 *	@param[in]	i_PropHeadP		自定义属性记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoType::ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP)
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
		if ((RecordPP[ii]->type == REC_TYPE_METADATA))
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				stkMetaDataP = new Stk_MetaData(this);
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
 *	@brief		从SVL文件中读取用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[in]	i_UserHeadP		自定义属性记录头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::ReadUserDatas(Stk_File *i_FileP, StkSegmentHead *i_UserHeadP)
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
				stkUserDataP = new Stk_UserData(this);
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
				stkUserDataP = new Stk_UserData(this);
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

/*************************************************************************//**
 *	@brief		往SVL文件中填写自定义属性数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoType::WriteMetaData(Stk_File* i_stkFileP)
{
	StkSegmentHead		metaDataHead = {0};
	StkMetaDataRec		metaDataRec = {0};
	STK_INT32			ret = 0;
	STK_UINT32			nuMetaDataNum = 0;
	STK_UINT32			ii;
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
			_T("【MetaData段】输出 MetaData"));
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	nuMetaDataNum = GetMetaDataNum();
	if (nuMetaDataNum <= 0)
	{
		return STK_SUCCESS;
	}

	// 【第1步】 输出段头记录
	metaDataHead.id = 0;
	metaDataHead.kind = SEG_TYPE_METADATA;
	ret = i_stkFileP->WriteSegmentHeadData(&metaDataHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPrototype && 1 == stkSVLLog.bMetaData)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MetaData段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
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
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPrototype && 1 == stkSVLLog.bMetaData)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MetaData段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		添加配置路径与显隐状态之间的映射
 *	@param[in]	i_wstrDisplayPath		配置路径
 *	@param[in]	i_eDisplayState			显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		为适应UG场景而添加
 *	@attention	无
 ****************************************************************************/

//STK_STATUS Stk_ProtoType::AddDisplayState(wstring i_wstrDisplayPath, StkDisplayStateEnum i_eDisplayState)
//{
//	m_mapDisplayState.insert(pair<wstring, StkDisplayStateEnum>(i_wstrDisplayPath, i_eDisplayState));
//
//	return STK_SUCCESS;
//}

/*************************************************************************//**
 *	@brief		获取配置路径与显隐状态之间的映射
 *	@param[in]	i_wstrPlcPath			实例配置路径
 *	@param[out]	o_eDisplayState			显隐状态
 *	@retval		STK_SUCCESS				获取成功
 *	@retval		STK_ERROR				获取失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//STK_STATUS Stk_ProtoType::GetDisplayStateByPlcPath(wstring i_wstrPlcPath, StkDisplayStateEnum& o_eDisplayState)
//{
//	map<wstring, StkDisplayStateEnum>::iterator itor = m_mapDisplayState.find(i_wstrPlcPath);
//	if (itor != m_mapDisplayState.end())
//	{
//		o_eDisplayState = itor->second;
//		return STK_SUCCESS;
//	}
//	else
//	{
//		o_eDisplayState = STK_DISPLAY;
//		return STK_ERROR;
//	}
//}

/*************************************************************************//**
 *	@brief		根据CAD文件路径注册SVL
 *	@param[in]	i_wstrCADPath			CAD物理文件路径
 *	@param[out]	o_stkCADFileP			File文件对象指针
 *	@retval		STK_SUCCESS				注册成功
 *	@note		适用于转换器
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::RegCADFile(wstring i_wstrCADPath, Stk_File*& o_stkCADFileP)
{
	// 这个接口需实现的功能：
	// 1. 创建Stk_File
	// 2. 为ProtoType指向File
	// 3. 为File设置顶级ProtoType
	// 4. 为File设置CADPath、CADPathName
	// 5. 为DocManager设置File

	STK_STATUS		state = STK_SUCCESS;
	STK_ID			idCADFile = 0;

	Stk_GlobalIDManager* stkGlobalIDManagerP = m_DocManager->GetGlobalIDManger();

	state = stkGlobalIDManagerP->RegCADFilePath(i_wstrCADPath, idCADFile);
	if (state == STK_SUCCESS_EXIST)
	{
		state = stkGlobalIDManagerP->GetFileByID(idCADFile, o_stkCADFileP);
		if (state != STK_SUCCESS )
		{
			return STK_ERROR;
		}
	}
	else if (m_isCADFileTop)
	{
		// 1. 创建Stk_File
		o_stkCADFileP = new Stk_File(m_DocManager);

		// 4. 为File设置CADPath、CADPathName
		o_stkCADFileP->SetCADFilePath(i_wstrCADPath);
		o_stkCADFileP->RegisterID(STK_NEW_ID);

		// 5. 为DocManager设置File
		m_DocManager->AddFile(o_stkCADFileP);
	}

	// 3. 为File设置顶级ProtoType
	if (m_isCADFileTop)
	{
		o_stkCADFileP->AddTopProtoType(this);
	}

	// 2. 为ProtoType指向File
	this->SetFile(o_stkCADFileP);

	return STK_SUCCESS;
}


/************************************************************************//**
 *	@brief		读取文件中的ProtoType数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_ProtoHeadP		ProtoType记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR			读取失败
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		V2.2及以后使用此接口读SVL文件数据
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoType::ReadCADFile(Stk_File *i_FileP, StkSegmentHead *i_ProtoHeadP)
{
	StkSegmentHead	head ={0};
	StkControl		**RecordPP = NULL;
	STK_UINT32		nuElemCount = 0;
	STK_UINT32		ii,jj;
	STK_ID			idFileProto;
	STK_MTX32		mtxPlacement;
	STK_ID			idPlacement;
	STK_INT32		nNumber = 0;
	Stk_Instance	*CurInsP = NULL;
	wstring			wstrCurLink;
	wstring			wstrInsName;
	wstring			wstrChildProtoName = L"";
	wstring			wstrChildInsSavePath = L"";
	WCHAR			wchIDA[32];
	wstring			wstrPlcPath = L"";
	STK_UINT32		nuStrIdx;
	STK_FLOAT32		fInsColorA[4]={0};
	STK_UINT32		nuHasItem = 0;
	Stk_Body		*StkBodyP = NULL; 
	Stk_PMI			*StkPMIP = NULL;
	Stk_View		*StkViewP = NULL;
	Stk_MetaData*	stkMetaDataP = NULL;
	STK_STATUS		state;
	wstring			wstrLoadPath = L"";
	wstring			wstrChildProtoLink = L"";
	wstring			wstrPaProtoLoadPath = L"";
	STK_BOOL		flag = false;
	STK_UINT32		nuDisplayState = 0;
	STK_STATUS		eState;
	STK_ID			idTopNode = 1;
	STK_UINT32		nuChildProtoType = StkProtoTypeEnum::PROTO_TYPE_UNKNOWN;
	string			strSVLPath = "";
	STK_BOOL		bIsOldVersion = false;
	Stk_GroupFeature*		stkGroupFeatP = NULL;
	Stk_PatternFeature*		stkPatternFeatP = NULL;
	Stk_AssemblyFeature*	stkAssemblyFeatP = NULL;
	vector<Stk_Body*>		vcBodys;
	vector<Stk_View*>		vcViews;
	wstring			wstrGUID = L"";
	wstring			wstrPassword = L"";
	wstring			wstrDePassword = L"";
	wstring			wstrVirtualBomID = L"";
	Stk_Render*		stkRenderP = NULL;

	// 版本标识
	bIsOldVersion = m_stkFileP->IsOldVersion();
	if (bIsOldVersion)
	{
		m_TopNodeP = new Stk_Node(m_DocManager, this);
		//m_TopNodeP->RegisterID(STK_NEW_ID); //为了解决V2.162前PMI的ID是1与顶级NodeID冲突的特殊情况，仅当读取旧文件时调整顶级Node注册ID的时机至最后
		m_TopNodeP->SetNodeType(NODE_TYPE_VIRTUAL);
		m_TopNodeP->AddRef();
		SetNodeMode(true);
	}

	/*
	 * 读取ProtoType段的数据
	 */
	RecordPP = (StkControl **)calloc(i_ProtoHeadP->recnum, sizeof(StkControl*));
	if (i_ProtoHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_ProtoHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取ProtoType段的各记录
	 */
	while (1)
	{
		eState = i_FileP->ReadSegmentHeadData( &head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		switch (head.kind) 
		{
		case SEG_TYPE_Node:
			SetNodeMode(true);
#if 0
			wstrCurLink.erase();
			//_itow_s( idFileProto, wchIDA, 10);
			swprintf(wchIDA, 32, L"%d", idTopNode); //支持Andriod
			wstrCurLink += wchIDA;
			wstrCurLink += L"|";
			swprintf(wchIDA, 32, L"%d", m_ID); //支持Andriod
			wstrCurLink += wchIDA;
			wstrCurLink += L"|";
			wstrCurLink += i_FileP->GetCADFilePath();

			// 设置顶级Node
			m_TopNodeP = new Stk_Node(m_DocManager,this);
			m_TopNodeP->SetLinkString(wstrCurLink);
			
			eState = m_TopNodeP->RegNodeLink();
			if (eState == STK_SUCCESS_EXIST)
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				break;
			}
			if (eState == STK_ERROR  )
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				return eState;
			}
			m_TopNodeP->RegisterID(head.id);
#else
			m_TopNodeP = new Stk_Node(m_DocManager,this);
			eState = m_TopNodeP->RegisterID(head.id);
			if (eState == STK_SUCCESS_EXIST)
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				break;
			}
			if (eState == STK_ERROR  )
			{
				if (m_TopNodeP != NULL)
				{
					delete m_TopNodeP;
					m_TopNodeP = NULL;
				}
				return eState;
			}
			m_TopNodeP->AddRef();

#endif
			eState = m_TopNodeP->ReadFile(i_FileP,&head);
			if (eState!=STK_SUCCESS)
			{
				//delete m_TopNodeP;
				//return eState;
				STK_STATUS eStateTmp = m_TopNodeP->Release();
				if ( eStateTmp == STK_SUCCESS)
				{
					m_TopNodeP = NULL;
				}
				return eState;
			}

			// 如果不从二进制中读取PMI
			if(m_DocManager->GetPMIViewReadFlag() && m_TopNodeP->GetNodeType() == NODE_TYPE_PMI)
			{
				eState = m_TopNodeP->Release();
				if ( eState == STK_SUCCESS)
				{
					m_TopNodeP = NULL;
				}
			}
			break;
		case SEG_TYPE_BODY:
			StkBodyP = new Stk_Body(this);
			eState = StkBodyP->ReadFile(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete StkBodyP;
				return eState;
			}
			// @@@
			//ConvertToNode(i_FileP, NODE_TYPE_BODY, (Stk_Entity*)StkBodyP);
			//AddBody(StkBodyP);//转换为Node插入RootNode下
			vcBodys.push_back(StkBodyP);
			break;

		case SEG_TYPE_PMI:
			if(m_DocManager->GetPMIViewReadFlag())
			{
				StkPMIP = new Stk_PMI(this);
				eState = StkPMIP->ReadFile(i_FileP, &head);
				if (eState!=STK_SUCCESS)
				{
					delete StkPMIP;
					return eState;
				}
				//ConvertToNode(i_FileP, NODE_TYPE_PMI, (Stk_Entity*)StkBodyP);
				ConvertPMI(StkPMIP);//转换为Node插入RootNode下
				break;
			}
			else
			{
				StkControl **TmpRecordPP = NULL;
				TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
				if (head.recnum !=0 && TmpRecordPP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
				if (eState!=STK_SUCCESS)
				{
					return eState;
				}
				eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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

		case SEG_TYPE_VIEW:
			if(m_DocManager->GetPMIViewReadFlag())
			{
				StkViewP = new Stk_View(this);
				eState = StkViewP->ReadFile(i_FileP, &head );
				if (eState!=STK_SUCCESS)
				{
					delete StkViewP;
					return eState;
				}
				//ConvertToNode(i_FileP, NODE_TYPE_VIEW, (Stk_Entity*)StkViewP);
				//AddView(StkViewP);//转换为Node插入RootNode下
				vcViews.push_back(StkViewP);
				break;
			}
			else
			{
				StkControl **TmpRecordPP = NULL;
				TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
				if (head.recnum !=0 && TmpRecordPP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
				if (eState!=STK_SUCCESS)
				{
					return eState;
				}
				eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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

		case SEG_TYPE_METADATA:
			eState = ReadMetaDatas(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			//ConvertToNode(i_FileP, NODE_TYPE_META, (Stk_Entity*)); //ProtoType的SEG_TYPE_METADATA不属于Node，无须转换
			break;

		case  SEG_TYPE_USERDEFDATA:
			eState = ReadUserDatas(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			break;

		case SEG_TYPE_COMP_GROUPFEAT:
			stkGroupFeatP = new Stk_GroupFeature(m_DocManager, this);
			eState = stkGroupFeatP->ReadFile(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkGroupFeatP;
				return eState;
			}
			AddComponentFeature(stkGroupFeatP);
			break;

		case SEG_TYPE_COMP_PATTERN:
			stkPatternFeatP = new Stk_PatternFeature(m_DocManager, this);
			eState = stkPatternFeatP->ReadFile(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkPatternFeatP;
				return eState;
			}
			AddComponentFeature(stkPatternFeatP);
			break;

		case SEG_TYPE_COMP_ASSEMBLYFEAT:
			stkAssemblyFeatP = new Stk_AssemblyFeature(m_DocManager, this);
			eState = stkAssemblyFeatP->ReadFile(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkAssemblyFeatP;
				return eState;
			}
			AddComponentFeature(stkAssemblyFeatP);
			break;

		case SEG_TYPE_INSATTRIBUTE:
			eState = ReadInsAttribute(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			break;

		case SEG_TYPE_RENDER:
			stkRenderP = new Stk_Render(this);
			eState = stkRenderP->ReadFile(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			AddRender(stkRenderP);
			break;
		// 当下一段是ProtoType-END表时
		case SEG_TYPE_PROTO_END:
			goto pt1;
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
				return eState;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	// 填写V2.2版本之前的Body、View数据
	for (int ii = 0; ii < vcBodys.size(); ii++)
	{
		if (vcBodys[ii] != NULL)
		{
			ConvertBody(vcBodys[ii]);
		}
	}
	for (int jj = 0; jj < vcViews.size(); jj++)
	{
		if (vcViews[jj] != NULL)
		{
			ConvertView(vcViews[jj]);
		}
	}
	if (bIsOldVersion)
	{
		m_TopNodeP->RegisterID(STK_NEW_ID);
	}

	/*
	 *  填写ProtoType类的子实例
	 */
	
	//  根据ProtoType段的内容，
	//  创建子实例并设定实例状态
	for (ii = 0; ii < i_ProtoHeadP->recnum; ii++)
	{
		// 创建子实例
		if (RecordPP[ii]->type == REC_TYPE_PROTO)
		{
			// version 1 --- version 5，是为兼容V2.2版本之前的SVL而添加
			if (RecordPP[ii]->version == 1)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType*)RecordPP[ii])->name);
#else
				m_wstrName = ((StkProtoType*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType*)RecordPP[ii])->childnum;
				if (nuElemCount > 0)
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_ASSY;
				}
				else
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_PART;
				}
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType*)RecordPP[ii])->childdat[jj].childId;
					memcpy(&mtxPlacement, ((StkProtoType*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16 );
					idPlacement = ((StkProtoType*)RecordPP[ii])->childdat[jj].plcId;

#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					//wstrCurLink += curFilePath; //### 多Document时需要改
					wstrCurLink += i_FileP->GetSVLPath();

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(i_FileP->GetSVLPath());
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(i_FileP->GetSVLPath());
					CurInsP->SetProtoLink(wstrCurLink);
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName(wstrInsName); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement(idPlacement, mtxPlacement);
					m_vcChildInss.push_back(CurInsP); //打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement, m_vcChildInss.size()-1));
				}
			}
			else if (RecordPP[ii]->version == 2)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName =Stk_Platform::STKWCHARStrToWString(((StkProtoType2*)RecordPP[ii])->name);
#else
				m_wstrName = ((StkProtoType2*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType2*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType2*)RecordPP[ii])->childnum;
				if (nuElemCount > 0)
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_ASSY;
				}
				else
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_PART;
				}
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{

					idFileProto = ((StkProtoType2*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType2*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType2*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType2*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType2*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";
					//wstrCurLink += curFilePath; //### 多Document时需要改
					wstrCurLink += i_FileP->GetSVLPath();

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(i_FileP->GetSVLPath());
					if (access(strSVLPath.c_str(), 0) == -1)//@@@ Android 可以？
					{
						continue;
					}

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(i_FileP->GetSVLPath());
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP); //打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size()-1));
				}
			}
			else if (RecordPP[ii]->version == 3)
			{
				flag = true;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->name);				
#else
				m_wstrName = ((StkProtoType3*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType3*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType3*)RecordPP[ii])->childnum;
				if (nuElemCount > 0)
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_ASSY;
				}
				else
				{
					m_eProtoType = StkProtoTypeEnum::PROTO_TYPE_PART;
				}
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType3*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType3*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType3*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType3*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetPath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType3*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType3*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1)//@@@ Android 可以？
					{
						continue;
					}

					if (wstrLoadPath == wstrPaProtoLoadPath)		// 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
// 					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
// 					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
// 					}
// 					else
// 					{
// 						CurInsP->SetFilePath(L"Local");
// 					}
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP); //打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size()-1));
				}
			}
			else if (RecordPP[ii]->version == 4)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->name);	
#else
				m_wstrName = ((StkProtoType4*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType4*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType4*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType4*)RecordPP[ii])->types;
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType4*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType4*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType4*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType4*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetPath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType4*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType4*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1)//@@@ Android 可以？
					{
						continue;
					}

					if (wstrLoadPath == wstrPaProtoLoadPath) // 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//_itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
// 					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
// 					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
// 					}
// 					else
// 					{
// 						CurInsP->SetFilePath(L"Local");
// 					}
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP); //打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size()-1));
				}
			}
			else if (RecordPP[ii]->version == 5)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->name);	
#else
				m_wstrName = ((StkProtoType5*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType5*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType5*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType5*)RecordPP[ii])->types;
				
				if( ((StkProtoType5*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}

				if (((StkProtoType5*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType5*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType5*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType5*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType5*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetCADFilePath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType5*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType5*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					if (wstrLoadPath == wstrPaProtoLoadPath) // 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//_itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
// 					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
// 					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
// 					}
// 					else
// 					{
// 						CurInsP->SetFilePath(L"Local");
//					}
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size()-1));
				}
			}
			// 版本6
			else if ( RecordPP[ii]->version == 6)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType6*)RecordPP[ii])->name);	
#else
				m_wstrName = ((StkProtoType6*)RecordPP[ii])->name;
#endif
				m_nuLODCount = ((StkProtoType6*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType6*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType6*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType6*)RecordPP[ii])->UnitFactor; //单位

				if( ((StkProtoType6*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}

				if (((StkProtoType6*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				m_isCADFileTop = true;

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType6*)RecordPP[ii])->childdat[jj].childId;
					memcpy( &mtxPlacement, ((StkProtoType6*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType6*)RecordPP[ii])->childdat[jj].plcId;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType6*)RecordPP[ii])->childdat[jj].InsName);
#else
					wstrInsName = ((StkProtoType6*)RecordPP[ii])->childdat[jj].InsName;
#endif
					wstrCurLink.erase();
					/*_itow_s( idFileProto, wchIDA, 10);
					wstrCurLink += wchIDA;
					wstrCurLink += L"|";*/
					//wstrCurLink += curFilePath; //### 多Document时需要改
					//wstrCurLink += i_FileP->GetPath();
#ifdef __MOBILE__
					wstrChildInsSavePath = Stk_Platform::STKWCHARStrToWString(((StkProtoType6*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildInsSavePath = ((StkProtoType6*)RecordPP[ii])->childdat[jj].filePath;
#endif
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
					if(wstrChildInsSavePath != L"" && wstrChildInsSavePath != L"Local")
					{
						wstrLoadPath = ConvertChildInsLoadPath(wstrChildInsSavePath, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}
					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1)//@@@ Android 可以？
					{
						continue;
					}

					if (wstrLoadPath == wstrPaProtoLoadPath)		// 当为单一文件时ProtoLink为“ID+路径”，为分散文件时ProtoLink为“路径”
					{
						//itow_s( idFileProto, wchIDA, 10);
						swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
						wstrCurLink += wchIDA;
						wstrCurLink += L"|";
					}

					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
// 					if(wstrChildInsSavePath != L"Local" && wstrChildInsSavePath != L"")
// 					{
						CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
// 					}
// 					else
// 					{
// 						CurInsP->SetFilePath(L"Local");
// 					}
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本7
			else if ( RecordPP[ii]->version == 7)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType7*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType7*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType7*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType7*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType7*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType7*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType7*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType7*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType7*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType7*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType7*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType7*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType7*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType7*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType7*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType7*)RecordPP[ii])->childdat[jj].plcId; 
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType7*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType7*)RecordPP[ii])->childdat[jj].ProtoName);
#else
					wstrInsName = ((StkProtoType7*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType7*)RecordPP[ii])->childdat[jj].ProtoName;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType7*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType7*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}
					
					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式
					
					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
							nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本8
			else if ( RecordPP[ii]->version == 8)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType8*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType8*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType8*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType8*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType8*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType8*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType8*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType8*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType8*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType8*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType8*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType8*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}
				if (((StkProtoType8*)RecordPP[ii])->IsItem & STK_PROTO_IS_USE_NUMBER)
				{
					m_bIsUseNumber = true;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType8*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType8*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType8*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType8*)RecordPP[ii])->childdat[jj].plcId; 
					nNumber = ((StkProtoType8*)RecordPP[ii])->childdat[jj].nNumber;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType8*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType8*)RecordPP[ii])->childdat[jj].ProtoName);
#else
					wstrInsName = ((StkProtoType8*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType8*)RecordPP[ii])->childdat[jj].ProtoName;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType8*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType8*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式

					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
						nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					CurInsP->SetNumber(nNumber);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本9
			else if ( RecordPP[ii]->version == 9)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType9*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType9*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType9*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType9*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType9*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType9*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType9*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType9*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType9*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}
				if (((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_IS_USE_NUMBER)
				{
					m_bIsUseNumber = true;
				}
				if (((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_HAS_COLOR)
				{
					m_bHasColor = true;
					m_rgbaProtoType = ((StkProtoType9*)RecordPP[ii])->Color;
				}
				if (!(((StkProtoType9*)RecordPP[ii])->IsItem & STK_PROTO_IS_DISPLAY))
				{
					m_bDisplay = false;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType9*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType9*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType9*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType9*)RecordPP[ii])->childdat[jj].plcId; 
					nNumber = ((StkProtoType9*)RecordPP[ii])->childdat[jj].nNumber;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType9*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType9*)RecordPP[ii])->childdat[jj].ProtoName);
#else
					wstrInsName = ((StkProtoType9*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType9*)RecordPP[ii])->childdat[jj].ProtoName;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType9*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType9*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式

					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
						nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					CurInsP->SetNumber(nNumber);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			// 版本10
			else if ( RecordPP[ii]->version == 10)
			{
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType10*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType10*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType10*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType10*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType10*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType10*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType10*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}
				if (((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_IS_USE_NUMBER)
				{
					m_bIsUseNumber = true;
				}
				if (((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_HAS_COLOR)
				{
					m_bHasColor = true;
					m_rgbaProtoType = ((StkProtoType10*)RecordPP[ii])->Color;
				}
				if (!(((StkProtoType10*)RecordPP[ii])->IsItem & STK_PROTO_IS_DISPLAY))
				{
					m_bDisplay = false;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType10*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType10*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType10*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType10*)RecordPP[ii])->childdat[jj].plcId; 
					nNumber = ((StkProtoType10*)RecordPP[ii])->childdat[jj].nNumber;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->childdat[jj].ProtoName);
					wstrGUID = Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->childdat[jj].wchGUID);
#else
					wstrInsName = ((StkProtoType10*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType10*)RecordPP[ii])->childdat[jj].ProtoName;
					wstrGUID = ((StkProtoType10*)RecordPP[ii])->childdat[jj].wchGUID;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType10*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType10*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式

					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
						nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					CurInsP->SetNumber(nNumber);
					CurInsP->SetGUID(wstrGUID);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
				}
			}
			else if (RecordPP[ii]->version == 11)
			{
				// 待解密逻辑
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType11*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType11*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType11*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType11*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType11*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType11*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType11*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}
				if (((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_IS_USE_NUMBER)
				{
					m_bIsUseNumber = true;
				}
				if (((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_HAS_COLOR)
				{
					m_bHasColor = true;
					m_rgbaProtoType = ((StkProtoType11*)RecordPP[ii])->Color;
				}
				if (!(((StkProtoType11*)RecordPP[ii])->IsItem & STK_PROTO_IS_DISPLAY))
				{
					m_bDisplay = false;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType11*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType11*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType11*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType11*)RecordPP[ii])->childdat[jj].plcId; 
					nNumber = ((StkProtoType11*)RecordPP[ii])->childdat[jj].nNumber;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->childdat[jj].ProtoName);
					wstrGUID = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->childdat[jj].wchGUID);
					wstrPassword = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->childdat[jj].Password);
#else
					wstrInsName = ((StkProtoType11*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType11*)RecordPP[ii])->childdat[jj].ProtoName;
					wstrGUID = ((StkProtoType11*)RecordPP[ii])->childdat[jj].wchGUID;
					wstrPassword = ((StkProtoType11*)RecordPP[ii])->childdat[jj].Password;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType11*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType11*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式

					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
						nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					CurInsP->SetNumber(nNumber);
					CurInsP->SetGUID(wstrGUID);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
					if (wstrPassword != L"")
					{
						m_DocManager->DePassword(wstrPassword, wstrDePassword);
						m_DocManager->SetUserDePassword(i_FileP->GetSVLPath(), wstrDePassword);
					}
				}
			}
			else if (RecordPP[ii]->version == 12)
			{
				// 待解密逻辑
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->name);	
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->CADFilePath);
#else
				m_wstrName = ((StkProtoType12*)RecordPP[ii])->name;
				m_wstrCADPath = ((StkProtoType12*)RecordPP[ii])->CADFilePath;
#endif
				m_wstrCADFileName = GetFile()->ComputeCADFileName(m_wstrCADPath);
				m_nuLODCount = ((StkProtoType12*)RecordPP[ii])->LODnum;
				nuElemCount = ((StkProtoType12*)RecordPP[ii])->childnum;
				m_eProtoType = (StkProtoTypeEnum)((StkProtoType12*)RecordPP[ii])->types;
				m_dUnitFactor =  ((StkProtoType12*)RecordPP[ii])->UnitFactor; //单位

				if ((((StkProtoType12*)RecordPP[ii])->IsItem) & STK_PROTO_IS_NODE_MODE)
				{
					SetNodeMode(true);
				}
				if( ((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_IS_CATALOG)
				{
					m_isCatalog = true;
				}
				if (((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_IS_PIPE)
				{
					m_bPipe = true;
				}
				if(((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_IS_TOP)
				{
					m_isCADFileTop = true;
				}
				if (((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_IS_USE_NUMBER)
				{
					m_bIsUseNumber = true;
				}
				if (((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_HAS_COLOR)
				{
					m_bHasColor = true;
					m_rgbaProtoType = ((StkProtoType12*)RecordPP[ii])->Color;
				}
				if (!(((StkProtoType12*)RecordPP[ii])->IsItem & STK_PROTO_IS_DISPLAY))
				{
					m_bDisplay = false;
				}

				for (jj=0; jj<nuElemCount; jj++)
				{
					idFileProto = ((StkProtoType12*)RecordPP[ii])->childdat[jj].childId;
					nuChildProtoType = ((StkProtoType12*)RecordPP[ii])->childdat[jj].nuType;
					memcpy( &mtxPlacement, ((StkProtoType12*)RecordPP[ii])->childdat[jj].Matrix, sizeof(FLOAT)*16);
					idPlacement = ((StkProtoType12*)RecordPP[ii])->childdat[jj].plcId; 
					nNumber = ((StkProtoType12*)RecordPP[ii])->childdat[jj].nNumber;
#ifdef __MOBILE__
					wstrInsName =Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].InsName);
					wstrChildProtoName = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].ProtoName);
					wstrGUID = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].wchGUID);
					wstrPassword = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].Password);
					wstrVirtualBomID = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].wchBomID);
#else
					wstrInsName = ((StkProtoType12*)RecordPP[ii])->childdat[jj].InsName;
					wstrChildProtoName = ((StkProtoType12*)RecordPP[ii])->childdat[jj].ProtoName;
					wstrGUID = ((StkProtoType12*)RecordPP[ii])->childdat[jj].wchGUID;
					wstrPassword = ((StkProtoType12*)RecordPP[ii])->childdat[jj].Password;
					wstrVirtualBomID = ((StkProtoType12*)RecordPP[ii])->childdat[jj].wchBomID;
#endif
					wstrCurLink.erase();
#ifdef __MOBILE__
					wstrChildProtoLink = Stk_Platform::STKWCHARStrToWString(((StkProtoType12*)RecordPP[ii])->childdat[jj].filePath);
#else
					wstrChildProtoLink = ((StkProtoType12*)RecordPP[ii])->childdat[jj].filePath;
#endif
					// 如果子实例路径为空，则不再创建子实例
					if (wstrChildProtoLink == L"")
					{
						continue;
					}

					// 调整路径
					wstrPaProtoLoadPath = i_FileP->GetSVLPath();
#ifdef __MOBILE__
					if(wstrChildProtoLink.find(L"|Local") == std::wstring::npos)
#else
					if(wstrChildProtoLink.rfind(L"|Local") == std::wstring::npos)
#endif
					{
						wstrLoadPath = AdjustChildInsLoadPath(wstrChildProtoLink, wstrPaProtoLoadPath);
					}
					else
					{
						wstrLoadPath = wstrPaProtoLoadPath; //单一时使用父Path
					}

					// 检测SVL文件是否存在，如果不存在，则跳出
					strSVLPath = Stk_Platform::WStringToString(wstrLoadPath);
					if (access(strSVLPath.c_str(), 0) == -1) //@@@ Android 可以？
					{
						continue;
					}

					//_itow_s( idFileProto, wchIDA, 10);
					swprintf(wchIDA, 32, L"%d", idFileProto); //支持Andriod
					//wstrCurLink += wchIDA;
					wstrCurLink += wstrChildProtoName;	// 更改为ProtoName.后缀|路径  的形式

					if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
					{
						wstrCurLink += L"*ASM";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
						nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
					{
						wstrCurLink += L"*PRT";
					}
					else if (nuChildProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
					{
						wstrCurLink += L"*SHT";
					}
					wstrCurLink += L"|";
					wstrCurLink += wstrLoadPath;

					// 创建实例
					CurInsP = new Stk_Instance(m_DocManager);
					CurInsP->SetFilePath(wstrLoadPath);	// add by Shao on 2014/01/09
					CurInsP->SetProtoLink( wstrCurLink );
					CurInsP->RegisterID(STK_NEW_ID);
					CurInsP->SetInsName( wstrInsName ); //add by Gaoqq on 2013.5.31
					CurInsP->SetPlacement( idPlacement, mtxPlacement);
					CurInsP->SetNumber(nNumber);
					CurInsP->SetGUID(wstrGUID);
					CurInsP->SetVirtualBOMID(wstrVirtualBomID);
					m_vcChildInss.push_back( CurInsP);//打开SVL时通过AddProtoRef为ProtoType计数，不能用AddChild接口替代
					m_mapChildInss.insert(pair<STK_ID,STK_UINT32>(idPlacement,m_vcChildInss.size() - 1));
					if (wstrPassword != L"")
					{
						m_DocManager->DePassword(wstrPassword, wstrDePassword);
						m_DocManager->SetUserDePassword(i_FileP->GetSVLPath(), wstrDePassword);
					}
				}
			}
			else
			{
				//错误
			}
		}

		// 读取PROTO TITLE信息	Added by Shaoqy on 2014/1/6
		else if (RecordPP[ii]->type == REC_TYPE_PROTO_TITLE)
		{
			if(RecordPP[ii]->version == 1)
			{
// #ifdef __MOBILE__
// 				m_wstrCADFileName = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->FileName);
// 				Stk_Utility::ReplaceImproperCode(m_wstrCADFileName);
// 				m_wstrFilePathName = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->FilePathName);
// 				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(((StkProtoTitleRec*)RecordPP[ii])->DataSource);
// 				m_nuPolygonNum = ((StkProtoTitleRec*)RecordPP[ii])->PolygonNum;
// #else
// 				m_wstrCADFileName = ((StkProtoTitleRec*)RecordPP[ii])->FileName;
// 				Stk_Utility::ReplaceImproperCode(m_wstrCADFileName);  //ProtoType名字不允许出现'<'、'>'字符，该字符被转换为'_'
// 				m_wstrFilePathName = ((StkProtoTitleRec*)RecordPP[ii])->FilePathName;
// 				m_wstrDataSource = ((StkProtoTitleRec*)RecordPP[ii])->DataSource;
// 				m_nuPolygonNum = ((StkProtoTitleRec*)RecordPP[ii])->PolygonNum;
// #endif
			}
		}

		// 获取ProtoType的Key
		else if( RecordPP[ii]->type == REC_TYPE_PROTO_KEY){
			// 版本1
			if( RecordPP[ii]->version == 1){
#ifdef __MOBILE__
				m_wstrProtokey = Stk_Platform::STKWCHARStrToWString(((StkProtoKey*)RecordPP[ii])->atr);
#else
				m_wstrProtokey = ((StkProtoKey*)RecordPP[ii])->atr;
#endif
				state = i_FileP->RegProtoTypeKey(m_wstrProtokey,this);
				if( state!= STK_SUCCESS){
					return state;
				}
			}
		}
		// 获取子实例的状态信息
		else if(RecordPP[ii]->type == REC_TYPE_INS_ATR)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
#ifdef __MOBILE__
				wstrPlcPath = Stk_Platform::STKWCHARStrToWString(((StkInstanceAtr*)RecordPP[ii])->atr);
#else
				wstrPlcPath = ((StkInstanceAtr*)RecordPP[ii])->atr;
#endif
				nuStrIdx = wstrPlcPath.find( L"|",0 );
				nuStrIdx = wstrPlcPath.find( L"|",nuStrIdx+1 );
				idPlacement = wcstol(wstrPlcPath.substr(nuStrIdx+1).c_str(),NULL,10);

#if 0 // 提高读取大规模实例的SVL文件打开速度 add by Gaoqq on 2013.10.21
				nuElemCount = m_vcChildInss.size();
				for (jj=0; jj<nuElemCount; jj++)
				{
					CurInsP = m_vcChildInss[jj];
					if (CurInsP->GetPlcID() == idPlacement)
						break;
				}
#else
				CurInsP = GetChildByPlcID(idPlacement);
			}
#endif
		}

		// 设定子实例的颜色
		else if (RecordPP[ii]->type == REC_TYPE_INS_COL)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				if (CurInsP)
				{
					memcpy( fInsColorA, ((StkInstanceColor*)RecordPP[ii])->color, sizeof(STK_FLOAT32)*4 );
					CurInsP->SetColor( fInsColorA[0], fInsColorA[1], fInsColorA[2], fInsColorA[3] );
				}
			}
		}

		// 设定子实例的显隐状态
		else if (RecordPP[ii]->type == REC_TYPE_INS_DSP)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				nuHasItem = ((StkInstanceDisplay*)RecordPP[ii])->disp;
				if (nuHasItem == STK_INS_NODISP)
				{
					if(CurInsP)
					{
						CurInsP->SetDisplayState(STK_NO_DISPLAY);			
					}

				}
			}
		}

		// 设定子实例的材质
		else if (RecordPP[ii]->type == REC_TYPE_INS_MAT)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				STK_MAT32 insMat;
				insMat.Ambient  = ((StkInstanceMater*)RecordPP[ii])->mat[0];
				insMat.Diffuse  = ((StkInstanceMater*)RecordPP[ii])->mat[1];
				insMat.Specular = ((StkInstanceMater*)RecordPP[ii])->mat[2];

				if (nuHasItem == STK_INS_NODISP)
				{
					if (CurInsP)
					{
						CurInsP->SetMaterial(insMat);
					}
				}
			}
		}
		// 实例的自定义属性段
		else if ((RecordPP[ii]->type == REC_TYPE_METADATA))
		{
			Stk_MetaData*		stkMetaDataP = NULL;
			StkMetaDataRec*		MetaDataRecP = NULL;
			STK_UINT32			nuNumTitle = 0;
			STK_UINT32			nuNumValue = 0;
			STK_UINT32			nuNumUnits = 0;
			STK_UINT32			jj;
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				stkMetaDataP = new Stk_MetaData(this);
				if (stkMetaDataP == NULL)
				{
					return STK_ERROR_MEMORY;
				}

				MetaDataRecP = (StkMetaDataRec*)RecordPP[ii];

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
				if (stkMetaDataP != NULL && CurInsP != NULL)
				{

					CurInsP->AddMetaData(stkMetaDataP);
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
//		else if (RecordPP[ii]->type == REC_TYPE_PLCDISPLAY)
//		{
//			if (RecordPP[ii]->version == 1)
//			{
//#ifdef __MOBILE__
//				wstrPlcPath = Stk_Platform::STKWCHARStrToWString(((StkPlcPathDisplayRec*)RecordPP[ii])->plcPath);
//#else
//				wstrPlcPath = ((StkPlcPathDisplayRec*)RecordPP[ii])->plcPath;
//#endif
//				nuDisplayState = ((StkPlcPathDisplayRec*)RecordPP[ii])->nuDisplayState;
//				AddDisplayState(wstrPlcPath, (StkDisplayStateEnum)nuDisplayState);
//			}
//		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_ProtoHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *  读取下一个ProtoType
	 */
	nuElemCount = m_vcChildInss.size();
	for (ii = 0; ii < nuElemCount; ii++)
	{
		// 注意！V2.2版本后，对于File、ProtoType的读取可以不讲究先后顺序。
		// 一个直接改变就是下一个Proto不再由Instance->ReadFile()来读取，
		// 而是放在File->OpenCADFile中，不断的去读取下一个ProtoType。
		// 因此与V2.156之前的版本相比，这里没有调用Instance->ReadFile()方法。
		//if (CurInsP->GetFilePath() != L"" && CurInsP->GetFilePath() != L"Local")	// 不需要这个判断条件，因为所有的子实例都属于ProtoType的引用 add by gaoqq on 2014.9.5
		//{
			CurInsP = m_vcChildInss[ii];
			STK_ID idProto = 0;
			m_DocManager->GetGlobalIDManger()->RegProtoLinkString(CurInsP->GetProtoLink(),idProto);

			CurInsP->SetProtoID(idProto); // 虽然没有向下加载Proto，但是这里建立起来了“关联”。
			m_DocManager->AddProtoRef(idProto);	// ProtoType计数器计数
		//}
	}

	for (int ii = 0; ii < m_vcChildInss.size(); ii ++)
	{
		STK_ID idChildPlc = m_vcChildInss[ii]->GetPlcID();
		if (m_idPlcMax < idChildPlc){
			m_idPlcMax = idChildPlc;
		}
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取CAD物理文件名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrCADFileName		CAD物理文件名称
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::GetCADFileName()
{
	return m_wstrCADFileName;
}

/*************************************************************************//**
 *	@brief		设置CAD物理文件名称
 *	@param[in]	i_wstrCADFileName
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
//STK_STATUS Stk_ProtoType::SetCADFileName(wstring i_wstrCADFileName)
//{
//	m_wstrCADFileName = i_wstrCADFileName;
//
//	return STK_SUCCESS;
//}


/*****************************************************************************
 *	@brief		往SVL文件中填写用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_ProtoType::WriteUserData(Stk_File* i_stkFileP)
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
 *	@brief		是否是线缆(Pipe)零件
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bPipe			是否是线缆(Pipe)零件
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsPipe()
{
	return m_bPipe;
}

/*************************************************************************//**
 *	@brief		设置是否是线缆(Pipe)零件的标识
 *	@param[in]	i_bPipe			是否是线缆(Pipe)零件
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetPipeFlag(STK_BOOL i_bPipe)
{
	m_bPipe = i_bPipe;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取长度单位
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_DOUBLE64		长度单位
 *	@note		25.4 代表英尺
 *				1.0  代表毫米
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_ProtoType::GetConceptionUnitScale()
{
	return m_dUnitFactor;
}

/*************************************************************************//**
 *	@brief		设置长度单位
 *	@param[in]	i_UnitFactor		长度单位
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
VOID  Stk_ProtoType::SetConceptionUnitScale(STK_DOUBLE64 i_UnitFactor)
{
	m_dUnitFactor = i_UnitFactor;
}


/****************************************************************************
 *	@brief		插入用户数据
 *	@param[in]	i_stkUserDataP		用户数据指针
 *	@retval		STK_SUCCESS			成功
 *	@retval		其它				失败
 *	@note		1.请new Stk_UserData对象并将用户数据流填入，然后使用ProtoType::AddUserData()插入
 *				2.通过“用户数据名称”+“用户数据ID”的方式来作为唯一标识
 *				  例如“动画”+“1” 表示第一个动画 “动画”+“2” 表示第二个动画
 *				  当两次设置的唯一标识相同时，会自动覆盖掉前次的数据
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddUserData(Stk_UserData* i_stkUserDataP)
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
map<STK_ID,Stk_UserData*>* Stk_ProtoType::GetUserDataByName(wstring i_wstrDataName)
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
STK_STATUS  Stk_ProtoType::DelUserData( wstring i_wstrDataName, STK_ID i_id)
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

/************************************************************************//**
 *	@brief		将V2.2之前的Body、PMI等转换为Node模式
 *	@param[in]	i_eType			Node类型
 *	@param[in]	i_stkDataP		Node对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		转换成功
 *	@retval		其他			转换失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoType::ConvertEntityToNode(StkNodeTypeEnum i_eType, Stk_Entity* i_stkDataP)
{
	Stk_Node*		stkNodeP = NULL;
	STK_STATUS		eState = STK_SUCCESS;
	STK_ID			idPlc;

	// 设置顶级Node
	SetNodeMode(true);
	stkNodeP = new Stk_Node(m_DocManager,this);

	if (i_eType == NODE_TYPE_PMI && m_stkFileP->IsOldVersion())
	{
		eState = stkNodeP->RegisterID(i_stkDataP->GetID());
		if (eState != STK_SUCCESS)
		{
			delete stkNodeP;
			return eState;
		}
	}
	else
	{
		eState = stkNodeP->RegisterID(STK_NEW_ID);
		if (eState != STK_SUCCESS)
		{
			delete stkNodeP;
			return eState;
		}
	}

	switch(i_eType)
	{
	case NODE_TYPE_VIRTUAL:
		stkNodeP->SetNodeType(NODE_TYPE_VIRTUAL);
		break;
	case NODE_TYPE_BODY:
		stkNodeP->SetNodeType(NODE_TYPE_BODY);
		stkNodeP->SetBody((Stk_Body*)i_stkDataP);
		break;
	case NODE_TYPE_MESH:
		stkNodeP->SetNodeType(NODE_TYPE_MESH);
		stkNodeP->SetMesh((Stk_Mesh_Com*)i_stkDataP);
		break;
	case NODE_TYPE_PMI:
		stkNodeP->SetNodeType(NODE_TYPE_PMI);
		stkNodeP->SetPMI((Stk_PMI*)i_stkDataP);
		break;
	case NODE_TYPE_META:
		stkNodeP->SetNodeType(NODE_TYPE_META);
		stkNodeP->SetMetaData((Stk_MetaData*)i_stkDataP);
		break;
	case NODE_TYPE_VIEW:
		stkNodeP->SetNodeType(NODE_TYPE_VIEW);
		stkNodeP->SetView((Stk_View*)i_stkDataP);
		break;
	default:
		return STK_ERROR;
		break;
	}

	if (m_TopNodeP == NULL)
	{
		return STK_ERROR_NOMORE_NODE;
	}

	m_TopNodeP->AddChildNode(stkNodeP->GetID(), idPlc);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		将V2.2之前的Body、PMI等转换为Node模式
 *	@param[in]	i_eType			Node类型
 *	@param[in]	i_stkDataP		要素指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		转换成功
 *	@retval		其他			转换失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_ProtoType::AddEntityToTopNode(StkNodeTypeEnum i_eType, Stk_Entity* i_stkDataP)
{
	Stk_Node*		stkNodeP = NULL;
	STK_STATUS		eState = STK_SUCCESS;
	STK_ID			idPlc;

	// 设置顶级Node
	SetNodeMode(true);
	stkNodeP = new Stk_Node(m_DocManager,this);

	eState = stkNodeP->RegisterID(STK_NEW_ID);
	if (eState != STK_SUCCESS)
	{
		delete stkNodeP;
		return eState;
	}

	switch(i_eType)
	{
	case NODE_TYPE_VIRTUAL:
		stkNodeP->SetNodeType(NODE_TYPE_VIRTUAL);
		break;
	case NODE_TYPE_BODY:
		stkNodeP->SetNodeType(NODE_TYPE_BODY);
		stkNodeP->SetBody((Stk_Body*)i_stkDataP);
		break;
	case NODE_TYPE_MESH:
		stkNodeP->SetNodeType(NODE_TYPE_MESH);
		stkNodeP->SetMesh((Stk_Mesh_Com*)i_stkDataP);
		break;
	case NODE_TYPE_PMI:
		stkNodeP->SetNodeType(NODE_TYPE_PMI);
		stkNodeP->SetPMI((Stk_PMI*)i_stkDataP);
		break;
	case NODE_TYPE_META:
		stkNodeP->SetNodeType(NODE_TYPE_META);
		stkNodeP->SetMetaData((Stk_MetaData*)i_stkDataP);
		break;
	case NODE_TYPE_VIEW:
		stkNodeP->SetNodeType(NODE_TYPE_VIEW);
		stkNodeP->SetView((Stk_View*)i_stkDataP);
		break;
	default:
		return STK_ERROR;
		break;
	}

	if (m_TopNodeP == NULL)
	{
		return STK_ERROR_NOMORE_NODE;
	}

	m_TopNodeP->AddChildNode(stkNodeP->GetID(), idPlc);

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		遍历指定类型的所有Node
 *	@param[in]	i_eType			Node类型
 *	@param[in]	i_stkDataP		Node对象指针
 *	@param[out]	o_vcNodes		指定类型的所有Node
 *	@retval		STK_SUCCESS		遍历成功
 *	@retval		其他			遍历失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::PreOrderNode(Stk_Node* i_stkNodeP, StkNodeTypeEnum i_eType, vector<Stk_Node*>& o_vcNodes)
{
	STK_UINT32			nuChildNodeNum = 0;
	StkNodeTypeEnum		eNodeType = NODE_TYPE_UNKNOWN;
	Stk_Node*			stkNodeP = NULL;
	STK_ID				nuPlcID;

	if (i_stkNodeP == NULL)
	{
		return STK_ERROR_NOMORE_NODE;
	}

	if(i_stkNodeP->GetNodeType()==i_eType){
		o_vcNodes.push_back(i_stkNodeP);
	}
	
	nuChildNodeNum = i_stkNodeP->GetChildNodeNum();
	for (int ii = 0; ii <nuChildNodeNum; ii++)
	{
		stkNodeP = i_stkNodeP->GetChildNode(ii,nuPlcID);
		if (stkNodeP == NULL)
		{
			return STK_ERROR;
		}
		
		PreOrderNode(stkNodeP, i_eType, o_vcNodes);
	}
	
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		添加组件特征
 *	@param[in]	i_ComponentFeatP	组件特征
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddComponentFeature(Stk_ComponentFeature* i_ComponentFeatP)
{
	if (i_ComponentFeatP != NULL)
	{
		m_vcComponentFeats.push_back(i_ComponentFeatP);
	}
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取组件特征数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		组件特征数量
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_UINT32 Stk_ProtoType::GetComponentFeatureNum()
{
	return m_vcComponentFeats.size();
}

/****************************************************************************
 *	@brief		获取指定的组件特征
 *	@param[in]	i_ComponentFeatP	组件特征
 *	@param[out]	无
 *	@retval		指定的组件特征
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_ComponentFeature* Stk_ProtoType::GetComponentFeatureByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcComponentFeats.at(i_nuIndex);
}

/************************************************************************//**
 *	@brief		转换Body信息
 *	@param[in]	i_StkBodyP		Body对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入Body型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::ConvertBody(Stk_Body *i_StkBodyP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = ConvertEntityToNode(NODE_TYPE_BODY, (Stk_Entity*)i_StkBodyP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	m_bIsDirtyData = TRUE;
	return eState;
}

/************************************************************************//**
 *	@brief		转换View信息
 *	@param[in]	i_StkViewP		View对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入View型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::ConvertView(Stk_View *i_StkViewP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = ConvertEntityToNode(NODE_TYPE_VIEW, (Stk_View*)i_StkViewP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	return eState;
}

/************************************************************************//**
 *	@brief		转换PMI数据信息
 *	@param[in]	i_StkPMIP		PMI对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入PMI型的Node节点
 *				V2.2后请按照Node树的方式插入
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::ConvertPMI(Stk_PMI* i_StkPMIP)
{
	STK_STATUS eState = STK_SUCCESS;
	eState = ConvertEntityToNode(NODE_TYPE_PMI, (Stk_Entity*)i_StkPMIP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	m_bIsDirtyData = TRUE;
	return eState;
}

/****************************************************************************
 *	@brief		是否处于析构状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		true		是
 *	@retval		false		否
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsDeleting()
{
	return m_bIsDeleting;
}

/************************************************************************//**
 *	@brief		是否使用编号方式显示装配面板
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		true		是
 *	@retval		false		否
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::IsUseNumber()
{
	return m_bIsUseNumber;
}

/************************************************************************//**
 *	@brief		设置是否使用编号方式显示装配面板
 *	@param[in]	i_bUseNumber是否使用编号方式显示装配面板
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@retval		false		否
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetUseNumber(STK_BOOL i_bUseNumber)
{
	m_bIsUseNumber = i_bUseNumber;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		调整ProtoType下的子实例文件路径
 *	@param[in]	i_wstrInsSavePath			Ins的保存路径
 *	@param[in]	i_wstrPaProtoLoadPath		上级ProtoType的加载路径
 *	@param[out]	无
 *	@retval		wstrPath					ChildIns的加载路径
 *	@note		路径调整原则：
 *				1.单一SVL文件，不需要判断是否调整，直接使用顶级SVL的路径即可；
 *				2.检查打开文件目录下是否存在子实例路径的SVL文件，如果存在，则调整，
 *					目的是避免操作的拷贝的SVL文件；
 *				3.当打开文件目录下不存在子实例路径的SVL文件时，
 *					检查子实例路径对应的目录（即目标路径）下是否存在SVL文件，
 *					如果目标路径下存在SVL文件，则不调整；
 *					只有目标路径下的SVL文件不存在时，才去调整路径
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::AdjustChildInsLoadPath(const wstring& i_wstrInsSavePath, const wstring& i_wstrPaProtoLoadPath)
{
	wstring::size_type	indexPath;
	wstring				wstrSVLCheckPath = L"";
	string				strSVLCheckPath = "";
	wstring				wstrLoadPath = L"";

	wstrSVLCheckPath = ConvertChildInsLoadPath(i_wstrInsSavePath, i_wstrPaProtoLoadPath);
	strSVLCheckPath = Stk_Platform::WStringToString(wstrSVLCheckPath);
	if (access(strSVLCheckPath.c_str(), 0) != -1)
	{
		wstrLoadPath = wstrSVLCheckPath;
	}
	else // 打开目录下不存在，到目标路径查找
	{
		indexPath = i_wstrInsSavePath.find(L"|");
		wstrSVLCheckPath = i_wstrInsSavePath.substr(indexPath + 1, i_wstrInsSavePath.size() - indexPath -1);
		strSVLCheckPath = Stk_Platform::WStringToString(wstrSVLCheckPath);
		if (access(strSVLCheckPath.c_str(), 0) != -1)
		{
			wstrLoadPath = wstrSVLCheckPath;
		}
	}
	return wstrLoadPath;
}

/************************************************************************//**
 *	@brief		ProtoType是否有颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		是否有颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::HasColor()
{
	return m_bHasColor;
}

/************************************************************************//**
 *	@brief		设置ProtoType颜色
 *	@param[in]	i_rgbaProtoType	ProtoType颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetColor(STK_RGBA32 i_rgbaProtoType)
{
	m_rgbaProtoType = i_rgbaProtoType;
	m_bHasColor = true;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		ProtoType颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_ProtoType::GetColor()
{
	return m_rgbaProtoType;
}

/************************************************************************//**
 *	@brief		设置ProtoType的显隐
 *	@param[in]	i_eDispState	实例的显隐状态
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SetDisplayState(STK_BOOL i_bDisplay)
{
	m_bDisplay = i_bDisplay;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType的显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		实例的显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_ProtoType::GetDisplayState()
{
	return m_bDisplay;
}

/************************************************************************//**
 *	@brief		添加子实例的显隐状态
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_eDisplayState		显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	
	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);

	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetDisplayState(i_eDisplayState);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetDisplayState(i_eDisplayState);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定配置路径下子实例的显隐状态
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_eDisplayState		子实例的显隐状态
 *	@retval		子实例的显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_eDisplayState = stkInsAttributeP->GetDisplayState();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		添加子实例的颜色
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_rgbaIns			子实例的颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;

	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetColor(i_rgbaIns);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetColor(i_rgbaIns);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定配置路径下的子实例的颜色
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_rgbaIns			子实例的颜色
 *	@retval		指定的子实例的颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_rgbaIns = stkInsAttributeP->GetColor();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		添加配置路径下子实例的材质
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[in]	i_matIns			子实例的材质
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;

	if (i_wstrInsPlcPath == L"")
	{
		return STK_ERROR;
	}

	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);

	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		stkInsAttributeP->SetMaterial(i_matIns);
	}
	else
	{
		stkInsAttributeP = new Stk_InsAttribute();
		stkInsAttributeP->SetMaterial(i_matIns);
		m_mapInsAttribute.insert(pair<wstring, Stk_InsAttribute*>(i_wstrInsPlcPath, stkInsAttributeP));
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定的子实例的材质
 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
 *	@param[out]	o_matIns			子实例的材质
 *	@retval		指定的子实例的材质
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns)
{
	Stk_InsAttribute*	stkInsAttributeP = NULL;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		stkInsAttributeP = itor->second;
		o_matIns = stkInsAttributeP->GetMaterial();
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		写ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
 *	@param[in]	i_wstrInsPlcPath	实例配置路径
 *	@param[in]	i_stkInsAttrP		实例属性
 *	@param[in]	i_stkFileP			文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention
 ****************************************************************************/
STK_STATUS Stk_ProtoType::WriteInsAttribute(wstring i_wstrInsPlcPath, Stk_InsAttribute* i_stkInsAttrP, Stk_File* i_stkFileP)
{
	StkSegmentHead		InsAttributeHead = {0};
	StkInstanceAtr*		InsPlcPathRecP = NULL;
	STK_STATUS			eState = STK_SUCCESS;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	// 【第1步】 输出段头记录
	InsAttributeHead.id = 0;
	InsAttributeHead.kind = SEG_TYPE_INSATTRIBUTE;
	eState = i_stkFileP->WriteSegmentHeadData(&InsAttributeHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第2步】 输出实例配置路径记录
	STK_UINT32 nuCharSize = i_wstrInsPlcPath.size() + 1;
	InsPlcPathRecP = (StkInstanceAtr*)calloc(1, sizeof(StkControl) + sizeof(STK_UINT32)+ nuCharSize * sizeof(STK_WCHAR));
	InsPlcPathRecP->Control.type = REC_TYPE_INS_ATR;
	InsPlcPathRecP->Control.version = _VER_INS_ATR_RECORD;
	InsPlcPathRecP->charnum = nuCharSize;
	InsPlcPathRecP->Control.length = sizeof(STK_UINT32) + nuCharSize * sizeof(STK_WCHAR);
#ifdef __MOBILE__
	memcpy(InsPlcPathRecP->atr,Stk_Platform::WStringToSTKWCHARStr(i_wstrInsPlcPath),(i_wstrInsPlcPath.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(InsPlcPathRecP->atr, nuCharSize, (WCHAR*)i_wstrInsPlcPath.c_str());
#endif

	eState = i_stkFileP->WriteSegmentRecordData(&InsAttributeHead, (StkControl *)InsPlcPathRecP);
	if( eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第3步】 输出实例属性记录
	eState = i_stkInsAttrP->WriteFile(&InsAttributeHead, i_stkFileP);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【第4步】 完成实例属性段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&InsAttributeHead);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_InsAttrHeadP		实例属性段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention
 ****************************************************************************/
STK_STATUS Stk_ProtoType::ReadInsAttribute(Stk_File* i_stkFileP, StkSegmentHead *i_InsAttrHeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkControl**		RecordPP = NULL;
	StkInstanceAtr*		InsPlcPathRecP = NULL;
	StkInstanceColor*	InsColorRecP = NULL;
	StkInstanceDisplay*	InsDisplayRecP = NULL;
	StkInstanceMater*	InsMaterialRecP = NULL;

	wstring				wstrPlcPath = L"";
	STK_RGBA32			rgbaIns;
	STK_MAT32			matIns;
	StkDisplayStateEnum	eDisplay = STK_DISPLAY;
	/*
	 *	读取实例属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_InsAttrHeadP->recnum);
	if (i_InsAttrHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_stkFileP->ReadSegmentAllRecordData(i_InsAttrHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取属性段各记录
	 */
	for (int ii = 0; ii < i_InsAttrHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_INS_ATR:
			if (RecordPP[ii]->version == 1)
			{
				InsPlcPathRecP = (StkInstanceAtr*)RecordPP[ii];
#ifdef __MOBILE__
				wstrPlcPath = Stk_Platform::STKWCHARStrToWString(InsPlcPathRecP->atr);
#else
				wstrPlcPath = InsPlcPathRecP->atr;
#endif
			}
			break;
		case REC_TYPE_INS_COL:
			if (RecordPP[ii]->version == 1)
			{
				InsColorRecP = (StkInstanceColor*)RecordPP[ii];
				memcpy(&rgbaIns, InsColorRecP->color, sizeof(STK_FLOAT32)*4);
				eState = AddInsColor(wstrPlcPath, rgbaIns);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		case REC_TYPE_INS_DSP:
			if (RecordPP[ii]->version == 1)
			{
				InsDisplayRecP = (StkInstanceDisplay*)RecordPP[ii];
				eDisplay = (StkDisplayStateEnum)InsDisplayRecP->disp;
				eState = AddInsDisplayState(wstrPlcPath, eDisplay);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		case REC_TYPE_INS_MAT:
			if (RecordPP[ii]->version == 1)
			{
				InsMaterialRecP = (StkInstanceMater*)RecordPP[ii];
				memcpy(&matIns, InsMaterialRecP->mat, sizeof(STK_FLOAT32)*3);
				eState = AddInsMaterial(wstrPlcPath, matIns);
				if (eState != STK_SUCCESS)
				{
					return eState;
				}
			}
			break;
		default:
			break;
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_InsAttrHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		获取默认视图
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		默认视图
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_View* Stk_ProtoType::GetDefaultView()
{
	STK_STATUS			eState = STK_SUCCESS;
	Stk_View*			stkViewP = NULL;
	vector<Stk_Node*>	vcNodes;
	StkViewUsageEnum	eUsageType = VIEW_USAGE_UNKNOWN;

	eState = PreOrderNode(m_TopNodeP, NODE_TYPE_VIEW, vcNodes);
	if (eState != STK_SUCCESS)
	{
		return NULL;
	}

	for (int ii = 0; ii < vcNodes.size(); ii++)
	{
		stkViewP = (vcNodes[ii])->GetView();
		eUsageType = stkViewP->GetUsageType();
		if (eUsageType == VIEW_USAGE_DEFAULT_VIEW)
		{
			return stkViewP;
		}
	}
	return NULL;
}

/************************************************************************//**
 *	@brief		获取包含实例属性的所有配置路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所有配置路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<wstring> Stk_ProtoType::GetAllInsAttrPlcPath()
{
	vector<wstring>	vcInsPlcPaths;
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.begin();
	while(itor != m_mapInsAttribute.end())
	{
		vcInsPlcPaths.push_back(itor->first);
		itor++;
	}
	return vcInsPlcPaths;
}

/************************************************************************//**
 *	@brief		删除指定配置路径的实例属性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS	删除成功
 *	@retval		STK_ERROR	删除失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ProtoType::DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath)
{
	map<wstring, Stk_InsAttribute*>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
	if (itor != m_mapInsAttribute.end())
	{
		delete itor->second;
		m_mapInsAttribute.erase(itor);
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

/************************************************************************//**
 *	@brief		添加渲染(纹理)信息
 *	@param[in]	i_stkRenderP	渲染(纹理)信息
 *	@param[out]	无
 *	@retval		STK_SUCCESS		添加成功
 *	@retval		其它			添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::AddRender(Stk_Render* i_stkRenderP)
{
	if (i_stkRenderP == NULL)
	{
		return STK_ERROR;
	}
	m_vcRenders.push_back(i_stkRenderP);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取渲染(纹理)数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		渲染(纹理)数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_UINT32 Stk_ProtoType::GetRenderNum()
//{
//	return m_vcRenders.size();
//}

/************************************************************************//**
 *	@brief		获取指定的渲染(纹理)
 *	@param[in]	i_nuIndex		索引
 *	@param[out]	无
 *	@retval		指定的渲染(纹理)
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//Stk_Render* Stk_ProtoType::GetRenderByIndex(STK_UINT32 i_nuIndex)
//{
//	return m_vcRenders.at(i_nuIndex);
//}

/****************************************************************************
 *	@brief		检测混合存储下的路径设置
 *	@param[in]	i_stkParentProtoP	当前实例的父级ProtoType
 *	@param[in]	i_stkCurProtoP		当前实例对应的ProtoType
 *	@param[out]	o_wstrSVLPath		当前ProtoType的SVL路径
 *	@retval		STK_SUCCESS			检测成功
 *	@retval		其他				检测失败
 *	@note		检测原则：
 *				1.针对场景：混合存储；
 *				2.当父级与子级的ProtoType的SVL路径相同时，使用"Local"；
 *				3.当父级与子级的ProtoType的SVL路径不相同时，使用各自自己的SVL路径；
 *	@attention
 ****************************************************************************/
STK_STATUS Stk_ProtoType::DetectPathFromBeforeSave(Stk_ProtoType* i_stkParentProtoP, Stk_ProtoType* i_stkCurProtoP, wstring& o_wstrSVLPath)
{
	STK_STATUS		eState = STK_SUCCESS;
	wstring			wstrParentPath = L"";
	wstring			wstrCurPath = L"";
	Stk_File*		stkFileP = NULL;

	wstrParentPath = i_stkParentProtoP->GetFile()->GetSVLPath();
	stkFileP = i_stkCurProtoP->GetFile();
	if (stkFileP == NULL)
	{
		return STK_ERROR;
	}
	wstrCurPath = stkFileP->GetSVLPath();

	if (wstrParentPath == wstrCurPath)
	{
		o_wstrSVLPath = L"Local";
	}
	else
	{
		o_wstrSVLPath = wstrCurPath;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		根据输入的CADFile路径计算CADFile名称
 *	@param[in]	i_wstrCADPath	CADFile路径
 *	@param[out]	无
 *	@retval		wstrSVLName		CADFile文件名称
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_ProtoType::ComputeCADFileName(wstring i_wstrCADPath)
{
	STK_INT32	nuPos = 0;
	wstring		wstrSVLName = L"";

	nuPos = i_wstrCADPath.find_last_of(L"\\");
	if (nuPos != -1)
	{
		wstrSVLName = i_wstrCADPath.substr(nuPos + 1, i_wstrCADPath.length() - nuPos - 1);
	}
	else{
		nuPos = i_wstrCADPath.find_last_of(L"/");
		wstrSVLName = i_wstrCADPath.substr(nuPos + 1, i_wstrCADPath.length() - nuPos - 1);
	}

	return wstrSVLName;
}

/************************************************************************//**
 *	@brief		获取总实例数量
 *	@param[in]	i_stkProtoTypeP		当前ProtoType节点
 *	@param[out]	无
 *	@retval		总实例数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_ProtoType::GetTotalInstanceNum(Stk_ProtoType* i_stkProtoTypeP)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_UINT32		nuInstanceNum = 0;

	eState = SearchChildInstanceFromProtoType(i_stkProtoTypeP, nuInstanceNum);
	if (eState != STK_SUCCESS)
	{
		nuInstanceNum = 0;
		return nuInstanceNum;
	}

	return nuInstanceNum;
}

/************************************************************************//**
 *	@brief		遍历当前ProtoType的所有子实例
 *	@param[in]	i_stkProtoTypeP		当前ProtoType对象
 *	@param[out]	o_nuInstanceNum		子实例数量
 *	@retval		STK_SUCCESS			遍历成功
 *	@retval		其它				遍历失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SearchChildInstanceFromProtoType(Stk_ProtoType* i_stkProtoTypeP, STK_UINT32& o_nuInstanceNum)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_UINT32		nuChildInsNum = 0;
	Stk_Instance*	stkInstanceP = NULL;
	Stk_ProtoType*	stkProtoTypeP = NULL;

	nuChildInsNum = i_stkProtoTypeP->GetChildInsNum();
	o_nuInstanceNum += nuChildInsNum;
	for (int ii = 0; ii < nuChildInsNum; ii++)
	{
		stkInstanceP = i_stkProtoTypeP->GetChildIns(ii);
		if (stkInstanceP == NULL)
		{
			return STK_ERROR;
		}

		stkProtoTypeP = stkInstanceP->GetProtoType();
		if (stkProtoTypeP == NULL)
		{
			continue;	// continue的原因是有可能装配结构中只有实例，而没有Proto的情形
		}

		eState = SearchChildInstanceFromProtoType(stkProtoTypeP, o_nuInstanceNum);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}

	}
	return eState;
}

/************************************************************************//**
 *	@brief		获取Node节点数量
 *	@param[in]	i_stkProtoTypeP		当前ProtoType节点
 *	@param[out]	无
 *	@retval		Node节点数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_ProtoType::GetNodeNum(Stk_ProtoType* i_stkProtoTypeP)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_UINT32		nuNodeNum = 0;
	Stk_Node*		stkTopNodeP = NULL;
	Stk_Node*		stkNodeP = NULL;
	STK_UINT32		nuChildNodeNum = 0;

	if (i_stkProtoTypeP == NULL)
	{
		return 0;
	}
	stkTopNodeP = i_stkProtoTypeP->GetTopNode();
	if (stkTopNodeP == NULL)
	{
		return 0;
	}
	nuChildNodeNum = stkTopNodeP->GetChildNodeNum();
	nuNodeNum += nuChildNodeNum;
	for (int ii = 0; ii < nuChildNodeNum; ii++)
	{
		STK_ID idPlc = 0;
		stkNodeP = stkTopNodeP->GetChildNode(ii, idPlc);
		eState = SearchChildNodeFromTopNode(stkNodeP, nuNodeNum);
		if (eState != STK_SUCCESS)
		{
			return 0;
		}
	}
	return nuNodeNum;
}

/************************************************************************//**
 *	@brief		遍历当前ProtoType的所有子实例
 *	@param[in]	i_stkNodeP			顶级Node
 *	@param[out]	o_nuNodeNum			子Node数量
 *	@retval		STK_SUCCESS			遍历成功
 *	@retval		其它				遍历失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ProtoType::SearchChildNodeFromTopNode(Stk_Node* i_stkNodeP, STK_UINT32& o_nuNodeNum)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_UINT32		nuChildNodeNum = 0;
	Stk_Node*		stkChildNodeP = NULL;

	nuChildNodeNum = i_stkNodeP->GetChildNodeNum();
	o_nuNodeNum += nuChildNodeNum;
	for (int ii = 0; ii < nuChildNodeNum; ii++)
	{
		STK_ID	idPlc = 0;
		stkChildNodeP = i_stkNodeP->GetChildNode(ii, idPlc);
		if (stkChildNodeP == NULL)
		{
			continue;
		}
		eState = SearchChildNodeFromTopNode(stkChildNodeP, o_nuNodeNum);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}
	return STK_SUCCESS;
}
