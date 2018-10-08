/*
 * ShaderManager.cpp
 *
 *  Created on: 2016-2-16
 *      Author: Administrator
 */
#include "m3d/M3D.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/renderer/gl20/ShaderSrcCode.h"

namespace M3D
{

const string ShaderManager::Base = "Base";
const string ShaderManager::BaseVertex ="BaseVertex" ;
const string ShaderManager::Background = "Background";
const string ShaderManager::LightWithoutTexture = "LightWithoutTexture";
const string ShaderManager::LightWithTexture = "LightWithTexture";
const string ShaderManager::Wireframe = "Wireframe";
const string ShaderManager::Axis = "Axis";
const string ShaderManager::Edge("Edge");
const string ShaderManager::Brdf("Brdf");
const string ShaderManager::Image("Image");
const  string ShaderManager::Multilight("Multilight");
const  string ShaderManager::CubeMap("CubeMap");
const string  ShaderManager::DepthMap("DepthMap");
const string  ShaderManager::FBODebug("FBODebug");
const string  ShaderManager::PlaneShadow("PlaneShadow");
const string  ShaderManager::Blur("Blur");
const string  ShaderManager::Specular("Specular");
const  string ShaderManager::EdgeDetection("EdgeDetection");
const string ShaderManager::Quad("Quad");
const string ShaderManager::MultilightPerFrag("MultilightPerFrag");
/**
 * @brief 获取name名字的效果
 * @param name
 * @return
 */
ShaderProgram* ShaderManager::GetEffect(string name)
{
    map<string,ShaderProgram*>::iterator it = this->shaderList.find(name);
    if(it != this->shaderList.end() )
    {
        return it->second;
    }
    else
    {
    	ShaderStringsMap::iterator it = this->m_shaderStrMap.find(name);
        if(it != this->m_shaderStrMap.end() )
        {
        	ShaderProgram* temp = CreateShaderProgramFrmCde(name,(it->second).m_vs,(it->second).m_fs);
        	temp->SetType(m_SMTMap[name]);
        	return temp;
        }
        else
        {
        	return NULL;
        }
    }
}

/**
 * @brief 从文件中获取shader源码，并返回链接后的shaderProgram
 * @param effect_name
 * @param vsh
 * @param fsh
 * @return
 */
ShaderProgram* ShaderManager::CreateShaderProgramFrmFile(string effect_name,
		string vsh, string fsh)
{
	map<string, ShaderProgram*>::iterator it = this->shaderList.find(
			effect_name);
	if (it == this->shaderList.end())
	{
		Shader * vertexShader = new Shader(GL_VERTEX_SHADER);
		Shader * fragmentShader = new Shader(GL_FRAGMENT_SHADER);
		vertexShader->CompileSourceFile(vsh);
		fragmentShader->CompileSourceFile(fsh);
		ShaderProgram * newShader = new ShaderProgram(vertexShader,
				fragmentShader);
		if (newShader->LinkProgram())
		{
			this->shaderList.insert(
					std::make_pair(string(effect_name), newShader));
			return newShader;
		}
		else
		{
			LOGI("Shaer Create Error！！");
			return NULL;
		}
	}
	else
	{

		return it->second;
	}
}

/**
 * @brief 从字符串中获取shader源码，并返回链接后的shaderProgram
 * @param effect_name
 * @param vsh
 * @param fsh
 * @return
 */
ShaderProgram * ShaderManager::CreateShaderProgramFrmCde(string effect_name, const char * vsh,  const char * fsh)
{
	map<string, ShaderProgram*>::iterator it = this->shaderList.find(
			effect_name);
	if (it == this->shaderList.end())
	{
		Shader * vertexShader = new Shader(GL_VERTEX_SHADER);
		Shader * fragmentShader = new Shader(GL_FRAGMENT_SHADER);
		LOGI("comepile shader");
		vertexShader->CompileSourceCode(vsh);
		fragmentShader->CompileSourceCode(fsh);
		LOGI("comepile shader end");
		ShaderProgram * newShader = new ShaderProgram(vertexShader,
				fragmentShader);

		if (newShader->LinkProgram())
		{
			this->shaderList.insert(
					std::make_pair(string(effect_name), newShader));
			delete vertexShader;
			vertexShader = NULL;
			delete fragmentShader;
			fragmentShader = NULL;
			return newShader;
		}
		else
		{
			LOGI("Shaer Create Error！！");
			return NULL;
		}
	}
	else
	{

		return it->second;
	}
}

/**
 * @brife 当构造shaderManager时，生成预定义的效果
 */
ShaderManager::ShaderManager()
{
#ifdef HUAWEI
	this->CreateShaderProgramFrmCde(Multilight, ShaderSrcCode::BaseVertexVert(), ShaderSrcCode::BaseVertexFrag())->SetType(
			ShaderMaterialType::SMT_MultiLight);
#else
	this->CreateShaderProgramFrmCde(Multilight, ShaderSrcCode::MultilightVert(), ShaderSrcCode::MultilightFrag())->SetType(
			ShaderMaterialType::SMT_MultiLight);
#endif
	this->CreateShaderProgramFrmCde(MultilightPerFrag, ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFrag())->SetType(
		ShaderMaterialType::SMT_MultilightPerFrag);
	this->CreateShaderProgramFrmCde(Background, ShaderSrcCode::BackgroundVert(), ShaderSrcCode::BackgroundFrag())->SetType(
			ShaderMaterialType::SMT_Background);

//	this->CreateShaderProgramFrmCde(Wireframe,ShaderSrcCode::WireframeVert(),
//			ShaderSrcCode::WireframeFrag());

	this->CreateShaderProgramFrmCde(Axis, ShaderSrcCode::AxisVert(), ShaderSrcCode::AxisFrag())->SetType(
			ShaderMaterialType::SMT_Axis);

	this->CreateShaderProgramFrmCde(Edge, ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFrag())->SetType(
			ShaderMaterialType::SMT_Edge);

//	this->CreateShaderProgramFrmCde(Brdf,ShaderSrcCode::BrdfVert(),
//			ShaderSrcCode::BrdfFrag());
	LOGI("Brdf has finish");
	this->CreateShaderProgramFrmCde(Image, ShaderSrcCode::ImageVert(), ShaderSrcCode::ImageFrag())->SetType(
			ShaderMaterialType::SMT_Image);

//	this->CreateShaderProgramFrmCde(Base, ShaderSrcCode::ModelBaseVert(), ShaderSrcCode::ModelBaseFrag());
//
//	this->CreateShaderProgramFrmCde(BaseVertex, ShaderSrcCode::BaseVertexVert(), ShaderSrcCode::BaseVertexFrag())->SetType(
//			ShaderMaterialType::SMT_BaseVertex);

	this->CreateShaderProgramFrmCde(CubeMap, ShaderSrcCode::CubeMapVert(), ShaderSrcCode::CubeMapFrag())->SetType(
			ShaderMaterialType::SMT_CubeMap);

	this->CreateShaderProgramFrmCde(Quad, ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFrag())->SetType(
			ShaderMaterialType::SMT_Quad);

//	this->CreateShaderProgramFrmCde(DepthMap, ShaderSrcCode::ShadowMapVert(), ShaderSrcCode::ShadowMapFrag())->SetType(
//			ShaderMaterialType::SMT_DepthMap);
//
//	this->CreateShaderProgramFrmCde(FBODebug, ShaderSrcCode::FBODebugVert(), ShaderSrcCode::FBODebugFrag())->SetType(
//			ShaderMaterialType::SMT_FBODebug);

//	this->CreateShaderProgramFrmCde(PlaneShadow, ShaderSrcCode::PlaneShadowVert(), ShaderSrcCode::PlaneShadowFrag())->SetType(
//			ShaderMaterialType::SMT_PlaneShadow);
//	this->CreateShaderProgramFrmCde(Blur, ShaderSrcCode::BlurVert(), ShaderSrcCode::BlurFrag())->SetType(
//			ShaderMaterialType::SMT_Blur);
//	this->CreateShaderProgramFrmCde(Specular, ShaderSrcCode::SpecularVert(), ShaderSrcCode::SpecularFrag())->SetType(
//			ShaderMaterialType::SMT_Specular);
//
//	this->CreateShaderProgramFrmCde(EdgeDetection, ShaderSrcCode::EdgeDetectionVert(), ShaderSrcCode::EdgeDetectionFrag())->SetType(
//			ShaderMaterialType::SMT_EdgeDetection);


	ShaderStrings sMultilight(ShaderSrcCode::MultilightVert(), ShaderSrcCode::MultilightFrag());
//	m_shaderStrMap[Multilight] =sMultilight;
	m_shaderStrMap.insert(std::make_pair(Multilight,sMultilight));
	m_SMTMap.insert(std::make_pair(Multilight,ShaderMaterialType::SMT_MultiLight));

	ShaderStrings sMultilightPerFrag(ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFrag());
	//	m_shaderStrMap[Multilight] =sMultilight;
	m_shaderStrMap.insert(std::make_pair(MultilightPerFrag, sMultilightPerFrag));
	m_SMTMap.insert(std::make_pair(MultilightPerFrag, ShaderMaterialType::SMT_MultilightPerFrag));

	ShaderStrings sBackground(ShaderSrcCode::BackgroundVert(), ShaderSrcCode::BackgroundFrag());
	m_shaderStrMap.insert(std::make_pair(Background,sBackground));
	m_SMTMap.insert(std::make_pair(Background,ShaderMaterialType::SMT_Background));

	ShaderStrings sAxis(ShaderSrcCode::AxisVert(), ShaderSrcCode::AxisFrag());
	m_shaderStrMap.insert(std::make_pair(Axis,sAxis));
	m_SMTMap.insert(std::make_pair(Axis,ShaderMaterialType::SMT_Axis));

	ShaderStrings sEdge(ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFrag());
	m_shaderStrMap.insert(std::make_pair(Edge,sEdge));
	m_SMTMap.insert(std::make_pair(Edge,ShaderMaterialType::SMT_Edge));

	ShaderStrings sImage(ShaderSrcCode::ImageVert(), ShaderSrcCode::ImageFrag());
	m_shaderStrMap.insert(std::make_pair(Image,sImage));
	m_SMTMap.insert(std::make_pair(Image,ShaderMaterialType::SMT_Image));

	ShaderStrings sCubeMap(ShaderSrcCode::CubeMapVert(), ShaderSrcCode::CubeMapFrag());
	m_shaderStrMap.insert(std::make_pair(CubeMap,sCubeMap));
	m_SMTMap.insert(std::make_pair(CubeMap,ShaderMaterialType::SMT_CubeMap));


	ShaderStrings sDepthMap(ShaderSrcCode::ShadowMapVert(), ShaderSrcCode::ShadowMapFrag());
	m_shaderStrMap.insert(std::make_pair(DepthMap,sDepthMap));
	m_SMTMap.insert(std::make_pair(DepthMap,ShaderMaterialType::SMT_DepthMap));


	ShaderStrings sFBODebug(ShaderSrcCode::FBODebugVert(), ShaderSrcCode::FBODebugFrag());
	m_shaderStrMap.insert(std::make_pair(FBODebug,sFBODebug));
	m_SMTMap.insert(std::make_pair(FBODebug,ShaderMaterialType::SMT_FBODebug));


	ShaderStrings sPlaneShadow(ShaderSrcCode::PlaneShadowVert(), ShaderSrcCode::PlaneShadowFrag());
	m_shaderStrMap.insert(std::make_pair(PlaneShadow,sPlaneShadow));
	m_SMTMap.insert(std::make_pair(PlaneShadow,ShaderMaterialType::SMT_PlaneShadow));


	ShaderStrings sBlur(ShaderSrcCode::BlurVert(), ShaderSrcCode::BlurFrag());
	m_shaderStrMap.insert(std::make_pair(Blur,sBlur));
	m_SMTMap.insert(std::make_pair(Blur,ShaderMaterialType::SMT_Blur));


	ShaderStrings sSpecular(ShaderSrcCode::SpecularVert(), ShaderSrcCode::SpecularFrag());
	m_shaderStrMap.insert(std::make_pair(Specular,sSpecular));
	m_SMTMap.insert(std::make_pair(Specular,ShaderMaterialType::SMT_Specular));


	ShaderStrings sEdgeDetection(ShaderSrcCode::EdgeDetectionVert(), ShaderSrcCode::EdgeDetectionFrag());
	m_shaderStrMap.insert(std::make_pair(EdgeDetection,sEdgeDetection));
	m_SMTMap.insert(std::make_pair(EdgeDetection,ShaderMaterialType::SMT_EdgeDetection));

	ShaderStrings sQuad(ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFrag());
	m_shaderStrMap.insert(std::make_pair(Quad,sQuad));
	m_SMTMap.insert(std::make_pair(Quad,ShaderMaterialType::SMT_Quad));

}

ShaderManager::~ShaderManager()
{
	map<string,ShaderProgram *>::iterator it = shaderList.begin();
	for(;it != shaderList.end();it ++)
	{
		delete (it->second);
	}

	shaderList.clear();
}

RenderAction * ShaderManager::GetCurrentAction()
{
	return m_currentAction;
}

void ShaderManager::SetCurrentAction(RenderAction * action )
{
	m_currentAction = action;
}

} /* namespace M3D */
