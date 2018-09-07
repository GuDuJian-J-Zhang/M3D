/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-23
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_GLSHAPEDRAWERHELPER_H_
#define M3D_GLSHAPEDRAWERHELPER_H_
#include "m3d/M3D.h"
#include "m3d/renderer/GL20/GLDrawer20.h"
namespace M3D
{
class ShaderProgram;
class BaseMaterial;
class RenderAction;
class DirectionalLight;

/**
 * @class
 * @brief 渲染辅助类。主要是设置一些uniform量
 */
class M3D_API GLShapeDrawerHelper
{
public:
	GLShapeDrawerHelper();
	virtual ~GLShapeDrawerHelper();
public:
	/**
	 * 设置灯光uniform
	 * @param shaderEffect
	 * @param light
	 * @param index
	 */
	static void SetLightUniform(ShaderProgram* shaderEffect,DirectionalLight * light,string index);
	/**
	 * 获取shader uniform量
	 * @param action
	 * @param ShaderMaterialType
	 * @return
	 */
	static ShaderProgram * GetShaderProgram(RenderAction* action,int ShaderMaterialType);
	/**
	 * 设置矩阵uniform量
	 * @param shaderEffect
	 * @param material
	 */
	static void SetMaterialUniform(ShaderProgram* shaderEffect,BaseMaterial * material);
};
}

#endif /* M3D_GLSHAPEDRAWERHELPER_H_ */
