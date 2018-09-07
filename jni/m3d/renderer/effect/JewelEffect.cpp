#include "m3d/renderer/effect/JewelEffect.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "sview/views/Parameters.h"
namespace M3D
{
	JewelEffect::JewelEffect(RenderAction* action) :Effect(action)
	{
		HardWareFrameBuffer& diamondFrontFBO = GetHardWareFrameBuffer("diamondFrontFBO");
		diamondFrontFBO.SetParameters();
		HardWareFrameBuffer& diamondBackFBO = GetHardWareFrameBuffer("diamondBackFBO");
		diamondBackFBO.SetParameters();
		HardWareFrameBuffer& ringtFBO = GetHardWareFrameBuffer("ringtFBO");
		ringtFBO.SetParameters();
		HardWareFrameBuffer& jewelTypeFBO = GetHardWareFrameBuffer("jewelTypeFBO");
		jewelTypeFBO.SetParameters();
		HardWareFrameBuffer& jewelSpecularFBO = GetHardWareFrameBuffer("jewelSpecularFBO");
		jewelSpecularFBO.SetParameters();
		HardWareFrameBuffer& jewelBlendFBO = GetHardWareFrameBuffer("jewelBlendFBO");
		jewelBlendFBO.SetParameters();
        HardWareFrameBuffer& jewelNoteFBO = GetHardWareFrameBuffer("jewelNoteFBO");
        jewelNoteFBO.SetParameters();

	}
	void JewelEffect::Render()
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
					RenderStateArray.push_back(&it->second);
				}
			}
		}

		DrawJewelPassGroup( RenderStateArray);
	}

	void JewelEffect::ClearResource()
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.ClearResource();
		}
	}

	void JewelEffect::SetSize(int w, int h)
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.SetSize(w, h);
		}
	}

	//************************************
	// Method:    RenderJewelFront
	// FullName:  M3D::JewelEffect::RenderJewelFront
	// Access:    private 
	// Returns:   void
	// Qualifier: 玉石、珍珠和晶石正面用此流程渲染，玉石Jade type 101 ;珍珠peral type 102 ;晶石crystal type 103 ;金属mental type 104
	// Parameter: RenderAction * action
	// Parameter: RenderQueue * RenderStateArray
	//************************************
	void JewelEffect::RenderJewelFront( RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelFront);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);

//		if (texCoords)
//			shaderEffect->EnableAttributeArray(texCoords->m_location);
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;

			//�任����
			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			tempUnifomValueList[VSP_NORMALMAT] = Uniform("Matrix4", &normalMat);
			tempUnifomValueList[("u_worldNormalMat")] = Uniform("Matrix4", &worldNormalMat);
			const Color &color = faceRenderData->GetRenderColor();
			Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (&color == Color::SelectColor)
			{
				selectColor = color;
			}
			tempUnifomValueList[FSP_SELECTCOLOR] = Uniform("Color", &selectColor);
			Vector3 cameraPosition = camera->GetWorldPosition();
			tempUnifomValueList[VSP_LIGHTPOSITION] = Uniform("Vector3", &cameraPosition);
			tempUnifomValueList[VSP_EYEPOSITION] = Uniform("Vector3", &cameraPosition);
			//材质
			BaseMaterial* material = faceRenderData->GetRenderMaterial();
			if (!material)
			{
				continue;
			}
			map<string, Uniform>& materialUnifoms = material->GetUnifomParameters();
			//shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(FSP_USEAMBIENTTEX)->m_location, 0);
			
			int vec = anyCast<int>(material->GetUniformParameter("type")->value);
			int type = vec;

			//将uniform中的值合并到tempUniformList中
			this->MergeUnifom(materialUnifoms, tempUnifomValueList);

			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			if(type == 104){
				continue;
			}
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);
#pragma region Draw
			if (type != 104)
			{
				int dataLength = faceRenderData->GetDataLength();
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
//				if (texCoords)
//				{
//					shaderEffect->EnableAttributeArray(texCoords->m_location);
//					shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
//						(GLvoid *)texoffset);
//				}
				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}
				vertexBuffer->UnBind();
			}
#pragma endregion Draw

#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion
		}

