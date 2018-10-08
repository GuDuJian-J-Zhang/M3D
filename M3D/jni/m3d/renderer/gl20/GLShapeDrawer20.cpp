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
#include "m3d/renderer/GL20/GLShapeDrawerHelper.h"
#include "m3d/graphics/Light.h"
#include "m3d/scene/SceneGroundNode.h"
#include "m3d/graphics/GeometryBuffer.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/Body.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/graphics/CrossSection.h"
//#define FBO
//#define framebufferDebug
using namespace SVIEW;

namespace M3D
{
long GLShapeDrawer20::drawBoxTime = 0;
long GLShapeDrawer20::drawMeshTime = 0;
long GLShapeDrawer20::drawPMITime = 0;

int GLShapeDrawer20::m_lastWidth = 0;

/**
 * sh
 */
void GLShapeDrawer20::DrawTriWithIndex(HardWareVertexBuffer* vertexBuffer, HardWareIndexBuffer * indexBuffer,
		int dataLength, M3D_OFFSET_TYPE indexOffset)
{
	char* indexAddress = (char*)indexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *) (indexAddress + indexOffset));
	indexBuffer->UnBind();
}

//draw background color
void GLShapeDrawer20::DrawBackGround(SceneNode* node, RenderAction* action)
{
	ShaderManager * shaderManager = NULL;
	if (!action)
	{
		return;
	}

	BackgroundNode * backgroundNode = (BackgroundNode*) node;
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

	Matrix4 modelViewMatrix, projectionMatrix,modelMat;

	//设置投影矩阵
	projectionMatrix.Ortho(backgroundNode->m_ProArray[0],
			backgroundNode->m_ProArray[1], backgroundNode->m_ProArray[2],
			backgroundNode->m_ProArray[3], backgroundNode->m_ProArray[4],
			backgroundNode->m_ProArray[5]);
	//设置模型视图矩阵
	modelViewMatrix.ToIdentity();
	modelMat.ToIdentity();

	//设置视口
//	glViewport(backgroundNode->m_iViewX, backgroundNode->m_iViewY,
//			backgroundNode->m_iWidth, backgroundNode->m_iHeight);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	if(!backgroundNode->IsUseImage()&& !Parameters::Instance()->m_cubeMode){

    backgroundEffect->UseProgram();
	ShaderParameter * position = backgroundEffect->GetShaderAttributeParameter(VSP_POSITION);
	ShaderParameter * color = backgroundEffect->GetShaderAttributeParameter(VSP_COLOR);

	backgroundEffect->SetVertexAttribPointer(position->m_location,3,GL_FLOAT,0,backgroundNode->m_BackPnt);
	backgroundEffect->EnableAttributeArray(position->m_location);

	backgroundEffect->SetVertexAttribPointer(color->m_location,4,GL_FLOAT,0,backgroundNode->m_BackColor);
	backgroundEffect->EnableAttributeArray(color->m_location);

	Matrix4 mvp = projectionMatrix *modelViewMatrix;

	backgroundEffect->SetUniformValue(VSP_MVPMAT,mvp);

	glDrawArrays(GL_TRIANGLES,0,6);

	backgroundEffect->DisableAttributeArray(position->m_location);
	backgroundEffect->DisableAttributeArray(color->m_location);
	backgroundEffect->ReleaseShaderProgram();
	}
	else if(Parameters::Instance()->m_cubeMode)
	{
		glEnable(GL_DEPTH_TEST);
	   glDepthMask(GL_FALSE);
		backgroundEffect = shaderManager->GetEffect(
				ShaderManager::CubeMap);

			backgroundEffect->UseProgram();

		CameraNode *camera = action->GetCamera();
		RenderContext* gl = action->GetGLContext();
		unsigned int cubeMapObj = action->GetScene()->GetResourceManager()->GetDefaultCubeMap()->GetOGLObj();
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
					(void *) skyboxVertices);
			backgroundEffect->EnableAttributeArray(vertex->m_location);
			int sampler0 = 0;
			Matrix4 mat(camera->GetView().ToMatrix3());
			CameraNode cameraP ;
			cameraP.SetFov(90.0);
			cameraP.SetNearClip(0.1f);
			cameraP.SetFarClip(100.0f);
			cameraP.SetAspectRatio(camera->GetAspectRatio());
			cameraP.SetOrthographic(false);
			Matrix4 per= cameraP.GetProjection();
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
	else
	{
		backgroundEffect = shaderManager->GetEffect(
				ShaderManager::Image);

			backgroundEffect->UseProgram();

		Texture* texture = backgroundNode->GetTexture();
        texture->SetResourceManager(action->GetScene()->GetResourceManager());
        
		if(texture)
		{
			unsigned int OGLObj = texture->GetOGLObj();
			if(OGLObj)
			{
				Color VoiceBackColor(1.0f,1.0f,1.0f,1.0f);
				ShaderParameter * vertex = backgroundEffect->GetShaderAttributeParameter(
						VSP_POSITION);
				backgroundEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
						(void *) backgroundNode->GetVertexs()->Data());
				backgroundEffect->EnableAttributeArray(vertex->m_location);

				ShaderParameter * texCoords = backgroundEffect->GetShaderAttributeParameter(
						VSP_TEXCOORDS);
				backgroundEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT,
						0, (void *) backgroundNode->GetTextureCoords()->Data());
				backgroundEffect->EnableAttributeArray(texCoords->m_location);

				Vector4 tc(action->m_clipPlane[0]);
				backgroundEffect->SetUniformValue(FSP_CLIPPLANE, tc);
				float enableClip = 0;
				ShaderParameter* enableClipPra = backgroundEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
				backgroundEffect->SetUniformValue(enableClipPra->m_location, enableClip);

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
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);


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
	glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *) (indexBuffer));
}

