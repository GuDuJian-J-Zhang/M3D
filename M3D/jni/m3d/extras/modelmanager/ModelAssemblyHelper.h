/**@file
 *@brief
 *@author	XL
 *@date		2016-1-4
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_MODELASSEMBLYHELPER_H_
#define M3D_MODELASSEMBLYHELPER_H_
#include "m3d/M3D.h"

#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/Model.h"
#include "Sview/views/view.h"
#include "m3d/scene/ModelNode.h"
#include "m3d/base/Quaternion.h"

using SVIEW::View;
namespace M3D
{
class Model;
class Matrix3x4;

/**
 * @brief 模型装配辅助类
 */
class M3D_API ModelAssemblyHelper
{
public:
	ModelAssemblyHelper();
	virtual ~ModelAssemblyHelper();


	/**
	 * 获取model装配矩阵
	 * @param model 模型
	 * @return	返回矩阵
	 */
	static Matrix3x4 GetPlaceMatrix(Model* model);

	/**
	 * 获取model世界矩阵
	 * @param model 模型
	 * @return 返回矩阵
	 */
	static Matrix3x4 GetWorldMatrix(Model* model);

	/**
	 * 获取model装配路径
	 * @param model 模型
	 * @return	返回路径
	 */
	static string GetPlacePath(Model* model);

	/**
	 * 设置model装配路径path
	 * @param model 模型
	 * @param path 要设置的路径
	 */
	static void SetPlacePath(Model* model, string path);

	/**
	 * @brief 设置装配路径
	 * @param model 模型
	 * @param pareModel 父路径
	 */
	static void SetPlacePath(Model* model,Model* pareModel);


	/**
	 * 设置model装配矩阵plcMat
	 * @param model 模型
	 * @param plcMat 配置路径
	 */
	static void SetPlaceMatrix(Model* model, const Matrix3x4 & plcMat);

	/**
	 * 设置model装配矩阵
	 * @param model 模型
	 */
	static void ComputePlaceMatrix(Model* model);

	/**
	 * 设置model世界矩阵wldMat
	 * @param model
	 * @param wldMat
	 */
	static void SetWorldMatrix(Model* model, const Matrix3x4 & wldMat);

	/**
	 * 获取model最大装配 ID
	 * @param model
	 * @return 返回ID最大值对应的字符串
	 */
	static int GetMaxPlcId(Model * model);
	/**
	 *	将sourceModel插入到desModel最后位置
	 * @param sourceModel 	第一次选中的零件
	 * @param desModel		第二次选中的零件
	 * @return	返回操作结果对应的代码
	 */
	static bool InsertBefore(View * view,Model* sourceModel, Model* desModel);

	/**
	 * 解除model模型与父节点联系
	 * @param model 模型
	 * @return 返回操作结果对应的代码
	 */
	static bool DetachModel(Model* model);

	/**
	 * 删除视图view中的model模型
	 * @param view 当前视图
	 * @param model 模型
	 * @return 返回操作结果对应的代码
	 */
	static bool DeleteModel(View * view,Model* model);

	/**
	 * @brief 添加模型
	 * @param view 当前视图
	 * @param fileName 文件名字
	 * @return 返回加入的模型
	 */
	static Model* AddModel(View * view,const string& fileName);

	/**
	 * 从ID获取模型
	 * @param view 当前视图
	 * @param modelId 模型ID
	 * @return ID对应的模型指针
	 */
	static Model * GetModelFromId(View * view,int modelId);

	/**
	 * 获取模型父节点
	 * @param model 模型
	 * @return 模型对应的ModelNode节点指针
	 */
	static ModelNode * GetParentNode(Model * model);

	/**
	 * 获取模型节点
	 * @param model 模型
	 * @return 模型对应的ModelNode节点指针
	 */
	static ModelNode * GetModelNode(Model * model);

	/**
	 * @brief 旋转装配模型
	 * @param model 要旋转的模型
	 * @param quat 旋转量
	 */
	static void RotateAssemblyModel(Model * model,Quaternion & quat);

	/**
	 * @brief 平移装配模型
	 * @param model 要平移的模型
	 * @param move  平移量
	 */
	static void TranslateAssemblyModel(Model * model,Vector3 & move);

	/**
	 * @brief 缩放装配模型
	 * @param model 要缩放的模型
	 * @param scale 缩放量
	 */
	static void ScaleAssemblyModel(Model * model ,Vector3 & scale);
private:
	/**
	 * @brief 获取模型世界包围盒
	 * @param model 模型
	 * @return 返回包围盒
	 */
	static BoundingBox GetModelWorldBoundingBox(Model * model);

	static void PrintPlcID(Model * model);
};

} /* namespace M3D */
#endif /* M3D_MODELASSEMBLYHELPER_H_ */
