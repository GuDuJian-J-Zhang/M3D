// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_DocumentManager.h
 *
 *	@brief	多文件管理策略类
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

#ifndef _STK_DOCUMENTMANAGER_H_
#define _STK_DOCUMENTMANAGER_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Manager.h"
#include "DataManager.h"
#include "Stk_Define.h"
#include "Stk_File.h"
#include "Stk_Instance.h"
#include "Stk_ProtoType.h"

#ifdef _WINDOWS_
class CLogFile;
class Stk_LogFileEx;

typedef struct SVLLog 
{
	STK_BOOL bLogOutput;
	STK_BOOL bPrototype;
	STK_BOOL bBody;
	STK_BOOL bPMIAndView;
	STK_BOOL bMesh;
	STK_BOOL bMetaData;
}Stk_SVLLog;
#endif

class Stk_GlobalIDManager;
class Stk_AesHelper;
class Stk_VirtualBomItemInfo;
class Stk_Listener;
class Stk_ReaderListener;

/**************************************************************************************
	 *	@class	Stk_DocumentManager
	 *	@brief	多文件管理策略类
	 *
	 *	多文件的管理，加载/保存。<br>
***************************************************************************************/
class DATAMANAGER_API Stk_DocumentManager
{
private:
	Stk_ProtoType*			m_DefaultTopProtoP;		//!< 默认顶级ProtoType
	vector<Stk_ProtoType*>	m_vcTopProtoTypes;		//!< 顶级Prototype数组
	vector<Stk_File*>		m_vcFiles;				//!< 文件对象数组
	StkFileSaveTypeEnum		m_eSaveType;			//!< 保存类型
	StkFileSaveTypeEnum		m_eBeforType;			//!< 顶级的保存类型
	STK_UINT32				m_nuFilesNum;			//!< 打开的SVL文件的个数
	STK_BOOL				m_bPmiRead;				//!< PMI读取标志
	Stk_GlobalIDManager*	m_GlobalIDManger;		//!< 全局ID管理器（零件、实例、文件等的管理）
	wstring					m_wstrLoadPath;			//!< SVL文件打开路径
	wstring					m_wstrSavePath;			//!< SVL文件保存路径
	
	map<wstring,STK_BOOL>	m_mapSVLSaveList;		//!< 临时变量，用于表示保存SVL文件路径
	map<wstring,Stk_File*>	m_mapSVLLoadList;		//!< 临时变量，用于表示打开/插入SVL文件的路径
	map<STK_ID,STK_BOOL>	m_mapTopProtoOfTopFile;	//!< 临时变量，仅供ComputeTopProtos()使用
	map<STK_ID,STK_UINT32>	m_mapLoadProtoRef;		//!< 临时变量，ProtoType计数器
	StkDocumentSaveTypeEnum	m_eDocumentSaveType;	//!< 文档保存类型：保存/另存为
	map<wstring, Stk_File*>	m_mapParentFileList;	//!< 子File路径与父File对象的映射，用于从子路径查找父级的File，仅内部使用，不对外
	map<wstring, wstring>	m_mapUserDePassword;	//!< 用户输入的指定文件的解密密码
	vector<wstring>			m_vecPaths;				//!< SVL文件路径
#ifdef _WINDOWS_
	Stk_SVLLog				m_SVLLog;				//!< 日志设置参数
	Stk_LogFileEx*			m_svlgLogFile;			//!< 日志文件
#endif
	Stk_AesHelper*			m_stkAesHelperP;		//!< 文件加密对象指针
	map<wstring, Stk_VirtualBomItemInfo*>	m_mapBomTable;	//!< BOM表格，内存的释放不在Stk_DocumentManager中，放在File中释放
	map<wstring, STK_UINT32>	m_mapProtoTypeName;		//!< 所有文件列表中的ProtoType名字
	Stk_Listener*				m_stkListenerP;
	Stk_ReaderListener*			m_ReaderListenerP;		//!< 监听对象
private:
	STK_STATUS				SetAllSVLFilePath(wstring i_wstrSaveDir, StkFileSaveTypeEnum i_eSaveType);

