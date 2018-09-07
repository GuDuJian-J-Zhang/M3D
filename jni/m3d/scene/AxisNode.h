/**
 *@file
 *@brief   节点
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef   M3D_AXISNODE_H_
#define   M3D_AXISNODE_H_
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/AxisNode.h"

namespace M3D
{
	class ImageBoard;
/**
 * 坐标轴绘制节点 在代码内部存储了坐标轴绘制所需要的三角网格数据，包括 X Y Z 和远点o
 */
class M3D_API AxisNode: public SceneNode
{

public:
	static float axisPntX[];//!<坐标轴X的网格数据
	static float axisPntY[];//!<坐标轴Y的网格数据
	static float axisPntZ[];//!<坐标轴Z的网格数据
	static float axisPntO[];//!<坐标轴O的网格数据

	//坐标轴位置
	static const int NONE;//!<默认位置
	static const int LEFTTOP;//!<左上角
	static const int LEFTDOWN;//!<左下角
	static const int RIGHTTOP;//!<右上角
	static const int RIGHTDOWN;//!<右下角
	static const int CENTER;//!<屏幕中间

public:
	AxisNode();
	virtual ~AxisNode();
	virtual int GetType(void);
	void SetViewSize(int width, int height, float ratio = 0.3f);

	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 设置坐标轴显示位置
	 * @param position
	 */
	void SetAxisPosition(int position);

private:
	void UpdataAxisImages(RenderAction* renderAction);

public:
	int m_iPntXNum; //!<纪录各个坐标轴绘制点的个数
	int m_iPntYNum;
	int m_iPntZNum;
	int m_iPntONum;

	float m_ProArray[6];

	int m_iViewX; //!<纪录坐标轴绘制视口的大小
	int m_iViewY;
	int m_iWidth;
	int m_iHeight;
	int m_iW;

	int m_srcWidth; //!<当前视口的宽和高
	int m_srcHeigh;

	Color m_xColor; //!<纪录坐标轴绘制颜色
	Color m_yColor;
	Color m_zColor;
	Color m_oColor;

	ImageBoard* m_xImage;
	ImageBoard* m_yImage;
	ImageBoard* m_zImage;

private:
	int m_position;  //!<坐标轴显示位置 0=不显示  1=左上 2左下 3右上 4右下 5居中


};
}
#endif // AXISNODE_H
