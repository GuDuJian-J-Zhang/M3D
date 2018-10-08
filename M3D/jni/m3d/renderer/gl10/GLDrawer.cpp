#include "m3d/M3D.h"
#include "m3d/renderer/gl10/GLDrawer.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/model/MeshData.h"
#include "m3d/graphics/ImageBoard.h"

#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/Section.h"
#include "m3d/graphics/Texture.h"

#include "m3d/SceneManager.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/TextNode.h"
#include "m3d/graphics/Light.h"
#include "m3d/RenderManager.h"

#include "m3d/scene/ModelNode.h"
#include "Utility.h"

#include "sview/views/Parameters.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/model/CText.h"

#include "m3d/Handler/HandlerGroup.h"
#include "m3d/model/ComText.h"
#include "m3d/renderer/gl10/GLShapeDrawer.h"

#ifdef __MOBILE__
#include <FTGL/ftgles.h>
#endif

/*GLEWContext _glewctx;*/

using namespace SVIEW;

namespace M3D
{
//bool GLDrawer::useVBO = true;
//bool GLDrawer::firstUp = true;
//float GLDrawer::drawLimit = 0.0f;
//bool GLDrawer::clipPlaneDrawing = false;
//int GLDrawer::CurClipPlaneCount = 0;

//draw background color
void GLDrawer::DrawBackGround(SceneNode* node, RenderAction* action)
{
	if (node == NULL)
	{
		return;
	}
	ClearGLState();
	//LOGE("GLDrawer::DrawBackGround  1");
	if (Parameters::Instance()->m_UseBackImage)
	{
		BackgroundNode* backgroundNode = (BackgroundNode*) node;

		glViewport(backgroundNode->m_iViewX, backgroundNode->m_iViewY,
				backgroundNode->m_iWidth, backgroundNode->m_iHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

#ifdef  __MOBILE__
		glOrthof(backgroundNode->m_ProArray[0], backgroundNode->m_ProArray[1],
			backgroundNode->m_ProArray[2], backgroundNode->m_ProArray[3],
			backgroundNode->m_ProArray[4], backgroundNode->m_ProArray[5]);
#elif _WIN32
		glOrtho(backgroundNode->m_ProArray[0], backgroundNode->m_ProArray[1],
			backgroundNode->m_ProArray[2], backgroundNode->m_ProArray[3],
			backgroundNode->m_ProArray[4], backgroundNode->m_ProArray[5]);
#endif

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glShadeModel(GL_SMOOTH);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		if(!backgroundNode->IsUseImage())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, backgroundNode->m_BackPnt);
			glColorPointer(4, GL_FLOAT, 0, backgroundNode->m_BackColor);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
		{
			Texture* texture = backgroundNode->GetTexture();

			if(texture)
			{
                texture->SetResourceManager(action->GetScene()->GetResourceManager());
				unsigned int OGLObj = texture->GetOGLObj();
				if(OGLObj)
				{
					//设置混合颜色
					glColor4f(1.0f,1.0f,1.0f,1.0f);
					glEnable(GL_BLEND);
					// 启用纹理
					glEnable(GL_TEXTURE_2D);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glBindTexture(GL_TEXTURE_2D, backgroundNode->GetTexture()->GetOGLObj());

					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);

					glVertexPointer(3, GL_FLOAT, 0,backgroundNode->GetVertexs()->Data());

					glTexCoordPointer(2, GL_FLOAT, 0, backgroundNode->GetTextureCoords()->Data());
					glDrawArrays(GL_TRIANGLES, 0, 6);

					glDisableClientState(GL_VERTEX_ARRAY);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);

					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
				}

			}
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
}

//draw axis
void GLDrawer::DrawAxis(SceneNode* node, RenderAction* action)
{
	if (node == NULL)
	{
		return;
	}

	if (Parameters::Instance()->m_IsShowAxis)
	{
		RenderContext* gl = action->GetGLContext();

		AxisNode* axis = (AxisNode*) node;

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glViewport(axis->m_iViewX, axis->m_iViewY, axis->m_iW, axis->m_iW);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

#ifdef  __MOBILE__
		glOrthof(axis->m_ProArray[0], axis->m_ProArray[1], axis->m_ProArray[2],
			axis->m_ProArray[3], axis->m_ProArray[4], axis->m_ProArray[5]);
#elif _WIN32
		glOrtho(axis->m_ProArray[0], axis->m_ProArray[1], axis->m_ProArray[2],
			axis->m_ProArray[3], axis->m_ProArray[4], axis->m_ProArray[5]);
#endif
 
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glMultMatrixf((float*)node->GetGLWorldTransform().Data());

		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, axis->axisPntX);
		glColor4f(1, 0, 0, 1);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntXNum / 3);

