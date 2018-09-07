/**@file
*@brief
*@author	QYP
*@date		2018-06-07
*@version	1.0
*@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
*
*/
#ifndef M3D_REPLACEMODEL_COMMAND_H
#define M3D_REPLACEMODEL_COMMAND_H

#include 	"m3d/extras/Operation.h"

namespace M3D
{
	/**
	* @class 替换模型操作
	*/
	class ReplaceModelOperation :public Operation
	{
	public:
		/**
		* @brief 初始化
		* @param view 当前视图
		* @param oldModel 将被替换的模型
		* @param oldParentModel 将被替换的模型的父模型
		* @param filePath 用来替换的文件全路径
		*/
		ReplaceModelOperation(View * view, Model* oldModel, Model* oldParentModel, string& filePath);
		virtual ~ReplaceModelOperation();
	public:
		/**
		* @Brief 执行操作
		* @return 成功 true 失败 false
		*/
		bool OnExecute();
		/**
		* @brief 获取新的模型
		* @return 返回模型
		*/
		Model * GetNewModel() const;
		/**
		* @brief 使用原有模型的数据替换新模型的数据（含子模型）
		* @param oldModel 原有模型 newModel 新模型
		* @return 无
		*/
		void ReplaceModelData(Model* oldModel, Model* newModel);
	private:
		/**
		* @brief 初始化
		* @param view 当前视图
		* @param oldModel 将被替换的模型
		* @param oldParentModel 将被替换的模型的父模型
		* @param filePath 用来替换的文件全路径
		*/
		void Init(View * view, Model* oldModel, Model* oldParentModel, string& filePath);

	protected:
		Model*  m_oldModel;//!<被替换的模型
		Model*  m_oldParentModel;//!<被替换的模型父模型
		string  m_filePath;//!<文件路径
		mutable Model*  m_newModel;//!<新建的模型
	};
}

#endif //M3D_REPLACEMODEL_COMMAND_H