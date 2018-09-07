#include "m3d/renderer/effect/SAOEffect.h"
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
#include "sview/views/Parameters.h"
#include "m3d/utils/IDCreator.h"
using namespace SVIEW;
#define MAX_MIP_LEVEL 5
namespace M3D
{

	SAOEffect::SAOEffect(RenderAction* action) :Effect(action)
	{
		m_ssaoTextureObj = 0;
		m_ssaoTexture = NULL;
		m_cszBuffer = NULL;
		InitBuffers();
	}
	void SAOEffect::Render()
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
				if (renderType == RenderableType::RGT_SOLID || renderType == RenderableType::RGT_TRANSPARENT)
				{
					RenderStateArray.push_back(&it->second);
				}
			}
		}

		InnerRender(RenderStateArray);
	}

	void SAOEffect::Render(RenderQueue * RenderStateQueue)
	{
		vector<RenderQueue*> RenderStateArray;
		RenderStateArray.push_back(RenderStateQueue);
		RenderContext::OgldevError();
		InnerRender(RenderStateArray);
	}

	void SAOEffect::ClearResource()
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.ClearResource();
		}
	}

	void SAOEffect::SetSize(int w, int h)
	{
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.begin();
		for (it; it != m_hardWareFrameBufferMap.end(); it++)
		{
			it->second.SetSize(w, h);
		}
		if (m_cszBuffer)
		{
			
			m_cszBuffer->NeedUpdate();
		}
	}

	GLint SAOEffect::GetSAOTextureObject()
	{
		return m_ssaoTextureObj;
	}

	GeometryBuffer* SAOEffect::GetSAOTexture()
	{
		return m_ssaoTexture;
	}

	void SAOEffect::InitBuffers()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		m_compute_guard_band = intRect.Width() / 10;
		//渲染高精度深度，不经过fragmentshader
		HardWareFrameBuffer& zBuffer = GetHardWareFrameBuffer("zBuffer");
		zBuffer.ColorAttachmentNumber(0);
		zBuffer.UseDepthBuffer(true);
#ifdef _WIN32
		zBuffer.DepthInternalFormat(GL_DEPTH_COMPONENT32);
#endif
		zBuffer.SetSize(intRect.Width() , intRect.Height() );

		////重构相机坐标系下的Z值
		//GeometryBuffer* texture = NULL;
		//string key = "cameraspaceZ_texture";
		//m_cszBuffer = (GeometryBuffer*)action->GetScene()->GetResourceManager()->GetOrCreateTexture(key,
		//	Texture::TEXTURE_GEO);
		//m_cszBuffer->AddRef();
		//m_cszBuffer->MinmapFliter(GL_NEAREST_MIPMAP_NEAREST);
		//m_cszBuffer->MipMap(true);
		//m_cszBuffer->GetOGLObj();
		HardWareFrameBuffer& cameraSpaceMinifyBuffer0 = GetHardWareFrameBuffer("cameraSpaceMinifyBuffer0" );
		cameraSpaceMinifyBuffer0.ColorAttachmentNumber(1);
		cameraSpaceMinifyBuffer0.ColorTextureMipmap(true);
		//cameraSpaceMinifyBuffer0.OutColorTexture(m_cszBuffer);
		//cameraSpaceMinifyBuffer0.OutColorTextureLevel(0);
		cameraSpaceMinifyBuffer0.UseDepthBuffer(false);
#ifdef _WIN32
		cameraSpaceMinifyBuffer0.ColorInternalFormat(GL_R32F);
