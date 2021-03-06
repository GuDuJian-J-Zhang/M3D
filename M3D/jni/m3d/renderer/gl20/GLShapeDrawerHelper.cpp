/*
 * GLShapeDrawerHelper.cpp
 *
 *  Created on: 2016-3-23
 *      Author: CDJ
 */

#include "m3d/renderer/GL20/GLShapeDrawerHelper.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{
GLShapeDrawerHelper::GLShapeDrawerHelper()
{


}

GLShapeDrawerHelper::~GLShapeDrawerHelper()
{

}

void GLShapeDrawerHelper::SetLightUniform(ShaderProgram* shaderEffect,
		DirectionalLight* light,string index)
{
	string front= "u_lights[";
	string name = "";
	if (light && light->IsVisible())
	{
		name = front+index+string("].ambient");
		shaderEffect->SetUniformValue(name, light->GetAmbient());
		name = front+index+string("].diffuse");
		shaderEffect->SetUniformValue(name, light->GetDiffuse());
		name = front+index+string("].specular");
		shaderEffect->SetUniformValue(name, light->GetSpecular());
		name = front + index + string("].position");
		shaderEffect->SetUniformValue(name, light->GetPositionOld());
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

void GLShapeDrawerHelper::SetLightUniform(ShaderProgram* shaderEffect, DirectionalLight * light)
{
	string name = "";
	if (light && light->IsVisible())
	{
		name = string("diffuse");
		shaderEffect->SetUniformValue(name, light->GetDiffuse());
		name = string("specular");
		shaderEffect->SetUniformValue(name, light->GetSpecular());
		name = string("position");
		shaderEffect->SetUniformValue(name, light->GetPositionOld());
		name = string("spotDirection");
		shaderEffect->SetUniformValue(name, light->GetSpotDirection());
		name = string("spotExponent");
		shaderEffect->SetUniformValue(name, 1, &light->GetSpotExponent());
		name = string("spotCutoff");
		shaderEffect->SetUniformValue(name, 1, &light->GetSpotCutoff());
		name = string("spotCosCutoff");
		shaderEffect->SetUniformValue(name, 1, &light->GetSpotCosCutoff());
		name = string("constant");
		shaderEffect->SetUniformValue(name, 1, &light->GetConstantAttenuation());
		name = string("linear");
		shaderEffect->SetUniformValue(name, 1, &light->GetLinearAttenuation());
		name = string("quadratic");
		shaderEffect->SetUniformValue(name, 1, &light->GetQuadraticAttenuation());
		name = string("intensity");
		Vector3 intensityVec(light->GetSpecularIntensity(), 0.0, 0.0);
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

void GLShapeDrawerHelper::SetMaterialUniform(ShaderProgram* shaderEffect, BaseMaterial* material)
{
	Material* tempMaterial = dynamic_cast<Material*>(material);
	const Color &ambient = tempMaterial->GetAmbient();
	const Color& diffuse = tempMaterial->GetDiffuse();
	const Color &spec = tempMaterial->GetSpecular();
	float shininess = tempMaterial->GetShininess();
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
