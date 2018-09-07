// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_DocumentManager.cpp
 *
 *	@brief	多文件管理
 *
 *	@par	历史:
 *		2013/12/24	创建。Shaoqy@HOTEAMSOFT
 *		2014/03/26	更新。Gaoqq@HOTEAMSOFT
 *					改善LoadSubFile的调用策略
 *		2014/04/12	更新。Gaoqq@HOTEAMSOFT
 *					改善IDManger的设计，纳入DocumentManager下
 *		2014/04/24	更新。Gaoqq@HOTEAMSOFT
 *					支持Andriod平台
 *		2014/07/21	更新。WangY@HOTEAMSOFT
 *					支持UG、CATIA存储改善
 *		2014/09/05	更新。WangY@HOTEAMSOFT
 *					稳定UG、CATIA存储改善
****************************************************************************/
#include "StdAfx.h"
#include "Stk_DocumentManager.h"
#include "Stk_Manager.h"
#include "Stk_Utility.h"
#include "Stk_Platform.h"
#include "Stk_Preview.h"
#include "Stk_AESHelper.h"
#include "Stk_VirtualBomItemInfo.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif
#include "Stk_Listener.h"


/************************************************************************//**
 *	@brief		Stk_DocumentManager构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_DocumentManager::Stk_DocumentManager()
{
	m_DefaultTopProtoP = NULL;
	m_nuFilesNum = 0;
	m_eSaveType = FILESAVE_TYPE_UNIQUE;
	m_eBeforType = FILESAVE_TYPE_UNIQUE;
	m_bPmiRead = TRUE;
	m_GlobalIDManger = new Stk_GlobalIDManager();
	m_wstrLoadPath = L"";
	m_wstrSavePath = L"";
#ifdef _WINDOWS_
	// 日志配置参数初始化
	m_SVLLog.bLogOutput = 0;
	m_SVLLog.bPrototype = 1;
	m_SVLLog.bBody = 1;
	m_SVLLog.bPMIAndView = 0;
	m_SVLLog.bMesh = 1;
	m_SVLLog.bMetaData = 0;
	m_svlgLogFile = new Stk_LogFileEx();
#endif
	m_eDocumentSaveType = DOCUMENT_SAVE_TYPE_UNKNOWN;
	m_stkAesHelperP = new Stk_AesHelper();
	m_stkListenerP = NULL;
	m_ReaderListenerP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_DocumentManager析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_DocumentManager::~Stk_DocumentManager()
{
	for (int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		// V2.156及以前
		//m_GlobalIDManger->DelFileLinkString(m_vcFiles[ii]->GetSVLPath());
		// V2.2及以上 ###
		m_GlobalIDManger->DelFileID(m_vcFiles[ii]->GetFileID());
		m_GlobalIDManger->DelCADFileIDByPath(m_vcFiles[ii]->GetCADFilePath());
	}

	// 以ProtoType为起始点进行内存释放
	for (int ii = 0; ii < m_vcTopProtoTypes.size(); ii++)
	{
		if (m_vcTopProtoTypes[ii] != NULL)
		{
			m_vcTopProtoTypes[ii]->Release();
		}
	}
	
	ClearSVLLoadList();

#ifdef _WINDOWS_
	m_svlgLogFile->Close();
	delete m_svlgLogFile;
#endif
	if (m_stkAesHelperP != NULL)
	{
		delete m_stkAesHelperP;
		m_stkAesHelperP = NULL;
	}

	if(m_ReaderListenerP != NULL)
	{
		delete m_ReaderListenerP;
		m_ReaderListenerP = NULL;
	}

	if(m_GlobalIDManger != NULL)
	{
		delete m_GlobalIDManger;
		m_GlobalIDManger = NULL;
	}
}

#if 0
/************************************************************************//**
 *	@brief		加载需要打开的文件
 *	@param[in]	i_wstrLoadPath		文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			加载成功
  *	@retval		其它				加载失败
 *	@note		V2.2版本以后不再使用此接口
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::LoadStruct(wstring i_wstrLoadPath)
{
	Stk_File* stkTopFileP = NULL;

	m_wstrLoadPath = i_wstrLoadPath;

	stkTopFileP = LoadSubFile(i_wstrLoadPath, true);
	if (stkTopFileP==NULL)
	{
		return STK_ERROR; // 读取到错误的SVL文件
	}

// 	if(stkTopFileP->GetTopInstance())
// 	{
// 		m_TopInstanceP = stkTopFileP->GetTopInstance();
// 		return STK_SUCCESS;
// 	}
// 	else
// 	{
// 		return STK_ERROR;
// 	}

	// 对于2.2版本以前的SVL文件，一个StkFile只对应一个Prototype，因此取第0个Prototype即可。
	m_vcTopProtoTypes.push_back(stkTopFileP->GetTopProtoTypeByIndex(0));
	m_DefaultTopProtoP = m_vcTopProtoTypes[0];

	return STK_SUCCESS;
}


/************************************************************************//**
 *	@brief		加载子装配文件
 *	@param[in]	i_wstrLoadPath		文件路径
 *	@param[in]	i_bIsFirst			是否为第一个文件标志
 *	@param[out]	无
 *	@retval		Stk_File*			文件对象
 *	@note		V2.2版本以后不再使用此接口
 *	@attention	无
 ****************************************************************************/
Stk_File* Stk_DocumentManager::LoadSubFile(wstring i_wstrLoadPath, STK_BOOL i_bIsFirst)
{
	STK_STATUS stat = STK_SUCCESS;
	Stk_File* stkCurFileP = NULL;
	Stk_Instance* stkTopInstanceP = NULL;
	Stk_ProtoType* stkTopProtoP = NULL;
	Stk_Instance* stkChildInsP = NULL;
	wstring wstrChildPath;

	/*
	 *	打开当前文件
	 */
	stkCurFileP = new Stk_File(this);
	stkCurFileP->SetSVLPath(i_wstrLoadPath);

	stat = stkCurFileP->RegFileLink();
	if(stat == STK_SUCCESS)
	{
		stkCurFileP->AddRef(); //增加引用
	}
	else if(stat == STK_SUCCESS_EXIST)
	{
		Stk_File* stkFileP = NULL;
		for (int ii = 0; ii < m_vcFiles.size(); ii ++)
		{
			if (m_vcFiles[ii]->GetSVLPath() == i_wstrLoadPath)
			{
				stkFileP = m_vcFiles[ii];
				Stk_ProtoType* stkProtoP = NULL;
				// 对于2.2版本以前的SVL文件，一个StkFile只对应一个Prototype，因此取第0个Prototype即可。
				stkProtoP = stkFileP->GetTopProtoTypeByIndex(0);
				stkProtoP->AddRef();		// 对于文件已打开的，Ins对应的Proto的引用计数器加1

				delete stkCurFileP;

				stkCurFileP = stkFileP;
				stkCurFileP->AddRef();

				/*if(i_nuFileNum != 0 && m_nuFilesNum == i_nuFileNum)*/ // 若是右键打开的新SVL文件，则子proto应加1
				//if (m_nuFilesNum != 0 && i_bIsFirst == true)
				//{
				//	for (int jj = 0; jj < stkProtoP->GetChildInsNum(); jj ++)
				//	{
				//		stkProtoP->GetChildIns(jj)->GetProtoType()->AddRef();
				//	}
				//	return stkFileP;
				//} //Delete by Gaoqq on 2014.5.7 仅顶级Proto需要AddRef
				//    子件不能AddRef否则多次装配后子件不能释放 
				break;
			}
		}

		return stkCurFileP; // 改进LoadSubFile方法，仅当加载失败时才返回NULL。
	}

// 注意！因为消除顶级实例，所以取消了这个接口的调用。
// 以后File下只有有顶级ProtoType。
// 	if(i_bIsFirst == true)		// 如果打开的为第一个文件，则需要创建Instance对象。
// 	{
// 		stat = stkCurFileP->OpenFile(i_wstrLoadPath);
// 		if(stat!=STK_SUCCESS) 
// 		{
// 			return NULL;
// 		}
// 		stkCurFileP->GetTopProtoType()->AddRef();
// 	}
// 	else						// 对于分散文件的子实例的文件，其相应的Instance已经在相应的顶级实例创建。
// 	{
		m_eBeforType = FILESAVE_TYPE_DISTRIBUTION;
		stat = stkCurFileP->OpenFile(i_wstrLoadPath);
		if(stat!=STK_SUCCESS)
		{
			return NULL;
		}
		// 对于2.2版本以前的SVL文件，一个StkFile只对应一个Prototype，因此取第0个Prototype即可。
		stkCurFileP->GetTopProtoTypeByIndex(0)->AddRef();		// 对于分散文件子实例的Proto引用计数器进行加1
//	}

	if(stkCurFileP != NULL)
	{
		m_nuFilesNum ++;
		m_vcFiles.push_back(stkCurFileP);
	}

	/*
	 *	向下级递归
	 */
	STK_UINT32 idxChild = 0;
	// 对于2.2版本以前的SVL文件，一个StkFile只对应一个Prototype，因此取第0个Prototype即可。
	stkTopProtoP = stkCurFileP->GetTopProtoTypeByIndex(0);
	if(stkTopProtoP != NULL)
	{
		while(idxChild < stkTopProtoP->GetChildInsNum() )
		{
			Stk_File* stkChildFileP = NULL;

			stkChildInsP = stkTopProtoP->GetChildIns(idxChild);
			wstrChildPath = stkChildInsP->GetFilePath();	// 获取子实例对应的Proto所在的SVL文件路径
			if (wstrChildPath == L"" || wstrChildPath == L"Local")	// 单一文件，直接跳出
			{
				break;
			}
			else if(wstrChildPath != L"" && wstrChildPath != L"Local")	// 若打开的SVL文件为早于2.13版本的单一存储的，childPath为空。 2.14版本之后的若路径和顶级Proto所在的文件路径相同，则是单一文件。
			{
				stkChildFileP = LoadSubFile(wstrChildPath, false);
				//注意！
				// 1. 没找到SVL文件，返回NULL
				// 2. 读取SVL文件，返回指针
				// 3. 此前SVL已被读取，返回指针
				if(stkChildFileP==NULL)
				{
					stkTopProtoP->RemoveChild(stkChildInsP);
					delete stkChildInsP;
				}else
				{
					idxChild++;
				}
			}
		}
	}

	return stkCurFileP;
}

/****************************************************************************
 *	@brief		保存文件
 *	@param[in]	i_wstrSavePath		保存路径
 *	@param[out]	无
 *	@retval		状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SaveConstruct(wstring i_wstrSavePath, Stk_ProtoType* i_stkProtoP, BOOL i_flag)
{
	STK_STATUS ret = STK_SUCCESS;

	if(m_eSaveType == FILESAVE_TYPE_BEFOR)
	{
		m_eSaveType = m_eBeforType;
	}

#ifdef _WINDOWS_
	InitSVLLog(1, 2, i_wstrSavePath, i_stkProtoP);			// Log
#endif

	m_GlobalIDManger->SetModifiedFlag(TRUE);	// 设置需保存的所有ProtoType为脏数据
	ret = SaveSubFile(i_wstrSavePath, i_stkProtoP, i_flag);

	return ret;
}

/****************************************************************************
 *	@brief		保存单个SVL文件
 *	@param[in]	i_wstrSavePath		保存的路径
 *	@param[in]	i_stkProtoP			需保存的文件的顶级ProtoType对象指针
 *	@param[in]	i_flag				是否继续向下保存
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
  *	@retval		其它				保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SaveSubFile(wstring i_wstrSavePath, Stk_ProtoType* i_stkProtoP, BOOL i_flag)
{
	Stk_File* stkCurFileP = NULL;
	STK_STATUS ret = STK_SUCCESS;
	STK_UINT32 childNum = 0;
	Stk_Instance* stkChildInsP = NULL;
	StkProtoTypeEnum eProtoType;
	wstring wstrSaveDir = i_wstrSavePath;


	// 查找对应的Stk_File
	if (m_vcFiles.size() < 1)
	{
		// 无SVL文件，无法保存
		return STK_ERROR;
	}
	else if (m_vcFiles.size() == 1)
	{
		// 根据Proto找到对应的File
		stkCurFileP = m_vcFiles[0];
	}
	else
	{
		// START 注意！这里的设计是有问题的。例如以下场景
		// 打开单一装配SVL，插入分散装配SVL，此时stkCurFileP = m_vcFiles[ii];是获取不到File的。
		// 虽然这里做了缺陷对应，但不是一个正式的解决方案。在进行混合文件的时候进行彻底改善。
		for (int ii = 0; ii < m_vcFiles.size(); ii++)
		{
			if(m_vcFiles[ii]->GetTopProtoTypeByIndex(0)->GetID() == i_stkProtoP->GetID())
			{
				stkCurFileP = m_vcFiles[ii];
				break;
			}
		}

		if (stkCurFileP == NULL)
		{
			stkCurFileP = i_stkProtoP->GetFile();
		}
		// END
	}

	if(stkCurFileP == NULL)
	{
		return STK_ERROR;
	}

	// 求出文件夹
	wstring::size_type index;
	index = wstrSaveDir.find_last_of (L"\\");
	if(index != -1)
	{
		if(index!=(wstrSaveDir.size()-1))
		{
			if(wstrSaveDir.find_last_of (L".")>wstrSaveDir.find_last_of (L"\\"))
			{
				wstrSaveDir = wstrSaveDir.substr(0, index + 1);
			}
			else
			{
				wstrSaveDir.append(L"\\");
			}
		}
	}

	// 输出SVL文件
	if(i_stkProtoP->IsDirtyData())	// 判断此Proto是否已保存
	{
		stkCurFileP->AddTopProtoType(i_stkProtoP);	// 设置需保存的Proto

		ret = stkCurFileP->WriteFile((WCHAR*)i_wstrSavePath.c_str());
		if( ret!= STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 当分散保存时,向下递归
	eProtoType = i_stkProtoP->GetType();
	if ( i_flag == true && m_eSaveType == FILESAVE_TYPE_DISTRIBUTION && eProtoType == PROTO_TYPE_ASSY)
	{
		childNum = i_stkProtoP->GetChildInsNum();
		for(int ii = 0; ii < childNum; ii ++)
		{
			stkChildInsP = i_stkProtoP->GetChildIns(ii);
			if(stkChildInsP != NULL)
			{
				wstring wstrSavePath = wstrSaveDir;
				Stk_ProtoType *stkChildProtoP = stkChildInsP->GetProtoType();
				StkProtoTypeEnum eChildType = stkChildProtoP->GetType();

				wstring wstrProtoName = stkChildProtoP->GetProtoName();
				switch(eChildType)
				{
				case PROTO_TYPE_ASSY:
					wstrSavePath.append(wstrProtoName);
					wstrSavePath.append(L".asm.svl");
					break;

				case PROTO_TYPE_UNASSY:
				case PROTO_TYPE_PART:
					wstrSavePath.append(wstrProtoName);
					wstrSavePath.append(L".prt.svl");
					break;

				case PROTO_TYPE_SHEET:
					wstrSavePath.append(wstrProtoName);
					wstrSavePath.append(L".sht.svl");
					break;

				default:
					wstrSavePath.append(wstrProtoName);
					wstrSavePath.append(L".asm.svl");
					break;
				}

				ret = SaveSubFile(wstrSavePath, stkChildProtoP);
				if (ret == STK_ERROR)
				{
					goto rtn;
				}
			}
		}
	}
rtn:
	return ret;
}
#endif

/************************************************************************//**
 *	@brief		计算出顶级ProtoType列
 *	@param[in]	i_vcFiles		文件列
 *	@param[out]	o_vcTopProtos	顶级ProtoType列
 *	@retval		STK_SUCCESS 计算成功
 *	@retval		STK_ERROR   计算失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::ComputeTopProtos(map<Stk_File*,bool> i_mapFiles, vector<Stk_ProtoType*> &o_vcTopProtos)
{
	map<STK_ID,STK_BOOL> mapInsProto;
	map<STK_ID,STK_BOOL> mapProto;
	STK_SIZE_T		i,j,k;
	Stk_File		*stkFileP = NULL;
	Stk_ProtoType	*stkProtoP = NULL;
	Stk_Instance	*stkInsP = NULL;
	STK_SIZE_T		cnt,cnt2;
	STK_STATUS		ret = STK_SUCCESS;
	map<Stk_File*,bool>::iterator Fitor;
	map<STK_ID,STK_BOOL>::iterator protoItor;

	o_vcTopProtos.clear();
	for(Fitor=i_mapFiles.begin(); Fitor != i_mapFiles.end(); Fitor ++)
	{
		stkFileP = Fitor->first;
		cnt = stkFileP->GetTopProtoTypeNum();
		for( j=0; j<cnt; j++)
		{
			stkProtoP = stkFileP->GetTopProtoTypeByIndex(j);
			mapProto.insert(pair<STK_ID,STK_BOOL>(stkProtoP->GetID(),true));

			// 提高检索效率：当ProtoType存在于mapInsProto时，表示已检索过，不再检索其子装配
			protoItor = mapInsProto.find(stkProtoP->GetID());
			if (protoItor != mapInsProto.end())
			{
				continue;
			}
			ret = SearchChildInsProtoType(stkProtoP, mapInsProto);
			if (ret != STK_SUCCESS)
			{
				return ret;
			}
		}
	}

	map<STK_ID,STK_BOOL>::iterator itor = mapInsProto.begin();
	map<STK_ID,STK_BOOL>::iterator itorProto;
	while(itor != mapInsProto.end())
	{
		itorProto = mapProto.find(itor->first);
		if (itorProto != mapProto.end())
		{
			mapProto.erase(itorProto);
		}
		itor++;
	}

	if (mapProto.size() == 0)
	{
		return STK_ERROR;
	}

	itorProto = mapProto.begin();
	while(itorProto != mapProto.end())
	{
		m_GlobalIDManger->GetProtoTypeByID(itorProto->first, stkProtoP);
		o_vcTopProtos.push_back(stkProtoP);
		itorProto++;
	}

	if (o_vcTopProtos.size()==0)
	{
		ret = STK_ERROR;
	}

	return ret;
}

/************************************************************************//**
 *	@brief		打开SVL装配文件
 *	@param[in]	i_wstrSVLPath					SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS						成功
 *	@retval		STK_ERROR_VERSION_UNKNOWNFILE	不可识别版本
 *	@retval		STK_ERROR_VERSION_HIGHER		读入更高版本
 *	@retval		其它							错误
 *	@note		由于V2.2后SVL文件可能存在多个顶级ProtoType，因此需要用户指定哪个作为默认顶级ProtoType。<br>
 *				打开后务必通过Stk_DocumentManager::SetDefaultTopProto()方法设置顶级ProtoType
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::LoadDocument(wstring& i_wstrSVLPath)
{
	STK_STATUS ret = STK_SUCCESS;

	/* 
	 *	加载SVL装配文件
	 */
	ret = LoadCADSubFile(i_wstrSVLPath, m_vcTopProtoTypes);
	m_wstrLoadPath = i_wstrSVLPath;

	/*
	 *	判断文件类型：单一、分散 @@@临时方案，正式方案需要实现混合存储
	 */
	// 混合装配
	//if (m_DefaultTopProtoP != NULL)
	//{
	//	m_eSaveType = ComputeFileSaveType(m_DefaultTopProtoP);
	//}
	//else if(m_vcTopProtoTypes.size()>0)
	//{
	//	m_eSaveType = ComputeFileSaveType(m_vcTopProtoTypes[0]);
	//}

	return ret;
}


