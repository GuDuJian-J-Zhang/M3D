#include "GLShapeDrawer20.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/model/MeshData.h"

#include "m3d/scene/ShapeNode.h"

#include "m3d/model/PMIData.h"
#include "m3d/model/PolyLine.h"
#include "m3d/graphics/TextNode.h"
#include "sview/views/Parameters.h"

#include "m3d/utils/Trackball.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/base/Color.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/renderer/gl10/GLShapeDrawer.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/graphics/MeshBoard.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/Handler/Handler.h"
#include "m3d/model/Point.h"
#include "m3d/model/Note.h"
#include "m3d/extras/note/VoiceNote.h"
#include "m3d/graphics/Section.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/ResourceManager.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/RenderManager.h"
#include "m3d/renderer/gl20/GLShapeDrawerHelper.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/scene/SceneGroundNode.h"
#include "m3d/graphics/GeometryBuffer.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/Body.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/graphics/CrossSection.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/renderer/effect/EffectManager.h"
#include "m3d/renderer/effect/Effect.h"
#include "m3d/renderer/effect/OutlineEffect.h"
#include "m3d/renderer/LightManager.h"
#include "m3d/renderer/UniformHelper.h"
#include "m3d/graphics/ShaderMaterial.h"
#include "m3d/renderer/gl20/ShaderLib.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/extras/annotation/Annotation.h"
#include "m3d/scene/GroundNode.h"
#include "../effect/SSAOEffect.h"
#include "../effect/SAOEffect.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/graphics/MatCapMaterial.h"
#include "m3d/graphics/DepthMaterial.h"
#include "m3d/graphics/LightShadow.h"
#include "m3d/graphics/DirectionalLightShadow.h"

//#define FBO
//#define framebufferDebug
using namespace SVIEW;

namespace M3D
{
	long GLShapeDrawer20::drawBoxTime = 0;
	long GLShapeDrawer20::drawMeshTime = 0;
	long GLShapeDrawer20::drawPMITime = 0;

	int GLShapeDrawer20::m_lastWidth = 0;

	bool GreaterSort(TransparentObject a, TransparentObject b) { return (a.z > b.z); }
	void GLShapeDrawer20::RenderFaces(RenderAction* action, vector<Renderable*>& renderableArray, BaseMaterial* overrideMaterial /*= nullptr*/)
	{
		if (renderableArray.size() == 0) {
			return;
		}
		m_currentCamera = 0;
		m_currentMaterial = 0;
		ResourceManager* rsmgr = action->GetScene()->GetResourceManager();
		LightManager* lightManager = action->GetScene()->GetLightManager();
		CameraNode* camera = action->GetCamera();
		lightManager->SetUp(camera);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		vector<Renderable*>::iterator it = renderableArray.begin();
		for (; it != renderableArray.end(); it++)
		{
			Renderable* faceRenderData = (*it);
			Face* face = static_cast<Face*>(faceRenderData);
			//BaseMaterial* material = face->GetMaterial();
			BaseMaterial* material = overrideMaterial == nullptr ? face->GetMaterial() : overrideMaterial;
			RenderFace(action, faceRenderData, camera, nullptr);
		}
		glDisable(GL_CULL_FACE);
		glDisableVertexAttribArray(0);
		M3D_GL_ERROR_CHECK
			glDisableVertexAttribArray(1);
		M3D_GL_ERROR_CHECK
			glDisableVertexAttribArray(2);
		M3D_GL_ERROR_CHECK
			m_shadowState = Parameters::Instance()->m_shadowMapEnabled;
	}
	void GLShapeDrawer20::RenderFaces(RenderAction* action, vector<TransparentObject>& renderableArray, BaseMaterial* overrideMaterial /*= nullptr*/)
	{
		if (renderableArray.size() == 0) {
			return;
		}
		m_currentCamera = 0;
		m_currentMaterial = 0;
		ResourceManager* rsmgr = action->GetScene()->GetResourceManager();
		LightManager* lightManager = action->GetScene()->GetLightManager();
		CameraNode* camera = action->GetCamera();
		lightManager->SetUp(camera);


		vector<TransparentObject>::iterator it = renderableArray.begin();


		for (; it != renderableArray.end(); it++)
		{
			Renderable* faceRenderData = (*it).renderable;

			//BaseMaterial* material = face->GetMaterial();

			RenderFace(action, faceRenderData, camera);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		m_shadowState = Parameters::Instance()->m_shadowMapEnabled;
	}

	void GLShapeDrawer20::RenderFace(RenderAction* action, Renderable*renderable, CameraNode*camera /*= nullptr*/, BaseMaterial* overrideMaterial /*= nullptr*/)
	{
		LOGI("material->RenderFace ");
		//if (!overrideMaterial)
		//{
		//	return;
		//}
		Face* face = static_cast<Face*>(renderable);
		BaseMaterial* material = overrideMaterial == nullptr ? face->GetMaterial() : overrideMaterial;
		map<int, GLenum> textureBindingTargetMap;//保存纹理绑定信息
		ShaderProgram* program = SetProgram(action, renderable, textureBindingTargetMap, camera, material);

		ShaderParameter* vertex = program->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = program->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = program->GetShaderAttributeParameter(VSP_TEXCOORDS);
		program->EnableAttributeArray(vertex->m_location);
		if (normal)
		{
			program->EnableAttributeArray(normal->m_location);
		}
		if (texCoords)
		{
			program->EnableAttributeArray(texCoords->m_location);
		}
		int dataLength = renderable->GetDataLength();
		bool isUseIndex = renderable->IsUseIndex();

		//shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		//shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

		HardWareVertexBuffer* vertexBuffer = renderable->GetHardWareVertexBuffer();
		HardWareIndexBuffer* indexBuffer = renderable->GetHardWareIndexBuffer();
		char* vertexAddress = (char*)vertexBuffer->Bind();

		M3D_OFFSET_TYPE veroffset = renderable->GetVertexOffset();
		M3D_OFFSET_TYPE normaloffset = renderable->GetNormalOffset();
		M3D_OFFSET_TYPE texoffset = renderable->GetTextureCoordsOffset();
		if (!texoffset && texCoords)
		{
			program->DisableAttributeArray(texCoords->m_location);
		}

		program->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
		if (normal)
		{
			program->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
		}

		if (texCoords && texoffset)
		{
			program->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
				(GLvoid *)texoffset);
		}


		if (isUseIndex)
		{
			M3D_OFFSET_TYPE indexArray = renderable->GetIndexOffset();
			GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
		}
		else
		{
			LOGI("material->!isUseIndex ");
			GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
		}
		//解绑VBO，ios必须做
		vertexBuffer->UnBind();
		//解绑纹理
		if (material != m_currentMaterial)
		{
			map<int, GLenum>::iterator it = textureBindingTargetMap.begin();
			for (; it != textureBindingTargetMap.end(); it++) {
				GLenum value = it->second;
				glActiveTexture(GL_TEXTURE0 + it->first);
				M3D_GL_ERROR_CHECK
					glBindTexture(value, 0);
				M3D_GL_ERROR_CHECK
			}
		}

	}

	ShaderProgram* GLShapeDrawer20::SetProgram(RenderAction* action, Renderable* renderable, map<int, GLenum>& textureBindingTargetMap, CameraNode*camera /*= nullptr*/, BaseMaterial* material /*= nullptr*/)
	{
		_usedTextureUnits = 0;
		SceneManager* sceneManager = action->GetScene();
		LightManager* lightManager = sceneManager->GetLightManager();
		bool refreshProgram = false;
		bool refreshMaterial = false;
		bool refreshLights = false;
		if (!material->NeedUpdate())
		{
			if (!material->Program())
			{
				material->NeedUpdate(true);
			}
			else if (material->AcceptLight() && material->LightHash() != lightManager->GetState().hash)//TODO，可能某些材质不需要灯光
			{
				material->NeedUpdate(true);
			}


			//else if (m_ssaoLastState != Parameters::Instance()->m_useSSAO)
			//{
			//	material->NeedUpdate(true);
			//	m_ssaoLastState = Parameters::Instance()->m_useSSAO;
			//}
		}
		if (m_shadowState != Parameters::Instance()->m_shadowMapEnabled)
		{
			material->NeedUpdate(true);
		}

		if (material->NeedUpdate())
		{
			InitMaterial(action, material, action->GetShaderMananger());
			material->NeedUpdate(false);
			refreshLights = true;
		}

		ShaderProgram* program = nullptr;
		program = material->Program();
		program->UseProgram();

		UniformMap* tempUnifomValueList = UniformHelper::GetUniformMap(program->GetName());//material删除时，与其相关联的uniform列表也应该清理掉。

		if (m_currentProgram != program)
		{
			refreshProgram = true;
			refreshMaterial = true;
			refreshLights = true;
			m_currentProgram = program;
		}
		if (material != m_currentMaterial)
		{
			refreshMaterial = true;
			m_currentMaterial = material;
			//refreshLights = true;
		}

		if (refreshProgram || camera != m_currentCamera)
		{
			Matrix4 viewMatrix = camera->GetView().ToMatrix4().Transpose();
			Matrix4 projectionMatrix = camera->GetProjection().Transpose();
			program->SetUniformValue(VIEW_MATRXI, viewMatrix);//局部变量不能用Any
			program->SetUniformValue(PROJECTION_MATRXI, projectionMatrix);

			//clipping
			Vector4 tc[3];
			int enableClips[3];
			Matrix4 tempMat = viewMatrix;
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat.Inverse() * tc[i];
				enableClips[i] = action->m_enableClip[i];
			}
			program->SetUniformValue(FSP_CLIPPLANES, 3, tc);
#ifdef __MOBILE__
			program->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
#endif
			ShaderParameter* reverseClipPara = program->GetShaderUniformParameter(FSP_REVERSECLIP);
			program->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			m_currentCamera = camera;//相机记录
			refreshMaterial = true;//刷新材质
			refreshLights = true;//刷新灯光

			//加载材质指定的uniforms
			if (material->GetMaterialType() == MaterialType::MaterialType_Shader ||
				material->GetMaterialType() == MaterialType::MaterialType_Phong ||
				material->GetMaterialType() == MaterialType::MaterialType_Pbr ||
				material->GetMaterialType() == MaterialType::MaterialType_MatCap
				)
			{
				Vector3 cameraPostion = camera->GetPosition();
				program->SetUniformValue(CAMERA_POSITION, cameraPostion);
			}
		}
#ifdef WIN32

		Face* face = dynamic_cast<Face*>(renderable);
		LOGI("face->GetNeedClip() : %d ",face->GetNeedClip());
			int enableClips[3];

		if (face && face->GetNeedClip())
		{
			int enableClips[3];
			for (int i = 0; i < 3; i++)
			{
				enableClips[i] = action->m_enableClip[i];
			}
			program->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		}
		else
		{
			int enableClips[3] = { 0 };
			program->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		}
#endif
		if (refreshMaterial)
		{
			//TODO toneMapping?

			if (material->GetUseLight() && refreshLights)
			{
				vector<BaseLight*>& allLights = lightManager->GetAllLight();
				SceneLightState& state = lightManager->GetState();
				int directionalLightNumber = 0;
				int pointLightNumber = 0;
				int spotLightNumber = 0;
				int hemisphereLightNumber = 0;


				//环境光
				program->SetUniformValue(AMBIENT_LIGHT_COLOR, state.ambient);
				if (Parameters::Instance()->m_shadowMapEnabled)
				{
					if (tempUnifomValueList->m_uniformMap.find("directionalShadowMap") != tempUnifomValueList->m_uniformMap.end())
					{
						vector<GeometryBuffer*> geoVector = anyCast<vector<GeometryBuffer*>>(tempUnifomValueList->m_uniformMap["directionalShadowMap"].value);
						geoVector.clear();
						UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMap", "GeometryBufferArray", geoVector);
					}

					//同上
					if (tempUnifomValueList->m_uniformMap.find("directionalShadowMatrix") != tempUnifomValueList->m_uniformMap.end())
					{
						vector<Matrix4> geoVector = anyCast<vector<Matrix4>>(tempUnifomValueList->m_uniformMap["directionalShadowMatrix"].value);
						geoVector.clear();
						UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMatrix", "Matrix4Array", geoVector);
					}
				}


				for (int i = 0, l = allLights.size(); i < l; i++)
				{
					BaseLight* light = allLights[i];
					if (!light->IsTurnOn())
					{
						continue;
					}
					int lightType = light->GetLightSourceType();
					LightUniforms* uniforms = NULL;

					float intensity = light->GetIntensity();
					Color lightColor = light->GetLightColor();

					if (lightType == LightType_Directional)
					{
						//if (light->NeedUpdateInfo())
						{
							LightShadow* shadow = light->GetLightShadow();
							uniforms = state.directional[directionalLightNumber];
							DirectionalLightUniforms* directionLightUniforms = static_cast<DirectionalLightUniforms*>(uniforms);
							string baseName = "directionalLights[" + IntToString(directionalLightNumber) + "]";
							program->SetUniformValue((baseName + ".color"), directionLightUniforms->color);
							program->SetUniformValue((baseName + ".direction"), directionLightUniforms->direction);
							program->SetUniformValue((baseName + ".shadow"), (GLint)(directionLightUniforms->shadow));
							if (light->CastShadow() && Parameters::Instance()->m_shadowMapEnabled)
							{
								program->SetUniformValue(baseName + ".shadowBias", directionLightUniforms->shadowBias);
								program->SetUniformValue(baseName + ".shadowRadius", directionLightUniforms->shadowRadius);
								program->SetUniformValue(baseName + ".shadowMapSize", directionLightUniforms->shadowMapSize);

								//可能传递的是个renderTarget数组
								if (tempUnifomValueList->m_uniformMap.find("directionalShadowMap") == tempUnifomValueList->m_uniformMap.end())
								{
									vector<GeometryBuffer*> geoVector;
									geoVector.push_back(static_cast<GeometryBuffer*>(shadow->RenderTarget()->GetColorTarget(0)));
									UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMap", "GeometryBufferArray", geoVector);
								}
								else
								{
									vector<GeometryBuffer*> geoVector = anyCast<vector<GeometryBuffer*>>(tempUnifomValueList->m_uniformMap["directionalShadowMap"].value);
									geoVector.push_back(static_cast<GeometryBuffer*>(shadow->RenderTarget()->GetColorTarget(0)));
									UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMap", "GeometryBufferArray", geoVector);
								}

								//同上
								if (tempUnifomValueList->m_uniformMap.find("directionalShadowMatrix") == tempUnifomValueList->m_uniformMap.end())
								{
									vector<Matrix4> geoVector;
									geoVector.push_back(shadow->Matrix().Transpose());
									UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMatrix", "Matrix4Array", geoVector);
								}
								else
								{
									//tempUnifomValueList->m_uniformMap["directionalShadowMatrix"].value = shadow->Matrix().Transpose();

									vector<Matrix4> geoVector = anyCast<vector<Matrix4>>(tempUnifomValueList->m_uniformMap["directionalShadowMatrix"].value);
									geoVector.push_back(shadow->Matrix().Transpose());
									UniformHelper::SetUniformValueToMap(tempUnifomValueList, "directionalShadowMatrix", "Matrix4Array", geoVector);
								}


							}

							light->NeedUpdateInfo(false);
						}
						directionalLightNumber++;
					}
					else if (lightType == LightType_Point)
					{
						//if (light->NeedUpdateInfo())
						{
							uniforms = state.point[pointLightNumber];
							PointLightUniforms* pointLightUniforms = static_cast<PointLightUniforms*>(uniforms);

							string baseName = "pointLights[" + IntToString(pointLightNumber) + "]";
							program->SetUniformValue((baseName + ".color"), pointLightUniforms->color);
							program->SetUniformValue((baseName + ".position"), pointLightUniforms->position);
							program->SetUniformValue((baseName + ".distance"), &pointLightUniforms->distance);
							program->SetUniformValue((baseName + ".decay"), &pointLightUniforms->decay);
							program->SetUniformValue((baseName + ".shadow"), pointLightUniforms->shadow);
							light->NeedUpdateInfo(false);
						}
						pointLightNumber++;
					}
					else if (lightType == LightType_Spot)
					{
						//if (light->NeedUpdateInfo())
						{
							uniforms = state.spot[spotLightNumber];
							SpotLightUniforms* spotLightUniforms = static_cast<SpotLightUniforms*>(uniforms);

							string baseName = "spotLights[" + IntToString(spotLightNumber) + "]";
							program->SetUniformValue((baseName + ".color"), spotLightUniforms->color);
							program->SetUniformValue((baseName + ".position"), spotLightUniforms->position);
							program->SetUniformValue((baseName + ".direction"), spotLightUniforms->direction);
							program->SetUniformValue((baseName + ".distance"), &spotLightUniforms->distance);
							program->SetUniformValue((baseName + ".decay"), &spotLightUniforms->decay);
							program->SetUniformValue((baseName + ".coneCos"), &spotLightUniforms->coneCos);
							program->SetUniformValue((baseName + ".penumbraCos"), &spotLightUniforms->penumbraCos);
							program->SetUniformValue((baseName + ".shadow"), &spotLightUniforms->shadow);

							//glUniform3f(7, spotLightUniforms->color.m_r, spotLightUniforms->color.m_g, spotLightUniforms->color.m_b);
							//glUniform3f(13, spotLightUniforms->position.m_x, spotLightUniforms->position.m_y, spotLightUniforms->position.m_z);
							//glUniform3f(10, spotLightUniforms->direction.m_x, spotLightUniforms->direction.m_y, spotLightUniforms->direction.m_z);
							//glUniform1f(11, spotLightUniforms->distance);
							//glUniform1f(9, spotLightUniforms->decay);
							//glUniform1f(8, spotLightUniforms->coneCos);
							//glUniform1f(12, spotLightUniforms->penumbraCos);
							//glUniform1f(14, spotLightUniforms->shadow);
							light->NeedUpdateInfo(false);
						}
						spotLightNumber++;
					}
					else if (lightType == LightType_Hemisphere)
					{
						//if (light->NeedUpdateInfo())
						{
							uniforms = state.hemisphere[hemisphereLightNumber];
							HemisphereLightUniforms* hemisphereLightUniforms = static_cast<HemisphereLightUniforms*>(uniforms);

							string baseName = "hemisphereLights[" + IntToString(hemisphereLightNumber) + "]";
							program->SetUniformValue((baseName + ".skyColor"), hemisphereLightUniforms->skyColor);
							program->SetUniformValue((baseName + ".groundColor"), hemisphereLightUniforms->groundColor);
							program->SetUniformValue((baseName + ".direction"), hemisphereLightUniforms->direction);
							light->NeedUpdateInfo(false);
						}
						hemisphereLightNumber++;
					}
				}
				//vector<LightUniforms*> state.directional
			}

			if (material->GetMaterialType() == MaterialType::MaterialType_Phong || material->GetMaterialType() == MaterialType::MaterialType_Pbr)
			{
				InnerMaterial* innerMaterial = static_cast<InnerMaterial*>(material);
				const Matrix3& mat = innerMaterial->GetUvTransform().Transpose();
				program->SetUniformValue(UV_TRANSFORM_MATRIX, mat);
			}

			if (material->GetMaterialType() == MaterialType::MaterialType_Phong)
			{
				RefreshUniformsPhong(action, tempUnifomValueList, static_cast<Material*>(material));
			}
			else if (material->GetMaterialType() == MaterialType::MaterialType_Pbr)
			{
				RefreshUniformsPbr(action, tempUnifomValueList, static_cast<PbrMaterial*>(material));
			}
			else if (material->GetMaterialType() == MaterialType::MaterialType_MatCap)
			{
				RefreshUniformsMatCap(action, tempUnifomValueList, static_cast<MatCapMaterial*>(material));
			}
			if (Parameters::Instance()->m_useSSAO)
			{
				program->SetUniformValue(("screenSize"), Vector2(camera->GetViewPort().GetRect().Width(), camera->GetViewPort().GetRect().Height()));
				EffectManager* effectManager = action->GetRenderManager()->GetEffectManager();
				SAOEffect* ssaoEffect = (SAOEffect*)effectManager->GetEffect("SAOEFFECT");
				UniformHelper::SetUniformValueToMap(tempUnifomValueList, ("ssaoMap"), "GeometryBuffer", (ssaoEffect->GetSAOTexture()));
			}

			UniformHelper::UpdateUniform(action, textureBindingTargetMap, program, program->GetShaderUniformMap(), tempUnifomValueList->m_uniformMap);

		}
		Matrix4 M = *(renderable->GetRenderWorldMatrix());
		Matrix4 normalMat = (M * camera->GetView().ToMatrix4().Transpose()).Inverse().Transpose();
		program->SetUniformValue((MODEL_MATRXI), M);
		program->SetUniformValue((NORMAL_MATRXI), normalMat);
		float faceAlpha = ((Face*)renderable)->GetAlpha();
		Color color(1, 1, 1, faceAlpha);
		if (!Parameters::Instance()->m_IsShowBox && ((Face*)renderable)->IsSelected())
		{
			color = Color(Color::SelectColor->m_r, Color::SelectColor->m_g, Color::SelectColor->m_b, faceAlpha);
		}
		program->SetUniformValue(FSP_SELECTCOLOR, color);

