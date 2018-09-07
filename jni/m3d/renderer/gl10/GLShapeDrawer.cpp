#include "m3d/renderer/gl10/GLShapeDrawer.h"
#include "m3d/M3DMacros.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"

#include "m3d/base/Vector3.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/Note.h"
#include "m3d/model/PMIData.h"
#include "m3d/model/PolyLine.h"
#include "m3d/model/Image.h"

#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"

#include "m3d/scene/ShapeNode.h"

#include "sview/views/Parameters.h"
#include "m3d/utils/Trackball.h"

#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/Section.h"
#include "m3d/graphics/TextNode.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/graphics/Texture.h"
#include "m3d/graphics/Texture.h"

#include "m3d/graphics/Material.h"

#include "m3d/Handler/Handler.h"
#include "m3d/Handler/HandlerRotateCenter.h"
#include "m3d/Handler/HandlerNode.h"
#include "m3d/Handler/HandlerGroup.h"

#include "m3d/Handler/HandlerPoint.h"

#include "m3d/extras/note/NoteGroup.h"
#include "m3d/extras/note/VoiceNote.h"
#include "m3d/graphics/MeshBoard.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/model/Point.h"

#define TEXT_USE_RENDER
//#define GL_GLEXT_PROTOTYPES

using namespace SVIEW;

namespace M3D
{

long GLShapeDrawer::drawBoxTime = 0;
long GLShapeDrawer::drawMeshTime = 0;
long GLShapeDrawer::drawPMITime = 0;

const float GLShapeDrawer::PolygonOffset = 0.1f;

void GLShapeDrawer::DrawShapeBox(BoundingBox& box, bool isSelected)
{
	bool drawBoxMinMaxPoint = false;

	Color color;
	if (isSelected)
		color = Parameters::Instance()->m_SelectedColor;
	else
		color = Parameters::Instance()->m_BoxColor;

	if (box.Defined())
	{
		glColor4f(color.m_r, color.m_g, color.m_b, color.m_a);
		vector<float> boxVertex; box.GetVertexs(boxVertex);
		float* points = boxVertex.data();
		glVertexPointer(3, GL_FLOAT, 0, points);
		glDrawElements(GL_LINES, 24, M3D_GL_INDEX_TYPE,
				BoundingBox::boxIndexs);

		//绘制max 和min 点
		if (drawBoxMinMaxPoint)
		{
			Vector3& maxPos = box.m_max;
			Vector3& minPos = box.m_min;

			glColor4f(0, 1, 0, 1);
			glVertexPointer(3, GL_FLOAT, 0, minPos.Data());
			glDrawArrays(GL_POINTS, 0, 1);

			glColor4f(1, 0, 1, 1);
			glVertexPointer(3, GL_FLOAT, 0, maxPos.Data());
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}
}

void GLShapeDrawer::DrawRenderPassGroup(RenderAction* action)
{
	ApplyCamera(NULL,action);
	if(DoSection(action, true))
	{
		glClearStencil(0x0);
		glEnable(GL_STENCIL_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0, 0x1);
		glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
		glStencilMask(0x1);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		DrawSampleModelPassGroug(action);
//		glStencilMask(0x0);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_CLIP_PLANE0);
		glStencilFunc(GL_EQUAL, 1, 0x1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		DrawCapPolygon(action);
//		//绘制结束///置
//		glStencilMask(0x1);
        glClearStencil(0x0);
        glClear(GL_STENCIL_BUFFER_BIT);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xffff);
		glDisable(GL_STENCIL_TEST);
	}

	DoSection(action,true);
    GLShapeDrawer::ApplyLights(action);

	RenderEffect* renderType = action->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	RenderQueuePriority& currentEffect = renderType->GetRenderQueuePriority();
	vector<RenderTech*>& effectsData = currentEffect.GetData();

	RenderTech* effect = NULL;

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
				GLShapeDrawer::DrawSolidRenderPassGroup(action, &it->second);
			}else if(renderType == RenderableType::RGT_POINT)
			{
				GLShapeDrawer::DrawPointPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_TRANSPARENT)
			{
				GLShapeDrawer::DrawTranRenderPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_INTOP)
			{
				//LOGE("render in top");
//				GLShapeDrawer::DrawINTOPRenderPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_EDGELINE)
			{
				GLShapeDrawer::DrawEdgesRenderPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_EDGELINEINTOP)
			{
				GLShapeDrawer::DrawEdgesInTopRenderPassGroup(action,
						&it->second);
			}
			else if (renderType == RenderableType::RGT_PMI)
			{
				//LOGE("preparPMIS error");
				GLShapeDrawer::DrawPMISRenderPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_BOX)
			{
				GLShapeDrawer::DrawBoxRenderPassGroup(action, &it->second);
			}
			else if (renderType == RenderableType::RGT_TRILINE)
			{
				GLShapeDrawer::DrawTriLineRenderPassGroup(action,
						&it->second);
			}
			else if (renderType == RenderableType::RGT_NOTE)
			{
				GLShapeDrawer::DrawNoteRenderPassGroup(action,
						&it->second);
			}
			else if (renderType == RenderableType::RGT_MEASURE)
			{
				GLShapeDrawer::DrawMeasureRenderPassGroup(action,
						&it->second);
			}
			else if (renderType == RenderableType::RGT_HANDLER)
			{
				GLShapeDrawer::DrawHandlerRenderPassGroup(action,
						&it->second);
			}
			else
			{

			}
		}

	}

	DoSection(action,false);
}

