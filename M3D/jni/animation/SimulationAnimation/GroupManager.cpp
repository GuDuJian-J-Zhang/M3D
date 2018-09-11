// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		GroupManager.cpp
 *
 *	@brief	分组管理器数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#include "stdafx.h"
#include "GroupManager.h"
#include "Group.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "tinyxml/tinyxml2.h"
#include "GroupItem.h"
#include <algorithm>

using namespace tinyxml2;

CGroupManager *CGroupManager::m_pInstance=NULL;
CGroupManager::CGroupManager()
:m_nNextGroupID(0)
,m_bSelecting(false)
{

}

CGroupManager::~CGroupManager(void)
{
	DeleteAllGroup();
}
CGroupManager* CGroupManager::GetInstance()
{
	if (!m_pInstance)
	{		
		m_pInstance=new CGroupManager();				
	}
	return m_pInstance;
}
void CGroupManager::DeleteAllGroup()
{
	m_vecSelGroupList.clear();
	GroupVector::iterator iter;
	for (iter=m_vecGroupList.begin();iter!=m_vecGroupList.end();++iter)
	{
		delete *iter;
	}
	m_vecGroupList.clear();
}
bool CGroupManager::DeleteGroup(CGroup *pGroup)
{
	SelectGroup(pGroup,false);
	GroupVector::iterator iter=std::find(m_vecGroupList.begin(),m_vecGroupList.end(),pGroup);
	if (iter==m_vecGroupList.end())
	{
		return false;
	}
	m_vecGroupList.erase(iter);
	delete pGroup;
	return true;
}


CGroup * CGroupManager::AddGroup(const int ID /*= -1*/, std::string strName/*=""*/)
{
	CGroup *pGroup=NULL;
	if(ID > 0 && (pGroup=GetGroupByID(ID)))
	{
		return NULL;
	}else
	{
		//创建组
		if(ID == -1)
		{
			pGroup = new CGroup(m_nNextGroupID,strName);
			++m_nNextGroupID;
		}
		else
		{
			pGroup = new CGroup(ID, strName);
			if(ID>=m_nNextGroupID)
			{
				m_nNextGroupID=ID+1;
			}
		}
		m_vecGroupList.push_back(pGroup);
	}
	return pGroup;
}


bool CGroupManager::RemoveSelectGroup(size_t nIndex)
{
	if (nIndex>=m_vecSelGroupList.size())
	{
		return false;
	}
	m_vecSelGroupList.erase(m_vecSelGroupList.begin()+nIndex);
	return true;
}
bool CGroupManager::SelectGroup(CGroup *pGroup,bool bSelected/*=true*/)
{
	GroupVector::iterator iter;
	if (bSelected)
	{
		if (std::find(m_vecSelGroupList.begin(),m_vecSelGroupList.end(),pGroup)==m_vecSelGroupList.end())
		{
			m_vecSelGroupList.push_back(pGroup);
		}else
		{
			return false;
		}
	}else
	{
		if ((iter=std::find(m_vecSelGroupList.begin(),m_vecSelGroupList.end(),pGroup))!=m_vecSelGroupList.end())
		{
			m_vecSelGroupList.erase(iter);
		}else
		{
			return false;
		}
	}
	return true;
}
bool CGroupManager::SelectedInGroup(std::string strPath)
{
	GroupVector::iterator iter=m_vecSelGroupList.begin();
	for (;iter!=m_vecSelGroupList.end();++iter)
	{
		CGroup *pGroup=*iter;
		if (!pGroup)
		{
			continue;
		}
		if (pGroup->IsInGroup(strPath))
		{
			return true;
		}
	}
	return false;
}

CGroup *CGroupManager::GetGroupByID(int nID)
{
	for (size_t i=0;i<m_vecGroupList.size();++i)
	{
		if (nID==m_vecGroupList.at(i)->GetID())
		{
			return m_vecGroupList.at(i);
		}
	}
	return NULL;
}
CGroup *CGroupManager::GetGroup(size_t nIndex)
{
	if (nIndex>=m_vecGroupList.size())
	{
		return NULL;
	}
	return m_vecGroupList.at(nIndex);
}
CGroup *CGroupManager::GetSelGroup(size_t nIndex)
{
	if (nIndex>=m_vecSelGroupList.size())
	{
		return NULL;
	}
	return m_vecSelGroupList.at(nIndex);
}
void CGroupManager::Init()
{
	m_nNextGroupID=0;
	DeleteAllGroup();
}
void CGroupManager::WriteToFile(const char *filename)
{
	//写入XML文件
	string strFilePath(filename);
	if(strFilePath.empty())
		return;
	XMLDocument* pDoc = new XMLDocument();
	
	//XMLDeclaration* pDecl = new XMLDeclaration("1.0","UTF-8","");
	//pDoc->LinkEndChild(pDecl);
	XMLElement* pGroupListEle = pDoc->NewElement("GroupList");
	pDoc->LinkEndChild(pGroupListEle);

	GroupVector::iterator iter=m_vecGroupList.begin();
	for (;iter!=m_vecGroupList.end();++iter)
	{
		CGroup *pGroup=*iter;
		if (!pGroup)
		{
			continue;
		}
		XMLElement* pGroupEle = pDoc->NewElement("Group");
		pGroupListEle->LinkEndChild(pGroupEle);
		//pGroup->WriteToFile(pGroupEle);
	}
	//由ANSI码转UTF-8
	XMLPrinter printer;
	pDoc->Accept(&printer);
	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
	FILE *fp;
	fp = fopen(strFilePath.c_str(), "w");
	if(fp)
	{
		int nSize = fwrite(UTF8BOM, 1, sizeof(UTF8BOM), fp);
		if(nSize == 3)
			fwrite(printer.CStr(), 1, strlen(printer.CStr()), fp);
		fclose(fp);
	}
	delete pDoc;
}