void GLShapeDrawer20::DrawImageBoard(ImageBoard* imageboard, CameraNode * camera, Matrix4 & modelMat,
		ShaderProgram* shaderEffect)
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
	glDisable(GL_DEPTH_TEST);
	Matrix4 V, P, M, MVP;
	P = camera->GetProjection().Transpose();
	V = camera->GetView().ToMatrix4().Transpose();
	int textGLObj = image->GetOGLObj();
	if (textGLObj > 0)
	{
//		LOGI("draw image borad1");
		M = (*(imageboard->GetRenderWorldMatrix())) * modelMat;
		const Color& VoiceBackColor = imageboard->GetRenderColor();

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				(void *) imageboard->GetVertexs()->Data());
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0,
				(void *) imageboard->GetTextureCoords()->Data());
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
		//LOGI("sphereMapObj end DrawTexture2DWithIndex");
		glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *) (indexOffset));
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

	Section* section = (Section*) action->GetSection();
	//若果section没有数据则表示不需要进行剖切操作，可以返回
	if (!section)
	{
		return false;
	}

	list<SectionPlane*>* planeList = section->GetPlaneList();

	if (state)
	{
		list<SectionPlane*>::iterator iter;

		int i = 0;
		for (iter = planeList->begin(); iter != planeList->end(); ++iter, ++i)
		{
			float * tempPointer = (*iter)->GetEquation();
			clipPlane[i].m_x = tempPointer[0];
			clipPlane[i].m_y = tempPointer[1];
			clipPlane[i].m_z = tempPointer[2];
			clipPlane[i].m_w = tempPointer[3];
			//LOGI("---------tempPointer %d = %f", j, clipPlane[4*i+j]);

			enableClip[i] = 1; //表示开启剪裁面
			if(Parameters::Instance()->m_clipPlaneMode == 1)
			{
				enableClip[i] = 0;
			}
			//LOGI("---------tempPointer %d ",enableClip[i]);
		}
	}
	else
	{
		list<SectionPlane*>::iterator iter;

		int i = 0;

		for (iter = planeList->begin(); iter != planeList->end(); ++iter, ++i)
		{
			enableClip[i] = 0; //表示关闭剪裁面
		}
		if (Parameters::Instance()->m_showSection)
		{
			for (iter = planeList->begin(); iter != planeList->end(); ++iter)
			{
				//LOGI("BEGIN section");
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



		for (iter = planeList->begin(); iter != planeList->end(); ++iter)
		{
			//LOGI("BEGIN section");
			DrawSectionLines(action,(*iter)->m_figureSectonLine.linesData); //TODO 剖切线，盖面

		}
	}

	if(section->IsShowCappingPlane())
	{
		return true;
	}
	else
	{
		return false;
	}

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

	Section* section = (Section*) action->GetSection();
	//若果section没有数据则表示不需要进行剖切操作，可以返回
	if (!section)
	{
		return;
	}

	list<SectionPlane*>* planeList = section->GetPlaneList();

	list<SectionPlane*>::iterator iter;

	int i = 0;

	for (iter = planeList->begin(); iter != planeList->end(); ++iter)
	{

		SectionPlane* sectionPlane = *iter;
		if (sectionPlane)
		{
			CameraNode* camera = action->GetCamera();
			if (!camera)
			{
				return;
			}
			const IntRect& intRect = camera->GetViewPort().GetRect();
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
			ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
			shaderEffect->EnableAttributeArray(vertex->m_location);
			Color faceColor(0.6,0.3,0.0);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetPointArray());

			shaderEffect->SetUniformValue(FSP_DIFFUSE, faceColor);
			DrawTriNoIndex(36);
			shaderEffect->DisableAttributeArray(vertex->m_location);
			shaderEffect->ReleaseShaderProgram();
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
// clip plane
	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

	for(int i = 0;i<tempRenderQueues.size();i++)
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
			bool isUseIndex = faceRenderData->IsUseIndex();
			HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			M = *(faceRenderData->GetRenderWorldMatrix());
			vertexBuffer->Bind();
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) veroffset);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);
			const Color &color = faceRenderData->GetRenderColor();
			shaderEffect->SetUniformValue(FSP_DIFFUSE, color);

			if (dataLength > 0)
			{
				if (isUseIndex)
				{
					M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
					GLShapeDrawer20::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
                    indexBuffer->UnBind();
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
		const IntRect& intRect = camera->GetViewPort().GetRect();
//		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glLineWidth(1);

//		gl->PushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		ShaderParameter* vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		Color& faceColor = sectionPlane->GetFaceColor();

		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetPointArray());
		shaderEffect->SetUniformValue(FSP_DIFFUSE, faceColor);

		DrawTriNoIndex(36);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		Color& edgeColor = sectionPlane->GetEdgeColor();
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, sectionPlane->GetLinePointArray());
		shaderEffect->SetUniformValue(FSP_DIFFUSE, edgeColor);
		glDrawArrays(GL_LINES, 0, 24);
		shaderEffect->DisableAttributeArray(vertex->m_location);
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