		return program;
	}

	void GLShapeDrawer20::InitMaterial(RenderAction* action, BaseMaterial* material, ShaderManager* shaderManager)
	{
		ResourceManager* rsmgr = action->GetScene()->GetResourceManager();
		LightManager* lightManager = action->GetScene()->GetLightManager();

		shaderManager->CreateParameters(material, lightManager);

		string code = shaderManager->GetProgramCode(material);

		ShaderProgram* program = material->Program();

		bool programChange = true;
		if (!program) {

		}
		else if (program->GetName() != code)
		{
			material->Program(NULL);
			//TODO是否要释放不用的shader？不释放，可能会积累比较多，释放下次相同code可能需要重建
		}
		else
		{
			programChange = false;
		}

		string vs = "";
		string fs = "";
		if (programChange)
		{
			if (material->GetMaterialType() == MaterialType_Shader)
			{
				ShaderMaterial* shaderMaterial = static_cast<ShaderMaterial*>(material);
				vs = shaderMaterial->VertexShader();
				fs = shaderMaterial->FragmentShader();
			}
			else
			{
				int materialtype = material->GetMaterialType();
				//TODO
                pair<const char*, const char*>* ret = ShaderLib::GetSrcCode(IntToString(materialtype));
                if (ret != NULL) {
                    vs = ShaderLib::GetSrcCode(IntToString(materialtype))->first;
                    fs = ShaderLib::GetSrcCode(IntToString(materialtype))->second;
                }
			}
			material->LightHash(lightManager->GetState().hash);
			program = shaderManager->AcquireProgram(material, vs, fs, code);
			material->Program(program);
		}
	}

	BaseMaterial * GLShapeDrawer20::GetDepthMaterial(BaseLight * light, ResourceManager * resourceManager, bool isPointLight)
	{
		if (!depthMaterial) {
			depthMaterial = (DepthMaterial*)resourceManager->GetOrCreateMaterial("shadowDepthMaterial", MaterialType_Depth);
			depthMaterial->AddRef();
			depthMaterial->AcceptLight(false);
		}
		return depthMaterial;
	}

	void GLShapeDrawer20::ShadowRender(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		ResourceManager* rsmgr = action->GetScene()->GetResourceManager();

		LightManager* lightManager = action->GetScene()->GetLightManager();

		//if (!depthMaterial)
		//{
		//	depthMaterial = static_cast<DepthMaterial*>( rsmgr->GetOrCreateMaterial("shadowDepthMaterial",MaterialType_Depth));
		//}
		for (int i = 0, l = lightManager->GetAllLight().size(); i < l; i++)
		{
			BaseLight* light = lightManager->GetAllLight()[i];
			if (!light->CastShadow())
			{
				continue;
			}

			LightShadow* shadow = light->GetLightShadow();

			CameraNode* camera = shadow->GetCamera();
			if (!shadow->RenderTarget())
			{
				HardWareFrameBuffer* temp = new HardWareFrameBuffer;
				temp->SetParameters();
				temp->SetSize((int)shadow->GetMapSize().m_x, (int)shadow->GetMapSize().m_y);
				temp->SetResourceManager(rsmgr);
				shadow->RenderTarget(temp);
			}
			float sceneBoxLength = 0;
			if (light->GetLightSourceType() == LightType_Directional)
			{
				sceneBoxLength = action->GetScene()->GetSceneBox().Length();
				DirectionalLightShadow* directionalLightShadow = static_cast<DirectionalLightShadow*>(shadow);
				directionalLightShadow->SetCameraInfo(sceneBoxLength, sceneBoxLength*NEAR_CLIP_PLANE_FACTOR, sceneBoxLength*FAR_CLIP_PLANE_FACTOR * 3);
			}

			HardWareFrameBuffer* shadowTarget = shadow->RenderTarget();
			renderTarget = shadowTarget;
			shadowTarget->ReShape();
			shadowTarget->Bind();
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			int faces = light->GetLightSourceType() == LightType_Point ? 6 : 1;
			bool isPointLight = light->GetLightSourceType() == LightType_Point;
			for (int f = 0; f < faces; f++)
			{
				if (isPointLight)
				{
					//TODO
				}
				else
				{
					shadow->Update(light, action->GetCamera());
				}

				if (isPointLight)
				{
					//TODO
				}
				else
				{
					glViewport(0, 0, (int)shadow->GetMapSize().m_x, (int)shadow->GetMapSize().m_y);
				}

				Matrix4& lightMatrix = shadow->Matrix();
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				BaseMaterial* depthMaterial = GetDepthMaterial(light, rsmgr, isPointLight);
				m_currentCamera = 0;
				m_currentMaterial = 0;
				RenderabelArray& renderableArray = RenderStateArray->GetRenderableArray();
				for (int i = 0; i < renderableArray.size(); i++)
				{
					Face* renderable = static_cast<Face*>(renderableArray[i]);
					if (camera->GetFrustum().IsInsideFast(renderable->GetBody()->GetModel()->GetWorldBoundingBox()) == 0)
					{
						continue;
					}

					RenderFace(action, renderable, camera, depthMaterial);
				}
				glDisable(GL_CULL_FACE);

			}

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			shadowTarget->UnBind();


		}

		CameraNode* mainCamera = action->GetScene()->GetCamera();
		const IntRect& intRect = mainCamera->GetViewPort().GetRect();
#if 0 //测试深度图
		glViewport(0, 50, 300, 300);
		DrawFrameBufferDebug(action);
#endif
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

	}

	void GLShapeDrawer20::ShadowRenderPass(RenderAction * action)
	{
	}

	void GLShapeDrawer20::RefreshUniformsPhong(RenderAction*action, UniformMap * unifomValueList, Material* material)
	{
		if (material&& unifomValueList)
		{
			material->Program()->SetUniformValue((DIFFUSE), material->GetDiffuse());
			material->Program()->SetUniformValue((SPECULAR), material->GetSpecular());
			material->Program()->SetUniformValue((EMISSIVE), material->GetEmissive());
			material->Program()->SetUniformValue((SHININESS), material->GetShininess());
			material->Program()->SetUniformValue(OPACITY, material->Opacity());

			if (material->GetEmissiveMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, EMISSIVE_MAP, "Texture2D", static_cast<Texture2D*>(material->GetEmissiveMap()));
			}

			if (material->GetDiffuseMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, DIFFUSE_TEXTURE, "Texture2D", static_cast<Texture2D*>(material->GetDiffuseMap()));
			}

			if (material->GetSpecularMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, SPECULAR_MAP, "Texture2D", static_cast<Texture2D*>(material->GetSpecularMap()));
			}

			if (material->GetNormalMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP, "Texture2D", static_cast<Texture2D*>(material->GetNormalMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP_SCALE, "Vector2", &material->NormalMapScale());
			}

			if (material->MatcapMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, MATCAP_MAP, "Texture2D", static_cast<Texture2D*>(material->MatcapMap()));
			}

			if (material->DisplacementMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_MAP, "Texture2D", static_cast<Texture2D*>(material->DisplacementMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_SCALE, "Float", material->DisplacementScale());
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_BIAS, "Float", material->DisplacementBias());
			}

			//if (material->GetDiffuseMap())
			//{
			//	UniformHelper::SetUniformValueToMap(unifomValueList, "diffuseTexture", "Texture2D", material->GetDiffuseMap());
			//}


		}

	}

	void GLShapeDrawer20::RefreshUniformsPbr(RenderAction*action, UniformMap* unifomValueList, PbrMaterial* material)
	{
		ResourceManager * rsgMgr = action->GetScene()->GetResourceManager();
		if (material&& unifomValueList)
		{
			BoundingBox& sceneBox = action->GetScene()->GetSceneBox();
			Vector3 pos = sceneBox.Center();
			Vector3& minPos = sceneBox.m_min;
			Vector3& maxPos = sceneBox.m_max;
			float length = sceneBox.Length();
			Vector3 vLenght = Vector3(length / 2, length / 2, length / 2);
			Vector3 BMin = pos - vLenght;
			Vector3 BMax = pos + vLenght;
			material->Program()->SetUniformValue(string("cubeMapWorldPosition"), pos);
			material->Program()->SetUniformValue(string("boxMin"), BMin);
			material->Program()->SetUniformValue(string("boxMax"), BMax);

			material->Program()->SetUniformValue((DIFFUSE), material->AlbedoColor());
			material->Program()->SetUniformValue((EMISSIVE), material->EmissiveColor());
			material->Program()->SetUniformValue((ROUGHNESS), material->RougthnessFactor());
			material->Program()->SetUniformValue((METALNESS), material->MetalnessFactor());
			material->Program()->SetUniformValue((GAMMA), Parameters::Instance()->m_gammaFactor);
			material->Program()->SetUniformValue((TONE_MAPPING_EXPOSURE), Parameters::Instance()->m_toneMappingExposure);
			material->Program()->SetUniformValue(OPACITY, material->Opacity());
			if (material->UseClearCoat())
			{
				material->Program()->SetUniformValue((CLEARCOAT), material->ClearCoat());
				material->Program()->SetUniformValue((CLEARCOATROUGHNESS), material->ClearCoatRoughness());
			}


			if (material->AlbedoMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, DIFFUSE_TEXTURE, "Texture2D", static_cast<Texture2D*>(material->AlbedoMap()));
			}

			if (material->EnvMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_TEXTURE, "TextureCube", static_cast<TextureCube*>(material->EnvMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_MAP_INTENSITY, "Float", material->EnvMapIntensity());
			}
			else
			{
				material->EnvMap(rsgMgr->DefaultPBRSpecularTexture());
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_TEXTURE, "TextureCube", static_cast<TextureCube*>(rsgMgr->DefaultPBRSpecularTexture()));
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_MAP_INTENSITY, "Float", material->EnvMapIntensity());
			}

			if (material->EnvIrradianceMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_DIFFUSE_TEXTURE, "TextureCube", static_cast<TextureCube*>(material->EnvIrradianceMap()));
			}
			else
			{
				material->EnvIrradianceMap(rsgMgr->DefaultPBRDiffuseTexture());
				TextureCube* temp = static_cast<TextureCube*>(rsgMgr->DefaultPBRDiffuseTexture());
				UniformHelper::SetUniformValueToMap(unifomValueList, ENV_DIFFUSE_TEXTURE, "TextureCube", temp);
			}

			if (material->EmissiveMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, EMISSIVE_MAP, "Texture2D", static_cast<Texture2D*>(material->EmissiveMap()));
			}

			if (material->AmbientOcclusiontMap())
			{
				material->Program()->SetUniformValue((AO_MAP_INTENSITY), material->AoMapIntensity());
				UniformHelper::SetUniformValueToMap(unifomValueList, AO_MAP, "Texture2D", static_cast<Texture2D*>(material->AmbientOcclusiontMap()));
			}

			if (material->NormalMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP, "Texture2D", static_cast<Texture2D*>(material->NormalMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP_SCALE, "Vector2", &material->NormalMapScale());
			}

			if (material->DisplacementMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_MAP, "Texture2D", static_cast<Texture2D*>(material->DisplacementMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_SCALE, "Float", material->DisplacementScale());
				UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_BIAS, "Float", material->DisplacementBias());
			}

			if (material->MetalnessRoughnessMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, METALLIC_ROUGHNESS_TEXTURE, "Texture2D", static_cast<Texture2D*>(material->MetalnessRoughnessMap()));
			}


			UniformHelper::SetUniformValueToMap(unifomValueList, LUT, "Texture2D", static_cast<Texture2D*>(rsgMgr->DefaultPBRLUTTexture()));




		}
	}

	void GLShapeDrawer20::RefreshUniformsMatCap(RenderAction*action, UniformMap* unifomValueList, MatCapMaterial* material)
	{
		if (material&& unifomValueList)
		{
			material->Program()->SetUniformValue((DIFFUSE), material->GetDiffuse());
			material->Program()->SetUniformValue((SPECULAR), material->GetSpecular());
			material->Program()->SetUniformValue((EMISSIVE), material->GetEmissive());
			material->Program()->SetUniformValue((SHININESS), material->GetShininess());
			material->Program()->SetUniformValue(OPACITY, material->Opacity());

			//if (material->GetDiffuseMap())
			//{
			//	UniformHelper::SetUniformValueToMap(unifomValueList, DIFFUSE_TEXTURE, "Texture2D", static_cast<Texture2D*>(material->GetDiffuseMap()));
			//}

			//if (material->GetSpecularMap())
			//{
			//	UniformHelper::SetUniformValueToMap(unifomValueList, SPECULAR_MAP, "Texture2D", static_cast<Texture2D*>(material->GetSpecularMap()));
			//}

			if (material->GetNormalMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP, "Texture2D", static_cast<Texture2D*>(material->GetNormalMap()));
				UniformHelper::SetUniformValueToMap(unifomValueList, NORMAL_MAP_SCALE, "Vector2", &material->NormalMapScale());
			}

			if (material->MatcapMap())
			{
				UniformHelper::SetUniformValueToMap(unifomValueList, MATCAP_MAP, "Texture2D", static_cast<Texture2D*>(material->MatcapMap()));
			}

			//if (material->DisplacementMap())
			//{
			//	UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_MAP, "Texture2D", static_cast<Texture2D*>(material->DisplacementMap()));
			//	UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_SCALE, "Float", material->DisplacementScale());
			//	UniformHelper::SetUniformValueToMap(unifomValueList, DISPLACEMENT_BIAS, "Float", material->DisplacementBias());
			//}

			//if (material->GetDiffuseMap())
			//{
			//	UniformHelper::SetUniformValueToMap(unifomValueList, "diffuseTexture", "Texture2D", material->GetDiffuseMap());
			//}


		}
	}

	ShaderProgram* GLShapeDrawer20::m_currentProgram = NULL;

	BaseMaterial* GLShapeDrawer20::m_currentMaterial = NULL;

	CameraNode* GLShapeDrawer20::m_currentCamera = NULL;

	bool GLShapeDrawer20::m_ssaoLastState = false;

	bool GLShapeDrawer20::m_shadowState = false;

	DepthMaterial* GLShapeDrawer20::depthMaterial = NULL;

	HardWareFrameBuffer* GLShapeDrawer20::renderTarget = NULL;

	GLuint GLShapeDrawer20::_usedTextureUnits = 0;
	/**
	 * sh
	 */
	void GLShapeDrawer20::DrawTriWithIndex(HardWareVertexBuffer* vertexBuffer, HardWareIndexBuffer * indexBuffer,
		int dataLength, M3D_OFFSET_TYPE indexOffset)
	{
		char* indexAddress = (char*)indexBuffer->Bind();
		M3D_GL_ERROR_CHECK
			glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *)(indexAddress + indexOffset));
		M3D_GL_ERROR_CHECK
			indexBuffer->UnBind();
		M3D_GL_ERROR_CHECK
	}

	//draw background color
	void GLShapeDrawer20::DrawBackGround(SceneNode* node, RenderAction* action)
	{
		ShaderManager * shaderManager = NULL;
		if (!action)
		{
			return;
		}

		BackgroundNode * backgroundNode = (BackgroundNode*)node;
		shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}

		ShaderProgram * backgroundEffect = shaderManager->GetEffect(
			ShaderManager::Background);

		if (!backgroundEffect)
		{
			return;
		}

		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		const IntRect& intRect = camera->GetViewPort().GetRect();
