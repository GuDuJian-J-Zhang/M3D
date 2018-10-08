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

private:
	void Init();
protected:
	ResourceManager* m_resourceMgr;//!<设置资源管理
};
}

#endif /*M3D_RESOURCE_H_*/
