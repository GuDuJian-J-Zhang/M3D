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
#include "GroupItem.h"
#include "GroupManager.h"


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
