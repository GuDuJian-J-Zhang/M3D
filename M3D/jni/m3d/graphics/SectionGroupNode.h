/**@file
 *@brief
 *@author	CDJ
 *@date		2016-6-24
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SECTIONGROUPNODE_H_
#define M3D_SECTIONGROUPNODE_H_

#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"

namespace M3D
{
class CameraNode;
class SceneManager;
/**
 * @class
 * @brief 盖面组类。用来临时保存盖面
 */
class M3D_API SectionGroupNode: public M3D::GroupNode
{
public:
	SectionGroupNode();
	virtual ~SectionGroupNode();
	/**
	 *
	 * @param scenemanager
	 */
	void SetScene(SceneManager* scenemanager);
	/**
	 *
	 * @return
	 */
	SceneManager* GetScene();
private:
	SceneManager* m_sceneManager;//!<场景管理
};

} /* namespace M3D */
#endif /* M3D_SECTIONGROUPNODE_H_ */