bool GLShapeDrawer::DoSection(RenderAction* action,bool state)
{
	//if (SVIEW::Parameters::Instance()->m_OpenGLESVersion > 1)
	{
		Section* section = (Section*)action->GetSection();
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
				if ((*iter)->IsPlaneAvaliable())
				{
#if _WIN32
					float* clipPlaneData = (*iter)->GetTransformPlaneParam();
					double tempClipPlane[4];
					tempClipPlane[0] = clipPlaneData[0];
					tempClipPlane[1] = clipPlaneData[1];
					tempClipPlane[2] = clipPlaneData[2];
					tempClipPlane[3] = clipPlaneData[3];
					glClipPlane(GL_CLIP_PLANE0 + i, tempClipPlane);
					glEnable(GL_CLIP_PLANE0 + i);
#else
					glClipPlanef(GL_CLIP_PLANE0 + i, (*iter)->GetEquation());
					glEnable(GL_CLIP_PLANE0 + i);
#endif
				}
			}
		}
		else
		{
			list<SectionPlane*>::iterator iter;

			int i = 0;

			for (iter = planeList->begin(); iter != planeList->end(); ++iter, ++i)
			{
				glDisable(GL_CLIP_PLANE0 + i);


			}
			if (Parameters::Instance()->m_showSection)
			{
				for (iter = planeList->begin(); iter != planeList->end(); ++iter)
				{
					//LOGI("BEGIN section");
					DrawSectionPlane(action, *iter);
				}
			}

			for (iter = planeList->begin(); iter != planeList->end(); ++iter)
			{
				//LOGI("BEGIN section");
				DrawSectionLines(action, (*iter)->m_figureSectonLine.linesData); //TODO 剖切线，盖面

			}

		}

		if (section->IsShowCappingPlane())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void GLShapeDrawer::DrawSectionPlane(RenderAction* action,
		SectionPlane* sectionPlane)
{
	if(sectionPlane)
	{
		ClearGLState();
		RenderContext* gl = action->GetGLContext();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glDisable(GL_LIGHTING);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glLineWidth(1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		Color& faceColor = sectionPlane->GetFaceColor();
		glColor4f(faceColor.m_r, faceColor.m_g, faceColor.m_b, faceColor.m_a);
		DrawTriNoIndex(sectionPlane->GetPointArray(),NULL,36);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		Color& edgeColor = sectionPlane->GetEdgeColor();
		glColor4f(edgeColor.m_r, edgeColor.m_g, edgeColor.m_b, edgeColor.m_a);

		GLShapeDrawer::DrawTriLinesNoIndex(sectionPlane->GetLinePointArray(),
				NULL, 24);
	}
}

void GLShapeDrawer::DrawBoxRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}
	ClearGLState();
	RenderContext* gl = action->GetGLContext();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glLineWidth(3);

	if (action->GetRenderBox().Defined())
	{
		glPushMatrix();
		DrawShapeBox(action->GetRenderBox(), false);
		glPopMatrix();
	}

	//RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	//for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	//{
	//	Model* shape = (Model*) (*it);
	//	glPushMatrix();
	//	glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
	//	DrawShapeBox(shape->GetBoundingBox(), shape->IsSelected());
	//	glPopMatrix();
	//}
}

void GLShapeDrawer::DrawPMISRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}

	glDisable(GL_CULL_FACE);
	ClearGLState();
	RenderContext* gl = action->GetGLContext();

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	RenderTech* effect = RenderStateArray->GetRenderTech();
	effect->BeginEffect();

	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(2);
	glDisable(GL_LIGHTING);

	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		PMIData* pmi = (PMIData*) (*it);

		glPushMatrix();
		glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
		DrawPMINew(pmi, action);
		glPopMatrix();
	}

	effect->EndEffect();
	glDisableClientState(GL_VERTEX_ARRAY);

}
void GLShapeDrawer::DrawEdgesInTopRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}
	ClearGLState();
	RenderContext* gl = action->GetGLContext();
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(6);

	glPushMatrix();
	glLoadIdentity();

	vector<Edge*>* edges = NULL;
	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{

	}
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_LINE_SMOOTH);
}

void GLShapeDrawer::DrawEdgesRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}

	RenderContext* gl = action->GetGLContext();
	ClearGLState();
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glLineWidth(2);

	vector<Edge*>* edges = NULL;
	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Renderable* edgeRenderData = *it;
		if (edgeRenderData->GetDataLength())
		{
			glPushMatrix();
			glMultMatrixf(edgeRenderData->GetRenderWorldMatrix()->Data());

			const Color& edgeColor = edgeRenderData->GetRenderColor();
			glColor4f(edgeColor.m_r, edgeColor.m_g, edgeColor.m_b, edgeColor.m_a);

			//根据材质属性，设置显示状态
			//		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
			//				(*it)->GetRenderColor().Data());

			glVertexPointer(3, GL_FLOAT, 0, edgeRenderData->GetVertexPos());
			glDrawArrays(GL_LINES, 0, edgeRenderData->GetDataLength());

			glPopMatrix();
		}
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
}

void GLShapeDrawer::DrawTriLineRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}

	RenderContext* gl = action->GetGLContext();
	ClearGLState();
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glLineWidth(2);

	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Face* faceRenderData = (Face*)(*it);
	

		int dataLength = faceRenderData->GetDataLength()*2;
		bool isUseIndex = faceRenderData->IsUseIndex();

		glPushMatrix();
		glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
		Vector3* positionArray = faceRenderData->GetLinesModeVertexPos();
		if (dataLength > 0 && positionArray)
		{
			Vector3* positionArray = faceRenderData->GetLinesModeVertexPos();
			Vector3* normalArray = faceRenderData->GetVertexNormal();
			if(!faceRenderData->IsSelected())
			{
				const Color& triColor	= Color::YELLOW;
				glColor4f(triColor.m_r, triColor.m_g, triColor.m_b, triColor.m_a);
			}
			else
			{
				const Color& triColor	= (*it)->GetRenderColor();
				glColor4f(triColor.m_r, triColor.m_g, triColor.m_b, triColor.m_a);
			}

			if (isUseIndex)
			{
				M3D_INDEX_TYPE* indexArray = faceRenderData->GetLineModeIndex();
				GLShapeDrawer::DrawTriLinesWithIndex(positionArray,
						normalArray, indexArray,
						dataLength);
			}
			else
			{
				GLShapeDrawer::DrawTriLinesNoIndex(positionArray,
						normalArray, dataLength);
			}
		}
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
}

void GLShapeDrawer::DrawNoteRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}

	RenderContext* gl = action->GetGLContext();

	RenderabelArray::iterator it =
			RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		glPushMatrix();

		Note* note = (Note*) (*it);
		int noteType = note->GetType();

		glMultMatrixf(note->GetRenderWorldMatrix()->Data());

		switch (noteType)
		{
		case SHAPE_VOICE_NOTE:
		{
			VoiceNote* voiceNote = (VoiceNote*) note;
			DrawImageBoard(voiceNote->GetImageboard());
		}
			break;
		case SHAPE_TEXT_NOTE:
			DrawNote(note, action);
			break;
		case SHAPE_SEQUENCE_NUMBER_NOTE:
			DrawNote(note, action);
			break;
		case SHAPE_NOTE:
			DrawNote(note, action);
			break;
		case SHAPE_MEASURE_BASE:
		case SHAPE_MEASURE_DISTANCE:
		case SHAPE_MEASURE_ANGLE:
		case SHAPE_MEASURE_PROPERTY:
			DrawNote(note, action);
			break;
		}

		glPopMatrix();
	}
}

void GLShapeDrawer::DrawMeasureRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{

}

void GLShapeDrawer::DrawHandlerRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}

		RenderContext* gl = action->GetGLContext();
		ClearGLState();
		glEnable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glEnableClientState(GL_VERTEX_ARRAY);

		RenderabelArray::iterator it =
				RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			glPushMatrix();
			Handler* handler = (Handler*) (*it);
			int noteType = handler->GetType();

			glMultMatrixf(handler->GetRenderWorldMatrix()->Data());

			if (noteType == SHAPE_POINT_HANDLE)
			{
				HandlerPoint* pointHandler = (HandlerPoint*)handler;
				GLShapeDrawer::DrawPointHandler(pointHandler);
			}
			else if (noteType == SHAPE_PLANE_HANDLE ||
					noteType == SHAPE_AXIS_HANDLE ||
					noteType == SHAPE_ROTATE_HANDLE ||
					noteType == SHAPE_ROTATE_HANDLE ||
					noteType == SHAPE_ROTATECENTER
					)
			{
				GLShapeDrawer::DrawMeshHandler(handler);
			}

			glPopMatrix();
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
}