	STK_STATUS				AddToSVLSaveList(wstring i_wstrSVLFilePath);
	STK_STATUS				ClearSVLSaveList();

	STK_STATUS				AddToSVLLoadList(wstring i_wstrSVLFilePath, Stk_File* i_stkTopFile);
	Stk_File*				InqTopFileFromSVLLoadList(wstring i_wstrSVLFilePath);
	STK_STATUS				ClearSVLLoadList();

	STK_STATUS				DeleteSVLFile();
	STK_STATUS				ComputeTopProtos(map<Stk_File*,bool> i_mapFiles, vector<Stk_ProtoType*> &o_vcTopProtos);
	STK_STATUS				ClearProtoRef();
	STK_STATUS				ConvertTitleToCADFileTitle(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP);
	STK_STATUS				ConvertPreViewToCADFilePreView(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP);
	STK_STATUS				ConvertOptionToCADFileOption(Stk_File* i_stkCADFileP, Stk_File* i_stkFirstFileP);
	StkFileSaveTypeEnum		ComputeFileSaveType(Stk_ProtoType* i_stkProtoTypeP);
	STK_STATUS				DeleteInsAttrInfo(wstring i_wstrInsPlcPath);
	STK_STATUS				SetGivenFilePath(wstring i_wstrSaveDir, StkFileSaveTypeEnum i_eSaveType, vector<Stk_File*> i_vcFiles);
	STK_STATUS				DeleteGivenSVLFile(vector<Stk_File*> i_vcFiles);
	STK_STATUS				SearchChildInsProtoType(Stk_ProtoType* i_stkProtoTypeP, map<STK_ID,STK_BOOL>& o_mapInsProto);
private:
	/****************************************************************************
	 *	@brief		加密
	 *	@param[in]	i_wstrPassword	用户输入的加密密码
	 *	@param[in]	o_wstrPassword	经过加密后的密码流
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		加密成功
	 *	@retval		其他			加密失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				Password(wstring i_wstrPassword, wstring& o_wstrPassword);

	/****************************************************************************
	 *	@brief		从解密列表中查询指定文件的用户解密密码
	 *	@param[in]	i_wstrUserDePassword	用户输入的解密密码
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		其他					设置失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring					SearchUserDePasswordBySVLPath(wstring i_wstrSVLPath);

	/*****************************************************************************
	 *	@brief		遍历指定装配结构下的所有文件
	 *	@param[in]	i_stkFileP				指定装配结构对应的File文件
	 *	@param[out]	o_mapFiles				文件列表
	 *	@retval		STK_SUCCESS				遍历成功
	  *	@retval		STK_ERROR				遍历失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SearchFilesOfAssembly(Stk_File* i_stkFileP, map<Stk_File*, STK_BOOL>& o_mapFiles);

	/*****************************************************************************
	 *	@brief		遍历指定装配结构下的所有实例
	 *	@param[in]	i_stkProtoP				指定装配结构对应的ProtoType
	 *	@param[in]	i_wstrBomID				工艺BOM指定的序号
	 *	@param[out]	o_vcInstances			指定BOM序号对应的实例数组
	 *	@retval		STK_SUCCESS				遍历成功
	  *	@retval		STK_ERROR				遍历失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SearchInstancesByIDFromVirtualBomTable(Stk_ProtoType* i_stkProtoP, wstring i_wstrBomID, vector<Stk_Instance*>& o_vcInstances);
public:
	static Stk_DocumentManager* NewDocumentManager();

	static void DeleteDocumentManager(Stk_DocumentManager* documentMgr);

public:
	STK_STATUS				LoadCADSubFile(wstring& i_wstrSVLPath, vector<Stk_ProtoType*>& o_vcTopProtos );

	/****************************************************************************
	 *	@brief		解密
	 *	@param[in]	i_wstrPassword	用户输入的解密密码
	 *	@param[in]	o_wstrPassword	经过解密后的密码流
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		解密成功
	 *	@retval		其他			解密失败
	 *	@note		无
	 *	@attention	内部接口，用户不能使用此接口
	 ****************************************************************************/
	STK_STATUS				DePassword(wstring i_wstrPassword, wstring& o_wstrPassword);

public:
	/****************************************************************************
	 *	@brief		Stk_DocumentManager构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_DocumentManager();

	/****************************************************************************
	 *	@brief		Stk_DocumentManager析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_DocumentManager();

	/****************************************************************************
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
	STK_STATUS				LoadDocument(wstring& i_wstrSVLPath);


	/****************************************************************************
	 *	@brief		保存CAD装配
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			保存成功
	 *	@retval		其它				保存失败
	 *	@note		保存命令使用此接口
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SaveDocument();

	/****************************************************************************
	 *	@brief		另存SVL装配文件
	 *	@param[in]	i_wstrSavePath		保存的路径
	 *	@param[in]	i_eCADFileSaveType	保存类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			保存成功
	  *	@retval		其它				保存失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SaveDocumentAs(wstring i_wstrSavePath, StkFileSaveTypeEnum i_eCADFileSaveType);

	/****************************************************************************
	 *	@brief		获取GlobalIDManger
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_GlobalIDManger	GlobalIDManger
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/

	Stk_GlobalIDManager*	GetGlobalIDManger();

	/*****************************************************************************
	 *	@brief		往顶级ProtoType列中添加顶级ProtoType
	 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddTopProtoType(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		获取当前File文件的顶级ProtoType数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		顶级ProtoType数量
	 *	@note		由于零件族等特殊情况，可能存在多个顶级ProtoType <br>
	 *				需要用户通过SetDefaultTopProto设置，并通过GetDefaultTopProto来获取默认顶级
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetTopProtoTypeNum();

	/*****************************************************************************
	 *	@brief		获取指定索引对应的顶级ProtoType
	 *	@param[in]	i_nuIndex				指定索引
	 *	@param[out]	无
	 *	@retval		指定的顶级ProtoType
	 *	@note		由于零件族等特殊情况，可能存在多个顶级ProtoType <br>
	 *				需要用户通过SetDefaultTopProto设置，并通过GetDefaultTopProto来获取默认顶级
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*			GetTopProtoTypeByIndex(STK_UINT32 i_nuIndex);

	/*****************************************************************************
	 *	@brief		通过ID号获取ProtoType
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[out]	无
	 *	@retval		ProtoPtr				ProtoType对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*			GetProtoTypeByID(STK_ID i_idProtoType);

	/*****************************************************************************
	 *	@brief		设置默认的顶级ProtoType，作为各种操作的根节点
	 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SCUCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		必须使用GetTopProtoTypeByIndex()中取得的ProtoType作为输入参数来设置
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDefaultTopProto(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		获取默认的顶级ProtoType
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_DefaultTopProtoP			默认顶级ProtoType
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*			GetDefaultTopProtoType();

	/*****************************************************************************
	 *	@brief		ProtoType计数
	 *	@param[in]	i_idProto				ProtoType ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				计数成功
	 *	@note		ID采用预分配方式，计数器根据预分配的ID进行
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddProtoRef(STK_ID i_idProto);

	/****************************************************************************
	 *	@brief		通过路径获取SVL中的顶级File对象
	 *	@param[in]	i_wstrPath			文件路径
	 *	@param[out]	无
	 *	@retval		stkFileP			File对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//Stk_File*				GetFileByPath(const wstring& i_wstrPath);

	/****************************************************************************
	 *	@brief		通过索引获取File对象
	 *	@param[in]	i_nuKeyPos			索引值
	 *	@param[out]	无
	 *	@retval		stkFileP			File文件指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_File*				GetFileByIndex(STK_UINT32 i_nuKeyPos);

	/****************************************************************************
	 *	@brief		获取File文件的个数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		nuFileNum			File文件个数
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetFileNum();

	/****************************************************************************
	 *	@brief		获取加载文件的路径
	 *	@param[in]	无
	 *	@retval		加载文件的路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetFileLoadPath();

	/****************************************************************************
	 *	@brief		获取文件保存的路径
	 *	@param[in]	无
	 *	@retval		文件保存的路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetFileSavePath();

	/****************************************************************************
	 *	@brief		获取文件保存类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eSaveType			文件保存类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkFileSaveTypeEnum		GetFileSaveType();

	/****************************************************************************
	 *	@brief		设置文件保存类型
	 *	@param[in]	i_eSaveType			文件保存类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFileSaveType(StkFileSaveTypeEnum i_eSaveType);

	/*****************************************************************************
	 *	@brief		往File数组中添加File对象指针
	 *	@param[in]	i_stkFileP				文件对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID					AddFile(Stk_File *i_stkFileP);

	/*****************************************************************************
	 *	@brief		移除File数组中指定的File，仅删除vector<Stk_File*>数组中关系
	 *	@param[in]	i_stkFileP				指定的File指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RemoveFile(Stk_File* i_stkFileP);

	/*****************************************************************************
	 *	@brief		从LoadList中删除指定SVL文件路径
	 *	@param[in]	i_wstrSVLFilePath		要删除的SVL文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		其他					删除失败
	 *	@note		用于打开/插入SVL文件时用
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				DelSVLFileFromLoadList(wstring i_wstrSVLFilePath);

	/*****************************************************************************
	 *	@brief		通过ID号获取实例Instance
	 *	@param[in]	i_idInstance			Instance ID
	 *	@param[out]	无
	 *	@retval		InstanceP				Instance对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Instance*			GetInstanceByID(STK_ID i_idInstance);

	/****************************************************************************
	 *	@brief		通过实例配置路径获取实例
	 *	@param[in]	i_wstrPlcPath			实例配置路径
	 *	@retval		returnIns				根据路径获取的实例指针
	 *	@note		找不到对应Ins则返回NULL
	 *	@attention	无
	 ****************************************************************************/
	Stk_Instance*			GetInstanceByPath(const wstring& i_wstrPlcPath);

