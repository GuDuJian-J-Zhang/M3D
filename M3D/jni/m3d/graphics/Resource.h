/**@file
 *@brief	GL纹理贴图封装
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_RESOURCE_H_
#define M3D_RESOURCE_H_

#include "m3d/base/Object.h"

namespace M3D
{
class ResourceManager;

/**
 * @class
 * @brief 资源类
 */
class M3D_API Resource:public Object
{
public:
	Resource();
	virtual ~Resource();
	/**
	 *
	 * @param resourceMgr
	 */
	void SetResourceManager(ResourceManager* resourceMgr);
	ResourceManager* GetResourceManager() { return m_resourceMgr; }
	
	int GetSvlId() const { return m_svlId; }
	//用来设置SVLX 对象中对应的ID
	void SetSvlId(int val) { m_svlId = val; }
private:
	void Init();
protected:
	ResourceManager* m_resourceMgr;//!<设置资源管理

	int m_svlId;
};
}

#endif /*M3D_RESOURCE_H_*/
