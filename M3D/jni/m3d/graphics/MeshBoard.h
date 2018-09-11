/**@file
 *@brief	图片的显示板
 *@brief
 *@author  XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_MESH_BOARD_H_
#define M3D_MESH_BOARD_H_
#include "m3d/model/Shape.h"
#include "m3d/graphics/Billboard.h"
namespace M3D
{
class M3D_API MeshBoard: public Shape,public Renderable
{
public:
	MeshBoard();
	MeshBoard(const Vector3& pos, const Vector2& size);

	/**
	 *
	 * @param pos
	 * @param size
	 */
	void Set(const Vector3& pos, const Vector2& size);

	virtual ~MeshBoard();

	/**
	 * 获取线的顶点数据集合
	 * @return
	 */
	Vector3* GetLinesVertexs();

	/**
	 * 计算包围盒
	 */
	virtual void ComputeBox();
	virtual void RayPick(RayPickAction* action);
	void UpdateRenderData(RenderAction* renderAction);

private:
    Vector3 m_position;//!<
    Vector2 size_;//!<
//  Rect uv_;

	vector<Vector3> points;//!<
	mutable Mutex m_mutex; //!<线程锁对象
	Billboard m_bindBillboard;//!<
};
}

#endif /* M3D_IMAGE_BOARD_H_ */