void GLShapeDrawer20::DrawRenderPassGroup(RenderAction* action)
{
	ApplyCamera(action);
	///注释的为盖面生成程序
	if(DoSection(action, true))
	{
//		ApplyCamera(action);
		glClearStencil(0x0);
		glEnable(GL_STENCIL_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
		glStencilMask(0x1);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		DrawSampleModelPassGroug(action);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		DrawCapPolygon(action);

		//绘制结束///置
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xffff);
		glDisable(GL_STENCIL_TEST);

//		{
//			ApplyCamera(action);
//			glClearStencil(0x0);
//			glEnable(GL_STENCIL_TEST);
//			glClear(GL_STENCIL_BUFFER_BIT);
//			glStencilFunc(GL_ALWAYS, 0, 0);
//			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//			glStencilMask(~0);
//			glDisable(GL_DEPTH_TEST);
//			glDepthMask(GL_FALSE);
//
//			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
//			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_DECR_WRAP); // 改进后
//			DrawSampleModelPassGroug(action);
//
//			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//			glStencilMask(0x00);
//			Parameters::Instance()->m_clipPlaneMode = 1;
//			DoSection(action, true);
//			glEnable(GL_DEPTH_TEST);
//			glDepthMask(GL_TRUE);
//			glStencilOpSeparate(GL_FRONT_AND_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
//			glStencilFunc(GL_NOTEQUAL, 0, ~0);
//			DrawCapPolygon(action);
//			glStencilMask(~0);
//			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//			//glStencilFunc(GL_EQUAL, 0, 0xffff);
//			glDisable(GL_STENCIL_TEST);
//			Parameters::Instance()->m_clipPlaneMode = 0;
//
//		}
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
		if(!(Parameters::Instance()->m_clipPlaneMode == 2))
		{
			//action->BeginDelayDraw();
			DrawPhongPass(action);
			//action->EndDelayDraw();
//			DrawSampleModelPassGroug(action);
		}

//				DrawSkyShadow(action);//绘制天光效果，只在地面有阴影
	}
//	DrawFrameBufferDebug(action); //测试FBO挂载的纹理是否生成成功
	DoSection(action, false);
}

void GLShapeDrawer20::PrepareDepthMap(RenderAction* action)
{

	if(action->GetRenderManager()->UseLowQuality())
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
		PMIData* pmi = (PMIData*) (*it);

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

	Color tmpColor = pmiData->GetDrawColor();

	//set attribute
	ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
	shaderEffect->EnableAttributeArray(vertex->m_location);

	//set uniform
	shaderEffect->SetUniformValue(VSP_MODELMAT, M);
	shaderEffect->SetUniformValue(VSP_VIEWMAT, V);
	shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, P);

	RenderAction * action = shaderManager->GetCurrentAction();
	Vector4 tc(action->m_clipPlane[0]);
	tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	GLfloat enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

//	glLineWidth(2);

	//绘制线
	for (int i = 0; i < pmiData->m_Lines.size(); i++)
	{
		PolyLine* line = pmiData->m_Lines[i];
		int vertexNum, indexNum;
		float* vertexBuf;
		M3D_INDEX_TYPE* indexBuf;
		line->GetBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
		if (indexNum > 0) //索引数为零则使用顶点绘制
		{
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, vertexBuf);

			shaderEffect->SetUniformValue(FSP_DIFFUSE, tmpColor);

			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE, indexBuf);
		}
		else
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
		bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
		Vector3* positionArray = textMesh->GetRefMesh()->GetPositionArray()->data() + textMesh->GetDataOffset();
		shaderEffect->SetUniformValue(FSP_DIFFUSE, tmpColor);
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

