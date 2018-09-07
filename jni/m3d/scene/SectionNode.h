/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef M3D_SECTION_NODE_H_
#define M3D_SECTION_NODE_H_

#include "m3d/model/IShape.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
class Section;
class SceneManager;
class M3D_API SectionNode: public SceneNode
{
public:
	SectionNode();
	virtual ~SectionNode();
	/**
	 * @see SceneNode
	 */
	virtual int GetType(void);

	virtual void OnMarkedDirty();

	/**
	 * @see SceneNode
	 */
	virtual int GetPickType();
 
	/**
	* @brief 获取存储剖面信息的节点  @see M3D::SectionNode
	* @return SectionNode节点指针
	*/
	Section * GetSection();

	/**
	 * @see SceneNode
	 */
	virtual void ComputeBox();

	/**
	 * @see SceneNode
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);
 
	M3D::SceneManager* GetScene() const { return m_scene; }
	void SetScene(M3D::SceneManager* val) { m_scene = val; }
protected:
	/**
	 * @see SceneNode
	 */
	virtual void RayPick(RayPickAction * action);

protected:
 
	Section* m_section;
	SceneManager* m_scene;
};
}
#endif // SHAPE_NODE_H
