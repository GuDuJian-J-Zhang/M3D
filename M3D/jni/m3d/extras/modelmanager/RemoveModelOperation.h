/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_REMOVEMODEL_COMMAND_H
#define     M3D_REMOVEMODEL_COMMAND_H
#include 	"m3d/extras/Operation.h"

namespace M3D
{
/**
 * @class 删除模型操作
 */
class M3D_API RemoveModelOperation:public Operation
{
public:
	static const int TYPE;//类型
	RemoveModelOperation(View * view,Model * model);
	virtual~RemoveModelOperation();
	/**
	 * @Brief 执行操作
	 * @return 成功 true 失败 false
	 */
	bool OnExecute();

	/**
	 * @brief 转换为JSon字符串
	 * @return 返回Json串
	 */
	virtual string ToJsonString();

	/**
	 * @brief 获取配置路径
	 * @return 返回路径
	 */
	string GetPlcPath();
private:
	/**
	 * @brief 初始化
	 * @param view 当前视图
	 * @param model 模型
	 */
	void Init(View * view,Model * model);
private:
	string m_plcPath;//!<配置路径
	Model * m_model;//!<模型
};

}
#endif