void GLShapeDrawer20::DrawSkyShadow(RenderAction* action)
{
	if(action->GetRenderManager()->UseLowQuality())
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
	GLfloat xpo = (float) (1.0f /  intRect.m_right);
	GLfloat ypo = (float) (1.0f / intRect.m_bottom);
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
	shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) sceneGroundNode->GetVertexs()->Data());
	shaderEffect->EnableAttributeArray(vertex->m_location);

	ShaderParameter * normal = shaderEffect->GetShaderAttributeParameter(VSP_NORMAL);
	shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (void *) sceneGroundNode->GetNormals()->Data());
	shaderEffect->EnableAttributeArray(normal->m_location);

	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

	Matrix4 modelMat;
	shaderEffect->SetUniformValue(VSP_MODELMAT, modelMat);
	Color color(1.0f, 1.0f, 1.0f, 1.0f);
	shaderEffect->SetUniformValue(FSP_DIFFUSE, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindTexture(GL_TEXTURE_2D,0);
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
		DrawBrdfPassGroup(action,RenderStateArray);
	}
	else
	{
#ifdef HUAWEI
		DrawPhongPassSampleGroup(action,RenderStateArray);//没特殊情况就用phong着色
#else
		DrawPhongPassGroup(action,RenderStateArray);//没特殊情况就用phong着色
#endif

	}
	glPolygonOffset(0.0, 0.0);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);

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
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *) debugcoord);
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
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(GLShapeDrawer::PolygonOffset, GLShapeDrawer::PolygonOffset);
	if (Parameters::Instance()->m_LightingMode == 500)
	{
		DrawBrdfPassGroup(action,RenderStateArray);
	}
	else
	{
#ifdef HUAWEI
		DrawPhongPassSampleGroup(action,RenderStateArray);//没特殊情况就用phong着色
#else
		DrawPhongPassGroup(action,RenderStateArray);//没特殊情况就用phong着色
#endif

	}

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
	const IntRect& intRect = camera->GetViewPort().GetRect();
//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
//			intRect.m_bottom);
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

	Vector4 tc(action->m_clipPlane[0]);
	tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(
			FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

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
		Face* faceRenderData = (Face*) ((*it));
		int dataLength = faceRenderData->GetDataLength() * 2;
		bool isUseIndex = faceRenderData->IsUseIndex();
		Matrix4 M;
		M = *(faceRenderData->GetRenderWorldMatrix());
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		Vector3* positionArray = faceRenderData->GetLinesModeVertexPos();
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				(GLvoid *) positionArray);
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
	shaderEffect->DisableAttributeArray(vertex->m_location);
#ifdef DEPTH_EDGE
	{
	glBindTexture(GL_TEXTURE_2D,0);
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
	glLineWidth(2);
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

	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse()* tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(
			FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
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
		const Color& edgeColor = edgeRenderData->GetRenderColor();
		HardWareVertexBuffer* vertexBuffer =
				edgeRenderData->GetHardWareVertexBuffer();
		HardWareIndexBuffer * indexBuffer =
				edgeRenderData->GetHardWareIndexBuffer();

		M = *(edgeRenderData->GetRenderWorldMatrix());
		Color triColor;
		triColor = edgeColor;
		int dataLength = edgeRenderData->GetDataLength();
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				(GLvoid *) edgeRenderData->GetVertexPos());
				shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(FSP_DIFFUSE, triColor);

		M3D_OFFSET_TYPE indexArray = edgeRenderData->GetIndexOffset();

		glDrawArrays(GL_LINES, 0, dataLength);


	}

#ifdef DEPTH_EDGE
	{

	glBindTexture(GL_TEXTURE_2D,0);
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
	int drawType = point->GetDrawType();
	if (drawType == 1 || drawType == 2 || drawType == 3)
	{
		ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);
		shaderEffect->UseProgram();
		Vector4 tc(action->m_clipPlane[0]);
		tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		float enableClip = action->m_enableClip[0];
		ShaderParameter* enableClipPra =
				shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
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
		Vector4 tc(action->m_clipPlane[0]);
		tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		float enableClip = action->m_enableClip[0];
		ShaderParameter* enableClipPra =
				shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
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
	Vector4 tc(action->m_clipPlane[0]);
	tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
	float enableClip = action->m_enableClip[0];
	ShaderProgram * shaderEffect = NULL;
	if (drawType == 1 || drawType == 2 || drawType == 3)
	{
		shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);
		shaderEffect->UseProgram();

		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		ShaderParameter* enableClipPra =
				shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
		DrawImageBoard(point->GetImageboard(), camera, M, shaderEffect);
		shaderEffect->ReleaseShaderProgram();
//		glDisable(GL_BLEND);
	}
	else if (drawType == 0)
	{
		shaderEffect = shaderManager->GetEffect(
				ShaderManager::Edge);
		shaderEffect->UseProgram();
		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		float enableClip = action->m_enableClip[0];
		ShaderParameter* enableClipPra =
				shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
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
	const IntRect& intRect = camera->GetViewPort().GetRect();
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
		Point* point = (Point*) (*it);

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
	const IntRect& intRect = camera->GetViewPort().GetRect();
	Matrix4 V, P, M, MVP;
	P = camera->GetProjection().Transpose();
	V = camera->GetView().ToMatrix4().Transpose();
//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
//			intRect.m_bottom);
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
	Vector4 tc(action->m_clipPlane[0]);
	tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(
			FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glLineWidth(3);
	RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Model* shape = (Model*) (*it);

		Matrix4 M = ((*(shape->GetRenderWorldMatrix())));
		DrawShapeBox(shape->GetBoundingBox(), shape->IsSelected(), vertex,
				shaderEffect, M);

	}
	shaderEffect->DisableAttributeArray(vertex->m_location);
	shaderEffect->ReleaseShaderProgram();
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
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,
				box.GetVertexs());
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
					GL_FLOAT, 0, (void *) minPos.Data());
			shaderEffect->EnableAttributeArray(vertex->m_location);
			shaderEffect->SetUniformValue(FSP_DIFFUSE, colormin);
			glDrawArrays(GL_POINTS, 0, 1);
			shaderEffect->DisableAttributeArray(vertex->m_location);

			Color colormax(1, 0, 1, 1);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, (void *) maxPos.Data());
			shaderEffect->EnableAttributeArray(vertex->m_location);
			shaderEffect->SetUniformValue(FSP_DIFFUSE, colormax);

			glDrawArrays(GL_POINTS, 0, 1);
			shaderEffect->DisableAttributeArray(vertex->m_location);
		}
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
	const IntRect& intRect = camera->GetViewPort().GetRect();
