#include "m3d/renderer/effect/OutlineEffect.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/graphics/CameraNode.h"
#include "sview/views/Parameters.h"
#include "m3d/renderer/gl20/ShaderManager.h"
namespace M3D
{
	OutlineEffect::OutlineEffect(RenderAction* action) :Effect(action)
	{
		m_firstTime = true;
		m_outlineTextureObj = 0;
	}
	void OutlineEffect::Render()
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
				if (renderType == RenderableType::RGT_SOLID || renderType == RenderableType::RGT_TRANSPARENT )
				{
					RenderStateArray.push_back(&it->second);
				}
			}
		}

		InnerRender(RenderStateArray);
	}
	GLint OutlineEffect::GetOutlineTextureObject() {
		return m_outlineTextureObj;
	}
	void OutlineEffect::Render(RenderQueue* RenderStateQueue)
	{
		vector<RenderQueue*> RenderStateArray ;
		RenderStateArray.push_back(RenderStateQueue);
		InnerRender(RenderStateArray);
	}

	void OutlineEffect::ClearResource()
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.ClearResource();
		}
	}

	void OutlineEffect::SetSize(int w, int h)
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.SetSize(w, h);
		}
	}


	void OutlineEffect::RenderModel(RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::Outline);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		// multi-clip planes
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = gl->GetViewMatrix();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);

		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			map<string, Uniform> tempUnifomValueList;
			
			vector<int> boolArray(enableClips, enableClips+3);
			int boolValue = (int)action->m_bReverseClip;
			tempUnifomValueList[FSP_ENABLECLIPS] = Uniform("BoolArray", &boolArray);
			tempUnifomValueList[FSP_REVERSECLIP] = Uniform("Bool", boolValue);


			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;


			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
			const Color &color = faceRenderData->GetRenderColor();
			Color red = SVIEW::Parameters::Instance()->m_outlineColor;
			tempUnifomValueList[FSP_DIFFUSE] = Uniform("Vector4", &red);
			//设置uniform值
			SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();

			this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

			int dataLength = faceRenderData->GetDataLength();
			bool isUseIndex = faceRenderData->IsUseIndex();
			HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			char* vertexAddress = (char*)vertexBuffer->Bind();
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
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



			for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
			{
				glActiveTexture(GL_TEXTURE0 + it->first);
				glBindTexture(it->second, 0);
			}

		}
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void OutlineEffect::RenderGaussianBlurWidth()
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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::GaussianBlur);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		Matrix4 V, P, M, MVP;

		tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));

		//vector<float> vec;
		//vec.push_back(xpo);
		tempUnifomValueList["tex_offset"] =  Uniform("Float", xpo);
		int isH = 1;
		tempUnifomValueList["horizontal"] =  Uniform("Bool", isH);

		if (m_firstTime) {
			tempUnifomValueList["image"] =  Uniform("Texture2D", (GeometryBuffer*)(GetHardWareFrameBuffer("model").GetColorTarget(0)));
			m_firstTime = false;
		}
		else {
			tempUnifomValueList["image"] =  Uniform("Texture2D", (GeometryBuffer*)(GetHardWareFrameBuffer("height").GetColorTarget(0)));
		}
		
		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void OutlineEffect::RenderGaussianBlurHeight()
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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::GaussianBlur);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		Matrix4 V, P, M, MVP;

		tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));

		//vector<float> vec;
		//vec.push_back(ypo);
		tempUnifomValueList["tex_offset"] = Uniform("Float", ypo);
		int isH = 0;
		tempUnifomValueList["horizontal"] = Uniform("Bool", isH);


		tempUnifomValueList["image"] = Uniform("Texture2D", (GeometryBuffer*)(GetHardWareFrameBuffer("width").GetColorTarget(0)));


		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void OutlineEffect::InnerRender(vector<RenderQueue*>& RenderStateArray)
	{
		m_firstTime= true;
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();



		HardWareFrameBuffer& modelFbo = this->GetHardWareFrameBuffer("model");
		modelFbo.SetParameters();
		modelFbo.ReShape();
	
		modelFbo.Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		for (int i = 0; i < RenderStateArray.size(); i++) {
			this->RenderModel(RenderStateArray[i]);
		}
		modelFbo.UnBind();


		HardWareFrameBuffer&  widthFbo = this->GetHardWareFrameBuffer("width");
		widthFbo.ReShape();
		//widthFbo.checkStatus();
		widthFbo.Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();


		HardWareFrameBuffer&  heightFbo = this->GetHardWareFrameBuffer("height");
		heightFbo.ReShape();
		//heightFbo.checkStatus();
		heightFbo.Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		widthFbo.Bind();
		this->RenderGaussianBlurWidth();
		widthFbo.UnBind();

		heightFbo.Bind();
		this->RenderGaussianBlurHeight();
		heightFbo.UnBind();

		HardWareFrameBuffer&  outlineFbo = this->GetHardWareFrameBuffer("outline");
		outlineFbo.ReShape();
		//outlineFbo.checkStatus();
		outlineFbo.Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		this->RenderOutline();
		outlineFbo.UnBind();
		GeometryBuffer* texture = (GeometryBuffer*)(this->GetHardWareFrameBuffer("outline").GetColorTarget(0));
		this->m_outlineTextureObj = texture->GetOGLObj();
		//DrawFrameBufferDebug();
	}

	void OutlineEffect::RenderOutline()
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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::GaussianBlurOutline);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		Matrix4 V, P, M, MVP;

		tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));
		

		tempUnifomValueList["height"] = Uniform("Texture2D", (GeometryBuffer*)(GetHardWareFrameBuffer("height").GetColorTarget(0)));
		tempUnifomValueList["model"] = Uniform("Texture2D", (GeometryBuffer*)(GetHardWareFrameBuffer("model").GetColorTarget(0)));

		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
	}



	void OutlineEffect::DrawFrameBufferDebug()
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
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
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
			//		LOGI(" ((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID()==%d", ((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID());
			GeometryBuffer* texture = (GeometryBuffer*)(GetHardWareFrameBuffer("outline").GetColorTarget(0));
			glBindTexture(GL_TEXTURE_2D, texture->GetOGLObj());
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