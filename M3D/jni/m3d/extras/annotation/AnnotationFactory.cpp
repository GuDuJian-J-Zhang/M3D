#include "AnnotationFactory.h"
#include "m3d/extras/annotation/Annotation.h"
#include "m3d/extras/annotation/AnnotationDisplayHelper.h"
#include "m3d/model/Shape.h"
#include "m3d/model/Point.h"
#include "m3d/model/PolyLine.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/handler/HandlerPoint.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Plane.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/extras/annotation/AnnotationGroup.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/action/RenderAction.h"
#include "m3d/base/Viewport.h"
#include "m3d/base/Rect.h"
#include "m3d/utils/ShapeHelper.h"

namespace M3D
{
	AnnotationFactory::AnnotationFactory()
	{

	}
	AnnotationFactory::~AnnotationFactory()
	{

	}

	Annotation* AnnotationFactory::CreateAnnotation(int firstShapId,
		Vector2& firstPnt, Vector2& secondPnt,
		const string& text, Color& frameColor, Color& fillColor, 
		bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed, 
		SceneManager* scene)
	{
		Annotation* note = NULL;
		note = CreatePntAnnotation(firstShapId,firstPnt, secondPnt,
			text, frameColor, fillColor, 
			bLenderLine, bEnvelope, bStub, bFixed, scene);

		if (note)
		{
			AddNoteToScene(scene, note);
			//LOGI("CreateTextNote 1");
		}
		return note;
	}

	bool AnnotationFactory::CreateAnnotation(Annotation* annotation, const string& text,
		Color& frameColor, Color& fillColor,
		bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed,
		SceneManager* scene)
	{
		bool ret = false;
		ret = CreatePntAnnotation(annotation, text, frameColor, fillColor, bLenderLine, bEnvelope, bStub, bFixed, scene);
		if (annotation && ret)
		{
			AddNoteToScene(scene, annotation);
		}
		return ret;
	}
	bool AnnotationFactory::CreateAnnotation(Annotation* annotation, const string& text,
		Color& frameColor, Color& fillColor,
		SceneManager* scene)
	{
		bool ret = false;
		ret = CreatePntAnnotation(annotation, text, frameColor, fillColor, scene);
		return ret;
	}


	Annotation* AnnotationFactory::CreatePntAnnotation(int firstShapId,
		Vector2& firstPnt, Vector2& secondPnt,
		const string& text, Color& frameColor, Color& fillColor, 
		bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed, 
		SceneManager* scene)
	{
		Annotation *annotation = new Annotation();
		annotation->SetText(text);

		Vector3 fCoordiante;
		Vector3 SCoordiante;
		//Vector3 spntInPlane;

		Vector3 TCoordiante;
		//Vector3 tpntInPlane;

		Vector3 textInpane;
		Shape* firstShape = (Shape*)scene->GetShape(firstShapId);
		if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
		{
			
			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;

			fCoordiante = firstPnt->GetPosition();
			annotation->SetAnnotationPos(fCoordiante);
			annotation->SetTextsPos(fCoordiante);
			textInpane = fCoordiante;
		}
		else
		{
			if (scene->GetPickPoint(firstPnt, fCoordiante, false))
			{
				annotation->SetAnnotationPos(fCoordiante);
				annotation->SetTextsPos(fCoordiante);
				textInpane = fCoordiante;
			}
		}

		if (bLenderLine)
		{
			if (scene->GetPickPoint(secondPnt, SCoordiante, false))
			{
				annotation->SetCenterPos(SCoordiante);
				annotation->SetTextsPos(SCoordiante);
				annotation->SetHaveLeaderLine(true);
				textInpane = SCoordiante;
			}
		}

		if (bStub)
		{
			annotation->SetHaveStub(true);
			//if (scene->GetPickPoint(thirdPnt, TCoordiante, false))
			//{
			//	annotation->SetTextsPos(TCoordiante);
			//	annotation->SetHaveStub(true);
			//	textInpane = TCoordiante;
			//}
		}
		
		if (bLenderLine)
		{
			Color lineColor = Color(0.03, 0.38, 0.73);
			Line3D* line1 = new Line3D(fCoordiante, SCoordiante);
			line1->SetColor(lineColor);
			line1->SetName("LenderLine");
			annotation->AddLine(line1);
			//if (bStub)
			//{
			//	Line3D* line2 = new Line3D(SCoordiante, TCoordiante);
			//	line2->SetColor(lineColor);
			//	line2->SetName("parallelLines");
			//	annotation->AddLine(line2);
			//}
		}
			
			vector<Texts2D*> temptext;
			Texts2D *content = new Texts2D;
			content->m_size = 12.0f;
			content->m_texts = text;
			temptext.push_back(content);

			string note;
			float fontSize;
			Vector2 rectStart, rectEnd;
			int rows = 0;

			AnnotationDisplayHelper::GetAnnotationTextRectInfo(scene, temptext, note, fontSize, rectStart, rectEnd, rows);
			float tmpLength = (rectEnd.m_x - rectStart.m_x) / 4.0;
			annotation->SetPosAverage(tmpLength);
			annotation->SetFillColor(fillColor);
			annotation->SetFrameColor(frameColor);

			ImageBoard * imageBroad = NULL;
			if (bEnvelope)
			{
				annotation->SetHaveEnvelope(true);
				imageBroad = AnnotationDisplayHelper::createAnnotationImage(scene, note, fontSize, rows, rectStart, rectEnd, frameColor, fillColor, textInpane);
			}
			else
			{
				imageBroad = AnnotationDisplayHelper::createAnnotation(scene, note, fontSize, rows, rectStart, rectEnd, textInpane);
					//MeasureDisplayHelper::createAnnotation(scene, temptext, textInpane);
			}

			//释放内存
			for (int i = 0; i < temptext.size(); i++)
			{
				delete temptext[i];
				temptext[i] = NULL;
			}
			
			annotation->AddImage(imageBroad);
			
			ComText* ct = new ComText();
			CText* t = new CText;

			t->SetText(text);
			ct->AddCText(t);
			annotation->m_ComTexts.push_back(ct);
			annotation->SetFixed(bFixed);
			annotation->SetFrontShow(false); //是否前端显示
		

		return annotation;
	}

