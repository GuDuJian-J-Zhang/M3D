/**@file
 *@brief	不同状态差值工具类，能够实现不同状态之间切换的动画效果
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef  M3D_ANIMATIONCREATOR_H
#define  M3D_ANIMATIONCREATOR_H
#include "m3d/graphics/MovedMatrixInfo.h"

using M3D::MovedMatrixInfo;

typedef map<string, MovedMatrixInfo> NODEINFO;

namespace M3D
{
class ModelView;
class SceneManager;
/**
 * @class
 * @brief M3D动画插值类,用来模拟不同状态之间切换的动画
 */
class M3D_API Animationcreator
{
public:
	static void StateSlerp(NODEINFO& beginState, NODEINFO& endState,
			vector<NODEINFO>& midStates, int times, SceneManager* scene);

	static void CameraStateSlerp(Matrix4& beginState, Matrix4& endState,
			vector<Matrix4>& midStates, int times);

	static void CameraMatrixSlerp(Matrix4& beginState, Matrix4& endState,
			Matrix4& midStates, float interPos);
	static void MatrixInfoSlerp(MovedMatrixInfo& fromState,
			MovedMatrixInfo& toState, MovedMatrixInfo& midStates,
			float interPos);
	static void CreateAnnitation(ModelView* modelView, SceneManager* scene,
			int frameTimes);

};

}

#endif /*M3D_ANIMATIONCREATOR_H*/