void GLShapeDrawer::DrawTranRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{

	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}
	ClearGLState();
	RenderContext* gl = action->GetGLContext();
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	RenderTech* effect = RenderStateArray->GetRenderTech();
	effect->BeginEffect();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(PolygonOffset, PolygonOffset);

	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Renderable* faceRenderData = *it;
        int vertexNum = faceRenderData->GetUseIndexVertexNumber();
		Matrix3 rot_= action->GetCamera()->GetView().ToMatrix3();//TODO
		int dataLength = faceRenderData->GetDataLength();
		bool isUseIndex = faceRenderData->IsUseIndex();

		

		glPushMatrix();
		glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
		HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
		if (dataLength > 0 && vertexBuffer)
		{
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			char* vertexAddress = (char*)vertexBuffer->Bind();
 
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
			M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
			M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();

			glVertexPointer(3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
			glNormalPointer(GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));

			Material* material = dynamic_cast<Material*>(faceRenderData->GetRenderMaterial());//获取材质

			//如果材质存在，则使用材质绘制
			if (material)
			{
				//根据材质属性，设置显示状态
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
						(*it)->GetRenderColor().Data());

				Vector3* textureCoordsArray = faceRenderData->GetTextureCoords();

				if(material->GetDiffuseMap())
				{
					unsigned int OGLObj = material->GetDiffuseMap()->GetOGLObj();

					if (OGLObj) //纹理存在
					{
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(3, GL_FLOAT, 0, (GLvoid *)texoffset);
						if (!textureCoordsArray)
						{
							//textureCoordsArray =
							//		faceRenderData->GetNewTextureCooords(
							//				vertexNum);
							//M3D_INDEX_TYPE* indexArray =
							//		faceRenderData->GetIndex();
							//bool isContinue = ComputerSphereTexCoords(OGLObj,
							//		material, faceRenderData,
							//		textureCoordsArray, indexArray, normalArray,
							//		positionArray, rot_, vertexNum);
							//if (isContinue)
							//{
							//	glPopMatrix();
							//	continue;
							//}

						}
						Matrix4* texture2DTransform = material->GetTexture2DTransform();
						if (isUseIndex)
						{
							//M3D_INDEX_TYPE* indexArray =
							//		(M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
							//DrawTexture2DWithIndex(OGLObj, positionArray,
							//		normalArray,textureCoordsArray,
							//		indexArray, dataLength,texture2DTransform);

							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							DrawTexture2DWithIndex(OGLObj, vertexBuffer, indexBuffer, texture2DTransform, dataLength,
								indexArray);

						}
						else
						{
							//DrawTexture2DNoIndex(OGLObj, positionArray,
							//		normalArray,textureCoordsArray,
							//		dataLength,texture2DTransform);

							DrawTexture2DNoIndex(OGLObj, vertexBuffer, texture2DTransform, dataLength);
						}

					}
					else
					{
						//if (isUseIndex)
						//{
      //                      M3D_INDEX_TYPE* indexArray =
      //                      (M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
						//	GLShapeDrawer::DrawTriWithIndex(positionArray,
						//			normalArray, indexArray, dataLength);
						//}
						//else
						//{
						//	GLShapeDrawer::DrawTriNoIndex(positionArray,
						//			normalArray, dataLength);
						//}

						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						if (isUseIndex)
						{
							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
						}
						else
						{
							GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
						}
					}
				}
				else
				{
					//if (isUseIndex)
					//{
     //                   M3D_INDEX_TYPE* indexArray =
     //                   (M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
					//	GLShapeDrawer::DrawTriWithIndex(positionArray,
					//			normalArray, indexArray, dataLength);
					//}
					//else
					//{
					//	GLShapeDrawer::DrawTriNoIndex(positionArray,
					//			normalArray, dataLength);
					//}

					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}
			}
			else
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
						(*it)->GetRenderColor().Data());
				{
					//根据颜色绘制
					//if (isUseIndex)
					//{
     //                   M3D_INDEX_TYPE* indexArray =
     //                   (M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
					//	GLShapeDrawer::DrawTriWithIndex(positionArray,
					//			normalArray, indexArray, dataLength);
					//}
					//else
					//{
					//	GLShapeDrawer::DrawTriNoIndex(positionArray,
					//			normalArray, dataLength);
					//}

					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}

			}	
			vertexBuffer->UnBind();
		}
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	effect->EndEffect();

	glPolygonOffset(0.0, 0.0);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

}
void GLShapeDrawer::DrawSolidRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
	{
		return;
	}
	ClearGLState();
	RenderContext* gl = action->GetGLContext();

	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(PolygonOffset, PolygonOffset);