	bool AnnotationFactory::CreatePntAnnotation(Annotation* annotation, const string& text,
		Color& frameColor, Color& fillColor,
		bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed,
		SceneManager* scene)
	{
		bool state = false;
		if (!annotation)
		{
			return state;
		}
		annotation->SetText(text);

		Vector3 fCoordiante, SCoordiante, textInpane;
		if (bLenderLine)
		{
			annotation->SetHaveLeaderLine(true);
			fCoordiante = annotation->GetAnnotationPos();
			SCoordiante = annotation->GetCenterPos();
			Color lineColor = Color(0.03, 0.38, 0.73);
			Line3D* line1 = new Line3D(fCoordiante, SCoordiante);
			line1->SetColor(lineColor);
			line1->SetName("LenderLine");
			annotation->AddLine(line1);
		}

		textInpane = annotation->GetAnnotationPos();

		if (bStub)
		{
			annotation->SetHaveStub(true);
		}

		vector<Texts2D*> temptext;
		Texts2D *content = new Texts2D;
		content->m_size = 12.0f;
		content->m_texts = text;
		temptext.push_back(content);

		string note;
		float fontSize;
		Vector2 rectStart, rectEnd;
		int rows = 0;

		AnnotationDisplayHelper::GetAnnotationTextRectInfo(scene, temptext, note, fontSize, rectStart, rectEnd, rows);
		float tmpLength = (rectEnd.m_x - rectStart.m_x) / 4.0;
		annotation->SetPosAverage(tmpLength);
		annotation->SetFillColor(fillColor);
		annotation->SetFrameColor(frameColor);

		ImageBoard * imageBroad = NULL;
		if (bEnvelope)
		{
			annotation->SetHaveEnvelope(true);
			imageBroad = AnnotationDisplayHelper::createAnnotationImage(scene, note, fontSize, rows, rectStart, rectEnd, frameColor, fillColor, textInpane);
		}
		else
		{
			imageBroad = AnnotationDisplayHelper::createAnnotation(scene, note, fontSize, rows, rectStart, rectEnd, textInpane);
		}

		//释放内存
		for (int i = 0; i < temptext.size(); i++)
		{
			delete temptext[i];
			temptext[i] = NULL;
		}

		annotation->AddImage(imageBroad);

		ComText* ct = new ComText();
		CText* t = new CText;

		t->SetText(text);
		ct->AddCText(t);
		annotation->m_ComTexts.push_back(ct);
		annotation->SetFixed(bFixed);
		annotation->SetFrontShow(false); //是否前端显示
		state = true;
		return state;
	}

