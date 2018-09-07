/**@file
 *@brief
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *ShapeSet类头文件
 *
 */

#ifndef M3D_CAD_NODE_H_
#define M3D_CAD_NODE_H_

#include "m3d/graphics/Renderable.h"
#include "m3d/model/Shape.h"
#include "m3d/model/LODData.h"
namespace M3D
{
class Edge;
class Body;
class RenderAction;
class Model;

/**
 * @class ShapeSet
 * @brief ShapeSet类
 *
 * CAD中几何图形集类
 */
class M3D_API ShapeSet: public Object
{
public:
	/**
	 * 节点类型
	 */
	typedef enum
	{
		Undefine, //!<未定义
		Sketch, //!<草图
		GeometricalSet, //!<几何图形集
		Point, //!<点
		Plane, //!<平面
		Line, //!<直线
		Curve, //!<曲线
		Circle, //!<圆
		Surface, //!<曲面
		Nurbs, //!<Nurbs
		AbsoluteAxis, //!<绝对轴
		Origin, //!<绝对轴：原点
		HDirection, //!<绝对轴：横向
		VDirection, //!<绝对轴：纵向
		Constraints, //!<约束
		Parallelism, //!<约束：平行
		Angle, //!<约束：角度
		Coincidence, //!<约束：相合
		Perpendicularity, //!<约束：垂直

	} ShapeSetType;
public:
	ShapeSet();
	virtual ~ShapeSet();
	ShapeSet(const ShapeSet& orig);
	ShapeSet& operator =(const ShapeSet& orig);

	void SetID(long id);
	long GetID();

	/**
	 * 添加关联下级几何对象
	 * @param shape
	 */
	void AddShape(Shape* shape);

	/**
	 * 获取所有下级几何图形集对象
	 * @return
	 */
	vector<Shape*>& GetShapes();

	/**
	 * 几何图形集类型
	 * @return
	 */
	ShapeSetType GetType();
	void SetType(ShapeSetType type);

	string GetName();
	void SetName(string &name);

	vector<ShapeSet*>& GetChildren();
	void AddChild(ShapeSet *child);

	/**
	 * 由于Node节点在拷贝构造时，无法将其管理的shape对象进行拷贝，
	 * 已经拷贝过的对象在model中进行存储，通过其内部存储的id，通过查找来进行更新
	 * @param model
	 */
	void UpdataRelevateShape(Model* model);

	void SetParent(ShapeSet* node);
	ShapeSet* GetParent();

	void SetVisible(bool visiable);
	bool Getvisible();

	bool IsOrigVisible();

	void SetOrigVisible(bool visible);

	void Restore();
private:
	vector<Shape*> m_shapes;//!< 存储所有的几何图形集对象
	bool m_visible;//!< 可见标识
	bool m_origVisible;///读取时候的显示隐藏状态
	int m_ID;//!<ID
	ShapeSetType m_Type;//!<类型
	vector<ShapeSet*> m_Children;//!<子几何图形集对象
	string m_Name;//!<名称
	ShapeSet* m_parent;//!<上级几何图形集对象
};

}

#endif /*M3D_CAD_NODE_H_*/