//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
//			intRect.m_bottom);
	//LOGI("=11=");
	ShaderManager* shaderManager = action->GetShaderMananger();
	//LOGI("=1=");
	RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Note* note = (Note*) (*it);
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
			Vector4 tc(action->m_clipPlane[0]);
			tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
			shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
			float enableClip = action->m_enableClip[0];
			ShaderParameter* enableClipPra =
					shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
			shaderEffect->SetUniformValue(enableClipPra->m_location,
					enableClip);
			VoiceNote* voiceNote = (VoiceNote*) note;
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
			ShaderManager::Edge); //绘制线时使用
	shaderEffect->UseProgram();
	RenderAction * action = shaderManager->GetCurrentAction();
	Vector4 tc(action->m_clipPlane[0]);
	tc = camera->GetView().ToMatrix4().Inverse().Transpose() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(
			FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
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
	if (pNote->IsFrontShow())
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
	//	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_DEPTH_TEST);
	//	//根据材质属性，设置显示状态
	Color tmpColor = pNote->GetDrawColor();

	glLineWidth(4);

	for (int i = 0; i < pNote->m_LineList.size(); i++)
	{
		Line3D* line = pNote->m_LineList[i];
		Color tempColor = line->GetDrawColor();
		int lineWidth = line->GetLineWidth();
        glLineWidth(lineWidth);
//        m_lastWidth = lineWidth;
		if (line->GetName() == "exLine")
		{
            
			glLineWidth(2);
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

		shaderEffect->SetUniformValue(FSP_DIFFUSE, tempColor);

		glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE, indexBuf);

		//draw Arrows
		vector<vector<float> > startArrowBufList = line->GetStartArrowBuffer();
		for (int i = 0; i < startArrowBufList.size(); i++)
		{
			vector<float>* arrowBuf = &startArrowBufList.at(i);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
//			shaderEffect->EnableAttributeArray(vertex->m_location);
			shaderEffect->SetUniformValue(FSP_DIFFUSE, tempColor);
			glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
		}

		vector<vector<float> > endArrowBufList = line->GetEndArrowBuffer();
		for (int i = 0; i < endArrowBufList.size(); i++)
		{
			vector<float>* arrowBuf = &endArrowBufList.at(i);
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, arrowBuf);
//			shaderEffect->EnableAttributeArray(vertex->m_location);
			shaderEffect->SetUniformValue(FSP_DIFFUSE, tempColor);
			glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
		}

		if (line->GetName() == "exLine")
		{
			glLineWidth(4);
		}
	}

	//PolyLine
	//	LOGI("GLShapeDrawer::DrawNote PolyLine:%d",pNote->m_PolyLineList.size());
	for (int i = 0; i < pNote->m_PolyLineList.size(); i++)
	{
		PolyLine* line = pNote->m_PolyLineList[i];
		int vertexNum, indexNum;
		float* vertexBuf;
		M3D_INDEX_TYPE* indexBuf;
		line->GetBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
		Color tempColor = line->GetDrawColor();
		shaderEffect->SetUniformValue(FSP_DIFFUSE, tempColor);
		if (indexNum > 0) //索引数为零则使用顶点绘制
		{
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
//			shaderEffect->EnableAttributeArray(vertex->m_location);
			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE,
					indexBuf);
		}
		else
		{
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3,
					GL_FLOAT, 0, vertexBuf);