	bool AnnotationFactory::CreatePntAnnotation(Annotation* annotation, const string& text,
		Color& frameColor, Color& fillColor,
		SceneManager* scene)
	{
		bool state = false;
		if (!annotation)
		{
			return state;
		}
		annotation->SetText(text);

		vector<Texts2D*> temptext;
		Texts2D *content = new Texts2D;
		content->m_size = 12.0f;
		content->m_texts = text;
		temptext.push_back(content);

		string note;
		float fontSize;
		Vector2 rectStart, rectEnd;
		int rows = 0;
		Vector3 textInpane = annotation->GetAnnotationPos();

		AnnotationDisplayHelper::GetAnnotationTextRectInfo(scene, temptext, note, fontSize, rectStart, rectEnd, rows);
		float tmpLength = (rectEnd.m_x - rectStart.m_x) / 4.0;
		annotation->SetPosAverage(tmpLength);
		annotation->SetFillColor(fillColor);
		annotation->SetFrameColor(frameColor);

		ImageBoard * imageBroad = NULL;
		if (annotation->GetHaveEnvelope())
		{
			imageBroad = AnnotationDisplayHelper::createAnnotationImage(scene, note, fontSize, rows, rectStart, rectEnd, frameColor, fillColor, textInpane);
		}
		else
		{
			imageBroad = AnnotationDisplayHelper::createAnnotation(scene, note, fontSize, rows, rectStart, rectEnd, textInpane);
		}

		//释放内存
		for (int i = 0; i < temptext.size(); i++)
		{
			delete temptext[i];
			temptext[i] = NULL;
		}

		vector<ImageBoard*>& imageboards = annotation->GetImageBoards();

		for (int i = 0; i < imageboards.size(); i++)
		{
			delete imageboards[i];
			imageboards[i] = NULL;
		}
		imageboards.clear();

		annotation->AddImage(imageBroad);
		note = text;
		annotation->SetTextValue(note);
		state = true;
		return state;
	}


	void AnnotationFactory::ModifyPntAnnotation(Annotation* annotation, RenderAction* renderAction)
	{
		if (annotation && !annotation->GetHaveLeaderLine())
		{
			return;
		}
		CameraNode* camera = renderAction->GetCamera();
		SceneManager* scene = renderAction->GetScene();
		IntRect rect = camera->GetViewPort().GetRect();
		int viewLength = rect.m_right;
		int viewHeight = rect.m_bottom;

		Vector3 annoPnt = annotation->GetAnnotationPos();
		Vector3 centerPnt = annotation->GetCenterPos();
		Vector3 textPnt;

		Vector2 annoAnchor = camera->WorldToScreenPoint(annoPnt);
		Vector2 centerAnchor = camera->WorldToScreenPoint(centerPnt);
		annoAnchor.m_x *= viewLength;
		annoAnchor.m_y *= viewHeight;
		centerAnchor.m_x *= viewLength;
		centerAnchor.m_y *= viewHeight;
		Vector2 endAnchor, textAnchor;

		float tmpLength = 0;

		if (annoAnchor.m_x <= centerAnchor.m_x)
		{
			if (annotation->GetHaveStub())
			{
				endAnchor = centerAnchor;
				endAnchor.m_x += 25;
				textAnchor = endAnchor;
			}
			else
			{
				textAnchor = centerAnchor;
				textAnchor.m_x += 5;
			}
		}
		else
		{
			if (annotation->GetHaveStub())
			{
				endAnchor = centerAnchor;
				endAnchor.m_x -= 25;
				textAnchor = endAnchor;
			}
			else
			{
				textAnchor = centerAnchor;
				textAnchor.m_x -= 5;
			}
		}
		if (annotation->GetHaveLeaderLine())
		{
			if (annotation->m_LineList.size() > 0)
			{
				Line3D* LeaderLine = annotation->m_LineList[0];
				LeaderLine->m_EndPoint = centerPnt;
			}
		}

		if (annotation->GetHaveStub())
		{
			Vector3 endCoor;
			endCoor = scene->GetCamera()->GetViewPort().ScreenToWorldPoint(endAnchor, 0.5);
			Line3D* StubLine = NULL;
			if (annotation->m_LineList.size() > 1)
			{
				StubLine = annotation->m_LineList[1];
				StubLine->m_StartPoint = centerPnt;
				StubLine->m_EndPoint = endCoor;
			}
			else
			{
				StubLine = new Line3D(centerPnt, endCoor);
				Color lineColor = Color(0.03, 0.38, 0.73);
				StubLine->SetColor(lineColor);
				StubLine->SetName("parallelLines");
				annotation->AddLine(StubLine);
			}
		}
		tmpLength = annotation->GetPosAverage() * 0.92;

		if (annoAnchor.m_x <= centerAnchor.m_x)
		{
			textAnchor.m_x += tmpLength;
		}
		else
		{
			textAnchor.m_x -= tmpLength;
		}

		textPnt = scene->GetCamera()->GetViewPort().ScreenToWorldPoint(textAnchor, 0.5);
		{
			annotation->SetTextsPos(textPnt);
		}
		UpdateAnnotationImagePos(annotation, textAnchor, scene);
	}

