/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_ADDFILE_COMMAND_H
#define     M3D_ADDFILE_COMMAND_H
#include 	"m3d/extras/Operation.h"

namespace M3D
{
/**
 * @class 添加文件操作
 */
class M3D_API AddFileOperation:public Operation
{
public:
	static const int TYPE; //!<类型
	/**
	 * @brief 构造函数
	 * @param view 当前视图
	 * @param filePath 文件路径
	 * @param desModel 目标模型
	 */
	AddFileOperation(View * view,string& filePath, Model* desModel);

	virtual ~AddFileOperation();

	/**
	 * @brief 转换为JSon字符串
	 * @return 返回Json串
	 */
	virtual string ToJsonString();

	/**
	 * @brief 获取上级配置路径
	 * @return 上级配置路径
	 */
	string  GetUpperPlcPath();

	/**
	 * @Brief 执行操作
	 * @return 成功 true 失败 false
	 */
	bool OnExecute();
    
	/**
	 * @brief 获取文件路径
	 * @return 文件路径
	 */
    string GetFilePath();
    
    /**
     * @brief 获取新的模型
     * @return 返回模型
     */
	Model * GetNewModel() const;
private:

	/**
	 * @brief 初始化
	 * @param view 当前视图
	 * @param filePath 文件路劲
	 * @param desModel 目标模型
	 */
	void Init(View * view,string& filePath,Model * desModel);
private:
	string m_filePath;//!<文件路径
	string m_upperPlcPath; //!<上级配置路径
	Model * m_desModel;//!<上级模型
	mutable Model * m_retModel;//!<插入后生成的模型

};

}
#endif

