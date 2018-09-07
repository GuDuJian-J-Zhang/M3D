/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_COPYMODEL_COMMAND_H
#define     M3D_COPYMODEL_COMMAND_H
#include 	"m3d/extras/Operation.h"

namespace M3D
{
/**
 * @class 复制模型操作
 */
class M3D_API CopyModelOperation:public Operation
{
public:
	static const int TYPE;//!<类型
	CopyModelOperation(View * view,Model * srcModel,Model * desModel);
	virtual~CopyModelOperation();
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
     * @brief 获取新的模型
     * @return 返回模型
     */
	Model * GetNewMode() const;
    
	/**
	 * @brief 获取目标路径
	 * @return 返回目标路径
	 */
    const string& GetDestPlcPath();
    /**
     * @brief 获取源路径
     * @return 源路径
     */
    const string& GetSrcPlcPath();

private:
    string m_srcPlcPath;//!<源路径
    string m_destPlcPath;//!<目标路径
	Model * m_srcModel;//!<源模型
	Model * m_desModel;//!<目标路劲
	mutable Model * m_newModel;//!<新模型

};

}
#endif

