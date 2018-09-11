#include "m3d/renderer/effect/XRayEffect.h"
#include "m3d/graphics/Material.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/TextNode.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "../gl10/GLShapeDrawer.h"
#include "sview/extern/PerspectiveOperator.h"
#include "sview/views/Parameters.h"
namespace M3D{
    
	XRayEffect::XRayEffect(RenderAction* action) :Effect(action)
	{
		
	}

	XRayEffect::~XRayEffect()
	{
		 
	}

	void XRayEffect::Render()
	{
		RenderAction* action = this->m_action;
		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();

		vector<RenderQueue *> RenderStateArray;
		for (int i = 0; i < effectsData.size(); i++)
		{
			effect = effectsData[i];
			it = renderPassQueueGroup.find(effect->GetRenderGroupType().GetType());
			if (it != renderPassQueueGroup.end())
			{
				it->second.SetRenderTech(effect);
				int renderType = it->second.GetType().GetType();
				if (renderType == RenderableType::RGT_SOLID || renderType == RenderableType::RGT_TRANSPARENT || renderType == RenderableType::RGT_POINT
					|| renderType == RenderableType::RGT_NOTE)
				{
				    DrawXRayPass(action, &it->second);
				}
				else if(renderType == RenderableType::RGT_INTOP)
				{
					DrawBasePass(action,&it->second);
				}
			}
		}
	}

	void XRayEffect::DrawBasePass(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (SVIEW::Parameters::Instance()->m_selectedStyle == 1 || SVIEW::Parameters::Instance()->m_selectedStyle == 2)
		{
			GLShapeDrawer20::DrawOutlinePassGroup(action, RenderStateArray);
		}
		else if (SVIEW::Parameters::Instance()->m_selectedStyle == 3)
		{
			GLShapeDrawer20::DrawINTOPRenderPassGroup(action, RenderStateArray);
		}

		GLShapeDrawer20::DrawImageBoardQueue(action);
		GLShapeDrawer20::DrawRenderActionBox(action);
	}

	void XRayEffect::DrawXRayPass(RenderAction* action, RenderQueue* RenderStateArray)
	{
		ShaderManager * shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}

		ShaderProgram * xrayEffect = shaderManager->GetEffect(
			ShaderManager::XRay);
		if (!xrayEffect)
		{
			return;
		}

		glEnable(GL_BLEND);
		CameraNode* camera = action->GetCamera();
		Matrix4 projectMatrix = camera->GetProjection().Transpose();
		Matrix4 viewMatrix = camera->GetView().ToMatrix4().Transpose();
		xrayEffect->UseProgram();

		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();

		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Renderable* faceRenderData = *it;
			int dataLength = faceRenderData->GetDataLength();

			if (dataLength > 0)
			{
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();

				ShaderParameter* vertex = xrayEffect->GetShaderAttributeParameter(VSP_POSITION);
				ShaderParameter* normal = xrayEffect->GetShaderAttributeParameter(VSP_NORMAL);

				Matrix4 modelMatrix = *(faceRenderData->GetRenderWorldMatrix());
				//mvp = modelMatrix*vp;

				Matrix3 normalMat = (modelMatrix * camera->GetView().ToMatrix4().Transpose()).Inverse().Transpose().ToMatrix3();

				xrayEffect->SetUniformValue(VSP_MODELMAT, modelMatrix);
				xrayEffect->SetUniformValue(VSP_VIEWMAT, viewMatrix);
				xrayEffect->SetUniformValue(VSP_PROJECTIONMAT, projectMatrix);
				xrayEffect->SetUniformValue(VSP_NORMALMAT, normalMat);

				const static string VSP_C = string("edgefalloff");
				//const static string VSP_P = string("u_p");
				//const static string VSP_VIEWVECTOR = string("u_viewVector");
				const static string FSP_GLOWCOLOR = string("u_Color");

				xrayEffect->SetUniformValue(VSP_C, 1.0f);
				//xrayEffect->SetUniformValue(VSP_P, 3.0f);

				Vector3 cameraDirection = -camera->GetDirection();
				//xrayEffect->SetUniformValue(VSP_VIEWVECTOR, cameraDirection);

				Color glowColor(0.5f,0.5f,0.5f,1.0f);
				xrayEffect->SetUniformValue(FSP_GLOWCOLOR, glowColor);

				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();

				xrayEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				xrayEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
				xrayEffect->EnableAttributeArray(vertex->m_location);
				xrayEffect->EnableAttributeArray(normal->m_location);

				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}

				xrayEffect->DisableAttributeArray(normal->m_location);
				xrayEffect->DisableAttributeArray(vertex->m_location);
			}
		}
		xrayEffect->ReleaseShaderProgram();
	}

}