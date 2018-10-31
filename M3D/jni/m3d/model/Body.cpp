#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Model.h"
#include "m3d/action/Action.h"
#include "m3d/action/RenderAction.h"
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
		Object::operator =(orig); 
		this->SetModel(NULL);
		this->SetType(SHAPE_BODY);
		SetPolyLine(NULL);
		this->m_Id = OBJID++;
		this->m_FaceArray.resize(0);
		this->m_EdgeArray.resize(0);
		//拷贝面数据
		for (int i = 0; i < orig.m_FaceArray.size(); i++)
		{
			Face* face = new Face(*(Face*)orig.m_FaceArray[i]);
			this->AddFace(face);
		}
		//拷贝边数据
		for (int i = 0; i < orig.m_EdgeArray.size(); i++)
		{
			Edge* edge = new Edge(*orig.m_EdgeArray[i]);
			this->AddEdge(edge);
		}

		this->m_polyLine = orig.m_polyLine;
		if (this->m_polyLine)
		{
			this->m_polyLine->AddRef();
		}
		this->m_BodyExtInfo = orig.m_BodyExtInfo;	
		m_isDrawDataDirty = true;

		this->SetSVLId(orig.m_svlId);
	}
	return *this;
}

Body::Body(const Body& orig)
{
	*this = orig;
}

Body::Body() :
	Object(),m_polyLine(NULL)
{
	this->SetModel(NULL);
	//	this->SetParentBody(NULL);
	this->SetType(SHAPE_BODY);
	SetPolyLine(NULL);
	this->m_svlId = OBJID++;

	this->m_Id = OBJID++;

	this->m_FaceArray.resize(0);
	this->m_EdgeArray.resize(0);
	m_BodyExtInfo = NULL;
	m_isDrawDataDirty = true;

}

Body::~Body()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		m_FaceArray[i]->Release();
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		m_EdgeArray[i]->Release();
	}
	this->m_FaceArray.resize(0);
	this->m_EdgeArray.resize(0);
	if (this->m_polyLine)
		this->m_polyLine->Release();
	this->m_polyLine = NULL;
}

bool Body::UseExtInfo()
{
	return false;
}

bool Body::UnUseExtInfo()
{
	return false;
}

void  Body::OnOptimize()
{
	//	this->m_EdgeArray.shrink_to_fit();
	//	this->m_FaceArray.shrink_to_fit();
	//	this->m_pointArray.shrink_to_fit();
}

void Body::AddFace(Face* face)
{
	if (face)
	{
		face->AddRef();
		face->SetBody(this);
		this->m_FaceArray.push_back(face);
	}	
	m_isDrawDataDirty = true;
}

void Body::AddEdge(Edge* edge)
{
	edge->AddRef();
	edge->SetBody(this);
	this->m_EdgeArray.push_back(edge);

	m_isDrawDataDirty = true;
}
vector<Edge*>* Body::GetEdges()
{
	return &this->m_EdgeArray;
}

vector<Face*>& Body::GetFaces()
{
	return m_FaceArray;
}

void Body::AddPoint(Point* pnt)
{
	if (pnt)
	{

	}	
	m_isDrawDataDirty = true;
}

vector<Point*>* Body::GetPoints()
{
	return NULL;
}

void Body::ComputeBox()
{

}

void Body::Restore()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->Restore();
	}
	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->Restore();
	}
	m_isDrawDataDirty = true;
	//ResetColor();
	//SetVisible(this->IsOrigVisible());
	//SetSelected(false);
}
void Body::Restore(bool resub)
{
	Restore();
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

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->ResetColor();
	}
	m_isDrawDataDirty = true;
}
 
void Body::SetColor(const Color &color)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetColor(color);
	}
	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->ResetColor();
	}	
	m_isDrawDataDirty = true;
}

void Body::SetColor(const Color& color, bool reSub)
{
	SetColor(color);
}
 
void Body::SetAlpha(float a)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetAlpha(a);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetAlpha(a);
	}
	m_isDrawDataDirty = true;
}
 
void Body::SetModel(Model* model)
{
	this->model = model;	
	m_isDrawDataDirty = true;
}

Model* Body::GetModel()
{
	return this->model;
}

void Body::SetInitAlpha(float alpha)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetInitAlpha(alpha);
	}
	m_isDrawDataDirty = true;
}

void Body::SetInitHightlight(bool isHighlight) 
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetInitHightlight(isHighlight);
	}	
	m_isDrawDataDirty = true;
}