	/*****************************************************************************
	 *	@brief		通过配置路径删除实例及子实例
	 *	@param[in]	i_wstrPlcPath			实例配置路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		其他					删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				DelInstanceByPlcPath(const wstring& i_wstrPlcPath);

	/*****************************************************************************
	 *	@brief		通过ID号获取实例Instance
	 *	@param[in]	i_wstrPlcPath			实例配置路径
	 *	@param[in]	i_eEntityType			实体类型
	 *	@param[in]	i_idEntity				实体ID
	 *	@param[out]	无
	 *	@retval		stkEntityP				实体对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Entity*				GetEntiyByPath(const wstring& i_wstrPlcPath,StkEntityTypeEnum i_eEntityType,STK_ID i_idEntity);

	/****************************************************************************
	 *	@brief		获取PMI读取标志
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bPmiRead			PMI读取标志
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				GetPMIViewReadFlag();

	/****************************************************************************
	 *	@brief		设置PMI读取标志
	 *	@param[in]	i_bflag			PMI读取标志
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID					SetPMIViewReadFlag(STK_BOOL i_bflag);

	/*****************************************************************************
	 *	@brief		通过实例配置路径拖拽实例
	 *	@param[in]	i_wstrNowPlcPath		实例的当前配置路径
	 *	@param[in]	i_wstrToPlcPath			被拖拽到的上级的配置路径
	 *	@param[out]	无
	 *	@retval		wstrPlcPath				配置路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					DragAssemble(const wstring& i_wstrFormPlcPath, const wstring& i_wstrAfterPlcPath);	//! 拖拽配置

	/*****************************************************************************
	 *	@brief		通过实例配置路径拷贝实例
	 *	@param[in]	i_wstrNowPlcPath		实例的当前配置路径
	 *	@param[in]	i_wstrToPlcPath			被拷贝位置的上级的配置路径
	 *	@param[out]	o_wstrGenPlcPath		拷贝结束后生成的配置路径
	 *	@retval		STK_SUCCESS				拷贝配置路径成功
	 *	@retval		其它					拷贝配置路径失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				CopyAssemble(const wstring& i_wstrFormPlcPath, const wstring& i_wstrToPlcPath, wstring& o_wstrGenPlcPath);	//! 拷贝配置
#if 0

	/*****************************************************************************
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
	STK_STATUS				AddAssemble(const wstring& i_wstrPlcPath, const wstring& i_wstrFilePath, wstring& o_wstrPlcPath);	//!< 添加新SVL文件,返回状态为STK_ERROR_LOOP时，表明环路检测时存在环路，不允许添加
#endif

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
	STK_STATUS				AddAssemble(const wstring& i_wstrPlcPath, Stk_ProtoType* i_stkProtoTypeP, wstring& o_wstrPlcPath);

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
	STK_STATUS				AddNewProto(const wstring& i_wstrPlcPath, const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType,wstring& o_wstrPlcPath);

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
	STK_STATUS				CheckProtoExist(const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType);
	
	/*****************************************************************************
	 *	@brief		环路检测
	 *	@param[in]	i_stkInsP				要装配的Instance
	 *	@param[in]	i_wstrPlcPath			父Instance的配置路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				没有环路，可以进行装配
	 *	@retval		STK_ERROR				存在环路，不能进行装配
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				CheckLoop(Stk_Instance* i_stkInsP, const wstring& i_wstrPlcPath);	//! 环路检测

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
	//STK_STATUS				AddNewProto(const wstring& i_wstrPlcPath, const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType,wstring& o_wstrPlcPath); //!< 插入空白的ProtoType，请指定名称、类型

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
	//STK_STATUS				CheckProtoExist( const wstring& i_wstrProtoName, StkProtoTypeEnum i_eType);

#ifdef _WINDOWS_
	/****************************************************************************
	 *	@brief		获取日志文件类对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_svlgLogFile			日志文件类对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LogFileEx*			GetSVLLogFile();

	/****************************************************************************
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
	void					InitSVLLog(STK_INT32 i_nLogType, STK_INT32 i_nLogLevel, wstring i_wstrLogFullPath, Stk_ProtoType* i_stkProtoP);
	
	/****************************************************************************
	 *	@brief		获取日志配置参数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_SVLLog				日志配置参数
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	SVLLog&					GetSVLLogIni();
#endif
	/****************************************************************************
	 *	@brief		获取文档的保存类型：保存/另存为
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	StkDocumentSaveTypeEnum	GetDocumentSaveType();

	/****************************************************************************
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
	STK_STATUS				SaveSingleFileAs(wstring i_wstrSVLPath, Stk_File* i_stkFileP, StkFileSaveTypeEnum i_eFileSaveType);

	/*****************************************************************************
	 *	@brief		设置所有文件的压缩类型
	 *	@param[in]	i_eCompressType			压缩类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	  *	@retval		STK_ERROR				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetAllFilesCompressType(StkCompressTypeEnum i_eCompressType);

	/****************************************************************************
	 *	@brief		通过子File路径获取父级的File对象
	 *	@param[in]	i_wstrChildPath		子File路径
	 *	@param[out]	无
	 *	@retval		父级的File对象
	 *	@note		无
	 *	@attention	用于从子路径查找父级的File，仅内部使用，不对外
	 ****************************************************************************/
	Stk_File*				GetParentFileByChildFilePath(wstring i_wstrChildPath);

