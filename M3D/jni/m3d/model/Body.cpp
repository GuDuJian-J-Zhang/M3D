#include "m3d/model/Face.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Curve.h"
#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"

namespace M3D
{
Body& Body::operator=(const Body& orig)
{
	if (this != &orig)
	{
		Shape::operator =(orig);
		LOD::operator =(orig);
		//拷贝面数据
		for (int i = 0; i < orig.m_FaceArray.size(); i++)
		{
			Face* face = new Face(*orig.m_FaceArray[i]);
			this->AddFace(face);
		}

		//拷贝边数据
		for (int i = 0; i < orig.m_EdgeArray.size(); i++)
		{
			Edge* edge = new Edge(*orig.m_EdgeArray[i]);
			this->AddEdge(edge);
		}

		//拷贝点数据
		for (int i = 0; i < orig.m_pointArray.size(); i++)
		{
			Point* point = new Point(*orig.m_pointArray[i]);
			this->AddPoint(point);
		}
        
        this->m_polyLine = orig.m_polyLine;
		if(this->m_polyLine)
		{
			this->m_polyLine->AddRef();
		}

	}
	return *this;
}

Body::Body(const Body& orig)
{
	*this = orig;
}

Body::Body() :
		Shape(),LOD()
{
	this->SetModel(NULL);
//	this->SetParentBody(NULL);
	this->SetType(SHAPE_BODY);
	SetPolyLine(NULL);

	this->m_EdgeArray.resize(0);
	this->m_FaceArray.resize(0);
	this->m_pointArray.resize(0);
}

Body::~Body()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		 m_FaceArray[i]->Release();
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		delete m_EdgeArray[i];
	}

	for (int i = 0; i < this->m_pointArray.size(); i++)
	{
		delete m_pointArray[i];
	}

	this->m_EdgeArray.resize(0);
	this->m_FaceArray.resize(0);
	this->m_pointArray.resize(0);


	if(this->m_polyLine)
	this->m_polyLine->Release();
	this->m_polyLine = NULL;
}

void  Body::OnOptimize()
{
//	this->m_EdgeArray.shrink_to_fit();
//	this->m_FaceArray.shrink_to_fit();
//	this->m_pointArray.shrink_to_fit();
}

void Body::AddFace(Face* face)
{
	if(face)
	{
		face->AddRef();
		face->SetBody(this);
		this->m_FaceArray.push_back(face);
	}
}

void Body::AddEdge(Edge* edge)
{
	edge->SetBody(this);
	this->m_EdgeArray.push_back(edge);

}
vector<Edge*>& Body::GetEdges()
{
	return this->m_EdgeArray;
}

vector<Face*>& Body::GetFaces()
{
	return m_FaceArray;
}

void Body::AddPoint(Point* pnt)
{
	if(pnt)
	{
		pnt->SetBody(this);
		this->m_pointArray.push_back(pnt);
	}
}

vector<Point*>& Body::GetPoints()
{
	return this->m_pointArray;
}

void Body::ComputeBox()
{
	this->m_BoundingBox.Clear();

	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		Face* face = this->m_FaceArray[i];
		face->ComputeBox();
		if(face->GetBoundingBox().Defined())
		{
			this->m_BoundingBox.Merge(face->GetBoundingBox());
		}
	}

	vector<Edge*>& bodyEdges = this->GetEdges();
	for (int i = 0; i < bodyEdges.size(); i++)
	{
		Edge* edge = bodyEdges[i];
		if (edge->GetBoundingBox().Defined() && edge->IsVisible())
		{
			this->m_BoundingBox.Merge(edge->GetBoundingBox());
		}
	}

	vector<Point*>& points = this->GetPoints();
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		if (point->GetBoundingBox().Defined()) //所有隐藏的点
		{
			this->m_BoundingBox.Merge(point->GetBoundingBox());
		}
	}
}

void Body::Restore()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->Restore();
	}
	Shape::Restore();
}

/**
 * 设置颜色为初始颜色
 */
void Body::ResetColor()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->ResetColor();
	}

}
void Body::SetInitColor(const Color &color)
{
    for (int i = 0; i < this->m_FaceArray.size(); i++)
    {
        this->m_FaceArray[i]->SetInitColor(color);
    }
}

void Body::ResetAlpha()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->ResetAlpha();
	}
}
    
void Body::SetColor(const Color &color)
{
    for (int i = 0; i < this->m_FaceArray.size(); i++)
    {
        this->m_FaceArray[i]->SetColor(color);
    }
}

void Body::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}
	if (action->Intersect(GetBoundingBox()))
	{
		for (int i = 0; i < this->m_FaceArray.size(); i++)
		{
			this->m_FaceArray[i]->RayPick(action);
		}

		if (action->CanPickShape(SHAPE_EDGE))
		{
			for (int i = 0; i < this->m_EdgeArray.size(); i++)
			{
				m_EdgeArray[i]->RayPick(action);
			}
		}

		if (action->CanPickShape(SHAPE_POINT))
		{
			for (int i = 0; i < this->m_pointArray.size(); i++)
			{
				m_pointArray[i]->RayPick(action);
			}
		}
	}
}