//	RenderTech* effect = RenderStateArray->GetRenderTech();
//	effect->BeginEffect();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	Matrix3 rot_= action->GetCamera()->GetView().ToMatrix3();//TODO
	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Renderable* faceRenderData = *it;

		int dataLength = faceRenderData->GetDataLength();
		bool isUseIndex = faceRenderData->IsUseIndex();
		int vertexNum = faceRenderData->GetUseIndexVertexNumber();

		glPushMatrix();
		glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
		HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();

		if (dataLength > 0 && vertexBuffer)
		{
			HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
			char* vertexAddress = (char*)vertexBuffer->Bind();
			M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
			M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
			M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();

			glVertexPointer(3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
			glNormalPointer( GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));

			Material* material = dynamic_cast<Material*>(faceRenderData->GetRenderMaterial());//获取材质

			//如果材质存在，则使用材质绘制
			if (material)
			{
				//根据材质属性，设置显示状态
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
						(*it)->GetRenderColor().Data());

				Vector3* textureCoordsArray = faceRenderData->GetTextureCoords();

				if(material->GetDiffuseMap() )
				{
					unsigned int OGLObj = material->GetDiffuseMap()->GetOGLObj();

					if (OGLObj) //纹理存在
					{

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(3, GL_FLOAT, 0, (GLvoid *)texoffset);
						if (!textureCoordsArray)
						{
							//textureCoordsArray =
							//		faceRenderData->GetNewTextureCooords(
							//				vertexNum);
							//M3D_INDEX_TYPE* indexArray =
							//		faceRenderData->GetIndex();
							// bool isContinue=ComputerSphereTexCoords( OGLObj,material, faceRenderData,textureCoordsArray,indexArray,
							//		normalArray,positionArray, rot_, vertexNum);
							// if(isContinue)
							// {
							//	 glPopMatrix();
							//	 continue;
							// }
						}
						Matrix4* texture2DTransform = material->GetTexture2DTransform();

						if (isUseIndex)
						{
							//M3D_INDEX_TYPE* indexArray =
							//		(M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
							//DrawTexture2DWithIndex(OGLObj, positionArray,
							//		normalArray,textureCoordsArray,
							//		indexArray, dataLength,texture2DTransform);

							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							DrawTexture2DWithIndex(OGLObj, vertexBuffer, indexBuffer, texture2DTransform, dataLength,
								indexArray);

						}
						else
						{
							//DrawTexture2DNoIndex(OGLObj, positionArray,
							//		normalArray,textureCoordsArray,
							//		dataLength,texture2DTransform);

							DrawTexture2DNoIndex(OGLObj, vertexBuffer, texture2DTransform, dataLength);
						}

					}
					else
					{
						//if (isUseIndex)
						//{
      //                      M3D_INDEX_TYPE* indexArray =
      //                      (M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
						//	GLShapeDrawer::DrawTriWithIndex(positionArray,
						//			normalArray, indexArray, dataLength);
						//}
						//else
						//{
						//	GLShapeDrawer::DrawTriNoIndex(positionArray,
						//			normalArray, dataLength);
						//}

						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
						if (isUseIndex)
						{
							M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
							GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
						}
						else
						{
							GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
						}
					}

				}
				else
				{
					//if (isUseIndex)
					//{
     //                   M3D_INDEX_TYPE* indexArray =
     //                   (M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
					//	GLShapeDrawer::DrawTriWithIndex(positionArray,
					//			normalArray, indexArray, dataLength);
					//}
					//else
					//{
					//	GLShapeDrawer::DrawTriNoIndex(positionArray,
					//			normalArray, dataLength);
					//}
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}
			}
			else
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
						(*it)->GetRenderColor().Data());

				//			//如果性能跟不上，进入简化点模式
				//			if(simplifyFactor > 0)
				//			{
				//				if (isUseIndex)
				//				{
				//					M3D_INDEX_TYPE* indexArray = faceRenderData->GetIndexBegin();
				//					GLShapeDrawer::DrawPntWithIndex(positionArray,
				//							normalArray, indexArray,
				//							dataLength/simplifyFactor,stride);
				//				}
				//				else
				//				{
				////					LOGE("simplifyFactor %d",simplifyFactor);
				//					GLShapeDrawer::DrawPntNoIndex(positionArray,
				//							normalArray, dataLength/simplifyFactor,stride);
				//				}
				//			}
				//			else
				{
					//根据颜色绘制
					//if (isUseIndex)
					//{
					//	M3D_INDEX_TYPE* indexArray =
					//		(M3D_INDEX_TYPE*)faceRenderData->GetIndexOffset();
					//	GLShapeDrawer::DrawTriWithIndex(positionArray,
					//		normalArray, indexArray, dataLength);
					//}
					//else
					//{
					//	GLShapeDrawer::DrawTriNoIndex(positionArray,
					//			normalArray, dataLength);
					//}
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					if (isUseIndex)
					{
						M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
						GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
					}
					else
					{
						GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
					}
				}

			}
		}
		glPopMatrix();
		vertexBuffer->UnBind();
	}
	//CloseCubeMapping(action);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

//	effect->EndEffect();

	glPolygonOffset(0.0, 0.0);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}


/*void GLShapeDrawer::DrawSolidRenderPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0){
		return;
	}

	ClearGLState();
	RenderContext* gl = action->GetGLContext();

	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(PolygonOffset, PolygonOffset);

//	OpenCubeMapping(action);

	RenderTech* effect = RenderStateArray->GetRenderTech();
	effect->BeginEffect();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY); //fanhuan
	//glShadeModel(GL_FLAT);
	Matrix3 rot_=action->GetCamera()->GetView().ToMatrix3();//fanhuan
	//Vector3 eye =action->GetCamera()->GetPosition();

	//GLfloat light_specular[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
	//static int tmp = 1;
	std::string str_diamond = "/storage/emulated/0/hoteamsoft/sview/sample/diam.bmp";
	std::string str_ring = "/storage/emulated/0/hoteamsoft/sview/sample/GoldShny.bmp";
	RenderabelArray::iterator it = RenderStateArray->GetRenderableArray().begin();
	for (; it != RenderStateArray->GetRenderableArray().end(); it++)
	{
		Renderable* faceRenderData = *it;
		int dataLength = faceRenderData->GetDataLength();
		int vertexNumber = faceRenderData->GetUseIndexVertexNumber();
		if(dataLength<=0){
		//	LOGI("4444444444444444444444444444444");
			continue;
		}
		bool isUseIndex = faceRenderData->IsUseIndex();

		glPushMatrix();
		gl->MultMatrix(*(*it)->GetRenderWorldMatrix());

		Vector3* positionArray = faceRenderData->GetVertexPos();
		Vector3* normalArray = faceRenderData->GetVertexNormal();
		Material* material = faceRenderData->GetRenderMaterial();//获取材质
		//Color  color = faceRenderData->GetRenderColor();
		//color.m_a = 0.3f;
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color.Data());
		//int  verLen = faceRenderData->GetDataLength();
		//LOGI("长度：%d",verLen);
		//color.m_a =(tmp>0) ? 0.6f : 1.0f;

	//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color.Data());
		bool bRenderTex = true;

		if(!material||!material->GetTexture()||!material->GetTexture()->GetOGLObj()){
			bRenderTex = false;
		}

		if(bRenderTex){
			unsigned int OGLObj = material->GetTexture()->GetOGLObj();
			std::string name = material->GetTexture()->GetName();
		//	LOGI("!!!!!!!!=========%s",name.c_str());
			if(name.compare(str_diamond) && name.compare(str_ring)){   //非ring模型 照旧
			//	LOGI("00000000000000000000000000000000000000");
				glShadeModel(GL_SMOOTH);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,faceRenderData->GetRenderColor().Data());

				Vector3* textureCoordsArray = faceRenderData->GetTextureCoords();

				if (isUseIndex){
					M3D_INDEX_TYPE* indexArray =faceRenderData->GetIndex();
					DrawTexture2DWithIndex((GLuint)OGLObj, positionArray,normalArray,
							textureCoordsArray, indexArray, dataLength);
				}else{
					DrawTexture2DNoIndex((GLuint)OGLObj, positionArray,
											normalArray,textureCoordsArray, dataLength);
				}
			}
			else if(!name.compare(str_diamond)){  //ring 模型的钻石部分
			//	LOGI("111111111111111111111111111111111111111111");
				glShadeModel(GL_FLAT);
				Color  color = faceRenderData->GetRenderColor();
				color.m_a = 0.6;
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,color.Data());


				if(isUseIndex) {
					M3D_INDEX_TYPE* indexArray =faceRenderData->GetIndex();
					int len = dataLength;
					Vector3 position[len];
					Vector3 normal[len];
					Vector3 textureCoord[len];

					for(int i = 0 ;i < len ;i +=3){
						position[i] = positionArray[indexArray[i]];
						position[i+1] = positionArray[indexArray[i+1]];
						position[i+2] = positionArray[indexArray[i+2]];

						Vector3 a = position[i] - position[i+2];
						Vector3 b = position[i+1] - position[i+2];
						Vector3 n = a.CrossProduct(-b);
						normal[i]=normal[i+1]=normal[i+2]=n;
						Vector3 uv =  rot_ * n;
						textureCoord[i].m_x = textureCoord[i+1].m_x = textureCoord[i+2].m_x = (uv.m_x / 2.0 + 0.5);
						textureCoord[i].m_y = textureCoord[i+1].m_y = textureCoord[i+2].m_y = (uv.m_y / 2.0 + 0.5);

					}

					DrawTexture2DNoIndex((GLuint)OGLObj, position,normal,textureCoord, len);
				}
				else{LOGI("钻石顶点无索引！！！！！！！！！！");

				}
			}
			else if(!name.compare(str_ring)){  //指环部分
			//	LOGI("2222222222222222222222222222222222222");
				glShadeModel(GL_SMOOTH);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,faceRenderData->GetRenderColor().Data());

				Vector3 textureCoordsArray[vertexNumber];
				for(int i = 0 ;i < vertexNumber ;i ++){

					Vector3 uv = rot_ * normalArray[i];
					textureCoordsArray[i].m_x = (uv.m_x / 2.0 + 0.5);
					textureCoordsArray[i].m_y = (uv.m_y / 2.0 + 0.5);
				}
				if(isUseIndex) {
					M3D_INDEX_TYPE* indexArray =faceRenderData->GetIndex();
					DrawTexture2DWithIndex((GLuint)OGLObj, positionArray,normalArray,
							textureCoordsArray, indexArray, dataLength);
				}
				else{
					DrawTexture2DNoIndex((GLuint)OGLObj, positionArray,
										normalArray,textureCoordsArray, dataLength);
				}
			}

		}
		else{
		//	LOGI("333333333333333333333333333333");
			//const Color& color=faceRenderData->GetRenderColor();
			//glColor4f(color.m_r,color.m_g,color.m_b,color.m_a);
			glShadeModel(GL_SMOOTH);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,faceRenderData->GetRenderColor().Data());

			if (isUseIndex){
				M3D_INDEX_TYPE* indexArray = faceRenderData->GetIndex();
				DrawTriWithIndex(positionArray,normalArray, indexArray, dataLength);
			}
			else{
				DrawTriNoIndex(positionArray,normalArray, dataLength);//GLShapeDrawer
			}
		}
		glPopMatrix();

	}
//	CloseCubeMapping(action);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	effect->EndEffect();

	glPolygonOffset(0.0, 0.0);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisable(GL_COLOR_MATERIAL);//fanhuan
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);



}*/ //TODO fan huan's code