#endif
		cameraSpaceMinifyBuffer0.SetSize(intRect.Width(), intRect.Height());
		RenderContext::OgldevError();
		for (int i = 1;i<MAX_MIP_LEVEL;++i)
		{
			HardWareFrameBuffer& cameraSpaceMinifyBuffer = GetHardWareFrameBuffer("cameraSpaceMinifyBuffer"+IntToString(i));
			cameraSpaceMinifyBuffer.ColorAttachmentNumber(1);
			RenderContext::OgldevError();
			//cameraSpaceMinifyBuffer.OutColorTexture(m_cszBuffer);
			//cameraSpaceMinifyBuffer.OutColorTextureLevel(i);
			cameraSpaceMinifyBuffer.UseDepthBuffer(false);
#ifdef _WIN32
			cameraSpaceMinifyBuffer.ColorInternalFormat(GL_R32F);
#endif
			cameraSpaceMinifyBuffer.SetSize(intRect.Width(), intRect.Height());
		}

		//渲染原生SAO
		HardWareFrameBuffer& rawSAOBuffer = GetHardWareFrameBuffer("rawSAOBuffer");
		rawSAOBuffer.ColorAttachmentNumber(1);
		rawSAOBuffer.UseDepthBuffer(false);
		rawSAOBuffer.ColorInternalFormat(GL_RGBA);
		rawSAOBuffer.SetSize(intRect.Width(), intRect.Height());

		//模糊原生SAO得到SAO
		//H
		HardWareFrameBuffer& blurHorizontalBuffer = this->GetHardWareFrameBuffer("blurHorizontalBuffer");
		blurHorizontalBuffer.ColorAttachmentNumber(1);
		blurHorizontalBuffer.UseDepthBuffer(false);
		blurHorizontalBuffer.ColorInternalFormat(GL_RGBA);
		blurHorizontalBuffer.SetSize(intRect.Width(), intRect.Height());

		//V
		HardWareFrameBuffer& blurVerticalBuffer = this->GetHardWareFrameBuffer("blurVerticalBuffer");
		blurVerticalBuffer.ColorAttachmentNumber(1);
		blurVerticalBuffer.UseDepthBuffer(false);
		blurVerticalBuffer.ColorInternalFormat(GL_RGBA);
		blurVerticalBuffer.SetSize(intRect.Width(), intRect.Height());

		//V
		HardWareFrameBuffer& blurBuffer = this->GetHardWareFrameBuffer("blur");
		blurBuffer.ColorAttachmentNumber(1);
		blurBuffer.UseDepthBuffer(false);
		blurBuffer.ColorInternalFormat(GL_RGBA);
		blurBuffer.SetSize(intRect.Width(), intRect.Height());
	}

	void SAOEffect::RenderModel(RenderQueue * RenderStateArray)
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

			vector<int> boolArray(enableClips, enableClips + 3);
			int boolValue = (int)action->m_bReverseClip;
			tempUnifomValueList[FSP_ENABLECLIPS] = Uniform("BoolArray", &boolArray);
			tempUnifomValueList[FSP_REVERSECLIP] = Uniform("Bool", boolValue);


			tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (&gl->GetViewMatrix()));
			tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (&gl->GetProjectMatrix()));
			GLShapeDrawer20::_usedTextureUnits = 0;

			Renderable* faceRenderData = *it;


			Matrix4 M = *(faceRenderData->GetRenderWorldMatrix());
			Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
			Matrix4 worldNormalMat = M.Inverse().Transpose();
			tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", &M);
			const Color &color = faceRenderData->GetRenderColor();
			Color red = SVIEW::Parameters::Instance()->m_outlineColor;
			tempUnifomValueList[FSP_DIFFUSE] = Uniform("Color", &red);
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

	void SAOEffect::RenderSAO()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		float cameraNear = camera->GetNearClip();
		float cameraFar = camera->GetFarClip();

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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SaoEffect);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		if (texCoords)
		{
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
			shaderEffect->EnableAttributeArray(texCoords->m_location);
		}

		Matrix4 V, P, M, MVP;

		//tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		//tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		//tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));
		HardWareFrameBuffer& depthFbo = this->GetHardWareFrameBuffer("zBuffer");
		
		float radius = Parameters::Instance()->g_radius;
		float bias = Parameters::Instance()->g_SSAO_Bias;
		float intensity = Parameters::Instance()->g_SSAOContrast;
		float intensityDivR6 = intensity / powf(radius, 6.0);
		Vector4 saoData(radius,bias,intensity, intensityDivR6);
		Vector2 screenSize(depthFbo.GetWidth(), depthFbo.GetHeight());
		//GeometryBuffer* texture = NULL;
		//string key = "cameraspaceZ_texture";
		//texture = (GeometryBuffer*)action->GetScene()->GetResourceManager()->GetOrCreateTexture(key,
		//	Texture::TEXTURE_GEO);
		const Matrix4& projectData = camera->GetProjection();
		GeometryBuffer* texture = (GeometryBuffer*)(GetHardWareFrameBuffer("cameraSpaceMinifyBuffer0").GetColorTarget(0));
		tempUnifomValueList["CS_Z_buffer"] = Uniform("GeometryBuffer", texture);
		int isPerspective = camera->IsOrthographic() ? 0 : 1;
		tempUnifomValueList["isPerspective"] = Uniform("Bool", isPerspective);
		tempUnifomValueList["near"] = Uniform("Float", cameraNear);
		tempUnifomValueList["far"] = Uniform("Float", cameraFar);
		float projScale = 300.0;
		if (isPerspective)
		{
			projScale = intRect.m_right / (2.0f*M3D::Tan(35.0));
		}
		else
		{
			projScale = projectData.m_m00*0.5*intRect.m_right*1000.0;
		}	
		
		tempUnifomValueList["projScale"] = Uniform("Float", projScale);
		tempUnifomValueList["saoData"] = Uniform("Vector4", &saoData);
		

		//int isPerspective = camera->IsOrthographic() ? 0 : 1;
		//shaderEffect->SetUniformValue("isPerspective", isPerspective);

		Vector4 projConstant;
		if (isPerspective)
		{
			projConstant = Vector4
			(float(-2.0 / (intRect.m_right * projectData.m_m00)),
				float(-2.0 / (intRect.m_bottom * projectData.m_m11)),
				float((1.0 - (double)projectData.m_m02) / projectData.m_m00),
				float((1.0 - (double)projectData.m_m12) / projectData.m_m11));
		}
		else
		{
			float r = 1.0/projectData.m_m00;
			float t = 1.0/projectData.m_m11;
			projConstant = Vector4
			(float(2.0*r / (intRect.m_right)),
				float(-2.0*t / (intRect.m_bottom)),
				float(-r),
				float(t));
		}
		//tempUnifomValueList["projectionMatrix"] = Uniform("Matrix4", &gl->GetProjectMatrix());
		tempUnifomValueList["projInfo"] = Uniform("Vector4", &projConstant);

		tempUnifomValueList["resolution"] = Uniform("Vector2", &screenSize);


		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		if (texCoords)
		{
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		}
		
		shaderEffect->ReleaseShaderProgram();
	}

	void SAOEffect::RenderCameraSpaceZ()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		float cameraNear = camera->GetNearClip();
		float cameraFar = camera->GetFarClip();
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
			ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SaoReconstructCSZ);
			shaderEffect->UseProgram();

			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			if (texCoords)
			{
				shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
				shaderEffect->EnableAttributeArray(texCoords->m_location);
			}

			Matrix4 V, P, M, MVP;
			//		P = camera->GetProjection().Transpose();
			//		V = camera->GetView().ToMatrix4().Transpose();
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			int sampler0 = 0;
			ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter("depthMap");
			
			shaderEffect->SetUniformValue("near", cameraNear); 
			shaderEffect->SetUniformValue("far", cameraFar);
			int isPerspective = camera->IsOrthographic() ? 0 : 1;
			shaderEffect->SetUniformValue("isPerspective", isPerspective);
			
			shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);

			glActiveTexture(GL_TEXTURE0);
			GeometryBuffer* texture = (GeometryBuffer*)(GetHardWareFrameBuffer("zBuffer").GetDepthTarget());
			glBindTexture(GL_TEXTURE_2D, texture->GetOGLObj());			
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindTexture(GL_TEXTURE_2D, 0);			
			shaderEffect->DisableAttributeArray(vertex->m_location);
			if (texCoords)
			{
				shaderEffect->DisableAttributeArray(texCoords->m_location);
			}

			shaderEffect->ReleaseShaderProgram();
			glEnable(GL_DEPTH_TEST);

		}
	}

	void SAOEffect::RenderCameraSpaceMinifyZ(int level)
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
			ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SaoMinifyCSZ);
			shaderEffect->UseProgram();

			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			if (texCoords)
			{
				shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
				shaderEffect->EnableAttributeArray(texCoords->m_location);
			}
			Matrix4 V, P, M, MVP;
			//		P = camera->GetProjection().Transpose();
			//		V = camera->GetView().ToMatrix4().Transpose();
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			int sampler0 = 0;
			ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter("cszBuffer");
			shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);
			shaderEffect->SetUniformValue("previousMIPNumber", level);
			glActiveTexture(GL_TEXTURE0);
			//GeometryBuffer* texture = NULL;
			
			GeometryBuffer* texture = (GeometryBuffer*)(GetHardWareFrameBuffer("cameraSpaceMinifyBuffer0").GetColorTarget(0));
			glBindTexture(GL_TEXTURE_2D, texture->GetOGLObj());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
			shaderEffect->DisableAttributeArray(vertex->m_location);
			if (texCoords)
			{
				shaderEffect->DisableAttributeArray(texCoords->m_location);
			}
			
			shaderEffect->ReleaseShaderProgram();
			glEnable(GL_DEPTH_TEST);

		}
	}

	void SAOEffect::RenderHorizontalBlur()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		float cameraNear = camera->GetNearClip();
		float cameraFar = camera->GetFarClip();

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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SaoBlur);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		if (texCoords)
		{
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
			shaderEffect->EnableAttributeArray(texCoords->m_location);
		}

		Matrix4 V, P, M, MVP;

		//tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		//tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		//tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));
		HardWareFrameBuffer& ssaoFbo = this->GetHardWareFrameBuffer("rawSAOBuffer");
		Vector2 axis(1.0,0.0);
		tempUnifomValueList["source"] = Uniform("GeometryBuffer", (GeometryBuffer*)(ssaoFbo.GetColorTarget()));
		tempUnifomValueList["axis"] = Uniform("Vector2", &axis);

		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		if (texCoords)
		{
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		}
		shaderEffect->ReleaseShaderProgram();
	}

	void SAOEffect::RenderVerticalBlur()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		float cameraNear = camera->GetNearClip();
		float cameraFar = camera->GetFarClip();

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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SaoBlur);
		shaderEffect->UseProgram();

		map<string, Uniform> tempUnifomValueList;

		GLShapeDrawer20::_usedTextureUnits = 0;

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		if (texCoords)
		{
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
			shaderEffect->EnableAttributeArray(texCoords->m_location);
		}

		Matrix4 V, P, M, MVP;

		//tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		//tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		//tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));
		HardWareFrameBuffer& ssaoFbo = this->GetHardWareFrameBuffer("blurHorizontalBuffer");
		Vector2 axis(0, 1);
		tempUnifomValueList["source"] = Uniform("GeometryBuffer", (GeometryBuffer*)(ssaoFbo.GetColorTarget()));
		tempUnifomValueList["axis"] = Uniform("Vector2", &axis);

		SPHashMap& shaderUniformMap = shaderEffect->GetShaderUniformMap();
		this->SetUniform(shaderEffect, shaderUniformMap, tempUnifomValueList);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (map<int, GLenum>::iterator it = textureBindingTargetMap.begin(); it != textureBindingTargetMap.end(); it++)
		{
			glActiveTexture(GL_TEXTURE0 + it->first);
			glBindTexture(it->second, 0);
		}


		shaderEffect->DisableAttributeArray(vertex->m_location);
		if (texCoords)
		{
			shaderEffect->DisableAttributeArray(texCoords->m_location);
		}
		
		shaderEffect->ReleaseShaderProgram();
	}

	void SAOEffect::InnerRender(vector<RenderQueue*>& RenderStateArray)
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		RenderContext::OgldevError();
		//------------------------------------------depth pass---------------------------------------------
		HardWareFrameBuffer& modelFbo = this->GetHardWareFrameBuffer("zBuffer");
		//modelFbo.SetParameters();
		modelFbo.ReShape();
		RenderContext::OgldevError();
		modelFbo.Bind();
		RenderContext::OgldevError();
		modelFbo.CheckStatus();
		RenderContext::OgldevError();
		glEnable(GL_DEPTH_TEST);
		RenderContext::OgldevError();
		glDepthFunc(GL_LEQUAL);
		RenderContext::OgldevError();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		RenderContext::OgldevError();
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderContext::OgldevError();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		RenderContext::OgldevError();
		for (int i = 0; i < RenderStateArray.size(); i++) {
			this->RenderModel(RenderStateArray[i]);
		}
		RenderContext::OgldevError();
		modelFbo.UnBind();
		RenderContext::OgldevError();
		//----------------------------------------ReconstructorCameraSpaceZ pass------------------------------------------------------

		//----0----
		RenderContext::OgldevError();
		HardWareFrameBuffer& cameraSpaceMinifyBuffer0 = this->GetHardWareFrameBuffer("cameraSpaceMinifyBuffer0");		
		RenderContext::OgldevError();
		cameraSpaceMinifyBuffer0.ReShape();
		RenderContext::OgldevError();
		cameraSpaceMinifyBuffer0.Bind();
		RenderContext::OgldevError();
		cameraSpaceMinifyBuffer0.CheckStatus();
		RenderContext::OgldevError();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		RenderContext::OgldevError();
		glClear(GL_COLOR_BUFFER_BIT);
		RenderContext::OgldevError();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		RenderContext::OgldevError();
		RenderCameraSpaceZ();
		RenderContext::OgldevError();
		cameraSpaceMinifyBuffer0.UnBind();
		RenderContext::OgldevError();
		//---1~4----
		RenderContext::OgldevError();
		for (int i = 1; i < MAX_MIP_LEVEL; i++)
		{
			HardWareFrameBuffer& cameraSpaceMinifyBuffer = this->GetHardWareFrameBuffer("cameraSpaceMinifyBuffer"+IntToString(i));
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.OutColorTexture((GeometryBuffer*)cameraSpaceMinifyBuffer0.GetColorTarget(0));
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.OutColorTextureLevel(i);
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.ReShape();
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.Bind();
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.CheckStatus();
			RenderContext::OgldevError();
			glClearColor(0.0, 0.0, 0.0, 1.0);
			RenderContext::OgldevError();
			glClear(GL_COLOR_BUFFER_BIT);
			RenderContext::OgldevError();
			glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
			RenderContext::OgldevError();
			RenderCameraSpaceMinifyZ(i-1);
			RenderContext::OgldevError();
			cameraSpaceMinifyBuffer.UnBind();
			RenderContext::OgldevError();
		}



		//-------------------------------------------------rawSAO pass----------------------------------------------------------------
		HardWareFrameBuffer&  saoFbo = this->GetHardWareFrameBuffer("rawSAOBuffer");
		saoFbo.ReShape();
		
		saoFbo.Bind();
		saoFbo.CheckStatus();
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		this->RenderSAO();
		saoFbo.UnBind();
		RenderContext::OgldevError();
		////-------------------------------------------------H blur----------------------------------
		//HardWareFrameBuffer&  blurHFbo = this->GetHardWareFrameBuffer("blurHorizontalBuffer");
		//blurHFbo.ReShape();
		////widthFbo.checkStatus();
		//blurHFbo.Bind();
		//
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		//glClear( GL_COLOR_BUFFER_BIT);
		//glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		//this->RenderHorizontalBlur();
		//blurHFbo.UnBind();

		////-------------------------------------------------V blur--------------------------------------------
		//HardWareFrameBuffer&  blurVFbo = this->GetHardWareFrameBuffer("blurVerticalBuffer");
		//blurVFbo.ReShape();
		////widthFbo.checkStatus();
		//blurVFbo.Bind();

		//glClearColor(0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		//this->RenderVerticalBlur();
		//blurVFbo.UnBind();
		////-------------------------------------------------blur--------------------------------------------
		//HardWareFrameBuffer&  blurFbo = this->GetHardWareFrameBuffer("blur");
		//blurFbo.ReShape();
		////widthFbo.checkStatus();
		//blurFbo.Bind();

		//glClearColor(0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		//this->RenderBlur();
		//blurFbo.UnBind();
		////----------------------------------------------------END-----------------------------------------------------
		//GeometryBuffer* texture = (GeometryBuffer*)blurVFbo.GetColorTarget(0);
		//m_ssaoTextureObj = texture->GetOGLObj();
		//m_ssaoTexture = texture;

		////-------------------------------------TEST--------------------------
		DrawFrameBufferDebug();
	}

	void SAOEffect::RenderBlur()
	{
		RenderAction* action = this->m_action;
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		float cameraNear = camera->GetNearClip();
		float cameraFar = camera->GetFarClip();

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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::SsaoBlur);
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

		//tempUnifomValueList[VSP_MODELMAT] = Uniform("Matrix4", (void*)&M);
		//tempUnifomValueList[VSP_VIEWMAT] = Uniform("Matrix4", (void *)(&V));
		//tempUnifomValueList[VSP_PROJECTIONMAT] = Uniform("Matrix4", (void *)(&P));
		HardWareFrameBuffer& ssaoFbo = this->GetHardWareFrameBuffer("rawSAOBuffer");
		Vector2 screenSize(ssaoFbo.GetWidth(), ssaoFbo.GetHeight());
		tempUnifomValueList["ssaoInput"] = Uniform("GeometryBuffer", (GeometryBuffer*)(ssaoFbo.GetColorTarget()));
		tempUnifomValueList["size"] = Uniform("Vector2", &screenSize);

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

	void SAOEffect::DrawFrameBufferDebug()
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
			GeometryBuffer* texture = (GeometryBuffer*)(GetHardWareFrameBuffer("rawSAOBuffer").GetColorTarget(0));
			if (!texture)
			{
				return;
			}
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