		glVertexPointer(3, GL_FLOAT, 0, axis->axisPntY);
		glColor4f(0, 1, 0, 1);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntYNum / 3);

		glVertexPointer(3, GL_FLOAT, 0, axis->axisPntZ);
		glColor4f(0, 0, 1, 1);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntZNum / 3);

		glVertexPointer(3, GL_FLOAT, 0, axis->axisPntO);
		glColor4f(1, 1, 0, 1);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntONum / 3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
}

//void GLDrawer::DrawMeasures(SceneNode* node,RenderAction* action)
//{
//	if (node == NULL)
//	{
//		return;
//	}
//
//	MeasureGroup* measuregroup = (MeasureGroup*)node;
//
//}

void GLDrawer::DrawFPS(SceneNode* node, RenderAction* action)
{
	if (node)
	{
		RenderContext* gl = action->GetGLContext();

		FPSNode* fpsNode = (FPSNode*) node;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glViewport(fpsNode->m_iViewX, fpsNode->m_iViewY, fpsNode->m_iViewWidth,
				fpsNode->m_iViewHeight);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrthof(-fpsNode->m_iViewWidth / 2, fpsNode->m_iViewWidth / 2,
				-fpsNode->m_iViewHeight / 2, fpsNode->m_iViewHeight / 2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glShadeModel(GL_SMOOTH);
		glScalef(2, 2, 1);
		glTranslatef(-50, 0, 0);

		fpsNode->SetFPS(action->GetRenderManager()->GetFPS());

		glEnableClientState(GL_VERTEX_ARRAY);
		ComText* comText = fpsNode->GetFPSMesh();
		Mesh* textMesh = comText->GetMesh();

		int dataLength = textMesh->GetDataLength();
		bool isUseIndex = textMesh->GetRefMesh()->IsUseIndex();
		Vector3* positionArray =
				textMesh->GetRefMesh()->GetPositionArray()->data()
						+ textMesh->GetDataOffset();
		if (isUseIndex)
		{
			M3D_INDEX_TYPE* indexArray =
					textMesh->GetRefMesh()->GetIndexArray()->data();
			GLShapeDrawer::DrawTriWithIndex(positionArray, NULL, indexArray,
					dataLength);
		}
		else
		{
			GLShapeDrawer::DrawTriNoIndex(positionArray, NULL, dataLength);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
//		DrawText(fpsNode->m_pTextNode, action);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

}

void GLDrawer::DrawText(SceneNode* node, RenderAction* action)
{
	TextNode* textNode = (TextNode*) node;
	RenderContext* gl = action->GetGLContext();
	glPushMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
#ifdef __MOBILE__
	FTFont *font = CText::GetRender(); //FTPolygonFont
	glColor4f(textNode->m_color.m_r, textNode->m_color.m_g, textNode->m_color.m_b,
			textNode->m_color.m_a);
	if (font != NULL)
	{
		FTPoint pos(textNode->m_Pos.m_x, textNode->m_Pos.m_y, textNode->m_Pos.m_z);
		font->Render(textNode->m_pText, -1, pos);
	}
	else

	{
		LOGE("GLDrawer::DrawText font not init.");
	}
#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPopMatrix();

}

//initial
void GLDrawer::InitialGL()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	float mat_specular[] ={ 0.7f, 0.7f, 0.7f, 1.0f };
	float mat_shininess[] = { 10.0f };
	// // Lighting

	//暂时改为true；适应stl牙模型未封闭模型的背面显示。
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_LIGHTING);

	glFrontFace(GL_CCW);

	glClearColor(0.9, 0.9, .9, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
}

void GLDrawer::ClearGLState()
{
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GLDrawer::ApplyCamera(RenderAction* action)
{
	GLDrawer::ApplyCamera(NULL, action);
}

void GLDrawer::ApplyCamera(SceneNode* sceneNode, RenderAction* action)
{
	ClearGLState();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	RenderContext* gl = action->GetGLContext();

	CameraNode* camera = action->GetCamera();
	if (!camera)
	{
		return;
	}

	const IntRect& intRect = camera->GetViewPort().GetRect();
	glViewport(intRect.m_left, intRect.m_top, intRect.m_right,
			intRect.m_bottom);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float*) camera->GetProjection().Transpose().Data());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float*) camera->GetView().ToMatrix4().Transpose().Data());
}

