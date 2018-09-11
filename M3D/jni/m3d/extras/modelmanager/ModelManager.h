/**@file
 *@brief
 *@author	CDJ
 *@date		2015-12-24
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_MODEL_MANAGER_H
#define M3D_MODEL_MANAGER_H

#include "m3d/M3D.h"

#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/Model.h"
#include "Sview/views/view.h"
#include "m3d/handler/dragger.h"

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
 * @brief 装配错误信息表
 */
typedef enum AssambleMsg
{
	ASSAMBLY_BASE = -1,         	 //!< ASSAMBLY_BASE
	ASSAMBLY_INS_BEFO_ERR = 0,		 //!< insert error
	ASSAMBLY_INS_AFTR_ERR = 1,  	 //!<
	ASSAMBLY_DET_ERR = 2,			 //!< detach error
	ASSAMBLY_MOVTO_ERR = 3,			 //!< move error
	ASSAMBLY_CPYTO_ERR = 4,			 //!< copy error
	ASSAMBLY_ADD_ERR = 5,			 //!< add error
	ASSAMBLY_DEL_ERR = 6,     	     //!< delete error

	ASSAMBLY_INS_BEFO_SUCES = 20,	 //!< insert success
	ASSAMBLY_INS_AFTR_SUCES = 21,	 //!<
	ASSAMBLY_DET_SUCES = 22,    	 //!< detach success
	ASSAMBLY_MOVTO_SUCES = 23,  	 //!< move success
	ASSAMBLY_CPYTO_SUCES = 24,  	 //!< copy success
	ASSAMBLY_ADD_SUCES = 25,    	 //!< add success
	ASSAMBLY_DEL_SUCES = 26,    	 //!< delete success

} ASSEMBLYMSG;


/**
 * @brief 模型管理器，用来对管理对模型的操作
 */
class M3D_API ModelManager:public Object
{
public:
	ModelManager(View * view);

	virtual ~ModelManager();

public:
	/**
	 * 删除模型model节点
	 * @param view
	 * @param model
	 * @return 返回操作结果对应的代码
	 */
	 bool Delete( Model * model);

	/**
	 * 将model移动到parentModel下子模型点最后一个位置
	 * @param view
	 * @param model
	 * @param parentModel
	 * @return 返回操作结果对应的代码
	 */
	 bool MoveTo(Model * model,Model * parentModel);

	/**
	 * 将model复制到parentModel下子模型最后一个位置，并传出新模型对象指针newModel
	 * @param view
	 * @param model
	 * @param parentModel
	 * @param newModel
	 * @return 返回操作结果对应的代码
	 */
	 bool CopyTo(Model * model,Model * parentModel ,Model * &newModel);

	/**
	 * 打开指定文件路径的模型，将模型加入到parentModel下子模型点最后位置,并传出新模型指针
	 * @param view
	 * @param fileName
	 * @param parentModel
	 * @param retModel
	 * @return 返回操作结果对应的代码
	 */
	 bool AddTo(string& fileName,Model * parentModel,Model*& retModel);

	 /**
	 * 打开指定文件路径的模型，用该模型替换掉model，同时将model的配置ID、路径等赋值给新建模型 并返回
	 * @param model 将被替换的模型
	 * @param parentModel 父模型
	 * @param fileName 文件全路径
	 * @return 新打开文件创建的模型
	 */
	 Model* ReplaceWith(Model* model, Model* parentModel, string& fileName);

	 /**
	  * @brief 重命名模型名字
	  * @param name
	  * @return 成功 true 失败 false
	  */
	 bool ReName(Model * model,string & name);

	 //void SetID(int id);
	 bool SetTransform(Model* model,const Matrix3x4& plcMatrix,int transformSpace);
	 /**
	  * @brief 获取命令操作记录
	  * @return 返回操作历史记录
	  */
	 OperationHistoryManager* GetCommandHistoryManager();
	 /**
	 * @brief 初始化新建装配
	 * @param model 新建model
	 */
	 void InitNewModel(Model* model);
 
private:
	OperationHistoryManager* m_commandHistoryManager;//!<操作历史记录
	View * m_view;//!<当前视图
};

} /* namespace M3D */
#endif /* M3D_MODEL_MANAGER_H */