void CGroupManager::ReadFromFile(const wchar_t *filename)
{
	////在XML文件读取根零件
	//char szFileName[512];
	//WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)(filename), -1,
	//	szFileName, sizeof(szFileName), NULL, NULL);
	//ReadFromFile(szFileName);
}

void CGroupManager::ReadFromFile(const char*filename)
{
	//在XML文件读取根零件
	string strFilePath(filename);
	if (strFilePath.empty())
		return;
	Init();


	XMLDocument doc;
	XMLError ret = doc.LoadFile(strFilePath.c_str());
	if(ret == XML_SUCCESS)
	{
		ReadFromXMLDocument(doc);
	}
}

void CGroupManager::setGroupData(char* pData)
{
	if (!pData)
		return;
	XMLDocument XMLDoc;
	XMLDoc.Parse(pData, 0);
	ReadFromXMLDocument(XMLDoc);
}

void CGroupManager::ReadFromXMLDocument(XMLDocument& doc)
{
	XMLElement* pGroupListEle = doc.RootElement();
	if (!pGroupListEle)
		return;
	for (XMLElement* pGroupEle = pGroupListEle->FirstChildElement("Group");
		pGroupEle; pGroupEle = pGroupEle->NextSiblingElement("Group"))
	{
		//创建根节点
		const char* pszID = NULL;
		int id;
		pszID = pGroupEle->Attribute("ID");
		//ID
		if (pszID)
		{
			sscanf(pszID, "%d", &id);
		}
		else
		{
			id = 0;
		}
		const char* pszName = NULL;
		string strPath = "";
		//名称
		pszName = pGroupEle->Attribute("Name");

		CGroup *pGroup = AddGroup(id, pszName);
		if (pGroup)
		{
			for (XMLElement* pGroupItemEle = pGroupEle->FirstChildElement("GroupItem");
				pGroupItemEle; pGroupItemEle = pGroupItemEle->NextSiblingElement("GroupItem"))
			{
				const char* pszItemName = NULL, *pszItemPLCPath = NULL,
					*pszPos = NULL, *pszQuat = NULL, *pszScale = NULL;
				string strName = "";
				string strPath = "";
				string strPLCPath = "";
				AniPoint hOffsetGroupPos(0, 0, 0), hOffsetGroupScale(1, 1, 1), hLocalPos(0, 0, 0), hLocalScale(1, 1, 1);
				AniQuat hOffsetGroupQuat(0, 0, 0, 1), hLocalQuat(0, 0, 0, 1);

				//名称
				pszItemName = pGroupItemEle->Attribute("Name");
				if (pszItemName)
				{
					strName = (string)pszItemName;
				}

				pszItemPLCPath = pGroupItemEle->Attribute("PLCPath");
				if (pszItemPLCPath)
					strPLCPath = (string)pszItemPLCPath;

				//char newPlcIdPath[SA_BUFFER_SIZE_SMALL];
				//CAnimationAPI::ConvertSAPlcPathToDisPlcPath(pszPLCPath,newPlcIdPath);
				strPath = strPLCPath;//newPlcIdPath;
									 //位置
				pszPos = pGroupItemEle->Attribute("OffsetGroupPos");
				if (pszPos)
				{
					CUtilityXMLParser::GetFloatPoint(pszPos, hOffsetGroupPos);
				}
				//方向
				pszQuat = pGroupItemEle->Attribute("OffsetGroupQuat");
				if (pszQuat)
				{
					CSACommonAPI::GetQuatPoint(pszQuat, hOffsetGroupQuat);
				}
				//比例
				pszScale = pGroupItemEle->Attribute("OffsetGroupScale");
				if (pszScale)
				{
					CUtilityXMLParser::GetFloatPoint(pszScale, hOffsetGroupScale);
				}
				//位置
				pszPos = pGroupItemEle->Attribute("LocalPos");
				if (pszPos)
				{
					CUtilityXMLParser::GetFloatPoint(pszPos, hLocalPos);
				}
				//方向
				pszQuat = pGroupItemEle->Attribute("LocalQuat");
				if (pszQuat)
				{
					CSACommonAPI::GetQuatPoint(pszQuat, hLocalQuat);
				}
				//比例
				pszScale = pGroupItemEle->Attribute("LocalScale");
				if (pszScale)
				{
					CUtilityXMLParser::GetFloatPoint(pszScale, hLocalScale);
				}
				CGroupItem *pGroupItem = new CGroupItem(pGroup, strPath, strPLCPath, hLocalPos, hLocalQuat, hLocalScale, hOffsetGroupPos, hOffsetGroupQuat, hOffsetGroupScale, strName);
				pGroup->AddGroupItem(pGroupItem);

			}
		}
	}
}