#ifdef WIN32
		if (camera->m_cameraMode == 1)
		{
			glViewport(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else if (camera->m_cameraMode == 2)
		{
			glViewport(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else
		{
			glViewport(intRect.m_left, intRect.m_top, intRect.Width(), intRect.Height());
		}
#else
		if (camera->m_cameraMode == 1)
		{
			camera->SetAspectRatio((intRect.Width() / 2.0f) / intRect.Height());
			glScissor(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
			glViewport(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else if (camera->m_cameraMode == 2)
		{
			camera->SetAspectRatio((intRect.Width() / 2.0f) / intRect.Height());
			glScissor(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
			glViewport(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else
		{
			camera->SetAspectRatio((intRect.Width()*1.0f) / intRect.Height());
			glViewport(intRect.m_left, intRect.m_top, intRect.Width(), intRect.Height());
		}

#endif // WIN32
		//设置投影矩阵 模型视图矩阵 一些初始化

		Matrix4 modelViewMatrix, projectionMatrix, modelMat;

		//设置投影矩阵
		projectionMatrix.Ortho(backgroundNode->m_ProArray[0],
			backgroundNode->m_ProArray[1], backgroundNode->m_ProArray[2],
			backgroundNode->m_ProArray[3], backgroundNode->m_ProArray[4],
			backgroundNode->m_ProArray[5]);
		//设置模型视图矩阵
		modelViewMatrix.ToIdentity();
		modelMat.ToIdentity();

		//设置视口
		//glViewport(backgroundNode->m_iViewX, backgroundNode->m_iViewY,
		//		backgroundNode->m_iWidth, backgroundNode->m_iHeight);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		if (backgroundNode->IsUseColor() || (backgroundNode->IsUseImage() == false && backgroundNode->IsUseSkyBox() == false)) {

			backgroundEffect->UseProgram();
			ShaderParameter * position = backgroundEffect->GetShaderAttributeParameter(VSP_POSITION);
			ShaderParameter * color = backgroundEffect->GetShaderAttributeParameter(VSP_COLOR);

			backgroundEffect->SetVertexAttribPointer(position->m_location, 3, GL_FLOAT, 0, backgroundNode->m_BackPnt);
			backgroundEffect->EnableAttributeArray(position->m_location);

			backgroundEffect->SetVertexAttribPointer(color->m_location, 4, GL_FLOAT, 0, backgroundNode->m_BackColor);
			backgroundEffect->EnableAttributeArray(color->m_location);

			Matrix4 mvp = projectionMatrix *modelViewMatrix;

			backgroundEffect->SetUniformValue(VSP_MVPMAT, mvp);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			backgroundEffect->DisableAttributeArray(position->m_location);
			backgroundEffect->DisableAttributeArray(color->m_location);
			backgroundEffect->ReleaseShaderProgram();
		}
		else if (backgroundNode->IsUseSkyBox())
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			backgroundEffect = shaderManager->GetEffect(
				ShaderManager::CubeMap);

			backgroundEffect->UseProgram();

			CameraNode *camera = action->GetCamera();
			RenderContext* gl = action->GetGLContext();
			Texture* texcube = backgroundNode->GetSkyBoxTexture();
			if (!texcube)
			{
				texcube = action->GetScene()->GetResourceManager()->GetDefaultCubeMap();
				if (!texcube)
				{
					return;
				}
			}
			unsigned int cubeMapObj = texcube->GetOGLObj();
			if (cubeMapObj)
			{

				GLfloat skyboxVertices[] = {
					// Positions
					-1.0f,  1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					 1.0f, -1.0f, -1.0f,
					 1.0f, -1.0f, -1.0f,
					 1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,

					-1.0f, -1.0f,  1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f,  1.0f,
					-1.0f, -1.0f,  1.0f,

					 1.0f, -1.0f, -1.0f,
					 1.0f, -1.0f,  1.0f,
					 1.0f,  1.0f,  1.0f,
					 1.0f,  1.0f,  1.0f,
					 1.0f,  1.0f, -1.0f,
					 1.0f, -1.0f, -1.0f,

					-1.0f, -1.0f,  1.0f,
					-1.0f,  1.0f,  1.0f,
					 1.0f,  1.0f,  1.0f,
					 1.0f,  1.0f,  1.0f,
					 1.0f, -1.0f,  1.0f,
					-1.0f, -1.0f,  1.0f,

					-1.0f,  1.0f, -1.0f,
					 1.0f,  1.0f, -1.0f,
					 1.0f,  1.0f,  1.0f,
					 1.0f,  1.0f,  1.0f,
					-1.0f,  1.0f,  1.0f,
					-1.0f,  1.0f, -1.0f,

					-1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f,  1.0f,
					 1.0f, -1.0f, -1.0f,
					 1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f,  1.0f,
					 1.0f, -1.0f,  1.0f
				};
				ShaderParameter * vertex = backgroundEffect->GetShaderAttributeParameter(
					VSP_POSITION);
				backgroundEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
					(void *)skyboxVertices);
				backgroundEffect->EnableAttributeArray(vertex->m_location);
				int sampler0 = 0;
				Matrix4 mat(camera->GetView().ToMatrix3());
				CameraNode cameraP;
				cameraP.SetFov(90.0);
				cameraP.SetNearClip(0.1f);
				cameraP.SetFarClip(100.0f);
				cameraP.SetAspectRatio(camera->GetAspectRatio());
				cameraP.SetOrthographic(false);
				Matrix4 per = cameraP.GetProjection();
				per = per.Transpose();
				backgroundEffect->SetUniformValue(FSP_SAMPLERCUBE0, 1, &sampler0);
				backgroundEffect->SetUniformValue(VSP_MODELMAT, modelMat);
				backgroundEffect->SetUniformValue(VSP_VIEWMAT, mat.Transpose());
				backgroundEffect->SetUniformValue(VSP_PROJECTIONMAT, per);
				// 启用纹理
				glActiveTexture(GL_TEXTURE0);
				glEnable(GL_TEXTURE_CUBE_MAP);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapObj);
				//		LOGI("Draw Point1111");
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				glDisable(GL_TEXTURE_CUBE_MAP);
				backgroundEffect->DisableAttributeArray(vertex->m_location);
				backgroundEffect->ReleaseShaderProgram();
				//			LOGI("sphereMapObj end");
			}
			glDepthMask(GL_TRUE);
			glDisable(GL_DEPTH_TEST);
		}
		else if (backgroundNode->IsUseImage())
		{
			backgroundEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			backgroundEffect->UseProgram();

			Texture* texture = backgroundNode->GetTexture();
			if (texture)
			{
				unsigned int OGLObj = texture->GetOGLObj();
				if (OGLObj)
				{
					Color VoiceBackColor(1.0f, 1.0f, 1.0f, 1.0f);
					ShaderParameter * vertex = backgroundEffect->GetShaderAttributeParameter(
						VSP_POSITION);
					backgroundEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
						(void *)backgroundNode->GetVertexs()->Data());
					backgroundEffect->EnableAttributeArray(vertex->m_location);

					ShaderParameter * texCoords = backgroundEffect->GetShaderAttributeParameter(
						VSP_TEXCOORDS);
					backgroundEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT,
						0, (void *)backgroundNode->GetTextureCoords()->Data());
					backgroundEffect->EnableAttributeArray(texCoords->m_location);

					// multi-clip planes
					Vector4 tc[3];
					for (int i = 0; i < 3; i++)
					{
						tc[i] = (action->m_clipPlane[i]);
					}
					backgroundEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
					int enableClips[3] = { 0 };
					backgroundEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
					ShaderParameter* reverseClipPara = backgroundEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
					backgroundEffect->SetUniformValue(reverseClipPara->m_location, 0);
					// LOGI("end use multi-clip planes");

					int sampler0 = 0;
					backgroundEffect->SetUniformValue(FSP_SAMPLER0, 1, &sampler0);
					backgroundEffect->SetUniformValue(FSP_DIFFUSE, VoiceBackColor);

					backgroundEffect->SetUniformValue(VSP_MODELMAT, modelMat);
					backgroundEffect->SetUniformValue(VSP_VIEWMAT, modelViewMatrix);
					backgroundEffect->SetUniformValue(VSP_PROJECTIONMAT, projectionMatrix);

					// 启用纹理
					glActiveTexture(GL_TEXTURE0);
					glEnable(GL_TEXTURE_2D);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glBindTexture(GL_TEXTURE_2D, OGLObj);
					//		LOGI("Draw Point1111");
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_TEXTURE_2D);
					backgroundEffect->DisableAttributeArray(vertex->m_location);
					backgroundEffect->DisableAttributeArray(texCoords->m_location);
					backgroundEffect->ReleaseShaderProgram();
				}
			}
		}
		
		if (backgroundNode->IsUseWaterMark())
		{
			backgroundEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			backgroundEffect->UseProgram();

			Texture* texture = backgroundNode->GetTextureOfWaterMark();
			if (texture)
			{
				//vector<Vector2> uv;
				//uv.push_back(Vector2(0, 0));
				//uv.push_back(Vector2(5, 0));
				//uv.push_back(Vector2(0, 3));
				//uv.push_back(Vector2(0, 3));
				//uv.push_back(Vector2(5, 0));
				//uv.push_back(Vector2(5, 3));
				//Vector2[] uv = { Vector2(0,5), Vector2(5,5), Vector2(5,0), Vector2(0,0) };
				//glTexCoordPointer(2, GL_FLOAT, 0, uv.data());
				
				//const float* uvd = uv.data()->Data();
				//Vector3* aa = backgroundNode->GetVertexs();
				//const float* a = backgroundNode->GetVertexs()->Data();
				//Vector2* bb = backgroundNode->GetTextureCoords();
				//const float* b = backgroundNode->GetTextureCoords()->Data();

				unsigned int OGLObj = texture->GetOGLObj();
				if (OGLObj)
				{
					Color VoiceBackColor(1.0f, 1.0f, 1.0f, 1.0f);
					ShaderParameter * vertex = backgroundEffect->GetShaderAttributeParameter(
						VSP_POSITION);
					backgroundEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
						(void *)backgroundNode->GetVertexs()->Data());
					backgroundEffect->EnableAttributeArray(vertex->m_location);

					ShaderParameter * texCoords = backgroundEffect->GetShaderAttributeParameter(
						VSP_TEXCOORDS);
					backgroundEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT,
						0, (void *)backgroundNode->GetTextureCoords()->Data()/*uv.data()->Data()*/);
					backgroundEffect->EnableAttributeArray(texCoords->m_location);
					
					// multi-clip planes
					Vector4 tc[3];
					for (int i = 0; i < 3; i++)
					{
						tc[i] = (action->m_clipPlane[i]);
					}
					backgroundEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
					int enableClips[3] = { 0 };
					backgroundEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
					ShaderParameter* reverseClipPara = backgroundEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
					backgroundEffect->SetUniformValue(reverseClipPara->m_location, 0);
					// LOGI("end use multi-clip planes");

					int sampler0 = 0;
					backgroundEffect->SetUniformValue(FSP_SAMPLER0, 1, &sampler0);
					backgroundEffect->SetUniformValue(FSP_DIFFUSE, VoiceBackColor);

					backgroundEffect->SetUniformValue(VSP_MODELMAT, modelMat);
					backgroundEffect->SetUniformValue(VSP_VIEWMAT, modelViewMatrix);
					backgroundEffect->SetUniformValue(VSP_PROJECTIONMAT, projectionMatrix);					

					// 启用纹理
					glActiveTexture(GL_TEXTURE0);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
					//glDisable(GL_DEPTH_TEST);
					glEnable(GL_TEXTURE_2D);
					//glEnable(GL_BLEND);
					//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.1f);

					glBindTexture(GL_TEXTURE_2D, OGLObj);
					//		LOGI("Draw Point1111");					
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_ALPHA_TEST);
					//glDisable(GL_BLEND);
					glDisable(GL_TEXTURE_2D);
					backgroundEffect->DisableAttributeArray(vertex->m_location);
					backgroundEffect->DisableAttributeArray(texCoords->m_location);
					backgroundEffect->ReleaseShaderProgram();
				}
			}
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void GLShapeDrawer20::DrawVRBackGround(RenderAction *action)
	{

		ShaderManager * shaderManager = NULL;
		if (!action)
		{
			return;
		}
		shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}

		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		const IntRect& intRect = camera->GetViewPort().GetRect();
		//	if (camera->m_cameraMode == 1)
		//	{
		//		glViewport(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
		//	}
		//	else if (camera->m_cameraMode == 2)
		//	{
		//		glViewport(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
		//	}
		//	else
		//	{
		//		glViewport(intRect.m_left, intRect.m_top, intRect.Width(), intRect.Height());
		//	}
		//#else

			//设置投影矩阵 模型视图矩阵 一些初始化

		Matrix4 modelViewMatrix, projectionMatrix, modelMat;
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		ShaderProgram * backgroundEffect = NULL;
		backgroundEffect = shaderManager->GetEffect(
			ShaderManager::CubeMap);

		backgroundEffect->UseProgram();
		RenderContext* gl = action->GetGLContext();
		Matrix4 P, V;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		unsigned int cubeMapObj = action->GetScene()->GetResourceManager()->GetDefaultCubeMap()->GetOGLObj();
		{

			GLfloat skyboxVertices[] = {
				// Positions
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f
			};
			ShaderParameter * vertex = backgroundEffect->GetShaderAttributeParameter(
				VSP_POSITION);
			backgroundEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				(void *)skyboxVertices);
			backgroundEffect->EnableAttributeArray(vertex->m_location);
			int sampler0 = 0;
			Matrix4 mat(camera->GetView().ToMatrix3());
			//			CameraNode cameraP;
			//			cameraP.SetFov(90.0);
			//			cameraP.SetNearClip(0.1f);
			//			cameraP.SetFarClip(100.0f);
			//			cameraP.SetAspectRatio(camera->GetAspectRatio());
			//			cameraP.SetOrthographic(false);
			//			Matrix4 per = cameraP.GetProjection();
			//			per = per.Transpose();
						//TODO 设置相机
			backgroundEffect->SetUniformValue(FSP_SAMPLERCUBE0, 1, &sampler0);
			backgroundEffect->SetUniformValue(VSP_MODELMAT, modelMat);
			backgroundEffect->SetUniformValue(VSP_VIEWMAT, mat.Transpose());
			//			backgroundEffect->SetUniformValue(VSP_PROJECTIONMAT, per);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapObj);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDisable(GL_TEXTURE_CUBE_MAP);
			backgroundEffect->DisableAttributeArray(vertex->m_location);
			backgroundEffect->ReleaseShaderProgram();

		}
		glDepthMask(GL_TRUE);
		glDisable(GL_DEPTH_TEST);
	}

	void GLShapeDrawer20::DrawTriNoIndex(HardWareVertexBuffer* vertexBuffer, int dataLength)
	{
		glDrawArrays(GL_TRIANGLES, 0, dataLength);
	}

	void GLShapeDrawer20::DrawTriNoIndex(int num)
	{
		glDrawArrays(GL_TRIANGLES, 0, num);
	}

	void GLShapeDrawer20::DrawTriWithIndex(M3D_INDEX_TYPE * indexBuffer, int num)
	{
		int dataLength = num;
		glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *)(indexBuffer));
	}

	void GLShapeDrawer20::DrawImageBoard(RenderAction* action, ImageBoard* imageboard, Matrix4 & V, Matrix4 & P, Matrix4 & modelMat)
	{
		ShaderManager * shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}
		Vector4 tc[3];
		int enableClips[3] = { 0 };
		ShaderProgram * shaderEffect = shaderManager->GetEffect(
			ShaderManager::NoteImage);
		shaderEffect->UseProgram();
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

		DrawImageBoard(imageboard, V, P, modelMat,
			shaderEffect, false);

		shaderEffect->ReleaseShaderProgram();
	}

	void GLShapeDrawer20::DrawImageBoard(ImageBoard* imageboard, Matrix4 & V, Matrix4 & P, Matrix4 & modelMat,
		ShaderProgram* shaderEffect, bool isAllowDepthTest)
	{
		//	LOGI("draw image borad");
		if (!imageboard)
		{
			return;
		}
		Texture* image = imageboard->GetTexture();
		if (!image)
		{
			return;
		}
		glEnable(GL_BLEND);
		float useMinDepth = 0;
		ShaderParameter* useMinDepthPra = shaderEffect->GetShaderUniformParameter(
			string("u_useMinDepth"));
		if (isAllowDepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			if (useMinDepthPra)
			{
				useMinDepth = 1;
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			if (useMinDepthPra)
			{
				useMinDepth = 0;
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
		}
        //是否前端显示
        if (imageboard->GetInTop()) {
            glDisable(GL_DEPTH_TEST);
        }else{
            glEnable(GL_DEPTH_TEST);
        }
		int textGLObj = image->GetOGLObj();
		if (textGLObj > 0)
		{
			Matrix4 M, MVP;
			//		LOGI("draw image borad1");
			M = (*(imageboard->GetRenderWorldMatrix())) * modelMat;
			const Color& VoiceBackColor = imageboard->GetRenderColor();

			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				(void *)imageboard->GetVertexs()->Data());
			shaderEffect->EnableAttributeArray(vertex->m_location);

			ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
			shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0,
				(void *)imageboard->GetTextureCoords()->Data());
			shaderEffect->EnableAttributeArray(texCoords->m_location);

			int sampler0 = 0;
			shaderEffect->SetUniformValue(FSP_SAMPLER0, 1, &sampler0);
			shaderEffect->SetUniformValue(FSP_DIFFUSE, VoiceBackColor);

			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);

			// 启用纹理
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, textGLObj);
			//		LOGI("Draw Point1111");
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->DisableAttributeArray(texCoords->m_location);
			glDisable(GL_BLEND);
		}
		else
		{
			LOGI("Draw Point Error texObj is null");
		}
	}

	void GLShapeDrawer20::DrawImageBoard(ImageBoard* imageboard, CameraNode * camera, Matrix4 & modelMat,
		ShaderProgram* shaderEffect, bool isNote)
	{
		Matrix4 V, P, M, MVP;
		if (camera)
		{
			P = camera->GetProjection().Transpose();
			V = camera->GetView().ToMatrix4().Transpose();
		}
		DrawImageBoard(imageboard, V, P, modelMat, shaderEffect, isNote);
	}

	void GLShapeDrawer20::DrawMeshBoard(MeshBoard* meshboard, CameraNode * camera, Matrix4 & modelMat,
		ShaderProgram* shaderEffect)
	{
		if (!meshboard)
		{
			return;
		}
		Matrix4 V, P, M, MVP;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		M = (*(meshboard->GetRenderWorldMatrix())) * modelMat;
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, meshboard->GetLinesVertexs()->Data());
		shaderEffect->EnableAttributeArray(vertex->m_location);
		const Color& pntColor = meshboard->GetRenderColor();
		shaderEffect->SetUniformValue(FSP_DIFFUSE, pntColor);

		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);

		glDrawArrays(GL_LINES, 0, 4);
		shaderEffect->DisableAttributeArray(vertex->m_location);

	}

	void GLShapeDrawer20::DrawPointHandler(HandlerPoint* handlerPoint, ShaderManager * shaderManager, CameraNode* camera,
		Matrix4 &modelMat)
	{
		if (!handlerPoint && (!handlerPoint->IsVisible()))
		{
			return;
		}
		Point* point = handlerPoint->GetPoint();

		DrawPoint(point, shaderManager, camera, modelMat);
	}

	void GLShapeDrawer20::DrawMeshHandler(Handler* handler)
	{
		//	if (!handler)
		//	{
		//		return;
		//	}
		//
		//	glPushMatrix();
		//
		//	glDisable(GL_LIGHTING);
		//	glDisable(GL_BLEND);
		//	glDisable(GL_DEPTH_TEST);
		//
		//	glEnableClientState(GL_VERTEX_ARRAY);
		//	glEnableClientState(GL_NORMAL_ARRAY);
		//	glDisableClientState(GL_COLOR_ARRAY);
		//
		//	int dataLength = handler->GetDataLength();
		//	bool isUseIndex = handler->IsUseIndex();
		//	int vertexNum = handler->GetUseIndexVertexNumber();
		//
		//	if (dataLength > 0)
		//	{
		//		const float* color = handler->GetRenderColor().Data();
		//		glColor4f(color[0], color[1], color[2], color[3]);
		//
		//		Vector3* positionArray = handler->GetVertexPos();
		//
		//		if (isUseIndex)
		//		{
		//			M3D_INDEX_TYPE* indexArray = handler->GetIndex();
		//			GLShapeDrawer20::DrawTriWithIndex(positionArray, NULL, indexArray,
		//					dataLength);
		//		}
		//		else
		//		{
		//			GLShapeDrawer20::DrawTriNoIndex(positionArray, NULL, dataLength);
		//		}
		//	}
		//
		//	glPopMatrix();
	}

	void GLShapeDrawer20::DrawTexture2DWithIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer,
		HardWareIndexBuffer * indexBuffer, Matrix4* texture2DTransform, int dataLength, M3D_OFFSET_TYPE indexOffset)
	{
		if (OGLObj)
		{
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, OGLObj);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			indexBuffer->Bind();
			vertexBuffer->Bind();
			LOGI("sphereMapObj end DrawTexture2DWithIndex");
			glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *)(indexOffset));
			indexBuffer->UnBind();
			vertexBuffer->UnBind();

			glBindTexture(GL_TEXTURE_2D, 0);

			glDisable(GL_TEXTURE_2D);

			//		glDisable(GL_BLEND);

		}
	}

	void GLShapeDrawer20::DrawTexture2DNoIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer,
		Matrix4* texture2DTransform, int dataLength)
	{
		if (OGLObj)
		{
			// 启用纹理
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, OGLObj);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			vertexBuffer->Bind();
			glDrawArrays(GL_TRIANGLES, 0, dataLength);
			vertexBuffer->UnBind();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}
	}

	bool GLShapeDrawer20::DoSection(RenderAction* action, bool state)
	{
		vector<Vector4> & clipPlane = action->m_clipPlane;
		vector<int> & enableClip = action->m_enableClip;

		//clipPlane.clear();
		//enableClip.clear();

		Section* section = (Section*)action->GetSection();
		//若果section没有数据则表示不需要进行剖切操作，可以返回
		if (!section)
		{
			//		glEnable(GL_CULL_FACE);
			return false;
		}
		glDisable(GL_CULL_FACE);
		list<SectionPlane*>* planeList = section->GetPlaneList();

		if (state)
		{
			list<SectionPlane*>::iterator iter;
			int i = 0;
			if (planeList->size() > 0)
			{
				for (iter = planeList->begin(); iter != planeList->end(); ++iter, ++i)
				{
					float * tempPointer = (*iter)->GetTransformPlaneParam();
					clipPlane[i].m_x = tempPointer[0];
					clipPlane[i].m_y = tempPointer[1];
					clipPlane[i].m_z = tempPointer[2];
					clipPlane[i].m_w = tempPointer[3];
					//LOGI("---------tempPointer %d = %f", j, clipPlane[4*i+j]);

					if ((*iter)->GetEnable())
						enableClip[i] = 1; //表示开启剪裁面
					else
						enableClip[i] = 0;
					if (Parameters::Instance()->m_clipPlaneMode == 1)
					{
						enableClip[i] = 0;
					}
					//LOGI("---------tempPointer %d ",enableClip[i]);
				}
				action->m_bReverseClip = section->IsReverseClipping();
			}
			else
			{
				for (i = 0; i < enableClip.size(); i++)
				{
					enableClip[i] = 0;
				}
				action->m_bReverseClip = false;
				return false;
			}
		}
		else
		{
			list<SectionPlane*>::iterator iter;

			int i = 0;

			//for (iter = planeList->begin(); iter != planeList->end(); ++iter, ++i)
			//{
			//	enableClip[i] = 0; //表示关闭剪裁面
			//}
			//if (Parameters::Instance()->m_showSection)
			{
				for (iter = planeList->begin(); iter != planeList->end(); ++iter)
				{
					//LOGI("BEGIN section");
					if ((*iter)->GetEnable())
						DrawSectionPlane(action, *iter);

				}

#ifdef NOCLOTH
				for (iter = planeList->begin(); iter != planeList->end(); ++iter)
				{
					//LOGI("BEGIN section");
	//				LOGI("*iter == %p",*iter);
					DrawCrossSection(action, *iter);

			}
#endif
		}


			if (Parameters::Instance()->m_isShowCutLine)
			{
				for (iter = planeList->begin(); iter != planeList->end(); ++iter)
				{
					//LOGI("BEGIN section");
					if ((*iter)->GetEnable())
						DrawSectionLines(action, (*iter)->m_figureSectonLine.linesData, (*iter)->GetID()); //TODO 剖切线，盖面

				}
			}
	}

		if (planeList->size() > 0)
		{
			list<SectionPlane*>::iterator iter = planeList->begin();
			if (iter != planeList->end() && (*iter)->GetShowCappingPlane())
			{
				return true;
			}
		}
		return false;
		//if(section->IsShowCappingPlane())
		//{
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}

	}

	/**
	 * 绘制盖面多边形
	 */
	void GLShapeDrawer20::DrawCapPolygon(RenderAction* action)
	{
		vector<Vector4> & clipPlane = action->m_clipPlane;
		vector<int> & enableClip = action->m_enableClip;

		//clipPlane.clear();
		//enableClip.clear();
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}

		Section* section = (Section*)action->GetSection();
		//若果section没有数据则表示不需要进行剖切操作，可以返回
		if (!section)
		{
			return;
		}

		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4();
		int i = 0;
		for (i = 0; i < 3; i++)
		{
			tc[i] = clipPlane[i];
			tc[i] = tempMat.Inverse().Transpose() * tc[i];
			enableClips[i] = enableClip[i];
		}

		list<SectionPlane*>* planeList = section->GetPlaneList();

		list<SectionPlane*>::iterator iter;

		for (i = 0, iter = planeList->begin(); iter != planeList->end(); ++iter, i++)
		{

			SectionPlane* sectionPlane = *iter;
			if (sectionPlane)
			{
				if (!sectionPlane->GetEnable())
					continue;

				//			const IntRect& intRect = camera->GetViewPort().GetRect();
				//			glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
				glDisable(GL_BLEND);
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
				glLineWidth(1);
				Matrix4 V, P, M, MVP;
				P = camera->GetProjection().Transpose();
				V = camera->GetView().ToMatrix4().Transpose();
				ShaderManager* shaderMananger = action->GetShaderMananger();
				ShaderProgram* shaderEffect = shaderMananger->GetEffect(ShaderManager::Edge);
				shaderEffect->UseProgram();
				shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
				shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				// multi-clip plane
				shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
				enableClips[i] = 0;
				shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
				ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
				shaderEffect->SetUniformValue(reverseClipPara->m_location, 0);

				ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
				shaderEffect->EnableAttributeArray(vertex->m_location);
				Color faceColor(0.6, 0.3, 0.0);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetPointArray());

				shaderEffect->SetUniformValue(FSP_DIFFUSE, faceColor);
				DrawTriNoIndex(6);
				shaderEffect->DisableAttributeArray(vertex->m_location);
				shaderEffect->ReleaseShaderProgram();

				enableClips[i] = enableClip[i];
			}
		}

	}

	/**
	* 绘制指定盖面多边形
	*/
	void GLShapeDrawer20::DrawCapPolygon(RenderAction* action, int index)
	{
		vector<Vector4> & clipPlane = action->m_clipPlane;
		vector<int> & enableClip = action->m_enableClip;

		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}

		Section* section = (Section*)action->GetSection();
		//若果section没有数据则表示不需要进行剖切操作，可以返回
		if (!section)
		{
			return;
		}

		list<SectionPlane*>* planeList = section->GetPlaneList();
		if (index >= planeList->size() || index < 0)
		{
			return;
		}
		if (planeList->size() > 0)
		{
			int i = 0;
			list<SectionPlane*>::iterator iter;
			for (i = 0, iter = planeList->begin(); iter != planeList->end(); ++iter, i++)
			{
				if (i == index)
				{
					SectionPlane* sectionPlane = *iter;
					if (sectionPlane)
					{
						glDisable(GL_BLEND);
						glEnable(GL_DEPTH_TEST);
						glDepthMask(GL_TRUE);
						glLineWidth(1);
						//					glEnable(GL_POLYGON_OFFSET_FILL);
						//					glPolygonOffset(2.0f, 1.0f);
						Matrix4 V, P, M, MVP;
						P = camera->GetProjection().Transpose();
						V = camera->GetView().ToMatrix4().Transpose();
						ShaderManager* shaderMananger = action->GetShaderMananger();
						ShaderProgram* shaderEffect = shaderMananger->GetEffect(ShaderManager::Edge);
						//ShaderProgram* shaderEffect = shaderMananger->GetEffect(ShaderManager::CapPlane);
						shaderEffect->UseProgram();
						shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
						shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
						shaderEffect->SetUniformValue(VSP_MODELMAT, M);
						//					Matrix4 normalMat = (M * camera->GetView().ToMatrix4()).Inverse().Transpose();
						//					shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
						//					Vector3 cameraPos = camera->GetWorldPosition();
						//					shaderEffect->SetUniformValue(VSP_EYEPOSITION, cameraPos);
											// multi-clip plane
						Vector4 tc[3];
						int enableClips[3];
						Matrix4 tempMat = camera->GetView().ToMatrix4();
						for (int j = 0; j < 3; j++)
						{
							tc[j] = clipPlane[j];
							tc[j] = tempMat.Inverse().Transpose() * tc[j];
							enableClips[j] = enableClip[j];
						}
						shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
						enableClips[i] = 0;
						shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
						ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
						shaderEffect->SetUniformValue(reverseClipPara->m_location, 0);

						ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
						//					ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
						shaderEffect->EnableAttributeArray(vertex->m_location);
						//					shaderEffect->EnableAttributeArray(normal->m_location);
						Color faceColor(0.6, 0.3, 0.0);
						if (i == 0)
							faceColor.SetColor(0.65, 0.3, 0.0);
						else if (i == 1)
							faceColor.SetColor(0.5, 0.3, 0.0);
						else
							faceColor.SetColor(0.6, 0.3, 0.0);
						shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetPointArray());
						//					shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, sectionPlane->GetNormalArray());
						shaderEffect->SetUniformValue(FSP_DIFFUSE, faceColor);
						DrawTriNoIndex(6);
						shaderEffect->DisableAttributeArray(vertex->m_location);
						//					shaderEffect->DisableAttributeArray(normal->m_location);
						shaderEffect->ReleaseShaderProgram();
					}
					break;
				}
			}
		}

	}

	/**
	 * 只绘制简单的模型主要模型
	 */
	void GLShapeDrawer20::DrawSampleModelPassGroug(RenderAction* action)
	{
		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();

		vector<RenderQueue *> tempRenderQueues;
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
					tempRenderQueues.push_back(&it->second);
				}
			}
		}


		CameraNode * camera = action->GetCamera();
		RenderContext* gl = action->GetGLContext();
		glEnable(GL_DEPTH_TEST);
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_Edge);
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		SceneGroundNode * sceneGroundNode = action->GetSceneGroundNode();

		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		for (int i = 0; i < tempRenderQueues.size(); i++)
		{
			if (tempRenderQueues[i]->GetRenderableArray().size() == 0)
			{
				continue;
			}
			Matrix4 M;
			RenderabelArray::iterator it = tempRenderQueues[i]->GetRenderableArray().begin();
			for (; it != tempRenderQueues[i]->GetRenderableArray().end(); it++)
			{
				Renderable* faceRenderData = *it;
				int dataLength = faceRenderData->GetDataLength();


				if (dataLength > 0)
				{
					bool isUseIndex = faceRenderData->IsUseIndex();
					HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
					HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
					M = *(faceRenderData->GetRenderWorldMatrix());
					vertexBuffer->Bind();
					M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
					shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)veroffset);
					shaderEffect->SetUniformValue(VSP_MODELMAT, M);
					const Color &color = faceRenderData->GetRenderColor();
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
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

			}

		}
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void GLShapeDrawer20::DrawOutlinePassGroup(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化

		EffectManager* effectManager = action->GetRenderManager()->GetEffectManager();
		OutlineEffect* outlineEffect = (OutlineEffect*)effectManager->GetEffect("OUTLINEEFFECT");
		outlineEffect->Render(RenderStateArray);
		GLuint outlineTextureObject = outlineEffect->GetOutlineTextureObject();


		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		//glblendFunc(GL_ONE, GL_ONE);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//glblendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::CombineOutline);
		shaderEffect->UseProgram();

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)debugcoord);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		Matrix4 V, P, M, MVP;

		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
		int sampler0 = 0;
		ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
		shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, outlineTextureObject);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();

		glDisable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

	}

	void GLShapeDrawer20::DrawSSAOPassGroup(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		//初始化

		EffectManager* effectManager = action->GetRenderManager()->GetEffectManager();
		SAOEffect* ssaoEffect = (SAOEffect*)effectManager->GetEffect("SAOEFFECT");
		ssaoEffect->Render(RenderStateArray);
	}


	void GLShapeDrawer20::DrawSectionPlane(RenderAction* action, SectionPlane* sectionPlane)
	{
		if (sectionPlane)
		{
			CameraNode* camera = action->GetCamera();
			if (!camera)
			{
				return;
			}
			//		const IntRect& intRect = camera->GetViewPort().GetRect();
			//		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glLineWidth(1);

			//		gl->PushMatrix();
			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			Matrix4 V, P, M, MVP;
			P = camera->GetProjection().Transpose();
			V = camera->GetView().ToMatrix4().Transpose();
			ShaderManager* shaderMananger = action->GetShaderMananger();
			ShaderProgram* shaderEffect = shaderMananger->GetEffect(ShaderManager::Edge);
			shaderEffect->UseProgram();
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);

			// multi-clip plane
			Vector4 tc[3];
			int enableClips[3] = { 0 };
			Matrix4 tempMat = camera->GetView().ToMatrix4();
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat.Inverse().Transpose() * tc[i];
				//enableClips[i] = action->m_enableClip[i];
			}
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			if (sectionPlane->GetID() == 1001)
				enableClips[0] = 0;
			else if (sectionPlane->GetID() == 1002)
				enableClips[1] = 0;
			else if (sectionPlane->GetID() == 1003)
				enableClips[2] = 0;
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, 0);

			if (sectionPlane->GetShowClipPlane())
			{
				ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
				shaderEffect->EnableAttributeArray(vertex->m_location);
				Color& faceColor = sectionPlane->GetFaceColor();

				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetPointArray());
				shaderEffect->SetUniformValue(FSP_DIFFUSE, faceColor);
				DrawTriNoIndex(6);
				glDisable(GL_BLEND);
				glDepthMask(GL_TRUE);
				Color& edgeColor = sectionPlane->GetEdgeColor();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetLinePointArray());
				shaderEffect->SetUniformValue(FSP_DIFFUSE, edgeColor);
				glDrawArrays(GL_LINES, 0, 8);
				shaderEffect->DisableAttributeArray(vertex->m_location);
			}

			shaderEffect->ReleaseShaderProgram();
			//		gl->PopMatrix();
		}
	}

	void GLShapeDrawer20::ApplyCamera(RenderAction* action)
	{
		RenderContext* gl = action->GetGLContext();
		Matrix4 P, V;
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}

		const IntRect& intRect = camera->GetViewPort().GetRect();