void GLShapeDrawer::OpenCubeMapping(RenderAction* action,string name)
{
#ifdef __ANDROID__
	if(!action || Parameters::Instance()->m_LightingMode != 200){
		return;
	}

	//Texture* texture = ResourceManager::GetInstance()->cubeMappingTexture;//这个是加载一个纹理的

	Texture* texture = action->GetScene()->GetResourceManager()->GetCubeMappingTexture(name);

	if (!texture || (!texture->GetOGLObj() > 0))
	{
		return;
	}
	glEnable(GL_TEXTURE_CUBE_MAP_OES);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_CUBE_MAP_OES, texture->GetOGLObj());
	//glBindTexture(GL_TEXTURE_2D, texture->GetOGLObj());
	glTexGeniOES(GL_TEXTURE_GEN_STR_OES, GL_TEXTURE_GEN_MODE_OES,
			GL_REFLECTION_MAP_OES);
	glEnable(GL_TEXTURE_GEN_STR_OES);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//glMatrixMode(GL_TEXTURE);
	glPushMatrix();

//	Matrix3x4& viewMatrix = action->GetCamera()->GetView();
//
//	Matrix3x4 viewInverse = viewMatrix.Inverse();
//
//	glMultMatrixf(viewInverse.ToMatrix4().Transpose().Data());
//
	glMatrixMode(GL_MODELVIEW);
#endif
}

void GLShapeDrawer::CloseCubeMapping(RenderAction* action )
{
#ifdef __ANDROID__

	glDisable(GL_TEXTURE_CUBE_MAP_OES);
	//glDisable(GL_TEXTURE_2D);
//	glMatrixMode(GL_TEXTURE);
	glDisable(GL_TEXTURE_GEN_STR_OES);
	glBindTexture(GL_TEXTURE_CUBE_MAP_OES, 0);
	if(!action || Parameters::Instance()->m_LightingMode != 200){
		return;
	}
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
#endif
}

void GLShapeDrawer::DrawPointPassGroup(RenderAction* action,
		RenderQueue* RenderStateArray)
{
	if (RenderStateArray->GetRenderableArray().size() == 0)
		{
			return;
		}

		vector<Point*> frontShowPoint;

		RenderContext* gl = action->GetGLContext();

		ClearGLState();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glEnableClientState(GL_VERTEX_ARRAY);

		RenderabelArray::iterator it =
				RenderStateArray->GetRenderableArray().begin();
		for (; it != RenderStateArray->GetRenderableArray().end(); it++)
		{
			glPushMatrix();

			Point* point = (Point*) (*it);

			if(!point->IsFrontShow())
			{
				DrawPoint(point);
			}else //找到需要最前端显示的，最后统一绘制
			{
				frontShowPoint.push_back(point);
			}
			glPopMatrix();
		}

		//绘制最前端显示的点
		if(frontShowPoint.size()>0)
		{
			for(int i=0;i<frontShowPoint.size();i++)
			{
				glPushMatrix();
				Point* point = frontShowPoint.at(i);
				DrawPoint(point);
				glPopMatrix();
			}
		}
}

void GLShapeDrawer::DrawTriWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		M3D_INDEX_TYPE * indexBuffer, int num)
{
	glVertexPointer(3, GL_FLOAT, 0, pointsBuffer);
	glNormalPointer(GL_FLOAT, 0, normalBuffer);
	glDrawElements(GL_TRIANGLES, num, M3D_GL_INDEX_TYPE, indexBuffer);
}

void GLShapeDrawer::DrawTriWithIndex(HardWareVertexBuffer * vertexBuffer, HardWareIndexBuffer * indexBuffer, int dataLength, M3D_OFFSET_TYPE indexOffset)
{
	char* indexAddress = (char*)indexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *)(indexAddress + indexOffset));
	indexBuffer->UnBind();
}

void GLShapeDrawer::DrawTriNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		int num)
{
	glVertexPointer(3, GL_FLOAT, 0, pointsBuffer);
	glNormalPointer(GL_FLOAT, 0, normalBuffer);
	glDrawArrays(GL_TRIANGLES, 0, num);
}