//			shaderEffect->EnableAttributeArray(vertex->m_location);
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
				bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
				Vector3* positionArray =
						textMesh->GetRefMesh()->GetPositionArray()->data()
								+ textMesh->GetDataOffset();
				shaderEffect->SetUniformValue(FSP_DIFFUSE, tmpColor);
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

	shaderEffect->DisableAttributeArray(vertex->m_location);
	shaderEffect->ReleaseShaderProgram();

	if (imageSize > 0)
	{
//		LOGI("Draw text Note");
		ShaderProgram * shaderEffect = shaderManager->GetEffect(
				ShaderManager::Image);
		shaderEffect->UseProgram();
		shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
		ShaderParameter* enableClipPra =
				shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
		shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
		for (int i = 0; i < pNote->m_imageBoardList.size(); i++)
		{
			ImageBoard* imageBoard = pNote->m_imageBoardList[i];
			DrawImageBoard(imageBoard, camera, modelMat, shaderEffect);
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
	const IntRect& intRect = camera->GetViewPort().GetRect();
//	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
//			intRect.m_bottom);
	ShaderManager* shaderMananger = action->GetShaderMananger();

	RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{

		Handler* handler = (Handler*) (*it);
		int noteType = handler->GetType();

		Matrix4 modelMat = *handler->GetRenderWorldMatrix();

		if (noteType == SHAPE_POINT_HANDLE)
		{
			HandlerPoint* pointHandler = (HandlerPoint*) handler;
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
		FPSNode* fpsNode = (FPSNode*) node;

		glDisable(GL_DEPTH_TEST);

		glViewport(fpsNode->m_iViewX, fpsNode->m_iViewY, fpsNode->m_iViewWidth,
				fpsNode->m_iViewHeight);

		Matrix4 modelViewMatrix = node->GetGLWorldTransform();
		Matrix4 projectionMatrix,viewMatrix;
		projectionMatrix.Ortho(-fpsNode->m_iViewWidth / 2, fpsNode->m_iViewWidth / 2,
				-fpsNode->m_iViewHeight / 2, fpsNode->m_iViewHeight / 2, -1, 1);
		//glShadeModel(GL_SMOOTH);
		Matrix4 scale,translate;
		Vector3 scalePara(2,2,1);
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
		bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
		Vector3* positionArray =
				textMesh->GetRefMesh()->GetPositionArray()->data()
						+ textMesh->GetDataOffset();
		Color tmpColor(1.0f,1.0f,1.0f,1.0f);
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
			GLShapeDrawer20::DrawTriWithIndex( indexArray,
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

	action->m_depthMapFBO.GetFBO();
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
	shaderEffect->SetUniformValue(VSP_PROJECTIONMAT,gl->GetProjectMatrix());
// clip plane
	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

	for(int i = 0;i<RenderStateArray.size();i++)
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
			bool isUseIndex = faceRenderData->IsUseIndex();
			HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			M = *(faceRenderData->GetRenderWorldMatrix());
			vertexBuffer->Bind();
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) veroffset);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);

			if (dataLength > 0)
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
	action->m_frameBffer.GetFBO();
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
	shaderEffect->SetUniformValue(VSP_PROJECTIONMAT,sceneGroundNode->GetProjectMat());
// clip plane
	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

	for(int i = 0;i<RenderStateArray.size();i++)
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
			bool isUseIndex = faceRenderData->IsUseIndex();
			HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			M = *(faceRenderData->GetRenderWorldMatrix());
			vertexBuffer->Bind();
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) veroffset);
			shaderEffect->SetUniformValue(VSP_MODELMAT, M);

			if (dataLength > 0)
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

	}

#define PLANE
#ifdef PLANE
	{
		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) sceneGroundNode->GetVertexs()->Data());
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
	action->m_teethFBO.GetFBO();
//	action->m_teethFBO.SetSize(intRect.m_right, intRect.m_bottom);
	action->m_teethFBO.CreateDepthAttachment(false);
	action->m_teethFBO.LinkTextureColorAttachment(0);//0号挂载点
	action->m_teethFBO.CheckStatus();
	action->m_teethFBO.Resize();
	action->m_teethFBO.Bind();
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	DrawBackGround(action->GetBackGroundNode(),action);
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
			else if (renderType == RenderableType::RGT_INTOP)
			{
				//LOGE("render in top");
				//				GLShapeDrawer::DrawINTOPRenderPassGroup(action, &it->second);
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
			else if (renderType == RenderableType::RGT_BOX)
			{
				GLShapeDrawer20::DrawBoxRenderPassGroup(action, &it->second);
			}
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
	shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) debugvertices);
	shaderEffect->EnableAttributeArray(vertex->m_location);

	ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
	shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *) (debugcoord));
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
	glBindTexture(GL_TEXTURE_2D,((GeometryBuffer*)(action->m_teethFBO.GetDepthAttachment()))->GetID());

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,((GeometryBuffer*)(action->m_teethFBO.GetColorAttachment(0)))->GetID());

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
			if (renderType == RenderableType::RGT_SOLID)
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
			else if (renderType == RenderableType::RGT_INTOP)
			{
				//LOGE("render in top");
				//				GLShapeDrawer::DrawINTOPRenderPassGroup(action, &it->second);
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
			else if (renderType == RenderableType::RGT_BOX)
			{
				GLShapeDrawer20::DrawBoxRenderPassGroup(action, &it->second);
			}
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

}