#ifdef WIN32
		if (camera->m_cameraMode == 1)
		{
			glViewport(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else if (camera->m_cameraMode == 2)
		{
			glViewport(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else
		{
			glViewport(intRect.m_left, intRect.m_top, intRect.Width(), intRect.Height());
		}
#else
		if (camera->m_cameraMode == 1)
		{
			camera->SetAspectRatio((intRect.Width() / 2.0f) / intRect.Height());
			glScissor(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
			glViewport(intRect.m_left, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else if (camera->m_cameraMode == 2)
		{
			camera->SetAspectRatio((intRect.Width() / 2.0f) / intRect.Height());
			glScissor(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
			glViewport(intRect.m_left + intRect.Width() / 2, intRect.m_top, intRect.Width() / 2, intRect.Height());
		}
		else
		{
			camera->SetAspectRatio((intRect.Width()*1.0f) / intRect.Height());
			glViewport(intRect.m_left, intRect.m_top, intRect.Width(), intRect.Height());
		}

#endif // WIN32


		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		gl->SetProjectMatrix(P);
		gl->SetViewMatrix(V);
		}

	void GLShapeDrawer20::DrawINTOPRenderPassGroup(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_Edge);

		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);

		shaderEffect->EnableAttributeArray(vertex->m_location);

		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

		Vector3 cameraPos = action->GetCamera()->GetWorldPosition();
		shaderEffect->SetUniformValue(VSP_EYEPOSITION, cameraPos);

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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 M;
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
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

				M = *(faceRenderData->GetRenderWorldMatrix());
				Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();

				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();

				const Color &color = faceRenderData->GetRenderColor();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				{
					Vector4 ambient(0.1, 0.1, 0.1, 1.0);
					Vector4 spec(0.50, 0.50, 0.50, 1.0);
					Vector4 lspec(0.8, 0.8, 0.8, 1.0);
					float shininess = 10.0f;
					Color outlineColor = Parameters::Instance()->m_outlineColor;
					Color selectColor(outlineColor.m_r, outlineColor.m_g, outlineColor.m_b, 0.2f);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, selectColor);


					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}
				vertexBuffer->UnBind();
			}

		}
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

	}

	void GLShapeDrawer20::DrawRenderPassGroup(RenderAction* action)
	{
		ApplyCamera(action);
		///注释的为盖面生成程序
		if (DoSection(action, true))
		{
			//		ApplyCamera(action);
			glClearStencil(0x0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_STENCIL_TEST);

			vector<int> & enableClip = action->m_enableClip;
			vector<int> enableState(enableClip);
			for (int i = 0; i < enableState.size(); i++)
			{
				if (enableState[i] == 0)
					continue;
				enableClip.assign(enableState.size(), 0);
				enableClip[i] = 1; //开启当前剖面
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS, 0, 0x1);
				glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
				glStencilMask(0x1);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);
				DrawSampleModelPassGroug(action);

				enableClip.assign(enableState.begin(), enableState.end()); //开启所有剖面
				enableClip[i] = 0; //关闭当前剖面
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);
				glStencilFunc(GL_EQUAL, 1, 0x1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				DrawCapPolygon(action, i);
			}
			enableClip.assign(enableState.begin(), enableState.end());

			//绘制结束///置
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilFunc(GL_EQUAL, 0, 0xffff);
			glDisable(GL_STENCIL_TEST);

		}

		DoSection(action, true);
		if (Parameters::Instance()->m_LightingMode == 500) //TODO 会和其他光照模式冲突，例如不能使用CUBE环境映射
		{
			DrawSSSPass(action);
			DrawBlur(action);
		}
		else if (Parameters::Instance()->m_LightingMode == 600)
		{
			DrawEdgeDetectionPass(action);

			DrawEdgeDetectionQuad(action);
		}
		else
		{
			//		DrawDepthMapPass(action);//TODO 这里是为实现背面线剔除做的
	//				PrepareDepthMap(action);//这里是在光源位置做的深度图
			if (!(Parameters::Instance()->m_clipPlaneMode == 2))
			{

				string effectName = action->GetRenderManager()->GetGlobalEffect();
				//effectName = "OUTLINEEFFECT";
				Effect* effect = action->GetRenderManager()->GetEffectManager()->GetEffect(effectName);
				if (effect != nullptr)
				{
					effect->Render();
				}
				else
				{
					//普通模型
					DrawPhongPass(action);
				}

			}

			//		DrawSkyShadow(action);//绘制天光效果，只在地面有阴影
		}

		//	DrawFrameBufferDebug(action); //测试FBO挂载的纹理是否生成成功
		DoSection(action, false);
	}

	void GLShapeDrawer20::PrepareDepthMap(RenderAction* action)
	{

		if (action->GetRenderManager()->UseLowQuality())
		{
			return;
		}
		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();

		vector<RenderQueue *> tempRenderQueues;
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
					tempRenderQueues.push_back(&it->second);
				}
			}
		}

		//	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		DrawShadowPass(action, tempRenderQueues);
		//	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void GLShapeDrawer20::DrawPMISRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			LOGI("=No camera=");
			return;
		}

		const IntRect& intRect = camera->GetViewPort().GetRect();
		//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

		ShaderManager* shaderManager = action->GetShaderMananger();

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glLineWidth(2);

		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			PMIData* pmi = (PMIData*)(*it);

			Matrix4 modelMat = *(pmi->GetRenderWorldMatrix());
			DrawPMINew(pmi, camera, shaderManager, modelMat);

		}

	}

	void GLShapeDrawer20::DrawPMINew(PMIData* pmiData, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat)
	{
		//LOGI("begin DrawPMI");
		if (pmiData->IsVisible() == false)
		{
			return;
		}
		ShaderProgram * shaderEffect = shaderManager->GetEffect(ShaderManager::Edge); //绘制线时使用
		shaderEffect->UseProgram();
		Matrix4 V, P, M, MVP;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		M = modelMat;

		Color* tmpColor = pmiData->GetDrawColor();

		//set attribute
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		//set uniform
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);

		// multi-clip planes
		RenderAction * action = shaderManager->GetCurrentAction();
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse().Transpose() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

	//	glLineWidth(2);

		//绘制线
		for (int i = 0; i < pmiData->m_Lines.size(); i++)
		{
			PolyLine* line = pmiData->m_Lines[i];
			int vertexNum, indexNum = 0;
			//float* vertexBuf;
			//M3D_INDEX_TYPE* indexBuf;
			//line->GetBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
			//if (indexNum > 0) //索引数为零则使用顶点绘制
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, line->GetPointList().data());

				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tmpColor);

				glDrawArrays(GL_LINE_STRIP, 0, line->GetPointList().size());
			}
			//else
			{
				//			LOGI("pmi:%d polyline:%d no index!",pmiData->GetID(),i);
				//			glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
				//			glDrawArrays(GL_LINE_STRIP, 0, vertexNum);
			}
		}
		//绘制外框和文字
		if (pmiData->m_IsParallelScreen) //&& pmiData->m_Type == 8000)
		{
			M = (*pmiData->GetOutFrameMatrix()) * M;
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		}

		if (pmiData->m_IsParallelScreen)
		{
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
		}

		vector<ComText*>& comTexts = pmiData->m_ComTexts;
		for (int i = 0; i < comTexts.size(); i++)
		{
			ComText* comText = comTexts[i];
			Mesh* textMesh = comText->GetMesh();

			int dataLength = textMesh->GetDataLength();
			if (dataLength)
			{
				bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
				Vector3* positionArray = textMesh->GetRefMesh()->GetPositionArray()->data() + textMesh->GetDataOffset();
				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tmpColor);
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, positionArray);
				if (isUseIndex)
				{
					M3D_INDEX_TYPE* indexArray = textMesh->GetRefMesh()->GetIndexArray()->data();
					GLShapeDrawer20::DrawTriWithIndex(indexArray, dataLength);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(dataLength);
				}
			}

		}
	}

	void GLShapeDrawer20::DrawSkyShadow(RenderAction* action)
	{
		if (action->GetRenderManager()->UseLowQuality())
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);

		glCullFace(GL_BACK);
		glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);

		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_PlaneShadow);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
		SceneGroundNode * sceneGroundNode = action->GetSceneGroundNode();
		shaderEffect->SetUniformValue(VSP_LIGHTMAT, sceneGroundNode->GetViewMat());
		shaderEffect->SetUniformValue(VSP_LIGHTPROJECT, sceneGroundNode->GetProjectMat());
		shaderEffect->SetUniformValue(FSP_SHADOW_LIGHTPOS, sceneGroundNode->GetSkyLightPos());
		const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		GLfloat xpo = (float)(1.0f / intRect.m_right);
		GLfloat ypo = (float)(1.0f / intRect.m_bottom);
		shaderEffect->SetUniformValue(SHADOW_X_PIXEL_OFFSET, 1, &xpo);
		shaderEffect->SetUniformValue(SHADOW_Y_PIXEL_OFFSET, 1, &ypo);


		int texUnit0 = 0;
		ShaderParameter * texUnitPra0 = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
		shaderEffect->SetUniformValue(texUnitPra0->m_location, texUnit0);
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			GeometryBuffer* tex = (GeometryBuffer*)(action->m_frameBffer.GetDepthAttachment());
			glBindTexture(GL_TEXTURE_2D, tex->GetID());
			//LOGI("action->m_frameBffer.GetDepthAttachment().GetObj() === %d",*action->m_frameBffer.GetDepthAttachment().GetObj());
		}

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)sceneGroundNode->GetVertexs()->Data());
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (void *)sceneGroundNode->GetNormals()->Data());
		shaderEffect->EnableAttributeArray(normal->m_location);

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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 modelMat;
		shaderEffect->SetUniformValue(VSP_MODELMAT, modelMat);
		Color color(1.0f, 1.0f, 1.0f, 1.0f);
		shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

	void GLShapeDrawer20::DrawSolidRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glDepthMask(GL_TRUE);
		glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);
		if (Parameters::Instance()->m_LightingMode == 500)//选用牙齿模式时，采用brdf着色
		{
			DrawBrdfPassGroup(action, RenderStateArray);
		}
		else
		{
#ifdef HUAWEI
			DrawPhongPassSampleGroup(action, RenderStateArray);//没特殊情况就用phong着色
#else
			//DrawPhongPassGroup(action,RenderStateArray);//没特殊情况就用phong着色
			RenderFaces(action, RenderStateArray->GetRenderableArray());
#endif

	}
		glPolygonOffset(0.0, 0.0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);

	}

	void GLShapeDrawer20::DrawDraggerSolidRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		glDisable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glDepthMask(GL_TRUE);
		glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);
		{
			DrawPhongPassGroup(action, RenderStateArray);//没特殊情况就用phong着色
		}
		glPolygonOffset(0.0, 0.0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
	}


	void GLShapeDrawer20::DrawFrameBufferDebug(RenderAction* action)
	{
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
			glBindTexture(GL_TEXTURE_2D, ((GeometryBuffer*)(renderTarget->GetColorTarget(0)))->GetID());
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

	void GLShapeDrawer20::DrawDraggerTranRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		//	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		//	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		//   glClear(GL_DEPTH_BUFFER_BIT);///加上会使边界线消失
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);
		if (Parameters::Instance()->m_LightingMode == 500)
		{
			DrawBrdfPassGroup(action, RenderStateArray);
		}
		else
		{
#ifdef HUAWEI
			DrawPhongPassSampleGroup(action, RenderStateArray);//没特殊情况就用phong着色
#else
			DrawPhongPassGroup(action, RenderStateArray);//没特殊情况就用phong着色
#endif
	}

		glPolygonOffset(0.0, 0.0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

}

	void GLShapeDrawer20::DrawTranRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		//	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		//	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			//   glClear(GL_DEPTH_BUFFER_BIT);///加上会使边界线消失
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);
		if (Parameters::Instance()->m_LightingMode == 500)
		{
			DrawBrdfPassGroup(action, RenderStateArray);
		}
		else
		{
#ifdef HUAWEI
			DrawPhongPassSampleGroup(action, RenderStateArray);//没特殊情况就用phong着色
#else
			//DrawPhongPassGroup(action,RenderStateArray);//没特殊情况就用phong着色

			//TODO sort transparent objects by projective position.
			const Matrix4& projective = camera->GetProjection();
			const Matrix3x4& viewMatrix = camera->GetView();
			vector<TransparentObject> transparentsObjects;
			RenderabelArray::iterator itt = RenderStateArray->GetRenderableArray().begin();
			for (; itt != RenderStateArray->GetRenderableArray().end(); itt++)
			{
				Renderable* faceRenderData = *itt;
				Face* face = static_cast<Face*>(faceRenderData);
				Vector3 center = face->GetBody()->GetModel()->GetWorldBoundingBox().Center();
				Vector4 hCenter(center, 1.0);
				Vector4 result = projective*viewMatrix*hCenter;
				TransparentObject obj;
				obj.z = result.m_z;
				obj.renderable = faceRenderData;
				transparentsObjects.push_back(obj);
			}
			std::sort(transparentsObjects.begin(), transparentsObjects.end(), GreaterSort);

			RenderFaces(action, transparentsObjects);
#endif

	}
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glPolygonOffset(0.0, 0.0);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

}

	void GLShapeDrawer20::DrawTriLineRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		RenderContext*gl = action->GetGLContext();
		if (!camera)
		{
			return;
		}
		//const IntRect& intRect = camera->GetViewPort().GetRect();
		//glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
		//		intRect.m_bottom);
		Matrix4 V, P, M;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		ShaderManager* shaderMananger = action->GetShaderMananger();
		ShaderProgram* shaderEffect = shaderMananger->GetEffect(
			ShaderManager::Edge);
		shaderEffect->UseProgram();
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		// multi-clip plane
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse().Transpose() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
#ifdef DEPTH_EDGE
		int texUnit0 = 0;
		ShaderParameter * texUnitPra0 = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
		shaderEffect->SetUniformValue(texUnitPra0->m_location, texUnit0);
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			GeometryBuffer* tex = (GeometryBuffer*)(action->m_depthMapFBO.GetDepthAttachment());
			glBindTexture(GL_TEXTURE_2D, tex->GetID());
			LOGI(" tex->GetID() =%d", tex->GetID());
			//LOGI("action->m_frameBffer.GetDepthAttachment().GetObj() === %d",*action->m_frameBffer.GetDepthAttachment().GetObj());
	}
