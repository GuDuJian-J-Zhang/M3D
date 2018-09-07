// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_File.h
 *
 *	@brief	文件类声明
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
 *		2013/10/6	更新。Gaoqq@HOTEAMSOFT
 *					增加预览图
****************************************************************************/

#ifndef _Stk_File_H_
#define _Stk_File_H_

#ifndef __MOBILE__
#pragma once
#endif

#include <time.h>
#include "DataManager.h"
#include "Stk_Define.h"
#include "Stk_Object.h"
#include "Stk_Instance.h"
#include "Stk_Entity.h"
#include "Stk_Confige.h"

class Stk_ProtoType;
class Stk_CurveEntity;
class Stk_Leader;
class Stk_Instance;
class Stk_Preview;
class Stk_OutFrame;
class Stk_ComText;
class Stk_Extensionline;
class Stk_SpecialLine;
class Stk_View;
class Stk_DocumentManager;
class Stk_Render;
class Stk_VirtualBomItemInfo;

/*************************************************************************************
	 *	@class	Stk_File
	 *	@brief	文件类
	 *
	 *	确定文件的基本属性信息。<br>
	 *	继承自Stk_Object类。
***************************************************************************************/

class  DATAMANAGER_API Stk_File:
	public Stk_Object
{
private:
	map<wstring,Stk_ProtoType*>	m_ProtoKeyMap;
	FILE						*m_BinFileP;
	STK_UINT64					m_nuHeadSeek;
	STK_ID						m_ID;
	STK_INT32					m_idRefCount;
	vector<Stk_ProtoType*>		m_vcTopProtoTypes;								//!< 顶级Prototype数组
	Stk_DocumentManager*		m_DocManager;
	STK_BOOL					m_bTrueFile;
	wstring						m_wstrTitle;									//!< 文件标志
	wstring						m_wstrCADFileName;								//!< 数据源-CADFile名称
	wstring						m_wstrDataSource;								//!< 数据源-CAD软件名称、版本
	wstring						m_wstrConvTookit;								//!< 数据交换类库-名称、版本
	wstring						m_wstrSVLConversion;							//!< SVL转换器-名称、版本
	wstring						m_wstrSVLVersion;								//!< SVL文件版本
	wstring						m_wstrCADPath;									//!< CAD物理文件路径
	wstring						m_wstrCreateTime;								//!< SVL生成时间
	wstring						m_wstrCompression;								//!< 压缩算法
	STK_UINT64					m_nuPolygonNum;									//!< 存储的面片数量
	STK_UINT64					m_nuProtoTypeNum;								//!< 存储的ProtoType数量
	Stk_Preview					*m_PreviewP;									//!< 预览图
	map<wstring,wstring>		m_mapOptStrings;								//!< 配置项
	STK_BOOL					m_bLineSetConvert;								//!< 转换线集标识
	STK_FLOAT32					m_fLineSetTol;
	wstring						m_wstrSVLPath;									//!< 文档的路径，多Document时需要改
	wstring						m_wstrSVLSaveTmpPath;							//!< 文档的临时保存路径
	wstring						m_wstrSVLSaveAsPath;							//!< 文档的另存路径，与保存路径m_wstrSVLPath区分开
	STK_BOOL					m_bIsTop;										//!< 是否是顶级文件
	STK_BOOL					m_bIsOldVersion;								//!< 是否是2.2以前的版本
private:
	StkCompressTypeEnum			m_eCompressType;								//!< 压缩方案
	STK_CHAR*					m_chSegBuffP;									//!< 段缓冲区，内部用
	STK_UINT32					m_nuCompressSegmentNum;							//!< 压缩段中包含的记录数，内部统计用
	STK_UINT32					m_nuReadCompressSegCount;						//!< 压缩段技术器，用于记录读取的压缩段数量
	STK_CHAR*					m_chBufferItorP;								//!< 缓冲区游标：用于记录头的记录数和长度的计算
	STK_BOOL					m_bIsCompress;									//!< 是否压缩，内部使用
	STK_UINT32					m_nuHaveAllocBlock;								//!< 已分配的内存块大小
	STK_UINT32					m_nuHaveUseBlock;								//!< 已用的内存大小
private:
	wstring						m_wstrPassword;									//!< 文件密码
	map<STK_ID, Stk_Render*>	m_mapRenders;									//!< 渲染(纹理)数据
	map<wstring, Stk_VirtualBomItemInfo*>	m_mapBomItemInfos;					//!< BOM表格
	map<Stk_ProtoType*, STK_BOOL>			m_mapFileProtos;					//!< File中的所有ProtoType
private:
	/****************************************************************************
	 *	@brief		设置段缓冲区指针
	 *	@param[in]	i_chSegBuffP	段缓冲区指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetSegBuffer(STK_CHAR* i_chSegBuffP);

	/****************************************************************************
	 *	@brief		获取段缓冲区指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		段缓冲区指针
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_CHAR*					GetSegBuffer();

	/****************************************************************************
	 *	@brief		清空段缓冲区
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		清空成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ClearSegBuffer();

	/****************************************************************************
	 *	@brief		添加压缩段数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					AddCompressSegmentNum();

	/****************************************************************************
	 *	@brief		获取压缩段数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		压缩段包含的记录数
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_UINT32					GetCompressSegmentNum();

	/****************************************************************************
	 *	@brief		清空压缩段数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		清空成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ClearCompressSegmentNum();

	/****************************************************************************
	 *	@brief		是否压缩标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		是否压缩标识
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_BOOL					IsCompressFile();

	/****************************************************************************
	 *	@brief		写工艺BOM文件
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteVirtualBomFile();

	/****************************************************************************
	 *	@brief		读取工艺BOM文件
	 *	@param[in]	i_VirtualBomHeadSP	工艺BOM表格段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_SUCCESS_EXIST	已读取跳出
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadVirtualBomFile(StkSegmentHead *i_VirtualBomHeadSP);

	/****************************************************************************
	 *	@brief		检索当前File中ProtoType的所有子ProtoType
	 *	@param[in]	i_stkProtoTypeP		当前File中ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			检索成功
	 *	@retval		其它				检索失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SearchFileProtoTypes(Stk_ProtoType* i_stkProtoTypeP);
public:
	/****************************************************************************
	 *	@brief		Stk_File构造函数
	 *	@param[in]	i_stkDocManagerP	DocManager对象指针
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_File(Stk_DocumentManager* i_stkDocManagerP);

	/****************************************************************************
	 *	@brief		Stk_File析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_File(void);

#if 0
	/****************************************************************************
	 *	@brief		File对象计数器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		内部接口
	 *	@attention	无
	 ****************************************************************************/
	void						AddRef();

	/****************************************************************************
	 *	@brief		File内存释放
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		V2.2版本以后不再使用此接口
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					Release();

	/****************************************************************************
	 *	@brief		注册File Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_SUCCESS_EXIST			已注册，跳出
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					RegFileLink();

	/****************************************************************************
	 *	@brief		查询File Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLPath			文件路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetFileLink();
#endif

	/*****************************************************************************
	 *	@brief		获取DocumentManager
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_DocManager	DocumentManager
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_DocumentManager*		GetDocumentManager();

	/****************************************************************************
	 *	@brief		注册File ID
	 *	@param[in]	i_idFile					文件ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		输入参数只能设置为STK_NEW_ID，不允许由用户指定ID <br>
	 *				指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idFile);

	/****************************************************************************
	 *	@brief		打开V2.2之前的文件
	 *	@param[in]	i_strSVLPath		文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			打开成功
	 *	@retval		STK_ERROR			打开失败
	 *	@note		V2.2以后不再使用此接口
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					OpenFile(wstring i_strSVLPath);


	/****************************************************************************
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
	STK_STATUS					OpenFile(wstring i_strSVLPath, FILE* &io_fileP, STK_UINT32& io_nuProtoTypeCount, vector<wstring>& o_vecChildPath);

	/****************************************************************************
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
	STK_STATUS					OpenCADFile(wstring i_strSVLPath, FILE* &io_fileP, vector<wstring>& o_vecChildPath);
	
	/*****************************************************************************
	 *	@brief		写CAD File文件
	 *	@param[in]	i_eSaveType			保存文件类型
	 *	@param[in]	i_AppendFlag		是否追加
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		STK_ERROR			写入失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					WriteCADFile(StkFileSaveTypeEnum i_eSaveType, STK_BOOL i_AppendFlag);

	/****************************************************************************
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
	STK_STATUS					ReadTitle(StkSegmentHead *i_TiHeadP);
	
	/*****************************************************************************
	 *	@brief		向SVL中写入文件头
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写入成功
	 *	@retval		STK_ERROR				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteTitle();
	
	/****************************************************************************
	 *	@brief		从SVL中读取文件配置项
	 *	@param[in]	i_OptHeadP						文件配置项的记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS						读取成功
	 *	@retval		STK_ERROR_MEMORY				内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadOption(StkSegmentHead *i_OptHeadP);
	
	/*****************************************************************************
	 *	@brief		向SVL中写入配置项
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写入成功
	 *	@retval		STK_ERROR				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteOption();

	/****************************************************************************
	 *	@brief		读取段头数据
	 *	@param[in]	i_HeadP				段头指针
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_HEADRD	读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentHead(StkSegmentHead *i_HeadP);
	
	/****************************************************************************
	 *	@brief		读取段中所有记录
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentAllRecord(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);				//!< 读取段中所有记录（内部接口）
	
	/****************************************************************************
	 *	@brief		段读取结束
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentFinish(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);
	
	/*****************************************************************************
	 *	@brief		写入数据段头
	 *	@param[in]	i_HeadSP				段头指针
	 *	@retval		STK_SUCCESS				写入成功
	 *	@retval		其它					写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentHead(StkSegmentHead *i_HeadSP);
	
	/*****************************************************************************
	 *	@brief		写入数据段记录
	 *	@param[in]	i_HeadSP				段头指针
	 *	@param[in]	i_ControlSP				表头指针
	 *	@retval		STK_SUCCESS				写入成功
	 *	@retval		其它					写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentRecord(StkSegmentHead *i_HeadSP,StkControl *i_ControlSP);
	
	/*****************************************************************************
	 *	@brief		写数据段的结束记录
	 *	@param[in]	i_HeadSP				头部
	 *	@retval		STK_SUCCESS				写文件成功
	 *	@retval		其它					写文件失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentFinish(StkSegmentHead *i_HeadSP);

	/*****************************************************************************
	 *	@brief		设置CADFile物理文件路径
	 *	@param[in]	i_wstrCADPath		CAD物理文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		调用该方法会自动计算CADFile名称并通过SetCADFileName设置到Stk_File中
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetCADFilePath(wstring i_wstrCADPath);
	
	/*****************************************************************************
	 *	@brief		获取CAD物理文件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrCADPath		CAD物理文件路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetCADFilePath();
	
	/****************************************************************************
	 *	@brief		设置SVL文件路径
	 *	@param[in]	i_wstrSVLPath
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetSVLPath(wstring i_wstrSVLPath);

	/****************************************************************************
	 *	@brief		获取SVL文件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLPath	SVL文件路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetSVLPath();

	/****************************************************************************
	 *	@brief		获取SVL文件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLPath	SVL文件路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetSVLSaveTmpPath();

	/****************************************************************************
	 *	@brief		根据输入的CADFile路径计算CADFile名称
	 *	@param[in]	i_wstrCADPath	CADFile路径
	 *	@param[out]	无
	 *	@retval		wstrSVLName		CADFile文件名称
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						ComputeCADFileName(wstring i_wstrCADPath);

	/****************************************************************************
	 *	@brief		获取数据源-文件名称	
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrCADFileName			文件名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetCADFileName();

	/****************************************************************************
	 *	@brief		设置PMI存储模式
	 *	@param[in]	i_eMode				存储模式
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetOptionOfPmiMode(StkPMIModeEnum i_eMode);

	/****************************************************************************
	 *	@brief		获取PMI存储模式
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		PMI_MODE_UNKNOW		模式未知
	 *	@retval		PMI_MODE_PARA		参数模式
	 *	@retval		PMI_MODE_POLY		折线模式
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkPMIModeEnum				GetOptionOfPmiMode();

	/*****************************************************************************
	 *	@brief		设置是否将SVL中的SubLine转换为LineSet
	 *	@param[in]	i_bflag					是否读取
	 *	@param[in]	i_fTol					公差
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						SetLineSetConvertFlag(STK_BOOL i_bflag, STK_FLOAT32 i_fTol);
	
	/*****************************************************************************
	 *	@brief		获取是否将SVL中的SubLine转换为LineSet
	 *	@param[in]	无
	 *	@param[out]	o_bflag					是否读取
	 *	@param[out]	o_fTol					公差
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						GetLineSetConvertFlag(STK_BOOL &o_bflag, STK_FLOAT32 &o_fTol);

	/****************************************************************************
	 *	@brief		设置文件预览图
	 *	@param[in]	i_PreviewP		文件预览图对象指针
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetFilePreview(Stk_Preview *i_PreviewP);

	/****************************************************************************
	 *	@brief		获取文件预览图
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_PreviewP		文件预览图对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Preview*				GetFilePreview();

	/****************************************************************************
	 *	@brief		查询File ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_ID			文件ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetFileID();

	/*****************************************************************************
	 *	@brief		添加顶级ProtoType
	 *	@param[in]	i_stkProtoTypeP			顶级ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddTopProtoType(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		获取当前File文件的顶级ProtoType数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		顶级ProtoType数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetTopProtoTypeNum();

	/*****************************************************************************
	 *	@brief		获取指定索引对应的顶级ProtoType
	 *	@param[in]	i_nuIndex			指定的索引号
	 *	@param[out]	无
	 *	@retval		指定的顶级ProtoType
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*				GetTopProtoTypeByIndex(STK_UINT32 i_nuIndex);
	
	/*****************************************************************************
	 *	@brief		从File文件中删除指定的Top ProtoType
	 *	@param[in]	i_stkProtoTypeP		指定的ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		一个File中有多个ProtoType时，除了是零件族之外，还有可能是CGR
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					DelTopProtoTypeFromFile(Stk_ProtoType* i_stkProtoTypeP);

	/******************************************************************************
	 *	@brief		设置数据源-软件名称、版本
	 *	@param[in]	i_wstrDataSource			软件名称、版本
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetDataSource(const wstring& i_wstrDataSource);

	/******************************************************************************
	 *	@brief		设置数据交换类库-名称、版本
	 *	@param[in]	i_wstrConvTookit			名称、版本
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetConvTookit(const wstring& i_wstrConvTookit);
	
	/******************************************************************************
	 *	@brief		设置SVL文件版本
	 *	@param[in]	i_wstrSVLConversion			名称、版本
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetSVLConversion(const wstring& i_wstrSVLConversion);
	
	/****************************************************************************
	 *	@brief		设置存储的面片数量
	 *	@param[in]	i_nuPolygonNum			面片数量
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetPolygonNum(STK_UINT64 i_nuPolygonNum);
	
	/****************************************************************************
	 *	@brief		设置存储的ProtoType数量
	 *	@param[in]	i_nuProtoTypeNum			ProtoType数量
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetProtoTypeNum(STK_UINT64 i_nuProtoTypeNum);

	/******************************************************************************
	 *	@brief		设置SVL文件版本
	 *	@param[in]	i_wstrSVLVersion			SVL文件版本
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetSVLVersion(const wstring& i_wstrSVLVersion);
	
	/******************************************************************************
	 *	@brief		设置SVL生成时间
	 *	@param[in]	i_wstrCreateTime			生成时间
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetSVLTime(const wstring& i_wstrCreateTime);
	
	/*******************************************************************************
	 *	@brief		设置SVL生成时间
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetSVLTime();

	/****************************************************************************
	 *	@brief		获取数据源-软件名称、版本	
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrDataSource			软件名称、版本
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetDataSource();

	/****************************************************************************
	 *	@brief		获取数据交换类库-名称、版本	
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrConvTookit			名称、版本
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetConvTookit();

	/****************************************************************************
	 *	@brief		获取SVL转换器-名称、版本	
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLConversion			名称、版本
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetSVLConversion();

	/****************************************************************************
	 *	@brief		获取SVL文件版本	
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLVersion			SVL文件版本
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetSVLVersion();

	/****************************************************************************
	 *	@brief		获取SVL生成时间
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrCreateTime			生成时间
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetSVLTime();

	/****************************************************************************
	 *	@brief		获取存储的面片数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuPolygonNum				面片数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	UINT64						GetPolygonNum();

	/****************************************************************************
	 *	@brief		获取当前文件中存储的ProtoType数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuProtoTypeNum			ProtoType数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	UINT64						GetProtoTypeNum();

	/*****************************************************************************
	 *	@brief		设置索引与ProtoType之间的关联
	 *	@param[in]	inKey					索引号
	 *	@param[in]	inProtoTypeP			要关联的ProtoType
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		STK_ERROR				设置失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					RegProtoTypeKey(wstring inKey, Stk_ProtoType *inProtoTypeP);
	
	/*****************************************************************************
	 *	@brief		获取指定的ProtoType
	 *	@param[in]	inKey					索引号
	 *	@param[out]	无
	 *	@retval		ProtoTypeP				指定的ProtoType
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*				GetProtoTypeByKey(wstring inKey);

	/****************************************************************************
	 *	@brief		判断File文件是否是顶级
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsTop		是否是顶级文件标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsTopFile();

	/****************************************************************************
	 *	@brief		设置是否是顶级File文件标识
	 *	@param[in]	i_bIsTop		是否是顶级File
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetTopFile(STK_BOOL i_bIsTop);

	/****************************************************************************
	 *	@brief		预先读取SVL的版本号
	 *	@param[in]	i_strSVLPath	SVL文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					PreOpenFile(wstring i_strSVLPath);

	/****************************************************************************
	 *	@brief		获取文件标志
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		文件标志
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetTitle();

	/****************************************************************************
	 *	@brief		获取文件标志
	 *	@param[in]	i_wstrTitle		文件标志
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetTitle(wstring i_wstrTitle);

	/****************************************************************************
	 *	@brief		设置文件版本
	 *	@param[in]	i_bOldVersion	版本标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetOldVersion(STK_BOOL i_bOldVersion);

	/****************************************************************************
	 *	@brief		是否是2.2之前的旧版本
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		版本标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsOldVersion();

	/****************************************************************************
	 *	@brief		设置SVL文件另存路径
	 *	@param[in]	i_wstrSVLSaveAsPath
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetSVLSaveAsPath(wstring i_wstrSVLSaveAsPath);

	/****************************************************************************
	 *	@brief		获取SVL文件另存路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrSVLSaveAsPath	SVL文件另存路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetSVLSaveAsPath();
	
	/****************************************************************************
	 *	@brief		设置压缩方案（压缩比/效率优先）
	 *	@param[in]	i_eCompressType	压缩方案
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCompressType(StkCompressTypeEnum i_eCompressType);

	/****************************************************************************
	 *	@brief		获取压缩方案
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		压缩方案
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkCompressTypeEnum			GetCompressType();

	/****************************************************************************
	 *	@brief		压缩数据
	 *	@param[in]	i_headP					段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				解压成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					CompressData(StkSegmentHead* i_stkHeadP);

	/****************************************************************************
	 *	@brief		解压数据
	 *	@param[in]	i_CompressRecordP		待解压记录的指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				解压成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					DeCompressData(StkCompressRec* i_CompressRecordP);

	/****************************************************************************
	 *	@brief		用于压缩记录的缓冲区申请
	 *	@param[in]	i_HeadSP				段头指针
	 *	@param[in]	i_RecordP				待压缩记录
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				申请成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ApplySegmentRecordBuffer(StkSegmentHead *i_HeadSP, StkControl* i_RecordP);

	/****************************************************************************
	 *	@brief		用于压缩段头的缓冲区申请
	 *	@param[in]	i_HeadP					待压缩段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				申请成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ApplySegmentHeadBuffer(StkSegmentHead* i_HeadP);

	/****************************************************************************
	 *	@brief		用于压缩段头的缓冲区申请
	 *	@param[in]	i_HeadP					待压缩段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				申请成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ApplySegmentFinishBuffer(StkSegmentHead* i_HeadP);

	/****************************************************************************
	 *	@brief		读取段头Buffer数据
	 *	@param[in]	i_HeadP				段头指针
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_HEADRD	读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentHeadBuffer(StkSegmentHead *i_HeadP);
	
	/****************************************************************************
	 *	@brief		读取段Buffer中所有记录
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentAllRecordBuffer(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);
	
	/****************************************************************************
	 *	@brief		段Buffer读取结束
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentFinishBuffer(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);
	
	/****************************************************************************
	 *	@brief		写段记录数据
	 *	@param[in]	i_HeadSP				段头指针
	 *	@param[in]	i_RecordP				段记录
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentRecordData(StkSegmentHead *i_HeadSP, StkControl* i_RecordP);

	/****************************************************************************
	 *	@brief		写段头数据
	 *	@param[in]	i_HeadP					段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentHeadData(StkSegmentHead* i_HeadP);

	/****************************************************************************
	 *	@brief		写段的结束记录数据
	 *	@param[in]	i_HeadP					段的结束记录
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSegmentFinishData(StkSegmentHead* i_HeadP);

	/****************************************************************************
	 *	@brief		读取段头Buffer数据
	 *	@param[in]	i_HeadP				段头指针
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_HEADRD	读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentHeadData(StkSegmentHead *i_HeadP);
	
	/****************************************************************************
	 *	@brief		读取段Buffer中所有记录
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentAllRecordData(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);
	
	/****************************************************************************
	 *	@brief		段Buffer读取结束
	 *	@param[in]	i_HeadP					段头指针
	 *	@param[in]	i_RecordA				记录指针
	 *	@retval		STK_SUCCESS				读取成功
	 *	@retval		STK_ERROR_FILE_MEMORY	文件内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSegmentFinishData(StkSegmentHead *i_HeadP, StkControl *i_RecordA[]);
	
	/*****************************************************************************
	 *	@brief		向SVL中写入各个段被压缩后的段
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				写入成功
	 *	@retval		STK_ERROR				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteCompress();

	/****************************************************************************
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
	STK_STATUS					ReadCompress(StkSegmentHead *i_headP);

	/****************************************************************************
	 *	@brief		设置文件密码
	 *	@param[in]	i_wstrPassword			文件密码
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		其他					设置失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetPassword(wstring i_wstrPassword);

	/****************************************************************************
	 *	@brief		获取文件密码
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		文件密码
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetPassword();

	/****************************************************************************
	 *	@brief		添加渲染(纹理)信息
	 *	@param[in]	i_stkRenderP	渲染(纹理)信息
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@retval		其它			添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddRender(Stk_Render* i_stkRenderP);

	/****************************************************************************
	 *	@brief		获取渲染(纹理)数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		渲染(纹理)数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetRenderNum();

	/****************************************************************************
	 *	@brief		获取指定的渲染(纹理)
	 *	@param[in]	i_nuIndex		索引
	 *	@param[out]	无
	 *	@retval		指定的渲染(纹理)
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Render*					GetRenderByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		添加工艺BOM表格信息
	 *	@param[in]	i_wstrID				BOM表格的序号
	 *	@param[in]	i_stkVirtualBomInfoP	工艺BOM表格信息
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@retval		其他					添加失败
	 *	@note		无
	 *	@attention	内部接口，只做存储时使用，不需要用户调用该接口
	 ****************************************************************************/
	STK_STATUS					AddVirtualBomTableInfo(wstring i_wstrID, Stk_VirtualBomItemInfo* i_stkVirtualBomInfoP);
};
#endif