/**@file
 *@brief
 *@author	CDJ
 *@date		2016-4-6
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SCENEGROUNDNODE_H_
#define M3D_SCENEGROUNDNODE_H_

#include "m3d/scene/SceneNode.h"

namespace M3D
{

/**
 * @class
 * @brief 用来创建场景地面的类
 */
class M3D_API SceneGroundNode: public M3D::SceneNode
{
public:
	SceneGroundNode();
	virtual ~SceneGroundNode();
	virtual int GetType(void);
	virtual void FindVisiableObject(RenderAction* renderAction);
	/**
	 * 获取顶点数据
	 * @return
	 */
	Vector3* GetVertexs();
	/**
	 * 获取法向量
	 * @return
	 */
	Vector3* GetNormals();
	/**
	 * 获取纹理坐标
	 * @return
	 */
	Vector2* GetTexCoords();
	/**
	 * 获取投影矩阵
	 * @return
	 */
	Matrix4& GetProjectMat();
	/**
	 * 获取视图矩阵
	 * @return
	 */
	Matrix4& GetViewMat();
	/**
	 * 获取天光效果的灯光位置
	 * @return
	 */
	Vector3& GetSkyLightPos();
private:
	vector<Vector3> m_vertexs;//!<顶点
	vector<Vector3> m_normals;//!<法向量
	vector<Vector2> m_texCoords;//!<纹理坐标
	BoundingBox m_box;//!<包围盒
	RenderAction* m_renderAction;//!<渲染动作
	Matrix4 m_projectMat;//!<投影矩阵
	Matrix4 m_viewMat;//!<视图矩阵
	Vector3 m_skyLightPos;//!<灯光位置
};

} /* namespace M3D */
#endif /* M3D_SCENEGROUNDNODE_H_ */
