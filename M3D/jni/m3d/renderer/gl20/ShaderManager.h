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

#include "m3d/renderer/gl20/ShaderProgram.h"
#include "m3d/M3DExport.h"

#include <map>
#include "m3d/graphics/Texture.h"

namespace M3D
{
	class BaseMaterial;
	class LightManager;
	class Texture;
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
	const static string Dragger;
	const static string DraggerEdge;
	const static string XRay;
    const static string Image;
    const static string Multilight;
    const static string CubeMap;
    const static string DepthMap;
    const static string FBODebug;
    const static string  PlaneShadow;
    const static string  Blur;
    const static string  Specular;
    const static string EdgeDetection;
    const static string Quad;
	const static string MultilightPerFrag;
	const static string NoteEdge;
	const static string NoteImage;
	const static string CapPlane;
	const static string JewelFront;
	const static string JewelBack;
	const static string JewelFinalQuad;
	const static string Ring;
	const static string JewelType;
	const static string JewelHighLight;
	const static string JewelBlendQuad;
	const static string Outline;
	const static string GaussianBlur;
	const static string GaussianBlurOutline;
	const static string CombineOutline;
	const static string SceneGround;
	const static string SsaoEffect;
	const static string SsaoBlur;
	const static string SaoEffect;
	const static string SaoReconstructCSZ;
	const static string SaoMinifyCSZ;
	const static string SaoBlur;

	const static string NewJewelFront;
	const static string NewJewelBack;
	const static string JewelFrontInfo;

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

	string GetMaxPrecision(string precision);	

	//************************************
	// Method:    GetProgramCode
	// FullName:  M3D::ShaderManager::GetProgramCode
	// Access:    public 
	// Returns:   string
	// Qualifier: 获取program编码，也用作program名称name
	// Parameter: BaseMaterial * material
	//************************************
	string GetProgramCode(BaseMaterial *material);

	//************************************
	// Method:    CreateParameters
	// FullName:  M3D::ShaderManager::CreateParameters
	// Access:    public 
	// Returns:   void
	// Qualifier: 创建材质parameter，配合生成材质需要的shaderprogram
	// Parameter: BaseMaterial * material
	// Parameter: LightManager * lightManager
	//************************************
	void CreateParameters(BaseMaterial *material, LightManager* lightManager);

	

	//************************************
	// Method:    GetPrefixShader
	// FullName:  M3D::ShaderManager::GetPrefixShader
	// Access:    public 
	// Returns:   void
	// Qualifier: TODO 
	// Parameter: string & prefixVertex
	// Parameter: string & prefixFragment
	//************************************
	void GetPrefixShader(string& prefixVertex,string& prefixFragment );
	//************************************
	// Method:    CreateShaderProgram
	// FullName:  M3D::ShaderManager::CreateShaderProgram
	// Access:    public 
	// Returns:   ShaderProgram*
	// Qualifier: 真正的最后编译连接用的代码
	// Parameter: BaseMaterial * material
	// Parameter: string & vsh
	// Parameter: string & fsh
	// Parameter: string & code
	//************************************
	ShaderProgram* CreateShaderProgram(BaseMaterial* material , string &vsh ,string& fsh , string& code);
	//************************************
	// Method:    AcquireProgram
	// FullName:  M3D::ShaderManager::AcquireProgram
	// Access:    public 
	// Returns:   ShaderProgram*
	// Qualifier: 根据code获取材质
	// Parameter: BaseMaterial * material
	// Parameter: string & vsh
	// Parameter: string & fsh
	// Parameter: string & code
	//************************************
	ShaderProgram* AcquireProgram(BaseMaterial* material, string &vsh, string& fsh,string &code);