bool Body::IsHightlight()
{
	return false;
}

void Body::SetInitColor(const Color &color)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetInitColor(color);
	}
	m_isDrawDataDirty = true;
}

void Body::ResetAlpha()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->ResetAlpha();
	}
	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->ResetAlpha();
	}	
	m_isDrawDataDirty = true;
}

void Body::RayPick(RayPickAction* action)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->RayPick(action);
	}

	//if (action->CanPickShape(SHAPE_EDGE))
	{
		for (int i = 0; i < this->m_EdgeArray.size(); i++)
		{
			m_EdgeArray[i]->RayPick(action);
		}
	}
}

void Body::FramePick(RayPickAction* action)
{

}

void Body::SetSelected(bool select)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetSelected(select);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetSelected(select);
	}
	m_isDrawDataDirty = true;
}


void Body::SetVisible(bool visible)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->SetVisible(visible);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->SetVisible(visible);
	}	
	m_isDrawDataDirty = true;
}

void Body::Selectable(bool selectable)
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		this->m_FaceArray[i]->Selectable(selectable);
	}

	for (int i = 0; i < this->m_EdgeArray.size(); i++)
	{
		this->m_EdgeArray[i]->Selectable(selectable);
	}

	m_isDrawDataDirty = true;
}

SHAPETYPE Body::GetType(void)
{
	return SHAPETYPE::SHAPE_BODY;
}

void Body::SetType(SHAPETYPE type)
{
	
}

Color* Body::GetDrawColor()
{
	return NULL;
}

Color* Body::GetColor()
{
	if (this->m_FaceArray.size() > 0)
	{
		return m_FaceArray[0]->GetColor();
	}
	return NULL;
}

float Body::GetAlpha()
{
	if (this->m_FaceArray.size() > 0)
	{
		return m_FaceArray[0]->GetAlpha();
	}
	return 1.0f;
}

bool Body::IsSelected() const
{
	return false;
}

void Body::SetBox(const BoundingBox& box)
{
	
}

bool Body::IsVisible()
{
	return true;
}

bool Body::IsOrigVisible()
{
	return true;
}

void Body::SetOrigVisible(bool visible)
{
	this->SetVisible(visible);
}

void Body::SetOrigVisible(bool visible, bool relSub)
{
	this->SetVisible(visible);
}

BoundingBox& Body::GetBoundingBox()
{
	if (!m_FaceArray.empty())
	{
		if (m_FaceArray[0] && m_FaceArray[0]->GetData())
		{
			VertexSet* vertexSet = m_FaceArray[0]->GetData()->GetRefMesh();
			if (vertexSet != NULL)
			{
				return vertexSet->GetBoundingBox();
			}
		}
	}
	return BoundingBox::NO_BOX;
}

string Body::GetProperties()
{
	return "";
}

void Body::ClearProperties()
{
	
}

/**
* 获取id
* @return
*/
IDTYPE Body::GetID()
{
	return this->m_Id;
}

/**
* 设置Id
* @param value
*/
void Body::SetID(IDTYPE  value)
{
	this->m_Id = value;
}

std::string Body::GetName()
{
	return "";
}

void Body::SetName(const std::string& value)
{
	 
}

/**
* 获取初始颜色
* @return
*/
Color* Body::GetInitColor()
{
	return NULL;
}


void Body::SetSceneNode(SceneNode* node)
{
	
}

SceneNode* Body::GetSceneNode()
{
	return NULL;
}

void Body::SetMaterial(BaseMaterial* material)
{
	for (int j = 0; j < GetFaces().size(); j++) {
		Face *face = GetFaces().at(j);
		if (face) {
			face->SetMaterial(material);
		}
	}
}

BaseMaterial* Body::GetMaterial()
{
	return NULL;
}

bool Body::AllowExculding()
{
	return false;
}

void Body::SetAlloewExculding(bool allow)
{
	
}


bool Body::RendreVisible()
{
	return true;
}

void Body::SetRenderVisible(bool visible)
{
	 
}

string Body::GetGeometryInfo()
{
	string geoInfo;

	return geoInfo;
}

void Body::SetAlpha(float a, bool relSub)
{
	this->SetAlpha(a);
}

