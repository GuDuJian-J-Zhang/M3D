/**@file
 *@brief
 *@author	CDJ
 *@date		2015-12-24
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_DRAGGER_MANAGER_H
#define M3D_DRAGGER_MANAGER_H

#include "m3d/M3D.h"

#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/Model.h"
#include "sview/views/View.h"
#include "m3d/Handler/dragger.h"
#include "m3d/extras/modelmanager/PivotCallbacks.h"

using namespace SVIEW;

namespace SVIEW
{
class View;
}

namespace M3D
{

class Model;
class Matrix3x4;
class OperationHistoryManager;
 

/**
 * @brief 模型管理器，用来对管理对模型的操作
 */
class M3D_API DraggerManager:public Object
{
public:
	DraggerManager(View * view);

	virtual ~DraggerManager();

public:

	 //
	 Dragger* BindExplosionDragger(vector<Model*> models, int explosionType);
	 Dragger* BindExplosionDragger(vector<Model*> models, int explosionType, Vector3 vect);
	 Dragger* BindExplosionFreedomDragger(vector<Model*> models, int explosionType, Vector3 direction, Vector3 pos);
	 Dragger* BindIndicatorDragger(Vector3 direction, Vector3 pos);
	 //用于预选的dragger的移动操作
	 bool MoveIndicatorDragger(Dragger* dragger,Vector3 direction,Vector3 pos);
	 Dragger* BindDragger(vector<Model*> models, int drggerType, bool bGloal = true);
	 Dragger* BindDragger(vector<Model*> models, int drggerType, Matrix3x4* ma, bool bMove = true, bool bMulGizm = false, bool bGloal = true);
	 bool UnBindDragger(Dragger* dragger);

	 Dragger* BindSectionDragger(int drggerType);
	 //绑定的剖切拖拽器到默认包围盒中心
	 Dragger* InitBindSectionDragger(int draggerType);	 
	 void SetPivotCB(M3D_PIVOTCB* pivotCB) { m_pivotCB = pivotCB; }
	 M3D_PIVOTCB* GetPivotCB() { return m_pivotCB; }

	 /**
	  * @brief 获取命令操作记录
	  * @return 返回操作历史记录
	  */
	OperationHistoryManager* GetCommandHistoryManager();
	//获取当前拖拽器
	Dragger* GetDragger() { return m_dragger; }

private:
	Dragger* BindExplosionAxisDragger(vector<Model*> models,int explosionType, Vector3 vect);
	Dragger* BindAxisDragger(vector<Model*> models, bool bGloal = true);
	Dragger* BindAxisDragger(vector<Model*> models, Matrix3x4* ma, bool bMove = true, bool bMulGizm = false, bool bGloal = true);
	Dragger* BindExplosionStdDragger(vector<Model*> models, int explosionType);
	Dragger* bindMinusAxisDragger(vector<Model*> models);
	Dragger* BindScaleDragger(vector<Model*> models);
	Dragger* BindRotateDragger(vector<Model*> models, bool bGloal = true);
	Dragger* BindRotateDragger(vector<Model*> models, Matrix3x4* ma, bool bMove = true, bool bMulGizm = false, bool bGloal = true);
	Dragger* BindRotateCylinderDragger(vector<Model*> models, bool bGloal = true);
public:
	Vector3 GetBindCenter(Model* model);
private:
	OperationHistoryManager* m_commandHistoryManager;//!<操作历史记录
	View * m_view;//!<当前视图
	Dragger* m_dragger;//当前拖拽器
	M3D_PIVOTCB* m_pivotCB;
};

} /* namespace M3D */
#endif /* M3D_MODEL_MANAGER_H */
