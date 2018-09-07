// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_PMIEntity.h
 *
 *	@brief	PMI类
 *
 *	@par	历史:
 *		2013/08/19	创建。WangY@HOTEAMSOFT
 *		2013/10/10	更新。Gaoqq@HOTEAMSOFT
 *					Curve与Leader等从属对应的改善
****************************************************************************/

#ifndef _STK_PMIENTITY_H_
#define _STK_PMIENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Enum.h"
//#include "Stk_File.h"
#include "Stk_Entity.h"
#include "Stk_CurveEntity.h"
#include "Stk_CompositeText.h"
#include "Stk_OutFrame.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_UserData.h"
#include "Stk_Picture.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_SpecialLine
	 *	@brief	特殊线类
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_SpecialLine
{
private:
	StkSpecLineUsageEnum		m_eLineUsage;		//!< 线用途
	vector<Stk_CurveEntity*>	m_vcCurves;			//!< 线条
public:
	/****************************************************************************
	 *	@brief		Stk_SpecialLine构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SpecialLine(void);

	/****************************************************************************
	 *	@brief		Stk_SpecialLine析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_SpecialLine(void);

	/****************************************************************************
	 *	@brief		获取特殊线用途
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eLineUsage	特殊线用途
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkSpecLineUsageEnum		GetSpecLinUse();

	/****************************************************************************
	 *	@brief		设置特殊线用途
	 *	@param[in]	i_eLineType	特殊线用途
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功	
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetSpecLinUse(StkSpecLineUsageEnum i_eLineType);

	/****************************************************************************
	 *	@brief		获取所有特殊线
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcCurves		所有特殊线
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_CurveEntity*>	GetCurves();

	/****************************************************************************
	 *	@brief		添加特殊线到特殊线数组中
	 *	@param[in]	i_CurveP		Curve数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@retval		STK_ERROR
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddCurve(Stk_CurveEntity* i_CurveP);

	/*****************************************************************************
	 *	@brief		写入特殊线数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteSpecialLine(StkSegmentHead *i_head, Stk_File *i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_Leader
	 *	@brief	引线类
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_Leader
{
private:
	STK_RGBA32					m_rgbaLeader;	//!< 颜色
	StkLeaderTypeEnum			m_eLeaderType;	//!< 类型：长度/角度
	StkTermTypeEnum				m_eTermType;	//!< 末端类型
	STK_FLOAT32					m_fTermSizeA[2];//!< 末端宽高,[0]->宽；[1]->高
	STK_PNT32					m_pntTermLoc;	//!< 末端起始位置
	STK_DIR32					m_dirTerm;		//!< 末端方向
	//Stk_CurveEntity*			m_CurveP;		//!< 线条
	vector<Stk_CurveEntity*>	m_vcCurves;		//!< 线条列
	STK_BOOL					m_bIsStub;		//!< 是否有二维折弯线
public:
	/****************************************************************************
	 *	@brief		Stk_Leader构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Leader(void);
	
	/****************************************************************************
	 *	@brief		Stk_Leader析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Leader(void);

	/****************************************************************************
	 *	@brief		获取引出线颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaLeader	引出线颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		设置引出线颜色
	 *	@param[in]	i_rgbaLeader	引出线颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetColor(STK_RGBA32 i_rgbaLeader);

	/****************************************************************************
	 *	@brief		获取引出线类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eLeaderType	引出线类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkLeaderTypeEnum			GetLeaderType();

	/****************************************************************************
	 *	@brief		设置引出线类型
	 *	@param[in]	i_eType			引出线类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetLeaderType(StkLeaderTypeEnum i_eType);

	/****************************************************************************
	 *	@brief		获取引出线末端类型、宽高、起始位置和方向
	 *	@param[in]	无
	 *	@param[out]	o_eTermType		末端类型
	 *	@param[out]	o_SizeA			末端宽高
	 *	@param[out]	o_pntLoc		末端起始位置
	 *	@param[out]	o_dirTerm		末端方向
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						GetTerminator(StkTermTypeEnum& o_eTermType, STK_FLOAT32 o_SizeA[2], STK_PNT32& o_pntLoc, STK_DIR32& o_dirTerm);
	
	/****************************************************************************
	 *	@brief		设置引出线末端类型、宽高、起始位置和方向
	 *	@param[in]	i_eTermType		末端类型
	 *	@param[in]	i_SizeA			末端宽高
	 *	@param[in]	i_pntLoc		末端起始位置
	 *	@param[in]	i_dirTerm		末端方向
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetTerminator(StkTermTypeEnum i_eTermType, STK_FLOAT32 i_SizeA[2], const STK_PNT32& i_pntLoc, const STK_DIR32& i_dirTerm);

	/****************************************************************************
	 *	@brief		获取引出线数组中最后一条线
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		最后一条引出线
	 *	@note		不再建议使用，请用GetAllCurve()替代
	 *	@attention	无
	 ****************************************************************************/
	Stk_CurveEntity*			GetCurve();

	/****************************************************************************
	 *	@brief		设置引出线数据
	 *	@param[in]	i_CurveP	Curve数据指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		STK_ERROR	设置失败
	 *	@note		不再建议使用，请使用AddCurve()替代
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCurve(Stk_CurveEntity* i_CurveP);

	/****************************************************************************
	 *	@brief		获取所有的引出线
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcCurves	所有引出线
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_CurveEntity*>	GetAllCurve();

	/****************************************************************************
	 *	@brief		添加引出线数据
	 *	@param[in]	i_CurveP	Curve数据指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	添加成功
	 *	@retval		STK_ERROR	添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddCurve(Stk_CurveEntity* i_CurveP);
	
	/*****************************************************************************
	 *	@brief		写入引出线数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteLeader(StkSegmentHead *i_head, Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		设置二维折弯线标识
	 *	@param[in]	i_bStub		二维折弯线标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		STK_ERROR	设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetStubDelta(STK_BOOL i_bStub);

	/****************************************************************************
	 *	@brief		是否是二维折弯线
	 *	@param[in]	i_bStub		二维折弯线标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		STK_ERROR	设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsStubDelta();
};

/**************************************************************************************
	 *	@class	Stk_Extensionline
	 *	@brief	延长线类
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_Extensionline
{
private:
	StkExtensTypeEnum			m_eExLineType;	//!< 延长线的类型
	Stk_CurveEntity*			m_CurveP;		//!< 线条
	STK_BOOL					m_bIsDisplay;	//!< 是否显示

public:
	/****************************************************************************
	 *	@brief		Stk_Extensionline构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Extensionline(void);

	/****************************************************************************
	 *	@brief		Stk_Extensionline析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Extensionline(void);

	/****************************************************************************
	 *	@brief		获取延长线的定义类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eExLineType	延长线的定义类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkExtensTypeEnum			GetExLineType();

	/****************************************************************************
	 *	@brief		设置延长线的定义类型
	 *	@param[in]	i_eType		延长线的定义类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		STK_SUCCESS	设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetExLineType(StkExtensTypeEnum i_nuType);

	/****************************************************************************
	 *	@brief		获取Curve数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_CurveP	Curve对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_CurveEntity*			GetCurve();

	/****************************************************************************
	 *	@brief		设置Curve数据
	 *	@param[in]	i_CurveP	Curve数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@retval		STK_ERROR	设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCurve(Stk_CurveEntity* i_CurveP);

	/*****************************************************************************
	 *	@brief		写入延长线数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteExLine(StkSegmentHead *i_head, Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		判断延长线显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsDisplay	显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsDisplay();

	/****************************************************************************
	 *	@brief		设置延长线显隐状态
	 *	@param[in]	i_bIsDisplay	显隐状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetDisplay(STK_BOOL i_bIsDisplay);
};

/**************************************************************************************
	 *	@class	Stk_PMI
	 *	@brief	PMI基类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_PMI :
	public Stk_Entity
{
private:
	Stk_Node*					m_NodeP;				//!< 所属Node
	STK_ID						m_idNode;				//!< 所属Node ID
	STK_BOOL					m_bIsDisplay;			//!< 显隐状态
	STK_BOOL					m_bIsAdjusted;			//!< 是否调整过
	StkPMITypeEnum				m_eType;				//!< PMI类型
	STK_MTX32					m_mtxDefPlane;			//!< 定义平面
	STK_RGBA32					m_rgbaPMI;				//!< 颜色
	vector<Stk_Leader*>			m_vcLeaders;			//!< 引出线
	vector<Stk_Extensionline*>	m_vcExLines;			//!< 延长线
	vector<Stk_ComText*>		m_vcComTexts;			//!< 复合文本
	vector<Stk_OutFrame*>		m_vcOutFrames;			//!< 外框
	vector<Stk_SpecialLine*>	m_vcSpecialLines;		//!< 特殊线列
	//vector<Stk_MetaData*>		m_vcMetaDatas;			//!< 自定义属性
	STK_UINT32					m_nuDim;				//!< 维度
	STK_BOOL					m_bIsParallelScreen;	//!< 该PMI的文字是否采用平行屏幕显示
	STK_BOOL					m_bIsFix;				//!< 该PMI的文字是固定位置
	StkPMIModeEnum				m_eMode;				//!< 存储模式
	wstring						m_strPMIName;			//!< PMI名称
	StkPMISourceEnum			m_ePMISource;			//!< PMI来源
	STK_DIR32					m_dirX;					//!< PMI的X方向向量
	STK_DIR32					m_dirY;					//!< PMI的Y方向向量
	STK_PNT32					m_pntSymBase;			//!< 符号基准点
	STK_BOOL					m_bIsOldVersion;		//!< 是否是V2.2之前的PMI
	
	map<wstring,map<STK_ID,Stk_UserData*>*>	m_mapUserDatas;	//!< 用户数据
	STK_SIZE_T					m_szUserDataCnt;			//!< 用户数据数量
	STK_BOOL					m_bIsFront;					//!< 是否最前端显示
	
	vector<Stk_Picture*>		m_vcPictures;			//!< 图片(图章)
public:
	/****************************************************************************
	 *	@brief		Stk_PMI构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PMI(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_PMI析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_PMI(void);

	/****************************************************************************
	 *	@brief		注册PMI ID
	 *	@param[in]	i_idPMI			PMI ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idPMI);

	/****************************************************************************
	 *	@brief		读取文件中的PMI数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_PMIHeadP			PMI记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_PMIHeadP);
	
	/*****************************************************************************
	 *	@brief		写PMI数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
	
	/****************************************************************************
	 *	@brief		获取PMI ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID			PMI ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();
	
	/****************************************************************************
	 *	@brief		设置PMI ID
	 *	@param[in]	i_ID			PMI ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);
	
	/****************************************************************************
	 *	@brief		获取组件 ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idProtoType	组件 ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetProtoTypeID();
	
	/****************************************************************************
	 *	@brief		获取PMI类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eType			PMI类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkPMITypeEnum				GetType();

	/****************************************************************************
	 *	@brief		设置PMI类型
	 *	@param[in]	i_eType			PMI类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetType(StkPMITypeEnum i_nuType);

	/****************************************************************************
	 *	@brief		设置PMI名称
	 *	@param[in]	i_strName		PMI名称
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		为保持Node与要素名称相同，在为要素设置名称时，同时设置Node名
	 *	@attention	无
	 ****************************************************************************/
	void						SetName(const wstring &i_strName);

	/****************************************************************************
	 *	@brief		获取PMI名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_strPMIName		PMI名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetName();

	/****************************************************************************
	 *	@brief		改变PMI名称
	 *	@param[in]	i_strName		PMI名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		保持从Node到要素方向的名字一致
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ChangeName(const wstring &i_strName);

	/****************************************************************************
	 *	@brief		获取PMI的维度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuDim		维度
	 *	@note		== 2 二维 <br>
	 *				== 3 三维
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetDim();

	/****************************************************************************
	 *	@brief		设置PMI的维度
	 *	@param[in]	i_nuDim			维度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		STK_ERROR		设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDim(STK_UINT32 i_nuDim);
	
	/****************************************************************************
	 *	@brief		获取定义平面
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_mtxDefPlane		定义平面
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const STK_MTX32&			GetDefPlane();

	/****************************************************************************
	 *	@brief		设置定义平面
	 *	@param[in]	i_mtxDefPlane		定义平面
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDefPlane(const STK_MTX32& i_mtxDefPlane);
	
	/****************************************************************************
	 *	@brief		获取PMI颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaPMI		PMI颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const STK_RGBA32&			GetColor();

	/****************************************************************************
	 *	@brief		设置PMI颜色
	 *	@param[in]	i_rgbaPMI		PMI颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetColor(const STK_RGBA32& i_rgbaPMI);

	/****************************************************************************
	 *	@brief		判断PMI显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsDisplay();

	/****************************************************************************
	 *	@brief		设置PMI显隐状态
	 *	@param[in]	i_bIsDisplay	显隐状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetDisplay(STK_BOOL i_bIsDisplay);

	/****************************************************************************
	 *	@brief		是否被调整过
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsAdjusted	调整状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsAdjusted();

	/****************************************************************************
	 *	@brief		设置调整标志
	 *	@param[in]	i_bIsAdjusted	调整状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetAdjusted(STK_BOOL i_bIsAdjusted);

	/****************************************************************************
	 *	@brief		判断PMI文字是否采用平行屏幕显示
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsParallelScreen	是否平行于屏幕
	 *	@note		== true 平行屏幕显示 <br>
	 *				== false 不平行屏幕显示
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsParallelScreen();
	
	/****************************************************************************
	 *	@brief		设置PMI文字是否采用平行屏幕显示
	 *	@param[in]	i_bIsParallelScreen	是否平行于屏幕
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		== true 平行屏幕显示 <br>
	 *				== false 不平行屏幕显示
	 *	@attention	无
	 ****************************************************************************/
	void						SetParallelScreen(STK_BOOL i_bIsParallelScreen);

	/****************************************************************************
	 *	@brief		判断PMI的文字是否固定位置
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsFix		是否固定
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsFix();

	/****************************************************************************
	 *	@brief		设置PMI的文字是否固定位置
	 *	@param[in]	i_bIsFix		是否固定
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetFix(STK_BOOL i_bIsFix);

	/****************************************************************************
	 *	@brief		获取PMI存储模式
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eMode	PMI存储模式
	 *	@note		== PMI_MODE_PARA 参数存储 <br>
	 *				== PMI_MODE_POLY 折线存储
	 *	@attention	无
	 ****************************************************************************/
	StkPMIModeEnum				GetMode();

	/****************************************************************************
	 *	@brief		设置PMI存储模式
	 *	@param[in]	i_eMode	PMI存储模式
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		== PMI_MODE_PARA 参数存储 <br>
	 *				== PMI_MODE_POLY 折线存储
	 *	@attention	无
	 ****************************************************************************/
	void						SetMode(StkPMIModeEnum i_eMode);

	/****************************************************************************
	 *	@brief		获取引出线数组数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcLeaders		引出线数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_Leader*>&	GetLeaders();

	/****************************************************************************
	 *	@brief		设置引出线数组数据
	 *	@param[in]	i_vcLeaders		引出线数组
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetLeaders(const vector<Stk_Leader*>& i_vcLeaders);
	
	/****************************************************************************
	 *	@brief		获取延长线数组数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcExLines		延长线数组
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_Extensionline*>& GetExLines();

	/****************************************************************************
	 *	@brief		设置延长线数组数据
	 *	@param[in]	i_vcExLines		延长线数组
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetExLines(const vector<Stk_Extensionline*>& i_vcExLines);

	/****************************************************************************
	 *	@brief		获取PMI中复合文本数组数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcComTexts	复合文本数组数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_ComText*>& GetComText();

	/****************************************************************************
	 *	@brief		往PMI中添加复合文本数据
	 *	@param[in]	i_ComTextP		复合文本
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@retval		STK_ERROR		添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddComText(Stk_ComText* i_ComTextP);

	/****************************************************************************
	 *	@brief		获取PMI中外框数组数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcOutFrames	外框数组数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_OutFrame*>& GetOutFrame();

	/****************************************************************************
	 *	@brief		PMI中设置外框数据
	 *	@param[in]	i_OutFrameP		指向外框的指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		STK_ERROR		设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetOutFrame(Stk_OutFrame* i_OutFrameP);

	/****************************************************************************
	 *	@brief		获取PMI中特殊线数组数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcSpecialLines	特殊线数组数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_SpecialLine*>& GetSpecialLines();

	/****************************************************************************
	 *	@brief		往PMI中添加特殊线数据
	 *	@param[in]	i_SpecLineP		特殊线数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@retval		STK_ERROR		添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddSpecialLine(Stk_SpecialLine* i_SpecLineP);

	/****************************************************************************
	 *	@brief		添加自定义属性
	 *	@param[in]	i_MetaDataP		自定义属性
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@retval		STK_ERROR		添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMetaData(Stk_MetaData* i_MetaDataP);

	/****************************************************************************
	 *	@brief		获取指定的自定义属性
	 *	@param[in]	i_nuIndex		自定义属性索引
	 *	@param[out]	无
	 *	@retval		指定的自定义属性
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MetaData*				GetMetaDataByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		获取当前节点自定义属性数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		自定义属性数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetMetaDataNum();

	/****************************************************************************
	 *	@brief		通过自定义属性名删除
	 *	@param[in]	i_wstrMetaDataTitle	自定义属性名
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败（没有找到）
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelMetaDataByTitle(wstring i_wstrMetaDataTitle);

	/****************************************************************************
	 *	@brief		设置PMI来源
	 *	@param[in]	i_ePMISource		PMI来源
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetPMISource(StkPMISourceEnum i_ePMISource);

	/****************************************************************************
	 *	@brief		获取PMI来源
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ePMISource	PMI来源
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkPMISourceEnum			GetPMISource();

	/****************************************************************************
	 *	@brief		设置PMI的X方向向量
	 *	@param[in]	i_dirX			X方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetAxisX(const STK_DIR32& i_dirX);

	/****************************************************************************
	 *	@brief		获取PMI的X方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		X方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32&					GetAxisX();

	/****************************************************************************
	 *	@brief		设置PMI的Y方向向量
	 *	@param[in]	i_dirY			Y方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetAxisY(const STK_DIR32& i_dirY);

	/****************************************************************************
	 *	@brief		获取PMI的Y方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Y方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32&					GetAxisY();

	/****************************************************************************
	 *	@brief		设置符号基准点位置
	 *	@param[in]	i_pntSymBase		符号基准点位置
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetSymBasePoint(const STK_PNT32& i_pntSymBase);

	/****************************************************************************
	 *	@brief		获取符号基准点位置
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		符号基准点位置
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32&					GetSymBasePoint();

	/****************************************************************************
	 *	@brief		是否是旧版本的PMI
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		新旧版本的PMI标志
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsOldVersion();

	/****************************************************************************
	 *	@brief		设置PMI新旧版本标志
	 *	@param[in]	i_bOldVersion		PMI新旧版本标志
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetOldVersion(STK_BOOL i_bOldVersion);

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
	 *	@brief		插入用户数据
	 *	@param[in]	i_stkUserDataP		用户数据指针
	 *	@retval		STK_SUCCESS			成功
	 *	@retval		其它				失败
	 *	@note		1.请new Stk_UserData对象并将用户数据流填入，然后使用Stk_PMI::AddUserData()插入
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

	/****************************************************************************
	 *	@brief		是否是最前端显示的PMI
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		最前端显示PMI标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsFront();

	/****************************************************************************
	 *	@brief		设置是否是最前端显示PMI的标识
	 *	@param[in]	i_bFront			是否是最前端显示PMI的标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetFront(STK_BOOL i_bFront);

	/****************************************************************************
	 *	@brief		添加图片(图章)
	 *	@param[in]	i_stkPictureP		图片(图章)
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddPicture(Stk_Picture* i_stkPictureP);

	/****************************************************************************
	 *	@brief		获取图片(图章)的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		图片(图章)的数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetPictureNum();

	/****************************************************************************
	 *	@brief		获取指定的图片(图章)
	 *	@param[in]	i_nuIndex			索引
	 *	@param[out]	无
	 *	@retval		指定的图片(图章)
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Picture*				GetPictureByIndex(STK_UINT32 i_nuIndex);
};

#endif