void GLShapeDrawer::DrawTriNoIndex(HardWareVertexBuffer * vertexBuffer, int dataLength)
{
	glDrawArrays(GL_TRIANGLES, 0, dataLength);
}

void GLShapeDrawer::DrawPntWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		M3D_INDEX_TYPE* indexBuffer, int num,int stride)
{
	glVertexPointer(3, GL_FLOAT, stride, pointsBuffer);
	glNormalPointer(GL_FLOAT, stride, normalBuffer);
	glDrawElements(GL_POINTS, num, M3D_GL_INDEX_TYPE, indexBuffer);
}

void GLShapeDrawer::DrawPntNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		int num,int stride)
{
	glVertexPointer(3, GL_FLOAT, stride, pointsBuffer);
	glNormalPointer(GL_FLOAT, stride, normalBuffer);
	glDrawArrays(GL_POINTS, 0, num);
}

void GLShapeDrawer::DrawTriLinesWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		M3D_INDEX_TYPE* indexBuffer, int num)
{
	glVertexPointer(3, GL_FLOAT, 0, pointsBuffer);
	glDrawElements(GL_LINES, num, M3D_GL_INDEX_TYPE, indexBuffer);
}

void GLShapeDrawer::DrawTexture2DWithIndex(GLuint OGLObj,
		Vector3* pointsBuffer,
		Vector3* normalsBuffer,
		Vector3* textureCoordsBuffer,
		M3D_INDEX_TYPE* indexBuffer, int num,Matrix4* texture2DTransform)
{
	if(OGLObj)
	{
		//设置混合颜色
//		glColor4f(1.0f,1.0f,1.0f,1.0f);
//		glEnable(GL_BLEND);
		// 启用纹理
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, OGLObj);

//		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if(texture2DTransform)
		{
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glMultMatrixf(texture2DTransform->Data());
		}

		glNormalPointer(GL_FLOAT, 0, normalsBuffer);
		glVertexPointer(3, GL_FLOAT, 0,pointsBuffer);

		//由于纹理坐标采用的Vector3存储，因此将stride设置为12

		//LOGI("textureCoordsBuffer not emperty");
		glTexCoordPointer(2, GL_FLOAT, 12, textureCoordsBuffer);
		glDrawElements(GL_TRIANGLES, num, M3D_GL_INDEX_TYPE, indexBuffer);

		//		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if(texture2DTransform)
		{
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);
	}
}

void GLShapeDrawer::DrawTexture2DWithIndex(GLuint OGLObj, HardWareVertexBuffer * vertexBuffer, HardWareIndexBuffer * indexBuffer, Matrix4 * texture2DTransform, int dataLength, M3D_OFFSET_TYPE indexOffset)
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
		glDrawElements(GL_TRIANGLES, dataLength, M3D_GL_INDEX_TYPE, (GLvoid *)(indexOffset));
		indexBuffer->UnBind();
		vertexBuffer->UnBind();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);

		//		glDisable(GL_BLEND);

	}
}

void GLShapeDrawer::DrawTexture2DNoIndex(GLuint OGLObj,
		Vector3* pointsBuffer,
		Vector3* normalsBuffer,
		Vector3* textureCoordsBuffer,
		int num,Matrix4* texture2DTransform)
{
	if(OGLObj)
	{
		//设置混合颜色
//		glColor4f(1.0f,1.0f,1.0f,1.0f);
//		glEnable(GL_BLEND);
		// 启用纹理
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, OGLObj);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glNormalPointer(GL_FLOAT, 0, normalsBuffer);
		glVertexPointer(3, GL_FLOAT, 0,pointsBuffer);

		if(texture2DTransform)
		{
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glMultMatrixf(texture2DTransform->Data());
		}

		//由于纹理坐标采用的Vector3存储，因此将stride设置为1
		glTexCoordPointer(2, GL_FLOAT, 12, textureCoordsBuffer);
		glDrawArrays(GL_TRIANGLES, 0, num);

		if(texture2DTransform)
		{
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void GLShapeDrawer::DrawTexture2DNoIndex(GLuint OGLObj, HardWareVertexBuffer * vertexBuffer, Matrix4 * texture2DTransform, int dataLength)
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

void GLShapeDrawer::DrawTriLinesNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
		int num)
{
	glVertexPointer(3, GL_FLOAT, 0, pointsBuffer);
	glDrawArrays(GL_LINES, 0, num);
}

void GLShapeDrawer::DrawPMINew(PMIData* pmiData, RenderAction* action)
{
	//LOGI("begin DrawPMI");
	if (pmiData->IsVisible() == false)
	{
		return;
	}
	RenderContext* gl = action->GetGLContext();
	glPushMatrix();

	Color* tmpColor = pmiData->GetDrawColor();
	glColor4f(tmpColor->m_r, tmpColor->m_g, tmpColor->m_b, tmpColor->m_a);

	glEnableClientState(GL_VERTEX_ARRAY);
	//绘制线
	for (int i = 0; i < pmiData->m_Lines.size(); i++)
	{
		PolyLine* line = pmiData->m_Lines[i];
		int vertexNum = line->GetPointList().size();
		int indexNum = 0;
		float* vertexBuf = (float*)line->GetPointList().data();
		M3D_INDEX_TYPE* indexBuf;
		if (indexNum > 0) //索引数为零则使用顶点绘制
		{
			glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE,
					indexBuf);
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
			glDrawArrays(GL_LINE_STRIP, 0, vertexNum);
		}
	}
	//绘制外框和文字
	if (pmiData->m_IsParallelScreen)//&& pmiData->m_Type == 8000)
	{
		glMultMatrixf(pmiData->GetOutFrameMatrix()->Data());
	}

	if(pmiData->m_IsParallelScreen)
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
		Vector3* positionArray = textMesh->GetRefMesh()->GetPositionArray()->data()
				+ textMesh->GetDataOffset();
		if (isUseIndex)
		{
			M3D_INDEX_TYPE* indexArray = textMesh->GetRefMesh()->GetIndexArray()->data();
			GLShapeDrawer::DrawTriWithIndex(positionArray,
					NULL, indexArray,
					dataLength);
		}
		else
		{
			GLShapeDrawer::DrawTriNoIndex(positionArray,
					NULL, dataLength);
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void GLShapeDrawer::DrawNote(Note* pNote, RenderAction* action)
{
	//绘制包围盒
	if (Parameters::Instance()->m_IsShowBox)
		DrawShapeBox(pNote->GetBoundingBox(), false);

	glPushMatrix();
	glDisable(GL_LIGHTING);
	if(pNote->IsFrontShow())
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
   // glEnable(GL_DEPTH_TEST);


	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

//	//根据材质属性，设置显示状态
	Color* tmpColor = pNote->GetDrawColor();
	glColor4f(tmpColor->m_r, tmpColor->m_g, tmpColor->m_b, tmpColor->m_a);

	//lines
//	LOGI("GLShapeDrawer::DrawNote lines:%d",pNote->m_LineList.size());

	glLineWidth(4);
	for (int i = 0; i < pNote->m_LineList.size(); i++)
	{
		Line3D* line = pNote->m_LineList[i];
		Color* tempColor = line->GetDrawColor();
		if(line->GetName() == "exLine")
		{
			glLineWidth(2);
		}
		glColor4f(tempColor->m_r, tempColor->m_g, tempColor->m_b, tempColor->m_a);
		int vertexNum = 2;
		int indexNum = 2;
		float vertexBuf[6] =
		{ line->m_StartPoint.m_x, line->m_StartPoint.m_y, line->m_StartPoint.m_z,
				line->m_EndPoint.m_x, line->m_EndPoint.m_y, line->m_EndPoint.m_z, };
		M3D_INDEX_TYPE indexBuf[2] =
		{ 0, 1 };
		//line->getBuffer(&vertexNum, &vertexBuf, &indexNum, &indexBuf);
		glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
		glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE, indexBuf);

		//draw Arrows
		vector<vector<float> > startArrowBufList = line->GetStartArrowBuffer();
		for (int i = 0; i < startArrowBufList.size(); i++)
		{
			vector<float>* arrowBuf = &startArrowBufList.at(i);
			glVertexPointer(3, GL_FLOAT, 0, arrowBuf->data());
			glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
		}

		vector<vector<float> > endArrowBufList = line->GetEndArrowBuffer();
		for (int i = 0; i < endArrowBufList.size(); i++)
		{
			vector<float>* arrowBuf = &endArrowBufList.at(i);
			glVertexPointer(3, GL_FLOAT, 0, arrowBuf->data());
			glDrawArrays(GL_LINE_STRIP, 0, arrowBuf->size() / 3);
		}

		if(line->GetName() == "exLine")
		{
			glLineWidth(4);
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
		if (indexNum > 0) //索引数为零则使用顶点绘制
		{
			glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
			glDrawElements(GL_LINE_STRIP, indexNum, M3D_GL_INDEX_TYPE,
					indexBuf);
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, 0, vertexBuf);
			glDrawArrays(GL_LINE_STRIP, 0, vertexNum);
		}
	}

	int pointSize = pNote->m_PointList.size();
	if (pointSize > 0)
	{
		for (int i = 0; i < pNote->m_PointList.size(); i++)
		{
			glPushMatrix();
			Point* point = pNote->m_PointList[i];

			DrawPoint(point);

			glPopMatrix();
		}
	}

	//绘制图片
	int imageSize = pNote->m_imageBoardList.size();
	if (imageSize > 0)
	{
		for (int i = 0; i < pNote->m_imageBoardList.size(); i++)
		{
			ImageBoard* imageBoard = pNote->m_imageBoardList[i];
			DrawImageBoard(imageBoard);
		}
	}
	glPushMatrix();
 
	if(imageSize == 0)
	{
        //使用公告板技术，来进行显示效果调整
        Billboard* billboard = pNote->GetBillBoard();
        
        if(billboard)
        {
            glMultMatrixf(billboard->GetGLWorldMatrix().Data());
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
					}
				}
			}
	}

	glPopMatrix();


	glPopMatrix();
}

