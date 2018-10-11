/****************************************************************************
All Rights Reserved. Copyright (C) 2017  山东山大华天软件有限公司

文件名：Stk_LoadInf.h
功能：
	加载文件的操作管理（读写操作）

更新记录：
	1、创建：likun 2017/7/18
***********************************************************************/

#ifndef _SVLLIB_LOADINF_H_
#define _SVLLIB_LOADINF_H_

#pragma once
#include "Stk_Object.h"

#include "Stk_ModelGroup.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_LoadInf {
		public:
			Stk_LoadInf();
			~Stk_LoadInf(void);

			static Stk_LoadInf* GetInstance();

			/*****************************************************************
			函数名	：RegisterModelGroupId
			功能	：生成模型分组ID
			参数	：
			返回值	：分组ID
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			int RegisterModelGroupId();

			/*****************************************************************
			函数名	：GetAllGroup
			功能	：获取分组列表
			参数	：
			返回值	：分组列表
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			vector<Stk_ModelGroup*>& GetAllGroup();


			/*****************************************************************
			函数名	：ClearModelGroup
			功能	：清空模型分组
			参数	：
			返回值	：
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			void ClearModelGroup();

			/*****************************************************************
			函数名	：AddGroup
			功能	：添加分组
			参数	：pModelGroup 模型分组
			返回值	：bool
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool AddModelGroup(Stk_ModelGroup* pModelGroup);

			/*****************************************************************
			函数名	：DeleteModelGroupById
			功能	：删除分组
			参数	：iModelGroupId 编号
			返回值	：bool
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool DeleteModelGroupById(int iModelGroupId);

			/*****************************************************************
			函数名	：GetModelGroupById
			功能	：获取模型分组
			参数	：iModelGroupId 编号
			返回值	：模型分组指针
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			Stk_ModelGroup* GetModelGroupById(int iModelGroupId);

			/*****************************************************************
			函数名	：GetModelGroupById
			功能	：获取模型分组
			参数	：iModelGroupId 编号
			返回值	：bool
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool GetModelGroupByName(string strName);

			/*****************************************************************
			函数名	：SaveLoadInfFile
			功能	：将加载信息保存到文件中
			参数	：strPath 保存路径
			返回值	：bool
			更新历史：
					add by zhangsm 2017/4/28
			*****************************************************************/
			bool SaveLoadInfFile(wstring strFilePath);

			/*****************************************************************
			函数名	：ToJSONString
			功能	：将加载信息序列化成JSON格式的字符串
			参数	：无
			返回值	：string
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			string ToJson();

			/*****************************************************************
			函数名	：ToModelGroup
			功能	：将JSON格式的字符串转换为分组信息
			参数	：strJSON JSON格式字符串
			返回值	：Stk_ModelGroup* 分组信息
			更新历史：
					add by zhangsm 2017/4/28
			*****************************************************************/
			void FromJson(string strJSON);

			/*****************************************************************
			函数名	：OpenLoadInfFile
			功能	：将文件信息加载到内存中
			参数	：m_listGroupP 分组列表
			返回值	：bool
			更新历史：
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool OpenLoadInfFile(wstring& strFilePath);

			string GetVersion() { return m_strVersion; }
			void SetVersion(string strVersion) { m_strVersion = strVersion; }

			void GetBoundingBox(float fBox[2][3]);
			void SetBoundingBox(float fBox[2][3]);

			int GetModelCount() { return m_iModelCount; }
			void SetModelCount(int iModeCount) { m_iModelCount = iModeCount; }

			int GetInstanceCount() { return m_iInstanceCount; }
			void SetInstanceCount(int iInstanceCount) { m_iInstanceCount = iInstanceCount; }

			unsigned int getMeshCount() {return m_meshCount;}
			void setMeshCount(unsigned int i_meshcount) { m_meshCount = i_meshcount;}

			unsigned int getMeshEdgeCount() {return m_meshEdgeCount;}
			void setMeshEdgeCount(unsigned int i_meshEdgecount) { m_meshEdgeCount = i_meshEdgecount;}
			
			int GetFaceCount() { return m_iFaceCount; }
			void SetFaceCount(int iFaceCount) { m_iFaceCount = iFaceCount; }

			int GetTriangleCount() { return m_iTriangleCount; }
			void SetTriangleCount(int iTriangleCount) { m_iTriangleCount = iTriangleCount; }

			int GetPMICount() { return m_iPMICount; }
			void SetPMICount(int iPMICount) { m_iPMICount = iPMICount; }

			int GetViewCount() { return m_iViewCount; }
			void SetViewCount(int iViewCount) { m_iViewCount = iViewCount; }

			string GetUnit() { return m_strUnit; }
			void SetUnit(string& strUnit) { m_strUnit = strUnit; }

			string GetPreviewImage() { return m_strPreviewImage; }
			void SetPreviewImage(string& strPreviewImage) { m_strPreviewImage = strPreviewImage; }

			int GetFileSize() { return m_iFileSize; }
			void SetFileSize(int iFileSize) { m_iFileSize = iFileSize; }

			unsigned int getMeshDataSize() { return m_meshDataSize; }
			void setMeshDataSize(unsigned int i_meshDataSize) { m_meshDataSize = i_meshDataSize; }

			int IsLoadHidden() { return m_bLoadHidden; }
			void SetIsLoadHidden(int iLoadHidden) { m_bLoadHidden = iLoadHidden; }

			int GetDefaultGroupID() { return m_iDefaultGroupID; }
			void SetDefaultGroupID(int iDefaultGroupID) { m_iDefaultGroupID = iDefaultGroupID; }

			Stk_CameraPtr GetCamera() { return m_Camera; }
			void SetCamera(Stk_CameraPtr camera) { m_Camera = camera; }
			/************************************************************************/
			/* 转换器相关参数                                                                     */
			/************************************************************************/
			string getConverterVersion(){return m_strConverterVersion;}
			void	  setConverterVersion(string i_strConverterVersion){m_strConverterVersion = i_strConverterVersion;}

			string getConvertComputerName(){return m_strConvertComputerName;}
			void	  setConvertComputerName(string i_strConvertComputerName){m_strConvertComputerName = i_strConvertComputerName;}

			string getConvertUserName(){return m_strConvertUserName;}
			void	  setConvertUserName(string i_istrConvertUserName){m_strConvertUserName = i_istrConvertUserName;}

			string getSourceFormat(){return m_strSourceFormat;}
			void	  setSourceFormat(string i_strSourceFormat){m_strSourceFormat = i_strSourceFormat;}

			string getSourceVersion(){return m_strSourceVersion;}
			void   setSourceVersion(string i_strSourceVersion){m_strSourceVersion = i_strSourceVersion;}

			string getConvertBeginTime(){return m_strConvertBeginTime;}
			void	  setConvertBeginTime(string i_strConvertBeginTime){m_strConvertBeginTime = i_strConvertBeginTime;}

			string getConvertEndTime(){return m_strConvertEndTime;}
			void	  setConvertEndTime(string i_strConvertEndTime){m_strConvertEndTime = i_strConvertEndTime;}

			double	getLinearTol(){return m_linearTol;}
			void      setLinearTol(double i_linearTol){m_linearTol = i_linearTol;}

			double getAngularTol(){return m_angularTol;}
			void		setAngularTol(double i_angularTol){m_angularTol = i_angularTol;}

			unsigned int getConverterTime(){return m_converterTime;}
			void setConverterTime(unsigned int i_converterTime){m_converterTime = i_converterTime;}

			string	getConvertConfig(){return m_strConvertConfig;}
			void		setConvertConfig(string i_strConvertConfig){ m_strConvertConfig = i_strConvertConfig;}

			/************************************************************************/
			/* 背景参数                                                                     */
			/************************************************************************/
			int GetBackgroundType() { return	this->m_backgroundType; }
			void SetBackgroundType(int type) { this->m_backgroundType = type; }

			int GetBackgroundStyle() { return this->m_backgroundStyle; }
			void SetBackGroundStyle(int style) { this->m_backgroundStyle = style; }

			map<string, string>& GetBackgroundItems() { return this->m_backgroundItems; }
			void SetBackgroundItem(string key, string value) {
				m_backgroundItems[key] = value;
			}

		protected:
			static Stk_LoadInf* m_pInstance;
			int m_iMaxGroupId;
			//文件的版本
			string m_strVersion;
			//加载配置文件路径
			wstring m_strPath;
			//分组列表
			vector<Stk_ModelGroup*> m_vecGroup;
			//是否加载隐藏件
			int m_bLoadHidden;
			//打开时的默认分组
			int m_iDefaultGroupID;
			//默认摄像机状态
			Stk_CameraPtr	m_Camera;
			/************************************************************************/
			/* 文件基本信息                                                                     */
			/************************************************************************/
			//模型包围盒大小
			float m_fBoundingBox[2][3];
			int m_iModelCount;
			int m_iInstanceCount;
			int m_iFaceCount;
			unsigned int m_meshEdgeCount;
			unsigned int m_meshCount;
			int m_iTriangleCount;
			int m_iPMICount;
			int m_iViewCount;
			string m_strUnit;
			string m_strPreviewImage;
			int m_iFileSize;
			unsigned int m_meshDataSize;
			/************************************************************************/
			/*转换器相关信息                                                                      */
			/************************************************************************/
			string m_strConverterVersion;		//转换器版本
			string m_strConvertComputerName;	//转换计算机名
			string m_strConvertUserName;				//转换用户名
			string m_strSourceFormat;			//源CAD类型
			string m_strSourceVersion;			//原CAD版本
			string m_strConvertBeginTime;		//转换开始时间
			string m_strConvertEndTime;		//转换结束时间
			unsigned int m_converterTime;		//转换时间
			double m_linearTol;							//折线公差
			double m_angularTol;						//角度公差
			string   m_strConvertConfig;					//转换参数文件内容

			/************************************************************************/
			/* 背景相关                                                                     */
			/************************************************************************/
			int m_backgroundType;
			int m_backgroundStyle;
			map<string, string> m_backgroundItems;
		};
	}
}
#endif
