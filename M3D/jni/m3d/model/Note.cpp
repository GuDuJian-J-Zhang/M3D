/*
 * Note.cpp
 *
 *  Created on: 2013-11-27
 *      Author: bing
 */
#include "m3d/model/Note.h"
#include "m3d/model/Line3D.h"
#include "m3d/model/Point.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/model/PolyLine.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/base/Vector3.h"
#include "sview/views/Parameters.h"
#include "m3d/action/RenderAction.h"
using namespace SVIEW;

namespace M3D
{
Properties::Properties()
{
    m_appendInfo.clear();
}
//int Note::MaxNoteId  = 0;
//map<int,Note*> Note::AllNoteMap;
string Properties::GetProperty(string key)
{
	string ret = "";

	map<string, string>::iterator it;
	it = m_appendInfo.find(key);
	if (it != m_appendInfo.end())
		ret = it->second;

	return ret;
}

void Properties::SetProperty(string key ,string value)
{
    m_appendInfo.insert(map<string, string>::value_type(key,value));
}

void Properties::ClearProperties()
{
	m_appendInfo.clear();
}

bool Properties::DeleteProperty(string key)
{
	bool ret;
	map<string, string>::iterator it;
	it = m_appendInfo.find(key);
	if (it != m_appendInfo.end())
	{
		m_appendInfo.erase(it);
		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool Properties::CopyProperties(Properties* properties)
{
	bool ret = false;
	if(properties)
	{
		this->m_appendInfo = properties->m_appendInfo;
		ret = true;
	}
	return ret;
}

Note::Note() :
		Shape(),Properties()
{
	this->Init();
}

///TODO
Note::~Note()
{
	this->Clear();
}

void Note::Clear()
{
	for (int i = 0; i < m_PointList.size(); i++) ///点列表
	{
		delete m_PointList[i];
	}
	m_PointList.resize(0);


	for (int i = 0; i < m_LineList.size(); i++) ///
	{
		delete m_LineList[i];
	}
	m_LineList.resize(0);

	for (int i = 0; i < m_PolyLineList.size(); i++) ///
	{
		delete m_PolyLineList[i];
	}
	m_PolyLineList.resize(0);


//	for (int i = 0; i < m_PlaneList.size(); i++) ///
//	{
//		delete m_PlaneList[i];
//	}
//	m_PlaneList.resize(0);


	for (int i = 0; i < m_ComTexts.size(); i++) ///
	{
		delete m_ComTexts[i];
	}
	m_ComTexts.resize(0);

	for (int i = 0; i < m_imageBoardList.size(); i++) ///图片列表
	{
		delete m_imageBoardList[i];
	}
	m_imageBoardList.resize(0);

}

void Note::Init()
{
	this->SetAlloewExculding(false);

	m_Color = Parameters::Instance()->m_DefaultNoteColor;
	this->SetType(SHAPE_NOTE);

	m_bindBillboard.AllowRotate(false);
	m_bindBillboard.AllowTran(true);
	m_bindBillboard.AllowScale(false);

	this->m_isDirty = true;
}

Vector3 Note::GetTextsCenter()
{
	BoundingBox box;
	for(int i=0;i<this->m_ComTexts.size();i++)
	{
		ComText* comtext = m_ComTexts.at(i);
		box.Merge(comtext->GetBoundingBox());
	}
	return box.m_min;
}

void Note::AddPoint(Point* pnt)
{
	this->m_PointList.push_back(pnt);
}
void Note::AddLine(Line3D* line)
{
	this->m_LineList.push_back(line);
}
void Note::AddPolyLine(PolyLine* polyLine)
{
	this->m_PolyLineList.push_back(polyLine);
}


void Note::AddImage(ImageBoard* imageBoard)
{
	this->m_imageBoardList.push_back(imageBoard);
}

//
void Note::AddText(ComText* ct)
{
	this->m_ComTexts.push_back(ct);

	this->MarkDirty();
}

void Note::AddText(const string& text, const Vector3& pos, float fontSize)
{
	//临时这样用
	ComText* ct = new ComText();
	CText* t = new CText;

	//LOGI("View::AddText2 '%s'",wstring2string(ws).c_str());
	t->SetText(text);
	t->SetInnerLoc(pos);

	Vector3 xAxis, yAxis;
	xAxis.m_x = 0;
	xAxis.m_y = 0;
	xAxis.m_z = 0;
	t->SetInnerXYAxis(xAxis, yAxis);

	//width height
	t->SetCharWidthHeight(fontSize, fontSize);
	ct->AddCText(t);
	this->AddText(ct);

	this->MarkDirty();
}

void Note::SetSelected(bool select)
{
	Shape::SetSelected(select);

	for (int i = 0; i < m_imageBoardList.size(); i++) ///图片列表
	{
		m_imageBoardList[i]->SetSelected(select);
	}

	for (int i = 0; i < m_LineList.size(); i++)
	{
		m_LineList.at(i)->SetSelected(select);
	}
}

void  Note::AddOutFrame(PolyLine* polyLine)
{
	if(polyLine != NULL)
	{
		this->m_outFrameLineList.push_back(polyLine);
	}
}

void Note::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		//return;
	}
	//判断线是否有交点
	Vector3 intersectPos;
	Line3D* line;

	//暂时屏蔽线相交测试
//	for (int i = 0; i < m_LineList.size(); i++)
//	{
//		line = m_LineList.at(i);
//		if (action->Intersect(line->m_StartPoint, line->m_EndPoint,
//				intersectPos))
//		{
//			action->AddIntersectPnt(intersectPos);
//		}
//	}

	//判断文字是否有交点
	//首先将射线变换到billboard校正后的位置
	MutexLock lock(m_mutex);
	if(m_imageBoardList.size() == 0){
		Ray billboardRay = action->GetData()->GetModelRay();
		Matrix3x4 modelMatrixInverse = m_bindBillboard.GetWorldMatrix().Inverse();
		billboardRay = billboardRay.Transformed(modelMatrixInverse);

		for (int i = 0; i < m_ComTexts.size(); i++)
		{
			ComText* comText = m_ComTexts.at(i);
			BoundingBox& comTextBox = comText->GetBoundingBox();
	//		LOGE("boudnding box %s",comTextBox.Tostring().c_str());
			if (RayPickAction::IsRayHitBox(billboardRay,comTextBox))
			{
				intersectPos = m_bindBillboard.GetWorldMatrix()*intersectPos;

				action->AddIntersectPnt(intersectPos);
			}
		}

		for(int i = 0;i<m_LineList.size();i++)
		{
			Line3D * line = m_LineList.at(i);
			Vector3& start = line->m_StartPoint;
			Vector3& end = line->m_EndPoint;
			if (action->Intersect(start, end, intersectPos))
			{
				action->AddIntersectPnt(intersectPos);
				break;
			}
		}
	}

	for (int i = 0; i < m_imageBoardList.size(); i++) ///图片列表
	{
		vector<Vector3> intersects = m_imageBoardList[i]->GetIntersects(action);
		if(intersects.size() > 0)
		{
			for(int j=0;j< intersects.size();j++)
			{
				action->AddIntersectPnt(intersects[j]);
			}
		}
	}

	action->AddShape(this);
}

void Note::FindVisiableObject(RenderAction* renderAction)
{
	if(this->IsVisible())
	{
		this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());

		//准备点的显示数据
//		LOGI("FindVisiableObject 10");
		for (int i = 0; i < m_PointList.size(); i++) ///点列表
		{
			 m_PointList[i]->UpdateRenderData(renderAction);
		}
//		LOGI("FindVisiableObject 11");
		if(this->IsDirty() && m_imageBoardList.size() == 0)
		{
			this->m_isDirty = false;
			this->m_bindBillboard.SetCenter(this->GetTextsCenter());
		}

		this->m_bindBillboard.GetGLWorldMatrix(renderAction);

		MutexLock lock(m_mutex);
//		LOGI("FindVisiableObject 12");
		for (int i = 0; i < m_imageBoardList.size(); i++) ///图片列表
		{
			m_imageBoardList[i]->UpdateRenderData(renderAction);
		}
//		LOGI("FindVisiableObject 1");
		renderAction->PrepareRenderNote(this);
	}
}

