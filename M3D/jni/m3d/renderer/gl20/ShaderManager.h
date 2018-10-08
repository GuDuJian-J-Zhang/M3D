/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-16
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_


#include "m3d/M3D.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
#include <map>

namespace M3D
{

/**
 * @brief 为了方便map索引而创建的存储shader源码指针的结构体
 */
struct ShaderStrings
{
	ShaderStrings(const char * vs,const char* fs):m_vs(vs),m_fs(fs)
	{

	}
	const char* m_vs;//!<vertex shader
	const char* m_fs;//!<fragment shader
};

typedef map<string,ShaderStrings > ShaderStringsMap;//!<索引源码

typedef map<string,ShaderMaterialType> ShaderMTMap;//!<索引对应的材质名

class RenderAction;
/**
 * @class
 * @brief 用来管理shader的创建和索引
 */
class M3D_API ShaderManager
{
public:
	/**
	 * 下面是shaderProgram 对应的名字，按照功能划分
	 */
    const static string Base;
    const static string BaseVertex;
    const static string Background;
    const static string Wireframe;
    const static string Edge;
    const static string Brdf;
    const static string LightWithoutTexture;
    const static string LightWithTexture;
    const static string Axis;
    const static string Image;
    const static string Multilight;
    const static string CubeMap;
    const static string DepthMap;
    const static string  FBODebug;
    const static string  PlaneShadow;
    const static string  Blur;
    const static string  Specular;
    const static string EdgeDetection;
    const static string Quad;
	const static string MultilightPerFrag;

public:
	virtual ~ShaderManager();
	ShaderManager();
	/**
	 * 获取功能
	 * @param name
	 * @return
	 */
    ShaderProgram * GetEffect(string  name);
    /**
     * 从文件创建
     * @param effect_name
     * @param vsh
     * @param fsh
     * @return
     */
    ShaderProgram * CreateShaderProgramFrmFile(string effect_name, string vsh,  string fsh);
    /**
     * 从字符串创建
     * @param effect_name
     * @param vsh
     * @param fsh
     * @return
     */
    ShaderProgram * CreateShaderProgramFrmCde(string effect_name, const char * vsh,  const char * fsh);
    /**
     * 获取当前的渲染动作
     * @return
     */
    RenderAction * GetCurrentAction();
    /**
     *
     * @param action
     */
    void SetCurrentAction(RenderAction * action );
private:
    map<string,ShaderProgram *> shaderList;//!<shader 列表
    RenderAction * m_currentAction;//!<当前的渲染动作
    ShaderStringsMap m_shaderStrMap;//!<
    ShaderMTMap m_SMTMap;//!<


};

} /* namespace M3D */
#endif /* SHADERMANAGER_H_ */
