/**@file
 *@brief	Point类头文件
 *@author	xuli
 *@date		2015-7-13
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Point类头文件
 *
 */

#ifndef M3D_POINT_H_
#define M3D_POINT_H_

#include "m3d/model/Shape.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class PointSet;
class Body;
class SceneManager;
class CameraNode;
class Texture;
class ImageBoard;
class MeshBoard;
class RenderAction;
/**@class Point
 * @brief Point类
 *
 * 提供Point类的相关方法
 */
class M3D_API Point: public Shape, public Renderable
{
public:
	static float BOX_LENGTH; ///包围盒的长度，由于点没有办法创建包围盒，因此根据点的坐标，沿着轴的方向偏移后，确定包围盒的最大点和最小点
public:
	Point();
	/**
	 * 根据位置构造一个显示点
	 * @param coord
	 */
	Point(const Vector3& coord);
	Point(const Point& orig);
	Point& operator =(const Point& orig);
	virtual ~Point();

	/**
	 * 设置显示位置
	 * @param coordinate
	 */
	void SetCoordinate(const Vector3& coordinate);
	/**
	 * 获取显示位置
	 * @return
	 */
	Vector3& GetCoordinate();

	virtual void ComputeBox();

	virtual void RayPick(RayPickAction* action);

	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 更新渲染所需数据
	 * @param renderAction
	 */
	void UpdateRenderData(RenderAction* renderAction);

	/**
	 * 设置上级body对象
	 * @param body
	 */
	void SetBody(Body* body);

	/**
	 * 获取 上级body对象
	 * @return
	 */
	Body* GetBody();

	/**
	 * 得到渲染颜色
	 * @return
	 */
	virtual const  Color& GetRenderColor();

	/**
	* 得到渲染颜色
	* @return
	*/
	virtual const  Color& GetShapeColor();

	/**
	 * 获取显示图片,使用图片方式绘制时使用
	 * @return
	 */
	ImageBoard* GetImageboard();

	/**
	 * 获取显示Mesh，使用Mesh方式绘制时使用
	 * @return
	 */
	MeshBoard* GetMeshBoard();

	/**
	 * 设置绘制方式，使用图片还是使用Mesh
	 * @param type
	 */
	void SetDrawType(int type);

	/**
	 * 获取绘制方式
	 * @return
	 */
	int GetDrawType();

	/**
	 * 设置显示时大小
	 * @param size
	 */
	void SetSize(float size);

	/**
	 * 获取显示大小
	 * @return
	 */
	float GetSize();
    
	/**
	 * 是否最前端显示标识
	 * @param frontShow
	 */
	virtual void SetFrontShow(bool frontShow);
private:
	void Init(const Vector3& coord);

private:
	ImageBoard* m_pointImage; //!< 使用图片方式绘制
	MeshBoard* m_meshboard;//!<使用Mesh方式绘制

	Vector3 m_coordinate;///点坐标值

	Body* m_body;//!<上级Body对象

	int m_drawType;//点显示的方式 0 划线 1 绘制图片

	float m_size;//!<显示大小
};

/**
 * TODO 使用引用的方式来绘制点，从而加快显示效率
 */
class M3D_API RefPoint: public Shape
{

};

/**
 * 存储一系列点数据，可以加快显示效率
 */
class M3D_API PointSet: public Shape
{

};

}

#endif /*M3D_POINT_H_*/

