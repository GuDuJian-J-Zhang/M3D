/**@file
 *@brief
 *@author XL
 *@date
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_BILLBOARD_H_
#define M3D_BILLBOARD_H_

#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/base/Mutex.h"
namespace M3D
{
class RenderAction;
/**
 * @class
 * @brief 公告板类
 */
class M3D_API Billboard
{
public:
	Billboard();
	virtual ~Billboard();
	/**
	 * @brief 获取世界变换矩阵
	 * @param renderAction 渲染动作
	 * @return 返回矩阵
	 */
	Matrix3x4& GetWorldMatrix(RenderAction* renderAction);

	Matrix3x4& GetWorldMatrix(const Matrix3x4& modelViewMatrix);
	/**
	 * @brief 获取世界矩阵
	 * @return 返回矩阵
	 */
	Matrix3x4& GetWorldMatrix();
	/**
	 * @brief 获取符合OpenGL格式的矩阵
	 * @param renderAction 渲染动作
	 * @return 返回矩阵
	 */
	Matrix4& GetGLWorldMatrix(RenderAction* renderAction);

	Matrix4& GetGLWorldMatrix(const Matrix3x4& modelViewMatrix);
	/**
	 * @brief 获取符合OpenGL格式的矩阵
	 * @return 返回矩阵
	 */
	Matrix4& GetGLWorldMatrix();

	/**
	 * @brief 是否允许平移
	 * @param allow true允许 false 不允许
	 */
	void AllowTran(bool allow);
	/**
	 * @brief 是否允许旋转
	 * @param allow true允许 false 不允许
	 */
	void AllowRotate(bool allow);
	/**
	 * @brief 是否允许缩放
	 * @param allow true允许 false 不允许
	 */
	void AllowScale(bool allow);

	/**
	 * @brief 返回允许平移状态
	 * @return true 允许 false 不允许
	 */
	bool IsAllowTran();
	/**
	 * @brief 返回允许旋转状态
	 * @return true 允许 false 不允许
	 */
	bool GetRotate();
	/**
	 * @brief 返回允许缩放状态
	 * @return true 允许 false 不允许
	 */
	bool GetScale();

	/**
	 * @brief 设置中心
	 * @param center 中心坐标
	 */
	void SetCenter(const Vector3& center);
	/**
	 * @brief 获取中心坐标
	 * @return 中心坐标
	 */
	Vector3& GetCenter();

	static float GetFitShowScale(RenderAction* renderAction,Vector3& position);

private:
	Matrix3x4 m_worldMatrix;//!<世界矩阵
	Matrix4 m_glWorldMatrix;//!<opengl格式世界矩阵
	bool m_allowRotate;//!<允许旋转状态
	bool m_allowTran;//!<允许平移状态
	bool m_allowScale;//!<允许缩放状态
	//中心点
	Vector3 m_center;//!<旋转中心
};
}
#endif /*M3D_BILLBOARD_H_*/