//void GLDrawer::PrepareInteractiveInfo(SceneNode* sceneNode,
//		RenderAction* action)
//{
//	LOGE("PrepareInteractiveInfo");
//	action->m_handlerGroupCache = (HandlerGroup*) sceneNode;
//}

//void GLDrawer::DrawInteractiveInfo(RenderAction*action)
//{
//	LOGE("DrawInteractiveInfo");
//	SceneNode*sceneNode = action->m_handlerGroupCache;
//	if (action != NULL)
//	{
//
//	}
//	else
//	{
//		return;
//	}
//
////	if (action->isRotating)
//	{
////		CameraNode* cameraNode = action->m_pCameraNodeCache;
////		if (cameraNode != NULL)
////		{
////			if (!(cameraNode->GetRotateCenter() == cameraNode->GetOrigRotateCenter())
////					|| action->IsShowRotationCenter())
////			{
////				Vector3& rotateCenter = cameraNode->GetRotateCenter();
////				glPushMatrix();
////				glLoadIdentity();
////				glDisable(GL_DEPTH_TEST);
////				glDisable(GL_LIGHTING);
////				glLoadMatrixf((float*) cameraNode->GetModelViewMatrix().GetData());
////				//绘制旋转中心点
////				glEnableClientState(GL_VERTEX_ARRAY);
////				//glEnableClientState(GL_COLOR_ARRAY);
////
////				glColor4f(1.0f, 0, 0, 1);
////				glPointSize(15);
////				float vec[3] =
////				{ rotateCenter.x, rotateCenter.y, rotateCenter.z };
////				//Vector3 vec=cameraNode->rotateCenter*-1;
////				glVertexPointer(3, GL_FLOAT, 0, vec);
////				//glColorPointer(4, GL_FLOAT, 0, backgroundNode->backColor);
////				glDrawArrays(GL_POINTS, 0, 1);
////
////				//glDisableClientState(GL_COLOR_ARRAY);
////				glDisableClientState(GL_VERTEX_ARRAY);
////
////				glPopMatrix();
////			}
////		}
//	}
//}

//void GLDrawer::DrawSection(SceneNode* sceneNode, RenderAction* action)
//{

	//LOGI("GLDrawer::DrawSection");
