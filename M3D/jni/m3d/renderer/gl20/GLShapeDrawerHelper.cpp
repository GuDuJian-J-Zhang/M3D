/*
 * GLShapeDrawerHelper.cpp
 *
 *  Created on: 2016-3-23
 *      Author: CDJ
 */

#include "m3d/renderer/GL20/GLShapeDrawerHelper.h"
#include "m3d/graphics/Light.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/GraphicsDefs.h"
namespace M3D
{
GLShapeDrawerHelper::GLShapeDrawerHelper()
{


}

GLShapeDrawerHelper::~GLShapeDrawerHelper()
{

}

void GLShapeDrawerHelper::SetLightUniform(ShaderProgram* shaderEffect,
		Light* light,string index)
{
	string front= "u_lights[";
	string name = "";
	if (light && light->LightState())
	{
		name = front+index+string("].ambient");
		shaderEffect->SetUniformValue(name, light->GetAmbient());
		name = front+index+string("].diffuse");
		shaderEffect->SetUniformValue(name, light->GetDiffuse());
		name = front+index+string("].specular");
		shaderEffect->SetUniformValue(name, light->GetSpecular());
		name = front+index+string("].position");
		shaderEffect->SetUniformValue(name, light->GetPosition());
		name = front+index+string("].spotDirection");
		shaderEffect->SetUniformValue(name, light->GetSpotDirection());
		name = front+index+string("].spotExponent");
		shaderEffect->SetUniformValue(name, 1, & light->GetSpotExponent());
		name = front+index+string("].spotCutoff");
		shaderEffect->SetUniformValue(name, 1, &light->GetSpotCutoff());
		name = front+index+string("].spotCosCutoff");
		shaderEffect->SetUniformValue(name, 1, &light->GetSpotCosCutoff());
		name = front+index+string("].constant");
		shaderEffect->SetUniformValue(name, 1, &light->GetConstantAttenuation());
		name = front+index+string("].linear");
		shaderEffect->SetUniformValue(name, 1, &light->GetLinearAttenuation());
		name = front+index+string("].quadratic");
		shaderEffect->SetUniformValue(name, 1, &light->GetQuadraticAttenuation());
		name = front+index+string("].intensity");
		Vector3 intensityVec(light->GetSpecularIntensity(),0.0,0.0);
		shaderEffect->SetUniformValue(name, intensityVec);
		name = string("u_lightModel.ambient");
		shaderEffect->SetUniformValue(name, light->GetLightModelAmbient());
	}
}

ShaderProgram* GLShapeDrawerHelper::GetShaderProgram(RenderAction* action, int ShaderMaterialType)
{
	ShaderProgram * shaderEffect = NULL;
	ShaderManager* shaderMananger = action->GetShaderMananger();
	switch (ShaderMaterialType)
	{
	case SMT_MultiLight:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::Multilight);
		break;
	case SMT_Brdf:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::Brdf);
		break;
	case SMT_DepthMap:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::DepthMap);
		break;
	case SMT_PlaneShadow:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::PlaneShadow);
		break;
	case SMT_Blur:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::Blur);
		break;
	case SMT_Specular:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::Specular);
		break;
	case SMT_Edge:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::Edge);
		break;
	case SMT_MultilightPerFrag:
		shaderEffect = shaderMananger->GetEffect(ShaderManager::MultilightPerFrag);
		break;
	default:
		LOGI("Unknown material type.");
		break;
	}

	shaderEffect->UseProgram();
	return shaderEffect;
}

void GLShapeDrawerHelper::SetMaterialUniform(ShaderProgram* shaderEffect, Material* material)
{
	const Color &ambient = material->GetAmbient();
	const Color& diffuse = material->GetDiffuse();
	const Color &spec = material->GetSpecular();
	float shininess = material->GetShininess();
	shininess = 20.0f;
	Color emission(0.01, 0.01, 0.01, 1.0);
	Color ambient1(0.1,0.1,0.1);
//	shaderEffect->SetUniformValue(FSP_DIFFUSE, diffuse);
	shaderEffect->SetUniformValue(FSP_MATERIAL_EMISSION, emission);
	shaderEffect->SetUniformValue(FSP_MATERIAL_AMBIENT, ambient1);
	shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, diffuse);
	shaderEffect->SetUniformValue(FSP_MATERIAL_SPECULAR, spec);
	shaderEffect->SetUniformValue(FSP_MATERIAL_SHININESS, 1, &shininess);

}
}
