/**@file
 *@brief	Point类头文件
 *@author	xuli
 *@date		2015-7-13
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *ShapeHelper类头文件
 *
 */

#ifndef M3D_SHAPEHELPER_H_
#define M3D_SHAPEHELPER_H_

#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/SceneManager.h"
#include "m3d/model/Model.h"

namespace M3D
{

class Point;

/**@class ShapeHelper
 * @brief ShapeHelper类
 *
 * 提供ShapeHelper类的相关方法
 */
class M3D_API ShapeHelper: public Object
{
public:
	ShapeHelper();
	virtual ~ShapeHelper();
	/**
	 * 构造一个临时点对象
	 * @param point
	 * @param scene
	 * @return
	 */
	static HandlerPoint* AddPointHandler(const Vector3& coordinate,float size,SceneManager* scene);
    /**
     * 选择特征点对象
     * @param screenPnt
     * @param scene
     * @return
     */
    static IShape* SelectFeaturePnt(Vector2& screenPnt,SceneManager* scene);
	/**
	 * 选择shape对象
	 * @param screenPnt
	 * @param shapeType
	 * @param geoType
	 * @param scene
	 * @return
	 */
	static IShape* SelectShape(Vector2& screenPnt,int shapeType,
			int geoType,SceneManager* scene);

	/**
	 * 获取当前shape对象显示时worldMatrix
	 * @param shape
	 * @return shape为空时返回单位矩阵，不为空时正常返回
	 */
	static Matrix3x4 GetShapeWorldMatrix(IShape* shape);

	/**
	 * 根据当前显示状态，确定屏幕上对应的大小，转换成空间中的对应大小
	 * @param scene
	 * @param size
	 * @return
	 */
	static  Vector2 GetCommonSize(SceneManager* scene,const Vector2& size);
};
}
#endif /*M3D_SHAPEHELPER_H_*/

