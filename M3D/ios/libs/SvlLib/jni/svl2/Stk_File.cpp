// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_File.cpp
 *
 *	@brief	文件类成员函数的实现
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
 *		2013/10/6	更新。Gaoqq@HOTEAMSOFT
 *					增加预览图
 *		2014/4/23	更新。Gaoqq@HOTEAMSOFT
 *					统一Andriod平台
****************************************************************************/

#include "StdAfx.h"
#include "Stk_File.h"
#include "Stk_ProtoType.h"
#include "Stk_Preview.h"
#include "Stk_Manager.h"
#include "Stk_DocumentManager.h"
#include "Stk_Utility.h"
#include "Stk_Platform.h"
#include <math.h>
#include "Stk_Render.h"
#include "Stk_VirtualBomItemInfo.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif
#include "Stk_Listener.h"

/************************************************************************//**
 *	@brief		Stk_File构造函数
 *	@param[in]	i_stkDocManagerP	DocManager对象指针
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_File::Stk_File(Stk_DocumentManager* i_stkDocManagerP)
{
	//m_typeID = STK_TYPE_DOCUMENT;
	//m_TopInstanceP = NULL;
	m_wstrSVLPath = L"";
	m_BinFileP = NULL;
	m_nuHeadSeek = 0;
	m_wstrTitle = SVL_FILE_TITLE;
	m_wstrCADFileName = L"";
	m_wstrDataSource = L"";
	m_wstrConvTookit = L"";
	m_wstrSVLConversion = L"";
	m_wstrSVLVersion = SVL_DATAMANAGER_VERSION;
	m_wstrCreateTime = L"";
	m_wstrCompression = L"";
	//m_wstrSVLStorage = L"";
	m_nuPolygonNum = 0;
	m_nuProtoTypeNum = 0;
	//m_nuTopProtoID = 0;
	m_PreviewP = NULL;
	m_ID = 0;
	//m_stkTopProtoP = NULL;
	m_bLineSetConvert = FALSE;
	m_fLineSetTol = 0.001;
	m_DocManager = i_stkDocManagerP;
	m_idRefCount = 0;
	m_bTrueFile = false;
	m_bIsTop = false;
	m_bIsOldVersion = false;
	m_wstrSVLSaveTmpPath = L"";
	m_eCompressType = TYPE_COMPRESS_NO;
	m_chSegBuffP = NULL;
	m_nuCompressSegmentNum = 0;
	m_nuReadCompressSegCount = 0;
	m_chBufferItorP = NULL;
	m_bIsCompress = false;
	m_nuHaveAllocBlock = 0;
	m_nuHaveUseBlock = 0;
	m_wstrPassword = L"";
}

/************************************************************************//**
 *	@brief		Stk_File析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_File::~Stk_File(void)
{
// 	if (m_TopInstanceP != NULL)
// 	{
// 		delete m_TopInstanceP;
// 		m_TopInstanceP = NULL;
// 	}
// 	else 
// 	{
		//if (m_stkTopProtoP != NULL)
		//{
		//	delete m_stkTopProtoP;
		//	m_stkTopProtoP = NULL;
		//}
//	}

//	由DocumentManager释放ProtoType
//	ProtoType会自动释放File

//	for(int ii = 0; ii < m_vcTopProtoTypes.size(); ii++)
//	{
//		if (m_vcTopProtoTypes[ii] != NULL)
//		{
//			delete m_vcTopProtoTypes[ii];
//			m_vcTopProtoTypes[ii] = NULL;
//		}
//	}

	if (m_PreviewP != NULL)
	{
		delete m_PreviewP;
		m_PreviewP = NULL;
	}

	if (m_bTrueFile)
	{
		//m_DocManager->GetGlobalIDManger()->DelFileLinkString(GetFileLink());
		m_DocManager->GetGlobalIDManger()->DelFileID(m_ID);
		m_DocManager->GetGlobalIDManger()->DelCADFileIDByPath(m_wstrCADPath);
	}

	map<STK_ID, Stk_Render*>::iterator itor = m_mapRenders.begin();
	while(itor != m_mapRenders.end())
	{
		if (itor->second != NULL)
		{
			delete itor->second;
			itor->second = NULL;
		}
		itor++;
	}

	// 释放BOM表格
	map<wstring, Stk_VirtualBomItemInfo*>::iterator itorBomItem = m_mapBomItemInfos.begin();
	while(itorBomItem != m_mapBomItemInfos.end())
	{
		if (itorBomItem->second != NULL)
		{
			delete itorBomItem->second;
			itorBomItem->second = NULL;
		}
		itorBomItem++;
	}
}

#if 0
/************************************************************************//**
 *	@brief		File对象计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		内部接口
 *	@attention	无
 ****************************************************************************/

void Stk_File::AddRef()
{
	m_idRefCount++;
}