void Note::SetVisiableObject(RenderAction* renderAction)
{
	if (this->IsVisible())
	{
		this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());

		//准备点的显示数据
		//		LOGI("FindVisiableObject 10");
		for (int i = 0; i < m_PointList.size(); i++) ///点列表
		{
			m_PointList[i]->UpdateRenderData(renderAction);
		}
		//		LOGI("FindVisiableObject 11");
		if (this->IsDirty() && m_imageBoardList.size() == 0)
		{
			this->m_isDirty = false;
			this->m_bindBillboard.SetCenter(this->GetTextsCenter());
		}

		this->m_bindBillboard.GetGLWorldMatrix(renderAction);

		MutexLock lock(m_mutex);
		//		LOGI("FindVisiableObject 12");
		for (int i = 0; i < m_imageBoardList.size(); i++) ///图片列表
		{
			m_imageBoardList[i]->UpdateRenderData(renderAction);
		}
	}
}

void Note::ComputeBox()
{
//	BoundingBox tempBox;
//
//	//point
//	Vector3 tmpMax;
//	Vector3 tmpMin;
//	for (int i = 0; i < m_PointList.size(); i++)
//	{
//		Vector3* pnt = m_PointList.at(i);
//		tempBox.Merge(*pnt);
//	}
//
//	//line
//	for (int i = 0; i < m_LineList.size(); i++)
//	{
//		Line3D* line = m_LineList.at(i);
//		line->ComputeBox();
//		tempBox.Merge(line->GetBoundingBox());
//	}
//
//	//	//polyline
//	//	for(int i=0;i<m_PolyLineList.size();i++)
//	//	{
//	//		PolyLine* pl = m_PolyLineList.at(i);
//	//		pl->clacBox();
//	//		BoundingBox::GetPlusBox(bdBox,*(pl->GetBoundingBox()));
//	//
//	//	}
//
//	//m_PlaneList
//
//	//Stk_ComText
//	for (int i = 0; i < m_ComTexts.size(); i++)
//	{
//		ComText* ct = m_ComTexts.at(i);
//		tempBox.Merge(ct->GetBoundingBox());
//	}
	m_BoundingBox = BoundingBox(Vector3::MINIMUM,Vector3::MAXMUN);
	//	bdBox.Max.Set(Vector3(max[0],max[1],max[2]));
	//	bdBox.Min.Set(Vector3(min[0],min[1],min[2]));
//	m_BoundingBox.CalculateDiagonal();
}

