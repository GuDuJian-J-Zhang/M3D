/****************************************************************************
All Rights Reserved. Copyright (C) 2017  山东山大华天软件有限公司

文件名：LoadFileManager.cpp
功能：
	加载文件的操作管理（读写操作）

更新记录：
	1、创建：zhangsm 2017/4/28
***********************************************************************/

#include "Stk_LoadInf.h"
#include "iostream"
#include "fstream"
#include "ChineseCode.h"
#include "RapidJsonTools.h"
#define CURRENT_VERSION					"2.0.2"


#define NODE_NAME_VERSION				"version"
#define NODE_NAME_INFO					"info"
#define NODE_NAME_LOADINF				"loader"

#define NODE_NAME_LOADHIDDEN			"loadHidden"
#define NODE_NAME_GROUPLIST				"groupList"
#define NODE_NAME_INITSTATE				"initState"
#define NODE_NAME_DEFAULTGROUP			"defaultGroup"

#define NODE_NAME_GROUP					"group"
#define NODE_NAME_GROUP_ID				"id"
#define NODE_NAME_GROUP_TYPE			"type"
#define NODE_NAME_GROUP_NAME			"name"
#define NODE_NAME_GROUP_PRIORITY		"priority"
#define NODE_NAME_GROUP_PRECISE			"precise"
#define NODE_NAME_GROUP_ITEMS			"groupItems"

#define NODE_NAME_CAMERA					"camera"
#define NODE_NAME_CAMERA_TYPE				"type"
#define NODE_NAME_CAMERA_FOCALDISTANCE		"focalDistance"
#define NODE_NAME_CAMERA_VIEWVOLUMESIZE		"vewVolumeSize"
#define NODE_NAME_CAMERA_POS				"pos"
#define NODE_NAME_CAMERA_QUAT				"quat"
#define NODE_NAME_ATTRIBUTES				"attributes"
#define NODE_NAME_ATTRIBUTE_BOUNDINGBOX		"boundingBox"
#define NODE_NAME_ATTRIBUTE_MODELCOUNT		"modelCount"
#define NODE_NAME_ATTRIBUTE_INSCOUNT		"instanceCount"
#define NODE_NAME_ATTRIBUTE_FACECOUNT		"faceCount"
#define NODE_NAME_ATTRIBUTE_PMICOUNT		"pmiCount"
#define NODE_NAME_ATTRIBUTE_VIEWCOUNT		"viewCount"
#define NODE_NAME_ATTRIBUTE_UNIT			"unit"
#define NODE_NAME_ATTRIBUTE_TRIANGLECOUNT	"triangleCount"
#define NODE_NAME_ATTRIBUTE_PREVIEWIMAGE	"previewImage"
#define NODE_NAME_ATTRIBUTE_FILESIZE		"fileSize"