/************************************************************************//**
 *	@brief		File内存释放
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		V2.2版本以后不再使用此接口
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::Release()
{
	m_idRefCount--;
	if (m_idRefCount <= 0)
	{
		delete this;
		return STK_SUCCESS;
	}

	return STK_SUCCESS_EXIST;
}

#if 0
/************************************************************************//**
 *	@brief		注册File Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			已注册，跳出
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::RegFileLink()
{
	STK_STATUS ret = m_DocManager->GetGlobalIDManger()->RegFileLink(this, m_ID);
	if (ret==STK_SUCCESS)
	{
		m_bTrueFile = true;
	}
	return ret;
}
#endif

/************************************************************************//**
 *	@brief		查询File Link
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLPath			文件路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

wstring Stk_File::GetFileLink()
{
	return m_wstrSVLPath; //FilePath就是FileLink
}
#endif

/************************************************************************//**
 *	@brief		注册File ID
 *	@param[in]	i_idFile					文件ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		其它						注册失败
 *	@note		输入参数只能设置为STK_NEW_ID，不允许由用户指定ID
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::RegisterID(STK_ID i_idFile)
{
	STK_STATUS ret = m_DocManager->GetGlobalIDManger()->RegFileID(this, i_idFile );
	if( ret== STK_SUCCESS)
	{
		m_ID = i_idFile;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		查询File ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_ID			文件ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_File::GetFileID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置文件预览图
 *	@param[in]	i_PreviewP		文件预览图对象指针
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetFilePreview( Stk_Preview *i_PreviewP)
{
	m_PreviewP = i_PreviewP;
}

/************************************************************************//**
 *	@brief		获取文件预览图
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_PreviewP		文件预览图对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Preview* Stk_File::GetFilePreview()
{
	return m_PreviewP;
}

/************************************************************************//**
 *	@brief		设置PMI存储模式
 *	@param[in]	i_eMode				存储模式
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetOptionOfPmiMode(StkPMIModeEnum i_eMode)
{
	wstring i_wstrOption,i_wstrValue;
	if (i_eMode == PMI_MODE_PARA)
	{
		i_wstrOption = L"SVL_PMI_MODE";
		i_wstrValue = L"PARA";
		m_mapOptStrings.insert(pair<wstring,wstring>(i_wstrOption, i_wstrValue));
	}
	else if (i_eMode == PMI_MODE_POLY)
	{
		i_wstrOption = L"SVL_PMI_MODE";
		i_wstrValue = L"POLY";
		m_mapOptStrings.insert(pair<wstring,wstring>(i_wstrOption, i_wstrValue));
	}
}

/************************************************************************//**
 *	@brief		获取PMI存储模式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		PMI_MODE_UNKNOW		模式未知
 *	@retval		PMI_MODE_PARA		参数模式
 *	@retval		PMI_MODE_POLY		折线模式
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPMIModeEnum Stk_File::GetOptionOfPmiMode()
{
	map<wstring,wstring> ::iterator itor;
	StkPMIModeEnum eMode = PMI_MODE_UNKNOWN;

	itor = m_mapOptStrings.find(L"SVL_PMI_MODE");
	if (itor != m_mapOptStrings.end())
	{
		if (itor->second == L"PARA")
		{
			eMode = PMI_MODE_PARA;
		}
		else if (itor->second == L"POLY")
		{
			eMode = PMI_MODE_POLY;
		}
	}

	return eMode;
}

#if 0
// 注意！因为消除顶级实例，所以取消了这个接口。
// 以后File下只有有顶级ProtoType。

/****************************************************************************
 *	@brief		打开文件
 *	@param[in]	i_strSVLPath		文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			打开成功
 *	@retval		STK_ERROR			打开失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_File::OpenFile(wstring i_strSVLPath)
{
	CHAR			chPath[STK_MAXPATH];
	WCHAR			wchTmpSTR[STK_MAXNAME];
	wstring			wstrCurLink;
	string			strTmpPath;
	STK_MTX32		mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	StkSegmentHead	head ={0};
	STK_STATUS		eRet = STK_SUCCESS;
	Stk_ProtoType	*ProtoTypeP = NULL;
	STK_STATUS		eState = STK_SUCCESS;
	Stk_Preview		*PreviewP = NULL;

	/*
	 *  打开SVL文件
	 */
	m_nuHeadSeek = 0;
	strTmpPath = Stk_Platform::WStringToString(i_strSVLPath );
	m_wstrSVLPath = i_strSVLPath;

	m_BinFileP = fopen(strTmpPath.c_str(),"rb");
	if (m_BinFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  读取整个文件
	 */
	while(1){
		eState = this->ReadSegmentHeadData(&head);
		// 发现未知文件时的跳出处理
		if (eState != STK_SUCCESS)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		if (head.Control.type != REC_TYPE_HEAD || head.Control.version != 1)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}

		switch( head.kind )
		{
		case SEG_TYPE_TITLE:
			eState = this->ReadTitle(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PREVIEW:
			PreviewP = new Stk_Preview();
			eState = PreviewP->ReadFile(this, &head );
			if (eState == STK_SUCCESS)
			{
				SetFilePreview(PreviewP);
			}
			else
			{
				delete PreviewP;
				PreviewP = NULL;
			}
			break;

		case SEG_TYPE_OPTION:
			eState = this->ReadOption(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PROTO:
			wstrCurLink.erase();
			swprintf(wchTmpSTR, STK_MAXNAME, L"%d", m_nuTopProtoID);
			wstrCurLink += wchTmpSTR; //###
			wstrCurLink += L"|";
			wstrCurLink += i_strSVLPath;
			m_TopInstanceP = new Stk_Instance(m_DocManager);
			m_TopInstanceP->SetProtoLink(wstrCurLink );
			m_TopInstanceP->RegisterID(STK_NEW_ID);
			m_TopInstanceP->SetFilePath(i_strSVLPath);
#if 0
			m_TopInstanceP->ReadFile( this, &head );
#else
			eRet = m_TopInstanceP->RegProtoLink(wstrCurLink);
			if (eRet == STK_SUCCESS_EXIST)
			{
				return STK_ERROR;
			}
			m_stkTopProtoP = new Stk_ProtoType(wstrCurLink,m_DocManager);
			m_stkTopProtoP->RegisterID(STK_NEW_ID);
			m_stkTopProtoP->SetFile(this);
			eState = m_stkTopProtoP->ReadFile(this, &head);
			if (eState!=STK_SUCCESS)
			{
				delete m_stkTopProtoP;
				return eState;
			}
#endif
			//m_TopInstanceP->GetProtoType()->AddRef();
			m_TopInstanceP->SetPlacement(0, mtxUnit);
			m_TopInstanceP->SetInsName(m_stkTopProtoP->GetProtoName()); // 设置顶级实例的名字 add on 2014.4.17 by Gaoqq
			break;

		//case 动画
		//case 注释
		//case 其它

			// 当下一段是File-END表时
		case SEG_TYPE_FILE_END:
			goto pt1;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = this->ReadSegmentAllRecord(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = this->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *  关闭SVL文件
	 */	
	if (m_BinFileP)
	{
		int tt = fclose(m_BinFileP);
		m_BinFileP = NULL;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		打开V2.2之前的文件
 *	@param[in]	i_strSVLPath		文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			打开成功
 *	@retval		STK_ERROR			打开失败
 *	@note		V2.2以后不再使用此接口
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::OpenFile(wstring i_strSVLPath)
{
	CHAR			chPath[STK_MAXPATH];
	WCHAR			wchTmpSTR[STK_MAXNAME];
	wstring			wstrCurLink;
	string			strTmpPath;
	STK_MTX32		mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	StkSegmentHead	head ={0};
	STK_STATUS		eRet = STK_SUCCESS;
	Stk_ProtoType	*ProtoTypeP = NULL;
	STK_STATUS		eState = STK_SUCCESS;
	Stk_Preview		*PreviewP = NULL;

	/*
	 *  打开SVL文件
	 */
	m_nuHeadSeek = 0;
	strTmpPath = Stk_Platform::WStringToString(i_strSVLPath );
	m_wstrSVLPath = i_strSVLPath;

	m_BinFileP = fopen(strTmpPath.c_str(),"rb");
	if (m_BinFileP == NULL)
	{
		return STK_ERROR;
	}


	/*
	 *  读取整个文件
	 */
	while(1){
		eState = this->ReadSegmentHeadData(&head);
		// 发现未知文件时的跳出处理
		if (eState != STK_SUCCESS)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		if (head.Control.type != REC_TYPE_HEAD || head.Control.version != 1)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}

		switch( head.kind )
		{
		case SEG_TYPE_TITLE:
			eState = this->ReadTitle(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PREVIEW:
			PreviewP = new Stk_Preview();
			eState = PreviewP->ReadFile(this, &head );
			if (eState == STK_SUCCESS)
			{
				SetFilePreview(PreviewP);
			}
			else
			{
				delete PreviewP;
				PreviewP = NULL;
			}
			break;

		case SEG_TYPE_OPTION:
			eState = this->ReadOption(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PROTO:
			wstrCurLink.erase();
			//swprintf(wchTmpSTR, L"%d", m_nuTopProtoID);
			//wstrCurLink += wchTmpSTR; //###
			//wstrCurLink += L"|";
			wstrCurLink += i_strSVLPath;
		
#if 0
			m_TopInstanceP->ReadFile( this, &head );
#else
			// 顶级ProtoType需要手动注册ProtoLink
			// 其它ProtoType是在创建其子Instance时注册的ProtoLink
			STK_ID tmpID;
			m_DocManager->GetGlobalIDManger()->RegProtoLinkString(wstrCurLink, tmpID);  //UG、CATIA支持

			ProtoTypeP = new Stk_ProtoType(wstrCurLink,m_DocManager);
			ProtoTypeP->RegisterID(STK_NEW_ID);
			ProtoTypeP->SetFile(this);
			eState =ProtoTypeP->ReadFile(this, &head);
			if (eState!=STK_SUCCESS)
			{
				delete ProtoTypeP;
				return eState;
			}

			m_vcTopProtoTypes.push_back(ProtoTypeP);
#endif
		
			break;

		//case 动画
		//case 注释
		//case 其它

			// 当下一段是File-END表时
		case SEG_TYPE_FILE_END:
			goto pt1;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = this->ReadSegmentAllRecord(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = this->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *  关闭SVL文件
	 */	
	if (m_BinFileP)
	{
		int tt = fclose(m_BinFileP);
		m_BinFileP = NULL;
	}

	return eState;
}
#endif

/************************************************************************//**
 *	@brief		兼容V2.2之前版本，以V2.2模式打开
 *	@param[in]	i_strSVLPath		文件路径
 *	@param[out]	io_fileP			文件指针
				 = NULL				文件读取结束
				 != NULL			文件未读取结束，文件中还有CADFile待读取
 *	@param[out]	io_nuProtoTypeCount	用于记录SVL中ProtoType数量
 *	@param[out]	o_vecChildPath		子文件的路径
 *	@retval		STK_SUCCESS			打开成功
 *	@retval		STK_ERROR			打开失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::OpenFile(wstring i_strSVLPath, FILE* &io_fileP, STK_UINT32& io_nuProtoTypeCount, vector<wstring>& o_vecChildPath)
{
	WCHAR			wchTmpSTR[STK_MAXNAME];
	wstring			wstrCurLink;
	wstring			wstrNewCADFilePath = L"";
	wstring			wstrProtoTypeName = L"";
	string			strTmpPath;
	STK_MTX32		mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	StkSegmentHead	head ={0};
	StkSegmentHead	headNext ={0};
	STK_STATUS		eRet = STK_SUCCESS;
	STK_STATUS		eState = STK_SUCCESS;
	Stk_Preview		*PreviewP = NULL;
	Stk_ProtoType	*stkProtoP = NULL;
	STK_SIZE_T		unNum = 0;
	Stk_Instance	*stkChildIns = NULL;
	STK_ID			tmpID;
	STK_BOOL		bIsDistribute = true;
	STK_BOOL		bIsOldVersion = false;
	Stk_File*		stkParentFileP = NULL;

	/*
	 *  打开SVL文件
	 */
	m_nuHeadSeek = 0;
	strTmpPath = Stk_Platform::WStringToString(i_strSVLPath );
	m_wstrSVLPath = i_strSVLPath;
	m_wstrSVLSaveTmpPath = i_strSVLPath;

	if(io_fileP == NULL){
		m_BinFileP = fopen(strTmpPath.c_str(),"rb");
		if (m_BinFileP == NULL)
		{
			return STK_ERROR;
		}
	}
	else{
		m_BinFileP = io_fileP;
		bIsDistribute = false;
	}
	
	/*
	 *  读取该File下的所有数据段
	 */
	while(1){
		eState = this->ReadSegmentHeadData(&head);
		// 发现未知文件时的跳出处理
		if (eState != STK_SUCCESS)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		if (head.Control.type != REC_TYPE_HEAD || head.Control.version != 1)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		
		/* 
		 *	读取当前数据段
		 */
		switch( head.kind )
		{
		case SEG_TYPE_TITLE:
			eState = this->ReadTitle(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PREVIEW:
			PreviewP = new Stk_Preview();
			eState = PreviewP->ReadFile(this, &head );
			if (eState == STK_SUCCESS)
			{
				SetFilePreview(PreviewP);
			}
			else
			{
				delete PreviewP;
				PreviewP = NULL;
			}
			break;

		case SEG_TYPE_OPTION:
			eState = this->ReadOption(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PROTO:

			stkProtoP = new Stk_ProtoType( L"", m_DocManager);
			stkProtoP->SetFile(this);
			eState = stkProtoP->ReadCADFile(this, &head);
			if (eState!=STK_SUCCESS)
			{
				delete stkProtoP;
				return eState;
			}

			// 注意！对于V2.2之前的旧SVL文件，一个ProtoType对应一个CADFile，而不是一个File对应一个CADFile
			// 因此，兼容旧文件需要进行特殊处理。每次Stk_File::OpenFile将读取一个Head与一个ProtoType，然后跳出
			// 这样就可以保持一个ProtoType对应一个CADFile的形式。
			// 判断下一个段是否是SEG_TYPE_FILE_END/SEG_TYPE_PROTO.
			// 如果是SEG_TYPE_FILE_END，则break；如果是SEG_TYPE_PROTO，则goto；如果不是这两者，则break
			eState = this->ReadSegmentHeadData(&headNext);
			if (eState != STK_SUCCESS)
			{
				eState = STK_ERROR_VERSION_UNKNOWNFILE;
				goto pt1;
			}
			// 由于用偏移判断段标志位的类型，此时读取数据还需要偏移到原位置
			
#ifdef __MOBILE__
			fseek(m_BinFileP, ftell(m_BinFileP) - HEADLEN, SEEK_SET);	
#else
			_fseeki64(m_BinFileP, _ftelli64(m_BinFileP) - HEADLEN, SEEK_SET);
#endif
			if (headNext.Control.type != REC_TYPE_HEAD || headNext.Control.version != 1)
			{
				eState = STK_ERROR_VERSION_UNKNOWNFILE;
				goto pt1;
			}

			wstrCurLink.erase();
			// 根据偏移后的数据判定该SVL文件是否是单一文件
			if (headNext.kind != SEG_TYPE_FILE_END || io_nuProtoTypeCount > 1)
			{
				swprintf(wchTmpSTR, STK_MAXNAME, L"%d", head.id);
				wstrCurLink += wchTmpSTR; // 对于V2.2之前的分散文件，还是使用ID|路径...的形式
				wstrCurLink += L"|";
			}

			// 注意！顶级ProtoType需要手动注册ProtoLink
			// 其它ProtoType是在创建其子Instance时注册的ProtoLink
			// 解决王士军所提的新旧版本的PRT兼容问题
			stkParentFileP = m_DocManager->GetParentFileByChildFilePath(i_strSVLPath);
			if (stkParentFileP != NULL)
			{
				bIsOldVersion = stkParentFileP->IsOldVersion();
			}
			if (bIsOldVersion != true && bIsDistribute != false)
			{
				wstrCurLink += stkProtoP->GetProtoName();
				wstrCurLink += L"*PRT";
				wstrCurLink += L"|";
				wstrCurLink += i_strSVLPath;
			}
			else
			{
				wstrCurLink += i_strSVLPath;
			}
			m_DocManager->GetGlobalIDManger()->RegProtoLinkString(wstrCurLink, tmpID);  //UG、CATIA支持
			stkProtoP->SetLinkString(wstrCurLink);
			stkProtoP->RegisterID(STK_NEW_ID);

			// 设置Stk_File的CADFilePath
			// 注意！V2.2之前版本没有CAD File Name，使用ProtoType Name代替
			// CADFilePath使用SVLDir+ProtoTypeName.asm(prt)代替
			wstrNewCADFilePath = stkProtoP->ConvertPathToDir(i_strSVLPath);
			wstrNewCADFilePath += stkProtoP->GetProtoName();
			if (stkProtoP->GetType() == StkProtoTypeEnum::PROTO_TYPE_ASSY)
			{
				wstrNewCADFilePath += L".asm";
			}
			else if (stkProtoP->GetType() == StkProtoTypeEnum::PROTO_TYPE_PART
				|| stkProtoP->GetType() == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
			{
				wstrNewCADFilePath += L".prt";
			}
			stkProtoP->SetCADFilePath(wstrNewCADFilePath);
			this->SetCADFilePath(wstrNewCADFilePath);

			// 填写下级SVL路径
			unNum = stkProtoP->GetChildInsNum();
			for (int i=0; i< unNum; i++)
			{
				stkChildIns = stkProtoP->GetChildIns(i);
			
				// 注意！如果发现路径为Local，则强制转为顶级的SVL路径
				// o_vecChildPath是供Stk_DocumentManager::LoadDocument()使用
				// 按照设计o_vecChildPath中允许存在重复，在LoadDocument()中会自动去重
				//if (stkChildIns->GetFilePath() == L"Local" || stkChildIns->GetFilePath() == L"" )
				//{
				//	stkChildIns->SetFilePath(i_strSVLPath);//已在Stk_ProtoType::ReadCADFile中处理，因此删除 //del by gaoqq on 2014.10.27
				//}
				o_vecChildPath.push_back(stkChildIns->GetFilePath());
			}

			// 设置顶级标志
			if (stkProtoP->IsCADFileTop())
			{
				m_vcTopProtoTypes.push_back(stkProtoP);
			}

			if (headNext.kind == SEG_TYPE_PROTO)
			{
				io_nuProtoTypeCount++;
				goto pt1;
			}
			if (headNext.kind == SEG_TYPE_FILE_END)
			{
				break;
			}

			break;

		//case 动画
		//case 注释
		//case 其它

			// 当下一段是File-END表时
		case SEG_TYPE_FILE_END:
			goto pt1;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = this->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = this->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *  关闭SVL文件
	 */
	int tmp;
	fread( &tmp, sizeof(int), 1, m_BinFileP);
	if (feof(m_BinFileP) != 0){ // V2.2后，仅当文件结束时才关闭文件
		fclose(m_BinFileP);
		m_BinFileP = NULL;
	}
	else{
#ifdef __MOBILE__
		fseek(m_BinFileP, ftell(m_BinFileP) - sizeof(int), SEEK_SET );
#else
		_fseeki64(m_BinFileP, _ftelli64(m_BinFileP) - sizeof(int), SEEK_SET );
#endif

	}
	io_fileP = m_BinFileP;

	return eState;
}

/************************************************************************//**
 *	@brief		打开CADFile文件
 *	@param[in]	i_strSVLPath		文件路径
 *	@param[out]	io_fileP			文件指针
									= NULL		文件读取结束
									!= NULL		文件未读取结束，文件中还有CADFile待读取
 *	@param[out]	o_vecChildPath		子文件的路径
 *	@retval		STK_SUCCESS			打开成功
 *	@retval		STK_ERROR			打开失败
 *	@note		V2.2及以后使用此接口
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::OpenCADFile(wstring i_strSVLPath, FILE* &io_fileP, vector<wstring>& o_vecChildPath)
{
	wstring			wstrCurLink;
	string			strTmpPath;
	STK_MTX32		mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	StkSegmentHead	head ={0};
	STK_STATUS		eState = STK_SUCCESS;
	Stk_Preview		*PreviewP = NULL;
	Stk_ProtoType	*stkProtoP = NULL;
	STK_SIZE_T		unNum = 0;
	Stk_Instance	*stkChildIns = NULL;
	StkProtoTypeEnum eProtoType = PROTO_TYPE_UNKNOWN;
	Stk_Render*		stkRenderP = NULL;
	wstring			wstrChildPath = L"";
	map<wstring, STK_BOOL>	mapChildPath;

	Stk_Listener*		stkListenerP = NULL;
	Stk_ReaderListener* stkReaderListenerP = NULL;

	/*
	 *  打开SVL文件
	 */
	m_nuHeadSeek = 0;
	strTmpPath = Stk_Platform::WStringToString(i_strSVLPath );
	m_wstrSVLPath = i_strSVLPath;
	m_wstrSVLSaveTmpPath = i_strSVLPath;
	if(io_fileP == NULL){
		// 当前File所在的SVL文件尚未打开
		m_BinFileP = fopen(strTmpPath.c_str(),"rb");
		if (m_BinFileP == NULL)
		{
			return STK_ERROR;
		}
	}
	else{
		// 当前File所在的SVL文件已经打开
		m_BinFileP = io_fileP;
	}
	
	/*
	 *  读取该File的数据
	 */
	while(1){

		eState = this->ReadSegmentHeadData(&head);
		// 发现未知文件时的跳出处理
		if (eState != STK_SUCCESS)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		if (head.Control.type != REC_TYPE_HEAD || head.Control.version != 1)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}

		// 遍历File的各个数据段
		switch( head.kind )
		{
		case SEG_TYPE_TITLE:
			eState = this->ReadTitle(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PREVIEW:
			PreviewP = new Stk_Preview();
			eState = PreviewP->ReadFile(this, &head );
			if (eState == STK_SUCCESS)
			{
				SetFilePreview(PreviewP);
			}
			else
			{
				delete PreviewP;
				PreviewP = NULL;
			}
			break;

		case SEG_TYPE_OPTION:
			eState = this->ReadOption(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;

		case SEG_TYPE_PROTO:

			// 注意！顶级ProtoType需要手动注册ProtoLink
			// 其它ProtoType是在创建其子Instance时注册的ProtoLink
			stkProtoP = new Stk_ProtoType( L"", m_DocManager);
			stkProtoP->SetFile(this);
			eState = stkProtoP->ReadCADFile(this, &head);
			/*
			 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
			 *	正式版本方案：参考NX，装配和模型数据分开存储。
			 */
			// begin
			//if (eState != STK_SUCCESS && eState != STK_LOADING_CANCEL)
			//{
			//	delete stkProtoP;
			//	return eState;
			//}
			// end

			wstrCurLink.erase();
			wstrCurLink = stkProtoP->GetProtoName();	// 更改为ProtoName.后缀|路径  的形式
			eProtoType = stkProtoP->GetType();
			if (eProtoType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
			{
				wstrCurLink += L"*ASM";
			}
			else if (eProtoType == StkProtoTypeEnum::PROTO_TYPE_PART ||
					 eProtoType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
			{
				wstrCurLink += L"*PRT";
			}
			else if (eProtoType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
			{
				wstrCurLink += L"*SHT";
			}
			wstrCurLink += L"|";
			wstrCurLink += i_strSVLPath;

			// 预发行ProtoType ID
			// 注意！对于数据管理而言，无非是对数据结构（线性表、树、图）的保存、读入编辑。
			// SVL是典型的“树”状结构。对于树来说有以下几关键点：
			// 1、节点间最好以ID关联。因此也要有一个ID管理器。
			// 2、要有一个“ID预发行机制”。例如 正在读取父节点A1时，从A1的信息中发现子文件
			// 的路径是P1。虽然还没有读到P1，我们先将“路径作为Key”，给P1预发行出ID为2。
			// 3、我完全没有必要非等到 读取P1的时候，再为P1创建对象、发型ID。
			// 不管什么时候、不管用考虑先后顺序。我只要再次遇到到P1时，依据“路径作为Key”，为P1、
			// 设置ID为2即可。 
			// 4、通过这个方式，我只是依次对节点读取（编码是顺序的，而不是按照树的结构循环
			// 调用的）。当全部顺序读取完的时候，一个完整的树就自动关联起来了。
			STK_ID tmpID;
			m_DocManager->GetGlobalIDManger()->RegProtoLinkString(wstrCurLink, tmpID);  //UG、CATIA支持

			stkProtoP->SetLinkString(wstrCurLink);
			stkProtoP->RegisterID(STK_NEW_ID);


			// 填写下级SVL路径
			unNum = stkProtoP->GetChildInsNum();
			for (int i=0; i< unNum; i++)
			{
				stkChildIns = stkProtoP->GetChildIns(i);
				// 注意！o_vecChildPath是供Stk_DocumentManager::LoadCADSubFile()使用
				// 按照设计o_vecChildPath中允许存在重复，在LoadCADSubFile()中会自动去重
				
				/*
				 *	SVL在加载大文件时，会暴露读取完文件后，有一段时间的等待过程
				 *	此过程是由于在LoadCADSubFile时，对文件是否已加载进行判断，当几十万的实例时，会比较明显
				 *	为解决该问题，需要对o_vecChildPath去重，保证每个File的o_vecChildPath数量尽可能小。
				 */
				wstrChildPath = stkChildIns->GetFilePath();
				map<wstring, STK_BOOL>::iterator itor = mapChildPath.find(wstrChildPath);
				if (/*mapChildPath.size() == 0 && */itor == mapChildPath.end())
				{
					mapChildPath.insert(pair<wstring, STK_BOOL>(wstrChildPath, TRUE));
					o_vecChildPath.push_back(stkChildIns->GetFilePath());
				}
				//o_vecChildPath.push_back(stkChildIns->GetFilePath());
			}
			// 设置顶级标志
			if (stkProtoP->IsCADFileTop())
			{
				m_vcTopProtoTypes.push_back(stkProtoP);
			}
			
			/*
			 *	加载进度监听
			 */
			stkListenerP = m_DocManager->GetListener();
			if (stkListenerP != NULL)
			{
				stkReaderListenerP = m_DocManager->GetReaderListener();
				if (stkReaderListenerP->GetTotalProtoTypeNum() == 0)
				{
					stkReaderListenerP->SetTotalProtoTypeNum(m_nuProtoTypeNum);
				}
				stkReaderListenerP->UpdataSVLLoadingPercent();
				stkReaderListenerP->ReaderProcess();
			}

			/*
			 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
			 *	正式版本方案：参考NX，装配和模型数据分开存储。
			 */
			// begin
			//if (eState == STK_LOADING_CANCEL)
			//{
				/*
				 *	释放解压时申请的内存
				 */
			//	if (m_chSegBuffP != NULL)
			//	{
			//		free(m_chSegBuffP);
			//		m_chSegBuffP = NULL;
			//	}

				/*
				 *  关闭文件
				 */
			//	if (m_BinFileP != NULL)
			//	{
			//		fclose(m_BinFileP);
			//		m_BinFileP = NULL;
			//	}

			//	return eState;
			//}

			// end
			break;
		case SEG_TYPE_RENDER:
			stkRenderP = new Stk_Render();
			eState = stkRenderP->ReadFile(this, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			AddRender(stkRenderP);
			break;
		case SEG_TYPE_VIRTUALBOM:
			eState = this->ReadVirtualBomFile(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;
		//case 动画
		//case 注释
		//case 其它
		case SEG_TYPE_COMPRESS:
			eState = ReadCompress(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			break;
			// 当下一段是File-END表时
		case SEG_TYPE_FILE_END:
			goto pt1;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = this->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = this->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *	释放解压时申请的内存
	 */
	if (m_chSegBuffP != NULL)
	{
		free(m_chSegBuffP);
		m_chSegBuffP = NULL;
	}

	/*
	 *  关闭SVL文件
	 *  注意！V2.2后，仅当文件结束时才关闭文件
	 */
	int tmp;
	fread( &tmp, sizeof(int), 1, m_BinFileP);
	if (feof(m_BinFileP) != 0){
		fclose(m_BinFileP);
		m_BinFileP = NULL;
	}
	else{
#ifdef __MOBILE__
		fseek(m_BinFileP, ftell(m_BinFileP) - sizeof(int), SEEK_SET );
#else
		_fseeki64(m_BinFileP, _ftelli64(m_BinFileP) - sizeof(int), SEEK_SET );
#endif
	}
	io_fileP = m_BinFileP;

	return eState;
}

#if 0
/*****************************************************************************
 *	@brief		写文件
 *	@param[in]	i_strPathName		文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		STK_ERROR			写入失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::WriteFile(wstring i_wstrPathName)
{
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_LogFileEx*	stkSVLLogFile;
	Stk_SVLLog		stkSVLLog;
	stkSVLLog = GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = GetDocumentManager()->GetSVLLogFile();
#endif

	if(m_vcTopProtoTypes[0] != NULL)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog,  _countof(strConvertLog), 
				_T("------------------开始写SVL文件-----------------------"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		STK_STATUS ret;
		StkSegmentHead endHead = {0};
		STK_CHAR wchPath[STK_MAXPATH];
		string strPath;

		strPath = Stk_Platform::WStringToString(i_wstrPathName);
		m_BinFileP = fopen(strPath.c_str(), "wb");
		if(m_BinFileP == NULL)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T(" ■ERROR：创建SVL文件失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		SetSVLTime();
		SetSavePath(i_wstrPathName);
		/*
		 * 【第一步】 输出Titile数据
		 */
		this->WriteTitle();

		/*
		 * 【第二步】 输出预览图
		 */
		if( m_PreviewP != NULL){
			m_PreviewP->WriteFile(this);
		}

		/*
		 * 【第三步】 输出配置项
		 */
		this->WriteOption();

		/*
		 * 【第四步】 输出ProtoType数据
		 */
		m_vcTopProtoTypes[0]->WriteFile(this);

		/*
		 * 【第五步】 输出FILE-END段
		 */
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("输出FILE-END"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		endHead.id = 0;
		endHead.kind = SEG_TYPE_FILE_END;
		ret = WriteSegmentHead(&endHead);
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：FILE-END段写入失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif
			return STK_ERROR;
		}

		if(m_BinFileP != NULL)
		{
			ret = (STK_STATUS)fclose(m_BinFileP);
			m_BinFileP = NULL;
		}
		if(ret < STK_SUCCESS)
		{
#ifdef _WINDOWS_
			if (1 == stkSVLLog.bLogOutput)
			{
				_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：关闭SVL文件失败！"));
				stkSVLLogFile->Log(strConvertLog);
			}
#endif

			return STK_ERROR;
		}
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("--------------------写SVL文件结束--------------------"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

	}
	else
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("空数据"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	return STK_SUCCESS;
}
#endif

#if 0
// 注意！因为消除顶级实例，所以取消了这个接口。
// 以后File下只有有顶级ProtoType。
/****************************************************************************
 *	@brief		获取顶级实例
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_TopInstanceP	顶级实例
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Instance* Stk_File::GetTopInstance(void)
{
	return m_TopInstanceP;
}

/*****************************************************************************
 *	@brief		设置顶级实例
 *	@param[in]	i_stkInsP		顶级实例
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		STK_ERROR		设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetTopInstance(Stk_Instance* i_stkInsP)
{
// 	if(i_stkInsP == NULL)
// 	{
// 		return STK_ERROR;
// 	}
//	else
//	{
		m_TopInstanceP = i_stkInsP;
		return STK_SUCCESS;
//	}
}
#endif

/*************************************************************************//**
 *	@brief		写CAD File文件
 *	@param[in]	i_eSaveType			保存文件类型
 *	@param[in]	i_AppendFlag		是否追加
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		STK_ERROR			写入失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_File::WriteCADFile(StkFileSaveTypeEnum i_eSaveType, STK_BOOL i_AppendFlag )
{

#ifdef _WINDOWS_
	Stk_LogFileEx*	stkSVLLogFile;
	Stk_SVLLog		stkSVLLog;
	stkSVLLog = GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = GetDocumentManager()->GetSVLLogFile();
#endif
	Stk_ProtoType*			stkTopPrototypeP = NULL;
	Stk_GlobalIDManager*	stkGlobalIDManager = m_DocManager->GetGlobalIDManger();
	STK_STATUS				state = STK_SUCCESS;
	StkSegmentHead			endHead = {0};
	string					strSVLPath = "";
	StkDocumentSaveTypeEnum	eDocumentSaveType = DOCUMENT_SAVE_TYPE_UNKNOWN;
	wstring					wstrProtoTypeName = L"";
	STK_UINT32				nuProtoNameNum = 0;
	WCHAR					wchProtoNameNum[STK_MAXNAME];
	STK_BOOL				bProtoNameFlag = false;
	Stk_ProtoType*			stkDefaultTopPrototypeP = NULL;

	eDocumentSaveType = GetDocumentManager()->GetDocumentSaveType();
	if (eDocumentSaveType == DOCUMENT_SAVE_TYPE_SAVE)
	{
		strSVLPath = Stk_Platform::WStringToString(m_wstrSVLPath);
	}
	else if (eDocumentSaveType == DOCUMENT_SAVE_TYPE_SAVEAS)
	{
		strSVLPath = Stk_Platform::WStringToString(m_wstrSVLSaveAsPath);
	}
	else
	{
		return STK_ERROR;
	}

	// 清空压缩的段的数量
	// 清空每次写file之前清空分配的buffer
	// 清空内存块计数器
	ClearSegBuffer();
	ClearCompressSegmentNum();
	m_nuHaveAllocBlock = 0;
	m_nuHaveUseBlock = 0;

	/*
	 * 【第一步】 选定SVL文件写入
	 */
	if( i_AppendFlag == false)
	{
		//新建SVL
		m_BinFileP = fopen(strSVLPath.c_str(), "wb");
	}
	else
	{
		// 追加写入SVL
		// 注意！因为没有找到实现本功能的C接口，因此通过以下方式实现相同功能
		m_BinFileP = fopen(strSVLPath.c_str(), "rb+");
		if (m_BinFileP)
		{
#ifdef __MOBILE__
			fseek(m_BinFileP, 0, SEEK_END);
#else
			_fseeki64(m_BinFileP, 0, SEEK_END);
#endif
		}
		else
		{
			m_BinFileP = fopen(strSVLPath.c_str(), "wb");
		}
	}
	if (m_BinFileP == NULL)
	{
		return STK_ERROR_SYSTEM_PERMISSIONS;
	}

	/*
	 * 【第二步】 写入File数据
	 */

	// 第2.1步 输出Titile数据
	state = this->WriteTitle();
	if (state != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 第2.2步 输出预览图
	if( m_PreviewP != NULL)
	{
		state = m_PreviewP->WriteFile(this);
		if (state != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 第2.3步 输出配置项
	state = this->WriteOption();
	if (state != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 第2.4步 输出ProtoType
	SetSVLTime();
	for (int ii = 0; ii < m_vcTopProtoTypes.size(); ii++)
	{
		stkTopPrototypeP = m_vcTopProtoTypes[ii];
		if (stkTopPrototypeP == NULL)
		{
			return STK_ERROR;
		}
		// 遍历当前File中的所有ProtoType，包括子实例的ProtoType
		// 首先检索ProtoType名称是否存在于ProtoTypeName列表中
		// 目的是为了保证ProtoTypeName的唯一性
		stkDefaultTopPrototypeP = m_DocManager->GetDefaultTopProtoType();
		if (stkDefaultTopPrototypeP != NULL)
		{
			if (this != stkDefaultTopPrototypeP->GetFile())	// 排除因合并而生成的Stk_File
			{
				SearchFileProtoTypes(stkTopPrototypeP);
			}
		}

		map<Stk_ProtoType*, STK_BOOL>::iterator itor = m_mapFileProtos.begin();
		while(itor != m_mapFileProtos.end())
		{
			wstrProtoTypeName = L"";
			nuProtoNameNum = 0;
			wstrProtoTypeName = itor->first->GetProtoName();
			bProtoNameFlag = m_DocManager->FindProtoTypeName(wstrProtoTypeName);
			if (bProtoNameFlag == true)
			{
				m_DocManager->AddProtoTypeName(wstrProtoTypeName, nuProtoNameNum);
				swprintf(wchProtoNameNum, STK_MAXNAME, L"%d", nuProtoNameNum);
				wstrProtoTypeName += wchProtoNameNum;
				itor->first->SetProtoName(wstrProtoTypeName);
			}
			m_DocManager->AddProtoTypeName(wstrProtoTypeName, nuProtoNameNum);
			itor++;
		}
		// 释放临时记录的用于改变ProtoType名称的数据(此处释放是因为零件族情形)
		m_mapFileProtos.clear();

		state = stkTopPrototypeP->WriteCADFile( this, i_eSaveType);
		if (state != STK_SUCCESS && state != STK_SUCCESS_EXIST)
		{
			return STK_ERROR;
		}
	}

	// 释放临时记录的用于改变ProtoType名称的数据
	m_DocManager->ClearAllProtoTypeName();
	m_mapFileProtos.clear();

	// 第2.5步 输出纹理段
	map<STK_ID, Stk_Render*>::iterator itor = m_mapRenders.begin();
	while(itor != m_mapRenders.end())
	{
		state = itor->second->WriteFile(this);
		if (state != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		itor++;
	}

	// 第2.6步 输出工艺BOM表格段
	if (m_mapBomItemInfos.size() > 0)
	{
		state = this->WriteVirtualBomFile();
		if (state != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 第2.7步 输出压缩段
	if (IsCompressFile() && GetSegBuffer() != NULL)
	{
		state = WriteCompress();
		if (state != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 第2.6步 输出FILE-END
	endHead.id = 0;
	endHead.kind = SEG_TYPE_FILE_END;
	state = WriteSegmentHead(&endHead);
	if(state < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 * 【第三步】 关闭SVL文件
	 */
	if(m_BinFileP != NULL)
	{
		state = (STK_STATUS)fclose(m_BinFileP);
		m_BinFileP = NULL;
		if(state < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取SVL文件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLPath	SVL文件路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetSVLPath()
{
	return m_wstrSVLPath;
}
/************************************************************************//**
 *	@brief		设置SVL文件路径
 *	@param[in]	i_wstrSVLPath
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetSVLPath(wstring i_wstrSVLPath)
{
	m_wstrSVLPath = i_wstrSVLPath;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取SVL文件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLPath	SVL文件路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_File::GetSVLSaveTmpPath()
{
	return m_wstrSVLSaveTmpPath;
}

/************************************************************************//**
 *	@brief		读取段头数据
 *	@param[in]	i_HeadP				段头指针
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_HEADRD	读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::ReadSegmentHead(StkSegmentHead *i_HeadP)
{
	STK_SIZE_T cnt = 0;
	STK_STATUS eRet = STK_SUCCESS;

	cnt = fread(i_HeadP, HEADLEN, 1, m_BinFileP);
	if (cnt <=0)
	{
		eRet = STK_ERROR_HEADRD;
	}

	return eRet;
}

/*************************************************************************//**
 *	@brief		写入数据段头
 *	@param[in]	i_HeadSP				段头指针
 *	@retval		STK_SUCCESS				写入成功
 *	@retval		其它					写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentHead(StkSegmentHead *i_HeadSP)
{
	STK_STATUS	ret;	/* Return Value			*/
	STK_SIZE_T	wrLen = 0;

	/*
	 * Initialize 
	 */ 
	ret = STK_SUCCESS;
	i_HeadSP->Control.type = REC_TYPE_HEAD;
	i_HeadSP->Control.length = HEADLEN - RECCTLLEN;
	i_HeadSP->Control.version = _VER_HEAD_RECORD;
	i_HeadSP->recnum = 0;
	i_HeadSP->datlen = 0;

	/*
	 * Put head into file
	 */
	wrLen = fwrite( (CHAR *)i_HeadSP, HEADLEN, 1, m_BinFileP );
	if( wrLen <= 0){
		ret = STK_ERROR_FILEWR;
	}

	/*
	 * 记录recnum和datalen的位置
	 */
#ifdef __MOBILE__
	m_nuHeadSeek = ftell(m_BinFileP) - HEADSEEKLEN; // 安卓输出单个文件不能超过2G
#else
	m_nuHeadSeek = _ftelli64(m_BinFileP) - HEADSEEKLEN;
#endif

	if( m_nuHeadSeek == -1){
		ret = STK_ERROR_FILEWR;
	}

	/*
	 * End process
	 */
exit:
	return(ret);

}
/************************************************************************//**
 *	@brief		读取段中所有记录
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::ReadSegmentAllRecord(
		StkSegmentHead	*i_HeadP,
		StkControl		*i_RecordA[]
)
{
	STK_STATUS	eRet;		/* return status	*/
	StkControl	tmpControl={0};
	STK_CHAR	*chTmpP;
	STK_UINT32	ii;
	STK_SIZE_T	cnt = 0;

	for (ii = 0; ii < i_HeadP->recnum; ii++)
	{

		cnt = fread(&tmpControl, RECCTLLEN, 1, m_BinFileP);
		if (cnt <=0)
		{
			eRet = STK_ERROR_FILERD;
		}
		if ((chTmpP = (STK_CHAR *)calloc(tmpControl.length + RECCTLLEN, sizeof(STK_CHAR))) == NULL)
		{
			eRet = STK_ERROR_FILE_MEMORY;
			goto pgend;
		}

		memcpy(chTmpP, &tmpControl, RECCTLLEN);
		cnt = fread(chTmpP + RECCTLLEN , tmpControl.length , 1, m_BinFileP);
		if (cnt <=0)
		{
			eRet = STK_ERROR_FILERD;
		}

		i_RecordA[ii] = (StkControl*)chTmpP;
	}

	eRet = STK_SUCCESS;

pgend:
	return eRet;

}

/*************************************************************************//**
 *	@brief		写入数据段记录
 *	@param[in]	i_HeadSP				段头指针
 *	@param[in]	i_ControlSP				表头指针
 *	@retval		STK_SUCCESS				写入成功
 *	@retval		其它					写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentRecord(StkSegmentHead *i_HeadSP,StkControl *i_ControlSP)
{
	STK_STATUS	ret;		/* Return Value			*/
	STK_SIZE_T	totalLen = 0;
	STK_SIZE_T	wrLen = 0;

	/*
	 * Insert a new record
	 */
	ret = STK_SUCCESS;
	totalLen = i_ControlSP->length + RECCTLLEN;

	if(i_ControlSP != NULL && i_ControlSP->length > 0){
		wrLen = fwrite((CHAR *)i_ControlSP, totalLen,1, m_BinFileP);
		if(totalLen > 0 && wrLen <= 0){
			ret = STK_ERROR_FILEWR;
		}
	}

	i_HeadSP->recnum ++;
	i_HeadSP->datlen += totalLen;

exit:
	return(ret);
}

/************************************************************************//**
 *	@brief		段读取结束
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::ReadSegmentFinish(
		StkSegmentHead		*i_HeadP,
		StkControl	*i_RecordA[]
)
{
	STK_STATUS	eRet;		/* return status	*/
	STK_UINT32	ii;

	if (i_HeadP->recnum != 0 && i_RecordA == NULL)
	{
		eRet = STK_ERROR_FILE_RECORD;
		goto pgend;
	}

	for (ii=0; ii < i_HeadP->recnum; ii++)
	{
		if (i_RecordA[ii] != NULL)
		{
			free(i_RecordA[ii]);
			i_RecordA[ii] = NULL;
		}
	}

	eRet = STK_SUCCESS;

pgend:
	return(eRet);
}

/*************************************************************************//**
 *	@brief		写数据段的结束记录
 *	@param[in]	i_HeadSP				头部
 *	@retval		STK_SUCCESS				写文件成功
  *	@retval		其它					写文件失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentFinish(StkSegmentHead *i_HeadSP)
{
	STK_STATUS	ret;	/* Return Value			*/
	STK_UINT64	nowseek;

	/*
	 * 保存现在的指针
	 */
	ret = STK_SUCCESS;
#ifdef __MOBILE__
	nowseek = ftell(m_BinFileP);
#else
	nowseek = _ftelli64(m_BinFileP);
#endif
	if( nowseek == -1){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}

	/*
	 * 回到Head的recnum位置
	 */
#ifdef __MOBILE__
	if(fseek(m_BinFileP, m_nuHeadSeek, SEEK_SET ) != 0){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}
#else
	if(_fseeki64(m_BinFileP, m_nuHeadSeek, SEEK_SET ) != 0){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}
#endif

	/*
	 * 写入recnum和datlen的值
	 */
	if(fwrite(&(i_HeadSP->recnum), 4, 2, m_BinFileP) <= 0){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}

	/*
	 * 恢复指针
	 */
#ifdef __MOBILE__
	if(fseek(m_BinFileP, nowseek, SEEK_SET ) != 0){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}
#else
	if(_fseeki64(m_BinFileP, nowseek, SEEK_SET ) != 0){
		ret = STK_ERROR_HEADWR;
		goto exit;
	}
#endif

exit:
	return(ret);
}

#if 0
STK_STATUS Stk_File::WriteXMLFile( string inFilePath ){

	char	tmpstr[MAX_STR_LEN];
	int		ii;

	// XML头部
	m_MeshFile = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","gb2312","");
	m_MeshFile->LinkEndChild(decl);

	// Mesh.xml版本号
	TiXmlElement *versElement = new TiXmlElement("version");
	m_MeshFile->LinkEndChild(versElement);

	TiXmlText *VersNum = new TiXmlText("1.0");
	versElement->LinkEndChild(VersNum);

	/*
	 * <Meshs>数据区
	 */
	// <Meshs ID="" FaceNum="">
	TiXmlElement *RootMeshs = new TiXmlElement("meshs");
	RootMeshs->SetAttribute("ID",(int)m_RootMesh->GetID());
	RootMeshs->SetAttribute("FaceNum",(int)m_RootMesh->GetMeshFaceNum());
	m_MeshFile->LinkEndChild(RootMeshs);

	//   <Layer></Layer>
	TiXmlElement *MeshLayer = new TiXmlElement("layer");
	RootMeshs->LinkEndChild(MeshLayer);
	sprintf_s( tmpstr, MAX_STR_LEN,"%d",m_RootMesh->GetLayerID());
	TiXmlText *LayerID = new TiXmlText(tmpstr);
	MeshLayer->LinkEndChild(LayerID);

	//   <Color></Color>
	TiXmlElement *MeshColor = new TiXmlElement("Color");
	RootMeshs->LinkEndChild(MeshColor);
	STK_RGB32 ColorRGB = m_RootMesh->GetColor();
	ii  = sprintf_s( tmpstr, MAX_STR_LEN,"%f,",ColorRGB.Red);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",ColorRGB.Green);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f",ColorRGB.Blue);
	TiXmlText *Color = new TiXmlText(tmpstr);
	MeshColor->LinkEndChild(Color);

	//   <ColorIndex><ColorIndex>
	TiXmlElement *MeshColorIndex = new TiXmlElement("ColorIndex");
	RootMeshs->LinkEndChild(MeshColorIndex);
	sprintf_s( tmpstr, MAX_STR_LEN,"%d",m_RootMesh->GetColorIndex());
	TiXmlText *ColorIndex = new TiXmlText(tmpstr);
	MeshColorIndex->LinkEndChild(ColorIndex);

	//   <BoudingBox></BoundingBox>
	TiXmlElement *MeshBoundBox = new TiXmlElement("BoundingBox");
	RootMeshs->LinkEndChild(MeshBoundBox);

	STK_BOX32 BBox = m_RootMesh->GetBoundingBox();
	ii  = sprintf_s( tmpstr, MAX_STR_LEN,"%f,",BBox.BoundBox[0][0]);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox.BoundBox[0][1]);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox.BoundBox[0][2]);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox.BoundBox[1][0]);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox.BoundBox[1][1]);
	ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f",BBox.BoundBox[1][2]);
	TiXmlText *Box = new TiXmlText(tmpstr);
	MeshBoundBox->LinkEndChild(Box);

	/*
	 * <Faces>数据区
	 */
	// <Faces>
	TiXmlElement *RootFaces = new TiXmlElement("faces");
	RootMeshs->LinkEndChild(RootFaces);

	// Mesh下的各个Face
	for(int jj=0; jj<(int)m_RootMesh->GetMeshFaceNum(); jj++){

		Stk_Mesh_Face *MeshFace = m_RootMesh->GetMeshFace(jj); 

		//<Face ID="" EdgeNum="">
		TiXmlElement *FaceElemnt = new TiXmlElement("face");
		RootFaces->LinkEndChild(FaceElemnt);
		FaceElemnt->SetAttribute("ID", (int)MeshFace->GetID());
		FaceElemnt->SetAttribute("EdgeNum", (int)MeshFace->GetMeshEdgeNum());
		
		//   <Color></Color>
		TiXmlElement *FaceColor = new TiXmlElement("Color");
		//FaceElemnt->LinkEndChild(FaceColor);
		STK_RGB32 ColorRGB = MeshFace->GetColor();
		ii  = sprintf_s( tmpstr, MAX_STR_LEN,"%f,",ColorRGB.Red);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",ColorRGB.Green);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f",ColorRGB.Blue);
		TiXmlText *Color2 = new TiXmlText(tmpstr);
		FaceColor->LinkEndChild(Color2);
		FaceElemnt->LinkEndChild(FaceColor);

		//   <BoudingBox></BoundingBox>
		TiXmlElement *FaceBoundBox = new TiXmlElement("BoundingBox");
		//FaceElemnt->LinkEndChild(FaceBoundBox);
		STK_BOX32 BBox2 = MeshFace->GetBoundingBox();
		ii  = sprintf_s( tmpstr, MAX_STR_LEN,"%f,",BBox2.BoundBox[0][0]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox2.BoundBox[0][1]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox2.BoundBox[0][2]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox2.BoundBox[1][0]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox2.BoundBox[1][1]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f",BBox2.BoundBox[1][2]);
		TiXmlText *Box2 = new TiXmlText(tmpstr);
		FaceBoundBox->LinkEndChild(Box2);
		FaceElemnt->LinkEndChild(FaceBoundBox);

		//   <PolygPoints></PolygPoints>
		TiXmlElement *PointsElemet = new TiXmlElement("PolygPoints");
		vector<STK_PNT32> PointsVector;
		STK_PNT32 PointUint;
		MeshFace->GetPolygPoints(PointsVector);
		STK_SIZE_T PointSize = PointsVector.size();
		PointsElemet->SetAttribute( "PointsNum",(int)PointSize);

		char * floatsStr = NULL;
		//floatsStr = (char*)malloc( sizeof(STK_PNT32)*(PointSize+1));
		floatsStr = (char*)malloc( 5000);
		if( floatsStr == NULL){
			return STK_ERROR_MEMORY;
		}
		ii=0;
		for( int kk=0; kk<(int)PointSize; kk++){
			PointUint = PointsVector[kk];
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint.X);
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint.Y);
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint.Z);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint.X);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint.Y);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint.Z);

		}
		TiXmlText *floatsTxt = new TiXmlText(floatsStr);
		PointsElemet->LinkEndChild(floatsTxt);
		FaceElemnt->LinkEndChild(PointsElemet );
		if( floatsStr != NULL){
			free(floatsStr);
			floatsStr = NULL;
		}

		//   <PolyIndeies></PolyIndeies>
		TiXmlElement *IndeiesElemet = new TiXmlElement("PolyIndeies");
		vector<STK_UINT32> IndeiesVector;
		STK_UINT32 IndexUint;
		MeshFace->GetPolyIndeies(IndeiesVector);
		STK_SIZE_T indeiesSize = IndeiesVector.size();
		IndeiesElemet->SetAttribute( "IndeiesNum",(int)indeiesSize);

		char * indeiesStr = NULL;
		/*indeiesStr = (char*)malloc( sizeof(STK_PNT32)*(indeiesSize+1));*/
		indeiesStr = (char*)malloc( sizeof(STK_PNT32)*(5000));
		if( indeiesStr == NULL){
			return STK_ERROR_MEMORY;
		}
		ii = 0;
		for( int kk=0; kk<(int)indeiesSize; kk++){
			IndexUint = IndeiesVector[kk];
			//ii  = sprintf_s( indeiesStr, sizeof(STK_PNT32),"%d,",IndexUint);
			ii  += sprintf_s( indeiesStr + ii, 5000 - ii,"%d,",IndexUint);
		}
		TiXmlText *IndeiesTxt = new TiXmlText(indeiesStr);
		IndeiesElemet->LinkEndChild(IndeiesTxt);
		FaceElemnt->LinkEndChild(IndeiesElemet );
		if( indeiesStr != NULL){
			free(indeiesStr);
			indeiesStr = NULL;
		}

		//<Edgeid></Edgeid>
		TiXmlElement *EdgeIDElemet = new TiXmlElement("Edgeid");
		vector<STK_ID> EdgeIDVector;
		STK_ID EdgeIDUint;
		MeshFace->GetMeshEdge(EdgeIDVector);
		STK_SIZE_T EdgeIDSize = EdgeIDVector.size();
		EdgeIDElemet->SetAttribute( "EdgeIDNum",(int)EdgeIDSize);

		char * EdgeIDStr = NULL;
		/*indeiesStr = (char*)malloc( sizeof(STK_PNT32)*(indeiesSize+1));*/
		EdgeIDStr = (char*)malloc( sizeof(STK_ID)*(5000));
		if( EdgeIDStr == NULL){
			return STK_ERROR_MEMORY;
		}
		ii = 0;
		for( int kk=0; kk<(int)EdgeIDSize; kk++){
			EdgeIDUint = EdgeIDVector[kk];
			//ii  = sprintf_s( indeiesStr, sizeof(STK_PNT32),"%d,",IndexUint);
			ii  += sprintf_s( EdgeIDStr + ii, 5000 - ii,"%d,",EdgeIDUint);
		}
		TiXmlText *EdgeIDTxt = new TiXmlText(EdgeIDStr);
		EdgeIDElemet->LinkEndChild(EdgeIDTxt);
		FaceElemnt->LinkEndChild(EdgeIDElemet );
		if( EdgeIDStr != NULL){
			free(EdgeIDStr);
			EdgeIDStr = NULL;
		}

	}

	/*
	 * <Edges>数据区
	 */

	// <Edges>
	TiXmlElement *RootEdges = new TiXmlElement("Edges");
	RootMeshs->LinkEndChild(RootEdges);

	// Mesh下的各Edge
	for(int jj=0; jj<(int)m_RootMesh->GetMeshEdgeNum(); jj++){
		Stk_Mesh_Edge *MeshEdge = m_RootMesh->GetMeshEdge(jj); 

		//<Edge ID="" EdgeNum="">
		TiXmlElement *EdgeElemnt = new TiXmlElement("Edge");
		RootEdges->LinkEndChild(EdgeElemnt);
		EdgeElemnt->SetAttribute("ID", (int)MeshEdge->GetID());

		//   <BoudingBox></BoundingBox>
		TiXmlElement *EdgeBoundBox = new TiXmlElement("BoundingBox");
		//RootMeshs->LinkEndChild(EdgeBoundBox);
		STK_BOX32 BBox3 = MeshEdge->GetBoundingBox();
		ii  = sprintf_s( tmpstr, MAX_STR_LEN,"%f,",BBox3.BoundBox[0][0]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox3.BoundBox[0][1]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox3.BoundBox[0][2]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox3.BoundBox[1][0]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f,",BBox3.BoundBox[1][1]);
		ii += sprintf_s( tmpstr + ii, MAX_STR_LEN - ii,"%f",BBox3.BoundBox[1][2]);
		TiXmlText *Box3 = new TiXmlText(tmpstr);
		EdgeBoundBox->LinkEndChild(Box3);
		EdgeElemnt->LinkEndChild(EdgeBoundBox);

		//   <GetPolylines></GetPolylines>
		TiXmlElement *PointsElemet2 = new TiXmlElement("Polylines");
		vector<STK_PNT32> PointsVector2;
		STK_PNT32 PointUint2;
		MeshEdge->GetPolylines(PointsVector2);
		STK_SIZE_T PointSize = PointsVector2.size();
		PointsElemet2->SetAttribute( "PointsNum",(int)PointSize);

		char * floatsStr = NULL;
		//floatsStr = (char*)malloc( sizeof(STK_PNT32)*(PointSize+1));
		floatsStr = (char*)malloc( 5000);
		if( floatsStr == NULL){
			return STK_ERROR_MEMORY;
		}
		ii=0;
		for( int kk=0; kk<(int)PointSize; kk++){
			PointUint2 = PointsVector2[kk];
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint2.X);
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint2.Y);
// 			ii += sprintf_s( floatsStr + ii, sizeof(STK_PNT32)*(PointSize+1) - ii,"%f,",PointUint2.Z);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint2.X);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint2.Y);
			ii += sprintf_s( floatsStr + ii, 5000 - ii,"%f,",PointUint2.Z);
		}
		TiXmlText *floatsTxt = new TiXmlText(floatsStr);
		PointsElemet2->LinkEndChild(floatsTxt);
		EdgeElemnt->LinkEndChild(PointsElemet2 );
		if( floatsStr != NULL){
			free(floatsStr);
			floatsStr = NULL;
		}
	}

	/*
	 *	输出meshs.xml
	 */
	m_MeshFile->SaveFile(inFilePath.c_str());

	return STK_SUCCESS;
}
#endif

/**************************************************************************//**
 *	@brief		设置数据源-软件名称、版本
 *	@param[in]	i_wstrDataSource			软件名称、版本
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetDataSource(const wstring& i_wstrDataSource)
{
	m_wstrDataSource = i_wstrDataSource;
}

/**************************************************************************//**
 *	@brief		设置数据交换类库-名称、版本
 *	@param[in]	i_wstrConvTookit			名称、版本
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetConvTookit(const wstring& i_wstrConvTookit)
{
	m_wstrConvTookit = i_wstrConvTookit;
}

/**************************************************************************//**
 *	@brief		设置SVL文件版本
 *	@param[in]	i_wstrSVLConversion			名称、版本
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetSVLConversion(const wstring& i_wstrSVLConversion)
{
	m_wstrSVLConversion = i_wstrSVLConversion;
}

/**************************************************************************//**
 *	@brief		设置SVL文件版本
 *	@param[in]	i_wstrSVLVersion			SVL文件版本
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetSVLVersion(const wstring& i_wstrSVLVersion)
{
	m_wstrSVLVersion = i_wstrSVLVersion;
}

/**************************************************************************//**
 *	@brief		设置SVL生成时间
 *	@param[in]	i_wstrCreateTime			生成时间
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetSVLTime(const wstring& i_wstrCreateTime)
{
	m_wstrCreateTime = i_wstrCreateTime;
}

/***************************************************************************//**
 *	@brief		设置SVL生成时间
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetSVLTime()
{
	wstring wstrSVLSaveTime;
	WCHAR year[32], mon[32], day[32], hour[32], min[32], sec[32];
	time_t tt = time(0);
	tm *newtime = localtime(&tt);
	if(newtime == NULL)
	{
		return STK_ERROR;
	}
	else
	{

#if 0
		_itow(newtime->tm_year + 1900, year, 10);
		_itow(newtime->tm_mon + 1, mon, 10);
		_itow(newtime->tm_mday, day, 10);
		_itow(newtime->tm_hour, hour, 10);
		_itow(newtime->tm_min, min, 10);
		_itow(newtime->tm_sec, sec, 10);
#else
		swprintf(year, 32, L"%d",newtime->tm_year + 1900);	//支持Andriod的改善
		swprintf(mon, 32, L"%d",newtime->tm_mon + 1);
		swprintf(day, 32, L"%d",newtime->tm_mday);
		swprintf(hour, 32, L"%d",newtime->tm_hour);
		swprintf(min,32, L"%d",newtime->tm_min);
		swprintf(sec, 32, L"%d",newtime->tm_sec);
#endif

		wstrSVLSaveTime += year;
		wstrSVLSaveTime += L"年";
		wstrSVLSaveTime += mon;
		wstrSVLSaveTime += L"月";
		wstrSVLSaveTime += day;
		wstrSVLSaveTime += L"日";
		wstrSVLSaveTime += hour;
		wstrSVLSaveTime += L"时";
		wstrSVLSaveTime += min;
		wstrSVLSaveTime += L"分";
		wstrSVLSaveTime += sec;
		wstrSVLSaveTime += L"秒";

		m_wstrCreateTime = wstrSVLSaveTime;
		return STK_SUCCESS;
	}
	
}
/************************************************************************//**
 *	@brief		设置存储的面片数量
 *	@param[in]	i_nuPolygonNum			面片数量
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetPolygonNum(STK_UINT64 i_nuPolygonNum)
{
	m_nuPolygonNum = i_nuPolygonNum;
}

/************************************************************************//**
 *	@brief		设置存储的ProtoType数量
 *	@param[in]	i_nuProtoTypeNum			ProtoType数量
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_File::SetProtoTypeNum(STK_UINT64 i_nuProtoTypeNum)
{
	m_nuProtoTypeNum = i_nuProtoTypeNum;
}

// /************************************************************************//**
//  *	@brief		设置SVL存储的方式	
//  *	@param[in]	i_wstrStorge			存储方式
//  *	@param[out]	无
//  *	@retval		无
//  *	@note		无
//  *	@attention	无
//  ****************************************************************************/
// 
// void Stk_File::SetSVLStorage(const wstring& i_wstrStorge)
// {
// 	m_wstrSVLStorage = i_wstrStorge;
// }

/************************************************************************//**
 *	@brief		获取数据源-文件名称	
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrCADFileName			文件名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetCADFileName()	
{
	return m_wstrCADFileName;
}

/************************************************************************//**
 *	@brief		获取数据源-软件名称、版本	
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrDataSource			软件名称、版本
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetDataSource()
{
	return m_wstrDataSource;
}

/************************************************************************//**
 *	@brief		获取数据交换类库-名称、版本	
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrConvTookit			名称、版本
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetConvTookit()
{
	return m_wstrConvTookit;
}

/************************************************************************//**
 *	@brief		获取SVL转换器-名称、版本	
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLConversion			名称、版本
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetSVLConversion()
{
	return m_wstrSVLConversion;
}

/************************************************************************//**
 *	@brief		获取SVL文件版本	
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLVersion			SVL文件版本
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetSVLVersion()
{
	return m_wstrSVLVersion;
}

/************************************************************************//**
 *	@brief		获取SVL生成时间
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrCreateTime			生成时间
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_File::GetSVLTime()
{
	return m_wstrCreateTime;
}

/************************************************************************//**
 *	@brief		获取存储的面片数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuPolygonNum				面片数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT64 Stk_File::GetPolygonNum()
{
	return m_nuPolygonNum;
}

/************************************************************************//**
 *	@brief		获取当前文件中存储的ProtoType数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuProtoTypeNum			ProtoType数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT64 Stk_File::GetProtoTypeNum()
{
	return m_nuProtoTypeNum;
}

// /************************************************************************//**
//  *	@brief		获取SVL存储的方式
//  *	@param[in]	无
//  *	@param[out]	无
//  *	@retval		m_wstrSVLStorage			存储方式
//  *	@note		无
//  *	@attention	无
//  ****************************************************************************/
// 
// wstring Stk_File::GetSVLStorage()
// {
// 	return m_wstrSVLStorage;
// }

/************************************************************************//**
 *	@brief		从SVL中读取文件头
 *	@param[in]	i_TiHeadP						文件头的记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS						读取成功
 *	@retval		STK_ERROR_MEMORY				内存申请失败
 *	@retval		STK_ERROR_VERSION_UNKNOWNFILE	读入未知版本的文件
 *	@retval		STK_ERROR_VERSION_HIGHER		读入更高的版本
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::ReadTitle(StkSegmentHead *i_TiHeadP)
{
	StkControl			**RecordPP = NULL;
	STK_UINT32			ii;
	StkTitleRec			*TitleRecP = NULL;
	StkTitleRec2		*TitleRecP2 = NULL;
	StkTitleRec3		*TitleRecP3 = NULL;
	StkTitleRec4		*TitleRecP4 = NULL;
	StkTitleRec5		*TitleRecP5 = NULL;
	StkTitleRec6		*TitleRecP6 = NULL;
	STK_DOUBLE64		dFileVer = 0.0;
	STK_DOUBLE64		dVer = 0.0;
	STK_STATUS			eState;

// 	eRet = this->ReadSegmentHead(&head);
// 	if( eRet != STK_SUCCESS){
// 		return STK_ERROR_VERSION_UNKNOWNFILE;
// 	}
	
	RecordPP = (StkControl **)calloc(i_TiHeadP->recnum, sizeof(StkControl*));
	if (i_TiHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = this->ReadSegmentAllRecordData(i_TiHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取File段的各记录
	 */
	for (ii = 0; ii < i_TiHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_FILE)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	填充File基础记录
				 */
				TitleRecP = (StkTitleRec *)RecordPP[ii];
				
#ifdef __MOBILE__
				m_wstrTitle =Stk_Platform::STKWCHARStrToWString(TitleRecP->FileTitle);
				m_wstrCADFileName = Stk_Platform::STKWCHARStrToWString(TitleRecP->FileName);
				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(TitleRecP->DataSource);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP->SVLVersion);
				//m_wstrSVLStorage = Stk_Platform::STKWCHARStrToWString(TitleRecP->SVLStorage);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP->Compression);
#else
				m_wstrTitle = TitleRecP->FileTitle;
				m_wstrCADFileName = TitleRecP->FileName;
				m_wstrDataSource = TitleRecP->DataSource;
				m_wstrConvTookit = TitleRecP->ConvTookit;
				m_wstrSVLConversion = TitleRecP->SVLConversion;
				m_wstrSVLVersion = TitleRecP->SVLVersion;
				//m_wstrSVLStorage = TitleRecP->SVLStorage;
				m_wstrCreateTime = TitleRecP->CreateTime;
				m_wstrCompression = TitleRecP->Compression;
#endif
				m_nuPolygonNum = TitleRecP->PolygonNum;
				m_nuProtoTypeNum = TitleRecP->ProtoTypeNum;
				//m_nuTopProtoID = TitleRecP->TopProtoID;
			}

			// !< 去除数据源的部分相关信息  add by Shaoqy on 2014/2/12
			else if(RecordPP[ii]->version == 2)
			{
				TitleRecP2 = (StkTitleRec2 *)RecordPP[ii];
#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString(TitleRecP2->FileTitle);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP2->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP2->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP2->SVLVersion);
				//m_wstrSVLStorage = Stk_Platform::STKWCHARStrToWString(TitleRecP2->SVLStorage);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP2->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP2->Compression);
#else
				m_wstrTitle = TitleRecP2->FileTitle;
				m_wstrConvTookit = TitleRecP2->ConvTookit;
				m_wstrSVLConversion = TitleRecP2->SVLConversion;
				m_wstrSVLVersion = TitleRecP2->SVLVersion;
				//m_wstrSVLStorage = TitleRecP2->SVLStorage;
				m_wstrCreateTime = TitleRecP2->CreateTime;
				m_wstrCompression = TitleRecP2->Compression;
#endif
				m_nuProtoTypeNum = TitleRecP2->ProtoTypeNum;
				//m_nuTopProtoID = TitleRecP2->TopProtoID;
			}
			else if (RecordPP[ii]->version == 3)
			{
				TitleRecP3 = (StkTitleRec3 *)RecordPP[ii];

#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString(TitleRecP3->FileTitle);
				m_wstrCADFileName =Stk_Platform::STKWCHARStrToWString( TitleRecP3->CADFileName);
				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(TitleRecP3->DataSource);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP3->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP3->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP3->SVLVersion);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP3->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP3->Compression);
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(TitleRecP3->CADFilePath);
#else
				m_wstrTitle = TitleRecP3->FileTitle;
				m_wstrCADFileName = TitleRecP3->CADFileName;
				m_wstrDataSource = TitleRecP3->DataSource;
				m_wstrConvTookit = TitleRecP3->ConvTookit;
				m_wstrSVLConversion = TitleRecP3->SVLConversion;
				m_wstrSVLVersion = TitleRecP3->SVLVersion;
				m_wstrCreateTime = TitleRecP3->CreateTime;
				m_wstrCompression = TitleRecP3->Compression;
				m_wstrCADPath = TitleRecP3->CADFilePath;
#endif
				m_nuProtoTypeNum = TitleRecP3->ProtoTypeNum;
				m_nuPolygonNum = TitleRecP3->PolygonNum;
			}
			else if (RecordPP[ii]->version == 4)
			{
				TitleRecP4 = (StkTitleRec4 *)RecordPP[ii];

#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString(TitleRecP4->FileTitle);
				m_wstrCADFileName =Stk_Platform::STKWCHARStrToWString( TitleRecP4->CADFileName);
				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(TitleRecP4->DataSource);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP4->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP4->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP4->SVLVersion);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP4->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP4->Compression);
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(TitleRecP4->CADFilePath);
#else
				m_wstrTitle = TitleRecP4->FileTitle;
				m_wstrCADFileName = TitleRecP4->CADFileName;
				m_wstrDataSource = TitleRecP4->DataSource;
				m_wstrConvTookit = TitleRecP4->ConvTookit;
				m_wstrSVLConversion = TitleRecP4->SVLConversion;
				m_wstrSVLVersion = TitleRecP4->SVLVersion;
				m_wstrCreateTime = TitleRecP4->CreateTime;
				m_wstrCompression = TitleRecP4->Compression;
				m_wstrCADPath = TitleRecP4->CADFilePath;
#endif
				m_nuProtoTypeNum = TitleRecP4->ProtoTypeNum;
				m_nuPolygonNum = TitleRecP4->PolygonNum;
			}
			else if (RecordPP[ii]->version == 5)
			{
				TitleRecP5 = (StkTitleRec5 *)RecordPP[ii];

#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString(TitleRecP5->FileTitle);
				m_wstrCADFileName =Stk_Platform::STKWCHARStrToWString(TitleRecP5->CADFileName);
				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(TitleRecP5->DataSource);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP5->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP5->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP5->SVLVersion);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP5->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP5->Compression);
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(TitleRecP5->CADFilePath);
#else
				m_wstrTitle = TitleRecP5->FileTitle;
				m_wstrCADFileName = TitleRecP5->CADFileName;
				m_wstrDataSource = TitleRecP5->DataSource;
				m_wstrConvTookit = TitleRecP5->ConvTookit;
				m_wstrSVLConversion = TitleRecP5->SVLConversion;
				m_wstrSVLVersion = TitleRecP5->SVLVersion;
				m_wstrCreateTime = TitleRecP5->CreateTime;
				m_wstrCompression = TitleRecP5->Compression;
				m_wstrCADPath = TitleRecP5->CADFilePath;
#endif
				m_nuProtoTypeNum = TitleRecP5->ProtoTypeNum;
				m_nuPolygonNum = TitleRecP5->PolygonNum;
				if (m_wstrCompression == L"LZ4")
				{
					m_eCompressType = TYPE_COMPRESS_SPEED;
				}
				else if (m_wstrCompression == L"7Z")
				{
					m_eCompressType = TYPE_COMPRESS_RATIO;
				}
				else if (m_wstrCompression == L"")
				{
					m_eCompressType = TYPE_COMPRESS_NO;
				}
			}
			else if (RecordPP[ii]->version == 6)
			{
				TitleRecP6 = (StkTitleRec6 *)RecordPP[ii];

#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString(TitleRecP6->FileTitle);
				m_wstrCADFileName =Stk_Platform::STKWCHARStrToWString(TitleRecP6->CADFileName);
				m_wstrDataSource = Stk_Platform::STKWCHARStrToWString(TitleRecP6->DataSource);
				m_wstrConvTookit = Stk_Platform::STKWCHARStrToWString(TitleRecP6->ConvTookit);
				m_wstrSVLConversion = Stk_Platform::STKWCHARStrToWString(TitleRecP6->SVLConversion);
				m_wstrSVLVersion = Stk_Platform::STKWCHARStrToWString(TitleRecP6->SVLVersion);
				m_wstrCreateTime = Stk_Platform::STKWCHARStrToWString(TitleRecP6->CreateTime);
				m_wstrCompression = Stk_Platform::STKWCHARStrToWString(TitleRecP6->Compression);
				m_wstrCADPath = Stk_Platform::STKWCHARStrToWString(TitleRecP6->CADFilePath);
				m_wstrPassword = Stk_Platform::STKWCHARStrToWString(TitleRecP6->Password);
#else
				m_wstrTitle = TitleRecP6->FileTitle;
				m_wstrCADFileName = TitleRecP6->CADFileName;
				m_wstrDataSource = TitleRecP6->DataSource;
				m_wstrConvTookit = TitleRecP6->ConvTookit;
				m_wstrSVLConversion = TitleRecP6->SVLConversion;
				m_wstrSVLVersion = TitleRecP6->SVLVersion;
				m_wstrCreateTime = TitleRecP6->CreateTime;
				m_wstrCompression = TitleRecP6->Compression;
				m_wstrCADPath = TitleRecP6->CADFilePath;
				m_wstrPassword = TitleRecP6->Password;
#endif
				m_nuProtoTypeNum = TitleRecP6->ProtoTypeNum;
				m_nuPolygonNum = TitleRecP6->PolygonNum;
				if (m_wstrCompression == L"LZ4")
				{
					m_eCompressType = TYPE_COMPRESS_SPEED;
				}
				else if (m_wstrCompression == L"7Z")
				{
					m_eCompressType = TYPE_COMPRESS_RATIO;
				}
				else if (m_wstrCompression == L"")
				{
					m_eCompressType = TYPE_COMPRESS_NO;
				}
			}
			else
			{
				// 读入更高的版本
				return STK_ERROR_VERSION_HIGHER;
			}
		}
		else
		{
			//不做任何操作
		}
	}

	// 释放内存
	this->ReadSegmentFinishData(i_TiHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

// ###
//	此处进行版本检查
	if (m_wstrTitle != SVL_FILE_TITLE)
	{
		return STK_ERROR_VERSION_UNKNOWNFILE;
	}

	if (m_wstrSVLVersion == L"")
	{
		return STK_ERROR_VERSION_UNKNOWNFILE;
	}

	dFileVer = wcstod(m_wstrSVLVersion.c_str(),NULL);
	dVer = wcstod(SVL_DATAMANAGER_VERSION,NULL);
	if (dFileVer > dVer)
	{
		// 读入更高的版本
		return STK_ERROR_VERSION_HIGHER;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		向SVL中写入文件头
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写入成功
 *	@retval		STK_ERROR				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteTitle()
{
	StkSegmentHead	head = {0};
	StkTitleRec6	titleRec6 = {0};
	STK_BOOL		bIsOldVersion = false;
	STK_STATUS		ret = STK_SUCCESS;
#ifdef _WINDOWS_	
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	/*
	 *	【第一步】 输出Title段
	 */

	// 【第1.1步】输出Title HEAD Record
	head.id = 0;
	head.kind = SEG_TYPE_TITLE;
	
	// 为兼容V2.2之前版本，在保存时，将V2.2之前的版本强制更改为V2.2的版本。
	// 这样可以保证在读取时，按照V2.2的逻辑进行读取
	//bIsOldVersion = Stk_Utility::IsOldVersion(m_wstrSVLVersion);
	//if (bIsOldVersion)
	//{
		m_wstrSVLVersion = SVL_DATAMANAGER_VERSION;
	//}

	switch(m_eCompressType)
	{
	case TYPE_COMPRESS_NO:
		m_wstrCompression = L"";
		break;
	case TYPE_COMPRESS_SPEED:
		m_wstrCompression = L"LZ4";
		break;
	case TYPE_COMPRESS_RATIO:
		m_wstrCompression = L"7Z";
		break;
	}

#ifdef _WINDOWS_
	stkSVLLog = GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = GetDocumentManager()->GetSVLLogFile();
	if (1 == stkSVLLog.bLogOutput)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【Title段】版本：%s"), m_wstrSVLVersion.c_str());
		stkSVLLogFile->Log(strConvertLog);
	}

#endif

	ret = WriteSegmentHead(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Title段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	// 【第1.2步】输出Title Record
	titleRec6.Control.type = REC_TYPE_FILE;
	titleRec6.Control.version = _VER_TITLE_RECORD /*6*/;
	titleRec6.Control.length = sizeof(StkTitleRec6) - sizeof(StkControl);
#ifdef __MOBILE__
	memcpy(titleRec6.FileTitle,Stk_Platform::WStringToSTKWCHARStr(m_wstrTitle),(m_wstrTitle.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.CADFileName,Stk_Platform::WStringToSTKWCHARStr(m_wstrCADFileName),(m_wstrCADFileName.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.DataSource,Stk_Platform::WStringToSTKWCHARStr(m_wstrDataSource),(m_wstrDataSource.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.ConvTookit,Stk_Platform::WStringToSTKWCHARStr(m_wstrConvTookit),(m_wstrConvTookit.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.SVLConversion,Stk_Platform::WStringToSTKWCHARStr(m_wstrSVLConversion),(m_wstrSVLConversion.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.SVLVersion,Stk_Platform::WStringToSTKWCHARStr(m_wstrSVLVersion),(m_wstrSVLVersion.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.CreateTime,Stk_Platform::WStringToSTKWCHARStr(m_wstrCreateTime),(m_wstrCreateTime.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.Compression,Stk_Platform::WStringToSTKWCHARStr(m_wstrCompression),(m_wstrCompression.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.CADFilePath, Stk_Platform::WStringToSTKWCHARStr(m_wstrCADPath), (m_wstrCADPath.size()+1)*sizeof(STK_WCHAR));
	memcpy(titleRec6.Password, Stk_Platform::WStringToSTKWCHARStr(m_wstrPassword), (m_wstrPassword.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(titleRec6.FileTitle, STK_MAXNAME, (WCHAR*)m_wstrTitle.c_str());
	wcscpy_s(titleRec6.CADFileName, STK_ULTRANAME, (WCHAR*)m_wstrCADFileName.c_str());
	wcscpy_s(titleRec6.DataSource, STK_ULTRANAME, (WCHAR*)m_wstrDataSource.c_str());
	wcscpy_s(titleRec6.ConvTookit, STK_MAXNAME, (WCHAR*)m_wstrConvTookit.c_str());
	wcscpy_s(titleRec6.SVLConversion, STK_MAXNAME, (WCHAR*)m_wstrSVLConversion.c_str());
	wcscpy_s(titleRec6.SVLVersion, STK_MAXNAME, (WCHAR*)m_wstrSVLVersion.c_str());
	wcscpy_s(titleRec6.CreateTime, STK_MAXNAME, (WCHAR*)m_wstrCreateTime.c_str());
	wcscpy_s(titleRec6.Compression, STK_MAXNAME, (WCHAR*)m_wstrCompression.c_str());
	wcscpy_s(titleRec6.CADFilePath, STK_MAXPATH, (WCHAR*)m_wstrCADPath.c_str());
	wcscpy_s(titleRec6.Password, STK_MAXPATH, (WCHAR*)m_wstrPassword.c_str());
#endif
	titleRec6.ProtoTypeNum = m_nuProtoTypeNum;
	titleRec6.PolygonNum = m_nuPolygonNum;

	switch(m_eCompressType)
	{
	case TYPE_COMPRESS_NO:
		titleRec6.nuCompressLevel = LEVEL_COMPRESS_UNKNOWN;
		break;
	case TYPE_COMPRESS_SPEED:
		titleRec6.nuCompressLevel = LZ4_LEVEL_COMPRESS_HIGH;
		break;
	case TYPE_COMPRESS_RATIO:
		titleRec6.nuCompressLevel = Zip7_LEVEL_COMPRESS_DEFAULT;
		break;
	}

	titleRec6.usetype = 0;
	titleRec6.dmy1 = 0;
	titleRec6.dmy2 = 0;

	ret = WriteSegmentRecord(&head, (StkControl*)&titleRec6);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Title Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	/*
	 *	【1.3 完成Title段的输出】
	 */
	ret = WriteSegmentFinish(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Title段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		从SVL中读取文件配置项
 *	@param[in]	i_OptHeadP						文件配置项的记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS						读取成功
 *	@retval		STK_ERROR_MEMORY				内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::ReadOption(StkSegmentHead *i_OptHeadP)
{
	StkControl			**RecordPP = NULL;
	STK_UINT32			ii,jj;
	STK_UINT32			nuStrIdx =0;
	wstring				wstrTmp;
	StkOptionRec		*OptionRecP = NULL;
	STK_STATUS			eState;


	RecordPP = (StkControl **)calloc(i_OptHeadP->recnum, sizeof(StkControl*));
	if (i_OptHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = this->ReadSegmentAllRecordData(i_OptHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取Option段的各记录
	 */
	for (ii = 0; ii < i_OptHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_OPTION)
		{
			/*
			 * 读取Option记录
			 */
			if (RecordPP[ii]->version == 1)
			{
				OptionRecP = (StkOptionRec *)RecordPP[ii];
				for (jj = 0;jj < OptionRecP->Optnum; jj++)
				{
					wstrTmp.clear();
#ifdef __MOBILE__
					wstrTmp = Stk_Platform::STKWCHARStrToWString((STK_WCHAR*)OptionRecP->Optstring[jj]);
#else
					wstrTmp = OptionRecP->Optstring[jj];
#endif

					nuStrIdx = wstrTmp.find(L"=", 0);
					m_mapOptStrings.insert(pair<wstring,wstring>
						(wstrTmp.substr(0,nuStrIdx),wstrTmp.substr(nuStrIdx+1)));
				}
			}
		}
		else
		{
			//不做任何操作
		}
	}

	// 释放内存
	this->ReadSegmentFinishData(i_OptHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		向SVL中写入配置项
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写入成功
 *	@retval		STK_ERROR				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteOption()
{
	StkSegmentHead	head = {0};
	StkOptionRec	*optionRecP = NULL;
	STK_UINT32		ret = 0;
	STK_UINT32		optNum = 0;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	optNum = m_mapOptStrings.size();
#ifdef _WINDOWS_
	stkSVLLog = GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = GetDocumentManager()->GetSVLLogFile();
#endif
	if(optNum == 0)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog,  _countof(strConvertLog), 
				_T("【Option段】Option：%s"), _T("未设置配置项"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_SUCCESS;		// 未设置配置项
	}

	/*
	 *	【第一步】 输出Option段
	 */

	// 【第1.1步】输出Option HEAD Record
	head.id = 0;
	head.kind = SEG_TYPE_OPTION;
	ret = WriteSegmentHeadData(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Option段-段头写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	// 【第1.2步】输出Option Record
	optionRecP = (StkOptionRec*)calloc(1, sizeof(StkOptionRec) - sizeof(STK_WCHAR)*STK_MAXNAME + optNum * sizeof(STK_WCHAR) * STK_MAXNAME);
	optionRecP->Control.type = REC_TYPE_OPTION;
	optionRecP->Control.version = _VER_OPTION_RECORD;
	optionRecP->Control.length = sizeof(StkOptionRec) - sizeof(STK_WCHAR) * STK_MAXNAME + optNum * sizeof(STK_WCHAR) * STK_MAXNAME - sizeof(StkControl);
	optionRecP->Optnum = optNum;
	
	map<wstring, wstring>::iterator itor = m_mapOptStrings.begin();
	wstring tmpstring;
	STK_UINT32 ii = 0;
	while(itor != m_mapOptStrings.end()){
		tmpstring.clear();
		tmpstring = itor->first;
		tmpstring += L"=";
		tmpstring += itor->second;
#ifdef __MOBILE__
		memcpy(optionRecP->Optstring[ii++],Stk_Platform::WStringToSTKWCHARStr(tmpstring),(tmpstring.size()+1)*sizeof(STK_WCHAR));
#else
		wcscpy_s(optionRecP->Optstring[ii++], STK_MAXNAME, (WCHAR*)tmpstring.c_str());
#endif
		itor++;
	}

#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【Option段】Option：%s"), optionRecP->Optstring[ii++]);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	ret = WriteSegmentRecordData(&head, (StkControl*)optionRecP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Option段Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	/*
	 *	【1.3 完成Option段的输出】
	 */
	ret = WriteSegmentFinishData(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Option段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置索引与ProtoType之间的关联
 *	@param[in]	inKey					索引号
 *	@param[in]	inProtoTypeP			要关联的ProtoType
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		STK_ERROR				设置失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::RegProtoTypeKey(wstring inKey, Stk_ProtoType *inProtoTypeP)
{
	if( inKey == L"" || inProtoTypeP == NULL)
	{
		return STK_ERROR;
	}

	m_ProtoKeyMap.insert(pair<wstring,Stk_ProtoType*>( inKey, inProtoTypeP));

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取指定的ProtoType
 *	@param[in]	inKey					索引号
 *	@param[out]	无
 *	@retval		ProtoTypeP				指定的ProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_File::GetProtoTypeByKey(wstring inKey)
{
	Stk_ProtoType *ProtoTypeP = NULL;
	map<wstring,Stk_ProtoType*>::iterator itor;

	if( inKey == L"" )
	{
		ProtoTypeP = NULL;
	}

	itor = m_ProtoKeyMap.find(inKey);

	if( itor != m_ProtoKeyMap.end()){
		ProtoTypeP = itor->second;
	}
	else{
		ProtoTypeP = NULL;
	}

	return ProtoTypeP;
}

/*****************************************************************************
 *	@brief		获取顶级Proto的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuTopProtoID			顶级Proto的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_ID Stk_File::GetTopProtoID()
//{
//	return m_nuTopProtoID;
//}

/*****************************************************************************
 *	@brief		设置顶级Proto的ID
 *	@param[in]	i_ID			顶级Proto的ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_STATUS Stk_File::SetTopProtoID(STK_ID i_ID)
//{
//	m_nuTopProtoID = i_ID;
//	return STK_SUCCESS;
//}

/*****************************************************************************
 *	@brief		获取顶级Proto
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_stkTopProtoP			顶级Proto
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//Stk_ProtoType* Stk_File::GetTopProtoType()
//{
//	return m_stkTopProtoP;
//}

/*****************************************************************************
 *	@brief		设置顶级Proto
 *	@param[in]	i_stkProtoP				顶级Proto
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		STK_ERROR				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_STATUS Stk_File::SetTopProtoType(Stk_ProtoType* i_stkProtoP)
//{
//// 	if (i_stkProtoP != NULL)
//// 	{
//		m_stkTopProtoP = i_stkProtoP;
//		return STK_SUCCESS;
//// 	}
//// 	else
//// 		return STK_ERROR;
//}

/*************************************************************************//**
 *	@brief		获取是否将SVL中的SubLine转换为LineSet
 *	@param[in]	无
 *	@param[out]	o_bflag					是否读取
 *	@param[out]	o_fTol					公差
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

VOID Stk_File::GetLineSetConvertFlag(STK_BOOL &o_bflag, STK_FLOAT32 &o_fTol)
{
	o_bflag = m_bLineSetConvert;
	o_fTol = m_fLineSetTol;
}

/*************************************************************************//**
 *	@brief		设置是否将SVL中的SubLine转换为LineSet
 *	@param[in]	i_bflag					是否读取
 *	@param[in]	i_fTol					公差
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

VOID Stk_File::SetLineSetConvertFlag(STK_BOOL i_bflag, STK_FLOAT32 i_fTol)
{
	m_bLineSetConvert = i_bflag;
	m_fLineSetTol = i_fTol;
}

/*************************************************************************//**
 *	@brief		获取DocumentManager
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_DocManager	DocumentManager
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_DocumentManager* Stk_File::GetDocumentManager()
{
	return m_DocManager;
}

/*************************************************************************//**
 *	@brief		添加顶级ProtoType
 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_File::AddTopProtoType(Stk_ProtoType* i_stkProtoTypeP)
{
	if (i_stkProtoTypeP != NULL)
	{
		m_vcTopProtoTypes.push_back(i_stkProtoTypeP);
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取当前File文件的顶级ProtoType数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		顶级ProtoType数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_File::GetTopProtoTypeNum()
{
	return m_vcTopProtoTypes.size();
}

/*************************************************************************//**
 *	@brief		获取指定索引对应的顶级ProtoType
 *	@param[in]	i_nuIndex			指定的索引号
 *	@param[out]	无
 *	@retval		指定的顶级ProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_File::GetTopProtoTypeByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcTopProtoTypes.at(i_nuIndex);
}

/*************************************************************************//**
 *	@brief		从File文件中删除指定的Top ProtoType
 *	@param[in]	i_stkProtoTypeP		指定的ProtoType
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		一个File中有多个ProtoType时，除了是零件族之外，还有可能是CGR
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::DelTopProtoTypeFromFile(Stk_ProtoType* i_stkProtoTypeP)
{
	vector<Stk_ProtoType*>::iterator itor = m_vcTopProtoTypes.begin();
	while(itor != m_vcTopProtoTypes.end())
	{
		if ((*itor) == i_stkProtoTypeP)
		{
			delete (*itor);
			m_vcTopProtoTypes.erase(itor);
			return STK_SUCCESS;
		}
		itor++;
	}
	return STK_ERROR_MEMORY;
}

/*************************************************************************//**
 *	@brief		设置CADFile物理文件路径
 *	@param[in]	i_wstrCADPath		CAD物理文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		调用该方法会自动计算CADFile名称并通过SetCADFileName设置到Stk_File中
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_File::SetCADFilePath(wstring i_wstrCADPath)
{
	wstring		wstrFileName = L"";
	wstring		wstrFileDir = L"";
	wstrFileName = ComputeCADFileName(i_wstrCADPath);
	m_wstrCADFileName = Stk_Utility::ReplaceImproperCode(wstrFileName);
	wstrFileDir = i_wstrCADPath.substr(0, i_wstrCADPath.size() - wstrFileName.size());
	m_wstrCADPath = (wstrFileDir + m_wstrCADFileName);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取CAD物理文件路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrCADPath		CAD物理文件路径
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

wstring Stk_File::GetCADFilePath()
{
	return m_wstrCADPath;
}

/************************************************************************//**
 *	@brief		根据输入的CADFile路径计算CADFile名称
 *	@param[in]	i_wstrCADPath	CADFile路径
 *	@param[out]	无
 *	@retval		wstrSVLName		CADFile文件名称
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

wstring Stk_File::ComputeCADFileName(wstring i_wstrCADPath)
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

#if 0
	// 去除ProE的数字版本
	nuPos = wstrSVLName.find(L".asm.");
	if (nuPos != -1)
	{
		wstrSVLName = wstrSVLName.substr( 0, nuPos);
		wstrSVLName += L".asm";
	}
	nuPos = wstrSVLName.find(L".prt.");
	if (nuPos != -1)
	{
		wstrSVLName = wstrSVLName.substr( 0, nuPos);
		wstrSVLName += L".prt";
	}
#endif

	return wstrSVLName;
}

/************************************************************************//**
 *	@brief		判断File文件是否是顶级
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bIsTop		是否是顶级文件标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_File::IsTopFile()
{
	return m_bIsTop;
}

/************************************************************************//**
 *	@brief		设置是否是顶级File文件标识
 *	@param[in]	i_bIsTop		是否是顶级File
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_File::SetTopFile(STK_BOOL i_bIsTop)
{
	m_bIsTop = i_bIsTop;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		预先读取SVL的版本号
 *	@param[in]	i_strSVLPath	SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::PreOpenFile(wstring i_strSVLPath)
{
	wstring			wstrCurLink;
	string			strTmpPath;
	StkSegmentHead	head ={0};
	STK_STATUS		eState = STK_SUCCESS;

	/*
	 *  打开SVL文件
	 */
	strTmpPath = Stk_Platform::WStringToString(i_strSVLPath );
	m_wstrSVLPath = i_strSVLPath;

	m_BinFileP = fopen(strTmpPath.c_str(),"rb");
	if (m_BinFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *  读取文件的Title
	 */
	while(1)
	{
		eState = this->ReadSegmentHeadData(&head);
		// 发现未知文件时的跳出处理
		if (eState != STK_SUCCESS)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}
		if (head.Control.type != REC_TYPE_HEAD || head.Control.version != 1)
		{
			eState = STK_ERROR_VERSION_UNKNOWNFILE;
			goto pt1;
		}

		switch( head.kind )
		{
		case SEG_TYPE_TITLE:
			eState = this->ReadTitle(&head);
			if (eState != STK_SUCCESS)
			{
				goto pt1;
			}
			goto pt1;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = this->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = this->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
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
	 *  关闭SVL文件
	 */
	if (m_BinFileP)
	{
		int tt = fclose(m_BinFileP);
		m_BinFileP = NULL;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		获取文件标志
 *	@param[in]	i_wstrTitle		文件标志
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetTitle(wstring i_wstrTitle)
{
	m_wstrTitle = i_wstrTitle;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文件标志
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrTitle		文件标志
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_File::GetTitle()
{
	return m_wstrTitle;
}

/************************************************************************//**
 *	@brief		设置文件版本
 *	@param[in]	i_bOldVersion	版本标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetOldVersion(STK_BOOL i_bOldVersion)
{
	m_bIsOldVersion = i_bOldVersion;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否是2.2之前的旧版本
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		版本标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_File::IsOldVersion()
{
	return m_bIsOldVersion;
}

/************************************************************************//**
 *	@brief		设置SVL文件另存路径
 *	@param[in]	i_wstrSVLSaveAsPath
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetSVLSaveAsPath(wstring i_wstrSVLSaveAsPath)
{
	m_wstrSVLSaveAsPath = i_wstrSVLSaveAsPath;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取SVL文件另存路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrSVLSaveAsPath	SVL文件另存路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_File::GetSVLSaveAsPath()
{
	return m_wstrSVLSaveAsPath;
}

/************************************************************************//**
 *	@brief		设置压缩方案（压缩比/效率优先）
 *	@param[in]	i_eCompressType	压缩方案
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_File::SetCompressType(StkCompressTypeEnum i_eCompressType)
{
	m_eCompressType = i_eCompressType;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取压缩方案
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		压缩方案
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkCompressTypeEnum Stk_File::GetCompressType()
{
	return m_eCompressType;
}

/************************************************************************//**
 *	@brief		设置段缓冲区指针
 *	@param[in]	i_chSegBuffP	段缓冲区指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::SetSegBuffer(STK_CHAR* i_chSegBuffP)
{
	m_chSegBuffP = i_chSegBuffP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取段缓冲区指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		段缓冲区指针
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_CHAR* Stk_File::GetSegBuffer()
{
	return m_chSegBuffP;
}

/************************************************************************//**
 *	@brief		清空段缓冲区
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		清空成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ClearSegBuffer()
{
	if (m_chSegBuffP != NULL)
	{
		free(m_chSegBuffP);
		m_chSegBuffP = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		添加压缩段数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::AddCompressSegmentNum()
{
	m_nuCompressSegmentNum++;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取压缩段数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		压缩段包含的记录数
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_UINT32 Stk_File::GetCompressSegmentNum()
{
	return m_nuCompressSegmentNum;
}

/************************************************************************//**
 *	@brief		清空压缩段数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS		清空成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ClearCompressSegmentNum()
{
	m_nuCompressSegmentNum = 0;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		解压数据
 *	@param[in]	i_CompressRecordP		待解压记录的指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS				解压成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::DeCompressData(StkCompressRec* i_CompressRecordP)
{
	StkControl*			DeCompressRecordP = NULL;	// 解压记录,中间变量
	STK_CHAR*			chDeCompressStreamP = NULL;	// 解压后的流
	STK_INT32			nDeCompressBuffSize = 0;	// 解压后的Buff大小

	switch(m_eCompressType)
	{
	case TYPE_COMPRESS_SPEED:
		nDeCompressBuffSize = Stk_Utility::LZ4_DeCompressRecord(i_CompressRecordP->atr, i_CompressRecordP->nuStreamSize, i_CompressRecordP->nuBufferSize, chDeCompressStreamP);
		if (nDeCompressBuffSize < 0 || nDeCompressBuffSize != i_CompressRecordP->nuBufferSize || chDeCompressStreamP == NULL)
		{
			return STK_ERROR;
		}
		break;
	case TYPE_COMPRESS_RATIO:
		nDeCompressBuffSize = Stk_Utility::Zip7_DeCompressRecord(i_CompressRecordP->atr, i_CompressRecordP->nuStreamSize, i_CompressRecordP->nuBufferSize, chDeCompressStreamP);
		if (nDeCompressBuffSize < 0 || nDeCompressBuffSize != i_CompressRecordP->nuBufferSize || chDeCompressStreamP == NULL)
		{
			return STK_ERROR;
		}
		break;
	default:
		break;
	}
	m_nuCompressSegmentNum = i_CompressRecordP->nuSegmentNum;
	m_chBufferItorP = chDeCompressStreamP;
	m_chSegBuffP = chDeCompressStreamP;	// 该赋值用于对解压时申请的内存进行释放

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		压缩数据
 *	@param[in]	i_headP					段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS				解压成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::CompressData(StkSegmentHead* i_stkHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkCompressRec*			CompressRecP = NULL;
	STK_CHAR*				chSegBuffP = NULL;
	STK_CHAR*				chStreamP = NULL;
	STK_UINT32				nuRecordBuffSize = 0;
	STK_INT32				nStreamSize = 0;
	StkCompressLevelEnum	eCompressLevel = LEVEL_COMPRESS_UNKNOWN;
	StkCompressTypeEnum		eCompressType = TYPE_COMPRESS_NO;

	chSegBuffP = GetSegBuffer();
	if (chSegBuffP == NULL)
	{
		return STK_ERROR;
	}

	eCompressType = GetCompressType();
	switch(eCompressType)
	{
	case TYPE_COMPRESS_NO:
		break;
		// 速度优先压缩算法：LZ4
	case TYPE_COMPRESS_SPEED:
		eCompressLevel = LZ4_LEVEL_COMPRESS_HIGH;
		nStreamSize = Stk_Utility::LZ4_CompressRecord((STK_CHAR*)chSegBuffP, m_nuHaveUseBlock, eCompressLevel, chStreamP);
		if (nStreamSize < 0 || chStreamP == NULL)
		{
			return STK_ERROR;
		}
		break;
		// 压缩比优先压缩算法：7Z
	case TYPE_COMPRESS_RATIO:
		eCompressLevel = Zip7_LEVEL_COMPRESS_DEFAULT;
		nStreamSize = Stk_Utility::Zip7_CompressRecord((STK_CHAR*)chSegBuffP, m_nuHaveUseBlock, eCompressLevel, chStreamP);
		if (nStreamSize < 0 || chStreamP == NULL)
		{
			return STK_ERROR;
		}
		break;
	}

	CompressRecP = (StkCompressRec*)calloc(1, sizeof(StkCompressRec) - sizeof(STK_CHAR) + nStreamSize * sizeof(STK_CHAR));
	CompressRecP->Control.type = REC_TYPE_COMPRESS;
	CompressRecP->Control.length = sizeof(StkCompressRec) - sizeof(STK_CHAR) + nStreamSize * sizeof(STK_CHAR) - sizeof(StkControl);
	CompressRecP->Control.version = _VER_COMPRESS_RECORD;
	CompressRecP->nuSegmentNum = GetCompressSegmentNum();
	CompressRecP->nuBufferSize = m_nuHaveUseBlock;
	CompressRecP->nuStreamSize = nStreamSize;
	memcpy(CompressRecP->atr, chStreamP, nStreamSize);
	if (chStreamP != NULL)
	{
		free(chStreamP);
		chStreamP = NULL;
	}
	ClearSegBuffer();
	chSegBuffP = NULL;
	ClearCompressSegmentNum();
	eState = WriteSegmentRecord(i_stkHeadP, (StkControl*)CompressRecP);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	if (CompressRecP != NULL)
	{
		free(CompressRecP);
		CompressRecP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否压缩标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		是否压缩标识
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_BOOL Stk_File::IsCompressFile()
{
	if (m_eCompressType == TYPE_COMPRESS_NO)
	{
		m_bIsCompress = false;
	}
	else
	{
		m_bIsCompress = true;
		
	}
	return m_bIsCompress;
}

/************************************************************************//**
 *	@brief		用于压缩记录的缓冲区申请
 *	@param[in]	i_HeadSP				段头指针
 *	@param[in]	i_RecordP				待压缩记录
 *	@param[out]	无
 *	@retval		STK_SUCCESS				申请成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ApplySegmentRecordBuffer(StkSegmentHead *i_headP, StkControl* i_RecordP)
{
	STK_CHAR*			chSegBuffP = NULL;
	STK_UINT32			nuRecordBuffSize = 0;
	STK_UINT32			nuSegBuffSize = 0;

	nuRecordBuffSize = i_RecordP->length + sizeof(StkControl);
	chSegBuffP = GetSegBuffer();
	if (chSegBuffP == NULL)
	{
		chSegBuffP = (STK_CHAR*)calloc(1, MEMBLOCK);
		if (chSegBuffP == NULL)
		{
			return STK_ERROR_MEMORY;
		}
		m_nuHaveAllocBlock += MEMBLOCK;
	}
	else
	{
		if ((m_nuHaveUseBlock + nuRecordBuffSize) > m_nuHaveAllocBlock)
		{
			chSegBuffP = (STK_CHAR*)realloc(chSegBuffP,  m_nuHaveAllocBlock + MEMBLOCK + nuRecordBuffSize );
			if (chSegBuffP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			m_nuHaveAllocBlock = m_nuHaveAllocBlock + MEMBLOCK + nuRecordBuffSize;
		}
	}
	memcpy(chSegBuffP + m_nuHaveUseBlock, (VOID*)i_RecordP, nuRecordBuffSize);
	SetSegBuffer(chSegBuffP);

	i_headP->recnum ++;
	i_headP->datlen += nuRecordBuffSize;

	m_nuHaveUseBlock += nuRecordBuffSize;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		用于压缩段头的缓冲区申请
 *	@param[in]	i_HeadP					待压缩段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS				申请成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ApplySegmentHeadBuffer(StkSegmentHead* i_HeadP)
{
	STK_CHAR*			chSegBuffP = NULL;
	STK_UINT32			nuHeadBuffSize = 0;
	STK_UINT32			nuSegBuffSize = 0;

	i_HeadP->Control.type = REC_TYPE_HEAD;
	i_HeadP->Control.length = HEADLEN - RECCTLLEN;
	i_HeadP->Control.version = _VER_HEAD_RECORD;
	i_HeadP->recnum = 0;
	i_HeadP->datlen = 0;

	nuHeadBuffSize = HEADLEN;
	chSegBuffP = GetSegBuffer();
	if (chSegBuffP == NULL)
	{
		chSegBuffP = (STK_CHAR*)calloc(1, MEMBLOCK);
		if (chSegBuffP == NULL)
		{
			return STK_ERROR_MEMORY;
		}
		m_nuHaveAllocBlock += MEMBLOCK;
	}
	else
	{
		if ((m_nuHaveUseBlock + nuHeadBuffSize) > m_nuHaveAllocBlock)
		{
			chSegBuffP = (STK_CHAR*)realloc(chSegBuffP, m_nuHaveAllocBlock + MEMBLOCK );
			if (chSegBuffP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			m_nuHaveAllocBlock += MEMBLOCK;
		}
	}
	memcpy(chSegBuffP + m_nuHaveUseBlock, (VOID*)i_HeadP, nuHeadBuffSize);
	SetSegBuffer(chSegBuffP);

	m_nuHaveUseBlock += nuHeadBuffSize;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		用于压缩段头的缓冲区申请
 *	@param[in]	i_HeadP					待压缩段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS				申请成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ApplySegmentFinishBuffer(StkSegmentHead* i_HeadP)
{
	STK_CHAR*			chSegBuffP = NULL;
	chSegBuffP = GetSegBuffer();
	// 游标计算
	m_chBufferItorP = chSegBuffP + m_nuHaveUseBlock - i_HeadP->datlen - sizeof(STK_UINT32)*2;

	memcpy(m_chBufferItorP, (STK_UINT32*)&i_HeadP->recnum, sizeof(STK_UINT32));
	m_chBufferItorP += sizeof(STK_UINT32);
	memcpy(m_chBufferItorP, (STK_UINT32*)&i_HeadP->datlen, sizeof(STK_UINT32));

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读取段头Buffer数据
 *	@param[in]	i_HeadP				段头指针
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_HEADRD	读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentHeadBuffer(StkSegmentHead *i_HeadP)
{
	STK_STATUS	eState = STK_SUCCESS;

	memcpy(i_HeadP, m_chBufferItorP, HEADLEN);
	m_chBufferItorP += HEADLEN;
	
	return eState;
}

/************************************************************************//**
 *	@brief		读取段Buffer中所有记录
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentAllRecordBuffer(StkSegmentHead *i_HeadP, StkControl *i_RecordA[])
{
	STK_STATUS	eState = STK_SUCCESS;
	StkControl	tmpControl={0};
	STK_CHAR	*chTmpP;
	STK_UINT32	ii;
	STK_SIZE_T	cnt = 0;

	for (ii = 0; ii < i_HeadP->recnum; ii++)
	{
		memcpy(&tmpControl, m_chBufferItorP, RECCTLLEN);
		m_chBufferItorP += RECCTLLEN;

		if ((chTmpP = (STK_CHAR *)calloc(tmpControl.length + RECCTLLEN, sizeof(STK_CHAR))) == NULL)
		{
			eState = STK_ERROR_FILE_MEMORY;
			goto pgend;
		}
		memcpy(chTmpP, &tmpControl, RECCTLLEN);
		memcpy(chTmpP + RECCTLLEN, m_chBufferItorP, tmpControl.length);
		m_chBufferItorP += tmpControl.length;

		i_RecordA[ii] = (StkControl*)chTmpP;
	}

pgend:
	return eState;
}

/************************************************************************//**
 *	@brief		段Buffer读取结束
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentFinishBuffer(StkSegmentHead *i_HeadP, StkControl *i_RecordA[])
{
	STK_STATUS	eRet;
	STK_UINT32	ii;

	if (i_HeadP->recnum != 0 && i_RecordA == NULL)
	{
		eRet = STK_ERROR_FILE_RECORD;
		goto pgend;
	}

	for (ii=0; ii < i_HeadP->recnum; ii++)
	{
		if (i_RecordA[ii] != NULL)
		{
			free(i_RecordA[ii]);
			i_RecordA[ii] = NULL;
		}
	}

	eRet = STK_SUCCESS;

pgend:
	return(eRet);
}

/************************************************************************//**
 *	@brief		写段记录数据
 *	@param[in]	i_HeadSP				段头指针
 *	@param[in]	i_RecordP				段记录
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentRecordData(StkSegmentHead *i_HeadSP, StkControl* i_RecordP)
{
	STK_STATUS	eState = STK_SUCCESS;
	STK_BOOL	bIsCompress = false;
	bIsCompress = IsCompressFile();
	if (bIsCompress)
	{
		eState = ApplySegmentRecordBuffer(i_HeadSP, i_RecordP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}
	else
	{
		eState = WriteSegmentRecord(i_HeadSP, i_RecordP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	return eState;
}

/************************************************************************//**
 *	@brief		写段头数据
 *	@param[in]	i_HeadP					段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentHeadData(StkSegmentHead* i_HeadP)
{
	STK_STATUS	eState = STK_SUCCESS;
	STK_BOOL	bIsCompress = false;
	bIsCompress = IsCompressFile();
	if (bIsCompress)
	{
		eState = ApplySegmentHeadBuffer(i_HeadP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		AddCompressSegmentNum();
	}
	else
	{
		eState = WriteSegmentHead(i_HeadP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	return eState;
}

/************************************************************************//**
 *	@brief		写段的结束记录数据
 *	@param[in]	i_HeadP					段的结束记录
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteSegmentFinishData(StkSegmentHead* i_HeadP)
{
	STK_STATUS	eState = STK_SUCCESS;
	STK_BOOL	bIsCompress = false;
	bIsCompress = IsCompressFile();
	if (bIsCompress)
	{
		eState = ApplySegmentFinishBuffer(i_HeadP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}
	else
	{
		eState = WriteSegmentFinish(i_HeadP);
		if (eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取段头Buffer数据
 *	@param[in]	i_HeadP				段头指针
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_HEADRD	读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentHeadData(StkSegmentHead *i_HeadP)
{
	STK_STATUS	eState = STK_SUCCESS;

	if (m_nuReadCompressSegCount >= m_nuCompressSegmentNum)
	{
		m_bIsCompress = false;
	}

	if (m_bIsCompress)
	{
		eState = ReadSegmentHeadBuffer(i_HeadP);
		
		m_nuReadCompressSegCount++;
	}
	else
	{
		eState = ReadSegmentHead(i_HeadP);
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取段Buffer中所有记录
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentAllRecordData(StkSegmentHead *i_HeadP, StkControl *i_RecordA[])
{
	STK_STATUS	eState = STK_SUCCESS;
	if (m_bIsCompress)
	{
		eState = ReadSegmentAllRecordBuffer(i_HeadP, i_RecordA);
	}
	else
	{
		eState = ReadSegmentAllRecord(i_HeadP, i_RecordA);
	}
	return eState;
}

/************************************************************************//**
 *	@brief		段Buffer读取结束
 *	@param[in]	i_HeadP					段头指针
 *	@param[in]	i_RecordA				记录指针
 *	@retval		STK_SUCCESS				读取成功
 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadSegmentFinishData(StkSegmentHead *i_HeadP, StkControl *i_RecordA[])
{
	STK_STATUS	eState = STK_SUCCESS;
	if (m_bIsCompress)
	{
		eState = ReadSegmentFinishBuffer(i_HeadP, i_RecordA);
	}
	else
	{
		eState = ReadSegmentFinish(i_HeadP, i_RecordA);
	}
	return eState;
}

/*************************************************************************//**
 *	@brief		向SVL中写入各个段被压缩后的段
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				写入成功
 *	@retval		STK_ERROR				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteCompress()
{
	StkSegmentHead	head = {0};
	STK_STATUS		eState = STK_SUCCESS;

	head.id = 0;
	head.kind = SEG_TYPE_COMPRESS;

	eState = WriteSegmentHead(&head);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 压缩段的记录
	eState = CompressData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	eState = WriteSegmentFinish(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		从SVL中读取各个段被压缩后的段
 *	@param[in]	i_headP							压缩段的记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS						读取成功
 *	@retval		STK_ERROR_MEMORY				内存申请失败
 *	@retval		STK_ERROR_VERSION_UNKNOWNFILE	读入未知版本的文件
 *	@retval		STK_ERROR_VERSION_HIGHER		读入更高的版本
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadCompress(StkSegmentHead *i_headP)
{
	StkControl**		RecordPP = NULL;
	StkCompressRec*		CompressRecP = NULL;
	STK_STATUS			eState = STK_SUCCESS;

	RecordPP = (StkControl **)calloc(i_headP->recnum, sizeof(StkControl*));
	if (i_headP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = this->ReadSegmentAllRecordData(i_headP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	for (int ii = 0; ii < i_headP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_COMPRESS)
		{
			if (RecordPP[ii]->version == 1)
			{
				CompressRecP = (StkCompressRec *)RecordPP[ii];
				eState = DeCompressData(CompressRecP);
				if (eState != STK_SUCCESS)
				{
					return STK_ERROR;
				}
			}
		}
	}

	// 释放内存
	eState = this->ReadSegmentFinishData(i_headP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	m_bIsCompress = true;

	return eState;
}

/************************************************************************//**
 *	@brief		设置文件密码
 *	@param[in]	i_wstrPassword			文件密码
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		其他					设置失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::SetPassword(wstring i_wstrPassword)
{
	m_wstrPassword = i_wstrPassword;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文件密码
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		文件密码
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
wstring Stk_File::GetPassword()
{
	return m_wstrPassword;
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
STK_STATUS Stk_File::AddRender(Stk_Render* i_stkRenderP)
{
	pair<map<STK_ID, Stk_Render*>::iterator, bool> ret;

	if (i_stkRenderP == NULL)
	{
		return STK_ERROR;
	}
	ret = m_mapRenders.insert(pair<STK_ID, Stk_Render*>(i_stkRenderP->GetID(), i_stkRenderP));
	if (ret.second)
	{
		return STK_SUCCESS;
	}
	else
	{
		return STK_SUCCESS_EXIST;
	}
}

/************************************************************************//**
 *	@brief		获取渲染(纹理)数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		渲染(纹理)数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_File::GetRenderNum()
{
	return m_mapRenders.size();
}

/************************************************************************//**
 *	@brief		获取指定的渲染(纹理)
 *	@param[in]	i_nuIndex		索引
 *	@param[out]	无
 *	@retval		指定的渲染(纹理)
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Render* Stk_File::GetRenderByIndex(STK_UINT32 i_nuIndex)
{
	STK_UINT32	nuIndex = 0;
	map<STK_ID, Stk_Render*>::iterator itor = m_mapRenders.begin();
	while(itor != m_mapRenders.end())
	{
		if (i_nuIndex == nuIndex)
		{
			return itor->second;
		}
		nuIndex ++;
		itor++;
	}
	return NULL;
}

/************************************************************************//**
 *	@brief		添加工艺BOM表格信息
 *	@param[in]	i_wstrID				BOM表格的序号
 *	@param[in]	i_stkVirtualBomInfoP	工艺BOM表格信息
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@retval		其他					添加失败
 *	@note		无
 *	@attention	内部接口，只做存储时使用，不需要用户调用该接口
 ****************************************************************************/
STK_STATUS Stk_File::AddVirtualBomTableInfo(wstring i_wstrID, Stk_VirtualBomItemInfo* i_stkVirtualBomInfoP)
{
	pair<map<wstring, Stk_VirtualBomItemInfo*>::iterator, bool> ret;

	if (i_stkVirtualBomInfoP == NULL)
	{
		return STK_ERROR;
	}
	ret = m_mapBomItemInfos.insert(pair<wstring, Stk_VirtualBomItemInfo*>(i_wstrID, i_stkVirtualBomInfoP));
	if (ret.second)
	{
		return STK_SUCCESS;
	}
	else
	{
		return STK_SUCCESS_EXIST;
	}
}

/************************************************************************//**
 *	@brief		写工艺BOM文件
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::WriteVirtualBomFile()
{
	STK_STATUS		eState = STK_SUCCESS;
	StkSegmentHead	virtualBomHead = {0};

	/*
	 *	【第1步】输出工艺BOM表格段
	 */

	// 【1.1 输出段头记录】
	virtualBomHead.id = 0;
	virtualBomHead.kind = SEG_TYPE_VIRTUALBOM;
	eState = WriteSegmentHeadData(&virtualBomHead);
	if(eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	//	【1.2 输出工艺BOM表格每一栏的记录】
	map<wstring, Stk_VirtualBomItemInfo*>::iterator itor = m_mapBomItemInfos.begin();
	while(itor != m_mapBomItemInfos.end())
	{
		eState = (itor->second)->WriteFile(this, &virtualBomHead);
		if(eState != STK_SUCCESS)
		{
			return STK_ERROR;
		}
		itor++;
	}

	// 【1.3 完成工艺BOM表格段的输出】
	eState = WriteSegmentFinishData(&virtualBomHead);
	if(eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取工艺BOM文件
 *	@param[in]	i_VirtualBomHeadSP	工艺BOM表格段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_SUCCESS_EXIST	已读取跳出
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::ReadVirtualBomFile(StkSegmentHead *i_VirtualBomHeadSP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkVirtualBomRec*		virtualBomRecP = NULL;
	Stk_VirtualBomItemInfo*	virtualBomItemP = NULL;

	/*
	 *	读取工艺BOM表格段的数据
	 */
	RecordPP = (StkControl **)calloc(i_VirtualBomHeadSP->recnum, sizeof(StkControl*));
	if (i_VirtualBomHeadSP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = this->ReadSegmentAllRecordData(i_VirtualBomHeadSP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取工艺BOM表格段的各记录
	 */
	for (int ii = 0; ii < i_VirtualBomHeadSP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_VIRTUALBOM_ITEM)
		{
			if (RecordPP[ii]->version == 1)
			{
				virtualBomRecP = (StkVirtualBomRec*)RecordPP[ii];
				virtualBomItemP = new Stk_VirtualBomItemInfo();
#ifdef __MOBILE__
				virtualBomItemP->SetID(Stk_Platform::STKWCHARStrToWString(virtualBomRecP->chID));
				virtualBomItemP->SetSymbol(Stk_Platform::STKWCHARStrToWString(virtualBomRecP->chSymbol));
				virtualBomItemP->SetName(Stk_Platform::STKWCHARStrToWString(virtualBomRecP->chName));
				virtualBomItemP->SetMaterial(Stk_Platform::STKWCHARStrToWString(virtualBomRecP->chMaterial));
#else
				virtualBomItemP->SetID(virtualBomRecP->chID);
				virtualBomItemP->SetSymbol(virtualBomRecP->chSymbol);
				virtualBomItemP->SetName(virtualBomRecP->chName);
				virtualBomItemP->SetMaterial(virtualBomRecP->chMaterial);
#endif
				virtualBomItemP->SetNumber(virtualBomRecP->nuNumber);
#ifdef __MOBILE__
				m_mapBomItemInfos.insert(pair<wstring, Stk_VirtualBomItemInfo*>(Stk_Platform::STKWCHARStrToWString(virtualBomRecP->chID), virtualBomItemP));
#else
				m_mapBomItemInfos.insert(pair<wstring, Stk_VirtualBomItemInfo*>(virtualBomRecP->chID, virtualBomItemP));
#endif
			}
		}
	}

	// 释放内存
	eState = this->ReadSegmentFinishData(i_VirtualBomHeadSP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		检索当前File中ProtoType的所有子ProtoType
 *	@param[in]	i_stkProtoTypeP		当前File中ProtoType
 *	@param[out]	无
 *	@retval		STK_SUCCESS			检索成功
 *	@retval		其它				检索失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_File::SearchFileProtoTypes(Stk_ProtoType* i_stkProtoTypeP)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_SIZE_T		nuChildInsNum = 0;
	Stk_Instance*	stkInstanceP = NULL;
	Stk_ProtoType*	stkProtoTypeP = NULL;

	if (i_stkProtoTypeP == NULL)
	{
		return STK_ERROR;
	}
	m_mapFileProtos.insert(pair<Stk_ProtoType*, STK_BOOL>(i_stkProtoTypeP, true));

	nuChildInsNum = i_stkProtoTypeP->GetChildInsNum();
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
			return STK_SUCCESS;	// 返回STK_SUCCESS，而不返回STK_ERROR的原因是有可能装配结构中只有实例，而没有Proto的情形
		}
		// 仅递归遍历当前File中的数据
		if (this != stkProtoTypeP->GetFile())
		{
			continue;
		}

		eState = SearchFileProtoTypes(stkProtoTypeP);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}
	}
	return eState;
}