/************************************************************************//**
 *	@brief		打开SVL装配及下级的文件
 *	@param[in]	i_wstrSVLPath					SVL文件路径
 *	@param[out]	o_vcTopProtos					顶级ProtoType列
 *	@retval		STK_SUCCESS						成功
 *	@retval		STK_ERROR_VERSION_UNKNOWNFILE	不可识别版本
 *	@retval		STK_ERROR_VERSION_HIGHER		读入更高版本
 *	@retval		其它							错误
 *	@note		对应UG、CATIA等场景，V2.2后使用该方法
 *	@attention	无
 ****************************************************************************/

STK_STATUS	Stk_DocumentManager::LoadCADSubFile(wstring& i_wstrSVLPath, vector<Stk_ProtoType*>& o_vcTopProtos )
{
	Stk_File			*stkCADFileP = NULL;
	Stk_File			*stkTempFileP = NULL;
	Stk_ProtoType		*stkProtoP = NULL;
	wstring				wstrCurFile;
	STK_SIZE_T			i;
	STK_STATUS			ret = STK_SUCCESS;
	FILE				*fileP = NULL;
	bool				bfirstFile; //SVL文件中的第一个CADFile
	bool				bTopSVL = true; //第一个SVL文件
	STK_ID				idFile = 0;
	map<Stk_File*,bool>	mapCADFileOfTopSVL;
	wstring				wstrSVLVersion = L"";
	STK_BOOL			bIsOldVersion = FALSE;
	STK_BOOL			bLoadingCancel = false;		// 配合李永杰项目验收，临时方案

	// 注意！在打开SVL的时刻不用清空SVLFileList
	// 原因是在插入的时刻为了判定SVL是否加载还需要用到SVLFileList
	//ret = ClearSVLSaveList();

	/*
	 *	循环打开所有的SVL文件
	 */
	m_vecPaths.push_back(i_wstrSVLPath);
	while(m_vecPaths.size()>0)
	{
		STK_UINT32 nuProtoTypeCount = 1;	// 用于计录SVL中ProtoType数量，初始默认为1
		vector<wstring> vecChild;
		vector<Stk_File*> vcFiles;
		Stk_File *stkExistFile = NULL;
		wstring	wstrFilePassword = L"";
		wstring	wstrFileDePassword = L"";

		// 检查SVLFileList来判定该SVL文件是否已加载
		// 注意！在打开时刻，SVLFileList作为已打开文件的临时工作区
		wstrCurFile = m_vecPaths[0];
		//m_vecPaths.erase(m_vecPaths.begin()); //等待加载的SVL路径列表
		stkExistFile = InqTopFileFromSVLLoadList(wstrCurFile);//已加载的SVL路径列表
		if(stkExistFile != NULL)
		{
			//已存在该文件
			if (bTopSVL)
			{
				mapCADFileOfTopSVL.insert(pair<Stk_File*,bool>(stkExistFile,true));
			}
			m_vecPaths.erase(m_vecPaths.begin()); //等待加载的SVL路径列表
			continue;
		}

		// 判断版本号
		// 预先读取SVL文件的版本号
		stkTempFileP = new Stk_File(this);
		ret = stkTempFileP->PreOpenFile(wstrCurFile);
		if (ret == STK_SUCCESS)
		{
			wstrSVLVersion = stkTempFileP->GetSVLVersion();
			bIsOldVersion = Stk_Utility::IsOldVersion(wstrSVLVersion);
			// 判定用户输入的解密密码是否正确
			wstrFilePassword = stkTempFileP->GetPassword();
			if (wstrFilePassword != L"")
			{
				wstring wstrUserDePassword = SearchUserDePasswordBySVLPath(wstrCurFile);
				DePassword(wstrFilePassword, wstrFileDePassword);
				if (wstrUserDePassword == L"")
				{
					delete stkTempFileP;
					return STK_WAITING_PASSWORD;
				}
				if (wstrUserDePassword != wstrFileDePassword)
				{
					delete stkTempFileP;
					return STK_ERROR_PASSWORD;
				}
			}
		}
		delete stkTempFileP;
		if (ret != STK_SUCCESS)
		{
			return ret;
		}

		m_vecPaths.erase(m_vecPaths.begin()); //等待加载的SVL路径列表

		/*
		 *	读取该SVL文件中的所有的CADFile（一个SVL中可以有多个CADFile）
		 */
		bfirstFile = true;
		// ###
		while( fileP != NULL || bfirstFile)
		{
			stkCADFileP = new Stk_File(this);
			if (bIsOldVersion)
			{
				// 加载V2.2版之前的旧SVL文件
				stkCADFileP->SetOldVersion(true);
				vcFiles.push_back(stkCADFileP);
				ret = stkCADFileP->OpenFile(wstrCurFile, fileP, nuProtoTypeCount, vecChild);
				if(ret != STK_SUCCESS)
				{
					return ret;
				}
				// 兼容旧文件特殊处理
				if (m_eSaveType == FILESAVE_TYPE_UNIQUE && vcFiles.size() > 1)
				{
					ConvertTitleToCADFileTitle(stkCADFileP, vcFiles[0]);
					//ConvertPreViewToCADFilePreView(stkCADFileP, vcFiles[0]);
					ConvertOptionToCADFileOption(stkCADFileP, vcFiles[0]);
				}
				/*
				 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
				 *	正式版本方案：参考NX，装配和模型数据分开存储。
				 */
				// begin
				//if (ret == STK_LOADING_CANCEL)
				//{
				//	bLoadingCancel = true;
				//}
				//end
			}
			else
			{
				//加载V2.2版以后的新SVL文件
				ret = stkCADFileP->OpenCADFile(wstrCurFile, fileP ,vecChild);
				if(ret != STK_SUCCESS)
				{
					return ret;
				}
				/*
				 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
				 *	正式版本方案：参考NX，装配和模型数据分开存储。
				 */
				// begin
				//if (ret == STK_LOADING_CANCEL)
				//{
				//	bLoadingCancel = true;
				//}
				//end
			}

			// 注册CADFile
			m_GlobalIDManger->RegCADFilePath(stkCADFileP->GetCADFilePath(), idFile); //CADFilePath
			stkCADFileP->RegisterID(STK_NEW_ID);
			m_vcFiles.push_back(stkCADFileP);

			// 收集子SVL
			for ( i=0; i<vecChild.size(); i++ )
			{
				m_vecPaths.push_back(vecChild[i]);
				m_mapParentFileList.insert(pair<wstring, Stk_File*>(vecChild[i], stkCADFileP));
			}
			vecChild.clear();

			// 准备用于计算顶级ProtoType的数据
			if (bTopSVL)
			{
				mapCADFileOfTopSVL.insert(pair<Stk_File*,bool>(stkCADFileP,true));
			}

			AddToSVLLoadList(wstrCurFile,stkCADFileP);
			bfirstFile = false;
		}
		bTopSVL = false;

		/*
		 *	为配合李永杰项目验收，临时解决方案：取消后，直接返回释放内存。
		 *	正式版本方案：参考NX，装配和模型数据分开存储。
		 */
		// begin
		//if (bLoadingCancel)
		//{
		//	m_bLoadingCancel = false;
		//	break;
		//}
		// end
	}

	/*
	 *	计算出DocManager的顶级ProtoType
	 *	注意！V2.2后顶级ProtoType可能有多个,仅从顶级SVL文件中判定
	 */
	ret = ComputeTopProtos(mapCADFileOfTopSVL, o_vcTopProtos);
	if(ret!= STK_SUCCESS)
	{
		return ret;
	}

	/*
	 *	设置Docmanger顶级ProtoType计数器
	 */
	for (int ii = 0; ii < o_vcTopProtos.size(); ii++)
	{
		o_vcTopProtos[ii]->AddRef();
	}

	/*
	 *	设置ProtoType计数器
	 */
	map<STK_ID, STK_UINT32>::iterator itor = m_mapLoadProtoRef.begin();
	while(itor != m_mapLoadProtoRef.end())
	{
		stkProtoP = GetProtoTypeByID(itor->first);
		if (stkProtoP != NULL)
		{
			stkProtoP->SetRef(itor->second);
		}
		itor++;
	}
	ClearProtoRef();
	m_mapUserDePassword.clear();

	//ret = ClearSVLSaveList();
	return ret;
}

/************************************************************************//**
 *	@brief		获取GlobalIDManger
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_GlobalIDManger	GlobalIDManger
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_GlobalIDManager* Stk_DocumentManager::GetGlobalIDManger()
{
	return m_GlobalIDManger;
}

/************************************************************************//**
 *	@brief		获取文件保存的路径
 *	@param[in]	无
 *	@retval		文件保存的路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_DocumentManager::GetFileSavePath()
{
	return m_wstrSavePath;
}

/************************************************************************//**
 *	@brief		获取文件保存类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eSaveType			文件保存类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkFileSaveTypeEnum Stk_DocumentManager::GetFileSaveType()
{
	return m_eSaveType;
}

/************************************************************************//**
 *	@brief		设置文件保存类型
 *	@param[in]	i_eSaveType			文件保存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SetFileSaveType(StkFileSaveTypeEnum i_eSaveType)
{
	m_eSaveType = i_eSaveType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取PMI读取标志
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bPmiRead			PMI读取标志
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_DocumentManager::GetPMIViewReadFlag()
{
	return m_bPmiRead;
}

/************************************************************************//**
 *	@brief		设置PMI读取标志
 *	@param[in]	i_bflag			PMI读取标志
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

VOID Stk_DocumentManager::SetPMIViewReadFlag(STK_BOOL i_bflag)
{
	m_bPmiRead = i_bflag;
}

/************************************************************************//**
 *	@brief		获取默认的顶级ProtoType
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_DefaultTopProtoP			默认顶级ProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_DocumentManager::GetDefaultTopProtoType()
{
	return m_DefaultTopProtoP;
}

/*************************************************************************//**
 *	@brief		往顶级ProtoType列中添加顶级ProtoType
 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::AddTopProtoType(Stk_ProtoType* i_stkProtoTypeP)
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

STK_UINT32 Stk_DocumentManager::GetTopProtoTypeNum()
{
	return m_vcTopProtoTypes.size();
}

/*************************************************************************//**
 *	@brief		获取指定索引对应的顶级ProtoType
 *	@param[in]	i_nuIndex				指定索引
 *	@param[out]	无
 *	@retval		指定的顶级ProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_DocumentManager::GetTopProtoTypeByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcTopProtoTypes.at(i_nuIndex);
};


/*************************************************************************//**
 *	@brief		设置默认的顶级ProtoType，作为各种操作的根节点
 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
 *	@param[out]	无
 *	@retval		STK_SCUCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		必须使用GetTopProtoTypeByIndex()中取得的ProtoType作为输入参数来设置
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SetDefaultTopProto(Stk_ProtoType* i_stkProtoTypeP)
{
	STK_STATUS	ret = STK_SUCCESS;
	STK_SIZE_T	i;
	Stk_File*	stkFileP = NULL;

	for (i=0; i<m_vcTopProtoTypes.size(); i++)
	{
		if ( m_vcTopProtoTypes[i] == i_stkProtoTypeP)
		{
			m_DefaultTopProtoP = i_stkProtoTypeP;
			break;
		}
	}

	if (i == m_vcTopProtoTypes.size())
	{
		m_DefaultTopProtoP = NULL;
		ret = STK_ERROR;
	}

	/*
	 *	设置顶级File
	 *	注意！因为V2.2的File列由树桩结构改为线性结构后，需要确认哪个是顶级File
	 *	因此需要设置顶级File
	 */
	stkFileP = i_stkProtoTypeP->GetFile();
	if (stkFileP == NULL)
	{
		return STK_ERROR;
	}
	stkFileP->SetTopFile(true);

	return ret;
}

