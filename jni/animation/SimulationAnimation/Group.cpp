// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		Group.cpp
 *
 *	@brief	分组数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#include "StdAfx.h"
#include "tinyxml/tinyxml2.h"
#include "Group.h"
#include "GroupItem.h"
#include <algorithm>
#include "Resource.h"

using namespace tinyxml2;
CGroup::CGroup(const int ID,std::string strName)
:m_ID(ID)
{
	m_Pos.Set(0,0,0);
	m_Quat.Set(0.0,0.0,0.0,1.0);
	if(!strName.empty())
	{
		m_strName=strName;
	}
	else
	{
		char wStrName[512];
		sprintf(wStrName, "组%d",ID+1);
		m_strName=wStrName;
	}
}

CGroup::~CGroup(void)
{
	for (size_t i=0;i<m_vGroupItems.size();++i)
	{
		delete m_vGroupItems.at(i);
	}
	m_vGroupItems.clear();
}


bool CGroup::AddGroupItem(CGroupItem *pItem)
{
	if (!pItem)
	{
		return false;
	}
	std::string strPath=pItem->GetPath();
	for (size_t i=0;i<m_vGroupItems.size();++i)
	{
		//上层节点或相同节点已经在组中，无需添加
		if (m_vGroupItems.at(i)->IsChild(pItem))
		{
			return false;
		}
		//删除下层节点
// 		if(curPath.find(strPath)!=std::string::npos)
		if(pItem->IsChild(m_vGroupItems.at(i)))
		{
			DeleteGroupItem(i);
			--i;
		}
	}
	m_vGroupItems.push_back(pItem);
	return true;
}

CGroupItem *CGroup::GetItem(size_t nIndex)
{
	if (nIndex>=m_vGroupItems.size())
	{
		return NULL;
	}
	return m_vGroupItems.at(nIndex);
}

	/*****************************************************************
	* 删除Item，成功返回true
	******************************************************************/
bool CGroup::DeleteGroupItem(CGroupItem *pGroupItem)
{
	std::vector<CGroupItem *>::iterator iter=std::find(m_vGroupItems.begin(),m_vGroupItems.end(),pGroupItem);
	if (iter==m_vGroupItems.end())
	{
		return false;
	}
	m_vGroupItems.erase(iter);
	delete pGroupItem;
	return true;
}
bool CGroup::DeleteGroupItem(size_t nIndex)
{
	if (nIndex>=m_vGroupItems.size())
	{
		return false;
	}
	std::vector<CGroupItem *>::iterator iter=m_vGroupItems.begin()+nIndex;
	delete *iter;
	m_vGroupItems.erase(iter);
	return true;
}
bool CGroup::IsInGroup(std::string strPath)
{
	for (size_t i=0;i<m_vGroupItems.size();++i)
	{
		std::string curPath=m_vGroupItems.at(i)->GetPath();
		//上层节点或相同节点在组中
		//if (strPath.find(curPath)!=std::string::npos)
		if(m_vGroupItems.at(i)->IsChild(strPath)||m_vGroupItems.at(i)->IsParent(strPath))
		{
			return true;
		}

	}
	return false;
}
void CGroup::WriteToFile(XMLElement *pGroupEle)
{
	//写入xml文件
	if(!pGroupEle)
		return;
	pGroupEle->SetAttribute("ID",m_ID);
	//名称
 	string strGroupName;
	SA_UTF8 utf8Obj(m_strName.c_str());
	const utf8_char *cUTFBuffer =  utf8Obj.encodedText();
	pGroupEle->SetAttribute("Name", cUTFBuffer);

	std::vector<CGroupItem *>::iterator iter=m_vGroupItems.begin();
	for (;iter!=m_vGroupItems.end();++iter)
	{
		CGroupItem *pGroupItem=*iter;
		if (!pGroupItem)
		{
			continue;
		}
		XMLElement* pGroupItemEle = pGroupEle->GetDocument()->NewElement("GroupItem");
		pGroupEle->LinkEndChild(pGroupItemEle);
		pGroupItem->WriteToFile(pGroupItemEle);
	}
}