#endif
		glLineWidth(2);
		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Face* faceRenderData = (Face*)((*it));
			int dataLength = faceRenderData->GetDataLength() * 2;
			if (dataLength)
			{
				bool isUseIndex = faceRenderData->IsUseIndex();
				Matrix4 M;
				M = *(faceRenderData->GetRenderWorldMatrix());
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				Vector3* positionArray = faceRenderData->GetLinesModeVertexPos();
				if (positionArray == NULL)
				{
					continue;
				}
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
					(GLvoid *)positionArray);
				Color triColor;
				if (!faceRenderData->IsSelected())
				{
					triColor = Color::YELLOW;
				}
				else
				{
					triColor = faceRenderData->GetRenderColor();
				}
				shaderEffect->SetUniformValue(FSP_DIFFUSE, triColor);
				if (dataLength > 0)
				{
					if (isUseIndex)
					{
						M3D_INDEX_TYPE *indexArray = faceRenderData->GetLineModeIndex();
						GLShapeDrawer20::DrawTriLinesWithIndex(indexArray, dataLength);
					}
					else
					{
						GLShapeDrawer20::DrawTriLinesNoIndex(dataLength);
					}
				}
			}

		}
		shaderEffect->DisableAttributeArray(vertex->m_location);
#ifdef DEPTH_EDGE
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
	}
#endif
		shaderEffect->ReleaseShaderProgram();

}

	void GLShapeDrawer20::DrawTriLinesWithIndex(M3D_INDEX_TYPE * indexBuffer,
		int num)
	{
		glDrawElements(GL_LINES, num, M3D_GL_INDEX_TYPE, indexBuffer);
	}

	void GLShapeDrawer20::DrawTriLinesNoIndex(int num)
	{
		glDrawArrays(GL_LINES, 0, num);
	}

	void GLShapeDrawer20::DrawEdgesRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
#ifdef WIN32
		glLineWidth(1);
#else
		glLineWidth(2);
#endif
		RenderContext* gl = action->GetGLContext();
		ShaderManager* shaderMananger = action->GetShaderMananger();
		ShaderProgram* shaderEffect = shaderMananger->GetEffect(
			ShaderManager::Edge);
		shaderEffect->UseProgram();

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

		// multi-clip planes
		Vector4 tc[3];
		int enableClips[3];
		int disableClips[3] = { 0 };
		Matrix4 tempMat = gl->GetViewMatrix();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		//shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		vector<Edge*>* edges = NULL;
		Matrix4 M;
		//#define DEPTH_EDGE
#ifdef DEPTH_EDGE
		int texUnit0 = 0;
		ShaderParameter * texUnitPra0 = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
		shaderEffect->SetUniformValue(texUnitPra0->m_location, texUnit0);
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			GeometryBuffer* tex = (GeometryBuffer*)(action->m_depthMapFBO.GetDepthAttachment());
			glBindTexture(GL_TEXTURE_2D, tex->GetID());
			//LOGI("action->m_frameBffer.GetDepthAttachment().GetObj() === %d",*action->m_frameBffer.GetDepthAttachment().GetObj());
	}
#endif

		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Renderable* edgeRenderData = *it;
			Edge* edge = dynamic_cast<Edge*>(edgeRenderData);
#ifdef WIN32
			if (edge && edge->GetNeedClip())
			{
				shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			}
			else
			{
				shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, disableClips);
			}
#elif __MOBILE__
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
#endif
			const Color& edgeColor = edgeRenderData->GetRenderColor();
			HardWareVertexBuffer* vertexBuffer =
				edgeRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer * indexBuffer =
				edgeRenderData->GetHardWareIndexBuffer();

			M = *(edgeRenderData->GetRenderWorldMatrix());
			Color triColor;
			triColor = edgeColor;
			int dataLength = edgeRenderData->GetDataLength();
			if (dataLength)
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
					(GLvoid *)edgeRenderData->GetVertexPos());
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, triColor);

				M3D_OFFSET_TYPE indexArray = edgeRenderData->GetIndexOffset();

				glDrawArrays(GL_LINES, 0, dataLength);
			}

		}

#ifdef DEPTH_EDGE
		{

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
	}
#endif
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
}

	void GLShapeDrawer20::DrawEdgesInTopRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glLineWidth(6);
		vector<Edge*>* edges = NULL;
		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
		}
	}

	void GLShapeDrawer20::DrawPoint(Point* point, ShaderManager * shaderManager,
		CameraNode* camera)
	{
		Matrix4 M;
		M = (*(point->GetRenderWorldMatrix()));
		const Color& pntColor = point->GetRenderColor();
		RenderAction * action = shaderManager->GetCurrentAction();

		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse().Transpose() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}

		int drawType = point->GetDrawType();
		if (drawType == 1 || drawType == 2 || drawType == 3)
		{
			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);
			shaderEffect->UseProgram();

			// multi-clip plane
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			// LOGI("end use multi-clip planes");

	//		glEnable(GL_BLEND);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//		LOGI("Draw Point 3");
			DrawImageBoard(point->GetImageboard(), camera, M, shaderEffect);
			shaderEffect->ReleaseShaderProgram();
			//		glDisable(GL_BLEND);
		}
		else if (drawType == 0)
		{
			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Edge);
			shaderEffect->UseProgram();

			// multi-clip planes
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			// LOGI("end use multi-clip planes");

	//		LOGI("Draw Point 4");
			DrawMeshBoard(point->GetMeshBoard(), camera, M, shaderEffect);
			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawPoint(Point* point, ShaderManager * shaderManager,
		CameraNode* camera, Matrix4 &modelMat)
	{
		Matrix4 M;
		M = (*(point->GetRenderWorldMatrix())) * modelMat;
		const Color& pntColor = point->GetRenderColor();
		int drawType = point->GetDrawType();
		RenderAction * action = shaderManager->GetCurrentAction();

		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}

		ShaderProgram * shaderEffect = NULL;
		if (drawType == 1 || drawType == 2 || drawType == 3)
		{
			shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);
			shaderEffect->UseProgram();
			// multi-clip planes
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			// LOGI("end use multi-clip planes");

			DrawImageBoard(point->GetImageboard(), camera, M, shaderEffect);
			shaderEffect->ReleaseShaderProgram();
			//		glDisable(GL_BLEND);
		}
		else if (drawType == 0)
		{
			shaderEffect = shaderManager->GetEffect(
				ShaderManager::Edge);
			shaderEffect->UseProgram();
			// multi-clip planes
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			// LOGI("end use multi-clip planes");

			DrawMeshBoard(point->GetMeshBoard(), camera, M, shaderEffect);
			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawPointPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{

		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
		//			intRect.m_bottom);
		ShaderManager* shaderMananger = action->GetShaderMananger();
		vector<Point*> frontShowPoint;

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);

		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Point* point = (Point*)(*it);

			if (!point->IsFrontShow())
			{
				//			LOGI("Draw Point 1");
				DrawPoint(point, shaderMananger, camera);
			}
			else //找到需要最前端显示的，最后统一绘制
			{
				frontShowPoint.push_back(point);
			}
		}

		//绘制最前端显示的点
		if (frontShowPoint.size() > 0)
		{
			for (int i = 0; i < frontShowPoint.size(); i++)
			{
				Point* point = frontShowPoint.at(i);
				//			LOGI("Draw Point 2");
				DrawPoint(point, shaderMananger, camera);
			}
		}
		//glDisable(GL_BLEND);
	}

	void GLShapeDrawer20::DrawBoxRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		//const IntRect& intRect = camera->GetViewPort().GetRect();
		Matrix4 V, P, M, MVP;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		//glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
		//		intRect.m_bottom);
		ShaderManager* shaderMananger = action->GetShaderMananger();
		ShaderProgram * shaderEffect = shaderMananger->GetEffect(
			ShaderManager::Edge);
		shaderEffect->UseProgram();
		//set attribute
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		//set uniform
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
		// multi-clip planes
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		glEnable(GL_DEPTH_TEST);
		//TODO MergeAndroid
#ifdef _WIN32
		glEnable(GL_LINE_STIPPLE);
#else

#endif


		glLineWidth(2);
		//glLineStipple(1, 0x0101);	//dotted
		glDepthMask(GL_TRUE);
		//TODO MergeAndroid
#ifdef _WIN32
		glLineStipple(1, 0x0F0F);
		glDisable(GL_LINE_STIPPLE);
#else