void GLShapeDrawer::DrawImageBoard(ImageBoard* imageboard)
{
	if(!imageboard)
	{
		return;
	}

//	LOGE("GLShapeDrawer::DrawImageBoard 11");

	Texture* image = imageboard->GetTexture();
	if(!image)
	{
		return;
	}
//	LOGE("GLShapeDrawer::DrawImageBoard 22");
	ClearGLState();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();

	int textGLObj = image->GetOGLObj();
	if(textGLObj > 0)
	{
		glMultMatrixf(imageboard->GetRenderWorldMatrix()->Data());

		const Color& VoiceBackColor = imageboard->GetRenderColor();
		glColor4f(VoiceBackColor.m_r, VoiceBackColor.m_g, VoiceBackColor.m_b, VoiceBackColor.m_a);

		// 启用纹理
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D,textGLObj);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0,
				imageboard->GetVertexs()->Data());
		glTexCoordPointer(2, GL_FLOAT, 0, imageboard->GetTextureCoords()->Data());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		LOGI("GLShapeDrawer::DrawImageBoard error !");
	}

	glPopMatrix();

}

void GLShapeDrawer::DrawMeshBoard(MeshBoard* meshboard)
{
	if(!meshboard)
	{
		return;
	}

	glMultMatrixf(meshboard->GetRenderWorldMatrix()->Data());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0,meshboard->GetLinesVertexs()->Data());
	glDrawArrays(GL_LINES, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void GLShapeDrawer::DrawPointHandler(HandlerPoint* handlerPoint)
{
	if(!handlerPoint && (!handlerPoint->IsVisible()))
	{
		return;
	}

	glPushMatrix();
	Point* point = handlerPoint->GetPoint();

	DrawPoint(point);

	glPopMatrix();
}

void GLShapeDrawer::DrawMeshHandler(Handler* handler)
{
	if (!handler)
	{
		return;
	}

	glPushMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	int dataLength = handler->GetDataLength();
	bool isUseIndex = handler->IsUseIndex();
	int vertexNum = handler->GetUseIndexVertexNumber();

	if (dataLength > 0)
	{
		const float* color = handler->GetRenderColor().Data();
		glColor4f(color[0], color[1], color[2], color[3]);

		Vector3* positionArray = handler->GetVertexPos();

		if (isUseIndex)
		{
			M3D_INDEX_TYPE* indexArray = handler->GetIndex();
			GLShapeDrawer::DrawTriWithIndex(positionArray, NULL, indexArray,
					dataLength);
		}
		else
		{
			GLShapeDrawer::DrawTriNoIndex(positionArray, NULL, dataLength);
		}
	}

	glPopMatrix();
}

void  GLShapeDrawer::DrawPoint(Point* point )
{
	glMultMatrixf(point->GetRenderWorldMatrix()->Data());

	const Color& pntColor = point->GetRenderColor();
	glColor4f(pntColor.m_r, pntColor.m_g, pntColor.m_b, pntColor.m_a);

	int drawType = point->GetDrawType();
	if(drawType == 1 || drawType == 2 || drawType == 3)
	{
		DrawImageBoard(point->GetImageboard());
	}
	else if(drawType == 0)
	{
		DrawMeshBoard(point->GetMeshBoard());
	}
}

bool  GLShapeDrawer::ComputerSphereTexCoords(unsigned int OGLObj,BaseMaterial* material,Renderable* faceRenderData,Vector3 * texArray,const M3D_INDEX_TYPE*indexArray,
		const Vector3 * normalArray,const Vector3 * positionArray,
		Matrix3 & rotateMatrix,int dataLength)
{
	string name = "";
	Material* tempMaterial = dynamic_cast<Material*>(material);
	name = tempMaterial->GetDiffuseMap()->GetName();

	int pos = name.find_last_of("\\/");
	if(pos != string::npos)
	{
		name = name.substr(pos+1);
		//LOGI("name === %s",name.c_str());
	}
	if(name == "GoldShny.bmp")
	{
		int i = 0;
		for (i = 0; i < dataLength; i++)
		{
			Vector3 uv = rotateMatrix * normalArray[i];
			uv.Normalize();

			texArray[i].m_x = (uv.m_x / 2.0 + 0.5);
			texArray[i].m_y = -(uv.m_y / 2.0 + 0.5);
			texArray[i].m_z = 0.0f;

		}
	}
	else if(name == "sdiam.bmp")
	{
		int indexLength = faceRenderData->GetDataLength();

		vector<Vector3> position;
		position.reserve(indexLength);

		vector<Vector3> normal;
		normal.reserve(indexLength);

		vector<Vector3> textureCoord;
		textureCoord.reserve(indexLength);

		for(int i = 0 ;i < indexLength ;i +=3){
			position[i] = positionArray[indexArray[i]];
			position[i+1] = positionArray[indexArray[i+1]];
			position[i+2] = positionArray[indexArray[i+2]];

			Vector3 a = position[i] - position[i+2];
			Vector3 b = position[i+1] - position[i+2];
			Vector3 n = a.CrossProduct(-b);
			n.Normalize();
			normal[i]=normal[i+1]=normal[i+2]=n;
			Matrix3x4 trans;


			trans.SetRotation(rotateMatrix);
			trans.MultiTranslate(position[i]);

			//trans = trans*rotateMatrix;

			Vector3 uv =  rotateMatrix * n;
//			textureCoord[i].m_x = textureCoord[i+1].m_x = textureCoord[i+2].m_x = (uv.m_x / 2.0 + 0.5);
//			textureCoord[i].m_y = textureCoord[i+1].m_y = textureCoord[i+2].m_y = (uv.m_y / 2.0 + 0.5);
			Vector3 n1 = normalArray[indexArray[i]];
			Vector3 uv1 =  trans * n1;
			textureCoord[i].m_x =(uv1.m_x / 2.0 + 0.5);
			textureCoord[i].m_y =-(uv1.m_y / 2.0 + 0.5);

			Vector3 n2 = normalArray[indexArray[i+1]];
			Vector3 uv2 =  trans * n2;
			textureCoord[i+1].m_x =(uv2.m_x / 2.0 + 0.5);
			textureCoord[i+1].m_y =-(uv2.m_y / 2.0 + 0.5);

			Vector3 n3 = normalArray[indexArray[i+2]];
			Vector3 uv3 =  trans * n3;
			textureCoord[i+2].m_x =(uv3.m_x / 2.0 + 0.5);
			textureCoord[i+2].m_y =-(uv3.m_y / 2.0 + 0.5);
		}
		DrawTexture2DNoIndex((GLuint)OGLObj, position.data(),normal.data(),
				textureCoord.data(), indexLength,NULL);

		return true;

	}
	else
	{
		int i = 0;
	//	LOGI("dataLength = %d",dataLength);
		for ( i = 0; i < dataLength; i++)
		{
			Vector3 uv = rotateMatrix * normalArray[i];
			uv.Normalize();

			texArray[i].m_x =
					(uv.m_x / 2.0 + 0.5);
			texArray[i].m_y =
					-(uv.m_y / 2.0 + 0.5);
			texArray[i].m_z =
					0.0f;
		}
	}

	return false;
}

void GLShapeDrawer::DrawSampleModelPassGroug(RenderAction* action)
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

	ClearGLState();
	RenderContext* gl = action->GetGLContext();
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);

	for (int i = 0; i < tempRenderQueues.size(); i++)
	{
		if (tempRenderQueues[i]->GetRenderableArray().size() == 0)
		{
			continue;
		}
		RenderabelArray::iterator it = tempRenderQueues[i]->GetRenderableArray().begin();
		for (; it != tempRenderQueues[i]->GetRenderableArray().end(); it++)
		{
            Renderable* faceRenderData = *it;
            
            int dataLength = faceRenderData->GetDataLength();
            bool isUseIndex = faceRenderData->IsUseIndex();
            int vertexNum = faceRenderData->GetUseIndexVertexNumber();
            
            HardWareVertexBuffer* vertexBuffer = faceRenderData->GetHardWareVertexBuffer();
            HardWareIndexBuffer* indexBuffer = faceRenderData->GetHardWareIndexBuffer();
            char* vertexAddress = (char*)vertexBuffer->Bind();
            
            glPushMatrix();
            glMultMatrixf((*it)->GetRenderWorldMatrix()->Data());
            
            if (dataLength > 0)
            {
                
                M3D_OFFSET_TYPE veroffset = faceRenderData->GetVertexOffset();
                M3D_OFFSET_TYPE normaloffset = faceRenderData->GetNormalOffset();
                M3D_OFFSET_TYPE texoffset = faceRenderData->GetTextureCoordsOffset();
                
                glVertexPointer(3, GL_FLOAT, 0, (GLvoid *)(vertexAddress + veroffset));
                glNormalPointer( GL_FLOAT, 0, (GLvoid *)(vertexAddress + normaloffset));
                
                BaseMaterial* material = faceRenderData->GetRenderMaterial();//获取材质
                
                //如果材质存在，则使用材质绘制
                
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                             (*it)->GetRenderColor().Data());
                
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                if (isUseIndex)
                {
                    M3D_OFFSET_TYPE indexArray = faceRenderData->GetIndexOffset();
                    GLShapeDrawer::DrawTriWithIndex(vertexBuffer, indexBuffer, dataLength, indexArray);
                }
                else
                {
                    GLShapeDrawer::DrawTriNoIndex(vertexBuffer, dataLength);
                }
                
            }
            glPopMatrix();
            vertexBuffer->UnBind();
		}
        glDisableClientState(GL_VERTEX_ARRAY);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
	}

}

 void GLShapeDrawer::DrawCapPolygon(RenderAction* action)
{
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
			ClearGLState();
			CameraNode* camera = action->GetCamera();
			if (!camera)
			{
				return;
			}
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glLineWidth(1);
			glDisable(GL_LIGHTING);
			glEnableClientState(GL_VERTEX_ARRAY);

			glColor4f(0.6,0.3,0.0,1.0);
			glVertexPointer(3, GL_FLOAT, 0, sectionPlane->GetPointArray());
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}
}

 void GLShapeDrawer::DrawSectionLines(RenderAction* action,vector<Vector3>& data)
{
	int size = data.size();
	if ((size == 0))
	{
		//		LOGI("data is NULL");
		return;
	}
	glLineWidth(4);
	if (Parameters::Instance()->m_clipPlaneMode == 1)
	{
		glLineWidth(10);
	}
	ClearGLState();
	CameraNode* camera = action->GetCamera();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0,1.0,0.0,1.0);
	glVertexPointer(3, GL_FLOAT, 0, (void*)(data.at(0).Data()));
	glDrawArrays(GL_LINES, 0, data.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

}
