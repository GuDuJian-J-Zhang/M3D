/****************************************************************************
All Rights Reserved. Copyright (C) 2017  ɽ��ɽ����������޹�˾

�ļ�����Stk_LoadInf.h
���ܣ�
	�����ļ��Ĳ���������д������

���¼�¼��
	1��������likun 2017/7/18
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
			������	��RegisterModelGroupId
			����	������ģ�ͷ���ID
			����	��
			����ֵ	������ID
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			int RegisterModelGroupId();

			/*****************************************************************
			������	��GetAllGroup
			����	����ȡ�����б�
			����	��
			����ֵ	�������б�
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			vector<Stk_ModelGroup*>& GetAllGroup();


			/*****************************************************************
			������	��ClearModelGroup
			����	�����ģ�ͷ���
			����	��
			����ֵ	��
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			void ClearModelGroup();

			/*****************************************************************
			������	��AddGroup
			����	����ӷ���
			����	��pModelGroup ģ�ͷ���
			����ֵ	��bool
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool AddModelGroup(Stk_ModelGroup* pModelGroup);

			/*****************************************************************
			������	��DeleteModelGroupById
			����	��ɾ������
			����	��iModelGroupId ���
			����ֵ	��bool
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool DeleteModelGroupById(int iModelGroupId);

			/*****************************************************************
			������	��GetModelGroupById
			����	����ȡģ�ͷ���
			����	��iModelGroupId ���
			����ֵ	��ģ�ͷ���ָ��
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			Stk_ModelGroup* GetModelGroupById(int iModelGroupId);

			/*****************************************************************
			������	��GetModelGroupById
			����	����ȡģ�ͷ���
			����	��iModelGroupId ���
			����ֵ	��bool
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			bool GetModelGroupByName(string strName);

			/*****************************************************************
			������	��SaveLoadInfFile
			����	����������Ϣ���浽�ļ���
			����	��strPath ����·��
			����ֵ	��bool
			������ʷ��
					add by zhangsm 2017/4/28
			*****************************************************************/
			bool SaveLoadInfFile(wstring strFilePath);

			/*****************************************************************
			������	��ToJSONString
			����	����������Ϣ���л���JSON��ʽ���ַ���
			����	����
			����ֵ	��string
			������ʷ��
			add by zhangsm 2017/4/28
			*****************************************************************/
			string ToJson();

			/*****************************************************************
			������	��ToModelGroup
			����	����JSON��ʽ���ַ���ת��Ϊ������Ϣ
			����	��strJSON JSON��ʽ�ַ���
			����ֵ	��Stk_ModelGroup* ������Ϣ
			������ʷ��
					add by zhangsm 2017/4/28
			*****************************************************************/
			void FromJson(string strJSON);

			/*****************************************************************
			������	��OpenLoadInfFile
			����	�����ļ���Ϣ���ص��ڴ���
			����	��m_listGroupP �����б�
			����ֵ	��bool
			������ʷ��
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
			/* ת������ز���                                                                     */
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
			/* ��������                                                                     */
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
			//�ļ��İ汾
			string m_strVersion;
			//���������ļ�·��
			wstring m_strPath;
			//�����б�
			vector<Stk_ModelGroup*> m_vecGroup;
			//�Ƿ�������ؼ�
			int m_bLoadHidden;
			//��ʱ��Ĭ�Ϸ���
			int m_iDefaultGroupID;
			//Ĭ�������״̬
			Stk_CameraPtr	m_Camera;
			/************************************************************************/
			/* �ļ�������Ϣ                                                                     */
			/************************************************************************/
			//ģ�Ͱ�Χ�д�С
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
			/*ת���������Ϣ                                                                      */
			/************************************************************************/
			string m_strConverterVersion;		//ת�����汾
			string m_strConvertComputerName;	//ת���������
			string m_strConvertUserName;				//ת���û���
			string m_strSourceFormat;			//ԴCAD����
			string m_strSourceVersion;			//ԭCAD�汾
			string m_strConvertBeginTime;		//ת����ʼʱ��
			string m_strConvertEndTime;		//ת������ʱ��
			unsigned int m_converterTime;		//ת��ʱ��
			double m_linearTol;							//���߹���
			double m_angularTol;						//�Ƕȹ���
			string   m_strConvertConfig;					//ת�������ļ�����

			/************************************************************************/
			/* �������                                                                     */
			/************************************************************************/
			int m_backgroundType;
			int m_backgroundStyle;
			map<string, string> m_backgroundItems;
		};
	}
}
#endif