/************************************************************************//**
 *	@brief		通过索引获取File对象
 *	@param[in]	i_nuKeyPos			索引值
 *	@param[out]	无
 *	@retval		stkFileP			File文件指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_File* Stk_DocumentManager::GetFileByIndex(STK_UINT32 i_nuKeyPos)
{
	Stk_File* stkFileP = NULL;

	if(i_nuKeyPos >= 0 && i_nuKeyPos < m_vcFiles.size())
	{
		stkFileP = m_vcFiles[i_nuKeyPos];
	}
	return stkFileP;
}

/************************************************************************//**
 *	@brief		获取File文件的个数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		nuFileNum			File文件个数
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_DocumentManager::GetFileNum()
{
	STK_UINT32 nuFileNum = 0;

	nuFileNum = m_vcFiles.size();
	return nuFileNum;
}

/************************************************************************//**
 *	@brief		获取加载文件的路径
 *	@param[in]	无
 *	@retval		加载文件的路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_DocumentManager::GetFileLoadPath()
{
	return m_wstrLoadPath;
}


#if 0
/************************************************************************//**
 *	@brief		通过路径获取SVL中的顶级File对象
 *	@param[in]	i_wstrPath			文件路径
 *	@param[out]	无
 *	@retval		stkFileP			File对象指针
 *	@note		
 *	@attention	无
 ****************************************************************************/
Stk_File* Stk_DocumentManager::GetFileByPath(const wstring& i_wstrPath)
{
	Stk_File* stkFileP = NULL;
	for(int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		wstring wstrFilePath = m_vcFiles[ii]->GetSVLPath();
		if(wstrFilePath == i_wstrPath)
			stkFileP = m_vcFiles[ii];
	}
	return stkFileP;
}

/****************************************************************************
 *	@brief		根据顶级Proto的存放路径获取ChildIns保存的路径
 *	@param[in]	i_wstrPath			顶级Proto的存放路径
 *	@param[out]	无
 *	@retval		wstrPath			ChildIns保存的路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_DocumentManager::GetChildInsSavePath(wstring i_wstrPath)
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
#endif

/************************************************************************//**
 *	@brief		通过实例配置路径获取实例
 *	@param[in]	i_wstrPlcPath			实例配置路径
 *	@retval		returnIns				根据路径获取的实例指针
 *	@note		找不到对应Ins则返回NULL
 *	@attention	无
 ****************************************************************************/

Stk_Instance* Stk_DocumentManager::GetInstanceByPath(const wstring& i_wstrPlcPath)
{                                                                                                 
	Stk_Instance* ChildIns = NULL;
	Stk_Instance* returnIns = NULL;
	Stk_ProtoType* TopProto = NULL;
	WCHAR *cPlcIdP = NULL;
	WCHAR srcPath[1024] = L"";
	WCHAR *tempP = NULL;
	INT wiPlcId = 0;

	if (i_wstrPlcPath ==  L"PATH|0" || i_wstrPlcPath ==  L""  )
	{
		return NULL;
	}

#ifdef __MOBILE__
	wcscpy(srcPath,i_wstrPlcPath.c_str());
	cPlcIdP = wcstok(srcPath,L"|",&tempP);
#else
	wcscpy_s(srcPath,1024,i_wstrPlcPath.c_str());
	cPlcIdP = wcstok(srcPath,L"|");
#endif

	int depth = 0;

	TopProto = m_DefaultTopProtoP;
	while(cPlcIdP !=NULL)
	{
		if(depth>1)
		{
			wiPlcId = wcstol(cPlcIdP,NULL,10);
			if(TopProto->GetChildInsNum() <=0)
			{
				returnIns = NULL;
				break;
			}

			for(int i = 0; i < TopProto->GetChildInsNum(); i++)
			{
				returnIns = NULL;
				ChildIns = TopProto->GetChildIns(i);
				if(ChildIns->GetPlcID() == wiPlcId)
				{
					returnIns = ChildIns;
					TopProto = ChildIns->GetProtoType();
					break;
				}
			}
		}

#ifdef __MOBILE__
		cPlcIdP = wcstok(NULL,L"|",&tempP);
#else
		cPlcIdP = wcstok(NULL,L"|");
#endif

		depth++;
	}

	return returnIns;
}

#if 0
/*****************************************************************************
 *	@brief		通过配置路径删除实例
 *	@param[in]	i_wstrPlcPath			配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		其他					删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::DelInstanceByPlcPath(const wstring& i_wstrPlcPath)
{
	Stk_Instance* stkInsP = GetInstanceByPath(i_wstrPlcPath);
	STK_STATUS stat = STK_SUCCESS;
	Stk_ProtoType* stkPaProtoP = NULL;

	if (i_wstrPlcPath != L"PATH|0")
	{
		wstring wstrPaInsPath;	// 上级实例的配置路径
		wstring::size_type index;
		index = i_wstrPlcPath.find_last_of(L"|");
		if (index != -1)
		{
			wstrPaInsPath = i_wstrPlcPath.substr(0, index);

			if (wstrPaInsPath==L"PATH|0") 
			{
				stkPaProtoP = m_DefaultTopProtoP;   //UG、CATIA支持
			}else
			{
				Stk_Instance* stkPaInsP = GetInstanceByPath(wstrPaInsPath);
				stkPaProtoP = stkPaInsP->GetProtoType();
			}
			stkPaProtoP->RemoveChild(stkInsP);
		}
	}

	//解决挂机的缺陷，这是一种临时解决的方案。
	//因为下一步会取消TopInstance，到这时可以彻底解决该缺陷
	for(STK_UINT32 i=0; i<m_vcFiles.size();i++)
	{

// 注意！因为消除顶级实例，所以取消了这个接口。
// 以后File下只有有顶级ProtoType。
// 		if( m_vcFiles[i]->GetTopInstance() != NULL  )
// 		{
// 			// 单一文件
// 			if ( stkInsP == m_vcFiles[i]->GetTopInstance())
// 			{
// 				// 从ID管理器中删除该File
// 				stat = m_vcFiles[i]->Release();
// 				if (stat == STK_SUCCESS)
// 				{
// 					vector<Stk_File*>::iterator itor = m_vcFiles.begin()+i;
// 					m_vcFiles.erase(itor);
// 				}
// 				//m_vcFiles[i]->SetTopInstance((Stk_Instance*)NULL); //m_vcFiles[i]的TopIns没有清空，因此置为NULL
// 				//m_vcFiles[i]->SetTopProtoType((Stk_ProtoType*)NULL);
// 				break;
// 			}
// 		}
// 		else
// 		{
			// 分散文件
			// 注意！支持UG、CATIA存储时，编辑装配结构是以File为单位。
			if (stkInsP->GetProtoType() == m_vcFiles[i]->GetTopProtoTypeByIndex(0))
			{
				// 从ID管理器中删除该File
				stat = m_vcFiles[i]->Release();
				if (stat == STK_SUCCESS)
				{
					vector<Stk_File*>::iterator itor = m_vcFiles.begin()+i;
					m_vcFiles.erase(itor);
				}
				stkInsP->SetProtoID(0);

				if (stkInsP != NULL)
				{
					delete stkInsP;
					stkInsP = NULL;
				}

				break;
			}
//		}
	}

	//if (stkInsP != NULL)
	//{
	//	delete stkInsP;
	//	stkInsP = NULL;
	//}

	return STK_SUCCESS;
}
#endif
/*************************************************************************//**
 *	@brief		通过实例配置路径拖拽实例
 *	@param[in]	i_wstrNowPlcPath		实例的当前配置路径
 *	@param[in]	i_wstrToPlcPath			被拖拽到的上级的配置路径
 *	@param[out]	无
 *	@retval		wstrPlcPath				配置路径,如果返回的wstrPlcPath为空，则表示不允许拖拽
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_DocumentManager::DragAssemble(const wstring& i_wstrNowPlcPath, const wstring& i_wstrToPlcPath)
{
	wstring wstrPlcPath = L"";		// 实例的新配置路径
	Stk_Instance* stkInsP = NULL;	// 被拖拽的实例
	Stk_Instance* stkCurInsP = NULL;// 被拖拽到的路径的上级实例
	Stk_Instance* stkPaInsP = NULL;
	Stk_ProtoType* stkPaProtoP = NULL;
	Stk_ProtoType* stkCurProtoP;	// 被拖拽到的路径的上级Proto
	wstring wstrPaInsPath = L"";	// 上级实例的配置路径
	STK_ID idPlc = 0;
	wstring::size_type index;
	StkProtoTypeEnum nuType;

	/*
	 *	在拖拽之前，首先判断即将要插入到得ProtoType是零件还是装配，如果是零件，则不允许拖拽
	 */
	if (i_wstrToPlcPath ==  L"PATH|0" || i_wstrToPlcPath ==  L"" )
	{
		stkCurProtoP = GetDefaultTopProtoType();
	}
	else
	{
		stkCurInsP = GetInstanceByPath(i_wstrToPlcPath);
		stkCurProtoP = stkCurInsP->GetProtoType();
	}
	nuType = stkCurProtoP->GetType();

	if (nuType == StkProtoTypeEnum::PROTO_TYPE_UNASSY ||
		nuType == StkProtoTypeEnum::PROTO_TYPE_PART)
	{
		return wstrPlcPath;
	}

	//注意！由于拖拽并没有改变ProtoType的计数，因此本接口中不需要进行AddRef处理
	/*
	 *	移除装配关系
	 */
	stkInsP = GetInstanceByPath(i_wstrNowPlcPath);
	index = i_wstrNowPlcPath.find_last_of(L"|");
	if (index != -1)
	{
		wstrPaInsPath = i_wstrNowPlcPath.substr(0, index);
		if (wstrPaInsPath ==  L"PATH|0" || wstrPaInsPath ==  L"" )
		{
			stkPaProtoP = GetDefaultTopProtoType();
		}
		else
		{
			stkPaInsP = GetInstanceByPath(wstrPaInsPath);
			stkPaProtoP = stkPaInsP->GetProtoType();
		}
		stkPaProtoP->RemoveChild(stkInsP); 
	}

	/*
	 *	新建装配关系
	 */
	//if (i_wstrToPlcPath ==  L"PATH|0" || i_wstrToPlcPath ==  L"" )
	//{
	//	stkCurProtoP = GetDefaultTopProtoType();
	//}
	//else
	//{
	//	stkCurInsP = GetInstanceByPath(i_wstrToPlcPath);
	//	stkCurProtoP = stkCurInsP->GetProtoType();
	//}

	idPlc = stkCurProtoP->InsertChild(stkInsP);
	//nuType = stkCurProtoP->GetType();
	//if (nuType == StkProtoTypeEnum::PROTO_TYPE_UNASSY) //改为在Stk_ProtoType::AddChild内设置成ASSY类型 add on 2014.4.17 by gaoqq
	//{
	//	stkCurProtoP->SetType(StkProtoTypeEnum::PROTO_TYPE_ASSY);
	//}

	wstrPlcPath += i_wstrToPlcPath;
	wstrPlcPath += L"|";
	WCHAR wchID[32];
	//_itow_s(idPlc, wchID, 10);
	swprintf(wchID,32, L"%d", idPlc); //支持Andriod
	wstrPlcPath += wchID;

	// 修改BOM结构和默认视图下实例状态的配置路径
	DeleteInsAttrInfo(i_wstrNowPlcPath);

	return wstrPlcPath;
}