#endif

		if (action->GetRenderBox().Defined())
		{
			DrawShapeBox(action->GetRenderBox(), false, vertex,
				shaderEffect, M);
		}

		//RenderabelArray::iterator it =
		//		RenderStateArray->GetRenderableArray().begin();
		//for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		//{
		//	Model* shape = (Model*) (*it);
		//	//TODO
		//	//Matrix4 M = ((*(shape->GetRenderWorldMatrix())));
		//	DrawShapeBox(shape->GetBoundingBox(), shape->IsSelected(), vertex,
		//			shaderEffect, M);

		//}

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void GLShapeDrawer20::DrawImageBoardQueue(RenderAction * action)
	{
		vector<ImageBoard*>& imageboardArray = action->GetRenderImageboards();
		if (imageboardArray.size() > 0)
		{
			CameraNode* camera = action->GetCamera();
			Matrix4 tempMat = camera->GetView().ToMatrix4();
			Matrix4 P = camera->GetProjection().Transpose();
			Matrix4 V = camera->GetView().ToMatrix4().Transpose();

			ShaderManager * shaderManager = action->GetShaderMananger();
			if (!shaderManager)
			{
				return;
			}

			Vector4 tc[3];
			int enableClips[3];
			int disableClips[3] = { 0 };
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat.Inverse().Transpose() * tc[i];
				enableClips[i] = action->m_enableClip[i];
			}

			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			shaderEffect->UseProgram();
			// multi-clip plane
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			//shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

			Matrix4 imageTransform = Matrix4::IDENTITY;

			for (int i = 0; i < imageboardArray.size(); i++)
			{
				ImageBoard* imageboard = imageboardArray[i];
				if (imageboard && imageboard->GetAllowClip())
				{
					shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
				}
				else
				{
					shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, disableClips);
				}
				//Matrix4* modelMatrix =  imageboard->GetRenderWorldMatrix();
				GLShapeDrawer20::DrawImageBoard(imageboard, V, P, imageTransform, shaderEffect, !imageboard->IsFrontShow());
			}

			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawUIQueue(RenderAction * action)
	{
		const vector<ImageBoard*>& imageboardArray = action->GetRenderUIs();
		if (imageboardArray.size() > 0)
		{
			CameraNode* camera = action->GetCamera();
			Matrix4 tempMat = camera->GetView().ToMatrix4();
			Matrix4 P = camera->GetProjection().Transpose();
			Matrix4 V = camera->GetView().ToMatrix4().Transpose();

			ShaderManager * shaderManager = action->GetShaderMananger();
			if (!shaderManager)
			{
				return;
			}

			Vector4 tc[3];
			int enableClips[3];
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat.Inverse().Transpose() * tc[i];
				enableClips[i] = false;
			}

			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			shaderEffect->UseProgram();
			// multi-clip plane
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

			Matrix4 imageTransform = Matrix4::IDENTITY;

			for (int i = 0; i < imageboardArray.size(); i++)
			{
				ImageBoard* imageboard = imageboardArray[i];
				GLShapeDrawer20::DrawImageBoard(imageboard, V, P, imageTransform, shaderEffect, true);
			}

			shaderEffect->ReleaseShaderProgram();
		}
	}


	void GLShapeDrawer20::DrawHUBImageQueue(RenderAction * action)
	{
		const vector<ImageBoard*>& imageboardArray = action->GetHudImages();
		if (imageboardArray.size() > 0)
		{
			CameraNode* camera = action->GetScene()->GetHudCamera();
			Matrix4 tempMat = camera->GetView().ToMatrix4();
			Matrix4 P = camera->GetProjection().Transpose();
			Matrix4 V = camera->GetView().ToMatrix4().Transpose();

			ShaderManager * shaderManager = action->GetShaderMananger();
			if (!shaderManager)
			{
				return;
			}

			Vector4 tc[3];
			int enableClips[3];
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat.Inverse().Transpose() * tc[i];
				enableClips[i] = false;
			}

			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			shaderEffect->UseProgram();
			// multi-clip plane
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);

			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

			Matrix4 imageTransform = Matrix4::IDENTITY;

			for (int i = 0; i < imageboardArray.size(); i++)
			{
				ImageBoard* imageboard = imageboardArray[i];
				GLShapeDrawer20::DrawImageBoard(imageboard, V, P, imageTransform, shaderEffect, true);
			}

			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawRenderActionBox(RenderAction* action) {
		if (action->GetRenderBox().Defined()) {
			CameraNode* camera = action->GetCamera();
			if (!camera)
			{
				return;
			}
			//const IntRect& intRect = camera->GetViewPort().GetRect();
			Matrix4 V, P, M, MVP;
			P = camera->GetProjection().Transpose();
			V = camera->GetView().ToMatrix4().Transpose();
			//glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
			//		intRect.m_bottom);
			ShaderManager* shaderMananger = action->GetShaderMananger();
			ShaderProgram * shaderEffect = shaderMananger->GetEffect(
				ShaderManager::Edge);
			shaderEffect->UseProgram();
			//set attribute
			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
				VSP_POSITION);
			shaderEffect->EnableAttributeArray(vertex->m_location);
			//set uniform
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			// multi-clip planes
			Vector4 tc[3];
			int enableClips[3];
			Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
			for (int i = 0; i < 3; i++)
			{
				tc[i] = (action->m_clipPlane[i]);
				tc[i] = tempMat * tc[i];
				enableClips[i] = action->m_enableClip[i];
			}

			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
			// LOGI("end use multi-clip planes");
			glEnable(GL_DEPTH_TEST);
#ifdef _WIN32
			glEnable(GL_LINE_STIPPLE);
#else

#endif
			glLineWidth(2);
			//glLineStipple(1, 0x0101);	//dotted
			glDepthMask(GL_TRUE);
			//TODO MergeAndroid
#ifdef _WIN32
			glLineStipple(1, 0x0F0F);
			glDisable(GL_LINE_STIPPLE);
#else

#endif
			if (action->GetRenderBox().Defined())
			{
				DrawShapeBox(action->GetRenderBox(), false, vertex,
					shaderEffect, M);
			}

			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawShapeBox(BoundingBox& box, bool isSelected,
		ShaderParameter * vertex, ShaderProgram * shaderEffect,
		Matrix4 & modelMat)
	{
		bool drawBoxMinMaxPoint = false;

		Color color;
		if (isSelected)
			color = Parameters::Instance()->m_SelectedColor;
		else
			color = Parameters::Instance()->m_BoxColor;

		Matrix4 M = modelMat;

		if (box.Defined())
		{
			vector<float> boxVertex; box.GetVertexs(boxVertex);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				boxVertex.data());

			shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			glDrawElements(GL_LINES, 24, M3D_GL_INDEX_TYPE, BoundingBox::boxIndexs);

			//绘制max 和min 点
			if (drawBoxMinMaxPoint)
			{

				Vector3& maxPos = box.m_max;
				Vector3& minPos = box.m_min;
				Color colormin(0, 1, 0, 1);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, (void *)minPos.Data());
				shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, colormin);
				glDrawArrays(GL_POINTS, 0, 1);
				shaderEffect->DisableAttributeArray(vertex->m_location);

				Color colormax(1, 0, 1, 1);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, (void *)maxPos.Data());
				shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, colormax);

				glDrawArrays(GL_POINTS, 0, 1);
				shaderEffect->DisableAttributeArray(vertex->m_location);
			}
		}
	}

	void GLShapeDrawer20::DrawAnnotationRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		//LOGI("=111=");
		//const IntRect& intRect = camera->GetViewPort().GetRect();
		//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
		//			intRect.m_bottom);
		//LOGI("=11=");
		ShaderManager* shaderManager = action->GetShaderMananger();
		//LOGI("=1=");
		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Note* note = (Note*)(*it);

			Matrix4 modelMat = *(note->GetRenderWorldMatrix());
			DrawAnnotation(note, camera, shaderManager, modelMat);
		}
	}
	void GLShapeDrawer20::DrawAnnotation(Note* pNote, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat)
	{
		Annotation* pAnnotation = dynamic_cast<Annotation*>(pNote);
		if (!pAnnotation)
		{
			return;
		}
		//绘制包围盒
		ShaderProgram * shaderEffect = shaderManager->GetEffect(
			ShaderManager::NoteEdge); //绘制线时使用
		shaderEffect->UseProgram();
		RenderAction * action = shaderManager->GetCurrentAction();
		// multi-clip planes
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 V, P, M, MVP;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		M = modelMat;

		//set attribute
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		//set uniform
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
		if (Parameters::Instance()->m_IsShowBox)
		{
			DrawShapeBox(pNote->GetBoundingBox(), false, vertex, shaderEffect,
				modelMat);
		}
		float useMinDepth = 0;
		ShaderParameter* useMinDepthPra = shaderEffect->GetShaderUniformParameter(
			string("u_useMinDepth"));
		if (pNote->IsFrontShow())
		{
			glEnable(GL_DEPTH_TEST);
			useMinDepth = 1;
			if (useMinDepthPra)
			{
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
		}
		else
		{
			useMinDepth = 0;
			if (useMinDepthPra)
			{
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
			glEnable(GL_DEPTH_TEST);
		}
		//	glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glEnable(GL_DEPTH_TEST);
		//	//根据材质属性，设置显示状态
		Color* tmpColor = pNote->GetDrawColor();

		glLineWidth(4);

		for (int i = 0; i < pNote->m_LineList.size(); i++)
		{
			Line3D* line = pNote->m_LineList[i];
			Color* tempColor = line->GetDrawColor();
			int lineWidth = line->GetLineWidth();
			glLineWidth(lineWidth);
			int vertexNum = 2;
			int indexNum = 2;
			float vertexBuf[6] =
			{ line->m_StartPoint.m_x, line->m_StartPoint.m_y,
				line->m_StartPoint.m_z, line->m_EndPoint.m_x,
				line->m_EndPoint.m_y, line->m_EndPoint.m_z, };
			M3D_INDEX_TYPE indexBuf[2] =
			{ 0, 1 };

			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				vertexBuf);

			shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);

			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE, indexBuf);

			//draw Arrows
			vector<vector<float> > startArrowBufList = line->GetStartArrowBuffer();
			for (int i = 0; i < startArrowBufList.size(); i++)
			{
				vector<float>* arrowBuf = &startArrowBufList.at(i);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
				//			shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
				glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
			}

			vector<vector<float> > endArrowBufList = line->GetEndArrowBuffer();
			for (int i = 0; i < endArrowBufList.size(); i++)
			{
				vector<float>* arrowBuf = &endArrowBufList.at(i);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
				//			shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
				glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
			}

		}

		//PolyLine
		//	LOGI("GLShapeDrawer::DrawNote PolyLine:%d",pNote->m_PolyLineList.size());
		for (int i = 0; i < pNote->m_PolyLineList.size(); i++)
		{
			PolyLine* line = pNote->m_PolyLineList[i];
			int vertexNum = line->GetPointList().size();
			int indexNum = 0;
			float* vertexBuf = (float*)line->GetPointList().data();
			M3D_INDEX_TYPE* indexBuf;
			//line->GetBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
			Color* tempColor = line->GetDrawColor();
			shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
			if (indexNum > 0) //索引数为零则使用顶点绘制
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
				glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE,
					indexBuf);
			}
			else
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
				glDrawArrays(GL_LINE_STRIP, 0, vertexNum);
			}
		}

		int pointSize = pNote->m_PointList.size();
		if (pointSize > 0)
		{
			for (int i = 0; i < pNote->m_PointList.size(); i++)
			{

				Point* point = pNote->m_PointList[i];

				DrawPoint(point, shaderManager, camera);

			}
			shaderEffect->UseProgram();
		}


		//绘制图片
		int imageSize = pNote->m_imageBoardList.size();
		if (imageSize == 0)
		{
			//使用公告板技术，来进行显示效果调整
			Billboard* billboard = pNote->GetBillBoard();

			Matrix4 modelMatBill = modelMat;

			if (billboard)
			{
				modelMatBill = (modelMat * (billboard->GetGLWorldMatrix()));
			}

			//绘制文字
			vector<ComText*>& comTexts = pNote->m_ComTexts;
			for (int i = 0; i < comTexts.size(); i++)
			{
				ComText* comText = comTexts[i];
				Mesh* textMesh = comText->GetMesh();

				if (textMesh)
				{
					int dataLength = textMesh->GetDataLength();
					if (dataLength)
					{
						bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
						Vector3* positionArray =
							textMesh->GetRefMesh()->GetPositionArray()->data()
							+ textMesh->GetDataOffset();
						shaderEffect->SetUniformValue(FSP_DIFFUSE, *tmpColor);
						shaderEffect->SetUniformValue(VSP_MODELMAT, modelMatBill);
						shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
							GL_FLOAT, 0, positionArray);
						//				shaderEffect->EnableAttributeArray(vertex->m_location);
						if (isUseIndex)
						{
							M3D_INDEX_TYPE* indexArray =
								textMesh->GetRefMesh()->GetIndexArray()->data();
							GLShapeDrawer20::DrawTriWithIndex(indexArray, dataLength);
						}
						else
						{
							GLShapeDrawer20::DrawTriNoIndex(dataLength);
						}
					}

				}
			}
		}

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
		if (imageSize > 0)
		{
			//		LOGI("Draw text Note");
			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::NoteImage);
			shaderEffect->UseProgram();
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

			for (int i = 0; i < pNote->m_imageBoardList.size(); i++)
			{
				ImageBoard* imageBoard = pNote->m_imageBoardList[i];
				DrawImageBoard(imageBoard, camera, modelMat, shaderEffect, true);
			}
			shaderEffect->ReleaseShaderProgram();
		}
	}

	void GLShapeDrawer20::DrawNoteRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{

		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		//LOGI("=111=");
		//const IntRect& intRect = camera->GetViewPort().GetRect();
	//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
	//			intRect.m_bottom);
		//LOGI("=11=");
		ShaderManager* shaderManager = action->GetShaderMananger();
		//LOGI("=1=");
		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Note* note = (Note*)(*it);
			int noteType = note->GetType();

			Matrix4 modelMat = *(note->GetRenderWorldMatrix());
			switch (noteType)
			{
			case SHAPE_VOICE_NOTE:
			{
				//			LOGI("=2=");
				ShaderProgram * shaderEffect = shaderManager->GetEffect(
					ShaderManager::Image);
				shaderEffect->UseProgram();
				// multi-clip planes
				Vector4 tc[3];
				int enableClips[3];
				Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
				for (int i = 0; i < 3; i++)
				{
					tc[i] = (action->m_clipPlane[i]);
					tc[i] = tempMat * tc[i];
					enableClips[i] = action->m_enableClip[i];
				}
				shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
				shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
				ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
				shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
				// LOGI("end use multi-clip planes");

				VoiceNote* voiceNote = (VoiceNote*)note;
				DrawImageBoard(voiceNote->GetImageboard(), camera, modelMat,
					shaderEffect);
				shaderEffect->ReleaseShaderProgram();
			}
			break;
			case SHAPE_TEXT_NOTE:
			case SHAPE_THREED_GESTURE_NOTE:
				//			LOGI("=3=");
				DrawNote(note, camera, shaderManager, modelMat);
				break;
			case SHAPE_SEQUENCE_NUMBER_NOTE:
				//			LOGI("=4=");
				DrawNote(note, camera, shaderManager, modelMat);
				break;
			case SHAPE_NOTE:
				//			LOGI("=5=");
				DrawNote(note, camera, shaderManager, modelMat);
				break;
			case SHAPE_MEASURE_BASE:
			case SHAPE_COMMON_NOTE:
			case SHAPE_MEASURE_DISTANCE:
			case SHAPE_MEASURE_ANGLE:
			case SHAPE_MEASURE_PROPERTY:
				//			LOGI("=6=");
				DrawNote(note, camera, shaderManager, modelMat);
				break;
			}
		}
	}
	void GLShapeDrawer20::DrawNote(Note* pNote, CameraNode* camera,
		ShaderManager * shaderManager, Matrix4& modelMat)
	{
		//绘制包围盒
		ShaderProgram * shaderEffect = shaderManager->GetEffect(
			ShaderManager::NoteEdge); //绘制线时使用
		shaderEffect->UseProgram();
		RenderAction * action = shaderManager->GetCurrentAction();
		// multi-clip planes
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4().Inverse().Transpose();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 V, P, M, MVP;
		P = camera->GetProjection().Transpose();
		V = camera->GetView().ToMatrix4().Transpose();
		M = modelMat;

		//set attribute
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		//set uniform
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
		if (Parameters::Instance()->m_IsShowBox)
		{
			DrawShapeBox(pNote->GetBoundingBox(), false, vertex, shaderEffect,
				modelMat);
		}
		float useMinDepth = 0;
		ShaderParameter* useMinDepthPra = shaderEffect->GetShaderUniformParameter(
			string("u_useMinDepth"));
		if (pNote->IsFrontShow())
		{
			glDisable(GL_DEPTH_TEST);
			useMinDepth = 1;
			if (useMinDepthPra)
			{
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
		}
		else
		{
			useMinDepth = 0;
			if (useMinDepthPra)
			{
				shaderEffect->SetUniformValue(useMinDepthPra->m_location, useMinDepth);
			}
			glEnable(GL_DEPTH_TEST);
		}
		//	glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glEnable(GL_DEPTH_TEST);
		//	//根据材质属性，设置显示状态
		Color* tmpColor = pNote->GetDrawColor();

		glLineWidth(4);

		for (int i = 0; i < pNote->m_LineList.size(); i++)
		{
			Line3D* line = pNote->m_LineList[i];
			Color* tempColor = line->GetDrawColor();
			int lineWidth = line->GetLineWidth();
			glLineWidth(lineWidth);
			if (line->GetName() == "exLine")
			{
				glLineWidth(2);
			}
			else if (line->GetName() == "dottedLine")
			{
#ifdef _WIN32
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0x00FF);
#else

#endif		
			}
			int vertexNum = 2;
			int indexNum = 2;
			float vertexBuf[6] =
			{ line->m_StartPoint.m_x, line->m_StartPoint.m_y,
					line->m_StartPoint.m_z, line->m_EndPoint.m_x,
					line->m_EndPoint.m_y, line->m_EndPoint.m_z, };
			M3D_INDEX_TYPE indexBuf[2] =
			{ 0, 1 };

			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				vertexBuf);

			shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);

			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE, indexBuf);

			//draw Arrows
			vector<vector<float> > startArrowBufList = line->GetStartArrowBuffer();
			for (int i = 0; i < startArrowBufList.size(); i++)
			{
				vector<float>* arrowBuf = &startArrowBufList.at(i);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
				//			shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
				glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
			}

			vector<vector<float> > endArrowBufList = line->GetEndArrowBuffer();
			for (int i = 0; i < endArrowBufList.size(); i++)
			{
				vector<float>* arrowBuf = &endArrowBufList.at(i);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
				//			shaderEffect->EnableAttributeArray(vertex->m_location);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
				glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
			}

			if (line->GetName() == "exLine")
			{
				glLineWidth(4);
			}
			else if (line->GetName() == "dottedLine")
			{
#ifdef _WIN32
				glDisable(GL_LINE_STIPPLE);
#else

#endif	
			}
		}
		//PolyLine
		//	LOGI("GLShapeDrawer::DrawNote PolyLine:%d",pNote->m_PolyLineList.size());
		for (int i = 0; i < pNote->m_PolyLineList.size(); i++)
		{
			PolyLine* line = pNote->m_PolyLineList[i];
			int vertexNum = line->GetPointList().size();
			int indexNum = 0;
			float* vertexBuf = (float*)line->GetPointList().data();
			M3D_INDEX_TYPE* indexBuf;
			//line->GetBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
			Color* tempColor = line->GetDrawColor();
			shaderEffect->SetUniformValue(FSP_DIFFUSE, *tempColor);
			if (indexNum > 0) //索引数为零则使用顶点绘制
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
				glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE,
					indexBuf);
			}
			else
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
				glDrawArrays(GL_LINE_STRIP, 0, vertexNum);
			}
		}

		int pointSize = pNote->m_PointList.size();
		if (pointSize > 0)
		{
			for (int i = 0; i < pNote->m_PointList.size(); i++)
			{

				Point* point = pNote->m_PointList[i];

				DrawPoint(point, shaderManager, camera);

			}
			shaderEffect->UseProgram();
		}

		//绘制图片
		int imageSize = pNote->m_imageBoardList.size();
		if (imageSize == 0)
		{
			//使用公告板技术，来进行显示效果调整
			Billboard* billboard = pNote->GetBillBoard();

			Matrix4 modelMatBill = modelMat;

			if (billboard)
			{
				modelMatBill = (modelMat * (billboard->GetGLWorldMatrix()));
			}

			//绘制文字
			vector<ComText*>& comTexts = pNote->m_ComTexts;
			for (int i = 0; i < comTexts.size(); i++)
			{
				ComText* comText = comTexts[i];
				Mesh* textMesh = comText->GetMesh();

				if (textMesh)
				{
					int dataLength = textMesh->GetDataLength();
					if (dataLength)
					{
						bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
						Vector3* positionArray =
							textMesh->GetRefMesh()->GetPositionArray()->data()
							+ textMesh->GetDataOffset();
						shaderEffect->SetUniformValue(FSP_DIFFUSE, *tmpColor);
						shaderEffect->SetUniformValue(VSP_MODELMAT, modelMatBill);
						shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
							GL_FLOAT, 0, positionArray);
						//				shaderEffect->EnableAttributeArray(vertex->m_location);
						if (isUseIndex)
						{
							M3D_INDEX_TYPE* indexArray =
								textMesh->GetRefMesh()->GetIndexArray()->data();
							GLShapeDrawer20::DrawTriWithIndex(indexArray, dataLength);
						}
						else
						{
							GLShapeDrawer20::DrawTriNoIndex(dataLength);
						}
					}

				}
			}
		}

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();

		if (imageSize > 0)
		{
			//		LOGI("Draw text Note");
			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::NoteImage);
			shaderEffect->UseProgram();
			shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
			shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
			ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
			shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);

			for (int i = 0; i < pNote->m_imageBoardList.size(); i++)
			{
				ImageBoard* imageBoard = pNote->m_imageBoardList[i];
				DrawImageBoard(imageBoard, camera, modelMat, shaderEffect, true);
			}
			shaderEffect->ReleaseShaderProgram();
		}

		//LOGI("end Draw Note");
	}

	void GLShapeDrawer20::DrawHandlerRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}

		RenderContext* gl = action->GetGLContext();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
		//			intRect.m_bottom);
		ShaderManager* shaderMananger = action->GetShaderMananger();

		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{

			Handler* handler = (Handler*)(*it);
			int noteType = handler->GetType();

			Matrix4 modelMat = *handler->GetRenderWorldMatrix();

			if (noteType == SHAPE_POINT_HANDLE)
			{
				HandlerPoint* pointHandler = (HandlerPoint*)handler;
				GLShapeDrawer20::DrawPointHandler(pointHandler, shaderMananger,
					camera, modelMat);
			}
			else if (noteType == SHAPE_PLANE_HANDLE || noteType == SHAPE_AXIS_HANDLE
				|| noteType == SHAPE_ROTATE_HANDLE
				|| noteType == SHAPE_ROTATE_HANDLE
				|| noteType == SHAPE_ROTATECENTER)
			{
				GLShapeDrawer20::DrawMeshHandler(handler);
			}
		}
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}


	void GLShapeDrawer20::DrawFPS(SceneNode* node, RenderAction* action)
	{
		if (node)
		{
			if (!action)
			{
				return;
			}

			ShaderManager * shaderManager = action->GetShaderMananger();
			if (!shaderManager)
			{
				return;
			}

			ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Edge);
			if (!shaderEffect)
			{
				return;
			}

			shaderEffect->UseProgram();
			FPSNode* fpsNode = (FPSNode*)node;

			glDisable(GL_DEPTH_TEST);

			glViewport(fpsNode->m_iViewX, fpsNode->m_iViewY, fpsNode->m_iViewWidth,
				fpsNode->m_iViewHeight);

			Matrix4 modelViewMatrix = node->GetGLWorldTransform();
			Matrix4 projectionMatrix, viewMatrix;
			projectionMatrix.Ortho(-fpsNode->m_iViewWidth / 2, fpsNode->m_iViewWidth / 2,
				-fpsNode->m_iViewHeight / 2, fpsNode->m_iViewHeight / 2, -1, 1);
			//glShadeModel(GL_SMOOTH);
			Matrix4 scale, translate;
			Vector3 scalePara(2, 2, 1);
			scale.SetScale(scalePara);
			Vector3 translatePara(-50, 0, 0);
			translate.SetTranslation(translatePara);

			modelViewMatrix = modelViewMatrix *translate.Transpose() *scale.Transpose();

			//set attribute
			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
				VSP_POSITION);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			fpsNode->SetFPS(action->GetRenderManager()->GetFPS());


			ComText* comText = fpsNode->GetFPSMesh();
			Mesh* textMesh = comText->GetMesh();

			int dataLength = textMesh->GetDataLength();
			if (dataLength)
			{
				bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
				Vector3* positionArray =
					textMesh->GetRefMesh()->GetPositionArray()->data()
					+ textMesh->GetDataOffset();
				Color tmpColor(1.0f, 1.0f, 1.0f, 1.0f);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, tmpColor);
				shaderEffect->SetUniformValue(VSP_VIEWMAT, viewMatrix);
				shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, projectionMatrix);
				shaderEffect->SetUniformValue(VSP_MODELMAT, modelViewMatrix);
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, positionArray);
				if (isUseIndex)
				{
					M3D_INDEX_TYPE* indexArray =
						textMesh->GetRefMesh()->GetIndexArray()->data();
					GLShapeDrawer20::DrawTriWithIndex(indexArray,
						dataLength);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(dataLength);
				}


				//		DrawText(fpsNode->m_pTextNode, action);
				glDisable(GL_DEPTH_TEST);
				shaderEffect->DisableAttributeArray(vertex->m_location);
				shaderEffect->ReleaseShaderProgram();
			}
		}


	}

	void GLShapeDrawer20::DrawDepthMapPass(RenderAction* action)
	{
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

		RenderContext* gl = action->GetGLContext();
		CameraNode * camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();

		action->m_depthMapFBO.Create();
		action->m_depthMapFBO.CreateDepthAttachment(false);
		action->m_depthMapFBO.CheckStatus();
		action->m_depthMapFBO.Resize();
		action->m_depthMapFBO.Bind();

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

		glEnable(GL_DEPTH_TEST);
		//	glEnable(GL_POLYGON_OFFSET_FILL);
		//	glPolygonOffset(1.0f, 0.0f);
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_DepthMap);
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		SceneGroundNode * sceneGroundNode = action->GetSceneGroundNode();

		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		for (int i = 0; i < RenderStateArray.size(); i++)
		{
			if (RenderStateArray[i]->GetRenderableArray().size() == 0)
			{
				continue;
			}
			Matrix4 M;
			RenderabelArray::iterator it = RenderStateArray[i]->GetRenderableArray().begin();
			for (; it != RenderStateArray[i]->GetRenderableArray().end(); it++)
			{
				Renderable* faceRenderData = *it;
				int dataLength = faceRenderData->GetDataLength();
				if (dataLength > 0)
				{
					bool isUseIndex = faceRenderData->IsUseIndex();
					HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
					HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
					M = *(faceRenderData->GetRenderWorldMatrix());
					vertexBuffer->Bind();
					M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
					shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)veroffset);
					shaderEffect->SetUniformValue(VSP_MODELMAT, M);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}

				}
			}

		}

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
		//		glPolygonOffset(0.0, 0.0);
		//		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		//glCullFace(GL_BACK);
	//	glDisable(GL_CULL_FACE);
		action->m_depthMapFBO.UnBind();
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

	}

	void GLShapeDrawer20::DrawShadowPass(RenderAction* action, vector<RenderQueue *> &RenderStateArray)
	{
		CameraNode * camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		action->m_frameBffer.Create();
		action->m_frameBffer.CreateDepthAttachment(false);
		action->m_frameBffer.CheckStatus();
		action->m_frameBffer.Resize();
		action->m_frameBffer.Bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		RenderContext* gl = action->GetGLContext();
		glEnable(GL_DEPTH_TEST);
		//	glEnable(GL_POLYGON_OFFSET_FILL);
		//	glPolygonOffset(1.0f, 0.0f);
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_DepthMap);
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		SceneGroundNode * sceneGroundNode = action->GetSceneGroundNode();

		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, sceneGroundNode->GetViewMat());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, sceneGroundNode->GetProjectMat());
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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		for (int i = 0; i < RenderStateArray.size(); i++)
		{
			if (RenderStateArray[i]->GetRenderableArray().size() == 0)
			{
				continue;
			}
			Matrix4 M;
			RenderabelArray::iterator it = RenderStateArray[i]->GetRenderableArray().begin();
			for (; it != RenderStateArray[i]->GetRenderableArray().end(); it++)
			{
				Renderable* faceRenderData = *it;
				int dataLength = faceRenderData->GetDataLength();
				if (dataLength > 0)
				{
					bool isUseIndex = faceRenderData->IsUseIndex();
					HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
					HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
					M = *(faceRenderData->GetRenderWorldMatrix());
					vertexBuffer->Bind();
					M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
					shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)veroffset);
					shaderEffect->SetUniformValue(VSP_MODELMAT, M);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}

				}
			}

		}

#define PLANE
#ifdef PLANE
		{
			ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)sceneGroundNode->GetVertexs()->Data());
			shaderEffect->EnableAttributeArray(vertex->m_location);
			Matrix4 modelMat;
			shaderEffect->SetUniformValue(VSP_MODELMAT, modelMat);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