#pragma region 关闭顶点属性
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		if (texCoords)
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
#pragma endregion 关闭顶点属性
	}
	void JewelEffect::RenderJewelBack( RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelBack);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);

//		if (texCoords)
//			shaderEffect->EnableAttributeArray(texCoords->m_location);
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;

			//变换矩阵
			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			tempUnifomValueList[VSP_NORMALMAT] = Uniform("Matrix4", &normalMat);
			tempUnifomValueList[string("u_worldNormalMat")] = Uniform("Matrix4", &worldNormalMat);
			const Color &color = faceRenderData->GetRenderColor();
			Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (&color == Color::SelectColor)
			{
				selectColor = color;
			}
			tempUnifomValueList[FSP_SELECTCOLOR] = Uniform("Color", &selectColor);
			Vector3 cameraPosition = camera->GetWorldPosition();
			tempUnifomValueList[VSP_LIGHTPOSITION] = Uniform("Vector3", &cameraPosition);
			tempUnifomValueList[VSP_EYEPOSITION] = Uniform("Vector3", &cameraPosition);
			//材质
			BaseMaterial* material = faceRenderData->GetRenderMaterial();
			if (!material)
			{
				continue;
			}
			map<string, Uniform>& materialUnifoms = material->GetUnifomParameters();


			int vec = anyCast<int>(material->GetUniformParameter("type")->value);
			int type =  vec ;

			//将uniform中的值合并到tempUniformList中
			this->MergeUnifom(materialUnifoms, tempUnifomValueList);

			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			if(type == 104){
				continue;
			}
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);
#pragma region Draw
			if (type != 104)
			{
				int dataLength = faceRenderData->GetDataLength();
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
//				if (texCoords)
//				{
//					shaderEffect->EnableAttributeArray(texCoords->m_location);
//					shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
//						(GLvoid *)texoffset);
//				}
				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}
				vertexBuffer->UnBind();
			}
#pragma endregion Draw

#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion
		}

#pragma region 关闭顶点属性
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		if (texCoords)
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
#pragma endregion 关闭顶点属性
	}

	void JewelEffect::RenderRing( RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::Ring);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);

		if (texCoords)
			shaderEffect->EnableAttributeArray(texCoords->m_location);
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;

			//变换矩阵
			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			tempUnifomValueList[VSP_NORMALMAT] = Uniform("Matrix4", &normalMat);
			tempUnifomValueList[string("u_worldNormalMat")] = Uniform("Matrix4", &worldNormalMat);
			const Color &color = faceRenderData->GetRenderColor();
			Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (&color == Color::SelectColor)
			{
				selectColor = color;
			}
			tempUnifomValueList[FSP_SELECTCOLOR] = Uniform("Color", &selectColor);
			Vector3 cameraPosition = camera->GetWorldPosition();
			tempUnifomValueList[VSP_LIGHTPOSITION] = Uniform("Vector3", &cameraPosition);
			tempUnifomValueList[VSP_EYEPOSITION] = Uniform("Vector3", &cameraPosition);
			//材质
			BaseMaterial* material = faceRenderData->GetRenderMaterial();
			if (!material)
			{
				continue;
			}
			map<string, Uniform>& materialUnifoms = material->GetUnifomParameters();
			if (materialUnifoms.find("normalMap")!= materialUnifoms.end())
			{
				tempUnifomValueList["u_useBumpMap"] = Uniform("Bool", 1);
			}
			else
			{
				tempUnifomValueList["u_useBumpMap"] = Uniform("Bool", 0);
			}
			

			int vec = anyCast<int>(material->GetUniformParameter("type")->value);
			int type =  vec ;
			if (type != 104)
			{
				continue;
			}
	

			//将uniform中的值合并到tempUniformList中
			this->MergeUnifom(materialUnifoms, tempUnifomValueList);

			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			if (type != 104){
				continue;
			}
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);
#pragma region Draw
			if (type == 104)
			{
				int dataLength = faceRenderData->GetDataLength();
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
				if (texCoords)
				{
					shaderEffect->EnableAttributeArray(texCoords->m_location);
					shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
						(GLvoid *)texoffset);
				}
				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}
				vertexBuffer->UnBind();
			}
#pragma endregion Draw

#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion
		}