	bool  AnnotationFactory::UpdateAnnotationImagePos(Annotation* annotation, Vector2& screenPnt, SceneManager* scene)
	{
		bool ret = false;

		if (annotation != NULL && scene != NULL)
		{
			vector<ImageBoard*>& imageboards = annotation->GetImageBoards();

			for (int i = 0; i < imageboards.size(); i++)
			{
				ImageBoard* imageBoard = imageboards.at(i);

				if (imageBoard)
				{
					{
						scene->Lock();
						Vector3 newPos = imageBoard->GetPosition();

						Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
							screenPnt.m_x, screenPnt.m_y);
						Plane projPlane(cameraRay.GetDirection(), newPos);


						newPos = projPlane.Project(cameraRay.GetOrigin());//得到文本点到平面的投影点

						imageBoard->SetPosition(newPos);
						scene->UnLock();
					}
				}
			}
		}
		return ret;
	}

	bool AnnotationFactory::AddNoteToScene(SceneManager* scene, Note* note)
	{
		bool ret = false;

		//判断场景是否为空
		if (scene == NULL || note == NULL)
		{
			return ret;
		}

		//得到测量组节点，创建的测量节点加入到此组中
		AnnotationGroup* noteGroup = scene->GetAnnotationGroup();

		//创建一个节点，用来挂载创建的测量measureNote对象
		ShapeNode* node = new ShapeNode();

		node->SetShape(note);

		//给新创建的Node设置一个名字
		char strID[10];
		sprintf(strID, "%d", node->GetID());
		node->SetName(noteGroup->GetName() + "|" + strID);

		//修改场景结构，加锁
		scene->Lock();

		//将创建的测量对象加入到测量组中
		noteGroup->AddChild(node);

		scene->AddShapeIDToMap(note);

		scene->GetRenderManager()->RequestRedraw();
		//完成修改，解锁
		scene->UnLock();

		ret = true;

		return ret;
	}

	float AnnotationFactory::calculateGap(Annotation* annotation, float dragX, SceneManager* scene)
	{
		if (!annotation)
		{
			return 0.0;
		}

		Vector3 pnt;
		CameraNode* camera = scene->GetCamera();
		IntRect rect = camera->GetViewPort().GetRect();
		int viewLength = rect.m_right;

		if (annotation->GetHaveLeaderLine())
		{
			pnt = annotation->GetCenterPos();
		}
		else
		{
			pnt = annotation->GetAnnotationPos();
		}

		Vector2 anchor = camera->WorldToScreenPoint(pnt);
		anchor.m_x *= viewLength;
		float gap = dragX - anchor.m_x;
		return gap;
	}


	bool AnnotationFactory::UpdateNoteImagePos(Note* note, Vector2& screenPnt, SceneManager* scene)
	{
		bool ret = false;
		if (!note)
		{
			return ret;
		}
		Annotation * annotation = dynamic_cast<Annotation*>(note);
		if (!annotation || annotation->GetFixed())
		{
			return ret;
		}

		CameraNode* camera = scene->GetCamera();
		IntRect rect = camera->GetViewPort().GetRect();
		int viewLength = rect.m_right;
		int viewHeight = rect.m_bottom;

		Vector3 annoPnt, centerPnt;
		Vector2 annoAnchor, centerAnchor;

		if (annotation->GetHaveLeaderLine())
		{
			centerAnchor = screenPnt;
			centerAnchor.m_x -= annotation->GetGap();
			centerPnt = annotation->GetCenterPos();

			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
				centerAnchor.m_x, centerAnchor.m_y);
			Plane projPlane(cameraRay.GetDirection(), centerPnt);
			centerPnt = projPlane.Project(cameraRay.GetOrigin());//得到文本点到平面的投影点
			annotation->SetCenterPos(centerPnt);
		}
		else
		{
			annoAnchor = screenPnt;
			annoAnchor.m_x -= annotation->GetGap();
			annoPnt = annotation->GetAnnotationPos();

			Ray cameraRay = scene->GetCamera()->GetViewPort().GetScreenRay(
				annoAnchor.m_x, annoAnchor.m_y);
			Plane projPlane(cameraRay.GetDirection(), annoPnt);
			annoPnt = projPlane.Project(cameraRay.GetOrigin());//得到文本点到平面的投影点
			annotation->SetAnnotationPos(annoPnt);
			
			AnnotationFactory::UpdateAnnotationImagePos(annotation, annoAnchor, scene);
		}
		return ret;
	}

	IDTYPE AnnotationFactory::CreateTmpPoint(Vector2& screenPnt, int ShapId, SceneManager* scene)
	{
		bool ret = false;
		Vector3 coordinate;
		IShape* shape = NULL;
		IDTYPE id = 0;
		Shape* firstShape = (Shape*)scene->GetShape(ShapId);
		if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
		{
			HandlerPoint* firstPnt = (HandlerPoint*)firstShape;
			coordinate = firstPnt->GetPosition();
			ret = true;
		}
		else
		{
			ret = scene->GetPickPoint(screenPnt, coordinate, false);
		}

		shape = ShapeHelper::AddPointHandler(coordinate, 1.0f, scene);
		if (shape)
		{
			id = shape->GetID();
		}
		return id;
	}
}