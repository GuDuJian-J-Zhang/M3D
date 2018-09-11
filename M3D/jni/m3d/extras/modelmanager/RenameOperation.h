/**@file
 *@brief
 *@author	XL
 *@date		2016-1-6
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef RENAMEOPERATION_H_
#define RENAMEOPERATION_H_

#include "m3d/extras/Operation.h"

namespace M3D
{

/**
 * @brief 重命名操作
 */
class M3D_API RenameOperation: public M3D::Operation
{
public:
	RenameOperation(View * view,Model * model,string & name);
	virtual ~RenameOperation();
	/**
	 * @Brief 执行操作
	 * @return 成功 true 失败 false
	 */
	bool OnExecute();
private:
	Model * m_model;//!<模型
	string m_lastName;//!<原名字
	string m_newName;//!<新名字
};

} /* namespace M3D */
#endif /* RENAMEOPERATION_H_ */