#endif

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
		//		glPolygonOffset(0.0, 0.0);
		//		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		//glCullFace(GL_BACK);
	//	glDisable(GL_CULL_FACE);
		action->m_frameBffer.UnBind();
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
	}

	void GLShapeDrawer20::PrepareBlurSourceMap(RenderAction* action)
	{
	}

	void GLShapeDrawer20::DrawSSSPass(RenderAction* action)
	{
		CameraNode * camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		action->m_teethFBO.Create();
		//	action->m_teethFBO.SetSize(intRect.m_right, intRect.m_bottom);
		action->m_teethFBO.CreateDepthAttachment(false);
		action->m_teethFBO.LinkTextureColorAttachment(0);//0号挂载点
		action->m_teethFBO.CheckStatus();
		action->m_teethFBO.Resize();
		action->m_teethFBO.Bind();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		DrawBackGround(action->GetBackGroundNode(), action);
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

		glEnable(GL_DEPTH_TEST);

		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();
		for (int i = 0; i < effectsData.size(); i++)
		{
			effect = effectsData[i];
			it = renderPassQueueGroup.find(effect->GetRenderGroupType().GetType());
			if (it != renderPassQueueGroup.end())
			{
				it->second.SetRenderTech(effect);
				int renderType = it->second.GetType().GetType();
				if (renderType == RenderableType::RGT_SOLID)
				{
					//GLShapeDrawer20::DrawSolidRenderPassGroup(action, &(it->second));
					//DrawSkyShadow(action, &(it->second));
				}
				else if (renderType == RenderableType::RGT_POINT)
				{
					GLShapeDrawer20::DrawPointPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_TRANSPARENT)
				{
					GLShapeDrawer20::DrawTranRenderPassGroup(action, &it->second);
					//DrawSkyShadow(action, &(it->second));
				}
				else if (renderType == RenderableType::RGT_INTOP)
				{
					//LOGE("render in top");
					//GLShapeDrawer::DrawINTOPRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_EDGELINE)
				{
					GLShapeDrawer20::DrawEdgesRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_EDGELINEINTOP)
				{
					GLShapeDrawer::DrawEdgesInTopRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_PMI)
				{
					//LOGE("preparPMIS error");
					GLShapeDrawer20::DrawPMISRenderPassGroup(action, &it->second);
				}
				/*else if (renderType == RenderableType::RGT_BOX)
				{
					GLShapeDrawer20::DrawBoxRenderPassGroup(action, &it->second);
				}*/
				else if (renderType == RenderableType::RGT_TRILINE)
				{
					GLShapeDrawer20::DrawTriLineRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_NOTE)
				{
					GLShapeDrawer20::DrawNoteRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_MEASURE)
				{
					//GLShapeDrawer::DrawMeasureRenderPassGroup(action,
					//		&it->second);
				}
				else if (renderType == RenderableType::RGT_HANDLER)
				{
					GLShapeDrawer20::DrawHandlerRenderPassGroup(action, &it->second);
				}
				else
				{
				}
			}
		}
		action->m_teethFBO.UnBind();
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
	}

	void GLShapeDrawer20::DrawBlur(RenderAction* action)
	{
		glEnable(GL_DEPTH_TEST);
		float debugvertices[] =
		{ // Vertices for the square
			-1.0f, 1.0f, -1.0f,// 0. left-bottom
			-1.0f, -1.0f, -1.0f,// 1. right-bottom
			1.0f, 1.0f, -1.0f,// 2. left-top
			1.0f, -1.0f, -1.0f// 3. right-top
		};

		float debugcoord[] = {  // Vertices for the square
					 0, 1,   // 0. left-bottom
					 0, 0,  // 1. right-bottom
				   1,  1,   // 2. left-top
					 1,  0  // 3. right-top
		};

		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::Blur);
		shaderEffect->UseProgram();

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *)debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *)(debugcoord));
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		{
			int sampler0 = 0;
			ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
			shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);
		}

		{
			int sampler1 = 1;
			ShaderParameter * sample1Pra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER1);
			shaderEffect->SetUniformValue(sample1Pra->m_location, sampler1);
		}

		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ((GeometryBuffer*)(action->m_teethFBO.GetDepthAttachment()))->GetID());

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
		glEnable(GL_DEPTH_TEST);
	}

	void GLShapeDrawer20::DrawPhongPass(RenderAction* action)
	{
		//	LOGI("Begin DrawPhonePass ");
		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();
		for (int i = 0; i < effectsData.size(); i++)
		{
			effect = effectsData[i];
			it = renderPassQueueGroup.find(effect->GetRenderGroupType().GetType());
			if (it != renderPassQueueGroup.end())
			{
				it->second.SetRenderTech(effect);
				int renderType = it->second.GetType().GetType();
				if (renderType == RenderableType::RGT_SHADOW)
				{
					//			GLShapeDrawer20::DrawSSAOPassGroup(action, &(it->second));
				//				if (Parameters::Instance()->m_useSSAO) 
				//				{
				////					GLShapeDrawer20::DrawSSAOPassGroup(action, &(it->second));
				//				}
					if (Parameters::Instance()->m_shadowMapEnabled)
					{
						GLShapeDrawer20::ShadowRender(action, &(it->second));
					}

				}
				else if (renderType == RenderableType::RGT_SOLID)
				{
					GLShapeDrawer20::DrawSolidRenderPassGroup(action, &(it->second));
					//DrawSkyShadow(action, &(it->second));
				}
				else if (renderType == RenderableType::RGT_POINT)
				{
					GLShapeDrawer20::DrawPointPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_TRANSPARENT)
				{
					GLShapeDrawer20::DrawTranRenderPassGroup(action, &it->second);
					//DrawSkyShadow(action, &(it->second));
				}
				else if (renderType == RenderableType::RGT_EDGELINE)
				{
					GLShapeDrawer20::DrawEdgesRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_EDGELINEINTOP)
				{
					GLShapeDrawer::DrawEdgesInTopRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_PMI)
				{
					//LOGE("preparPMIS error");
					GLShapeDrawer20::DrawPMISRenderPassGroup(action, &it->second);
				}
				//else if (renderType == RenderableType::RGT_BOX)
				//{
				//	GLShapeDrawer20::DrawBoxRenderPassGroup(action, &it->second);
				//}
				else if (renderType == RenderableType::RGT_TRILINE)
				{
					GLShapeDrawer20::DrawTriLineRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_NOTE)
				{
					GLShapeDrawer20::DrawNoteRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_ANNOTATION)
				{
					GLShapeDrawer20::DrawAnnotationRenderPassGroup(action, &it->second);
				}

				else if (renderType == RenderableType::RGT_MEASURE)
				{
					//GLShapeDrawer::DrawMeasureRenderPassGroup(action,
					//		&it->second);
				}
				else if (renderType == RenderableType::RGT_HANDLER)
				{
					GLShapeDrawer20::DrawHandlerRenderPassGroup(action, &it->second);
				}
				else if (renderType == RenderableType::RGT_INTOP)
				{
					//LOGE("render in top");
					if (SVIEW::Parameters::Instance()->m_selectedStyle == 1 || SVIEW::Parameters::Instance()->m_selectedStyle == 2)
					{
						GLShapeDrawer20::DrawOutlinePassGroup(action, &it->second);
					}
					else if (SVIEW::Parameters::Instance()->m_selectedStyle == 3)
					{
						GLShapeDrawer20::DrawINTOPRenderPassGroup(action, &it->second);
					}

				}
				else
				{

				}
			}
		}

		GLShapeDrawer20::DrawImageBoardQueue(action);
		GLShapeDrawer20::DrawUIQueue(action);
		GLShapeDrawer20::DrawHUBImageQueue(action);
		GLShapeDrawer20::DrawRenderActionBox(action);
	}

	void GLShapeDrawer20::DrawBasePass(RenderAction* action, RenderQueue* RenderStateArray)
	{
		ShaderManager * shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}

		ShaderProgram * draggerEffect = shaderManager->GetEffect(
			ShaderManager::Dragger);
		if (!draggerEffect)
		{
			return;
		}

		CameraNode* camera = action->GetCamera();
		Matrix4 projectMatrix = camera->GetProjection().Transpose();
		Matrix4 viewMatrix = camera->GetView().ToMatrix4().Transpose();
		draggerEffect->UseProgram();


		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();

		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{

			//		count ++;
			Renderable* faceRenderData = *it;
			int dataLength = faceRenderData->GetDataLength();

			//		LOGI("----------------------count==%d",count);
			if (dataLength > 0)
			{
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();

				ShaderParameter* vertex = draggerEffect->GetShaderAttributeParameter(VSP_POSITION);
				ShaderParameter* normal = draggerEffect->GetShaderAttributeParameter(VSP_NORMAL);

				Matrix4 modelMatrix = *(faceRenderData->GetRenderWorldMatrix());
				//mvp = modelMatrix*vp;

				draggerEffect->SetUniformValue(VSP_MODELMAT, modelMatrix);
				draggerEffect->SetUniformValue(VSP_VIEWMAT, viewMatrix);
				draggerEffect->SetUniformValue(VSP_PROJECTIONMAT, projectMatrix);
				//draggerEffect->SetUniformValue(VSP_NORMALMAT, mvp);
				const Color &color = faceRenderData->GetRenderColor();
				draggerEffect->SetUniformValue(VSP_UCOLOR, color);

				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();

				draggerEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				draggerEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
				draggerEffect->EnableAttributeArray(vertex->m_location);
				draggerEffect->EnableAttributeArray(normal->m_location);

				//int useTex = 0;
				//shaderEffect->SetUniformValue(FSP_USETEX, 1, &useTex);
				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
				}
				else
				{
					GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
				}

				draggerEffect->DisableAttributeArray(normal->m_location);
				draggerEffect->DisableAttributeArray(vertex->m_location);

			}
		}
		draggerEffect->ReleaseShaderProgram();
	}


	void GLShapeDrawer20::DrawDraggerLinePass(RenderAction * action, RenderQueue * RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
#ifdef WIN32
		glLineWidth(1);
#else
		glLineWidth(2);
#endif
		RenderContext* gl = action->GetGLContext();
		ShaderManager* shaderMananger = action->GetShaderMananger();
		ShaderProgram* shaderEffect = shaderMananger->GetEffect(
			ShaderManager::DraggerEdge);

		shaderEffect->UseProgram();
		CameraNode* camera = action->GetCamera();
		Matrix4 projectMatrix = camera->GetProjection().Transpose();
		Matrix4 viewMatrix = camera->GetView().ToMatrix4().Transpose();

		shaderEffect->SetUniformValue(VSP_VIEWMAT, viewMatrix);
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, projectMatrix);

		vector<Edge*>* edges = NULL;
		Matrix4 M;
		//#define DEPTH_EDGE

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(
			VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Renderable* edgeRenderData = *it;
			const Color& edgeColor = edgeRenderData->GetRenderColor();
			HardWareVertexBuffer* vertexBuffer =
				edgeRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer * indexBuffer =
				edgeRenderData->GetHardWareIndexBuffer();

			M = *(edgeRenderData->GetRenderWorldMatrix());
			Color triColor;
			triColor = edgeColor;
			int dataLength = edgeRenderData->GetDataLength();
			if (dataLength)
			{
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
					(GLvoid *)edgeRenderData->GetVertexPos());

				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				shaderEffect->SetUniformValue(VSP_UCOLOR, triColor);

				M3D_OFFSET_TYPE indexArray = edgeRenderData->GetIndexOffset();

				glDrawArrays(GL_LINES, 0, dataLength);
			}

		}

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
	}


	void GLShapeDrawer20::DrawDraggerPass(RenderAction * action)
	{
		//关闭剖切操作
		//action->SetSection(NULL);
		//	LOGI("Begin DrawPhonePass ");
		RenderEffect* renderType = action->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();
		RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
		vector<RenderTech*>& effectsData = currentEffect.GetData();
		RenderTech * effect = NULL;
		RenderQueueGroup& renderPassQueueGroup = action->GetDraggerRenderQueueGroup();
		map<int, RenderQueue>::iterator it = renderPassQueueGroup.begin();
		glEnable(GL_BLEND);
		for (int i = 0; i < effectsData.size(); i++)
		{
			effect = effectsData[i];
			it = renderPassQueueGroup.find(effect->GetRenderGroupType().GetType());
			if (it != renderPassQueueGroup.end())
			{
				it->second.SetRenderTech(effect);
				int renderType = it->second.GetType().GetType();
				if (renderType == RenderableType::RGT_SOLID)
				{
					glDisable(GL_CULL_FACE);
					//glEnable(GL_ALPHA_TEST);
					//glDisable(GL_DEPTH_TEST);

					GLShapeDrawer20::DrawBasePass(action, &(it->second));

					//DrawSkyShadow(action, &(it->second));
					//glEnable(GL_DEPTH_TEST);
				}
				//else if (renderType == RenderableType::RGT_POINT)
				//{
				//	GLShapeDrawer20::DrawPointPassGroup(action, &it->second);
				//}
				else if (renderType == RenderableType::RGT_TRANSPARENT)
				{
					glDisable(GL_CULL_FACE);
					//glEnable(GL_ALPHA_TEST);
					//glDisable(GL_DEPTH_TEST);

					GLShapeDrawer20::DrawBasePass(action, &(it->second));
					//GLShapeDrawer20::DrawTranRenderPassGroup(action, &it->second);
					//DrawSkyShadow(action, &(it->second));
				}
				//else if (renderType == RenderableType::RGT_INTOP)
				//{
				//	//LOGE("render in top");
				//	GLShapeDrawer20::DrawINTOPRenderPassGroup(action, &it->second);
				//}
				else if (renderType == RenderableType::RGT_EDGELINE)
				{
					//GLShapeDrawer20::DrawEdgesRenderPassGroup(action, &it->second);	
					GLShapeDrawer20::DrawDraggerLinePass(action, &(it->second));
				}
				//else if (renderType == RenderableType::RGT_EDGELINEINTOP)
				//{
				//	GLShapeDrawer::DrawEdgesInTopRenderPassGroup(action, &it->second);
				//}
				//else if (renderType == RenderableType::RGT_PMI)
				//{
				//	//LOGE("preparPMIS error");
				//	GLShapeDrawer20::DrawPMISRenderPassGroup(action, &it->second);
				//}
				//else if (renderType == RenderableType::RGT_BOX)
				//{
				//	GLShapeDrawer20::DrawBoxRenderPassGroup(action, &it->second);
				//}
				//else if (renderType == RenderableType::RGT_TRILINE)
				//{
				//	GLShapeDrawer20::DrawTriLineRenderPassGroup(action, &it->second);
				//}
				//else if (renderType == RenderableType::RGT_NOTE)
				//{
				//	GLShapeDrawer20::DrawNoteRenderPassGroup(action, &it->second);
				//}
				//else if (renderType == RenderableType::RGT_MEASURE)
				//{
				//	//GLShapeDrawer::DrawMeasureRenderPassGroup(action,
				//	//		&it->second);
				//}
				else if (renderType == RenderableType::RGT_HANDLER)
				{
					GLShapeDrawer20::DrawHandlerRenderPassGroup(action, &it->second);
				}
				//else
				//{
				//}
			}
		}
		glDisable(GL_BLEND);
	}

	void GLShapeDrawer20::DrawPhongPassGroup(RenderAction* action, RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_MultilightPerFrag);
		//LOGI("Get shader effect");
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
		///多光源着色器适用
	//
	//	//direction light
		Vector3 cameraPos = action->GetCamera()->GetWorldPosition();
		shaderEffect->SetUniformValue(VSP_EYEPOSITION, cameraPos);
		int enbleLight[3] = { 0 };


		if (shaderEffect->GetName() == ShaderManager::MultilightPerFrag || shaderEffect->GetType() == ShaderMaterialType::SMT_MultilightPerFrag)
		{
			if (action->GetModelLights()->size() > 0)
			{
				LightList* lightList = action->GetModelLights();
				for (int i = 0; i < lightList->size(); i++)
				{
					enbleLight[i] = 1;
					GLShapeDrawerHelper::SetLightUniform(shaderEffect, lightList->at(i), StringHelper::IntToString(i));
				}
				shaderEffect->SetUniformValue(FSP_ENABLELIGHTS, 3, enbleLight);
			}
			else
			{
				enbleLight[0] = 1; enbleLight[1] = 0; enbleLight[2] = 0;
				GLShapeDrawerHelper::SetLightUniform(shaderEffect, &(RenderAction::m_light), "0");
				shaderEffect->SetUniformValue(FSP_ENABLELIGHTS, 3, enbleLight);
			}

		}

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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 M;
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();

		bool TU0 = false, TU1 = false, TU2 = false, TU3 = false;
		//	LOGI("Begin Loop drawable ");
		//	int count = -1;
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			//		count ++;
			Renderable* faceRenderData = *it;
			int dataLength = faceRenderData->GetDataLength();

			//		LOGI("----------------------count==%d",count);
			if (dataLength > 0)
			{
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				char* vertexAddress = (char*)vertexBuffer->Bind();

				M = *(faceRenderData->GetRenderWorldMatrix());
				Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();

				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
				const Color &color = faceRenderData->GetRenderColor();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);

				shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
				int useAmbientTex = 0;
				ShaderParameter * useAmbientTexPra = shaderEffect->GetShaderUniformParameter(FSP_USEAMBIENTTEX);
				shaderEffect->SetUniformValue(useAmbientTexPra->m_location, useAmbientTex);
				int texUnit1 = 1;
				ShaderParameter * texUnitPra1 = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER1);
				shaderEffect->SetUniformValue(texUnitPra1->m_location, texUnit1);

				int useCubeMap = 0;
				ShaderParameter * useCubeMapTexPra = shaderEffect->GetShaderUniformParameter(FSP_USECUBEMAPTEX);
				shaderEffect->SetUniformValue(useCubeMapTexPra->m_location, useCubeMap);
				int texUnit2 = 2;
				ShaderParameter * texUnitPra2 = shaderEffect->GetShaderUniformParameter(FSP_SAMPLERCUBE0);
				shaderEffect->SetUniformValue(texUnitPra2->m_location, texUnit2);
				Material* material = dynamic_cast<Material*>(faceRenderData->GetRenderMaterial()); //获取材质
				//如果材质存在，则使用材质绘制
				if (material)
				{				//for multilight
					GLShapeDrawerHelper::SetMaterialUniform(shaderEffect, material);
					const Color& diffuse = material->GetDiffuse();
					Color diff(diffuse.m_r, diffuse.m_g, diffuse.m_b, color.m_a);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, diff);
					shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, color);
					Color tempSpCol(0.0, 0.0, 0.0);
					shaderEffect->SetUniformValue(FSP_MATERIAL_SPECULAR, tempSpCol);
					if (Parameters::Instance()->m_LightingMode == 600) //当插画模式时，颜色设为红色，生成的亮度图检测出的边缘比较好
					{
						Color redColor(1.0, 0.0, 0.0, color.m_a);
						shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, redColor);
					}

					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					if (&color == Color::SelectColor)
					{
						selectColor = color;
					}
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					if (material->GetAmbientMap())
					{
						unsigned int sphereMapObj = material->GetAmbientMap()->GetOGLObj();
						//LOGI("sphereMapObj == %d",sphereMapObj);
						if (sphereMapObj)
						{
							TU1 = true;
							glActiveTexture(GL_TEXTURE1);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, sphereMapObj);
							useAmbientTex = 1;
							shaderEffect->SetUniformValue(useAmbientTexPra->m_location, useAmbientTex);
							//			LOGI("sphereMapObj end");
						}
					}
					//cube Map
					if (Parameters::Instance()->m_LightingMode == 400 || material->GetReflectiveTextureTexture())
					{
						unsigned int cubeMapObj = 0;//action->GetScene()->GetResourceManager()->GetDefaultCubeMap()->GetOGLObj();
						if (material->GetReflectiveTextureTexture())
						{
							cubeMapObj = material->GetReflectiveTextureTexture()->GetOGLObj();
						}
						else
						{
							cubeMapObj = action->GetScene()->GetResourceManager()->GetDefaultCubeMap()->GetOGLObj();
						}

						//					LOGI("cubeMapObj === %d",cubeMapObj);
						if (cubeMapObj)
						{
							TU2 = true;
							glActiveTexture(GL_TEXTURE2);
							glEnable(GL_TEXTURE_CUBE_MAP);
							glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapObj);
							Vector4 zero(0.0f, 0.0f, 0.0f, 1.0f);
							shaderEffect->SetUniformValue(FSP_SPECULAR, zero);
							useCubeMap = 1;
							shaderEffect->SetUniformValue(useCubeMapTexPra->m_location, useCubeMap);
						}
					}
					//根据材质属性，设置显示状态
					if (material->GetDiffuseMap())
					{
						unsigned int OGLObj = material->GetDiffuseMap()->GetOGLObj();
						if (OGLObj) //纹理存在
						{
							LOGE("纹理存在");
							shaderEffect->EnableAttributeArray(texCoords->m_location);
							shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
								(GLvoid *)texoffset);

							Color temAmb(1.0, 1.0, 1.0);
							shaderEffect->SetUniformValue(FSP_MATERIAL_AMBIENT, temAmb);
							//LOGI("OGLObj == %d",OGLObj);
							Matrix4* texture2DTransform = material->GetTexture2DTransform();

							if (texture2DTransform)
							{
								Matrix4 mat4 = texture2DTransform->Transpose();
								shaderEffect->SetUniformValue(VSP_TEXTUREMAT, mat4);
							}
							else
							{
								shaderEffect->SetUniformValue(VSP_TEXTUREMAT, Matrix4::IDENTITY);
							}
							int useTex = 1;
							ShaderParameter * useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
							shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
							int texUnit = 0;
							ShaderParameter * texUnitPra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
							shaderEffect->SetUniformValue(texUnitPra->m_location, texUnit);
							if (isUseIndex)
							{
								M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
								DrawTexture2DWithIndex(OGLObj, vertexBuffer, indexBuffer, texture2DTransform, dataLength,
									indexArray);
							}
							else
							{
								DrawTexture2DNoIndex(OGLObj, vertexBuffer, texture2DTransform, dataLength);
							}
						}
						else
						{
							shaderEffect->DisableAttributeArray(texCoords->m_location);
							int useTex = 0;
							shaderEffect->SetUniformValue(FSP_USETEX, 1, &useTex);
							if (isUseIndex)
							{
								M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
								GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
							}
							else
							{
								GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
							}
						}
					}
					else
					{
						//shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
						shaderEffect->DisableAttributeArray(texCoords->m_location);

						int useTex = 0;
						ShaderParameter* useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
						shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
						if (isUseIndex)
						{
							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
						}
						else
						{
							GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
						}
					}
				}
				else
				{
					Vector4 ambient(0.3, 0.3, 0.3, 1.0);
					Vector4 spec(0.50, 0.50, 0.50, 1.0);
					Vector4 lspec(0.8, 0.8, 0.8, 1.0);
					//				shaderEffect->SetUniformValue(FSP_LIGHT_0_SPECULAR, lspec);
					float shininess = 10.0f;
					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					if (&color == Color::SelectColor)
					{
						selectColor = color;
					}
					shaderEffect->DisableAttributeArray(texCoords->m_location);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					//for multilight
					Color emission(0.01, 0.01, 0.01, 1.0);
					//				Color forEdge(1.0,0.0,0.0,color.m_a);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_MATERIAL_EMISSION, emission);
					shaderEffect->SetUniformValue(FSP_MATERIAL_AMBIENT, ambient);
					shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_MATERIAL_SPECULAR, spec);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					shaderEffect->SetUniformValue(FSP_MATERIAL_SHININESS, 1, &shininess);
					if (Parameters::Instance()->m_LightingMode == 600) //当插画模式时，颜色设为红色，生成的亮度图检测出的边缘比较好
					{
						Color redColor(1.0, 0.0, 0.0, color.m_a);
						shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, redColor);
					}

					int useTex = 0;
					ShaderParameter* useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
					shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}

					//				Vector4 zero(0.0, 0.0, 0.0, 1.0);
					//				shaderEffect->SetUniformValue(FSP_LIGHT_0_SPECULAR, zero);
				}
				vertexBuffer->UnBind();
			}

			if (TU1)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
			}
			if (TU2)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				glDisable(GL_TEXTURE_CUBE_MAP);
			}
		}
		//	LOGI("----------------------count==%d",count);
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
		//	LOGI("End phong");
	}