#pragma region 关闭顶点属性
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		if (texCoords)
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
#pragma endregion 关闭顶点属性
	}

	void JewelEffect::DrawJewelType( RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelType);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);

		shaderEffect->EnableAttributeArray(vertex->m_location);


		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;

			//变换矩阵
			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);


			//材质
			BaseMaterial* material = faceRenderData->GetRenderMaterial();
			if (!material)
			{
				continue;
			}
			map<string, Uniform>& materialUnifoms = material->GetUnifomParameters();


			int vec = anyCast<int>(material->GetUniformParameter("type")->value);
			int type =  vec ;
			if (type == 104)
			{
				continue;
			}

			//将uniform中的值合并到tempUniformList中
			this->MergeUnifom(materialUnifoms, tempUnifomValueList);

			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);
#pragma region Draw
			if (type != 104)
			{
				int dataLength = faceRenderData->GetDataLength();
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				

				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}
				vertexBuffer->UnBind();
			}
#pragma endregion Draw

#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion
		}

#pragma region 关闭顶点属性
		shaderEffect->DisableAttributeArray(vertex->m_location);

		shaderEffect->ReleaseShaderProgram();
#pragma endregion 关闭顶点属性


	}



	void JewelEffect::DrawJewelHighLight( RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelHighLight);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);

		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);


		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;

			//变换矩阵
			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			tempUnifomValueList[VSP_NORMALMAT] = Uniform("Matrix4", &normalMat);			
			const Color &color = faceRenderData->GetRenderColor();
			Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (&color == Color::SelectColor)
			{
				selectColor = color;
			}
			tempUnifomValueList[FSP_SELECTCOLOR] = Uniform("Color", &selectColor);
			//tempUnifomList[VSP_LIGHTPOSITION] = Uniform("Vector3", &camera->GetWorldPosition());
			//tempUnifomValueList[VSP_EYEPOSITION] = Uniform("Vector3", &camera->GetWorldPosition());
			//材质
			BaseMaterial* material = faceRenderData->GetRenderMaterial();
			if (!material)
			{
				continue;
			}

			map<string, Uniform>& materialUnifoms = material->GetUnifomParameters();

			Vector3 specular1(1.0, 1.0, 1.0);
			Vector3 specular2(0.0, 0.0, 0.0);
			int vec = anyCast<int>(material->GetUniformParameter("type")->value);
			int type =  vec ;
			if (type == 103)
			{
				tempUnifomValueList[FSP_SPECULAR] = Uniform("Vector3", &specular1);
			}
			else
			{
				tempUnifomValueList[FSP_SPECULAR] = Uniform("Vector3", &specular2);
			}

			//将uniform中的值合并到tempUniformList中
			this->MergeUnifom(materialUnifoms, tempUnifomValueList);

			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);
#pragma region Draw
			if (type != 104)
			{
				int dataLength = faceRenderData->GetDataLength();
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));

				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}
				vertexBuffer->UnBind();
			}
#pragma endregion Draw

#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion
		}

#pragma region 关闭顶点属性
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		shaderEffect->ReleaseShaderProgram();
#pragma endregion 关闭顶点属性
	}

	void JewelEffect::DrawJadeBlendQuad()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		{
			float debugvertices[] =
			{ // Vertices for the square
				-1.0f, 1.0f, -1.0f,// 0. left-bottom
				-1.0f, -1.0f, -1.0f,// 1. right-bottom
				1.0f, 1.0f, -1.0f,// 2. left-top
				1.0f, -1.0f, -1.0f// 3. right-top
			};

			float debugcoord[] =
			{ // Vertices for the square
				0, 1,// 0. left-bottom
				0, 0,// 1. right-bottom
				1, 1,// 2. left-top
				1, 0// 3. right-top
			};

			const IntRect& intRect = camera->GetViewPort().GetRect();
			glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
			float xpo = (float)(1.0f / intRect.m_right);
			float ypo = (float)(1.0f / intRect.m_bottom);
			ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelBlendQuad);
			shaderEffect->UseProgram();


			map<string, Uniform> tempUnifomValueList;

			GLShapeDrawer20::_usedTextureUnits = 0;


			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, debugvertices);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, debugcoord);
			shaderEffect->EnableAttributeArray(texCoords->m_location);
			Matrix4 V, P, M, MVP;

			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&V));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&P));

			tempUnifomValueList[FSP_SAMPLER0] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("diamondFrontFBO").GetColorTarget(0)));
			tempUnifomValueList[FSP_SAMPLER1] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("diamondBackFBO").GetColorTarget(0)));
			tempUnifomValueList[FSP_SAMPLER2] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("jewelTypeFBO").GetColorTarget(0)));
			tempUnifomValueList[string("u_sampler3")] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("jewelSpecularFBO").GetColorTarget(0)));
			
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#pragma region 解除texture绑定
			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}