	void Init();
private:
	//************************************
	// Method:    ResetParameters
	// FullName:  M3D::ShaderManager::ResetParameters
	// Access:    private 
	// Returns:   void
	// Qualifier: 重置参数
	//************************************
	void ResetParameters();
	//************************************
	// Method:    GenerateExtensions
	// FullName:  M3D::ShaderManager::GenerateExtensions
	// Access:    private 
	// Returns:   string
	// Qualifier: 获取扩展
	// Parameter: map<string
	// Parameter: string> & parameters
	//************************************
	string GenerateExtensions(map<string,string>&parameters);
	//************************************
	// Method:    ReplaceLightNums
	// FullName:  M3D::ShaderManager::ReplaceLightNums
	// Access:    private 
	// Returns:   void
	// Qualifier: 替换代码中宏定义的灯光数量
	// Parameter: string & src
	// Parameter: map<string
	// Parameter: string> & parameters
	//************************************
	void  ReplaceLightNums(string& src,map<string,string>& parameters );
	//************************************
	// Method:    ParseIncludes
	// FullName:  M3D::ShaderManager::ParseIncludes
	// Access:    private 
	// Returns:   bool
	// Qualifier: 替换shaderchunk中的include代码
	// Parameter: string & code
	// Parameter: string & src
	// Parameter: map<string
	// Parameter: string> & lib
	//************************************
	bool ParseIncludes(string& code, string& src, map<string, string>&lib);
	//************************************
	// Method:    SetEnvTextureDefine
	// FullName:  M3D::ShaderManager::SetEnvTextureDefine
	// Access:    private 
	// Returns:   void
	// Qualifier:获取环境贴图预定义宏
	// Parameter: BaseMaterial * material
	// Parameter: string & envTextureType
	// Parameter: string & envTextureMode
	//************************************
	void SetEnvTextureDefine(BaseMaterial*material,string& envTextureType,string& envTextureMode);

	//************************************
	// Method:    GetTextureEncodingFromMap
	// FullName:  M3D::ShaderManager::GetTextureEncodingFromMap
	// Access:    private 
	// Returns:   int
	// Qualifier: 获取纹理的编码，比如是线性的，还是伽马空间的
	// Parameter: Texture * texture
	// Parameter: bool gammaOverrideLinear
	//************************************
	int GetTextureEncodingFromMap(Texture* texture, bool gammaOverrideLinear);

	//************************************
	// Method:    GetEncodingCompents
	// FullName:  M3D::ShaderManager::GetEncodingCompents
	// Access:    private 
	// Returns:   vector<string>
	// Qualifier: 获取纹理空间对应的编码处理函数
	// Parameter: TextureEncodingType encoding
	//************************************
	vector<string> GetEncodingCompents(TextureEncodingType encoding);

	//************************************
	// Method:    GetTexelDecodingFunction
	// FullName:  M3D::ShaderManager::GetTexelDecodingFunction
	// Access:    private 
	// Returns:   string
	// Qualifier: 获取纹理空间对应的解码处理函数
	// Parameter: string functionName
	// Parameter: TextureEncodingType encoding
	//************************************
	string GetTexelDecodingFunction(string functionName,TextureEncodingType encoding );

	string GetTexelEncodingFunction(string functionName, TextureEncodingType encoding);

	//************************************
	// Method:    CreateShaderProgramFrmCdeNew
	// FullName:  M3D::ShaderManager::CreateShaderProgramFrmCdeNew
	// Access:    private 
	// Returns:   ShaderProgram *
	// Qualifier: 创建shaderprogram
	// Parameter: string effect_name
	// Parameter: const char * vsh
	// Parameter: const char * fsh
	//************************************
	ShaderProgram * CreateShaderProgramFrmCdeNew(string effect_name, const char * vsh, const char * fsh);

private:
    map<string,ShaderProgram *> shaderList;//!<shader 列表
    RenderAction * m_currentAction;//!<当前的渲染动作
    ShaderStringsMap m_shaderStrMap;//!<
    ShaderMTMap m_SMTMap;//!<
	string m_pcShaderPath;
	string m_esShaderPath;

	/*------------new -----------*/
	vector<string> m_prefixParameters;
	map<string, string> m_currentParameters;	


};

} /* namespace M3D */
#endif /* SHADERMANAGER_H_ */
