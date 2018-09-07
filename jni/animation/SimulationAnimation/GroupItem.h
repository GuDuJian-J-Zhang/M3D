// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		GroupItem.h 
 *
 *	@brief	分组成员数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#ifndef _GROUPITEM_H
#define _GROUPITEM_H

#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include <vector>
class CGroupManager;
class SoTempPath;
class CGroup;
namespace tinyxml2
{
	class XMLElement;
}

class SA_API CGroupItem
{
public:
	CGroupItem(CGroup *pGroup,std::string strPath,std::string strPlcPath,AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetGroupPos,AniQuat offsetGroupQuat,AniPoint offsetGroupScale,std::string strVirCompName);
	~CGroupItem(void);

	std::string GetPath(){ return m_strPath; }
 	std::string GetPlcPath(){ return m_strPlcPath; }
	std::string GetItemName(){ return m_strItemName; }
 	CGroup *GetGroup(){return m_pGroup;}
	void WriteToFile(tinyxml2::XMLElement *pGroupItemEle);
	//是否是当前节点的子节点
	bool IsChild(CGroupItem *pItem);
	bool IsChild(std::string strItemPath);
	//是否是当前节点的父节点
	bool IsParent(std::string strItemPath);
	void UpdateItem(AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetGroupPos,AniQuat offsetGroupQuat,AniPoint offsetGroupScale);
protected:
	/*****************************************************************
	//节点配置路径
	******************************************************************/
	std::string m_strPath;
 	std::string m_strPlcPath;
	/*****************************************************************
	//节点的名称
	******************************************************************/
	std::string m_strItemName;
	/*****************************************************************
	//所属分组
	******************************************************************/
 	CGroup *m_pGroup;
public:/*!<  The pos when add in group. */
	AniPoint				m_OffsetGroupPos; 				
	/*!<  The rotation  when add in group. */
	AniQuat				m_OffsetGroupQuat; 				
	/*!<  The scale  when add in group. */
	AniPoint				m_OffsetGroupScale;				
	/*!<  The local pos when add in group. */
	AniPoint				m_LocalPos; 					
	/*!<  The local rotation  when add in group. */
	AniQuat				m_LocalQuat; 					
	/*!<  The local scale  when add in group. */
	AniPoint				m_LocalScale;					

};
#endif