#pragma endregion

			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->DisableAttributeArray(texCoords->m_location);
			shaderEffect->ReleaseShaderProgram();
		}
	}

	void JewelEffect::DrawJewelQuad()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();

		float debugvertices[] =
		{ // Vertices for the square
			-1.0f, 1.0f, -1.0f,// 0. left-bottom
			-1.0f, -1.0f, -1.0f,// 1. right-bottom
			1.0f, 1.0f, -1.0f,// 2. left-top
			1.0f, -1.0f, -1.0f// 3. right-top
		};

		float debugcoord[] =
		{ // Vertices for the square
			0, 1,// 0. left-bottom
			0, 0,// 1. right-bottom
			1, 1,// 2. left-top
			1, 0// 3. right-top
		};

		const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		float xpo = (float)(1.0f / intRect.m_right);
		float ypo = (float)(1.0f / intRect.m_bottom);
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::JewelFinalQuad);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, debugcoord);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		Matrix4 V, P, M, MVP;

		tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
		tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
		tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));



		tempUnifomValueList[FSP_SAMPLER0] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("jewelBlendFBO").GetColorTarget(0)));
		tempUnifomValueList[FSP_SAMPLER1] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("diamondFrontFBO").GetDepthTarget()));
		tempUnifomValueList[FSP_SAMPLER2] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("ringtFBO").GetColorTarget(0)));
		tempUnifomValueList["u_sampler3"] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("ringtFBO").GetDepthTarget()));
                tempUnifomValueList["u_sampler6"] = Uniform("GeometryBuffer", (GeometryBuffer*)(GetHardWareFrameBuffer("jewelNoteFBO").GetColorTarget(0)));

		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#pragma region 解除texture绑定
		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}