namespace HoteamSoft {
	namespace SVLLib {
		Stk_LoadInf *Stk_LoadInf::m_pInstance = NULL;
		Stk_LoadInf::Stk_LoadInf()
		{
			m_iMaxGroupId = 0;
			m_strVersion = "";
			m_strPath = L"";
			m_iDefaultGroupID = 0;
			m_bLoadHidden = true;
			ClearModelGroup();
			// 	m_pIVOViewer = pIVOViewer;
			// 	m_pMainDoc = pMainDoc;

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m_fBoundingBox[i][j] = 0.0f;
				}
			}
			m_iModelCount = 0;
			m_iInstanceCount = 0;
			m_iTriangleCount = 0;
			m_iFaceCount = 0;
			m_iPMICount = 0;
			m_iViewCount = 0;
			m_iFileSize = 0;
			m_strUnit = "";
			m_strPreviewImage = "";
			m_converterTime = 0;
			m_strConverterVersion = "";
			m_strSourceFormat = "";
			m_strSourceVersion = "";
			m_linearTol = m_angularTol = 0.0;
		}

		Stk_LoadInf::~Stk_LoadInf(void)
		{
			ClearModelGroup();
		}

		Stk_LoadInf* Stk_LoadInf::GetInstance()
		{
			if (!m_pInstance)
			{
				m_pInstance = new Stk_LoadInf();
			}
			return m_pInstance;
		}

		int Stk_LoadInf::RegisterModelGroupId()
		{
			return m_iMaxGroupId++;
		}

		string Stk_LoadInf::ToJson()
		{
			string strRet = "";

			return strRet;
		}


		void Stk_LoadInf::FromJson(string strJSON)
		{
			
		}


		vector<Stk_ModelGroup*>& Stk_LoadInf::GetAllGroup()
		{
			return m_vecGroup;
		}

		void Stk_LoadInf::ClearModelGroup()
		{
			for (int i = 0; i < m_vecGroup.size(); i++)
			{
				Stk_ModelGroup* pGroup = m_vecGroup[i];
				if (pGroup == NULL)
					continue;
				delete pGroup;
			}
			m_vecGroup.clear();
			m_iMaxGroupId = 0;
		}

		bool Stk_LoadInf::AddModelGroup(Stk_ModelGroup* pModelGroup)
		{
			if (!pModelGroup)
				return false;
			pModelGroup->SetID(RegisterModelGroupId());
			m_vecGroup.push_back(pModelGroup);
			return true;
		}

		bool Stk_LoadInf::DeleteModelGroupById(int iModelGroupId)
		{
			bool bRet = false;
			vector<Stk_ModelGroup*>::iterator iter;
			for (iter = m_vecGroup.begin(); iter != m_vecGroup.end(); ++iter)
			{
				Stk_ModelGroup* pGroup = *iter;
				if (pGroup == NULL)
					continue;
				if (pGroup->GetID() == iModelGroupId)
				{
					m_vecGroup.erase(iter);
					delete *iter;
					bRet = true;
					break;
				}
			}

			return bRet;
		}

		Stk_ModelGroup* Stk_LoadInf::GetModelGroupById(int iModelGroupId)
		{
			Stk_ModelGroup* pModelGroup = NULL;
			vector<Stk_ModelGroup*>::iterator iter;
			for (iter = m_vecGroup.begin(); iter != m_vecGroup.end(); ++iter)
			{
				Stk_ModelGroup* pGroup = *iter;
				if (pGroup == NULL)
					continue;
				if (pGroup->GetID() == iModelGroupId)
				{
					pModelGroup = pGroup;
					break;
				}
			}
			return pModelGroup;
		}

		bool Stk_LoadInf::GetModelGroupByName(string strName)
		{
			Stk_ModelGroup* pModelGroup = NULL;
			vector<Stk_ModelGroup*>::iterator iter;
			for (iter = m_vecGroup.begin(); iter != m_vecGroup.end(); ++iter)
			{
				Stk_ModelGroup* pGroup = *iter;
				if (pGroup == NULL)
					continue;
				if (pGroup->GetName() == strName)
				{
					pModelGroup = pGroup;
					break;
				}
			}
			return pModelGroup;
		}

		bool Stk_LoadInf::SaveLoadInfFile(wstring strFilePath)
		{
			m_strPath = strFilePath;
			CRapidJsonTools::WriteLoadInf(strFilePath, this);
			return true;
		}

		bool Stk_LoadInf::OpenLoadInfFile(wstring& strFilePath)
		{
			bool bRet = true;
			m_strPath = strFilePath;
			ClearModelGroup();
			CRapidJsonTools::ReadLoadInf(strFilePath, this);
			return bRet;
		}

		void Stk_LoadInf::GetBoundingBox(float fBox[2][3])
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					fBox[i][j] = m_fBoundingBox[i][j];
				}
			}
		}

		void Stk_LoadInf::SetBoundingBox(float fBox[2][3])
		{
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m_fBoundingBox[i][j] = fBox[i][j];
				}
			}
		}
	}
}