	/****************************************************************************
	 *	@brief		设置文件密码
	 *	@param[in]	i_wstrPassword			文件密码
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		其他					设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPassword(wstring i_wstrPassword);

	/****************************************************************************
	 *	@brief		设置用户输入的解密密码
	 *	@param[in]	i_wstrUserDePassword	用户输入的解密密码
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		其他					设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetUserDePassword(wstring i_wstrSVLPath, wstring i_wstrUserDePassword);
	
	/*****************************************************************************
	 *	@brief		加载单个CAD File文件
	 *	@param[in]	i_wstrSVLPath			SVL文件路径
	 *	@param[out]	o_vcTopProtos			当前SVL文件的TopProtoTypes
	 *	@retval		STK_SUCCESS				加载成功
	  *	@retval		STK_ERROR				加载失败
	 *	@note		无
	 *	@attention	仅用于属性面板时调用
	 ****************************************************************************/
	STK_STATUS				LoadSingleCADFile(wstring& i_wstrSVLPath, vector<Stk_ProtoType*>& o_vcTopProtos);

	/****************************************************************************
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
	STK_STATUS				LoadDocumentOfSingleCADFile(wstring& i_wstrSVLPath);

	/****************************************************************************
	 *	@brief		添加工艺BOM表格信息
	 *	@param[in]	i_wstrID				BOM表格的序号
	 *	@param[in]	i_stkVirtualBomInfoP	工艺BOM表格信息
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@retval		其他					添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddVirtualBomTableInfo(wstring i_wstrID, Stk_VirtualBomItemInfo* i_stkVirtualBomInfoP);

	/****************************************************************************
	 *	@brief		获取工艺BOM表格信息
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		工艺BOM表格信息
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	map<wstring, Stk_VirtualBomItemInfo*>&	GetVirtualBomTableInfo();

	/****************************************************************************
	 *	@brief		获取指定的工艺BOM序号对应的实例信息
	 *	@param[in]	i_wstrBomID				指定的BOM序号
	 *	@param[out]	o_vcInstances			对应的实例
	 *	@retval		工艺BOM表格信息
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetInstanceByIDFromVirtualBomTable(wstring i_wstrBomID, vector<Stk_Instance*>& o_vcInstances);

	/****************************************************************************
	 *	@brief		添加ProtoType名称
	 *	@param[in]	i_wstrProtoTypeName		ProtoType名称
	 *	@param[out]	o_nuProtoNameNum		ProtoType名称重复数量
	 *	@retval		STK_SUCCESS				添加成功
	 *	@retval		其他					添加失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				AddProtoTypeName(wstring i_wstrProtoTypeName, STK_UINT32& o_nuProtoNameNum);

	/****************************************************************************
	 *	@brief		查询ProtoType名称是否在列表中
	 *	@param[in]	i_wstrProtoTypeName		ProtoType名称
	 *	@param[out]	无
	 *	@retval		TRUE					存在于列表中
	 *	@retval		FALSE					不存在于列表中
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_BOOL				FindProtoTypeName(wstring i_wstrProtoTypeName);

	/****************************************************************************
	 *	@brief		清空记录的所有文件列表中的ProtoType名字
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				清空成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				ClearAllProtoTypeName();

	/****************************************************************************
	 *	@brief	    获取Stk_Listener对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		监听对象	
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Listener*			GetListener();

	/****************************************************************************
	 *	@brief		设置外部监听回调函数
	 *	@param[in]	i_stkListenerP  函数指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetListener(Stk_Listener* i_stkListenerP);

	/************************************************************************//**
	 *	@brief	    获取Stk_ReaderListener对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		监听读取对象	
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_ReaderListener*		GetReaderListener();

	/****************************************************************************
	 *	@brief		设置是否取消加载标识
	 *	@param[in]	i_bLoadingCancel	取消加载标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		其它				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//STK_STATUS				SetLoadingCancel(STK_BOOL i_bLoadingCancel);

	/****************************************************************************
	 *	@brief		获取是否取消加载的标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		是否取消加载标识
	 *	@note		无
	 *	@attention	内部接口，不提供给用户使用
	 ****************************************************************************/
	//STK_BOOL				GetLoadingCancel();
};

#endif