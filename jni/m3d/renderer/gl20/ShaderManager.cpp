/*
 * ShaderManager.cpp
 *
 *  Created on: 2016-2-16
 *      Author: Administrator
 */
#include <sstream>
#include "m3d/M3D.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/renderer/gl20/ShaderSrcCode.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/graphics/BaseMaterial.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/graphics/ShaderMaterial.h"
#include "m3d/renderer/LightManager.h"
#include "m3d/graphics/Texture.h"
#include "m3d/renderer/gl20/ShaderChunk.h"
#include "m3d/graphics/MatCapMaterial.h"
#include "m3d/action/RenderAction.h"
#include "m3d/SceneManager.h"
#define SET_SHADER_MAP(name) ShaderStrings s ## name(ShaderSrcCode::name ## Vert(),ShaderSrcCode::name ## Frag());\
	m_shaderStrMap.insert(std::make_pair(name, s ## name));\
	m_SMTMap.insert(std::make_pair(name, SMT_ ## name));
namespace M3D
{

	const string ShaderManager::Base = "Base";
	const string ShaderManager::BaseVertex = "BaseVertex";
	const string ShaderManager::Background = "Background";
	const string ShaderManager::LightWithoutTexture = "LightWithoutTexture";
	const string ShaderManager::LightWithTexture = "LightWithTexture";
	const string ShaderManager::Wireframe = "Wireframe";
	const string ShaderManager::Axis = "Axis";
	const string ShaderManager::Dragger("Dragger");
	const string ShaderManager::XRay("XRay");
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
	const string ShaderManager::NoteEdge("NoteEdge");
	const string ShaderManager::NoteImage("NoteImage");
	const string ShaderManager::CapPlane("CapPlane");
	const string ShaderManager::JewelFront("JewelFront");
	const string ShaderManager::JewelBack("JewelBack");
	const string ShaderManager::JewelFinalQuad("JewelFinalQuad");
	const string ShaderManager::Ring("Ring");
	const string ShaderManager::JewelType("JewelType");
	const string ShaderManager::JewelHighLight("JewelHighLight");
	const string ShaderManager::JewelBlendQuad("JewelBlendQuad");
	const  string ShaderManager::Outline("Outline");
	const  string ShaderManager::GaussianBlur("GaussianBlur");
	const  string ShaderManager::GaussianBlurOutline("GaussianBlurOutline");
	const  string ShaderManager::CombineOutline("CombineOutline");
	const  string ShaderManager::SceneGround("SceneGround");
	const  string ShaderManager::SsaoEffect("SsaoEffect");
	const  string ShaderManager::SsaoBlur("SsaoBlur");
	const  string ShaderManager::SaoEffect("SaoEffect");
	const  string ShaderManager::SaoReconstructCSZ("SaoReconstructCSZ");
	const  string ShaderManager::SaoMinifyCSZ("SaoMinifyCSZ");
	const  string ShaderManager::SaoBlur("SaoBlur");
	/**
	 * @brief 获取name名字的效果
	 * @param name
	 * @return
	 */
	ShaderProgram* ShaderManager::GetEffect(string name)
	{
		map<string, ShaderProgram*>::iterator it = this->shaderList.find(name);
		if (it != this->shaderList.end())
		{
			return it->second;
		}
		else
		{
			ShaderStringsMap::iterator it = this->m_shaderStrMap.find(name);
			if (it != this->m_shaderStrMap.end())
			{
				ShaderProgram* temp = CreateShaderProgramFrmCde(name, (it->second).m_vs, (it->second).m_fs);
				temp->SetType(m_SMTMap[name]);
				temp->SetName(name);
				return temp;
			}
			else
			{
				return NULL;
			}

			//加载外部shader
	//==================================================================================
	//		string vs, fs;
	//#ifdef _WIN32
	//		string vsPath = SVIEW::Parameters::Instance()->m_appWorkPath + m_pcShaderPath + name + ".Vert";
	//		string fsPath = SVIEW::Parameters::Instance()->m_appWorkPath + m_pcShaderPath + name + ".Frag";
	//#else
	//		string vsPath = SVIEW::Parameters::Instance()->m_appWorkPath + m_esShaderPath + name + ".Vert";
	//		string fsPath = SVIEW::Parameters::Instance()->m_appWorkPath + m_esShaderPath + name + ".Frag";
	//#endif
	//
	//			
	//		ShaderProgram* temp = CreateShaderProgramFrmFile(name, vsPath, fsPath);
	//		temp->SetName(name);
	//		return temp;
	//==================================================================================

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
	ShaderProgram * ShaderManager::CreateShaderProgramFrmCde(string effect_name, const char * vsh, const char * fsh)
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
			newShader->SetResourceManager(m_currentAction->GetScene()->GetResourceManager());

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


	}
	void ShaderManager::Init()
	{
		//将shader放在外面的路径
		//========================================================
		m_pcShaderPath = "/data/shader/pc/";
		m_esShaderPath = "/data/shader/es/";
		//========================================================

		string maxPrecision = GetMaxPrecision("highp");
#ifdef HUAWEI
		this->CreateShaderProgramFrmCde(Multilight, ShaderSrcCode::BaseVertexVert(), ShaderSrcCode::BaseVertexFrag())->SetType(
			SMT_MultiLight);
#else
		this->CreateShaderProgramFrmCde(Multilight, ShaderSrcCode::MultilightVert(), ShaderSrcCode::MultilightFrag())->SetType(
			SMT_MultiLight);
#endif
		if (maxPrecision == "highp")
		{
			this->CreateShaderProgramFrmCde(MultilightPerFrag, ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFrag())->SetType(
				SMT_MultilightPerFrag);
			this->CreateShaderProgramFrmCde(Edge, ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFrag())->SetType(
				SMT_Edge);
			this->CreateShaderProgramFrmCde(Quad, ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFrag())->SetType(
				SMT_Quad);
		}
		else
		{
			this->CreateShaderProgramFrmCde(MultilightPerFrag, ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFragMediump())->SetType(
				SMT_MultilightPerFrag);
			this->CreateShaderProgramFrmCde(Edge, ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFragMediump())->SetType(
				SMT_Edge);
			this->CreateShaderProgramFrmCde(Quad, ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFragMediump())->SetType(
				SMT_Quad);
		}
		this->CreateShaderProgramFrmCde(Background, ShaderSrcCode::BackgroundVert(), ShaderSrcCode::BackgroundFrag())->SetType(
			SMT_Background);

		//	this->CreateShaderProgramFrmCde(Wireframe,ShaderSrcCode::WireframeVert(),
		//			ShaderSrcCode::WireframeFrag());

		this->CreateShaderProgramFrmCde(Axis, ShaderSrcCode::AxisVert(), ShaderSrcCode::AxisFrag())->SetType(
			SMT_Axis);

		//	this->CreateShaderProgramFrmCde(Brdf,ShaderSrcCode::BrdfVert(),
		//			ShaderSrcCode::BrdfFrag());
		LOGI("Brdf has finish");
		this->CreateShaderProgramFrmCde(Image, ShaderSrcCode::ImageVert(), ShaderSrcCode::ImageFrag())->SetType(
			SMT_Image);

		//	this->CreateShaderProgramFrmCde(Base, ShaderSrcCode::ModelBaseVert(), ShaderSrcCode::ModelBaseFrag());
		//
		//	this->CreateShaderProgramFrmCde(BaseVertex, ShaderSrcCode::BaseVertexVert(), ShaderSrcCode::BaseVertexFrag())->SetType(
		//			SMT_BaseVertex);

		this->CreateShaderProgramFrmCde(CubeMap, ShaderSrcCode::CubeMapVert(), ShaderSrcCode::CubeMapFrag())->SetType(
			SMT_CubeMap);

		//	this->CreateShaderProgramFrmCde(DepthMap, ShaderSrcCode::ShadowMapVert(), ShaderSrcCode::ShadowMapFrag())->SetType(
		//			SMT_DepthMap);
		//
		//	this->CreateShaderProgramFrmCde(FBODebug, ShaderSrcCode::FBODebugVert(), ShaderSrcCode::FBODebugFrag())->SetType(
		//			SMT_FBODebug);

		//	this->CreateShaderProgramFrmCde(PlaneShadow, ShaderSrcCode::PlaneShadowVert(), ShaderSrcCode::PlaneShadowFrag())->SetType(
		//			SMT_PlaneShadow);
		//	this->CreateShaderProgramFrmCde(Blur, ShaderSrcCode::BlurVert(), ShaderSrcCode::BlurFrag())->SetType(
		//			SMT_Blur);
		//	this->CreateShaderProgramFrmCde(Specular, ShaderSrcCode::SpecularVert(), ShaderSrcCode::SpecularFrag())->SetType(
		//			SMT_Specular);
		//
		//	this->CreateShaderProgramFrmCde(EdgeDetection, ShaderSrcCode::EdgeDetectionVert(), ShaderSrcCode::EdgeDetectionFrag())->SetType(
		//			SMT_EdgeDetection);


		ShaderStrings sMultilight(ShaderSrcCode::MultilightVert(), ShaderSrcCode::MultilightFrag());
		//	m_shaderStrMap[Multilight] =sMultilight;
		m_shaderStrMap.insert(std::make_pair(Multilight, sMultilight));
		m_SMTMap.insert(std::make_pair(Multilight, SMT_MultiLight));

		if (maxPrecision == "highp")
		{
			ShaderStrings sMultilightPerFrag(ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFrag());
			m_shaderStrMap.insert(std::make_pair(MultilightPerFrag, sMultilightPerFrag));
			m_SMTMap.insert(std::make_pair(MultilightPerFrag, SMT_MultilightPerFrag));

			ShaderStrings sEdge(ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFrag());
			m_shaderStrMap.insert(std::make_pair(Edge, sEdge));
			m_SMTMap.insert(std::make_pair(Edge, SMT_Edge));

			ShaderStrings sQuad(ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFrag());
			m_shaderStrMap.insert(std::make_pair(Quad, sQuad));
			m_SMTMap.insert(std::make_pair(Quad, SMT_Quad));
		}
		else
		{
			ShaderStrings sMultilightPerFrag(ShaderSrcCode::MultilightPerFragVert(), ShaderSrcCode::MultilightPerFragFragMediump());
			m_shaderStrMap.insert(std::make_pair(MultilightPerFrag, sMultilightPerFrag));
			m_SMTMap.insert(std::make_pair(MultilightPerFrag, SMT_MultilightPerFrag));

			ShaderStrings sEdge(ShaderSrcCode::EdgeVert(), ShaderSrcCode::EdgeFragMediump());
			m_shaderStrMap.insert(std::make_pair(Edge, sEdge));
			m_SMTMap.insert(std::make_pair(Edge, SMT_Edge));

			ShaderStrings sQuad(ShaderSrcCode::QuadVert(), ShaderSrcCode::QuadFragMediump());
			m_shaderStrMap.insert(std::make_pair(Quad, sQuad));
			m_SMTMap.insert(std::make_pair(Quad, SMT_Quad));
		}
		//	m_shaderStrMap[Multilight] =sMultilight;


		ShaderStrings sBackground(ShaderSrcCode::BackgroundVert(), ShaderSrcCode::BackgroundFrag());
		m_shaderStrMap.insert(std::make_pair(Background, sBackground));
		m_SMTMap.insert(std::make_pair(Background, SMT_Background));

		ShaderStrings sAxis(ShaderSrcCode::AxisVert(), ShaderSrcCode::AxisFrag());
		m_shaderStrMap.insert(std::make_pair(Axis, sAxis));
		m_SMTMap.insert(std::make_pair(Axis, SMT_Axis));


		ShaderStrings sModelDragger(ShaderSrcCode::DraggerVert(), ShaderSrcCode::DraggerFrag());
		m_shaderStrMap.insert(std::make_pair(Dragger, sModelDragger));
		m_SMTMap.insert(std::make_pair(Dragger, ShaderMaterialType::SMT_Dragger));

		ShaderStrings sXRay(ShaderSrcCode::XRayVert(), ShaderSrcCode::XRayFrag());
		m_shaderStrMap.insert(std::make_pair(XRay, sXRay));
		m_SMTMap.insert(std::make_pair(XRay, ShaderMaterialType::SMT_XRay));

		ShaderStrings sImage(ShaderSrcCode::ImageVert(), ShaderSrcCode::ImageFrag());
		m_shaderStrMap.insert(std::make_pair(Image, sImage));
		m_SMTMap.insert(std::make_pair(Image, SMT_Image));

		ShaderStrings sCubeMap(ShaderSrcCode::CubeMapVert(), ShaderSrcCode::CubeMapFrag());
		m_shaderStrMap.insert(std::make_pair(CubeMap, sCubeMap));
		m_SMTMap.insert(std::make_pair(CubeMap, SMT_CubeMap));


		ShaderStrings sDepthMap(ShaderSrcCode::ShadowMapVert(), ShaderSrcCode::ShadowMapFrag());
		m_shaderStrMap.insert(std::make_pair(DepthMap, sDepthMap));
		m_SMTMap.insert(std::make_pair(DepthMap, SMT_DepthMap));


		ShaderStrings sFBODebug(ShaderSrcCode::FBODebugVert(), ShaderSrcCode::FBODebugFrag());
		m_shaderStrMap.insert(std::make_pair(FBODebug, sFBODebug));
		m_SMTMap.insert(std::make_pair(FBODebug, SMT_FBODebug));


		ShaderStrings sPlaneShadow(ShaderSrcCode::PlaneShadowVert(), ShaderSrcCode::PlaneShadowFrag());
		m_shaderStrMap.insert(std::make_pair(PlaneShadow, sPlaneShadow));
		m_SMTMap.insert(std::make_pair(PlaneShadow, SMT_PlaneShadow));


		ShaderStrings sBlur(ShaderSrcCode::BlurVert(), ShaderSrcCode::BlurFrag());
		m_shaderStrMap.insert(std::make_pair(Blur, sBlur));
		m_SMTMap.insert(std::make_pair(Blur, SMT_Blur));


		ShaderStrings sSpecular(ShaderSrcCode::SpecularVert(), ShaderSrcCode::SpecularFrag());
		m_shaderStrMap.insert(std::make_pair(Specular, sSpecular));
		m_SMTMap.insert(std::make_pair(Specular, SMT_Specular));


		ShaderStrings sEdgeDetection(ShaderSrcCode::EdgeDetectionVert(), ShaderSrcCode::EdgeDetectionFrag());
		m_shaderStrMap.insert(std::make_pair(EdgeDetection, sEdgeDetection));
		m_SMTMap.insert(std::make_pair(EdgeDetection, SMT_EdgeDetection));

		SET_SHADER_MAP(NoteEdge);
		SET_SHADER_MAP(NoteImage);
		SET_SHADER_MAP(CapPlane);

		SET_SHADER_MAP(JewelFront);

		SET_SHADER_MAP(JewelBack);

		SET_SHADER_MAP(JewelFinalQuad);

		SET_SHADER_MAP(Ring);

		SET_SHADER_MAP(Dragger);

		SET_SHADER_MAP(JewelType);

		SET_SHADER_MAP(JewelHighLight);

		SET_SHADER_MAP(JewelBlendQuad);

		SET_SHADER_MAP(Outline);

		SET_SHADER_MAP(GaussianBlur);

		SET_SHADER_MAP(GaussianBlurOutline);

		SET_SHADER_MAP(CombineOutline);

		SET_SHADER_MAP(SceneGround);

		SET_SHADER_MAP(SsaoEffect);
		SET_SHADER_MAP(SsaoBlur);
		SET_SHADER_MAP(SaoEffect);
		SET_SHADER_MAP(SaoReconstructCSZ);
		SET_SHADER_MAP(SaoMinifyCSZ);
		SET_SHADER_MAP(SaoBlur);
		/*-----------------------prefixParameters--------------*/
		string parameters[] = { "shaderType","precision","emissiveMap","ambientMap","diffuseMap","specularMap","normalMap","displacementMap","envMap","albedoMap",
			"metallicRoughnessMap","ambientOcclusiontMap","envIrradianceMap","numAllLights","numDirectionalLights","numPointLights","numSpotLights","numHemisphereLights","shadowMapEnabled","shadowMapType"
			,"doubleSided","outputEncoding","diffuseMapEncoding","envMapEncoding","ssao","matcapMap","useClearCoat"
		};
		for (int i = 0; i < 27; i++)
		{
			m_prefixParameters.push_back(parameters[i]);
			m_currentParameters[parameters[i]] = "N";
		}
	}

	ShaderManager::~ShaderManager()
	{
		map<string, ShaderProgram *>::iterator it = shaderList.begin();
		for (; it != shaderList.end(); it++)
		{
			delete (it->second);
		}

		shaderList.clear();
	}

	RenderAction * ShaderManager::GetCurrentAction()
	{
		return m_currentAction;
	}

	void ShaderManager::SetCurrentAction(RenderAction * action)
	{
		m_currentAction = action;
	}

	string ShaderManager::GetMaxPrecision(string precision)
	{
		return "highp";
		GLint vrange[2], frange[2];
		GLint vprecision[2], fprecision[2];
		//TODO MergeAndroid
#ifdef _WIN32
		void * s = glGetShaderPrecisionFormat;
#else
		void * s = NULL;
#endif
		if (!s)
		{
			return "highp";
		}
		if (precision == "highp") {
			glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_HIGH_FLOAT, vrange, vprecision);
			glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, frange, fprecision);
			if (vprecision[0] > 0 &&
				fprecision[0] > 0) {

				return "highp";

			}

			precision = "mediump";

		}
		if (precision == "mediump") {

			glGetShaderPrecisionFormat(GL_VERTEX_SHADER, GL_MEDIUM_FLOAT, vrange, vprecision);
			glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, GL_MEDIUM_FLOAT, frange, fprecision);
			if (vprecision[0] > 0 &&
				fprecision[0] > 0) {

				return "mediump";

			}

		}

		return "lowp";
	}


	/************************************************************************/
	/* new function                                                                     */
	/************************************************************************/


	string ShaderManager::GetProgramCode(BaseMaterial *material)
	{
		int materialType = material->GetMaterialType();
		string temp = "";
		temp += StringHelper::IntToString(materialType);
		temp += ",";
		//如果是shader材质，就要添加shader源码
		if (materialType == MaterialType_Shader)
		{
			ShaderMaterial* shaderMaterial = static_cast<ShaderMaterial*>(material);
			temp += shaderMaterial->VertexShader();
			temp += ",";
			temp += shaderMaterial->FragmentShader();
			temp += ",";
		}

		//添加预定义参数
		for (int i = 1, length = m_prefixParameters.size(); i < length; i++)
		{
			temp += m_currentParameters[m_prefixParameters[i]];
			temp += ",";
		}

		return temp;
	}

	void ShaderManager::CreateParameters(BaseMaterial *material, LightManager* lightManager)
	{
		ResetParameters();
		int materialType = material->GetMaterialType();
		string temp = "";
		m_currentParameters["shaderType"] = StringHelper::IntToString(materialType);


		//material attributes
		switch (materialType)
		{
		case MaterialType_Phong:
		{
			Material* phongMaterial = static_cast<Material*>(material);
			if (phongMaterial)
			{
				m_currentParameters["outputEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(nullptr, phongMaterial->IsGammaOutpute()));
				m_currentParameters["ambientMap"] = phongMaterial->GetAmbientMap() ? "Y" : "N";
				m_currentParameters["diffuseMap"] = phongMaterial->GetDiffuseMap() ? "Y" : "N";
				m_currentParameters["specularMap"] = phongMaterial->GetSpecularMap() ? "Y" : "N";
				m_currentParameters["displacementMap"] = phongMaterial->DisplacementMap() ? "Y" : "N";
				m_currentParameters["emissiveMap"] = phongMaterial->GetEmissiveMap() ? "Y" : "N";
				m_currentParameters["normalMap"] = phongMaterial->GetNormalMap() ? "Y" : "N";
				m_currentParameters["matcapMap"] = phongMaterial->MatcapMap() ? "Y" : "N";
				m_currentParameters["envMap"] = phongMaterial->GetEvnMap() ? "Y" : "N";
				m_currentParameters["diffuseMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(phongMaterial->GetDiffuseMap(), phongMaterial->GetDiffuseMap() ? phongMaterial->GetDiffuseMap()->IsGammaInput() : false));
				m_currentParameters["envMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(phongMaterial->GetEvnMap(), phongMaterial->GetEvnMap() ? phongMaterial->GetEvnMap()->IsGammaInput() : false));

			}
		}

		break;
		case MaterialType_Pbr:
		{
			PbrMaterial* pbrMaterial = static_cast<PbrMaterial*>(material);
			if (pbrMaterial)
			{
				m_currentParameters["outputEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(nullptr, pbrMaterial->IsGammaOutpute()));
				m_currentParameters["albedoMap"] = pbrMaterial->AlbedoMap() ? "Y" : "N";
				m_currentParameters["metallicRoughnessMap"] = pbrMaterial->MetalnessRoughnessMap() ? "Y" : "N";
				m_currentParameters["emissiveMap"] = pbrMaterial->EmissiveMap() ? "Y" : "N";
				m_currentParameters["normalMap"] = pbrMaterial->NormalMap() ? "Y" : "N";
				m_currentParameters["displacementMap"] = pbrMaterial->DisplacementMap() ? "Y" : "N";
				m_currentParameters["envIrradianceMap"] = pbrMaterial->EnvIrradianceMap() || true ? "Y" : "N";
				m_currentParameters["envMap"] = pbrMaterial->EnvMap() || true ? "Y" : "N";
				m_currentParameters["ambientOcclusiontMap"] = pbrMaterial->AmbientOcclusiontMap() ? "Y" : "N";
				m_currentParameters["diffuseMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(pbrMaterial->AlbedoMap(), pbrMaterial->AlbedoMap() ? pbrMaterial->AlbedoMap()->IsGammaInput() : true));
				m_currentParameters["envMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(pbrMaterial->EnvMap(), pbrMaterial->EnvMap() ? pbrMaterial->EnvMap()->IsGammaInput() : true));
				m_currentParameters["useClearCoat"] = pbrMaterial->UseClearCoat()? "Y" : "N";
			}
		}

		break;
		//case MaterialType_Shader:
		//	ShaderMaterial* shaderMaterial = static_cast<ShaderMaterial*>(material);
		//	m_currentParameters["shaderType"]= shaderMaterial->VertexShader();	
		//	m_currentParameters["shaderType"]= shaderMaterial->FragmentShader();
		//	break;
		case MaterialType_MatCap:
		{
			MatCapMaterial* matCapMaterial = static_cast<MatCapMaterial*>(material);
			m_currentParameters["outputEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(nullptr, matCapMaterial->IsGammaOutpute()));
		
			m_currentParameters["normalMap"] = matCapMaterial->GetNormalMap() ? "Y" : "N";
			m_currentParameters["matcapMap"] = matCapMaterial->MatcapMap() ? "Y" : "N";
			m_currentParameters["diffuseMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(matCapMaterial->GetDiffuseMap(), matCapMaterial->GetDiffuseMap() ? matCapMaterial->GetDiffuseMap()->IsGammaInput() : false));
			m_currentParameters["envMapEncoding"] = StringHelper::IntToString(GetTextureEncodingFromMap(matCapMaterial->GetEvnMap(), matCapMaterial->GetEvnMap() ? matCapMaterial->GetEvnMap()->IsGammaInput() : false));
		}
		break;
		}

		//lights number
		m_currentParameters["numDirectionalLights"] = StringHelper::IntToString(lightManager->DirectionalLightNumber());
		m_currentParameters["numPointLights"] = StringHelper::IntToString(lightManager->PointLightNumber());
		m_currentParameters["numSpotLights"] = StringHelper::IntToString(lightManager->SpotLightNumber());
		m_currentParameters["numHemisphereLights"] = StringHelper::IntToString(lightManager->HemisphereLightNumber());
		m_currentParameters["numAllLights"] = StringHelper::IntToString(
			lightManager->DirectionalLightNumber()+
			lightManager->PointLightNumber()+
			lightManager->SpotLightNumber()+
			lightManager->HemisphereLightNumber()
		);
		m_currentParameters["shadowMapEnabled"] = SVIEW::Parameters::Instance()->m_shadowMapEnabled ? "Y" : "N";
		//m_currentParameters["shadowMapType"] = SVIEW::Parameters::Instance()->m_shadowMapType==2?"SHADOWMAP_TYPE_PCF":
		//	(SVIEW::Parameters::Instance()->m_shadowMapType==1 ? " SHADOWMAP_TYPE_BASE" : "SHADOWMAP_TYPE_PCF_SOFT");
		m_currentParameters["doubleSided"] = SVIEW::Parameters::Instance()->m_doubleSided ? "Y" : "N";
		m_currentParameters["precision"] = "highp";
		m_currentParameters["ssao"] = SVIEW::Parameters::Instance()->m_useSSAO ? "Y" : "N";


	}



	void ShaderManager::GetPrefixShader(string& prefixVertex, string& prefixFragment)
	{
		prefixVertex = "";
		prefixFragment = "";

		//--------------------vertex-------------------------------
		prefixVertex;
	}

	ShaderProgram* ShaderManager::CreateShaderProgram(BaseMaterial* material, string &vsh, string& fsh, string& code)
	{
		ShaderProgram* ret = nullptr;
		string envTextureTypeDefine = "ENVTEXTURE_TYPE_CUBE", envTextureModeDefine = "ENVTEXTURE_MODE_REFLECTION";
		//获取环境贴图预定义宏
		SetEnvTextureDefine(material, envTextureTypeDefine, envTextureModeDefine);
		//获取扩展宏
		string customExtensions = GenerateExtensions(m_currentParameters);

		string gammaFactorDefine = "2.2\n";

		//获取材质预定义宏
		//TODO

		//获取预定义着色器源码
		string prefixVertex = "", prefixFragment = "";
		string shadowType = (SVIEW::Parameters::Instance()->m_shadowMapType == 2 ? "SHADOWMAP_TYPE_PCF\n" :
			(SVIEW::Parameters::Instance()->m_shadowMapType == 1 ? " SHADOWMAP_TYPE_BASE\n" : "SHADOWMAP_TYPE_PCF_SOFT\n"));
		prefixVertex =
#ifndef WIN32
			"precision " + m_currentParameters["precision"] + " float;\n" +
			"precision " + m_currentParameters["precision"] + " int;\n" +
#endif


			"#define SHADER_NAME " + m_currentParameters["shaderType"] + "\n" +

			customExtensions + "\n" +
			material->Define() +

			"#define GAMMA_FACTOR " + gammaFactorDefine + "\n" +
			(m_currentParameters["diffuseMap"] != "N" || m_currentParameters["albedoMap"] != "N" ? "#define USE_DIFFUSEMAP\n" : "") +
			(m_currentParameters["envMap"] != "N" ? "#define USE_ENV_TEXTURE\n" : "") +
			(m_currentParameters["envMap"] != "N" ? "#define " + envTextureModeDefine + "\n" : "") +
			(m_currentParameters["ambientOcclusiontMap"] != "N" ? "#define USE_AOMAP\n" : "") +
			(m_currentParameters["emissiveMap"] != "N" ? "#define USE_EMISSIVEMAP\n" : "") +
			(m_currentParameters["normalMap"] != "N" ? "#define USE_NORMALMAP\n" : "") +
			(m_currentParameters["displacementMap"] != "N" ? "#define USE_DISPLACEMENTMAP\n" : "") +
			(m_currentParameters["specularMap"] != "N" ? "#define USE_SPECULARMAP\n" : "") +
			(m_currentParameters["metallicRoughnessMap"] != "N" ? "#define USE_METALLIC_ROUGHNESS_TEXTURE\n" : "") +
			(m_currentParameters["doubleSided"] != "N" ? "#define DOUBLE_SIDED\n" : "") +
			(m_currentParameters["shadowMapEnabled"] != "N" && m_currentParameters["numAllLights"] != "0" ? "#define USE_SHADOWMAP\n" : "") +
			(m_currentParameters["shadowMapEnabled"] != "N" && m_currentParameters["numAllLights"] != "0" ? "#define " + shadowType + "\n" : "") +
			"uniform mat4 modelMatrix;\n" +
			"uniform mat4 u_worldNormalMat;\n" +
			"uniform mat4 projectionMatrix;\n" +
			"uniform mat4 viewMatrix;\n" +
			"uniform mat4 normalMatrix;\n" +
			"uniform mat4 u_textureMat;\n" +
			"uniform vec3 cameraPosition;\n" +

			"attribute vec3 a_position;\n" +
			"attribute vec3 a_normal;\n" +
			"attribute vec3 a_texCoords;\n" +
			"attribute vec3 a_center;\n"
			;
		prefixFragment =
			customExtensions + "\n" +
#ifndef WIN32
			"precision " + m_currentParameters["precision"] + " float;\n" +
			"precision " + m_currentParameters["precision"] + " int;\n" +
#endif
			material->Define() +
			"#define USE_CLIP\n" +
			"#define SHADER_NAME " + m_currentParameters["shaderType"] + "\n" +
			"#define GAMMA_FACTOR " + gammaFactorDefine + "\n" +
			(m_currentParameters["diffuseMap"] != "N" || m_currentParameters["albedoMap"] != "N" ? "#define USE_DIFFUSEMAP\n" : "") +
			(m_currentParameters["envMap"] != "N" ? "#define USE_ENV_TEXTURE\n" : "") +
			(m_currentParameters["envMap"] != "N" ? "#define " + envTextureTypeDefine + "\n" : "") +
			(m_currentParameters["envMap"] != "N" ? "#define " + envTextureModeDefine + "\n" : "") +
			(m_currentParameters["ambientOcclusiontMap"] != "N" ? "#define USE_AOMAP\n" : "") +
			(m_currentParameters["emissiveMap"] != "N" ? "#define USE_EMISSIVEMAP\n" : "") +
			(m_currentParameters["normalMap"] != "N" ? "#define USE_NORMALMAP\n" : "") +
			(m_currentParameters["matcapMap"] != "N" ? "#define USE_MATCAPMAP\n" : "") +
			(m_currentParameters["specularMap"] != "N" ? "#define USE_SPECULARMAP\n" : "") +
			(m_currentParameters["metallicRoughnessMap"] != "N" ? "#define USE_METALLIC_ROUGHNESS_TEXTURE\n" : "") +
			(m_currentParameters["doubleSided"] != "N" ? "#define DOUBLE_SIDED\n" : "") +
			(m_currentParameters["ssao"] != "N" ? "#define SSAO\n" : "") +
			(m_currentParameters["shadowMapEnabled"] != "N" && m_currentParameters["numAllLights"] != "0" ? "#define USE_SHADOWMAP\n" : "") +
			(m_currentParameters["shadowMapEnabled"] != "N" && m_currentParameters["numAllLights"] != "0" ? "#define " + shadowType + "\n" : "") +
			(m_currentParameters["envMap"] != "N" && (RenderContext::CheckExtension("GL_ARB_shader_texture_lod", glGetString(GL_EXTENSIONS)) == 1) ?
				"#define TEXTURE_LOD_EXT\n" : "") +
				((m_currentParameters["outputEncoding"] != "N" || m_currentParameters["diffuseMapEncoding"] != "N" || m_currentParameters["envMap"] != "N") ? (ShaderChunk::GetShaderChunks())["encodings_pars_fragment"] : "")
			+
			(m_currentParameters["diffuseMapEncoding"] != "N" ? GetTexelDecodingFunction("mapTexelToLinear", (TextureEncodingType)StringHelper::StringToInt(m_currentParameters["diffuseMapEncoding"])) : "")
			+
			(m_currentParameters["envMap"] != "N" ? GetTexelDecodingFunction("envMapTexelToLinear", (TextureEncodingType)StringHelper::StringToInt(m_currentParameters["envMapEncoding"])) : "")
			+
			(m_currentParameters["outputEncoding"] != "N" ? GetTexelEncodingFunction("linearToOutputTexel", (TextureEncodingType)StringHelper::StringToInt(m_currentParameters["outputEncoding"])) : "")
			+
			//"#define PHYSICALLY_CORRECT_LIGHTS\n"+
			"uniform vec3 cameraPosition;\n" +
			"uniform vec4 u_selectColor;\n" +
			"uniform mat4 viewMatrix;\n" +
			"uniform bool u_enableWireframe;\n" +
			"uniform bool u_enableOnlyWireframe;\n"
			;
		string vertexShader = "", fragmentShader = "";
		ParseIncludes(vertexShader, vsh, ShaderChunk::GetShaderChunks());
		ReplaceLightNums(vertexShader, m_currentParameters);

		ParseIncludes(fragmentShader, fsh, ShaderChunk::GetShaderChunks());
		ReplaceLightNums(fragmentShader, m_currentParameters);

		string vectexGlsl = prefixVertex + vertexShader;
		string fragmentGlsl = prefixFragment + fragmentShader;
		//LOGI("%s",vectexGlsl.c_str());
		//LOGI("*************************************");
		//LOGI("%s",fragmentGlsl.c_str());
		ret = CreateShaderProgramFrmCdeNew(code, vectexGlsl.c_str(), fragmentGlsl.c_str());
		return ret;
	}

	ShaderProgram* ShaderManager::AcquireProgram(BaseMaterial* material, string &vsh, string& fsh, string &code)
	{
		ShaderProgram * shaderProgram = nullptr;
		map<string, ShaderProgram*>::iterator it = this->shaderList.find(
			code);
		if (it == this->shaderList.end())
		{
			shaderProgram = CreateShaderProgram(material, vsh, fsh, code);
		}
		else
		{
			shaderProgram = it->second;
		}
		return shaderProgram;
	}


	void ShaderManager::ResetParameters()
	{
		for (int i = 0, length = m_prefixParameters.size(); i < length; i++)
		{
			m_currentParameters[m_prefixParameters[i]] = "N";
		}
	}

	string ShaderManager::GenerateExtensions(map<string, string>&parameters)
	{
		string ret = "";
		const GLubyte* extStr = glGetString(GL_EXTENSIONS);
		ret += (parameters["envMap"] == "Y" && RenderContext::CheckExtension("GL_ARB_shader_texture_lod", extStr) == 1) ? "#extension GL_ARB_shader_texture_lod : enable\n" : "";
		ret += (parameters["normalMap"] == "Y") ? "#extension GL_OES_standard_derivatives : enable\n" : "";
		return ret;
	}

	void ShaderManager::ReplaceLightNums(string& src, map<string, string>& parameters)
	{
		StringHelper::Replace_all(src, "NUM_DIR_LIGHTS", parameters["numDirectionalLights"]);
		StringHelper::Replace_all(src, "NUM_SPOT_LIGHTS", parameters["numSpotLights"]);
		StringHelper::Replace_all(src, "NUM_POINT_LIGHTS", parameters["numPointLights"]);
		StringHelper::Replace_all(src, "NUM_HEMI_LIGHTS", parameters["numHemisphereLights"]);
	}

	bool ShaderManager::ParseIncludes(string& code, string& src, map<string, string>&lib)
	{
		std::stringstream ss(src);
		while (!ss.eof())
		{
			char buffer[1024];
			ss.getline(buffer, 1024, '\n');
			string line(buffer);
			string::size_type pos(0);
			if ((pos = line.find("#include ")) != string::npos)
			{
				string::size_type pos2 = line.find(">");
				string name = line.substr(pos + 10, pos2 - (pos + 10));
				string includeStr = lib[name];

				//TODO

				if (!ParseIncludes(code, includeStr, lib))
				{
					return false;
				}
			}
			else if (line != "")
			{
				code += line;
				code += "\n";
			}
		}
		//	code += "\n";
		return true;
	}

	void ShaderManager::SetEnvTextureDefine(BaseMaterial*material, string& envMapTypeDefine, string& envMapModeDefine)
	{
		if (m_currentParameters["envMap"] == "Y")
		{
			if (material->GetMaterialType() == MaterialType_Phong)
			{
				Material* phongMaterial = static_cast<Material*>(material);

				switch (phongMaterial->EnvTextureMapping()) {

				case CubeReflectionMapping:
				case CubeRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_CUBE";
					break;

				case CubeUVReflectionMapping:
				case CubeUVRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_CUBE_UV";
					break;

				case EquirectangularReflectionMapping:
				case EquirectangularRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_EQUIREC";
					break;

				case SphericalReflectionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_SPHERE";
					break;

				}

				switch (phongMaterial->EnvTextureMapping()) {

				case CubeRefractionMapping:
				case EquirectangularRefractionMapping:
					envMapModeDefine = "ENVTEXTURE_MODE_REFRACTION";
					break;

				}
			}
			else if (material->GetMaterialType() == MaterialType_Pbr)
			{
				PbrMaterial* pbrMaterial = static_cast<PbrMaterial*>(material);

				switch (pbrMaterial->EnvTextureMapping()) {

				case CubeReflectionMapping:
				case CubeRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_CUBE";
					break;

				case CubeUVReflectionMapping:
				case CubeUVRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_CUBE_UV";
					break;

				case EquirectangularReflectionMapping:
				case EquirectangularRefractionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_EQUIREC";
					break;

				case SphericalReflectionMapping:
					envMapTypeDefine = "ENVTEXTURE_TYPE_SPHERE";
					break;

				}

				switch (pbrMaterial->EnvTextureMapping()) {

				case CubeRefractionMapping:
				case EquirectangularRefractionMapping:
					envMapModeDefine = "ENVTEXTURE_MODE_REFRACTION";
					break;

				}
			}
		}
	}

	int ShaderManager::GetTextureEncodingFromMap(Texture* texture, bool gammaOverrideLinear)
	{
		int encoding = TEXEL_ENCODING_TYPE_GAMMA;
		if (!texture)
		{
			encoding = TEXEL_ENCODING_TYPE_LINEAR;
		}
		else if (texture)
		{
			encoding = texture->TextureEncoding();
		}

		if (encoding == TEXEL_ENCODING_TYPE_LINEAR && gammaOverrideLinear)
		{
			encoding = TEXEL_ENCODING_TYPE_GAMMA;
		}

		return encoding;
	}

	vector<string> ShaderManager::GetEncodingCompents(TextureEncodingType encoding)
	{
		vector<string> ret;
		switch (encoding) {

		case TEXEL_ENCODING_TYPE_LINEAR:
			ret.push_back("Linear"); ret.push_back("( value )");
		case TEXEL_ENCODING_TYPE_SRGB:
			ret.push_back("sRGB"); ret.push_back("( value )");
		case TEXEL_ENCODING_TYPE_RGBE:
			ret.push_back("RGBE"); ret.push_back("( value )");
		case TEXEL_ENCODING_TYPE_RGBM7:
			ret.push_back("RGBM"); ret.push_back("( value, 7.0 )");
		case TEXEL_ENCODING_TYPE_RGBM16:
			ret.push_back("RGBM"); ret.push_back("( value, 16.0 )");
		case TEXEL_ENCODING_TYPE_RGBD:
			ret.push_back("RGBD"); ret.push_back("( value, 256.0 )");
		case TEXEL_ENCODING_TYPE_GAMMA:
			ret.push_back("Gamma"); ret.push_back("( value, float( GAMMA_FACTOR ) )");
		default:
			;
			return ret;

		}
	}

	string ShaderManager::GetTexelDecodingFunction(string functionName, TextureEncodingType encoding)
	{
		vector<string> compents = GetEncodingCompents(encoding);
		return "vec4 " + functionName + "( vec4 value )\n{\n return " + compents[0] + "ToLinear" + compents[1] + ";\n}\n";
	}

	string ShaderManager::GetTexelEncodingFunction(string functionName, TextureEncodingType encoding)
	{
		vector<string> compents = GetEncodingCompents(encoding);
		return "vec4 " + functionName + "( vec4 value )\n{\n return LinearTo" + compents[0] + compents[1] + ";\n}\n";
	}

	ShaderProgram * ShaderManager::CreateShaderProgramFrmCdeNew(string effect_name, const char * vsh, const char * fsh)
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
			newShader->SetResourceManager(m_currentAction->GetScene()->GetResourceManager());
			if (newShader->LinkProgram())
			{
				this->shaderList.insert(
					std::make_pair(string(effect_name), newShader));
				newShader->SetName(effect_name);
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
		return NULL;
	}

} /* namespace M3D */
