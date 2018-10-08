/**@file
 *@brief	在执行绘制的流程中，能够执行指定的回调函数
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef  VIEW_OPERATOR_H_
#define  VIEW_OPERATOR_H_
#include "m3d/action/Action.h"

#include "m3d/base/BoundingBox.h"
#include "m3d/action/CallBackAction.h"

namespace M3D
{

class SceneManager;

/**
 * 视图操作类，
 * 实现充满窗口显示，
 * 实现单个零件充满窗口显示等逻辑
 * @class
 * @brief
 */
class M3D_API ViewOperator: public CallBackAction
{
public:
	static bool FitView(SceneManager* scene);

	/**
	 *
	 */
	ViewOperator();
	virtual ~ViewOperator();

	/**
	 *
	 * @param scene
	 */
	void SetScene(SceneManager* scene);

	/**
	 *
	 * @return
	 */
	SceneManager* GetScene();

	/**
	 *
	 * @return
	 */
	BoundingBox& GetFitViewBox();

	/**
	 *
	 * @param data
	 * @param node
	 */
	static void ComputeFitViewBox(void* data, SceneNode* node);

private:

	BoundingBox m_fitViewBox; //!<适应视口显示的包围盒

	SceneManager* m_scene; //!<
};

}

#endif  /*VIEW_OPERATOR_H_*/