#pragma endregion

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void JewelEffect::DrawJewelPassGroup(vector<RenderQueue*>& RenderStateArray)
	{
		if (RenderStateArray.at(0)->GetRenderableArray().size() == 0&& RenderStateArray.at(1)->GetRenderableArray().size() == 0)
		{
			return;
		}
		HardWareFrameBuffer& diamondFrontFBO = GetHardWareFrameBuffer("diamondFrontFBO");

		HardWareFrameBuffer& diamondBackFBO = GetHardWareFrameBuffer("diamondBackFBO");

		HardWareFrameBuffer& ringtFBO = GetHardWareFrameBuffer("ringtFBO");

		HardWareFrameBuffer& jewelTypeFBO = GetHardWareFrameBuffer("jewelTypeFBO");

		HardWareFrameBuffer& jewelSpecularFBO = GetHardWareFrameBuffer("jewelSpecularFBO");

		HardWareFrameBuffer& jewelBlendFBO = GetHardWareFrameBuffer("jewelBlendFBO");
        
        HardWareFrameBuffer& jewelNoteFBO = GetHardWareFrameBuffer("jewelNoteFBO");

		RenderAction* action = this->m_action;
		CameraNode * camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		
		diamondFrontFBO.ReShape();
		diamondFrontFBO.Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        if(!SVIEW::Parameters::Instance()->m_BackTransparent)
        {
            GLShapeDrawer20::DrawBackGround((SceneNode*)action->GetBackGroundNode(), action);
        }
		
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		for (int i = 0; i < RenderStateArray.size(); i++)
		{
			if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_SOLID || (RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_TRANSPARENT)
			{
				RenderJewelFront( RenderStateArray.at(i));
			}
		}
		diamondFrontFBO.UnBind();

        diamondBackFBO.ReShape();
        diamondBackFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        if(!SVIEW::Parameters::Instance()->m_BackTransparent)
        {
            GLShapeDrawer20::DrawBackGround((SceneNode*)action->GetBackGroundNode(), action);
        }
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        for (int i = 0; i < RenderStateArray.size(); i++)
        {
            if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_SOLID || (RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_TRANSPARENT)
            {
                RenderJewelBack( RenderStateArray.at(i));
            }
        }
        diamondBackFBO.UnBind();
        glFrontFace(GL_CCW);
        glDisable(GL_CULL_FACE);


        ringtFBO.ReShape();
        ringtFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        if(!SVIEW::Parameters::Instance()->m_BackTransparent)
        {
            GLShapeDrawer20::DrawBackGround((SceneNode*)action->GetBackGroundNode(), action);
        }
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        for (int i = 0; i < RenderStateArray.size(); i++)
        {
            if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_SOLID || (RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_TRANSPARENT)
            {
                RenderRing(RenderStateArray.at(i));
            }
        }
        ringtFBO.UnBind();

        jewelTypeFBO.ReShape();
        jewelTypeFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        for (int i = 0; i < RenderStateArray.size(); i++)
        {
            if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_SOLID || (RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_TRANSPARENT)
            {
                DrawJewelType(RenderStateArray.at(i));
            }
        }
        jewelTypeFBO.UnBind();

        jewelSpecularFBO.ReShape();
        jewelSpecularFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        for (int i = 0; i < RenderStateArray.size(); i++)
        {
            if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_SOLID || (RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_TRANSPARENT)
            {
                DrawJewelHighLight(RenderStateArray.at(i));
            }
        }
        jewelSpecularFBO.UnBind();

        jewelBlendFBO.ReShape();
        jewelBlendFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        DrawJadeBlendQuad();
        jewelBlendFBO.UnBind();

        jewelNoteFBO.ReShape();
        jewelNoteFBO.Bind();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0, 0.0, 0.0, 0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
        for (int i = 0; i < RenderStateArray.size(); i++)
        {
            if ((RenderStateArray.at(i))->GetType().GetType() == RenderableType::RGT_NOTE )
            {
                GLShapeDrawer20::DrawNoteRenderPassGroup(action, RenderStateArray.at(i) );
            }
        }
        jewelNoteFBO.UnBind();


		DrawJewelQuad();
		//DrawFrameBufferDebug();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
	}
	

	void JewelEffect::DrawFrameBufferDebug()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		{
			float debugvertices[] =
			{ // Vertices for the square
				-1.0f, 1.0f, -1.0f,// 0. left-bottom
				-1.0f, -1.0f, -1.0f,// 1. right-bottom
				1.0f, 1.0f, -1.0f,// 2. left-top
				1.0f, -1.0f, -1.0f// 3. right-top
			};

			float debugcoord[] =
			{ // Vertices for the square
				0, 1,// 0. left-bottom
				0, 0,// 1. right-bottom
				1, 1,// 2. left-top
				1, 0// 3. right-top
			};
			ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::FBODebug);
			shaderEffect->UseProgram();

			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, debugvertices);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, debugcoord);
			shaderEffect->EnableAttributeArray(texCoords->m_location);
			Matrix4 V, P, M, MVP;
			//		P = camera->GetProjection().Transpose();
			//		V = camera->GetView().ToMatrix4().Transpose();
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			int sampler0 = 0;
			ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
			shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			//unsigned int cubeMapObj = ResourceManager::GetInstance()->GetDefaultSphereMap()->GetOGLObj();
			//LOGI("cubeMapObj ===%d",cubeMapObj);
			//		LOGI(" ((GeometryBuffer*)(action->m_teethFBO.GetColorTarget(0)))->GetID()==%d", ((GeometryBuffer*)(action->m_teethFBO.GetColorTarget(0)))->GetID());
			glBindTexture(GL_TEXTURE_2D, ((GeometryBuffer*)(GetHardWareFrameBuffer("jewelNoteFBO").GetColorTarget(0)))->GetID());
			//		LOGI("Draw Point1111");
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->DisableAttributeArray(texCoords->m_location);
			shaderEffect->ReleaseShaderProgram();
			glEnable(GL_DEPTH_TEST);

		}

	}

}
