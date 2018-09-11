/**@file
 *@brief	Point类头文件
 *@author	xuli
 *@date		2015-7-13
 *@version	1.0
 *
 *ShapeHelper类头文件
 *
 */

#ifndef M3D_VIEWHELPER_H_
#define M3D_VIEWHELPER_H_

#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/SceneManager.h"

namespace M3D{
class Point;
}
using namespace M3D;

namespace SVIEW
{
class View;
/**@class ViewHelper
 * @brief ViewHelper类
 *
 * 提供ViewHelper类的相关方法
 */
class M3D_API ViewHelper
{
public:
	ViewHelper();
	virtual ~ViewHelper();

	/**
	 * 设置shape对象到屏幕坐标确定的三维位置
	 * @param screenPt
	 * @param shape
	 * @return
	 */
	static bool SetShapePosByScreenPoint(const Vector2& screenPnt, IShape* shape,View* view);

	/**
	 * 计算屏幕坐标反投影到空间中
	 * @param screenPnt
	 * @param depth
	 * @param view
	 * @return
	 */
	static Vector3 ScreenToWorldPoint(const Vector2& screenPnt,float depth,View* view);
    
    /**
     * 计算屏幕坐标反投影shape包围盒中心点所在的平面上，获取世界坐标系的点
     * @param screenPnt
     * @param depth
     * @param view
     * @return
     */
    static Vector3 ScreenToWorldPointByShape(const Vector2& screenPnt, IShape* shape,View* view);
    
    /**
     * 计算屏幕坐标反投影shape包围盒中心点所在的平面上，获取shape对象所在的局部坐标系的点
     * @param screenPnt
     * @param shape
     * @param view
     * @return
     */
    static Vector3 ScreenToLocalPointByShape(const Vector2& screenPnt, IShape* shape,View* view);
    
    static Vector3 WorldPointToLocal(const Vector3& worldPnt, IShape* shape,View* view);

    static IntVector2 WorldPointToScreen(const Vector3& worldPnt,View* view);

	static float GetCameraCompareZoom(View* view);
};
}
#endif /*M3D_VIEWHELPER_H_*/