/*************************************************************************//**
 *	@brief		通过实例配置路径拷贝实例
 *	@param[in]	i_wstrNowPlcPath		实例的当前配置路径
 *	@param[in]	i_wstrToPlcPath			被拷贝位置的上级的配置路径
 *	@param[out]	o_wstrGenPlcPath		拷贝结束后生成的配置路径
 *	@retval		STK_SUCCESS				拷贝配置路径成功
 *	@retval		其它					拷贝配置路径失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::CopyAssemble(const wstring& i_wstrFormPlcPath, const wstring& i_wstrToPlcPath, wstring& o_wstrGenPlcPath)
{
	STK_STATUS		eState = STK_SUCCESS;
	wstring			wstrPlcPath = L"";		// 实例的新配置路径
	Stk_Instance*	stkInsP = NULL;			// 当前被拷贝的实例
	Stk_Instance*	stkNewInsP = NULL;		// 拷贝生成的新实例
	Stk_ProtoType*	stkChildProtoP = NULL;	// 当前被拷贝的ProtoType对象
	Stk_Instance*	stkCurInsP = NULL;		// 被拷贝到的路径的上级实例
	Stk_ProtoType*	stkCurProtoP;			// 被拷贝到的路径的上级Proto
	STK_ID			idPlc = 0;
	StkProtoTypeEnum nuType = PROTO_TYPE_UNKNOWN;
	STK_UINT32		nuMetaDataNum = 0;
	STK_MTX32		mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };
	STK_RGBA32		rgbaIns;

	/*
	 *	在拷贝之前，首先判断即将要插入位置的ProtoType是零件还是装配，如果是零件，则不允许拷贝
	 */
	if (i_wstrToPlcPath ==  L"PATH|0" || i_wstrToPlcPath ==  L"" )
	{
		stkCurProtoP = GetDefaultTopProtoType();
	}
	else
	{
		stkCurInsP = GetInstanceByPath(i_wstrToPlcPath);
		stkCurProtoP = stkCurInsP->GetProtoType();
	}
	nuType = stkCurProtoP->GetType();

	if (nuType == StkProtoTypeEnum::PROTO_TYPE_UNASSY ||
		nuType == StkProtoTypeEnum::PROTO_TYPE_PART)
	{
		return STK_ERROR;
	}

	/*
	 *	新建装配关系
	 */
	stkInsP = GetInstanceByPath(i_wstrFormPlcPath);
	if (stkInsP == NULL)
	{
		return STK_ERROR;
	}

	// 将被拷贝实例的属性信息，复制到新生成的实例中
	stkNewInsP = new Stk_Instance(this);
	stkNewInsP->RegisterID(STK_NEW_ID);
	stkNewInsP->SetInsName(stkInsP->GetInsName());
	if (stkInsP->HasColor())
	{
		rgbaIns = stkInsP->GetColor();
		stkNewInsP->SetColor(rgbaIns.Red, rgbaIns.Green, rgbaIns.Blue, rgbaIns.Alpha);
	}
	stkNewInsP->SetDisplayState(stkInsP->GetDisplayState());
	if (stkInsP->HasMaterial())
	{
		stkNewInsP->SetMaterial(stkInsP->GetMaterial());
	}
	//stkNewInsP->SetProtoID(stkInsP->GetProtoID());
	stkNewInsP->SetProtoLink(stkInsP->GetProtoLink());
	stkNewInsP->RegProtoLink(stkNewInsP->GetProtoLink());
	stkNewInsP->SetNumber(stkInsP->GetNumber());
	stkNewInsP->SetGUID(stkInsP->GetGUID());
	nuMetaDataNum = stkInsP->GetMetaDataNum();
	for (int ii = 0; ii < nuMetaDataNum; ii++)
	{
		Stk_MetaData* stkMetaDataP = NULL;
		stkMetaDataP = stkInsP->GetMetaDataByIndex(ii);
		if (stkMetaDataP != NULL)
		{
			stkNewInsP->AddMetaData(stkMetaDataP);
		}
	}

	// 设置装配关系
	idPlc = stkCurProtoP->InsertChild(stkNewInsP);
	stkNewInsP->SetPlacement(idPlc, mtxUnit);

	// 增加AddRef，不需要再由外部调用
	stkChildProtoP = stkInsP->GetProtoType();
	stkChildProtoP->AddRef();
	//nuType = stkCurProtoP->GetType();
	//if (nuType == StkProtoTypeEnum::PROTO_TYPE_UNASSY) //改为在Stk_ProtoType::AddChild内设置成ASSY类型 add on 2014.4.17 by gaoqq
	//{
	//	stkCurProtoP->SetType(StkProtoTypeEnum::PROTO_TYPE_ASSY);
	//}

	wstrPlcPath += i_wstrToPlcPath;
	wstrPlcPath += L"|";
	WCHAR wchID[32];
	//_itow_s(idPlc, wchID, 10);
	swprintf(wchID,32, L"%d", idPlc); //支持Andriod
	wstrPlcPath += wchID;

	o_wstrGenPlcPath = wstrPlcPath;

	return eState;
}

/*************************************************************************//**
 *	@brief		通过ID号获取ProtoType
 *	@param[in]	i_idProtoType			ProtoType ID
 *	@param[out]	无
 *	@retval		ProtoPtr				ProtoType对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_DocumentManager::GetProtoTypeByID(STK_ID i_idProtoType)
{
	Stk_ProtoType* ProtoPtr = NULL;

	m_GlobalIDManger->GetProtoTypeByID(i_idProtoType, ProtoPtr);
	return ProtoPtr;
}

/*************************************************************************//**
 *	@brief		通过ID号获取实例Instance
 *	@param[in]	i_idInstance			Instance ID
 *	@param[out]	无
 *	@retval		InstanceP				Instance对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Instance* Stk_DocumentManager::GetInstanceByID(STK_ID i_idInstance)
{
	Stk_Instance* InstanceP = NULL;

	m_GlobalIDManger->GetInstanceByID(i_idInstance, InstanceP);
	return InstanceP;
}

/*************************************************************************//**
 *	@brief		通过ID号获取实例Instance
 *	@param[in]	i_wstrPlcPath			实例配置路径
 *	@param[in]	i_eEntityType			实体类型
 *	@param[in]	i_idEntity				实体ID
 *	@param[out]	无
 *	@retval		stkEntityP				实体对象指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Entity* Stk_DocumentManager::GetEntiyByPath(const wstring& i_wstrPlcPath, StkEntityTypeEnum i_eEntityType, STK_ID i_idEntity)
{
	Stk_Instance	*stkInstanceP = NULL;
	Stk_Entity		*stkEntityP = NULL;
	Stk_Mesh_Com	*stkMeshComP = NULL;
	Stk_Mesh_Face	*stkMeshFaceP = NULL;
	Stk_Mesh_Edge	*stkMeshEdgeP = NULL;
	Stk_PMI			*stkPMIP = NULL;
	STK_ID			idProtoType;


	if ( i_wstrPlcPath==L"PATH|0")
	{
		idProtoType = m_DefaultTopProtoP->GetID();   //UG、CATIA支持
	} 
	else
	{
		stkInstanceP = GetInstanceByPath(i_wstrPlcPath);
		if( stkInstanceP == NULL)
		{
			return stkEntityP;
		}
		idProtoType = stkInstanceP->GetProtoID();
	}

	switch(i_eEntityType)
	{
	case ENTITY_TYPE_MESH_COM:
		m_GlobalIDManger->GetMeshComByID(idProtoType, i_idEntity, stkMeshComP);
		stkEntityP = stkMeshComP;
		break;
	case ENTITY_TYPE_MESH_FACE:
		m_GlobalIDManger->GetMeshFaceByID(idProtoType, i_idEntity, stkMeshFaceP);
		stkEntityP = stkMeshFaceP;
		break;
	case ENTITY_TYPE_MESH_EDG:
		m_GlobalIDManger->GetMeshEdgeByID(idProtoType, i_idEntity, stkMeshEdgeP);
		stkEntityP = stkMeshEdgeP;
		break;
	case ENTITY_TYPE_PMI:
		m_GlobalIDManger->GetPMIByID(idProtoType, i_idEntity, stkPMIP);
		stkEntityP = stkPMIP;
		break;
	default:
		stkEntityP = NULL;
		break;
	}

	return stkEntityP;
}

#if 0
/*************************************************************************//**
 *	@brief		添加新的SVL文件
 *	@param[in]	i_wstrPlcPath			上级配置路径
 *	@param[in]	i_wstrFilePath			要加载的文件路径
 *	@param[out]	o_wstrPlcPath			配置路径。若为空，则装配失败
 *	@retval		ret						STK_SUCCESS		添加成功
 *	@retval		ret						STK_ERROR		添加失败
*	@retval		ret						STK_ERROR_LOOP	存在环路
 *	@note		V2.2版本以前使用此接口
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::AddAssemble(const wstring& i_wstrPlcPath, const wstring& i_wstrFilePath, wstring& o_wstrPlcPath)
{
	STK_STATUS	ret = STK_SUCCESS;
	Stk_Instance* stkTopInsP = NULL;
	Stk_Instance* stkPaInsP = NULL;
	Stk_ProtoType* stkPaProtoP = NULL;
	Stk_File* stkTopFileP = NULL;
	STK_ID idPlc = 0;
	STK_UINT32 nuFilesNum = m_nuFilesNum;
	
	o_wstrPlcPath = L"";

	stkTopFileP = LoadSubFile(i_wstrFilePath, true); //注意，这里不应该新建DocManager
	if(stkTopFileP==NULL)
	{
		ret = STK_ERROR;
		return ret;
	}

// 注意！因为消除顶级实例，所以取消了这个接口。
// 以后File下只有有顶级ProtoType。
// 因此这里无论如何都要创建Instance。

// 	if (m_nuFilesNum == nuFilesNum)	//!< 若打开的为已加载的文件，则需新建顶级Ins
// 	{
		stkTopInsP = new Stk_Instance(this);
		if(stkTopFileP != NULL && stkTopFileP->GetTopProtoTypeByIndex(0))
		{
			stkTopInsP->SetProtoID(stkTopFileP->GetTopProtoTypeByIndex(0)->GetID());
			stkTopInsP->SetInsName(stkTopFileP->GetTopProtoTypeByIndex(0)->GetProtoName());
		}
// 	}
// 	else
// 	{
// 		if(stkTopFileP != NULL && stkTopFileP->GetTopInstance())
// 		{
// 			stkTopInsP = stkTopFileP->GetTopInstance();
// 		}
// 	}

	//环路检测
	ret = CheckLoop(stkTopInsP, i_wstrPlcPath);
	if (ret == STK_ERROR_LOOP)
	{
		return ret;
	}

	// 创建装配关系
	if (i_wstrPlcPath==L"PATH|0")
	{
		stkPaProtoP = m_DefaultTopProtoP;   //UG、CATIA支持
	} 
	else
	{
		stkPaInsP = GetInstanceByPath(i_wstrPlcPath);
		stkPaProtoP = stkPaInsP->GetProtoType();
	}

	idPlc = stkPaProtoP->InsertChild(stkTopInsP);

	o_wstrPlcPath += i_wstrPlcPath;
	o_wstrPlcPath += L"|";
	WCHAR wchID[32];
	//_itow_s(idPlc, wchID, 10);
	swprintf(wchID, 32, L"%d", idPlc); //支持Andriod
	o_wstrPlcPath += wchID;

	return ret;
}
#endif

/*************************************************************************//**
 *	@brief		环路检测
 *	@param[in]	i_stkInsP				要装配的Instance
 *	@param[in]	i_wstrPlcPath			父Instance的配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				没有环路，可以进行装配
 *	@retval		STK_ERROR				存在环路，不能进行装配
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::CheckLoop(Stk_Instance* i_stkInsP, const wstring& i_wstrPlcPath)
{
	Stk_Instance* stkTopInsP = NULL;
	Stk_ProtoType* stkPaProtoP = NULL;
	Stk_ProtoType* stkCurProtoP = NULL;
	wstring wstrPaProName = L"";
	wstring wstrCurProName = L"";
	StkProtoTypeEnum ePaType = PROTO_TYPE_UNKNOWN;
	StkProtoTypeEnum eCurType = PROTO_TYPE_UNKNOWN;
	STK_STATUS ret = STK_SUCCESS;

	// 获取当前ProtoType名称
	if (i_wstrPlcPath==L"PATH|0")
	{
		stkPaProtoP = m_DefaultTopProtoP;   //UG、CATIA支持
	}
	else
	{
		stkTopInsP = GetInstanceByPath(i_wstrPlcPath);
		stkPaProtoP = stkTopInsP->GetProtoType();
	}
	wstrPaProName = stkPaProtoP->GetProtoName();
	ePaType = stkPaProtoP->GetType();

	// 获取父ProtoType名称
	stkCurProtoP = i_stkInsP->GetProtoType();
	if (stkCurProtoP == NULL)
	{
		ret = STK_ERROR_NOMORE_PROTO;
		goto rtn;
	}
	wstrCurProName = stkCurProtoP->GetProtoName();
	eCurType = stkCurProtoP->GetType();

	// 如果同名则存在LOOP
	if (wstrCurProName == wstrPaProName && ePaType == eCurType)		// 与顶级Proto相同，则存在环路，直接跳出，返回错误
	{
		ret = STK_ERROR_LOOP;
		goto rtn;
	}

	// 递归
	for (int ii = 0; ii < stkCurProtoP->GetChildInsNum(); ii ++)
	{
		ret = CheckLoop(stkCurProtoP->GetChildIns(ii), i_wstrPlcPath);
		if (ret != STK_SUCCESS)
		{
			goto rtn;
		}
	}
rtn:
	return ret;
}

/*************************************************************************//**
 *	@brief		往File数组中添加File对象指针
 *	@param[in]	i_stkFileP				文件对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
VOID Stk_DocumentManager::AddFile(Stk_File *i_stkFileP)
{
	if(i_stkFileP != NULL)
	{
		m_nuFilesNum ++;
		m_vcFiles.push_back(i_stkFileP);
	}
}

/*************************************************************************//**
 *	@brief		移除File数组中指定的File，仅删除vector<Stk_File*>数组中关系
 *	@param[in]	i_stkFileP				指定的File指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@note		仅删除vector<Stk_File*>数组中关系
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::RemoveFile(Stk_File* i_stkFileP)
{
	if (i_stkFileP != NULL)
	{
		vector<Stk_File*>::iterator itor = m_vcFiles.begin();
		while(itor != m_vcFiles.end())
		{
			if ((*itor) == i_stkFileP)
			{
				m_vcFiles.erase(itor);
				break;
			}
			itor++;
		}
	}
	return STK_SUCCESS;
}

#ifdef _WINDOWS_

/************************************************************************//**
 *	@brief		获取日志文件类对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_svlgLogFile			日志文件类对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LogFileEx* Stk_DocumentManager::GetSVLLogFile()
{
	return m_svlgLogFile;
}

/************************************************************************//**
 *	@brief		初始化日志文件
 *	@param[in]	i_nLogType				输出日志文件类型
 *	@param[in]	i_nLogLevel				输出日志文件等级
 *	@param[in]	i_wstrLogFullPath		输出日志文件路径
 *	@param[in]	i_stkProtoP				ProtoType对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_DocumentManager::InitSVLLog(STK_INT32 i_nLogType, STK_INT32 i_nLogLevel, wstring i_wstrLogFullPath, Stk_ProtoType* i_stkProtoP)
{
	//获取配置文件到缓存
	wstring	wstrIniPath;
	TCHAR dirBuf[MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	Stk_File*		stkFileP = NULL;

	GetModuleFileName(NULL, dirBuf, MAX_PATH);
	_tsplitpath(dirBuf, drive, dir, fname, ext);
	_tmakepath(dirBuf, drive, dir, _T("SVLLog"), _T("ini"));
	wstrIniPath = dirBuf;

	m_SVLLog.bLogOutput = GetPrivateProfileIntW(L"SVLLogLevelParam", L"LogOutput", 0, wstrIniPath.c_str());
	m_SVLLog.bPrototype = GetPrivateProfileIntW(L"SVLLogLevelParam", L"Prototype", 0, wstrIniPath.c_str());
	m_SVLLog.bBody = GetPrivateProfileIntW(L"SVLLogLevelParam", L"Body", 0, wstrIniPath.c_str());
	m_SVLLog.bPMIAndView = GetPrivateProfileIntW(L"SVLLogLevelParam", L"PMIAndView", 0, wstrIniPath.c_str());
	m_SVLLog.bMesh = GetPrivateProfileIntW(L"SVLLogLevelParam", L"Mesh", 0, wstrIniPath.c_str());
	m_SVLLog.bMetaData = GetPrivateProfileIntW(L"SVLLogLevelParam", L"MetaData", 0, wstrIniPath.c_str());
	
	for (int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		wstring::size_type index;
		wstring	wstrPathName;
		index = i_wstrLogFullPath.find_last_of (L"\\");
		if(index != -1)
		{
			wstrPathName = i_wstrLogFullPath.substr(0, index + 1);
		}
		//wstrPathName += i_stkProtoP->GetProtoName();
		stkFileP = i_stkProtoP->GetFile();
		if (stkFileP == NULL)
		{
			return;
		}
		wstrPathName += stkFileP->GetCADFileName();
		wstrPathName += L"_SVL.log";
		// 删除原有的SVL Log文件
		DeleteFile(wstrPathName.c_str());

		if(!wstrPathName.empty() && 0 != wstrPathName.at(0))
		{
			m_svlgLogFile->SetLogFileName(wstrPathName.c_str());
		}

		m_svlgLogFile->PrintLogType(i_nLogType);
		m_svlgLogFile->PrintLogLevel(i_nLogLevel);
	}
}

/************************************************************************//**
 *	@brief		获取日志配置参数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_SVLLog				日志配置参数
 *	@note		无
 *	@attention	无
 ****************************************************************************/

