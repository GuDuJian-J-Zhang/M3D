// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		GroupItem.cpp 
 *
 *	@brief	分组成员数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#include "StdAfx.h"
#include "tinyxml/tinyxml2.h"
#include "GroupItem.h"
#include "GroupManager.h"
using namespace tinyxml2;

CGroupItem::CGroupItem(CGroup *pGroup,std::string strPath,std::string strPlcPath,AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetPos,AniQuat offsetQuat,AniPoint offsetScale,std::string strItemName)
: m_pGroup(pGroup)
, m_LocalPos(localPos)
, m_LocalQuat(localQuat)
, m_LocalScale(localScale)
, m_OffsetGroupPos(offsetPos)
, m_OffsetGroupQuat(offsetQuat)
, m_OffsetGroupScale(offsetScale)
, m_strPlcPath(strPlcPath)
{
	m_strPath = strPath;
	m_strItemName = strItemName;
}

CGroupItem::~CGroupItem(void)
{
}
void CGroupItem::UpdateItem(AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetPos,AniQuat offsetQuat,AniPoint offsetScale)
{
	m_LocalPos=localPos;
	m_LocalQuat=localQuat;
	m_LocalScale=localScale;
	m_OffsetGroupPos=offsetPos;
	m_OffsetGroupQuat=offsetQuat;
	m_OffsetGroupScale=offsetScale;
}
bool CGroupItem::IsParent(std::string strItemPath)
{
	std::string::size_type pos=m_strPath.find(strItemPath);
	if (pos!=std::string::npos)
	{
		std::string::size_type nLength=strItemPath.length();
		std::string::size_type nItemLength=m_strPath.length();
		if (pos+nLength==nItemLength)
		{
			return true;
		}
		if (pos+nLength<nItemLength)
		{
			char nextChar=m_strPath.at(pos+nLength);
			if (nextChar=='\\'||nextChar=='|')
			{
				return true;
			}
		}
	}
	return false;
}
bool CGroupItem::IsChild(std::string strItemPath)
{
	std::string::size_type pos=strItemPath.find(m_strPath);
	if (pos!=std::string::npos)
	{
		std::string::size_type nLength=m_strPath.length();
		std::string::size_type nItemLength=strItemPath.length();
		if (pos+nLength==nItemLength)
		{
			return true;
		}
		if (pos+nLength<nItemLength)
		{
			char nextChar=strItemPath.at(pos+nLength);
			if (nextChar=='\\'||nextChar=='|')
			{
				return true;
			}
		}
	}
	return false;
}
bool CGroupItem::IsChild(CGroupItem *pItem)
{
	if (pItem)
	{
		return IsChild(pItem->GetPath());
	}
	return false;
}
void CGroupItem::WriteToFile(XMLElement *pGroupItemEle)
{
	//写入xml文件
	if(!pGroupItemEle)
		return;
	//节点配置路径
	pGroupItemEle->SetAttribute("PLCPath", m_strPlcPath.c_str());
	SA_UTF8 utf8ItemName(m_strItemName.c_str());
	const utf8_char *cUTFItemName = utf8ItemName.encodedText();
	pGroupItemEle->SetAttribute("Name", cUTFItemName);

	char buffer[SA_BUFFER_SIZE_BIG];
	SACLOCALE(sprintf(buffer, "[%f %f %f]", m_OffsetGroupPos.x, m_OffsetGroupPos.y, m_OffsetGroupPos.z));
	pGroupItemEle->SetAttribute("OffsetGroupPos", buffer);
	SACLOCALE(sprintf(buffer, "[%f %f %f %f]", m_OffsetGroupQuat.x, m_OffsetGroupQuat.y, m_OffsetGroupQuat.z, m_OffsetGroupQuat.w));
	pGroupItemEle->SetAttribute("OffsetGroupQuat", buffer);
	if(!m_OffsetGroupScale.Equal(AniPoint(1.0f,1.0f,1.0f)))
	{
		SACLOCALE(sprintf(buffer, "[%f %f %f]", m_OffsetGroupScale.x, m_OffsetGroupScale.y, m_OffsetGroupScale.z));
		pGroupItemEle->SetAttribute("OffsetGroupScale", buffer);
	}

	SACLOCALE(sprintf(buffer, "[%f %f %f]", m_LocalPos.x, m_LocalPos.y, m_LocalPos.z));
	pGroupItemEle->SetAttribute("LocalPos", buffer);
	SACLOCALE(sprintf(buffer, "[%f %f %f %f]", m_LocalQuat.x, m_LocalQuat.y, m_LocalQuat.z, m_LocalQuat.w));
	pGroupItemEle->SetAttribute("LocalQuat", buffer);
	if(!m_LocalScale.Equal(AniPoint(1.0f,1.0f,1.0f)))
	{
		SACLOCALE(sprintf(buffer, "[%f %f %f]", m_LocalScale.x, m_LocalScale.y, m_LocalScale.z));
		pGroupItemEle->SetAttribute("LocalScale", buffer);
	}

}