void Body::SetSelected(bool select)
{
	Shape::SetSelected(select);
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetSelected(select);
	}

	for(int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetSelected(select);
	}

	for (int i = 0; i < this->m_pointArray.size(); i++)
	{
		this->m_pointArray[i]->SetSelected(select);
	}
}

void Body::SetAlpha(float a)
{
	Shape::SetAlpha(a);

	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetAlpha(a);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetAlpha(a);
	}

	for (int i = 0; i < this->m_pointArray.size(); i++)
	{
		this->m_pointArray[i]->SetAlpha(a);
	}
}

void Body::SetVisible(bool visible)
{
	Shape::SetVisible(visible);

	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetVisible(visible);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetVisible(visible);
	}

	for (int i = 0; i < this->m_pointArray.size(); i++)
	{
		this->m_pointArray[i]->SetVisible(visible);
	}
}

int Body::GetVertexCount(int level)
{
	int count = 0;
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		count += this->m_FaceArray[i]->GetVertexCount(level);
	}

	return count;
}

void Body::SetModel(Model* model)
{
	this->model = model;
}

Model* Body::GetModel()
{
	return this->model;
}

void Body::InitProperties()
{
	AddProperty("体 名称", m_Name);
	AddProperty("体 ID", IntToString(m_Id));

	AddProperty("体  颜色",
			ColorToString(m_Color.m_r, m_Color.m_g, m_Color.m_b, m_Color.m_a));

	AddProperty("体  LOD0面片数量", IntToString(GetVertexCount(0)));
	AddProperty("体 LOD1 面片数量", IntToString(GetVertexCount(1)));

	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string vomlumeKey =language.GetCurrentTypeValue("FacePropertiesVolume");
	string areaKey =language.GetCurrentTypeValue("FacePropertiesArea");
	float volume = 0;
	float area = 0;

	this->GetVolumeAndArea(volume,area);

	AddProperty(vomlumeKey, M3DTOOLS::floatToString(volume));
	AddProperty(areaKey, M3DTOOLS::floatToString(area));
}

void Body::FindVisiableObject(RenderAction* renderAction)
{
	if(!this->IsVisible())
	{
		return;
	}

	renderAction->UpdataRenderPara(this);

	renderAction->StartMergeFace();
	renderAction->StartMergeEdge();

	RenderEffect* renderType = renderAction->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	//将存储在Body中的边界线，加入显示队列中
	if (dataType.Contain(RenderableType::RGT_EDGELINE))
	{
		vector<Edge*>& bodyEdges = this->GetEdges();
		for (int i = 0; i < bodyEdges.size(); i++)
		{
			Edge* edge = bodyEdges[i];
			renderAction->PrepareRenderEdges(edge);
		}
	}

	//将点显示数据，加入显示队列中
	if(dataType.Contain(RenderableType::RGT_POINT))
	{
		vector<Point*>& points = this->GetPoints();
		for (int i = 0; i < points.size(); i++)
		{
			Point* point = points[i];
			point->FindVisiableObject(renderAction);
		}
	}

	//将面显示数据，加入显示队列中
	if(dataType.Contain(RenderableType::RGT_SOLID))
	{
		vector<Face*>& faceArray = this->GetFaces();
		for (int i = 0; i < faceArray.size(); i++)
		{
			faceArray[i]->FindVisiableObject(renderAction);
		}
	}

	renderAction->FinishMergeFace();
	renderAction->FinishMergeEdge();
}

void Body::SetPolyLine(SPolyLine* polyLine)
{
	this->m_polyLine = polyLine;

	if(this->m_polyLine)
	this->m_polyLine->AddRef();
}

SPolyLine* Body::GetPolyLine()
{
	return this->m_polyLine;
}

void  Body::UpdateContainsShapesId(Model* model)
{
	vector<Edge*>& bodyEdges = this->GetEdges();
	for (int i = 0; i < bodyEdges.size(); i++)
	{
		Edge* edge = bodyEdges[i];
		model->UpdateContainsShapes(edge->GetCopyObjId(),edge);
	}

	vector<Point*>& points = this->GetPoints();
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		model->UpdateContainsShapes(point->GetCopyObjId(),point);
	}

	vector<Face*>& faceArray = this->GetFaces();
	for (int i = 0; i < faceArray.size(); i++)
	{
		Face* face =faceArray[i];
		model->UpdateContainsShapes(face->GetCopyObjId(),face);
	}
}

float Body::GetVolumeAndArea(float& volume,float& area)
{
	IVertexSet* meshData = this->GetData(0);
	if(meshData)
	{
		VertexSet* vertexSet = (VertexSet*)meshData;
		vertexSet->GetVolumeAndArea(volume,area);
	}
	return 0;
}

void Body::SetMaterial(Material* material)
{
    for (int i = 0; i < this->m_FaceArray.size(); i++)
    {
        this->m_FaceArray[i]->SetMaterial(material);
    }
}

}