//	SectionNode* sectionNode = (SectionNode*) sceneNode;
//	clipPlaneDrawing = sectionNode->GetEnable();
//	if (clipPlaneDrawing)
//	{
//		//CameraNode* camera =action->cameraNode;
//		//float plan[4]={sectionNode->m_position[0],sectionNode->m_position[1],sectionNode->m_position[2],sectionNode->m_position[3]};
//		//plan[3]=plan[3]-camera->frustumPos.z;
//		//
//		//LOGE("GLDrawer::DrawSection sectionNode->m_position %f %f %f %f ",sectionNode->m_position[0],
//		//	sectionNode->m_position[1],
//		//	sectionNode->m_position[2],
//		//	sectionNode->m_position[3]);
//
//		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//
//		int maxClipPlaneCount = 6;
//		int planeCount = sectionNode->GetEnabledPlaneList().size();
//		CurClipPlaneCount =
//				maxClipPlaneCount > planeCount ? planeCount : maxClipPlaneCount;
//		//LOGI("GLDrawer::DrawSection: count:%d",CurClipPlaneCount);
//		for (int i = 0; i < CurClipPlaneCount; i++)
//		{
//			Plane* plane = sectionNode->GetEnabledPlaneList().at(i);
//			glClipPlanef(GL_CLIP_PLANE0 + i, plane->GetEquation());
//		}
//	}
//	else
//	{
//		//LOGI("GLDrawer::DrawSection disable cnt:%d",CurClipPlaneCount);
//		for (int i = 0; i < CurClipPlaneCount; i++)
//		{
//			glDisable(GL_CLIP_PLANE0 + i);
//		}
//		CurClipPlaneCount = 0;
//		//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//	}
//	LOGI("GLDrawer::DrawSection end.");
//}

//void GLDrawer::DrawSolidPlan(ImageBoard* plan, RenderAction* action)
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glVertexPointer(3, GL_FLOAT, 0, plan->GetVertexsArray());
//	glDrawArrays(GL_TRIANGLES, 0, 24);
//	glDisableClientState(GL_VERTEX_ARRAY);
//}
//
void GLDrawer::LogGLState()
{
	LOGI("log gl state start.");

	if (glIsEnabled(GL_FOG))
		LOGE("GL_FOG");
	if (glIsEnabled(GL_LIGHTING))
		LOGE("GL_LIGHTING");
	if (glIsEnabled(GL_TEXTURE_2D))
		LOGE("GL_TEXTURE_2D");
	if (glIsEnabled(GL_CULL_FACE))
		LOGE("GL_CULL_FACE");
	if (glIsEnabled(GL_ALPHA_TEST))
		LOGE("GL_ALPHA_TEST");
	if (glIsEnabled(GL_BLEND))
		LOGE("GL_BLEND");
	if (glIsEnabled(GL_COLOR_LOGIC_OP))
		LOGE("GL_COLOR_LOGIC_OP");
	if (glIsEnabled(GL_DITHER))
		LOGE("GL_DITHER");
	if (glIsEnabled(GL_STENCIL_TEST))
		LOGE("GL_STENCIL_TEST");
	if (glIsEnabled(GL_DEPTH_TEST))
		LOGE("GL_DEPTH_TEST");
	if (glIsEnabled(GL_POINT_SMOOTH))
		LOGE("GL_POINT_SMOOTH");
	if (glIsEnabled(GL_LINE_SMOOTH))
		LOGE("GL_LINE_SMOOTH");
	if (glIsEnabled(GL_SCISSOR_TEST))
		LOGE("GL_SCISSOR_TEST");
	if (glIsEnabled(GL_COLOR_MATERIAL))
		LOGE("GL_COLOR_MATERIAL");
	if (glIsEnabled(GL_NORMALIZE))
		LOGE("GL_NORMALIZE");
	if (glIsEnabled(GL_RESCALE_NORMAL))
		LOGE("GL_RESCALE_NORMAL");
	if (glIsEnabled(GL_POLYGON_OFFSET_FILL))
		LOGE("GL_POLYGON_OFFSET_FILL");
	if (glIsEnabled(GL_VERTEX_ARRAY))
		LOGE("GL_VERTEX_ARRAY");
	if (glIsEnabled(GL_NORMAL_ARRAY))
		LOGE("GL_NORMAL_ARRAY");
	if (glIsEnabled(GL_COLOR_ARRAY))
		LOGE("GL_COLOR_ARRAY");
	if (glIsEnabled(GL_TEXTURE_COORD_ARRAY))
		LOGE("GL_TEXTURE_COORD_ARRAY");
	if (glIsEnabled(GL_MULTISAMPLE))
		LOGE("GL_MULTISAMPLE");
	if (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE))
		LOGE("GL_SAMPLE_ALPHA_TO_COVERAGE");
	if (glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE))
		LOGE("GL_SAMPLE_ALPHA_TO_ONE");
	if (glIsEnabled(GL_SAMPLE_COVERAGE))
		LOGE("GL_SAMPLE_COVERAGE");

	LOGI("log gl state end.");
}

