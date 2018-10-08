/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef  M3D_SECTION_NODE_H
#define  M3D_SECTION_NODE_H

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/graphics/SectionPlane.h"

namespace M3D
{


class ImageBoard;
/**
 * @class
 * @brief 提供剖面列表的绘制功能（可同时绘制多个剖面），并可挂接到场景树中。
 */
class M3D_API Section: public Shape
{
public:
	Section(void);
	virtual ~Section(void);

	/**
	 *
	 * @return
	 */
	virtual SHAPETYPE GetType(void);

	/**
	 * 获取平面列表
	 * @return
	 */
	list<SectionPlane*>* GetPlaneList();

	/**
	 *
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

//	void SetModel(Model* model);

	/**
	 * 添加平面
	 * @param plane
	 * @return
	 */
	bool AddPlane(SectionPlane* plane);

	/**
	 * 移除平面
	 * @param plane
	 * @return
	 */
	bool RemovePlane(SectionPlane* plane);

	/**
	 * 清除平面
	 */
	void ClearPlanes();

	/**
	 *
	 * @param box
	 */
	void SetDrawRection(const BoundingBox& box);

	void SetIsShowCappingPlane(bool state);

	bool IsShowCappingPlane();

private:
//	Model* m_Model;

	list<SectionPlane*> m_planeList;//!<

	BoundingBox m_drawBoxSize;//!<

	bool m_isShowCappingPlane;//是否显示盖面

//	SectionPlane* m_plane;

//	bool m_Enable;
//	ImageBoard* solidPlan;

//	int m_index;
};
}
#endif // M3D_SECTION_NODE_H
