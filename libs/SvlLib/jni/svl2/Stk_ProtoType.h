// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/****************************************************************************
 *	@file	Stk_ProtoType.h
 *	@brief	Stk_ProtoType组件类的声明文件
 ****************************************************************************/

#ifndef _Stk_ProtoType_H_
#define _Stk_ProtoType_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Manager.h"
#include "Stk_Node.h"
#include "Stk_Define.h"
#include "Stk_Object.h"
#include "Stk_TopoEntity.h"
#include "Stk_PMIEntity.h"
#include "Stk_View.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_Manager.h"
#include "Stk_UserData.h"
#include "Stk_ComponentFeature.h"
#include "Stk_InsAttribute.h"
#include "Stk_Render.h"
class Stk_DocumentManager;
class Stk_ProtoIDManager;

/**************************************************************************************
	 *	@class	Stk_ProtoType
	 *	@brief	ProtoType 组件类
	 *
	 *	继承自Stk_Object类。
***************************************************************************************/

class Stk_Instance;

class DATAMANAGER_API  Stk_ProtoType :
	public Stk_Object
{
protected:
	STK_ID						m_ID;
	wstring						m_wstrName;					//!< ProtoType名字
	StkProtoTypeEnum			m_eProtoType;				//!< 文件类型
	wstring						m_wstrLink;					//!< LinkString
	Stk_File*					m_stkFileP;					//!< ProtoType所属File
	STK_ID						m_idPlcMax;					//!< 配置矩阵ID，用于比较发行最大者
	wstring						m_wstrCADPath;				//!< CAD 物理文件路径
	//wstring					m_wstrSVLPath;				//!< SVL 文件路径
	wstring						m_wstrCADFileName;			//!< CAD File 名字
	vector<Stk_Instance*>		m_vcChildInss;				//!< 子实例数组
	map<STK_ID,STK_UINT32>		m_mapChildInss;				//!< 为提高m_ChildInss的查询速度所设计的map
	//vector<Stk_Body*>			m_vcBodys;					//!< Body
	//vector<Stk_PMI*>			m_vcPMIs;					//!< PMI
	//vector<Stk_View*>			m_vcViews;					//!< View
	//vector<Stk_Note*>			m_vcNotes;					//!< Note
	vector<Stk_MetaData*>		m_vcMetaDatas;				//!< 自定义属性
	map<wstring,map<STK_ID,Stk_UserData*>*>	m_mapUserDatas;	//!< 用户数据
	STK_SIZE_T					m_szUserDataCnt;			//!< 用户数据数量
	STK_INT32					m_idRefCount;				//!< 对象计数器
	STK_UINT32					m_nuLODCount;				//!< LOD等级数
	STK_BOOL					m_bIsDirtyData;				//!< 是否脏数据
	wstring						m_wstrProtokey;
	// 在2.2版之后，追加了Node层
	Stk_Node*					m_TopNodeP;					//!< 顶级Node
	// 在2.2版之前没有Node层
	STK_BOOL					m_bIsNodeMode;				//!< 是否是Node模式
	STK_BOOL					m_isCatalog;				//!< 是否是Catalog
	STK_BOOL					m_bPipe;					//!< 是否是线缆(Pipe)零件
	STK_BOOL					m_isCADFileTop;				//!< 是否是CADFile的顶级Proto
	//map<wstring, StkDisplayStateEnum>	m_mapDisplayState;	//!< Prototype显隐路径->已废弃，在实例属性中实现
	STK_DOUBLE64				m_dUnitFactor;				//!< 长度单位
	vector<Stk_ComponentFeature*>	m_vcComponentFeats;		//!< 组件特征数组
	STK_BOOL					m_bIsDeleting;				//!< 是否处于析构状态
	STK_BOOL					m_bIsUseNumber;				//!< 是否使用编号方式显示面板
	STK_BOOL					m_bDisplay;					//!< 显隐状态
	STK_BOOL					m_bHasColor;				//!< ProtoType是否有颜色
	STK_RGBA32					m_rgbaProtoType;			//!< ProtoType颜色
	map<wstring, Stk_InsAttribute*>	m_mapInsAttribute;		//!< 子实例属性
	vector<Stk_Render*>			m_vcRenders;				//!< 渲染(纹理)数据
private:
	Stk_DocumentManager*		m_DocManager;
	Stk_ProtoIDManager*			m_ProtoIDManager;

	/****************************************************************************
	 *	@brief		将V2.2之前的Body、PMI等转换为Node模式
	 *	@param[in]	i_eType			Node类型
	 *	@param[in]	i_stkDataP		Node对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		转换成功
	 *	@retval		其他			转换失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ConvertEntityToNode(StkNodeTypeEnum i_eType, Stk_Entity* i_stkDataP);

	/****************************************************************************
	 *	@brief		将V2.2之前的Body、PMI等转换为Node模式
	 *	@param[in]	i_eType			Node类型
	 *	@param[in]	i_stkDataP		要素指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		转换成功
	 *	@retval		其他			转换失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					AddEntityToTopNode(StkNodeTypeEnum i_eType, Stk_Entity* i_stkDataP);

	/****************************************************************************
	 *	@brief		转换Body信息
	 *	@param[in]	i_StkBodyP		Body对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入Body型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ConvertBody(Stk_Body *i_StkBodyP);

	/****************************************************************************
	 *	@brief		转换View信息
	 *	@param[in]	i_StkViewP		View对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入View型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ConvertView(Stk_View *i_StkViewP);

	/****************************************************************************
	 *	@brief		转换PMI数据信息
	 *	@param[in]	i_StkPMIP		PMI对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入PMI型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ConvertPMI(Stk_PMI* i_StkPMIP);

	/****************************************************************************
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
	wstring						AdjustChildInsLoadPath(const wstring& i_wstrInsSavePath, const wstring& i_wstrPaProtoLoadPath);
	
	/****************************************************************************
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
	STK_STATUS					WriteInsAttribute(wstring i_wstrInsPlcPath, Stk_InsAttribute* i_stkInsAttrP, Stk_File* i_stkFileP);

	/****************************************************************************
	 *	@brief		读ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)段
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_InsAttrHeadP		实例属性段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention
	 ****************************************************************************/
	STK_STATUS					ReadInsAttribute(Stk_File* i_stkFileP, StkSegmentHead *i_InsAttrHeadP);

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
	STK_STATUS					DetectPathFromBeforeSave(Stk_ProtoType* i_stkParentProtoP, Stk_ProtoType* i_stkCurProtoP, wstring& o_wstrSVLPath);

	/****************************************************************************
	 *	@brief		根据输入的CADFile路径计算CADFile名称
	 *	@param[in]	i_wstrCADPath	CADFile路径
	 *	@param[out]	无
	 *	@retval		wstrSVLName		CADFile文件名称
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						ComputeCADFileName(wstring i_wstrCADPath);

	/************************************************************************//**
	 *	@brief		遍历当前ProtoType的所有子实例
	 *	@param[in]	i_stkProtoTypeP		当前ProtoType对象
	 *	@param[out]	o_nuInstanceNum		子实例数量
	 *	@retval		STK_SUCCESS			遍历成功
	 *	@retval		其它				遍历失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SearchChildInstanceFromProtoType(Stk_ProtoType* i_stkProtoTypeP, STK_UINT32& o_nuInstanceNum);

	/************************************************************************//**
	 *	@brief		遍历当前ProtoType的所有子实例
	 *	@param[in]	i_stkNodeP			顶级Node
	 *	@param[out]	o_nuNodeNum			子Node数量
	 *	@retval		STK_SUCCESS			遍历成功
	 *	@retval		其它				遍历失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SearchChildNodeFromTopNode(Stk_Node* i_stkNodeP, STK_UINT32& o_nuNodeNum);
public:
	/****************************************************************************
	 *	@brief		遍历指定类型的所有Node
	 *	@param[in]	i_eType			Node类型
	 *	@param[in]	i_stkDataP		Node对象指针
	 *	@param[out]	o_vcNodes		指定类型的所有Node
	 *	@retval		STK_SUCCESS		遍历成功
	 *	@retval		其他			遍历失败
	 *	@note		无
	 *	@attention
	 ****************************************************************************/
	STK_STATUS					PreOrderNode(Stk_Node* i_stkNodeP, StkNodeTypeEnum i_eType, vector<Stk_Node*>& o_vcNodes);

	/****************************************************************************
	 *	@brief		Stk_ProtoType构造函数
	 *	@param[in]	i_wstrLink			LinkString
	 *	@param[in]	i_stkDocManagerP	DocManager
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		浏览器中，打开ProtoType时专用构造函数
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType(const wstring &i_wstrLink, Stk_DocumentManager* i_stkDocManagerP);
	//Stk_ProtoType(const Stk_ProtoType& i_stkProtoType);
	
	/****************************************************************************
	 *	@brief		Stk_ProtoType析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ProtoType(void);
	//Stk_ProtoType& operator = (const Stk_ProtoType& i_stkProtoType);

	/****************************************************************************
	 *	@brief		获取ProtoType ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID	ProtoType ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();
	
	/*****************************************************************************
	 *	@brief		设置ProtoType ID
	 *	@param[in]	i_ID			ProtoType ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);
	
	/****************************************************************************
	 *	@brief		获取ProtoType的名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrName	ProtoType的名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetProtoName();
	
	/*****************************************************************************
	 *	@brief		设置ProtoType的名称
	 *	@param[in]	i_wstrName			ProtoType的名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		ProtoType名字不允许出现'<'、'>'等特殊字符，该字符被转换为'_'
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetProtoName(const wstring& i_wstrName);
	
	/****************************************************************************
	 *	@brief		获取ProtoType的Key
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrProtokey		ProtoType的Key
	 *	@note		将PMI存储在外部XML文件的临时方案专用
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetProtoKey();
	
	/****************************************************************************
	 *	@brief		获取ProtoType类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eProtoType	ProtoType类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkProtoTypeEnum			GetType();
	
	/****************************************************************************
	 *	@brief		设置ProtoType类型
	 *	@param[in]	i_eType		ProtoType类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						SetType( StkProtoTypeEnum i_eType);	

	/*****************************************************************************
	 *	@brief		设置ProtoType所属的File
	 *	@param[in]	i_stkFileP		文件对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						SetFile(Stk_File* i_stkFileP);
	
	/*****************************************************************************
	 *	@brief		获取ProtoType所属的File
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_stkFileP		ProtoType所属的File
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_File*					GetFile();

	/*****************************************************************************
	 *	@brief		获取CAD文件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrCADPath	CAD文件路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetCADFilePath();

	/*****************************************************************************
	 *	@brief		设置CAD物理路径路径
	 *	@param[in]	i_wstrCADFilePath		CAD物理文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		通过SetCADFilePath时会自动匹配并填写CADFileName
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetCADFilePath(const wstring& i_wstrCADFilePath);

	/*****************************************************************************
	 *	@brief		获取CAD物理文件名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrCADFileName		CAD物理文件名称
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						GetCADFileName();


	/****************************************************************************
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
	//STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ProtoHeadP);
	
	/*****************************************************************************
	 *	@brief		保存CAD文件中的ProtoType
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[in]	i_eSaveType			保存类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			保存成功
	 *	@retval		STK_ERROR			保存失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					WriteCADFile(Stk_File *i_stkFileP, StkFileSaveTypeEnum i_eSaveType);
	
	/****************************************************************************
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
	STK_STATUS					ReadCADFile(Stk_File *i_FileP, StkSegmentHead *i_ProtoHeadP);
	
	/****************************************************************************
	 *	@brief		注册ProtoType ID
	 *	@param[in]	i_idProtoType	ProtoType ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数 <br>
	 *				1. 如果注册新ID，需要先使用IDManager进行RegProtoLinkString，再调用本类SetSetLinkString
	 *				2. 如果注册指定ID，直接调用本接口即可
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idProtoType);
	
	/****************************************************************************
	 *	@brief		获取ProtoType Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrLink		ProtoType Link
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetLinkString();
	
	/****************************************************************************
	 *	@brief		设置ProtoType Link
	 *	@param[in]	i_wstrLink		ProtoType Link
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetLinkString(wstring i_wstrLink);

	/****************************************************************************
	 *	@brief		获取ProtoIDManager
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ProtoIDManager		ProtoType内的ID管理器
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_ProtoIDManager*			GetProtoIDManager();
	
	/*****************************************************************************
	 *	@brief		根据CAD文件路径注册SVL
	 *	@param[in]	i_wstrCADPath			CAD物理文件路径
	 *	@param[out]	o_stkCADFileP			File文件对象指针
	 *	@retval		STK_SUCCESS				注册成功
	 *	@note		适用于转换器
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegCADFile(wstring i_wstrCADPath, Stk_File*& o_stkCADFileP);

	/****************************************************************************
	 *	@brief		对象计数器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	void						AddRef();

	/****************************************************************************
	 *	@brief		设置对象计数器
	 *	@param[in]	i_nuRef		对象计数器
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		在读取ReadCADFile时，下级Proto没有创建，因此只能先记录下数目，在LoadFile时将计数器设置给ProtoType
	 *	@attention	内部接口
	 ****************************************************************************/
	void						SetRef(STK_UINT32 i_nuRef);

	/****************************************************************************
	 *	@brief		获取对象计数器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idRefCount	对象计数器
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_ID						GetRefCount();

	/****************************************************************************
	 *	@brief		ProtoType内存释放
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	void						Release();

	/****************************************************************************
	 *	@brief		ProtoType子实例内存释放
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		1. 删除所有的子实例  <br>
	 *				2. 释放子实例 <br>
	 *				3. 子ProtoType引用计算器减1
	 *	@attention	内部接口
	 ****************************************************************************/
	void						DelAllChildren();
	
	/****************************************************************************
	 *	@brief		移除指定实例的装配关系
	 *	@param[in]	i_stkInsP		实例对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		移除成功
	 *	@note		解除配置关系，不释放Instance内存，不减引用计数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RemoveChild(Stk_Instance* i_stkInsP);

	/****************************************************************************
	 *	@brief		设置LOD等级数
	 *	@param[in]	i_nuLODCount	LOD等级数
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetLODCount(STK_UINT32 i_nuLODCount);
	
	/****************************************************************************
	 *	@brief		获取LOD等级数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuLODCount	LOD等级数
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetLODCount(void);
	
	/****************************************************************************
	 *	@brief		获取ProtoType的子实例数目
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcChildInss.size()	子实例数目
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetChildInsNum();
	
	/****************************************************************************
	 *	@brief		获取ProtoType中指定的子实例
	 *	@param[in]	i_nuKeyPose		要获取的子实例索引
	 *	@param[out]	无
	 *	@retval		pInstance		子实例
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Instance*				GetChildIns(STK_UINT32 i_nuKeyPose);
	
	/****************************************************************************
	 *	@brief		通过配置ID获取子实例
	 *	@param[in]	i_idPlacement	配置ID
	 *	@param[out]	无
	 *	@retval		pInstance		子实例
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Instance*				GetChildByPlcID(STK_ID i_idPlacement);
	
	/*****************************************************************************
	 *	@brief		插入实例
	 *	@param[in]	stkInsP			实例对象
	 *	@param[in]	i_IDPlc			配置ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@retval		其它			添加失败
	 *	@note		1. 由用户指定配置ID <br>
	 *				2. 需要调用者为子实例的ProtoType进行AddRef
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddChild(STK_ID i_IDPlc, Stk_Instance* stkInsP);
	
	/*****************************************************************************
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
	STK_ID						InsertChild(Stk_Instance* i_stkInsP);

	/****************************************************************************
	 *	@brief		获取Body数目
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcBodys.size()	Body数目
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetBodyNum();

	/****************************************************************************
	 *	@brief		获取指定的Body信息
	 *	@param[in]	i_nuKeyPose		指定的Body索引
	 *	@param[out]	无
	 *	@retval		StkBodyP		Body
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Body*					GetBody(STK_UINT32 i_nuKeyPose);

	/****************************************************************************
	 *	@brief		通过ID获取指定的Body信息
	 *	@param[in]	i_idBody		指定的BodyID
	 *	@param[out]	o_BodyP			Body指针
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetBodyByID(STK_ID i_idBody, Stk_Body* &o_BodyP);
	
	/****************************************************************************
	 *	@brief		添加Body信息
	 *	@param[in]	i_StkBodyP		Body对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入Body型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddBody(Stk_Body *i_StkBodyP);

	/****************************************************************************
	 *	@brief		获取View视图数目
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcViews.size()	View视图数目
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetViewNum();

	/****************************************************************************
	 *	@brief		获取指定的View信息
	 *	@param[in]	i_nuKeyPose		指定的View索引
	 *	@param[out]	无
	 *	@retval		StkViewP		View
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_View*					GetView(STK_UINT32 i_nuKeyPose);
	
	/****************************************************************************
	 *	@brief		添加View信息
	 *	@param[in]	i_StkViewP		View对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入View型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddView(Stk_View *i_StkViewP);

	/****************************************************************************
	 *	@brief		添加PMI数据信息
	 *	@param[in]	i_StkPMIP		PMI对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		该接口为V2.16前使用，为保持兼容性该接口会在Node树的顶级Node下插入PMI型的Node节点
	 *				V2.2后请按照Node树的方式插入
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddPMI(Stk_PMI* i_StkPMIP);

	/****************************************************************************
	 *	@brief		获取PMI数据数组
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcPMIs		PMI数据数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_PMI*>			GetPMI();
	
	/****************************************************************************
	 *	@brief		根据ID删除指定PMI
	 *	@param[in]	i_idPMI			指定的PMI ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelPMIByID(STK_ID i_idPMI);

	/****************************************************************************
	 *	@brief		释放所有的视图对象内存
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		释放成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RemoveAllView();

	/****************************************************************************
	 *	@brief		释放指定的视图对象内存
	 *	@param[in]	i_idView		视图ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		释放成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteView(STK_ID i_idView);

	/*****************************************************************************
	 *	@brief		设置数据状态(是否是脏数据)
	 *	@param[in]	i_bIsDirty	数据状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDataStatus(STK_BOOL i_bIsDirty);

	/****************************************************************************
	 *	@brief		判断是否是脏数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsDirtyData				是否脏数据
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_BOOL					IsDirtyData();

	/****************************************************************************
	 *	@brief		判断是否为Pipe组件
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		是否为Pipe组件
	 *	@note		无
	 *	@attention	
	 ****************************************************************************/
	STK_BOOL					IsPipe();

	/****************************************************************************
	 *	@brief		设置Pipe组件标志
	 *	@param[in]	i_bPipe		是否
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	
	 ****************************************************************************/
	STK_STATUS					SetPipeFlag(STK_BOOL i_bPipe);

	/****************************************************************************
	 *	@brief		获取长度单位
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		长度单元比例
	 *	@note		无
	 *	@attention	
	 ****************************************************************************/
	STK_DOUBLE64				GetConceptionUnitScale();

	/****************************************************************************
	 *	@brief		设置长度单位
	 *	@param[in]	i_UnitFactor	长度单元比例
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	
	 ****************************************************************************/
	VOID						SetConceptionUnitScale(STK_DOUBLE64 i_UnitFactor);

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
	STK_STATUS					AddUserData(Stk_UserData* i_stkUserDataP);

	/****************************************************************************
	 *	@brief		通过名称获取用户数据列表
	 *	@param[in]	用户数据名称
	 *	@param[out]	无
	 *	@retval		用户数据列表
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	map<STK_ID,Stk_UserData*>* GetUserDataByName(wstring i_wstrDataName);

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
	STK_STATUS					DelUserData( wstring i_wstrDataName, STK_ID i_id);

	/*****************************************************************************
	 *	@brief		往SVL文件中填写用户数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部使用
	 ****************************************************************************/
	STK_STATUS					WriteUserData(Stk_File* i_stkFileP);

	/*****************************************************************************
	 *	@brief		从SVL文件中读取用户数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[in]	i_UserHeadP		自定义属性记录头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					ReadUserDatas(Stk_File *i_FileP, StkSegmentHead *i_UserHeadP);
	
	/*****************************************************************************
	 *	@brief		往当前ProtoType中添加自定义属性数据
	 *	@param[in]	i_MetaDataP		自定义属性数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMetaData(Stk_MetaData* i_MetaDataP);
	
	/*****************************************************************************
	 *	@brief		获取当前ProtoType中所有的自定义属性数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcMetaDatas	自定义属性数据数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_MetaData*>& GetAllMetaDatas();
	
	/*****************************************************************************
	 *	@brief		获取当前ProtoType中所有的自定义属性数据数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcMetaDatas.size()	自定义属性数据数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetMetaDataNum();
	
	/****************************************************************************
	 *	@brief		通过自定义属性名删除指定的对象
	 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败（没有找到）
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelMetaDataByTitle(wstring i_wstrMetaDataTitle);

	/****************************************************************************
	 *	@brief		通过自定义属性名获取指定对象
	 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
	 *	@param[out]	无
	 *	@retval		自定义属性对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MetaData*				GetMetaDataByTitle(wstring i_wstrMetaDataTitle);

	/*****************************************************************************
	 *	@brief		从SVL文件中读取自定义属性数据
	 *	@param[in]	i_FileP			SVL文件指针
	 *	@param[in]	i_PropHeadP		自定义属性记录头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP);
	
	/*****************************************************************************
	 *	@brief		往SVL文件中填写自定义属性数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteMetaData(Stk_File* i_stkFileP);

	/*****************************************************************************
	 *	@brief		获取当前ProtoType下的所有线集
	 *	@param[in]	无
	 *	@param[out]	o_vcLineSets	线集
	 *	@retval		STK_SUCCESS		获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetAllLineSets(vector<Stk_LineSet *>& o_vcSubLines);
	
	/*****************************************************************************
	 *	@brief		获取当前ProtoType下的所有辅助线
	 *	@param[in]	无
	 *	@param[out]	o_vcSubLines	辅助线
	 *	@retval		STK_SUCCESS		获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetAllSubLines(vector<Stk_CurveEntity *>& o_vcSubLines);

	/****************************************************************************
	 *	@brief		判断是否是Node模式
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsNodeMode				Node模式标识
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_BOOL					IsNodeMode();										//查询是否是Node模式
	
	/****************************************************************************
	 *	@brief		设置是否是Node模式
	 *	@param[in]	i_bIsNodeMode				是否是Node模式标识
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	void						SetNodeMode(STK_BOOL i_bIsNodeMode);
	
	/****************************************************************************
	 *	@brief		设置顶级的Node节点
	 *	@param[in]	i_stkNodeP				Node节点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@retval		STK_ERROR				设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetTopNode(Stk_Node* i_stkNodeP);
	
	/****************************************************************************
	 *	@brief		获取顶级Node节点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_TopNodeP				顶级Node节点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Node*					GetTopNode();
	
	/*****************************************************************************
	 *	@brief		添加配置路径与显隐状态之间的映射
	 *	@param[in]	i_wstrDisplayPath		配置路径
	 *	@param[in]	i_eDisplayState			显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		为适应UG场景而添加
	 *	@attention	已废弃，在实例属性中实现
	 ****************************************************************************/
	//STK_STATUS					AddDisplayState(wstring i_wstrDisplayPath, StkDisplayStateEnum i_eDisplayState);	//实例显隐
	
	/*****************************************************************************
	 *	@brief		获取配置路径与显隐状态之间的映射
	 *	@param[in]	i_wstrPlcPath			实例配置路径
	 *	@param[out]	o_eDisplayState			显隐状态
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR				获取失败
	 *	@note		无
	 *	@attention	已废弃，在实例属性中实现
	 ****************************************************************************/
	//STK_STATUS					GetDisplayStateByPlcPath(wstring i_wstrPlcPath, StkDisplayStateEnum& o_eDisplayState);
	
	/*****************************************************************************
	 *	@brief		是否为CGR类型文件
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_isCatalog		是否为CGR标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsCatalog();
	
	/*****************************************************************************
	 *	@brief		设置是否为Catalog型的标识
	 *	@param[in]	i_bCatalog		是否为Catalog
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						SetCatalog(STK_BOOL i_bCatalog);

	/*****************************************************************************
	 *	@brief		是否为CADFile的TopProtoType
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_isCADFileTop		是否为TopProtoType
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsCADFileTop();
	
	/*****************************************************************************
	 *	@brief		设置该ProtoType为CADFile的Top标志
	 *	@param[in]	i_isCADFileTop		是否为TopProtoType
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	VOID						SetCADFileTop(STK_BOOL i_bCatalog);

	/****************************************************************************
	 *	@brief		根据上级ProtoType的加载路径和Instance的保存路径获取ChildIns的加载路径
	 *	@param[in]	i_wstrInsSavePath			Ins的保存路径
	 *	@param[in]	i_wstrPaProtoLoadPath		上级ProtoType的加载路径
	 *	@param[out]	无
	 *	@retval		wstrPath					ChildIns的加载路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						ConvertChildInsLoadPath(const wstring& i_wstrInsSavePath, const wstring& i_wstrPaProtoLoadPath);
	
	/****************************************************************************
	 *	@brief		根据顶级ProtoType的存放路径获取ChildIns保存的路径
	 *	@param[in]	i_wstrPath			顶级ProtoType的存放路径
	 *	@param[out]	无
	 *	@retval		wstrPath			ChildIns保存的路径
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring						ConvertPathToDir(const wstring& i_wstrPath);

	/****************************************************************************
	 *	@brief		添加组件特征
	 *	@param[in]	i_ComponentFeatP	组件特征
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					AddComponentFeature(Stk_ComponentFeature* i_ComponentFeatP);

	/****************************************************************************
	 *	@brief		获取组件特征数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		组件特征数量
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_UINT32					GetComponentFeatureNum();

	/****************************************************************************
	 *	@brief		获取指定的组件特征
	 *	@param[in]	i_ComponentFeatP	组件特征
	 *	@param[out]	无
	 *	@retval		指定的组件特征
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_ComponentFeature*		GetComponentFeatureByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		是否处于析构状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		true		是
	 *	@retval		false		否
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_BOOL					IsDeleting();

	/****************************************************************************
	 *	@brief		是否使用编号方式显示装配面板
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		true		是
	 *	@retval		false		否
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsUseNumber();

	/****************************************************************************
	 *	@brief		设置是否使用编号方式显示装配面板
	 *	@param[in]	i_bUseNumber是否使用编号方式显示装配面板
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		false		否
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetUseNumber(STK_BOOL i_bUseNumber);

	/****************************************************************************
	 *	@brief		ProtoType是否有颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		是否有颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasColor();

	/****************************************************************************
	 *	@brief		设置ProtoType颜色
	 *	@param[in]	i_rgbaProtoType	ProtoType颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetColor(STK_RGBA32 i_rgbaProtoType);

	/****************************************************************************
	 *	@brief		获取ProtoType颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		ProtoType颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		设置ProtoType的显隐
	 *	@param[in]	i_bDisplay		显隐状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDisplayState(STK_BOOL i_bDisplay);
	
	/****************************************************************************
	 *	@brief		获取ProtoType的显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例的显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					GetDisplayState();

	/****************************************************************************
	 *	@brief		添加子实例的显隐状态
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_eDisplayState		显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState);

	/****************************************************************************
	 *	@brief		获取指定配置路径下子实例的显隐状态
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_eDisplayState		子实例的显隐状态
	 *	@retval		子实例的显隐状态
	 *	@note		STK_SUCCESS			获取成功
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState);

	/****************************************************************************
	 *	@brief		添加子实例的颜色
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_rgbaIns			子实例的颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns);

	/****************************************************************************
	 *	@brief		获取指定配置路径下的子实例的颜色
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_rgbaIns			子实例的颜色
	 *	@retval		指定的子实例的颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns);

	/****************************************************************************
	 *	@brief		添加配置路径下子实例的材质
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[in]	i_matIns			子实例的材质
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns);

	/****************************************************************************
	 *	@brief		获取指定的子实例的材质
	 *	@param[in]	i_wstrInsPlcPath	子实例的配置路径
	 *	@param[out]	o_matIns			子实例的材质
	 *	@retval		指定的子实例的材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns);

	/****************************************************************************
	 *	@brief		获取默认视图
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		默认视图
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_View*					GetDefaultView();

	/****************************************************************************
	 *	@brief		获取包含实例属性的所有配置路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所有配置路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<wstring>				GetAllInsAttrPlcPath();

	/****************************************************************************
	 *	@brief		删除指定配置路径的实例属性
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath);

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
	//STK_UINT32					GetRenderNum();

	/****************************************************************************
	 *	@brief		获取指定的渲染(纹理)
	 *	@param[in]	i_nuIndex		索引
	 *	@param[out]	无
	 *	@retval		指定的渲染(纹理)
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//Stk_Render*					GetRenderByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		获取总实例数量
	 *	@param[in]	i_stkProtoTypeP		当前ProtoType节点
	 *	@param[out]	无
	 *	@retval		总实例数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetTotalInstanceNum(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		获取Node节点数量
	 *	@param[in]	i_stkProtoTypeP		当前ProtoType节点
	 *	@param[out]	无
	 *	@retval		Node节点数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetNodeNum(Stk_ProtoType* i_stkProtoTypeP);

};

#endif