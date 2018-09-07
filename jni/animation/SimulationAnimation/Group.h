// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		Group.h 
 *
 *	@brief	分组数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#ifndef _GROUP_H
#define _GROUP_H

#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include <vector>
#define GROUP_NAME_LEN 64
#define GROUP_DESC_LEN 6128
class CGroupManager;
class CGroupItem;
namespace tinyxml2
{
	class XMLElement;
}

class SA_API CGroup
{
public:
	CGroup(const int ID,std::string strName="");
	~CGroup(void);
	/*****************************************************************
	/*!	This method get CGroup ID.  	
	******************************************************************/
	int		GetID() {return m_ID;}
	/*****************************************************************
	/*! Return Name of the Group. 
	******************************************************************/
	std::string GetName(){return m_strName;};
	/*****************************************************************
	/*! Sets the Name of the Group. 
	******************************************************************/
	void SetName(std::string strName){m_strName=strName;}
	/*****************************************************************
	/*! Add group item. 
	******************************************************************/
	bool AddGroupItem(CGroupItem *pItem);
	/*****************************************************************
	/*! Get group item count.  
	******************************************************************/
	size_t GetItemCount(){return m_vGroupItems.size();}
	/*****************************************************************
	/*! Get group item. 
	******************************************************************/
	CGroupItem *GetItem(size_t nIndex);
	/*****************************************************************
	* 删除Item，成功返回true
	******************************************************************/
	bool DeleteGroupItem(CGroupItem *pGroupItem);
	bool DeleteGroupItem(size_t nIndex);
	/*****************************************************************
	//是否在分组中
	********************************************************************/
	bool IsInGroup(std::string strPath);
	/*****************************************************************
	//写入文件
	********************************************************************/
	void WriteToFile(tinyxml2::XMLElement *pGroupEle);
	/*****************************************************************
	//得到位置
	********************************************************************/
	AniPoint GetPos(){return m_Pos;}
	/*****************************************************************
	//设置位置
	********************************************************************/
	void SetPos(AniPoint pos){ m_Pos=pos;}
	/*****************************************************************
	//得到方向
	********************************************************************/
	AniQuat GetRotation(){return m_Quat;}
	/*****************************************************************
	//设置方向
	********************************************************************/
	void SetRotation(AniQuat quat){m_Quat=quat;}
protected:
	// ID of this CGroup.
	int					m_ID;						
	std::string		m_strName;
	// list of group items.
	std::vector<CGroupItem *> m_vGroupItems;		
	// The pos of group. 
	AniPoint				m_Pos; 					
	// The rotation of group.
 	AniQuat				m_Quat; 				
};
#endif
