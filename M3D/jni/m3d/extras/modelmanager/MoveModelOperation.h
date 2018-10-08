/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_MOVEMODEL_COMMAND_H
#define     M3D_MOVEMODEL_COMMAND_H
#include 	"m3d/extras/Operation.h"

namespace M3D
{
/**
 * @class 移动模型操作
 */
class M3D_API MoveModelOperation:public Operation
{
public:
	static const int TYPE;//!<模型

	MoveModelOperation(View * view, Model * srcModel,Model* desModel);
	virtual~MoveModelOperation();
	/**
	 * @brief 转换为JSon字符串
	 * @return 返回Json串
	 */
	virtual string ToJsonString();

	/**
	 * @brief 执行操作
	 * @return 成功 true 失败 false
	 */
	bool OnExecute();
	/**
	 * @brief 获取前面的配置路径
	 * @return 返回路径
	 */
	string GetForwPlcPath();
	/**
	 * @brief 获取之后的配置路径
	 * @return 返回路径
	 */
	string GetAfterPlcPath();
private:
	string m_ForwPlcPath;//!<前面的路径
	string m_AfterPlcpath;//!<之后的路径
	Model * m_srcModel;//!<源模型
	Model * m_desModel;//!<目标模型
};/*M3D_MOVEMODEL_COMMAND_H*/

}
#endif

