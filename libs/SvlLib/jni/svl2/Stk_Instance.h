// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Instance.h
 *
 *	@brief	为获取ProtoType时用
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_Instance_H_
#define _Stk_Instance_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"

class Stk_File;
class Stk_ProtoType;
class Stk_DocumentManager;
class Stk_MetaData;

/**************************************************************************************
	 *	@class	Stk_File
	 *	@brief	Instance实例类
	 *
	 *	ProtoType的实例。<br>
	 *	继承自Stk_Object类。
***************************************************************************************/

class DATAMANAGER_API  Stk_Instance :
	public Stk_Object
{
protected:
	STK_ID					m_ID;
	wstring					m_wstrInsName;
	STK_ID					m_idPlacement;
	STK_MTX32				m_mtxPlacement;
	wstring					m_wstrFilePath;		//!< 对应的SVL文件路径
	STK_RGBA32				m_rgbaInstance;
	STK_BOOL				m_bHasColor;
	StkDisplayStateEnum		m_eDispStat;
	STK_BOOL				m_bHasMaterial;
	STK_MAT32				m_matInstance;

	STK_ID					m_idProto;			//!< 该Instance的ProtoType ID
	wstring					m_wstrProtoLink;
	Stk_DocumentManager*	m_DocManager;
	STK_INT32				m_nNumber;			//!< 特征编号
	wstring					m_wstrGUID;			//!< 实例GUID
	vector<Stk_MetaData*>	m_vcMetaDatas;		//!< 自定义属性
	wstring					m_wstrBOMID;		//!< 工艺BOM的序号
public:
	/****************************************************************************
	 *	@brief		Stk_Instance构造函数
	 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		浏览器中打开Instance时专用构造函数
	 *	@attention	无
	 ****************************************************************************/
	Stk_Instance(Stk_DocumentManager* i_stkDocManagerP);

	/****************************************************************************
	 *	@brief		Stk_Instance析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Instance(void);

	/****************************************************************************
	 *	@brief		获取实例ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID		实例ID号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID					GetID();

	/****************************************************************************
	 *	@brief		获取配置ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idPlacement		配置ID号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID					GetPlcID();

	/****************************************************************************
	 *	@brief		设置配置ID
	 *	@param[in]	i_idPlacement		配置ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetPlcID(STK_ID i_idPlacement);

	/****************************************************************************
	 *	@brief		设置ProtoType ID
	 *	@param[in]	i_idProto		ProtoType ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetProtoID(STK_ID i_idProto);

	/****************************************************************************
	 *	@brief		获取ProtoType ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idProto		ProtoType ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID					GetProtoID();

	/****************************************************************************
	 *	@brief		获取ProtoType
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		ProtoType对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*			GetProtoType();

	/****************************************************************************
	 *	@brief		获取实例名
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrInsName		实例名
	 *	@note		如果没有实例名，则返回ProtoType名
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetInsName();

	/****************************************************************************
	 *	@brief		设置实例名
	 *	@param[in]	i_wstrInsName		实例名
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetInsName(const wstring &i_wstrInsName);
	
	/****************************************************************************
	 *	@brief		获取配置ID，配置矩阵
	 *	@param[out]	o_idPlacement			配置ID
	 *	@param[out]	o_mtxPlcMatrix			配置矩阵
	 *	@retval		STK_SUCCESS				获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix);	// 获取配置信息
	
	/****************************************************************************
	 *	@brief		设置配置ID，配置矩阵
	 *	@param[in]	i_idPlacement			配置ID
	 *	@param[in]	i_mtxPlcMatrix			配置矩阵
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix);		// 设置配置信息
	
	/****************************************************************************
	 *	@brief		设置组件路径
	 *	@param[in]	i_wstrLink		组件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetProtoLink(const wstring &i_wstrLink);
	
	/****************************************************************************
	 *	@brief		获取组件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrProtoLink		组件路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetProtoLink();

	/****************************************************************************
	 *	@brief		实例是否有颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasColor		是否有颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				HasColor();
	
	/****************************************************************************
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
	void					SetColor(STK_FLOAT32 i_fRed, STK_FLOAT32 i_fGreen, STK_FLOAT32 i_fBlue, STK_FLOAT32 i_fAlpha);
	
	/****************************************************************************
	 *	@brief		获取实例颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaInstance	实例颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32				GetColor();

	/****************************************************************************
	 *	@brief		设置实例的显隐
	 *	@param[in]	i_eDispState	实例的显隐状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetDisplayState(StkDisplayStateEnum i_eDispState);
	
	/****************************************************************************
	 *	@brief		获取实例的显隐
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eDispStat	实例的显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkDisplayStateEnum		GetDisplayState();
	
	/****************************************************************************
	 *	@brief		设置实例材质
	 *	@param[in]	i_matInstance	实例材质
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void					SetMaterial(const STK_MAT32 &i_matInstance);
	
	/****************************************************************************
	 *	@brief		获取实例材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_matInstance	实例材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MAT32				GetMaterial();
	
	/****************************************************************************
	 *	@brief		判断是否有材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasMaterial	是否有材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				HasMaterial();

	/****************************************************************************
	 *	@brief		读SVL文件
	 *	@param[in]	i_FileP					文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				读取成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				ReadFile(Stk_File *i_FileP);
	
	/*****************************************************************************
	 *	@brief		写实例数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[in]	i_eSaveType			文件保存类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteFile(Stk_File *i_stkFileP, StkFileSaveTypeEnum i_eSaveType);
	
	/****************************************************************************
	 *	@brief		注册组件路径
	 *	@param[in]	i_wstrLink			组件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_SUCCESS_EXIST	已注册
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RegProtoLink(const wstring &i_wstrLink);
	
	/****************************************************************************
	 *	@brief		注册ID
	 *	@param[in]	i_idInstance	实例ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RegisterID(STK_ID i_idInstance);

	/****************************************************************************
	 *	@brief		获取LOD级数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		LOD等级数
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetLODCount();

	/****************************************************************************
	 *	@brief		获取SVL文件路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrFilePath		文件路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetFilePath();

	/****************************************************************************
	 *	@brief		设置SVL文件路径
	 *	@param[in]	i_wstrfilePath	文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFilePath(wstring i_wstrfilePath);

	/****************************************************************************
	 *	@brief		设置特征编号
	 *	@param[in]	i_nNumber			特征编号
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNumber(STK_INT32 i_nNumber);

	/****************************************************************************
	 *	@brief		获取特征编号
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		特征编号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_INT32				GetNumber();

	/****************************************************************************
	 *	@brief		设置实例标识GUID
	 *	@param[in]	i_wstrGUID			实例标识GUID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetGUID(wstring i_wstrGUID);

	/****************************************************************************
	 *	@brief		获取实例标识GUID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例标识GUID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetGUID();

	/*****************************************************************************
	 *	@brief		往当前实例中添加自定义属性数据
	 *	@param[in]	i_MetaDataP		自定义属性数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMetaData(Stk_MetaData* i_MetaDataP);
	
	/*****************************************************************************
	 *	@brief		获取当前实例中所有的自定义属性数据数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		自定义属性数据数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetMetaDataNum();
	
	/*****************************************************************************
	 *	@brief		获取当前实例中指定的自定义属性数据
	 *	@param[in]	i_nuIndex		自定义数据索引
	 *	@param[out]	无
	 *	@retval		指定的自定义属性数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MetaData*				GetMetaDataByIndex(STK_UINT32 i_nuIndex);

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

	/*****************************************************************************
	 *	@brief		设置工艺BOM的序号
	 *	@param[in]	i_wstrBOMID			工艺BOM的序号
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetVirtualBOMID(wstring i_wstrBOMID);

	/****************************************************************************
	 *	@brief		获取工艺BOM的序号
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		工艺BOM的序号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetVirtualBOMID();
};

#endif //_Stk_Instance_H_