void GLShapeDrawer20::DrawPhongPassGroup(RenderAction* action,	RenderQueue* RenderStateArray)
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
	if (shaderEffect->GetType() == ShaderMaterialType::SMT_MultiLight)
	{
		GLShapeDrawerHelper::SetLightUniform(shaderEffect, &(RenderAction::m_light), "0");
	}

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

	bool TU0 = false, TU1 = false, TU2 = false, TU3 = false;
//	LOGI("Begin Loop drawable ");
//	int count = -1;
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
//		count ++;
		Renderable* faceRenderData = *it;
		int dataLength = faceRenderData->GetDataLength();
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
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) (vertexAddress + veroffset));
		shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *) (vertexAddress + normaloffset));
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
//		LOGI("----------------------count==%d",count);
		if (dataLength > 0)
		{
			Material* material = faceRenderData->GetRenderMaterial(); //获取材质
			//如果材质存在，则使用材质绘制
			if (material)
			{				//for multilight
				GLShapeDrawerHelper::SetMaterialUniform(shaderEffect, material);
				const Color& diffuse = material->GetDiffuse();
				Color diff(diffuse.m_r, diffuse.m_g, diffuse.m_b, color.m_a);
				shaderEffect->SetUniformValue(FSP_DIFFUSE, diff);
				shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE, color);
				Color tempSpCol(0.0,0.0,0.0);
				shaderEffect->SetUniformValue(FSP_MATERIAL_SPECULAR, tempSpCol);
				if(Parameters::Instance()->m_LightingMode == 600) //当插画模式时，颜色设为红色，生成的亮度图检测出的边缘比较好
				{
					Color redColor(1.0,0.0,0.0,color.m_a);
					shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE,redColor );
				}

				Color selectColor(1.0f, 1.0f, 1.0f, 1.0f);
				if (&color == Color::SelectColor)
				{
					selectColor = color;
				}
				shaderEffect->SetUniformValue(FSP_SELECTCOLOR, selectColor);
				if (material->GetAmbientTexture())
				{
					unsigned int sphereMapObj = material->GetAmbientTexture()->GetOGLObj();
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
				if (Parameters::Instance()->m_LightingMode == 400)
				{
					unsigned int cubeMapObj = action->GetScene()->GetResourceManager()->GetDefaultCubeMap()->GetOGLObj();
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
				if (material->GetTexture())
				{
					//		LOGI("material->GetTexture() ");
					unsigned int OGLObj = material->GetTexture()->GetOGLObj();
					if (OGLObj) //纹理存在
					{
						shaderEffect->EnableAttributeArray(texCoords->m_location);
						shaderEffect->SetVertexAttribPointer(texCoords->m_location, 3, GL_FLOAT, 0,
								(GLvoid *) texoffset);

						Color temAmb(1.0,1.0,1.0);
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
				Vector4 ambient(0.1, 0.1, 0.1, 1.0);
				Vector4 spec(0.50, 0.50, 0.50, 1.0);
				Vector4 lspec(0.8,0.8,0.9,1.0);
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
				if(Parameters::Instance()->m_LightingMode == 600) //当插画模式时，颜色设为红色，生成的亮度图检测出的边缘比较好
				{
					Color redColor(1.0,0.0,0.0,color.m_a);
					shaderEffect->SetUniformValue(FSP_MATERIAL_DIFFUSE,redColor );
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
		}
		vertexBuffer->UnBind();
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
void GLShapeDrawer20::DrawPhongPassSampleGroup(RenderAction* action,	RenderQueue* RenderStateArray)
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
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) veroffset);
		shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *) normaloffset);
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);

		shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
//		LOGI("----------------------count==%d",count);
		if (dataLength > 0)
		{
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
								(GLvoid *) texoffset);

						Color temAmb(1.0,1.0,1.0);
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

