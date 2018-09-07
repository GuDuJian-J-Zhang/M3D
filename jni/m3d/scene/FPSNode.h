/**@file
 *@brief    帧率计算节点
 *@author	bing
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef M3D_FPSNODE_H_
#define M3D_FPSNODE_H_

#include "m3d/scene/SceneNode.h"
#include "m3d/graphics/TextNode.h"

namespace M3D
{
class ComText;
class M3D_API FPSNode: public SceneNode
{
public:
	FPSNode();
	virtual ~FPSNode();
	virtual int GetType(void);

	/**
	 * 设置当前FPS的值
	 * @param fps
	 */
	void SetFPS(float fps);

	/**
	 * 设置屏幕高度，来近似控制FPS 显示位置
	 * @param
	 */
	void SetSceneHeight(int);

	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 获取绘制网格
	 * @return
	 */
	ComText* GetFPSMesh();

protected:
	static void InitFpsTexts();

private:
	static map<string,ComText*> fpsTexts;

public:
	///控制FPS 显示 和坐标轴绘制原理类似
	///@see AxisNode
	int m_iViewX;
	int m_iViewY;
	int m_iViewWidth;
	int m_iViewHeight;
	int m_iSceneHeight;
	TextNode* m_pTextNode;

private:
	float m_fps; //刷新率相关
	float m_fFrameRate;
	int m_iFrameCount;
	long m_lLastFrameTime;
};

}
#endif /* FPSNODE_H_ */
