// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		GroupManager.h 
 *
 *	@brief	分组管理器数据结构
 *
 *	@par	历史:
 *		2016-04-08 Create by hhw
****************************************************************************/
#ifndef _GROUPMANAGER_H
#define _GROUPMANAGER_H

#include "../SimulationCommon/SATools.h"
#include <vector>
class CGroup;
namespace tinyxml2
{
	class XMLDocument;
}

typedef std::vector<CGroup*> GroupVector;

class  SA_API CGroupManager
{
private:
	CGroupManager();
public:
	~CGroupManager(void);

	static CGroupManager* GetInstance();

	/*!
	Add a Group to GrouManager.
  	*/
	CGroup * AddGroup(const int ID = -1, std::string strName = "");

	/*****************************************************************
	* 删除分组，成功返回true
	******************************************************************/
	bool DeleteGroup(CGroup *pGroup);
	/*****************************************************************
	* 删除所有分组
	******************************************************************/
	void DeleteAllGroup();
	/*****************************************************************
	*	@brief		写入文件
	*	@param[in] filename:文件名 
	********************************************************************/
	void WriteToFile(const wchar_t* filename);
	void WriteToFile(const char *filename);	
	/*****************************************************************
	*	@brief		读取文件
	*	@param[in] filename:文件名 
	********************************************************************/
	void ReadFromFile(const char*filename);
	void ReadFromFile(const wchar_t *filename);

	/*****************************************************************
	//得到分组数量
	********************************************************************/
	size_t GetGroupCount(){return m_vecGroupList.size();}
	/*****************************************************************
	//得到选中分组数量
	********************************************************************/
	size_t GetSelGroupCount(){return m_vecSelGroupList.size();}
	/*****************************************************************
	//选中或取消选中某分组
	********************************************************************/
	bool SelectGroup(CGroup *pGroup,bool bSelected=true);
	/*****************************************************************
	//将选中列表中第nIdex个分组移出选中列表
	********************************************************************/
	bool RemoveSelectGroup(size_t nIndex);
	/*****************************************************************
	//清空选中分组列表
	********************************************************************/
	void ClearSelGroup(){m_vecSelGroupList.clear();}

	/*****************************************************************
	//通过下标获取分组
	********************************************************************/
	CGroup *GetGroup(size_t nIndex);
	/*****************************************************************
	//通过下标获取所选分组
	********************************************************************/
	CGroup *GetSelGroup(size_t nIndex);

	/*****************************************************************
	//通过id获取分组
	********************************************************************/
	CGroup *GetGroupByID(int nID);
	
	/*****************************************************************
	//是否在被选中分组中
	********************************************************************/
	bool SelectedInGroup(std::string strPath);
	/*****************************************************************
	//初始化
	********************************************************************/
	void Init();	
	/*****************************************************************
	//判断是否正在选中分组
	********************************************************************/
	bool IsSelecting(){return m_bSelecting;}
	/*****************************************************************
	//设置是否正在选中分组
	********************************************************************/
	void SetSelecting(bool bSelecting){m_bSelecting=bSelecting;}
	/*****************************************************************//**
	*	@brief	  设置组结构给管理器
	*   @param[in] pData 热点数据
	*	@retval 无
	*   @Creator 17/09/18 qinyp
	********************************************************************/
	void setGroupData(char* pData);
		/*****************************************************************//**
	*	@brief	  读文件
	*   @param[in] doc xml文档对象
	*	@retval 无
	*   @Creator 17/09/18 qinyp
	********************************************************************/
	void ReadFromXMLDocument(tinyxml2::XMLDocument& doc);
protected:	
	/*Group list*/
	GroupVector				m_vecGroupList;		
	/*Select Group list*/
	GroupVector				m_vecSelGroupList;	
	/*!Next new group id*/
	int						m_nNextGroupID;		
	/*Instance*/
	static CGroupManager	*m_pInstance;		
	bool					m_bSelecting;
};
#endif