bool Note::IsDirty()
{
	return this->m_isDirty;
}

void Note::MarkDirty()
{
	this->m_isDirty = true;
}

Vector3 Note::GetAnchorPnt()
{
	Vector3 ret ;
	if(m_PointList.size()>0)
	{
		ret = *(m_PointList.at(0)->GetVertexPos());
	}
	return ret;
}

Billboard* Note::GetBillBoard()
{
	return &this->m_bindBillboard;
}

vector<ImageBoard*>& Note::GetImageBoards()
{
	return this->m_imageBoardList;
}

Vector3 Note::GetImageBoardPosition()
{
	Vector3 position;
	if (m_imageBoardList.size() > 0)
	{
		position = m_imageBoardList[0]->GetPosition();
	}
	return position;
}

string Note::GetTextValue()
{
	string textValue;
	if (this->m_ComTexts.size() > 0)
	{
		ComText * comText = m_ComTexts.at(0);
		if(comText->GetCTextList().size()>0)
		{
			CText * cText = comText->GetCText(0);
			textValue = cText->GetText();
		}
	}
    return textValue;
}

void Note::SetTextValue(string& value)
{

	if (this->m_ComTexts.size() > 0)
	{
		ComText * comText = m_ComTexts.at(0);
		CText * tempText = new CText;
		tempText->SetText(value);
		comText->AddCText(tempText);
	}
	else
	{
		ComText * tempComeText = new ComText;
		CText * tempText = new CText;
		tempText->SetText(value);
		tempComeText->AddCText(tempText);
		this->m_ComTexts.push_back(tempComeText);
	}

}

}