#ifdef HUAWEI
	void GLShapeDrawer20::DrawPhongPassSampleGroup(RenderAction* action, RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_MultiLight);
		//LOGI("Get shader effect");
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);
		shaderEffect->EnableAttributeArray(texCoords->m_location);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
		///多光源着色器适用
	//
	//	//direction light
		Vector3 cameraPos = action->GetCamera()->GetWorldPosition();
		shaderEffect->SetUniformValue(VSP_EYEPOSITION, cameraPos);
		int enbleLight[1];
		enbleLight[0] = 1;
		shaderEffect->SetUniformValue(FSP_ENABLELIGHTS, 1, enbleLight);

		// clip plane
		Vector4 tc(action->m_clipPlane[0]);
		Matrix4 tempMat = gl->GetViewMatrix();
		tc = tempMat.Inverse() * tc;
		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		float enableClip = action->m_enableClip[0];
		ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
		//	LOGI("end use clip plane");

		Matrix4 M;
		RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();

		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			Renderable* faceRenderData = *it;
			int dataLength = faceRenderData->GetDataLength();

			//		LOGI("----------------------count==%d",count);
			if (dataLength > 0)
			{
				bool isUseIndex = faceRenderData->IsUseIndex();
				HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
				HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
				M = *(faceRenderData->GetRenderWorldMatrix());
				Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
				vertexBuffer->Bind();
				unsigned veroffset = faceRenderData->GetVertexOffset();
				unsigned normaloffset = faceRenderData->GetNormalOffset();
				unsigned texoffset = faceRenderData->GetTextureCoordsOffset();
				const Color &color = faceRenderData->GetRenderColor();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)veroffset);
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)normaloffset);
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);

				shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
				Material* material = faceRenderData->GetRenderMaterial(); //获取材质
				//如果材质存在，则使用材质绘制
				if (material)
				{				//for multilight
					GLShapeDrawerHelper::SetMaterialUniform(shaderEffect, material);
					const Color &ambient = material->GetAmbient();
					const Color& diffuse = material->GetDiffuse();
					const Color &spec = material->GetSpecular();
					float shininess = 20.0f;
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_AMBIENT, ambient);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_SPECULAR, spec);
					shaderEffect->SetUniformValue(FSP_SHININESS, 1, &shininess);
					Color diff(diffuse.m_r, diffuse.m_g, diffuse.m_b, color.m_a);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, diff);
					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					if (&color == Color::SelectColor)
					{
						selectColor = color;
					}
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					//根据材质属性，设置显示状态
					if (material->GetTexture())
					{
						//		LOGI("material->GetTexture() ");
						unsigned int OGLObj = material->GetTexture()->GetOGLObj();
						if (OGLObj) //纹理存在
						{
							shaderEffect->EnableAttributeArray(texCoords->m_location);
							shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
								(GLvoid *)texoffset);

							Color temAmb(1.0, 1.0, 1.0);
							shaderEffect->SetUniformValue(FSP_MATERIAL_AMBIENT, temAmb);
							//LOGI("OGLObj == %d",OGLObj);
							Matrix4* texture2DTransform = material->GetTexture2DTransform();

							if (texture2DTransform)
							{
								Matrix4 mat4 = texture2DTransform->Transpose();
								shaderEffect->SetUniformValue(VSP_TEXTUREMAT, mat4);
							}
							else
							{
								shaderEffect->SetUniformValue(VSP_TEXTUREMAT, Matrix4::IDENTITY);
							}
							int useTex = 1;
							ShaderParameter * useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
							shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
							int texUnit = 0;
							ShaderParameter * texUnitPra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
							shaderEffect->SetUniformValue(texUnitPra->m_location, texUnit);
							if (isUseIndex)
							{
								M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
								DrawTexture2DWithIndex(OGLObj, vertexBuffer, indexBuffer, texture2DTransform, dataLength,
									indexArray);
							}
							else
							{
								DrawTexture2DNoIndex(OGLObj, vertexBuffer, texture2DTransform, dataLength);
							}
						}
						else
						{
							shaderEffect->DisableAttributeArray(texCoords->m_location);
							int useTex = 0;
							shaderEffect->SetUniformValue(FSP_USETEX, 1, &useTex);
							if (isUseIndex)
							{
								M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
								GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
							}
							else
							{
								GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
							}
						}
					}
					else
					{
						shaderEffect->DisableAttributeArray(texCoords->m_location);

						int useTex = 0;
						ShaderParameter* useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
						shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
						if (isUseIndex)
						{
							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
						}
						else
						{
							GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
						}
					}
				}
				else
				{
					Vector4 ambient(0.10, 0.10, 0.10, 1.0);
					Vector4 spec(0.0, 0.0, 0.0, 1.0);
					float shininess = 10.0f;
					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					if (&color == Color::SelectColor)
					{
						selectColor = color;
					}
					shaderEffect->DisableAttributeArray(texCoords->m_location);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					//for multilight
					Color emission(0.01, 0.01, 0.01, 1.0);
					//				Color forEdge(1.0,0.0,0.0,color.m_a);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_AMBIENT, ambient);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
					shaderEffect->SetUniformValue(FSP_SPECULAR, spec);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					shaderEffect->SetUniformValue(FSP_SHININESS, 1, &shininess);
					int useTex = 0;
					ShaderParameter* useTexPra = shaderEffect->GetShaderUniformParameter(FSP_USETEX);
					shaderEffect->SetUniformValue(useTexPra->m_location, useTex);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}
			}
		}
		//	LOGI("----------------------count==%d",count);
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		shaderEffect->DisableAttributeArray(texCoords->m_location);
		shaderEffect->ReleaseShaderProgram();
		//	LOGI("End phong");
	}

#endif
	void GLShapeDrawer20::DrawGroundNode(SceneNode * node, RenderAction * action)
	{
		
		if (Parameters::Instance()->m_useGroundGrid)
		{
			if (!action || !node)
			{
				return;
			}

			ShaderManager * shaderManager = action->GetShaderMananger();
			if (!shaderManager)
			{
				return;
			}

			ShaderProgram * sceneGroundEffect = shaderManager->GetEffect(
				ShaderManager::SceneGround);
			
			if (!sceneGroundEffect)
			{
				return;
			}
			glEnable(GL_DEPTH_TEST);
			glLineWidth(1.0);
			//glDisable(GL_DEPTH_TEST);
			GroundNode* ground = (GroundNode*)node;

			//glViewport(axis->m_iViewX, axis->m_iViewY, axis->m_iW, axis->m_iW);
			//设置投影矩阵 模型视图矩阵 一些初始化
			BoundingBox& sceneBox = action->GetScene()->GetSceneBox();
			float length = sceneBox.Length()*1.2;
			int size = floor(length);
			int divisions = 20;
			if (size < 10)
			{
				size = 10;
				divisions = 10;
			}
			//else if (size<divisions)
			//{
			//	divisions = size;
			//}
			ground->SetGroundSize(size, divisions);

			RenderContext* gl = action->GetGLContext();
			sceneGroundEffect->UseProgram();
			sceneGroundEffect->SetUniformValue(VSP_MODELMAT, ground->GetGLWorldTransform());
			sceneGroundEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
			sceneGroundEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
			ShaderParameter * position = sceneGroundEffect->GetShaderAttributeParameter(VSP_POSITION);
			ShaderParameter * colors = sceneGroundEffect->GetShaderAttributeParameter(VSP_COLOR);

			sceneGroundEffect->SetVertexAttribPointer(position->m_location, 3, GL_FLOAT, 0, ground->GetVertexs());
			sceneGroundEffect->EnableAttributeArray(position->m_location);

			sceneGroundEffect->SetVertexAttribPointer(colors->m_location, 4, GL_FLOAT, 0, ground->GetColors());
			sceneGroundEffect->EnableAttributeArray(colors->m_location);


			glDrawArrays(GL_LINES, 0, ground->GetPointsNumber());
			//glEnable(GL_DEPTH_TEST);
		}
	}

    void GLShapeDrawer20::DrawMirrorGroundNode(SceneNode * node, RenderAction * action, const Matrix4 & mirrorMat, GLuint mirrorTexture, GLuint mirrorBackground)
    {
        //if (Parameters::Instance()->m_useGroundGrid)
        {
            if (!action || !node)
            {
                return;
            }

            ShaderManager * shaderManager = action->GetShaderMananger();
            if (!shaderManager)
            {
                return;
            }

            //ShaderProgram * sceneGroundEffect = shaderManager->GetEffect(
            //    ShaderManager::SceneGround);
            ShaderProgram * mirrorProgram = shaderManager->GetEffect(
                ShaderManager::Mirror);
            if (!mirrorProgram)
            {
                return;
            }
            glEnable(GL_DEPTH_TEST);
            glLineWidth(1.0);
            //glDisable(GL_DEPTH_TEST);
            GroundNode* ground = (GroundNode*)node;

            //glViewport(axis->m_iViewX, axis->m_iViewY, axis->m_iW, axis->m_iW);
            //设置投影矩阵 模型视图矩阵 一些初始化
            /*BoundingBox& sceneBox = action->GetScene()->GetSceneBox();
            float length = sceneBox.Length()*1.2;
            int size = floor(length);
            int divisions = 20;
            if (size < 10)
            {
                size = 100;
                divisions = 100;
            }
            //else if (size<divisions)
            //{
            //    divisions = size;
            //}
            ground->SetGroundSize(size, divisions);*/

            RenderContext* gl = action->GetGLContext();
            mirrorProgram->UseProgram();

            mirrorProgram->SetUniformValue("u_mirrorMat", mirrorMat);

            mirrorProgram->SetUniformValue(VSP_MODELMAT, ground->GetGLWorldTransform());
            mirrorProgram->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
            mirrorProgram->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
            string name1 = string("mirrorTexture");
            string name2 = string("mirrorFrontTexture");

            GLuint mirrorTextureLocation = mirrorProgram->GetUniformLocation(name1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mirrorTexture);
            mirrorProgram->SetUniformValue(mirrorTextureLocation, 0);

            GLuint mirrorFrontTextureLocation = mirrorProgram->GetUniformLocation(name2);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mirrorBackground);
            mirrorProgram->SetUniformValue(mirrorFrontTextureLocation, 1);

            ShaderParameter * position = mirrorProgram->GetShaderAttributeParameter(VSP_POSITION);
            ShaderParameter * colors = mirrorProgram->GetShaderAttributeParameter(VSP_COLOR);
            ShaderParameter * coords = mirrorProgram->GetShaderAttributeParameter(VSP_TEXCOORDS);

            mirrorProgram->SetVertexAttribPointer(position->m_location, 3, GL_FLOAT, 0, ground->GetVertexs());
            mirrorProgram->EnableAttributeArray(position->m_location);

            mirrorProgram->SetVertexAttribPointer(colors->m_location, 4, GL_FLOAT, 0, ground->GetColors());
            mirrorProgram->EnableAttributeArray(colors->m_location);


            mirrorProgram->SetVertexAttribPointer(coords->m_location, 2, GL_FLOAT, 0, ground->GetCoords());
            mirrorProgram->EnableAttributeArray(coords->m_location);

            glDrawArrays(GL_LINES, 0, ground->GetPointsNumber());

			//glEnable(GL_DEPTH_TEST);
		}
	}


	/*
		void GLShapeDrawer20::DrawMirrorGroundNode(SceneNode * node, RenderAction * action, const Matrix4 & mirrorMat, GLuint mirrorTexture, GLuint backgroundTexture)
	{

		
	}
	*/

	void GLShapeDrawer20::DrawBrdfPassGroup(RenderAction* action, RenderQueue* RenderStateArray)
	{
		if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_Specular);
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		ShaderParameter* normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);

		shaderEffect->EnableAttributeArray(vertex->m_location);
		shaderEffect->EnableAttributeArray(normal->m_location);

		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());

		Vector3 lightPos(0.0, 0.0, 10.0);
		shaderEffect->SetUniformValue(VSP_LIGHTPOSITION, lightPos);
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
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, (int)action->m_bReverseClip);
		// LOGI("end use multi-clip planes");

		Matrix4 M;
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
				M = *(faceRenderData->GetRenderWorldMatrix());
				Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
				vertexBuffer->Bind();
				M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
				M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
				const Color &color = faceRenderData->GetRenderColor();
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *)veroffset);
				shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *)normaloffset);
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
				shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
				Material* material = dynamic_cast<Material*>(faceRenderData->GetRenderMaterial()); //获取材质
				//如果材质存在，则使用材质绘制
				if (material)
				{
					const Color& diffuse = material->GetDiffuse();
					const Color& ambient = material->GetAmbient();
					Color diff(diffuse.m_r, diffuse.m_g, diffuse.m_b, color.m_a);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, diff);
					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					if (&color == Color::SelectColor)
					{
						selectColor = color;
					}
					shaderEffect->SetUniformValue(FSP_AMBIENT, ambient);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					//cube Map
					if (material->GetDiffuseMap())
					{
						//		LOGI("material->GetTexture() ");
						unsigned int OGLObj = material->GetDiffuseMap()->GetOGLObj();
						if (OGLObj) //纹理存在
						{

						}
						else
						{
							if (isUseIndex)
							{
								M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
								GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
							}
							else
							{
								GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
							}
						}
					}
					else
					{
						if (isUseIndex)
						{
							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
						}
						else
						{
							GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
						}
					}
				}
				else
				{
					Vector4 ambient(0.2, 0.2, 0.2, 1.0);
					Vector4 spec(0.0, 0.0, 0.0, 1.0);
					float shininess = 10.0f;
					Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
					shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
					//for multilight
					Color emission(0.01, 0.01, 0.01, 1.0);
					shaderEffect->SetUniformValue(FSP_DIFFUSE, color);

					if (isUseIndex)
					{
						unsigned indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer20::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}
			}
		}
		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->DisableAttributeArray(normal->m_location);
		shaderEffect->ReleaseShaderProgram();
	}

	void GLShapeDrawer20::DrawEdgeDetectionPass(RenderAction* action)
	{
		CameraNode * camera = action->GetCamera();
		const IntRect& intRect = camera->GetViewPort().GetRect();
		action->m_edgeDetectionFBO.Create();
		//	action->m_teethFBO.SetSize(intRect.m_right, intRect.m_bottom);
		action->m_edgeDetectionFBO.CreateDepthAttachment(false);
		action->m_edgeDetectionFBO.LinkTextureColorAttachment(0);//0号挂载点
		action->m_edgeDetectionFBO.CheckStatus();
		action->m_edgeDetectionFBO.Resize();
		action->m_edgeDetectionFBO.Bind();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		DrawPhongPass(action);
		action->m_edgeDetectionFBO.UnBind();
		//	const IntRect& intRect = camera->GetViewPort().GetRect();
		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
	}

	void GLShapeDrawer20::DrawEdgeDetectionQuad(RenderAction* action)
	{
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
			ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::EdgeDetection);
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
			shaderEffect->SetUniformValue(SHADOW_X_PIXEL_OFFSET, 1, &xpo);
			shaderEffect->SetUniformValue(SHADOW_Y_PIXEL_OFFSET, 1, &ypo);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			//unsigned int cubeMapObj = ResourceManager::GetInstance()->GetDefaultSphereMap()->GetOGLObj();
			//LOGI("cubeMapObj ===%d",cubeMapObj);
	//		LOGI(" ((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID()==%d", ((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID());
			glBindTexture(GL_TEXTURE_2D, ((GeometryBuffer*)(action->m_edgeDetectionFBO.GetColorAttachment(0)))->GetID());
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

	/**
	 * 绘制剖切线
	 */
	void GLShapeDrawer20::DrawSectionLines(RenderAction* action, vector<Vector3>& data, int id)
	{
		int size = data.size();
		if ((size == 0))
		{
			//		LOGI("data is NULL");
			return;
		}
#ifdef WIN32
		glLineWidth(1);
#else
		glLineWidth(2);
#endif // WIN32
		if (Parameters::Instance()->m_clipPlaneMode == 1)
		{
			glLineWidth(5);
		}
		//	LOGI("begin DrawSectionLines");
		RenderContext* gl = action->GetGLContext();
		CameraNode* camera = action->GetCamera();
		//	LOGI("0");
		ShaderProgram* shaderEffect = GLShapeDrawerHelper::GetShaderProgram(action, ShaderMaterialType::SMT_Edge);
		//	LOGI("1");
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);

		shaderEffect->EnableAttributeArray(vertex->m_location);
		Matrix4 V, P, M, MVP;
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
		shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
		shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::YELLOW);

		// multi-clip plane
		Vector4 tc[3];
		int enableClips[3];
		Matrix4 tempMat = camera->GetView().ToMatrix4();
		for (int i = 0; i < 3; i++)
		{
			tc[i] = (action->m_clipPlane[i]);
			tc[i] = tempMat.Inverse().Transpose() * tc[i];
			enableClips[i] = action->m_enableClip[i];
		}
		shaderEffect->SetUniformValue(FSP_CLIPPLANES, 3, tc);
		if (id == 1001)
			enableClips[0] = 0;
		else if (id == 1002)
			enableClips[1] = 0;
		else if (id == 1003)
			enableClips[2] = 0;
		shaderEffect->SetUniformValue(FSP_ENABLECLIPS, 3, enableClips);
		ShaderParameter* reverseClipPara = shaderEffect->GetShaderUniformParameter(FSP_REVERSECLIP);
		shaderEffect->SetUniformValue(reverseClipPara->m_location, 0);

		//	LOGI("00");
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void*)(data.at(0).Data()));
		//	LOGI("11");
		//	LOGI("Draw0");

		glDrawArrays(GL_LINES, 0, data.size());
		//	LOGI("Draw1");

		shaderEffect->DisableAttributeArray(vertex->m_location);
		shaderEffect->ReleaseShaderProgram();
		//	LOGI("end DrawSectionLines");
	}

	void GLShapeDrawer20::DrawCrossSection(RenderAction* action, SectionPlane* sectionPlane)
	{

		if (sectionPlane)
		{
			CameraNode* camera = action->GetCamera();
			if (!camera)
			{
				return;
			}
			//		const IntRect& intRect = camera->GetViewPort().GetRect();
			//		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glLineWidth(3);

			//		gl->PushMatrix();
			Matrix4 V, P, M, MVP;
			P = camera->GetProjection().Transpose();
			V = camera->GetView().ToMatrix4().Transpose();
			ShaderManager* shaderMananger = action->GetShaderMananger();
			ShaderProgram* shaderEffect = shaderMananger->GetEffect(ShaderManager::Edge);
			shaderEffect->UseProgram();
			shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
			shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->EnableAttributeArray(vertex->m_location);

			Color& faceColor = sectionPlane->GetFaceColor();

			vector<CrossSection*> &crossSections = sectionPlane->m_corssSections;
			int vsize = crossSections.size();
			//		LOGI("vsize == %d",vsize);
			for (int i = 0; i < vsize; i++)
			{
				CrossSection* pCS = crossSections[i];
				shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, &(pCS->m_trianglesVertex.at(0).m_x));
				shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::GREEN);
				glDrawArrays(GL_TRIANGLES, 0, pCS->m_trianglesVertex.size());
				//		glDrawElements(GL_TRIANGLES,pCS->m_triangleIndex.size(),GL_UNSIGNED_SHORT,&(pCS->m_triangleIndex[0]));

				if (pCS->m_outer->GetData()->size() > 0)
				{
					shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, &(pCS->m_outer->GetData()->at(0).m_x));
					shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::BLACK);
					glDrawArrays(GL_LINE_LOOP, 0, pCS->m_outer->GetData()->size());
				}
				//			Color& edgeColor = sectionPlane->GetEdgeColor();
					//		shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::BLACK);

					//		glDrawElements(GL_LINES,pCS->m_segmentIndex.size(),GL_UNSIGNED_SHORT,&(pCS->m_segmentIndex[0]));
			}

			glLineWidth(1);
			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->ReleaseShaderProgram();
			//		gl->PopMatrix();
		}

	}

	void GLShapeDrawer20::SetFocalLength(RenderAction * action)
	{

		SceneManager* scene = action->GetScene();
		CameraNode* camera = scene->GetCamera();
		BoundingBox pBoundingBox = scene->GetSceneBox();
		//pBoundingBox.ScaleBox(1.5f);

		///摄像机距离包围盒的距离
		float cameraToCenterDis =
			(camera->GetWorldPosition() - pBoundingBox.Center()).Length();

		float length = pBoundingBox.Length();
		float distance;
		float eyeDistance = 0.0f;

		Matrix3 viewMat = camera->GetView().ToMatrix3();
		//	LOGI("viewMat === %s",viewMat.Tostring().c_str());

		Vector3 directionZ(viewMat.m_m20, 0.0f, viewMat.m_m22);

		Vector3 directionX(viewMat.m_m00, 0.0f, viewMat.m_m02); //获取相机坐标系Z轴坐标

		directionZ = -directionZ; //反方向即为观察方向
		directionZ.Normalize();

		directionX = -directionX; //反方向即为观察方向
		directionX.Normalize();

		Vector3 direction = directionZ + directionX;

		float theta = M3D::Abs(direction.DotProduct(Vector3::RIGHT));
		if (theta > 0.7071)
		{
			distance = pBoundingBox.GetXLength();
		}
		else
		{
			distance = pBoundingBox.GetZLength();
		}
		distance = distance / 5 * 0.032f;
		//摄像机在包围盒外部，设置旋转中心为包围盒中心点，能够提升操作体验
		if (pBoundingBox.IsInside(camera->GetWorldPosition()) == OUTSIDE)
		{
			if (cameraToCenterDis < distance)
			{
				eyeDistance = distance;
				//camera->SetPupilDistance(distance);
			}
			else
			{
				eyeDistance = cameraToCenterDis*0.032f;
				//camera->SetPupilDistance(cameraToCenterDis*0.032f);
			}

		}
		else
		{
			eyeDistance = distance;
			//camera->SetPupilDistance(distance);
		}
		eyeDistance = eyeDistance + Parameters::Instance()->m_pupillaryDistanceFactor*eyeDistance*0.2f;
		camera->SetPupilDistance(eyeDistance);

	}

	GLuint GLShapeDrawer20::AllocTextureUnit()
	{
		GLuint textureUnit = _usedTextureUnits;

		_usedTextureUnits += 1;

		return textureUnit;
	}

	}