void Body::SetVisible(bool visible, bool relSub)
{
	this->SetVisible(visible);
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
 
void Body::InitProperties()
{

}

void Body::AddProperty(string key, string value)
{

}

void Body::FindVisiableObject(RenderAction* renderAction)
{
	if (!this->IsVisible())
	{
		return;
	}
	renderAction->StartMergeFace();
	renderAction->StartMergeEdge();

	RenderEffect* renderType = renderAction->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	//将面显示数据，加入显示队列中
	if (dataType.Contain(RenderableType::RGT_SOLID))
	{
		vector<Face*>& subFaces = this->GetFaces();
		if (this->m_isDrawDataDirty)
		{
			for (int j = 0; j < subFaces.size(); j++)
			{
				Face* shape = subFaces[j];
				shape->FindVisiableObject(renderAction);
			}
		}
		//else
		{
			for (int j = 0; j < subFaces.size(); j++)
			{
				Face* shape = subFaces[j];
				if (shape->GetRenderMeshData())
				{
					renderAction->AddFaceToDrawQueue(shape); 

					if (dataType.Contain(RenderableType::RGT_TRILINE))
					{
						renderAction->FacePrepareLineMesh(shape);
					}
				}
			}
		}
		this->m_isDrawDataDirty = false;
	}

	//将存储在Body中的边界线，加入显示队列中
	if (dataType.Contain(RenderableType::RGT_EDGELINE))
	{
		vector<Edge*>* bodyEdges = this->GetEdges();
		for (int i = 0; i < bodyEdges->size(); i++)
		{
			Edge* edge = bodyEdges->at(i);
			renderAction->PrepareRenderEdges(edge);
		}
	}
 
	renderAction->FinishMergeFace();
	renderAction->FinishMergeEdge();
}

void Body::Traverse(Action* action)
{

}

void Body::AddRef(void)
{
	Object::AddRef();
}

void Body::Release(void)
{
	Object::Release();
}

int Body::GetSVLId()
{
	return this->m_svlId;
}

void Body::SetSVLId(int Id)
{
	this->m_svlId = Id;
}

int Body::GetLodCount()
{
	return 1;
}

void Body::SetPolyLine(SPolyLine* polyLine)
{
	this->m_polyLine = polyLine;

	if (this->m_polyLine)
		this->m_polyLine->AddRef();
}

SPolyLine* Body::GetPolyLine()
{
	return this->m_polyLine;
}

void  Body::UpdateContainsShapesId(Model* model)
{
	/*vector<Edge*>& bodyEdges = this->GetEdges();
	for (int i = 0; i < bodyEdges.size(); i++)
	{
		Edge* edge = bodyEdges[i];
		model->UpdateContainsShapes(edge->GetCopyObjId(), edge);
	}

	vector<Point*>& points = this->GetPoints();
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		model->UpdateContainsShapes(point->GetCopyObjId(), point);
	}

	vector<Face*>& faceArray = this->GetFaces();
	for (int i = 0; i < faceArray.size(); i++)
	{
		Face* face = faceArray[i];
		model->UpdateContainsShapes(face->GetCopyObjId(), face);
	}*/
}

void Body::MarkDrawDataDirty()
{
	this->m_isDrawDataDirty = true;
}

float Body::GetVolumeAndArea(float& volume, float& area)
{
	//IVertexSet* meshData = this->GetData(0);
	//if (meshData)
	//{
	//	VertexSet* vertexSet = (VertexSet*)meshData;
	//	vertexSet->GetVolumeAndArea(volume, area);
	//}

	return 0;
}


void Body::SetBodyExtInfo()
{
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		m_FaceArray[i]->SetFaceExtInfo();
	}
}

VertexSet* Body::GetData(int lodLevel /*= 0*/)
{
	VertexSet* vertexSet = NULL;
	//默认返回面中第一个关联的Mesh
	for (int i = 0; i < this->m_FaceArray.size(); i++)
	{
		if (m_FaceArray[i])
		{
			if (m_FaceArray[i]->GetData())
			{
				if (m_FaceArray[i]->GetData()->GetRefMesh())
				{
					vertexSet = m_FaceArray[i]->GetData()->GetRefMesh();
					break;
				}
			}
		}
	}

	return vertexSet;
}

void Body::SetNeedClip(bool val)
{
	for (size_t i = 0; i < m_FaceArray.size(); i++)
	{
		if (m_FaceArray[i])
		{
			m_FaceArray[i]->SetNeedClip(val);
		}
	}
	for (size_t i = 0; i < m_EdgeArray.size(); i++)
	{
		if (m_EdgeArray[i])
		{
			m_EdgeArray[i]->SetNeedClip(val);
		}
	}
}

BodyExtInfo::BodyExtInfo()
{

}
BodyExtInfo::~BodyExtInfo()
{

}

}
