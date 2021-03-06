﻿/**@file
 *@brief	图片的显示板
 *@brief
 *@author  XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_IMAGE_BOARD_H_
#define M3D_IMAGE_BOARD_H_
#include "m3d/model/Shape.h"
#include "m3d/graphics/Billboard.h"
namespace M3D
{
class Image;
class Texture;
/**
 * @class
 * @brief 图片公告板类
 */
class M3D_API ImageBoard: public Shape,public Renderable
{
public:
	ImageBoard();
	ImageBoard(const Vector3& pos, const Vector2& size);

	/**
	 * 设置位置大小
	 * @param pos
	 * @param size
	 */
	void Set(const Vector3& pos, const Vector2& size);

	void SetOrigSize(const Vector3& pos, const Vector2& size);

	virtual ~ImageBoard();

	/**
	 * 设置平面
	 * @param p
	 */
	void SetPlan(float* p);
	/**
	 * 获取顶点
	 * @return
	 */
	Vector3* GetVertexs();

	//vector<Vector3> GetLocalTransformedVertexs();

	vector<Vector3> GetWorldTransformedVertexs();

	/**
	 * 获取纹理坐标
	 * @return
	 */
	Vector2* GetTextureCoords();

	/**
	 * 计算包围盒
	 */
	virtual void ComputeBox();
	/**
	 * 拾取
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);

	/**
	 * 获取交点
	 * @param action
	 * @return
	 */
	vector<Vector3> GetIntersects(RayPickAction* action);

	/**
	 * 更新渲染数据
	 * @param renderAction
	 */
	void UpdateRenderData(RenderAction* renderAction);

	void UpdateRenderDataByModelViewMatrix(const Matrix3x4& modelViewMatrix);

	/**
	 * 获取渲染颜色
	 * @return
	 */
	virtual const  Color& GetRenderColor();

	/**
	 * 设置纹理
	 * @param texture
	 */
	void SetTexture(Texture* texture);

	/**
	 * 获取纹理
	 * @return
	 */
	Texture* GetTexture();

	/**
	 * 设置位置
	 * @param pos
	 * @return
	 */
	bool SetPosition(const Vector3& pos);

	/**
	 * 获取位置
	 * @return
	 */
	Vector3& GetPosition();
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
	bool IsAllowRotate();
	/**
	 * @brief 返回允许缩放状态
	 * @return true 允许 false 不允许
	 */
	bool IsAllowScale();
	bool GetInTop();
	void SetInTop(bool val);

	bool GetFlipV() const { return m_flipV; }
	void SetFlipV(bool val) { m_flipV = val; }


	/**
	* @brief 返回图片是否固定在屏幕上面显示
	* @return 
	*/
	bool GetFixShowInScreen() const;

	/**
	* @brief 设置图片固定在屏幕上面显示
	*/
	void SetFixShowInScreen(bool val);
	Vector2 GetSize();

	void SetAllowClip(bool value);
	bool GetAllowClip();
private:
    Vector3 m_position;//!<位置
    Vector2 size_;//!<大小
	Vector2 origSize;
    Rect uv_;//!<纹理坐标
 
	vector<Vector3> points;//!<顶点
	vector<Vector2> textCoords;//!<纹理坐标

	Billboard m_bindBillboard;//!<公告板
	Texture* m_texture;//!<纹理
	mutable Mutex m_mutex; //!<线程锁对象

	bool m_fixShowInScreen;//在屏幕上固定位置显示

	bool m_flipV;

	bool m_bAllowClip;//是否允许剖切
};
}

#endif /* M3D_IMAGE_BOARD_H_ */
