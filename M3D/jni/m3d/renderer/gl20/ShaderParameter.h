/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-16
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef SHADERPARAMETER_H_
#define SHADERPARAMETER_H_
#include"m3d/M3D.h"
#include "m3d/renderer/GLESHead.h"
namespace M3D
{
/**
 * @class
 * @brief 用来管理shader中变量的一些参数
 */
class M3D_API ShaderParameter
{
public:
	ShaderParameter();
	ShaderParameter(GLint location,GLenum type,string name );
	virtual ~ShaderParameter();
	GLint m_location;//!<位置
	GLenum m_Type;//!<类型
	string m_name;//!<名字
};

} /* namespace M3D */
#endif /* SHADERPARAMETER_H_ */