void GLShapeDrawer20::DrawBrdfPassGroup(RenderAction* action,	RenderQueue* RenderStateArray)
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
// clip plane
	Vector4 tc(action->m_clipPlane[0]);
	Matrix4 tempMat = gl->GetViewMatrix();
	tc = tempMat.Inverse() * tc;
	shaderEffect->SetUniformValue(FSP_CLIPPLANE, tc);
	float enableClip = action->m_enableClip[0];
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);

	Matrix4 M;
	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Renderable* faceRenderData = *it;
		int dataLength = faceRenderData->GetDataLength();
		bool isUseIndex = faceRenderData->IsUseIndex();
		HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
		HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
		M = *(faceRenderData->GetRenderWorldMatrix());
		Matrix4 normalMat = (M * gl->GetViewMatrix()).Inverse().Transpose();
		vertexBuffer->Bind();
		M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
		M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
		const Color &color = faceRenderData->GetRenderColor();
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (GLvoid *) veroffset);
		shaderEffect->SetVertexAttribPointer(normal->m_location, 3, GL_FLOAT, 0, (GLvoid *) normaloffset);
		shaderEffect->SetUniformValue(VSP_MODELMAT, M);
		shaderEffect->SetUniformValue(VSP_NORMALMAT, normalMat);
		if (dataLength > 0)
		{
			Material* material = faceRenderData->GetRenderMaterial(); //获取材质
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
				if (material->GetTexture())
				{
					//		LOGI("material->GetTexture() ");
					unsigned int OGLObj = material->GetTexture()->GetOGLObj();
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
	action->m_edgeDetectionFBO.GetFBO();
//	action->m_teethFBO.SetSize(intRect.m_right, intRect.m_bottom);
	action->m_edgeDetectionFBO.CreateDepthAttachment(false);
	action->m_edgeDetectionFBO.LinkTextureColorAttachment(0);//0号挂载点
	action->m_edgeDetectionFBO.CheckStatus();
	action->m_edgeDetectionFBO.Resize();
	action->m_edgeDetectionFBO.Bind();
	 glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

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
		float xpo = (float) (1.0f /  intRect.m_right);
		float ypo = (float) (1.0f / intRect.m_bottom);
		ShaderProgram *shaderEffect = action->GetShaderMananger()->GetEffect(ShaderManager::EdgeDetection);
		shaderEffect->UseProgram();

		ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
		shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) debugvertices);
		shaderEffect->EnableAttributeArray(vertex->m_location);

		ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
		shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *) debugcoord);
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
void GLShapeDrawer20::DrawSectionLines(RenderAction* action,vector<Vector3>& data)
{
	int size = data.size();
	if((size==0))
	{
//		LOGI("data is NULL");
		return;
	}
	glLineWidth(5);
    if(Parameters::Instance()->m_clipPlaneMode == 1)
    {
       glLineWidth(10);
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
	float enableClip = 0;
	ShaderParameter* enableClipPra = shaderEffect->GetShaderUniformParameter(
			FSP_ENABLECLIP);
	shaderEffect->SetUniformValue(enableClipPra->m_location, enableClip);
	shaderEffect->SetUniformValue(VSP_MODELMAT, M);
	shaderEffect->SetUniformValue(VSP_VIEWMAT, gl->GetViewMatrix());
	shaderEffect->SetUniformValue(VSP_PROJECTIONMAT, gl->GetProjectMatrix());
	shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::YELLOW);
//	LOGI("00");
	shaderEffect->SetVertexAttribPointer(vertex->m_location,3,GL_FLOAT,0,(void*)(data.at(0).Data()));
//	LOGI("11");
//	LOGI("Draw0");

	glDrawArrays(GL_LINES,0,data.size());
//	LOGI("Draw1");

	shaderEffect->DisableAttributeArray(vertex->m_location);
	shaderEffect->ReleaseShaderProgram();
//	LOGI("end DrawSectionLines");
}

void GLShapeDrawer20::DrawCrossSection(RenderAction* action,SectionPlane* sectionPlane)
{

	if (sectionPlane)
	{
		CameraNode* camera = action->GetCamera();
		if (!camera)
		{
			return;
		}
		const IntRect& intRect = camera->GetViewPort().GetRect();
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

		vector<CrossSection*> &crossSections =  sectionPlane->m_corssSections;
		int vsize = crossSections.size();
//		LOGI("vsize == %d",vsize);
		for(int i = 0;i < vsize; i++)
		{
			CrossSection* pCS = crossSections[i];
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,&( pCS->m_trianglesVertex.at(0).m_x));
			shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::GREEN);
			glDrawArrays(GL_TRIANGLES,0,pCS->m_trianglesVertex.size());
	//		glDrawElements(GL_TRIANGLES,pCS->m_triangleIndex.size(),GL_UNSIGNED_SHORT,&(pCS->m_triangleIndex[0]));

			if(pCS->m_outer->GetData()->size()>0)
			{
			shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0,&( pCS->m_outer->GetData()->at(0).m_x));
			shaderEffect->SetUniformValue(FSP_DIFFUSE, Color::BLACK);
			glDrawArrays(GL_LINE_LOOP,0,pCS->m_outer->GetData()->size());
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

void GLShapeDrawer20::SetFocalLength(RenderAction* action)
{
	SceneManager* scene = action->GetScene();
	CameraNode* camera = scene->GetCamera();
	BoundingBox pBoundingBox = scene->GetSceneBox();
	//pBoundingBox.ScaleBox(1.5f);

	///摄像机距离包围盒的距离
	float cameraToCenterDis =
			(camera->GetWorldPosition() - pBoundingBox.Center()).Length();

    float length = pBoundingBox.Length();

	//摄像机在包围盒外部，设置旋转中心为包围盒中心点，能够提升操作体验
	if (pBoundingBox.IsInside(camera->GetWorldPosition()) == OUTSIDE)
	{
	camera->SetPupilDistance(cameraToCenterDis*0.03f);

	}
	else
	{
		float distance;

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
		distance = distance / 4 * 0.03;
		camera->SetPupilDistance(distance);


	}
}

}