SVLLog& Stk_DocumentManager::GetSVLLogIni()
{
	return m_SVLLog;
}

#endif

/************************************************************************//**
 *	@brief		另存SVL装配文件
 *	@param[in]	i_wstrSavePath		保存的路径
 *	@param[in]	i_eCADFileSaveType	保存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
  *	@retval		其它				保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SaveDocumentAs(wstring i_wstrSavePath, StkFileSaveTypeEnum i_eCADFileSaveType)
{
	STK_STATUS eState = STK_SUCCESS;
	m_eSaveType = i_eCADFileSaveType;
	m_wstrSavePath = i_wstrSavePath;
	STK_UINT32 nuPos = 0;
	wstring wstrNewSaveDir = L"";
	wstring wstrCADFileName = L"";

	m_eDocumentSaveType = DOCUMENT_SAVE_TYPE_SAVEAS;

	if (m_vcTopProtoTypes.size() <= 0)
	{
		eState = STK_ERROR_NO_TOPPROTO;
		return eState;
	}

#ifdef _WINDOWS_
	InitSVLLog(1, 2, i_wstrSavePath, m_vcTopProtoTypes[0]);			// Log
#endif

	/*
	 * 变更所有File的SVL保存路径
	 */
	SetAllSVLFilePath(i_wstrSavePath, i_eCADFileSaveType);

	// 如果保存类型为单一存储/混合存储，需要删除原有的SVL文件
	if (i_eCADFileSaveType == FILESAVE_TYPE_UNIQUE || i_eCADFileSaveType == FILESAVE_TYPE_BEFOR)
	{
		DeleteSVLFile();
	}

	//注意！在保存时刻，SVLFileList作为已保存SVL文件的临时工作区
	ClearSVLSaveList();
	// 设置需保存的所有ProtoType为脏数据
	m_GlobalIDManger->SetModifiedFlag(TRUE);

	/*
	 *	遍历File管理表(m_vcFiles)并写文件
	 */
	for (int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		eState = AddToSVLSaveList(m_vcFiles[ii]->GetSVLSaveAsPath());
		if (eState == STK_SUCCESS_EXIST)
		{
			//向已存在SVL文件追加写入File
			eState = m_vcFiles[ii]->WriteCADFile(i_eCADFileSaveType, true/*AppendFlag*/);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}
		else
		{
			//新建SVL文件
			eState = m_vcFiles[ii]->WriteCADFile(i_eCADFileSaveType, false);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}
		
	}
	ClearSVLSaveList();

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		保存CAD装配
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
  *	@retval		其它				保存失败
 *	@note		保存命令使用此接口
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SaveDocument()
{
	Stk_ProtoType*		stkTopPrototypeP = NULL;
	wstring				wstrSVLPath = L"";
	STK_STATUS			eState = STK_SUCCESS;

	m_eDocumentSaveType = DOCUMENT_SAVE_TYPE_SAVE;
	m_wstrSavePath = m_wstrLoadPath;

	if (m_vcTopProtoTypes.size() <=0 )
	{
		eState = STK_ERROR_NO_TOPPROTO;
		return eState;
	}

#ifdef _WINDOWS_
	InitSVLLog(1, 2, m_wstrSavePath, m_vcTopProtoTypes[0]);			// Log
#endif

	//注意！在保存时刻，SVLFileList作为已保存SVL文件的临时工作区
	ClearSVLSaveList();

	/*
	 *	设置需保存的所有ProtoType为脏数据
	 */
	m_GlobalIDManger->SetModifiedFlag(TRUE);

	/*
	 *	遍历File管理表(m_vcFiles)并写文件
	 */
	for (int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		eState = AddToSVLSaveList(m_vcFiles[ii]->GetSVLPath());
		if (eState == STK_SUCCESS_EXIST)
		{
			//向已存在SVL文件追加写入File
			eState = m_vcFiles[ii]->WriteCADFile(FILESAVE_TYPE_BEFOR, true/*AppendFlag*/);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}
		else
		{
			//新建SVL文件
			eState = m_vcFiles[ii]->WriteCADFile(FILESAVE_TYPE_BEFOR, false);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}

	}
	ClearSVLSaveList();

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		为所有SVL设置新的路径
 *	@param[in]	i_wstrSavePath		将要保存的路径
 *	@param[in]	i_eSaveType			保存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::SetAllSVLFilePath(wstring i_wstrSavePath, StkFileSaveTypeEnum i_eSaveType)
{
	STK_UINT32			nuPos = 0;
	STK_UINT32			nuTopPrototypeNum = 0;
	Stk_ProtoType*		stkTopPrototypeP = NULL;
	wstring				wstrNewSaveDir = L"";
	wstring				wstrCADFileName = L"";
	wstring				wstrSVLFileName = L"";
	StkProtoTypeEnum	eProtoType = PROTO_TYPE_UNKNOWN;
	STK_BOOL			bIsTopFile = false;

	nuPos = i_wstrSavePath.find_last_of(L"\\");
	wstrNewSaveDir = i_wstrSavePath.substr(0, nuPos + 1);

	// 单一时强制修改为“指定路径i_wstrSavePath”；
	if ( i_eSaveType == FILESAVE_TYPE_UNIQUE)
	{
		for (int ii = 0; ii < m_vcFiles.size(); ii++)
		{
			m_vcFiles[ii]->SetSVLSaveAsPath( i_wstrSavePath );
		}
	}
	// 分散时强制修改为“指定文件夹+CADFileName.svl”；
	else if (  i_eSaveType == FILESAVE_TYPE_DISTRIBUTION )
	{
		for (int ii = 0; ii < m_vcFiles.size(); ii++)
		{
			wstrCADFileName = m_vcFiles[ii]->GetCADFileName();
			wstrCADFileName += L".svl";
			bIsTopFile = m_vcFiles[ii]->IsTopFile();
			if (bIsTopFile)
			{
				m_vcFiles[ii]->SetSVLSaveAsPath(i_wstrSavePath);
				bIsTopFile = false;
			}
			else
			{
				m_vcFiles[ii]->SetSVLSaveAsPath( wstrNewSaveDir + wstrCADFileName);
			}
		}
	}
	// 混合时仅变更文件夹，文件名不修改
	else if (  i_eSaveType == FILESAVE_TYPE_BEFOR )
	{
		for (int ii = 0; ii < m_vcFiles.size(); ii++)
		{
			wstrSVLFileName = m_vcFiles[ii]->GetSVLPath();
			nuPos = wstrSVLFileName.find_last_of(L"\\");
			wstrSVLFileName = wstrSVLFileName.substr( nuPos,wstrSVLFileName.length() - 1 );
			if (bIsTopFile)
			{
				m_vcFiles[ii]->SetSVLSaveAsPath(i_wstrSavePath);
				bIsTopFile = false;
			}
			else
			{
				m_vcFiles[ii]->SetSVLSaveAsPath( wstrNewSaveDir + wstrSVLFileName);
			}
		}
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		将SVL文件路径添加到文件列表中
 *	@param[in]	i_wstrSVLFilePath		SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				注册成功
 *	@retval		STK_SUCCESS_EXIST		已经存在
 *	@note		用于保存SVL文件时用
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::AddToSVLSaveList(wstring i_wstrSVLFilePath)
{
	map<wstring, STK_BOOL>::iterator itor = m_mapSVLSaveList.find(i_wstrSVLFilePath);
	if (itor != m_mapSVLSaveList.end())
	{
		return STK_SUCCESS_EXIST;
	}
	m_mapSVLSaveList.insert(pair<wstring, STK_BOOL>(i_wstrSVLFilePath, true));
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		清空SVL文件列表中的路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				清空成功
 *	@retval		其他					清空失败
 *	@note		用于保存SVL文件时用
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::ClearSVLSaveList()
{
	m_mapSVLSaveList.clear();

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		通过SVL路径查询文件中的顶级Stk_File指针
 *	@param[in]	i_wstrSVLFilePathLoad		SVL文件路径
 *	@param[out]	无
 *	@retval		顶级Stk_File指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

 Stk_File*	Stk_DocumentManager::InqTopFileFromSVLLoadList(wstring i_wstrSVLFilePathLoad)
{
	map<wstring, Stk_File*>::iterator itor = m_mapSVLLoadList.find(i_wstrSVLFilePathLoad);
	if (itor != m_mapSVLLoadList.end())
	{
		return itor->second;
	}
	else
	{
		return NULL;
	}
}

/*************************************************************************//**
 *	@brief		插入SVL文件路径
 *	@param[in]	i_wstrSVLFilePathLoad		SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS					注册成功
 *	@retval		STK_SUCCESS_EXIST			已经存在
 *	@note		用于打开/插入SVL文件时用
 *	@attention	无
 ****************************************************************************/

STK_STATUS	Stk_DocumentManager::AddToSVLLoadList(wstring i_wstrSVLFilePathLoad, Stk_File* i_stkTopFile)
{
	map<wstring, Stk_File*>::iterator itor = m_mapSVLLoadList.find(i_wstrSVLFilePathLoad);
	if (itor != m_mapSVLLoadList.end())
	{
		return STK_SUCCESS_EXIST;
	}

	m_mapSVLLoadList.insert(pair<wstring, Stk_File*>(i_wstrSVLFilePathLoad, i_stkTopFile));
	
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		删除SVL文件列表中的路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		其他					删除失败
 *	@note		用于打开/插入SVL文件时用
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::ClearSVLLoadList()
{
	m_mapSVLLoadList.clear();

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		从LoadList中删除指定SVL文件路径
 *	@param[in]	i_wstrSVLFilePath		要删除的SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		其他					删除失败
 *	@note		用于打开/插入SVL文件时用
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::DelSVLFileFromLoadList(wstring i_wstrSVLFilePath)
{
	map<wstring, Stk_File*>::iterator itor = m_mapSVLLoadList.find(i_wstrSVLFilePath);
	if (itor != m_mapSVLLoadList.end())
	{
		m_mapSVLLoadList.erase(itor);
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		删除原有的SVL物理文件
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@note		目的是在保存或另存时避免出现将现有SVL数据写入原来已存在的SVL文件中
 *	@attention	无
 ****************************************************************************/
// ###
STK_STATUS Stk_DocumentManager::DeleteSVLFile()
{
	string	strSVLPath = "";
	for (int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		strSVLPath = Stk_Platform::WStringToString(m_vcFiles[ii]->GetSVLSaveAsPath());
		remove(strSVLPath.c_str()); //@@@ Android 可以？
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		ProtoType计数
 *	@param[in]	i_idProto				ProtoType ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS				计数成功
 *	@note		ID采用预分配方式，计数器根据预分配的ID进行
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::AddProtoRef(STK_ID i_idProto)
{
	map<STK_ID, STK_UINT32>::iterator itor = m_mapLoadProtoRef.find(i_idProto);
	if (itor != m_mapLoadProtoRef.end())
	{
		itor->second++;
	}
	else
	{
		m_mapLoadProtoRef.insert(pair<STK_ID, STK_UINT32>(i_idProto, 1));
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		清空临时的ProtoType计数器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				清空成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::ClearProtoRef()
{
	m_mapLoadProtoRef.clear();

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		通过配置路径删除实例及子实例
 *	@param[in]	i_wstrPlcPath			实例配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS				删除成功
 *	@retval		STK_SUCCESS_EXIST		实例不存在
 *	@retval		其他					删除失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::DelInstanceByPlcPath(const wstring& i_wstrPlcPath)
{
	Stk_Instance*	stkInsP = GetInstanceByPath(i_wstrPlcPath);
	if (stkInsP == NULL)
	{
		return STK_SUCCESS_EXIST;
	}
	STK_STATUS		stat = STK_SUCCESS;
	Stk_File*		stkFileP = NULL;
	Stk_ProtoType*	stkPaProtoP = NULL;
	Stk_ProtoType*	stkCurProtoTypeP = NULL;
	STK_UINT32		nuTopProtoTypeNum = 0;


	if (i_wstrPlcPath != L"PATH|0")
	{
		wstring wstrPaInsPath;	// 上级实例的配置路径
		wstring::size_type index;
		index = i_wstrPlcPath.find_last_of(L"|");
		if (index != -1)
		{
			wstrPaInsPath = i_wstrPlcPath.substr(0, index);

			if (wstrPaInsPath==L"PATH|0") 
			{
				stkPaProtoP = m_DefaultTopProtoP;   //UG、CATIA支持
			}else
			{
				Stk_Instance* stkPaInsP = GetInstanceByPath(wstrPaInsPath);
				stkPaProtoP = stkPaInsP->GetProtoType();
			}
			stkPaProtoP->RemoveChild(stkInsP);
		}
	}

	if (stkInsP != NULL)
	{
		delete stkInsP;
		stkInsP = NULL;
	}

	// 删除实例属性信息
	DeleteInsAttrInfo(i_wstrPlcPath);

	return STK_SUCCESS;
}

/*****************************************************************************
 *	@brief		添加新的SVL文件
 *	@param[in]	i_wstrPlcPath			上级配置路径
 *	@param[in]	i_stkProtoTypeP			为ProtoType创建Instance并建立关联
 *	@param[out]	o_wstrPlcPath			配置路径。若为空，则装配失败
 *	@retval		ret						STK_SUCCESS		添加成功
 *	@retval		ret						STK_ERROR		添加失败
 *	@retval		ret						STK_ERROR_LOOP	存在环路
 *	@note		由于V2.2后SVL文件可能存在多个顶级ProtoType，因此需要用户指定哪个作为默认顶级ProtoType以插入。<br>
 *				因此添加新SVL文件的应用，需要调用DataManager的两个接口来实现：<br>
 *				1. 通过Stk_DocumentManager::LoadCADSubFile()来加载子SVL文件，返回顶级ProtoType列。<br>
 *				2. 从ProtoType列中选取一个作为默认顶级ProtoType，通过通过Stk_DocumentManager::AddAssemble()来插入。<br>
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_DocumentManager::AddAssemble(const wstring& i_wstrPlcPath, Stk_ProtoType* i_stkProtoTypeP, wstring& o_wstrPlcPath)
{
	STK_STATUS			ret = STK_SUCCESS;
	Stk_Instance*		stkCurInsP = NULL;
	Stk_Instance*		stkPaInsP = NULL;
	Stk_ProtoType*		stkPaProtoP = NULL;
	STK_ID				idPlc = 0;
	STK_UINT32			nuFilesNum = m_nuFilesNum;
	StkProtoTypeEnum	eType = PROTO_TYPE_UNKNOWN;

	o_wstrPlcPath = L"";
	if(i_stkProtoTypeP == NULL || i_stkProtoTypeP->GetFile() == NULL)
	{
		return STK_ERROR;
	}

	// 新建Instance
	stkCurInsP = new Stk_Instance(this);
	stkCurInsP->SetFilePath(i_stkProtoTypeP->GetFile()->GetSVLPath());
	stkCurInsP->SetProtoLink(i_stkProtoTypeP->GetLinkString());
	stkCurInsP->RegisterID(STK_NEW_ID);
	stkCurInsP->SetInsName(i_stkProtoTypeP->GetProtoName());

	//建立与传入的顶级ProtoType的关联
	stkCurInsP->SetProtoID(i_stkProtoTypeP->GetID());
	//i_stkProtoTypeP->AddRef(); // LoadCADSubFile时已对顶级ProtoType进行AddRef

	//环路检测
	ret = CheckLoop(stkCurInsP, i_wstrPlcPath);
	if (ret == STK_ERROR_LOOP)
	{
		return ret;
	}

	// 创建装配关系
	if (i_wstrPlcPath==L"PATH|0")
	{
		stkPaProtoP = m_DefaultTopProtoP;   //UG、CATIA支持
	}
	else
	{
		stkPaInsP = GetInstanceByPath(i_wstrPlcPath);
		stkPaProtoP = stkPaInsP->GetProtoType();
	}

	// 如果被插入对象是零件，则不允许插入
	eType = stkPaProtoP->GetType();
	if (eType == StkProtoTypeEnum::PROTO_TYPE_UNASSY || eType == StkProtoTypeEnum::PROTO_TYPE_PART)
	{
		return STK_ERROR_FILE_ADD;
	}

	idPlc = stkPaProtoP->InsertChild(stkCurInsP);

	o_wstrPlcPath += i_wstrPlcPath;
	o_wstrPlcPath += L"|";
	WCHAR wchID[32];
	swprintf(wchID, 32, L"%d", idPlc); //支持Andriod
	o_wstrPlcPath += wchID;

	return ret;
}

/*****************************************************************************
 *	@brief		插入空白ProtoType
 *	@param[in]	i_wstrPlcPath			上级配置路径
 *	@param[in]	i_wstrProtoName			ProtoType名称
 *	@param[in]	i_eType					ProtoType类型
 *	@param[out]	o_wstrPlcPath			配置路径。若为空，则装配失败
 *	@retval		ret						STK_SUCCESS		添加成功
 *	@retval		ret						STK_ERROR		发现同名且同类型的ProtoType，添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::AddNewProto(const wstring& i_wstrPlcPath, const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType,wstring& o_wstrPlcPath)
{
	STK_STATUS	ret = STK_SUCCESS;
	Stk_Instance* stkPaInsP = NULL;
	Stk_Instance* stkNewInsP = NULL;
	Stk_File* stkPaFile = NULL;
	Stk_ProtoType* stkPaProtoP = NULL;
	Stk_ProtoType* stkNewProtoP = NULL;
	Stk_File* stkTopFileP = NULL;
	STK_ID idPlc = 0;
	STK_UINT32 nuFilesNum = m_nuFilesNum;
	wstring wstrCurLink;
	WCHAR wchTmpSTR[STK_MAXNAME];
	o_wstrPlcPath = L"";
	STK_STATUS eRet;
	STK_MTX32 mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };

	/*
	 *	查询父ProtoType
	 */
	if (i_wstrPlcPath ==  L"PATH|0" || i_wstrPlcPath ==  L""  )
	{
		return STK_ERROR;
	}

	stkPaInsP = GetInstanceByPath(i_wstrPlcPath);
	stkPaProtoP = stkPaInsP->GetProtoType();
	stkPaFile = stkPaProtoP->GetFile();
	if (stkPaFile == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	检测ProtoTypeName是否唯一
	 */
	eRet = CheckProtoExist(i_wstrProtoName,i_eType);
	if (eRet==STK_ERROR)
	{
		return STK_ERROR;
	}

	/*
	 *	新建ProtoType
	 */
	wstrCurLink.erase();
	wstrCurLink += i_wstrProtoName;
	if (i_eType == StkProtoTypeEnum::PROTO_TYPE_ASSY)
	{
		wstrCurLink += L"*ASM";
	}
	else if (i_eType == StkProtoTypeEnum::PROTO_TYPE_PART ||
		i_eType == StkProtoTypeEnum::PROTO_TYPE_UNASSY)
	{
		wstrCurLink += L"*PRT";
	}
	else if (i_eType == StkProtoTypeEnum::PROTO_TYPE_SHEET)
	{
		wstrCurLink += L"*SHT";
	}
	wstrCurLink += L"|NewProto";
	stkNewInsP = new Stk_Instance(this);
	stkNewInsP->SetProtoLink(wstrCurLink );
	stkNewInsP->RegisterID(STK_NEW_ID);
	//stkNewInsP->SetFilePath(i_strFilePath); //新建的ProtoType不需要设置FilePath
	stkNewInsP->SetPlacement(0, mtxUnit);
	stkNewInsP->SetInsName(i_wstrProtoName);

	eRet = stkNewInsP->RegProtoLink(wstrCurLink);
	if (eRet == STK_SUCCESS_EXIST)
	{
		return STK_ERROR;
	}

	stkNewProtoP = new Stk_ProtoType(wstrCurLink, this);
	stkNewProtoP->RegisterID(STK_NEW_ID);
	stkNewProtoP->SetFile(stkPaFile);
	stkNewProtoP->SetProtoName(i_wstrProtoName);
	stkNewProtoP->SetType(i_eType);

	/*
	 * 新建File
	 */
	Stk_File* stkNewFileP = new Stk_File(this);
	stkNewFileP->AddTopProtoType(stkNewProtoP);
	stkNewFileP->SetCADFilePath(wstrCurLink);
	stkNewProtoP->SetFile(stkNewFileP);
	m_vcFiles.push_back(stkNewFileP);

	/* 
	 * 创建装配关系
	 */
	idPlc = stkPaProtoP->InsertChild(stkNewInsP);
	stkNewProtoP->AddRef();

	o_wstrPlcPath += i_wstrPlcPath;
	o_wstrPlcPath += L"|";
	WCHAR wchID[32];
	//_itow_s(idPlc, wchID, 10);
	swprintf(wchID, 32, L"%d", idPlc); //支持Andriod
	o_wstrPlcPath += wchID;

	return ret;
}

/*****************************************************************************
 *	@brief		ProtoType重名检测
 *	@param[in]	i_stkInsP				顶级Instance
 *	@param[in]	i_wstrProtoName			ProtoType名称
 *	@param[in]	StkProtoTypeEnum		ProtoType类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS				不存在相同ProtoType
  *	@retval		STK_ERROR				存在相同ProtoType
 *	@note		请确保Stk_DocumentManager已经SetDefaultTopProto
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::CheckProtoExist( const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType)
{
	vector <Stk_ProtoType*> vecStack;	// 堆栈
	Stk_ProtoType* stkCurProtoP = NULL;
	Stk_ProtoType* stkChildProtoP = NULL;
	Stk_Instance* stkChildInsP = NULL;
	STK_STATUS ret = STK_SUCCESS;
	STK_UINT32	sz = 0;
	STK_UINT32	i = 0;

	if (m_DefaultTopProtoP == NULL)
	{
		return STK_ERROR;
	}

	vecStack.push_back(m_DefaultTopProtoP);
	while(vecStack.size()>0)
	{
		// 当前ProtoType
		stkCurProtoP = *(vecStack.rbegin());
		vecStack.pop_back();

		if (stkCurProtoP->GetProtoName() == i_wstrProtoName
			&& stkCurProtoP->GetType() == i_eType)
		{
			ret = STK_ERROR; //发现重复的ProtoType名
			break;
		}

		// 遍历下级ProtoType
		sz = stkCurProtoP->GetChildInsNum();
		for (i=0; i<sz; i++)
		{
			stkChildProtoP = stkCurProtoP->GetChildIns(i)->GetProtoType();
			if (stkChildProtoP==NULL)
				continue;
			vecStack.push_back(stkChildProtoP);
		}
	}

rtn:
	return ret;
}

#if 0 //@@@ 需要针对V2.2进行修改
/*****************************************************************************
 *	@brief		插入空白ProtoType
 *	@param[in]	i_wstrPlcPath			上级配置路径
 *	@param[in]	i_wstrProtoName			ProtoType名称
 *	@param[in]	i_eType					ProtoType类型
 *	@param[out]	o_wstrPlcPath			配置路径。若为空，则装配失败
 *	@retval		ret						STK_SUCCESS		添加成功
 *	@retval		ret						STK_ERROR		发现同名且同类型的ProtoType，添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::AddNewProto(const wstring& i_wstrPlcPath, const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType,wstring& o_wstrPlcPath)
{
	STK_STATUS	ret = STK_SUCCESS;
	Stk_Instance* stkPaInsP = NULL;
	Stk_Instance* stkNewInsP = NULL;
	Stk_File* stkPaFile = NULL;
	Stk_ProtoType* stkPaProtoP = NULL;
	Stk_ProtoType* stkNewProtoP = NULL;
	Stk_File* stkTopFileP = NULL;
	STK_ID idPlc = 0;
	STK_UINT32 nuFilesNum = m_nuFilesNum;
	wstring wstrCurLink;
	WCHAR wchTmpSTR[STK_MAXNAME];
	o_wstrPlcPath = L"";
	STK_STATUS eRet;
	STK_MTX32 mtxUnit = { 1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1 };

	/*
	 *	查询父ProtoType
	 */
	stkPaInsP = GetInstanceByPath(i_wstrPlcPath);
	stkPaProtoP = stkPaInsP->GetProtoType();
	stkPaFile = stkPaProtoP->GetFile();

	/*
	 *	检测ProtoTypeName是否唯一
	 */
	eRet = CheckProtoExist(i_wstrProtoName,i_eType);
	if (eRet==STK_ERROR)
	{
		return STK_ERROR;
	}

	/*
	 *	新建ProtoType
	 */
	wstrCurLink.erase();
	wstrCurLink += i_wstrProtoName;
	wstrCurLink += L"|NewProto";
	stkNewInsP = new Stk_Instance(this);
	stkNewInsP->SetProtoLink(wstrCurLink );
	stkNewInsP->RegisterID(STK_NEW_ID);
	//stkNewInsP->SetFilePath(i_strFilePath); //新建的ProtoType不需要设置FilePath
	stkNewInsP->SetPlacement(0, mtxUnit);
	stkNewInsP->SetInsName(i_wstrProtoName);

	eRet = stkNewInsP->RegProtoLink(wstrCurLink);
	if (eRet == STK_SUCCESS_EXIST)
	{
		return STK_ERROR;
	}

	stkNewProtoP = new Stk_ProtoType(wstrCurLink,this);
	stkNewProtoP->RegisterID(STK_NEW_ID);
	stkNewProtoP->SetFile(stkPaFile);
	stkNewProtoP->SetProtoName(i_wstrProtoName);
	stkNewProtoP->SetType(i_eType);

	/* 
	 * 创建装配关系
	 */
	idPlc = stkPaProtoP->InsertChild(stkNewInsP);
	stkNewProtoP->AddRef();

	o_wstrPlcPath += i_wstrPlcPath;
	o_wstrPlcPath += L"|";
	WCHAR wchID[32];
	//_itow_s(idPlc, wchID, 10);
	swprintf(wchID, 32, L"%d", idPlc); //支持Andriod
	o_wstrPlcPath += wchID;


	return ret;
}

/*****************************************************************************
 *	@brief		ProtoType重名检测
 *	@param[in]	i_stkInsP				顶级Instance
 *	@param[in]	i_wstrProtoName			ProtoType名称
 *	@param[in]	StkProtoTypeEnum		ProtoType类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS				不存在相同ProtoType
  *	@retval		STK_ERROR				存在相同ProtoType
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::CheckProtoExist( const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType)
{
	vector <Stk_ProtoType*> vecStack;	// 堆栈
	Stk_ProtoType* TopProtoTypeP = m_TopInstanceP->GetProtoType();
	Stk_ProtoType* stkCurProtoP = NULL;
	Stk_ProtoType* stkChildProtoP = NULL;
	Stk_Instance* stkChildInsP = NULL;
	STK_STATUS ret = STK_SUCCESS;
	STK_UINT32	sz = 0;
	STK_UINT32	i = 0;

	vecStack.push_back(TopProtoTypeP);
	while(vecStack.size()>0)
	{
		// 当前ProtoType
		stkCurProtoP = *(vecStack.rbegin());
		vecStack.pop_back();

		if (stkCurProtoP->GetProtoName() == i_wstrProtoName
			&& stkCurProtoP->GetType() == i_eType)
		{
			ret = STK_ERROR; //发现重复的ProtoType名
			break;
		}

		// 遍历下级ProtoType
		sz = stkCurProtoP->GetChildInsNum();
		for (i=0; i<sz; i++)
		{
			stkChildProtoP = stkCurProtoP->GetChildIns(i)->GetProtoType();
			if (stkChildProtoP==NULL)
				continue;
			vecStack.push_back(stkChildProtoP);
		}
	}

rtn:
	return ret;
}

#endif

STK_STATUS Stk_DocumentManager::ConvertTitleToCADFileTitle(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP)
{
	i_stkCADFileP->SetTitle(i_stkFirstFileP->GetTitle());
	// 旧版本没有CAD FileName，因此使用ProtoType Name代替
	i_stkCADFileP->SetDataSource(i_stkFirstFileP->GetDataSource());
	i_stkCADFileP->SetConvTookit(i_stkFirstFileP->GetConvTookit());
	i_stkCADFileP->SetSVLConversion(i_stkFirstFileP->GetSVLConversion());
	i_stkCADFileP->SetSVLVersion(i_stkFirstFileP->GetSVLVersion());
	i_stkCADFileP->SetSVLTime(i_stkFirstFileP->GetSVLTime());
	i_stkCADFileP->SetPolygonNum(i_stkFirstFileP->GetPolygonNum());
	i_stkCADFileP->SetProtoTypeNum(i_stkFirstFileP->GetProtoTypeNum());
	//i_stkCADFileP->SetCADFilePath(i_stkFirstFileP->GetCADFilePath());

	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentManager::ConvertPreViewToCADFilePreView(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP)
{
	Stk_Preview* stkFirstPreViewP = i_stkFirstFileP->GetFilePreview();
	if (stkFirstPreViewP == NULL)
	{
		return STK_ERROR_ID_NOT_FOUND;
	}

	Stk_Preview* stkCADFilePreViewP = new Stk_Preview();

	stkCADFilePreViewP->SetType(stkFirstPreViewP->GetType());
	stkCADFilePreViewP->SetStreamSize(stkFirstPreViewP->GetStreamSize());
	stkCADFilePreViewP->SetStream(stkFirstPreViewP->GetStream());

	i_stkCADFileP->SetFilePreview(stkCADFilePreViewP);

	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentManager::ConvertOptionToCADFileOption(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP)
{
	return STK_SUCCESS;
}

StkFileSaveTypeEnum Stk_DocumentManager::ComputeFileSaveType(Stk_ProtoType* i_stkProtoTypeP)
{
	STK_UINT32		nuChildInsNum = 0;
	wstring			wstrCurrentSVLPath = L"";
	wstring			wstrChildSVLPath = L"";
	Stk_Instance*	stkInstanceP = NULL;
	Stk_File*		stkFileP = NULL;

	nuChildInsNum = i_stkProtoTypeP->GetChildInsNum();
	stkFileP = i_stkProtoTypeP->GetFile();
	if (stkFileP != NULL)
	{
		wstrCurrentSVLPath = stkFileP->GetSVLPath();
	}
	
	m_eSaveType = FILESAVE_TYPE_UNIQUE;
	for (int ii = 0; ii < nuChildInsNum; ii++)
	{
		stkInstanceP = i_stkProtoTypeP->GetChildIns(ii);
		wstrChildSVLPath = stkInstanceP->GetFilePath();
		if (wstrChildSVLPath != wstrCurrentSVLPath)
		{
			m_eSaveType = FILESAVE_TYPE_DISTRIBUTION;
			break;
		}
	}
	return m_eSaveType;
}

/************************************************************************//**
 *	@brief		删除配置路径所对应的上级所有实例属性信息
 *	@param[in]	i_wstrInsPlcPath	实例配置路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			删除成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::DeleteInsAttrInfo(wstring i_wstrInsPlcPath)
{
	Stk_Instance*		stkInsP = NULL;
	Stk_ProtoType*		stkProtoTypeP = NULL;
	Stk_View*			stkViewP = NULL;
	vector<wstring>		vcInsAttrPlcPaths;
	wstring				wstrParaPlcPath = L"";
	//StkDisplayStateEnum	eDisplayState;
	//STK_RGBA32			rgbaIns;
	//STK_MAT32			matIns;
	for(int ii = 0; ii < i_wstrInsPlcPath.size(); ii++)
	{
		if (i_wstrInsPlcPath.at(ii) == L'|')
		{
			wstrParaPlcPath = i_wstrInsPlcPath.substr(0, ii);
			if (wstrParaPlcPath == L"PATH")
			{
				continue;
			}
			if (wstrParaPlcPath == L"PATH|0")
			{
				stkProtoTypeP = GetDefaultTopProtoType();
				vcInsAttrPlcPaths = stkProtoTypeP->GetAllInsAttrPlcPath();
				for (int jj = 0; jj < vcInsAttrPlcPaths.size(); jj++)
				{
					if (vcInsAttrPlcPaths[jj] == i_wstrInsPlcPath)
					{
						stkProtoTypeP->DeleteInsAttrByPlcPath(i_wstrInsPlcPath);
						stkViewP = stkProtoTypeP->GetDefaultView();
						if (stkViewP != NULL)
						{
							stkViewP->DeleteInsAttrByPlcPath(i_wstrInsPlcPath);
						}
						break;
					}
				}
				continue;
			}
			stkInsP = GetInstanceByPath(wstrParaPlcPath);
			stkProtoTypeP = stkInsP->GetProtoType();
			vcInsAttrPlcPaths = stkProtoTypeP->GetAllInsAttrPlcPath();
			for (int jj = 0; jj < vcInsAttrPlcPaths.size(); jj++)
			{
				if (vcInsAttrPlcPaths[jj] == i_wstrInsPlcPath)
				{
					stkProtoTypeP->DeleteInsAttrByPlcPath(i_wstrInsPlcPath);
					stkViewP = stkProtoTypeP->GetDefaultView();
					if (stkViewP != NULL)
					{
						stkViewP->DeleteInsAttrByPlcPath(i_wstrInsPlcPath);
					}
					break;
				}
			}
		}
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		另存单个SVL装配文件
 *	@param[in]	i_wstrSavePath		另存路径
 *	@param[in]	i_stkFileP			ProtoType所属的File文件对象
 *	@param[in]	i_eFileSaveType		SVL文件另存类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			保存成功
  *	@retval		其它				保存失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SaveSingleFileAs(wstring i_wstrSavePath, Stk_File* i_stkFileP, StkFileSaveTypeEnum i_eFileSaveType)
{
	STK_STATUS				eState = STK_SUCCESS;
	STK_UINT32				nuTopProtoTypeNum = 0;
	STK_UINT32				nuChildInsNum = 0;
	Stk_ProtoType*			stkTopProtoTypeP = NULL;
	Stk_Instance*			stkChildInstanceP = NULL;
	vector<Stk_File*>		vcFiles;
	map<Stk_File*, STK_BOOL> mapFiles;

	m_eDocumentSaveType = DOCUMENT_SAVE_TYPE_SAVEAS;
	vcFiles.push_back(i_stkFileP);
	/*
	 *	遍历File下的所有子File文件
	 */
	eState = SearchFilesOfAssembly(i_stkFileP, mapFiles);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	map<Stk_File*, STK_BOOL>::iterator itor = mapFiles.begin();
	while(itor != mapFiles.end())
	{
		vcFiles.push_back(itor->first);
		itor++;
	}
	/*
	 * 变更所有File的SVL保存路径
	 */
	SetGivenFilePath(i_wstrSavePath, i_eFileSaveType, vcFiles);

	// 如果保存类型为单一存储/混合存储，需要删除原有的SVL文件
	if (i_eFileSaveType == FILESAVE_TYPE_UNIQUE || i_eFileSaveType == FILESAVE_TYPE_BEFOR)
	{
		DeleteGivenSVLFile(vcFiles);
	}

	//注意！在保存时刻，SVLFileList作为已保存SVL文件的临时工作区
	ClearSVLSaveList();
	// 设置需保存的所有ProtoType为脏数据
	m_GlobalIDManger->SetModifiedFlag(TRUE);

	/*
	 *	遍历File管理表(m_vcFiles)并写文件
	 */
	for (int ii = 0; ii < vcFiles.size(); ii++)
	{
		eState = AddToSVLSaveList(vcFiles[ii]->GetSVLSaveAsPath());
		if (eState == STK_SUCCESS_EXIST)
		{
			//向已存在SVL文件追加写入File
			eState = vcFiles[ii]->WriteCADFile(i_eFileSaveType, true/*AppendFlag*/);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}
		else
		{
			//新建SVL文件
			eState = vcFiles[ii]->WriteCADFile(i_eFileSaveType, false);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
		}

	}

	ClearSVLSaveList();

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		为指定的SVL设置新的路径
 *	@param[in]	i_wstrSaveDir		将要保存的路径
 *	@param[in]	i_eSaveType			保存类型
 *	@param[in]	i_vcFiles			指定的SVL文件
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SetGivenFilePath(wstring i_wstrSaveDir, StkFileSaveTypeEnum i_eSaveType, vector<Stk_File*> i_vcFiles)
{
	STK_UINT32			nuPos = 0;
	STK_UINT32			nuTopPrototypeNum = 0;
	Stk_ProtoType*		stkTopPrototypeP = NULL;
	wstring				wstrNewSaveDir = L"";
	wstring				wstrCADFileName = L"";
	wstring				wstrSVLFileName = L"";
	StkProtoTypeEnum	eProtoType = PROTO_TYPE_UNKNOWN;
	STK_BOOL			bIsTopFile = false;

	nuPos = i_wstrSaveDir.find_last_of(L"\\");
	wstrNewSaveDir = i_wstrSaveDir.substr(0, nuPos + 1);

	// 单一时强制修改为“指定路径i_wstrSaveDir”；
	if ( i_eSaveType == FILESAVE_TYPE_UNIQUE)
	{
		for (int ii = 0; ii < i_vcFiles.size(); ii++)
		{
			i_vcFiles[ii]->SetSVLSaveAsPath( i_wstrSaveDir );
		}
	}
	// 分散时强制修改为“指定文件夹+CADFileName.svl”；
	else if (  i_eSaveType == FILESAVE_TYPE_DISTRIBUTION )
	{
		for (int ii = 0; ii < i_vcFiles.size(); ii++)
		{
			wstrCADFileName = i_vcFiles[ii]->GetCADFileName();
			wstrCADFileName += L".svl";
			bIsTopFile = i_vcFiles[ii]->IsTopFile();
			if (bIsTopFile)
			{
				i_vcFiles[ii]->SetSVLSaveAsPath(i_wstrSaveDir);
				bIsTopFile = false;
			}
			else
			{
				i_vcFiles[ii]->SetSVLSaveAsPath( wstrNewSaveDir + wstrCADFileName);
			}
		}
	}
	// 混合时仅变更文件夹，文件名不修改
	else if (  i_eSaveType == FILESAVE_TYPE_BEFOR )
	{
		for (int ii = 0; ii < i_vcFiles.size(); ii++)
		{
			wstrSVLFileName = i_vcFiles[ii]->GetSVLPath();
			nuPos = wstrSVLFileName.find_last_of(L"\\");
			wstrSVLFileName = wstrSVLFileName.substr( nuPos,wstrSVLFileName.length() - 1 );
			if (bIsTopFile)
			{
				i_vcFiles[ii]->SetSVLSaveAsPath(i_wstrSaveDir);
				bIsTopFile = false;
			}
			else
			{
				i_vcFiles[ii]->SetSVLSaveAsPath( wstrNewSaveDir + wstrSVLFileName);
			}
		}
	}

	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentManager::DeleteGivenSVLFile(vector<Stk_File*> i_vcFiles)
{
	string	strSVLPath = "";
	for (int ii = 0; ii < i_vcFiles.size(); ii++)
	{
		strSVLPath = Stk_Platform::WStringToString(i_vcFiles[ii]->GetSVLSaveAsPath());
		remove(strSVLPath.c_str()); //@@@ Android 可以？
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取文档的保存类型：保存/另存为
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
StkDocumentSaveTypeEnum Stk_DocumentManager::GetDocumentSaveType()
{
	return m_eDocumentSaveType;
}

/*************************************************************************//**
 *	@brief		设置所有文件的压缩类型
 *	@param[in]	i_eCompressType			压缩类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
  *	@retval		STK_ERROR				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SetAllFilesCompressType(StkCompressTypeEnum i_eCompressType)
{
	for(int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		m_vcFiles[ii]->SetCompressType(i_eCompressType);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		通过子File路径获取父级的File对象
 *	@param[in]	i_wstrChildPath		子File路径
 *	@param[out]	无
 *	@retval		父级的File对象
 *	@note		无
 *	@attention	用于从子路径查找父级的File，仅内部使用，不对外
 ****************************************************************************/
Stk_File* Stk_DocumentManager::GetParentFileByChildFilePath(wstring i_wstrChildPath)
{
	map<wstring, Stk_File*>::iterator itor = m_mapParentFileList.find(i_wstrChildPath);
	if (itor == m_mapParentFileList.end())
	{
		return NULL;
	}
	return itor->second;
}

/************************************************************************//**
 *	@brief		设置文件密码
 *	@param[in]	i_wstrPassword			文件密码
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		其他					设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SetPassword(wstring i_wstrPassword)
{
	STK_STATUS	eState = STK_SUCCESS;
	wstring		wstrPassword = L"";		// 经过加密后的密码
	eState = Password(i_wstrPassword, wstrPassword);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	for(int ii = 0; ii < m_vcFiles.size(); ii++)
	{
		m_vcFiles[ii]->SetPassword(wstrPassword);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		加密
 *	@param[in]	i_wstrPassword	用户输入的加密密码
 *	@param[in]	o_wstrPassword	经过加密后的密码流
 *	@param[out]	无
 *	@retval		STK_SUCCESS		加密成功
 *	@retval		其他			加密失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::Password(wstring i_wstrPassword, wstring& o_wstrPassword)
{
	STK_STATUS	eState = STK_SUCCESS;
	eState = m_stkAesHelperP->aesEncrypt(i_wstrPassword, o_wstrPassword);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		解密
 *	@param[in]	i_wstrPassword	用户输入的解密密码
 *	@param[in]	o_wstrPassword	经过解密后的密码流
 *	@param[out]	无
 *	@retval		STK_SUCCESS		解密成功
 *	@retval		其他			解密失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::DePassword(wstring i_wstrPassword, wstring& o_wstrPassword)
{
	STK_STATUS	eState = STK_SUCCESS;
	eState = m_stkAesHelperP->aesUncrypt(i_wstrPassword, o_wstrPassword);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置用户输入的解密密码
 *	@param[in]	i_wstrUserDePassword	用户输入的解密密码
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@retval		其他					设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SetUserDePassword(wstring i_wstrSVLPath, wstring i_wstrUserDePassword)
{
	map<wstring, wstring>::iterator itor = m_mapUserDePassword.begin();
	while (itor != m_mapUserDePassword.end())
	{
		if (itor->first == i_wstrSVLPath)
		{
			itor->second = i_wstrUserDePassword;
			return STK_SUCCESS;
		}
		itor++;
	}
	m_mapUserDePassword.insert(pair<wstring, wstring>(i_wstrSVLPath, i_wstrUserDePassword));
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		从解密列表中查询指定文件的用户解密密码
 *	@param[in]	i_wstrSVLPath			指定的SVL File文件路径
 *	@param[out]	无
 *	@retval		指定的SVL File文件密码
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_DocumentManager::SearchUserDePasswordBySVLPath(wstring i_wstrSVLPath)
{
	wstring	wstrDePassword = L"";
	map<wstring, wstring>::iterator itor = m_mapUserDePassword.begin();
	while (itor != m_mapUserDePassword.end())
	{
		if (itor->first == i_wstrSVLPath)
		{
			return itor->second;
		}
		itor++;
	}
	return wstrDePassword;
}

/*************************************************************************//**
 *	@brief		加载单个CAD File文件
 *	@param[in]	i_wstrSVLPath			SVL文件路径
 *	@param[out]	o_vcTopProtos			当前SVL文件的TopProtoTypes
 *	@retval		STK_SUCCESS				加载成功
  *	@retval		STK_ERROR				加载失败
 *	@note		无
 *	@attention	仅用于属性面板时调用
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::LoadSingleCADFile(wstring& i_wstrSVLPath, vector<Stk_ProtoType*>& o_vcTopProtos)
{
	Stk_File			*stkCADFileP = NULL;
	Stk_File			*stkTempFileP = NULL;
	Stk_ProtoType		*stkProtoP = NULL;
	vector<wstring>		vecPath;
	wstring				wstrCurFile;
	STK_SIZE_T			i;
	STK_STATUS			ret = STK_SUCCESS;
	FILE				*fileP = NULL;
	bool				bfirstFile; //SVL文件中的第一个CADFile
	STK_ID				idFile = 0;
	map<Stk_File*,bool>	mapCADFileOfTopSVL;
	wstring				wstrSVLVersion = L"";
	STK_BOOL			bIsOldVersion = FALSE;

	// 注意！在打开SVL的时刻不用清空SVLFileList
	// 原因是在插入的时刻为了判定SVL是否加载还需要用到SVLFileList
	//ret = ClearSVLSaveList();

	STK_UINT32 nuProtoTypeCount = 1;	// 用于计录SVL中ProtoType数量，初始默认为1
	vector<wstring> vecChild;
	vector<Stk_File*> vcFiles;

	// 检查SVLFileList来判定该SVL文件是否已加载
	// 注意！在打开时刻，SVLFileList作为已打开文件的临时工作区
	wstrCurFile = i_wstrSVLPath;
	// 判断版本号
	// 预先读取SVL文件的版本号
	stkTempFileP = new Stk_File(this);
	ret = stkTempFileP->PreOpenFile(wstrCurFile);
	if (ret == STK_SUCCESS)
	{
		wstrSVLVersion = stkTempFileP->GetSVLVersion();
		bIsOldVersion = Stk_Utility::IsOldVersion(wstrSVLVersion);
	}
	delete stkTempFileP;
	if (ret != STK_SUCCESS)
	{
		return ret;
	}

	/*
	 *	读取该SVL文件中的所有的CADFile（一个SVL中可以有多个CADFile）
	 */
	bfirstFile = true;
	// ###
	while( fileP != NULL || bfirstFile)
	{
		stkCADFileP = new Stk_File(this);
		if (bIsOldVersion)
		{
			// 加载V2.2版之前的旧SVL文件
			stkCADFileP->SetOldVersion(true);
			vcFiles.push_back(stkCADFileP);
			ret = stkCADFileP->OpenFile(wstrCurFile, fileP, nuProtoTypeCount, vecChild);
			if(ret != STK_SUCCESS)
			{
				return ret;
			}
			// 兼容旧文件特殊处理
			if (m_eSaveType == FILESAVE_TYPE_UNIQUE && vcFiles.size() > 1)
			{
				ConvertTitleToCADFileTitle(stkCADFileP, vcFiles[0]);
				//ConvertPreViewToCADFilePreView(stkCADFileP, vcFiles[0]);
				ConvertOptionToCADFileOption(stkCADFileP, vcFiles[0]);
			}
		}
		else
		{
			//加载V2.2版以后的新SVL文件
			ret = stkCADFileP->OpenCADFile(wstrCurFile, fileP ,vecChild);
			if(ret != STK_SUCCESS)
			{
				return ret;
			}
		}

		// 注册CADFile
		m_GlobalIDManger->RegCADFilePath(stkCADFileP->GetCADFilePath(), idFile); //CADFilePath
		stkCADFileP->RegisterID(STK_NEW_ID);
		m_vcFiles.push_back(stkCADFileP);

		// 准备用于计算顶级ProtoType的数据
		mapCADFileOfTopSVL.insert(pair<Stk_File*,bool>(stkCADFileP,true));

		AddToSVLLoadList(wstrCurFile,stkCADFileP);
		bfirstFile = false;
	}

	/*
	 *	计算出DocManager的顶级ProtoType
	 *	注意！V2.2后顶级ProtoType可能有多个,仅从顶级SVL文件中判定
	 */
	ret = ComputeTopProtos(mapCADFileOfTopSVL, o_vcTopProtos);
	if(ret!= STK_SUCCESS)
	{
		return ret;
	}

	/*
	 *	设置Docmanger顶级ProtoType计数器
	 */
	for (int ii = 0; ii < o_vcTopProtos.size(); ii++)
	{
		o_vcTopProtos[ii]->AddRef();
	}

	/*
	 *	设置ProtoType计数器
	 */
	map<STK_ID, STK_UINT32>::iterator itor = m_mapLoadProtoRef.begin();
	while(itor != m_mapLoadProtoRef.end())
	{
		stkProtoP = GetProtoTypeByID(itor->first);
		if (stkProtoP != NULL)
		{
			stkProtoP->SetRef(itor->second);
		}
		itor++;
	}
	ClearProtoRef();

	return ret;
}

/************************************************************************//**
 *	@brief		打开SVL装配文件
 *	@param[in]	i_wstrSVLPath					SVL文件路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS						成功
 *	@retval		STK_ERROR_VERSION_UNKNOWNFILE	不可识别版本
 *	@retval		STK_ERROR_VERSION_HIGHER		读入更高版本
 *	@retval		其它							错误
 *	@note		由于V2.2后SVL文件可能存在多个顶级ProtoType，因此需要用户指定哪个作为默认顶级ProtoType。<br>
 *				打开后务必通过Stk_DocumentManager::SetDefaultTopProto()方法设置顶级ProtoType
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::LoadDocumentOfSingleCADFile(wstring& i_wstrSVLPath)
{
	STK_STATUS ret = STK_SUCCESS;

	/* 
	 *	加载单个SVL装配文件
	 */
	ret = LoadSingleCADFile(i_wstrSVLPath, m_vcTopProtoTypes);
	m_wstrLoadPath = i_wstrSVLPath;

	return ret;
}

/*************************************************************************//**
 *	@brief		遍历指定装配结构下的所有文件
 *	@param[in]	i_stkFileP				指定装配结构对应的File文件
 *	@param[out]	o_mapFiles				文件列表
 *	@retval		STK_SUCCESS				遍历成功
  *	@retval		STK_ERROR				遍历失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SearchFilesOfAssembly(Stk_File* i_stkFileP, map<Stk_File*, STK_BOOL>& o_mapFiles)
{
	STK_STATUS			eState = STK_SUCCESS;
	STK_UINT32			nuTopProtoTypeNum = 0;
	STK_UINT32			nuChildInsNum = NULL;
	Stk_ProtoType*		stkTopProtoTypeP = NULL;
	Stk_Instance*		stkChildInstanceP = NULL;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	nuTopProtoTypeNum = i_stkFileP->GetTopProtoTypeNum();
	for (int ii = 0; ii < nuTopProtoTypeNum; ii++)
	{
		stkTopProtoTypeP = i_stkFileP->GetTopProtoTypeByIndex(ii);
		nuChildInsNum = stkTopProtoTypeP->GetChildInsNum();
		for (int jj = 0; jj < nuChildInsNum; jj++)
		{
			Stk_File* stkFileP = NULL;
			stkChildInstanceP = stkTopProtoTypeP->GetChildIns(jj);
			if (stkChildInstanceP != NULL)
			{
				stkFileP = (stkChildInstanceP->GetProtoType())->GetFile();
				if (i_stkFileP == stkFileP)
				{
					continue;
				}
				o_mapFiles.insert(pair<Stk_File*, STK_BOOL>(stkFileP, true));
				eState = SearchFilesOfAssembly(stkFileP, o_mapFiles);
				if (eState != STK_SUCCESS)
				{
					return STK_ERROR;
				}
			}
		}
	}
	return eState;
}

STK_STATUS Stk_DocumentManager::SearchChildInsProtoType(Stk_ProtoType* i_stkProtoTypeP, map<STK_ID,STK_BOOL>& o_mapInsProto)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_SIZE_T		nuChildInsNum = 0;
	Stk_Instance*	stkInstanceP = NULL;
	Stk_ProtoType*	stkProtoTypeP = NULL;
	STK_ID			idProtoType = 0;
	
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
			continue;	// continue的原因是有可能装配结构中只有实例，而没有Proto的情形
		}
		idProtoType = stkInstanceP->GetProtoID();
		o_mapInsProto.insert(pair<STK_ID,STK_BOOL>(idProtoType, true));

		eState = SearchChildInsProtoType(stkProtoTypeP, o_mapInsProto);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}
	}
	return eState;
}

/*************************************************************************//**
 *	@brief		遍历指定装配结构下的所有实例
 *	@param[in]	i_stkProtoP				指定装配结构对应的ProtoType
 *	@param[in]	i_wstrBomID				工艺BOM指定的序号
 *	@param[out]	o_vcInstances			指定BOM序号对应的实例数组
 *	@retval		STK_SUCCESS				遍历成功
  *	@retval		STK_ERROR				遍历失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SearchInstancesByIDFromVirtualBomTable(Stk_ProtoType* i_stkProtoP, wstring i_wstrBomID, vector<Stk_Instance*>& o_vcInstances)
{
	STK_STATUS		eState = STK_SUCCESS;
	STK_SIZE_T		nuChildInsNum = 0;
	Stk_Instance*	stkInstanceP = NULL;
	Stk_ProtoType*	stkProtoTypeP = NULL;
	wstring			wstrBomID = L"";

	nuChildInsNum = i_stkProtoP->GetChildInsNum();
	for (int ii = 0; ii < nuChildInsNum; ii++)
	{
		stkInstanceP = i_stkProtoP->GetChildIns(ii);
		if (stkInstanceP == NULL)
		{
			return STK_ERROR;
		}
		wstrBomID = stkInstanceP->GetVirtualBOMID();
		if (wstrBomID == i_wstrBomID)
		{
			o_vcInstances.push_back(stkInstanceP);
		}
		stkProtoTypeP = stkInstanceP->GetProtoType();
		if (stkProtoTypeP == NULL)
		{
			return STK_SUCCESS;	// 返回STK_SUCCESS，而不返回STK_ERROR的原因是有可能装配结构中只有实例，而没有Proto的情形
		}
		eState = SearchInstancesByIDFromVirtualBomTable(stkProtoTypeP, i_wstrBomID, o_vcInstances);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}
	}
	return eState;
}

/************************************************************************//**
 *	@brief		添加工艺BOM表格信息
 *	@param[in]	i_wstrID				BOM表格的序号
 *	@param[in]	i_stkVirtualBomInfoP	工艺BOM表格信息
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@retval		其他					添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::AddVirtualBomTableInfo(wstring i_wstrID, Stk_VirtualBomItemInfo* i_stkVirtualBomInfoP)
{
	STK_STATUS	eState = STK_SUCCESS;
	pair<map<wstring, Stk_VirtualBomItemInfo*>::iterator, bool> ret;
	STK_UINT32	nuNumber = 0;
	Stk_File*	stkFileP = NULL;

	if (i_stkVirtualBomInfoP == NULL)
	{
		return STK_ERROR;
	}

	// 存放于顶级File中
	if (m_vcTopProtoTypes[0] != NULL)
	{
		stkFileP = m_vcTopProtoTypes[0]->GetFile();
		if (stkFileP == NULL)
		{
			return STK_ERROR;
		}
		eState = stkFileP->AddVirtualBomTableInfo(i_wstrID, i_stkVirtualBomInfoP);
		if (eState == STK_ERROR)
		{
			return eState;
		}
	}
	
	ret = m_mapBomTable.insert(pair<wstring, Stk_VirtualBomItemInfo*>(i_wstrID, i_stkVirtualBomInfoP));
	if (ret.second)
	{
		nuNumber = 1;
		i_stkVirtualBomInfoP->SetNumber(nuNumber);
		return STK_SUCCESS;
	}
	else
	{
		nuNumber = i_stkVirtualBomInfoP->GetNumber();
		nuNumber++;
		i_stkVirtualBomInfoP->SetNumber(nuNumber);
		return STK_SUCCESS_EXIST;
	}
}

/************************************************************************//**
 *	@brief		获取工艺BOM表格信息
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		工艺BOM表格信息
 *	@note		无
 *	@attention	无
 ****************************************************************************/
map<wstring, Stk_VirtualBomItemInfo*>& Stk_DocumentManager::GetVirtualBomTableInfo()
{
	return m_mapBomTable;
}

/************************************************************************//**
 *	@brief		获取指定的工艺BOM序号对应的实例信息
 *	@param[in]	i_wstrBomID				指定的BOM序号
 *	@param[out]	o_vcInstances			对应的实例
 *	@retval		工艺BOM表格信息
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::GetInstanceByIDFromVirtualBomTable(wstring i_wstrBomID, vector<Stk_Instance*>& o_vcInstances)
{
	STK_STATUS				eState = STK_SUCCESS;
	vector<Stk_Instance*>	vecInstances;
	eState = SearchInstancesByIDFromVirtualBomTable(m_vcTopProtoTypes[0], i_wstrBomID, vecInstances);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}
	o_vcInstances = vecInstances;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		添加ProtoType名称
 *	@param[in]	i_wstrProtoTypeName		ProtoType名称
 *	@param[out]	o_nuProtoNameNum		ProtoType名称重复数量
 *	@retval		STK_SUCCESS				添加成功
 *	@retval		其他					添加失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::AddProtoTypeName(wstring i_wstrProtoTypeName, STK_UINT32& o_nuProtoNameNum)
{
	STK_UINT32	nuProtoTypeNameNum = 0;
	map<wstring, STK_UINT32>::iterator itor;
	itor = m_mapProtoTypeName.find(i_wstrProtoTypeName);
	if (itor != m_mapProtoTypeName.end())
	{
		nuProtoTypeNameNum = itor->second;
		nuProtoTypeNameNum++;
		m_mapProtoTypeName.erase(itor);
	}
	else
	{
		nuProtoTypeNameNum = 1;
	}

	m_mapProtoTypeName.insert(pair<wstring, STK_UINT32>(i_wstrProtoTypeName, nuProtoTypeNameNum));
	o_nuProtoNameNum = nuProtoTypeNameNum;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		查询ProtoType名称是否在列表中
 *	@param[in]	i_wstrProtoTypeName		ProtoType名称
 *	@param[out]	无
 *	@retval		TRUE					存在于列表中
 *	@retval		FALSE					不存在于列表中
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_BOOL Stk_DocumentManager::FindProtoTypeName(wstring i_wstrProtoTypeName)
{
	map<wstring, STK_UINT32>::iterator itor;
	itor = m_mapProtoTypeName.find(i_wstrProtoTypeName);
	if (itor != m_mapProtoTypeName.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/************************************************************************//**
 *	@brief		清空记录的所有文件列表中的ProtoType名字
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				清空成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::ClearAllProtoTypeName()
{
	m_mapProtoTypeName.clear();
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief	    获取Stk_Listener对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		监听对象	
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_Listener* Stk_DocumentManager::GetListener()
{
	return m_stkListenerP;
}

/************************************************************************//**
 *	@brief	    设置外部监听回调函数
 *	@param[in]	i_stkListenerP  函数指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其它			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_DocumentManager::SetListener(Stk_Listener* i_stkListenerP)
{
	if (i_stkListenerP == NULL)
	{
		return STK_ERROR;
	}
	m_stkListenerP = i_stkListenerP;
	m_ReaderListenerP = new Stk_ReaderListener(this);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief	    获取Stk_Listener对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		监听对象	
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_ReaderListener* Stk_DocumentManager::GetReaderListener()
{
	return m_ReaderListenerP;
}

/************************************************************************//**
 *	@brief		设置是否取消加载标识
 *	@param[in]	i_bLoadingCancel	取消加载标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
//STK_STATUS Stk_DocumentManager::SetLoadingCancel(STK_BOOL i_bLoadingCancel)
//{
//	m_bLoadingCancel = i_bLoadingCancel;
//	return STK_SUCCESS;
//}

/************************************************************************//**
 *	@brief		获取是否取消加载的标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		是否取消加载标识
 *	@note		无
 *	@attention	内部接口，不提供给用户使用
 ****************************************************************************/
//STK_BOOL Stk_DocumentManager::GetLoadingCancel()
//{
//	return m_bLoadingCancel;
//}