void GLDrawer::ApplyPointLight(Light* lightData)
{

}
void GLDrawer::ApplyDirLight(Light* light)
{

//	Matrix4* worldMatrix = light->getWorldMatrix();
//	Vector3 position  = worldMatrix-

    GLfloat light_ambient[]={0.2f,0.2f,0.2f,1.0f};
    GLfloat light_diffuse[]={0.8f,0.8f,0.8f,1.0};
    GLfloat light_specular[]={0.3f,0.3f,0.3f,1.0};
//  GLfloat light_position[]={2.0,2.0,2.0,0.0};

    GLfloat light_position[]={ 0.0, 0.0, 1000000.0, 1.0 };
//  GLfloat mat_diffuse[]={0.8,0.8,0.8,1.0};
    
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
 
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    //启用双面光照
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

void GLDrawer::ApplyLights(RenderAction* action)
{
	if (action != NULL)
	{
//        RenderContext* gl = action->GetGLContext();
//
//        gl->MatrixMode(GL_PROJECTION);
//        gl->LoadIdentity();
//       
//        gl->MatrixMode(GL_MODELVIEW);
//        gl->LoadIdentity();
//
//		//TODO 光照的实现没有很好的封装和实现
//		LightList* lightList = action->GetLights();
//		if (lightList != NULL)
//		{
//			Light* light = NULL;
//			for (int i = 0; i < lightList->size(); i++)
//			{
//				light = lightList->at(i);
//				int lightType = light->GetType();
//
//				switch (lightType)
//				{
//				case SHAPE_LIGHT_DIRECTIONAL :
//					ApplyDirLight(light);
//					break;
//				case SHAPE_LIGHT_POINT:
//					break;
//				case SHAPE_LIGHT_SPOT:
//					break;
//				}
//			}
//		}
		//TODO
		GLfloat light_ambient[4];
		GLfloat light_ambient1[] =
		{ 1.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light_diffuse[] =
		{ 1.0f, 1.0f, 1.0f, 1.0 };
		GLfloat light_specular[] =
		{ 1.0f, 1.0f, 1.0f, 0.1 };

		GLfloat material_specular[] =
		{ 0.70, 0.70, 0.70, 0.50 };
		GLfloat material_shininess[] =
		{ 100.3 };

		if(Parameters::Instance()->m_LightingMode==0 )
		{
			light_ambient[0]=0.08f;
			light_ambient[1]=0.08f;
			light_ambient[2]=0.08f;
			light_ambient[3]=1.0f;


		}
		else if(Parameters::Instance()->m_LightingMode==1)
		{
			light_ambient[0]=1.0f;
			light_ambient[1]=1.0f;
			light_ambient[2]=1.0f;
			light_ambient[3]=1.0f;

			light_specular[0] = 0.0f;
			light_specular[1] = 0.0f;
			light_specular[2] = 0.0f;
			light_specular[3] = 1.0f;

			light_diffuse[0]=0.7f;
			light_diffuse[1]=0.7f;
			light_diffuse[2]=0.7f;
			light_diffuse[3]=1.0f;

			//LOGI("人像模式");


		}
		else if(Parameters::Instance()->m_LightingMode== 3)
		{
			//高光模式

			light_ambient[0]=0.18f;
			light_ambient[1]=0.18f;
			light_ambient[2]=0.18f;
			light_ambient[3]=1.0f;

			material_specular[0]=0.70f;
			material_specular[1]=0.70f;
			material_specular[2]=0.70f;
			material_specular[3]=0.70f;

			material_shininess[0] = 90.0f;

			light_specular[0] = 1.0f;
			light_specular[1] = 1.0f;
			light_specular[2] = 1.0f;
			light_specular[3] = 1.0f;



		}
		else if(Parameters::Instance()->m_LightingMode==200)
		{
			light_ambient[0]=1.0f;
			light_ambient[1]=1.0f;
			light_ambient[2]=1.0f;
			light_ambient[3]=1.0f;

			light_specular[0] = 0.0f;
			light_specular[1] = 0.0f;
			light_specular[2] = 0.0f;
			light_specular[3] = 1.0f;

			light_diffuse[0]=0.7f;
			light_diffuse[1]=0.7f;
			light_diffuse[2]=0.7f;
			light_diffuse[3]=1.0f;

			//LOGI("人像模式");


		}
//		GLfloat material_ambient[] =
//		{0.250, 0.250, 0.250, 1.00};
//		GLfloat material_diffuse[]=
//		{0.400000, 0.400000, 0.400000, 1.000000};


		CameraNode* camera = action->GetCamera();

		Vector3 cameraWorldPos = camera->GetWorldDirection();

//		LOGE("camera pos %s",camera->GetWorldPosition().Tostring().c_str());

		Vector4 origDir(cameraWorldPos.m_x, cameraWorldPos.m_y,
				cameraWorldPos.m_z, 0.0);

		GLfloat ori[] =
		{ origDir.m_x, origDir.m_y, origDir.m_z, 1.0f };
		// glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,ori);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, origDir.Data());

//        GLfloat light_ambient3[]={0.05f,0.05f,0.05f,1.0f};
//               GLfloat light_diffuse3[]={0.10f,0.10f,0.10f,1.0};
//               GLfloat light_specular3[]={1.0f,1.0f,1.0f,1.0};
//
//        glLightfv(GL_LIGHT3,GL_AMBIENT,light_ambient3);
//                glLightfv(GL_LIGHT3,GL_DIFFUSE,light_diffuse3);
//                //glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient);
//                glLightfv(GL_LIGHT3,GL_SPECULAR,light_specular3);
//                glLightfv(GL_LIGHT3,GL_POSITION,origDir.Data());

		//暂时通过添加量过相对的光源来解决模型不能照亮的问题
		origDir = Vector4(-cameraWorldPos.m_x, -cameraWorldPos.m_y,
				-cameraWorldPos.m_z, 0.0);

		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, origDir.Data());

		/*聚光灯，增强效果*/
//		origDir = Vector4(cameraWorldPos.m_x + 50, cameraWorldPos.m_y,
//				cameraWorldPos.m_z + 50, 1.0);
//		GLfloat light_ambient1[] =
//		{ 0.08f, 0.08f, 0.08f, 1.0f };
//		GLfloat light_diffuse1[] =
//		{ 0.20f, 0.20f, 0.20f, 1.0 };
//		GLfloat light_specular1[] =
//		{ 0.20f, 0.20f, 0.20f, 1.0 };
//		glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient1);
//		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse1);
//		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient1);
//		glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular1);
//		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 32.0f);
//		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
//		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.14f);
//		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.07f);

		if(Parameters::Instance()->m_LightingMode== 3)
		{
			//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
			//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		// glEnable(GL_LIGHT2);
		//glEnable(GL_LIGHT3);

		glEnable(GL_NORMALIZE);
		//启用双面光照
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}
}
}

