// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_TopoEntity.h
 *
 *	@brief	Body类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_TOPOENTITY_H_
#define _STK_TOPOENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_MeshEntity.h"
#include "Stk_CurveEntity.h"
#include "Stk_LineSetEntity.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_SpacialPoint.h"
#include "Stk_Entity.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_Body
	 *	@brief	Body组件类
	 *
	 *	拓扑结构。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_Body :
	public Stk_Entity
{
private:
	Stk_Node*					m_NodeP;			//!< 所属Node
	vector<Stk_Mesh_Com*>		m_vcMeshComs;
	STK_BOOL					m_bHasColor;		//!< 是否有颜色
	STK_RGBA32					m_rgbaBody;			//!< RGBA颜色
	STK_BOOL					m_bDispStat;		//!< 是否隐藏
	STK_BOOL					m_bHasMaterial;		//!< 是否有材质
	STK_MAT32					m_matBody;			//!< 材质
	vector<Stk_CurveEntity*>	m_vcSubLines;		//!< 辅助线
	vector<Stk_LineSet*>		m_vcLineSets;		//!< 线集
	//vector<Stk_MetaData*>		m_vcMetaDatas;		//!< 自定义属性
	wstring						m_strBodyName;		//!< Body名称
	vector<Stk_SpacialPointEntity*>	m_vcSpacialPoints;	//!< 空间点集
public:
	/****************************************************************************
	 *	@brief		Stk_Body构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Body(Stk_ProtoType* i_stkProtoTypeP);
	
	/****************************************************************************
	 *	@brief		Stk_Body析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Body(void);

	/****************************************************************************
	 *	@brief		往Body中添加Mesh数据
	 *	@param[in]	i_MeshComP		Mesh类对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMeshCom(Stk_Mesh_Com* i_MeshComP);
	
	/****************************************************************************
	 *	@brief		获取Body中Mesh的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		szNum		Mesh数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetMeshComNum();
	
	/****************************************************************************
	 *	@brief		获取Body中指定的Mesh
	 *	@param[in]	i_nuKeyPose		指定的Mesh索引
	 *	@param[out]	无
	 *	@retval		Mesh对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Com*				GetMeshCom(STK_UINT32 i_nuKeyPose);

	/****************************************************************************
	 *	@brief		通过ID删除指定的MeshCom
	 *	@param[in]	i_idMeshCom		指定的MeshCom ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		其他			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteMeshComByID(STK_ID i_idMeshCom);

	/****************************************************************************
	 *	@brief		删除Body中所有的MeshCom
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		其他			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteAllMeshComs();

	/****************************************************************************
	 *	@brief		注册Body ID
	 *	@param[in]	i_idBody		Body ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idBody);
	
	/****************************************************************************
	 *	@brief		获取Body ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID		Body ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();
	
	/*****************************************************************************
	 *	@brief		设置Body ID
	 *	@param[in]	i_ID			Body ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);
	
	/****************************************************************************
	 *	@brief		获取当前Body所属的ProtoType的ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idProtoType	ProtoType的ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetProtoTypeID();
	
	/****************************************************************************
	 *	@brief		设置Body颜色
	 *	@param[in]	i_rgbaBody		Body颜色
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetColor(const STK_RGBA32 &i_rgbaBody);
	
	/****************************************************************************
	 *	@brief		获取Body颜色
	 *	@param[in]	无		
	 *	@param[out]	无
	 *	@retval		m_rgbaBody		Body颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();
	
	/****************************************************************************
	 *	@brief		判断Body是否有颜色
	 *	@param[in]	无		
	 *	@param[out]	无
	 *	@retval		m_bHasColor		是否有Body颜色标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasColor();

	/****************************************************************************
	 *	@brief		设置Body材质
	 *	@param[in]	i_matBody		Body材质	
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetMaterial(const STK_MAT32 &i_matBody);

	/****************************************************************************
	 *	@brief		获取Body材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_matBody		Body材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MAT32					GetMaterial();
	
	/****************************************************************************
	 *	@brief		判断是否有Body材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasMaterial	是否有Body材质的标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasMaterial();

	/****************************************************************************
	 *	@brief		设置Body显隐状态
	 *	@param[in]	i_bDisplayState	Body显隐状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetDisplayState(STK_BOOL i_bDisplayState);
	
	/****************************************************************************
	 *	@brief		获取Body显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bDispStat		Body显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					GetDisplayState();
	
	/*****************************************************************************
	 *	@brief		添加辅助线
	 *	@param[in]	i_stkSubLine		辅助线
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@retval		STK_ERROR			添加失败
	 *	@note		已废弃由Linset取代
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddSubLine(Stk_CurveEntity* i_stkSubLine);
	
	/*****************************************************************************
	 *	@brief		获取所有辅助线
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		已废弃由Linset取代
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_CurveEntity*>&	GetAllSubLines();
	
	/*****************************************************************************
	 *	@brief		读取所有辅助线
	 *	@param[in]	i_FileP				文件指针
	 *	@param[in]	i_SubLineHeadP		辅助线记录头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		已废弃由Linset取代
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSubLines(Stk_File* i_FileP, StkSegmentHead* i_SubLineHeadP);
	
	/*****************************************************************************
	 *	@brief		将所有辅助线写入SVL
	 *	@param[in]	i_stkFileP			File对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		已废弃由Linset取代
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSubLine(Stk_File *i_stkFileP);
	

	/*****************************************************************************
	 *	@brief		添加线集
	 *	@param[in]	i_stkLineSet		线集
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@retval		STK_ERROR			添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddLineSet(Stk_LineSet* i_stkLineSet);
	
	/*****************************************************************************
	 *	@brief		通过索引删除线集
	 *	@param[in]	i_nuIndex			索引
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelLinSetByIndex(STK_UINT32 i_nuIndex);

	/*****************************************************************************
	 *	@brief		获取所有线集
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			获取成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_LineSet*>&		GetAllLineSets();

	/*****************************************************************************
	 *	@brief		读取所有线集
	 *	@param[in]	i_FileP				文件指针
	 *	@param[in]	i_LineSetHeadP		线集段记录头
	 *	@param[out]	无
	 *	@retval		stat				读取所有线集状态
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadLineSets(Stk_File* i_FileP, StkSegmentHead* i_LineSetHeadP);
	
	/****************************************************************************
	 *	@brief		将所有线集写入SVL
	 *	@param[in]	i_stkFileP		File对象指针
	 *	@param[out]	无
	 *	@retval		stat			将所有线集写入SVL后的返回状态
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteLineSets(Stk_File *i_stkFileP);

	//自定义属性
	//STK_STATUS				AddMetaData(Stk_MetaData* i_MetaDataP);
	//const vector<Stk_MetaData*>& GetAllMetaDatas();
	//STK_SIZE_T				GetMetaDataNum();
	//STK_STATUS				ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP);
	//STK_STATUS				WriteMetaData(Stk_File* i_stkFileP);
	

	/****************************************************************************
	 *	@brief		设置Body名称
	 *	@param[in]	i_strName		Body名称
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
	 *	@attention	无
	 ****************************************************************************/
	void						SetName(const wstring &i_strName);
	
	/****************************************************************************
	 *	@brief		获取Body名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_strBodyName		Body名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetName();

	/****************************************************************************
	 *	@brief		改变Body名称
	 *	@param[in]	i_strName		Body名称
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		保持从Node到要素方向的名字一致
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ChangeName(const wstring &i_strName);

	/*****************************************************************************
	 *	@brief		向当前Body中添加空间点
	 *	@param[in]	i_stkSpacialPoint		空间点数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddSpacialPoints(Stk_SpacialPointEntity* i_stkSpacialPoint);
	
	/*****************************************************************************
	 *	@brief		获取当前Body中空间点的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		空间点的数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetSpacialPointNum();
	
	/*****************************************************************************
	 *	@brief		通过指定索引获取当前Body中指定的空间点
	 *	@param[in]	i_nuIndex				指定索引
	 *	@param[out]	无
	 *	@retval		指定的空间点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SpacialPointEntity*		GetSpacialPointByIndex(STK_UINT32 i_nuIndex);

	/*****************************************************************************
	 *	@brief		从SVL文件中读取空间点数据
	 *	@param[in]	i_FileP					SVL文件指针
	 *	@param[in]	i_SpacialPointHeadP		空间点记录头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				读取成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadSpacialPoints(Stk_File* i_FileP, StkSegmentHead* i_SpacialPointHeadP);
	
	/*****************************************************************************
	 *	@brief		向SVL文件中填写空间点数据
	 *	@param[in]	i_stkFileP		File对象指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSpacialPoints(Stk_File *i_stkFileP);

	/*****************************************************************************
	 *	@brief		设置所对应的Node
	 *	@param[in]	i_NodeP				所对应的Node
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetNode(Stk_Node* i_NodeP);

	/*****************************************************************************
	 *	@brief		获取所对应的Node
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所对应的Node
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Node*					GetNode();

	/****************************************************************************
	 *	@brief		读取文件中的Body数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_BodyHeadP			Body记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR			读取失败
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_BodyHeadP);
	
	/*****************************************************************************
	 *	@brief		写Body数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);

};

#endif