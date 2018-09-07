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

bool Body::MergeFace()
{
	vector<Face*> tmpFaceArray;
	Face* selectFace = NULL;
	bool bFirst = true;
	int nCount = 0;
	for (vector<Face*>::iterator ite = m_FaceArray.begin(); ite != m_FaceArray.end(); )
	{
		Face * face = *ite;
		if (!face)
		{
			continue;
		}
		selectFace = NULL;
		for (int i = 0; i < tmpFaceArray.size(); i++)
		{
			if (tmpFaceArray[i] && face->GetColor() == tmpFaceArray[i]->GetColor())
			{
				selectFace = tmpFaceArray[i];
				break;
			}
		}
		if (selectFace)
		{
			Mesh* selectMesh = selectFace->GetData();
			Mesh* mesh = face->GetData();
			if (!selectMesh || !mesh)
			{
				continue;
			}
			VertexSet*  selectVertexSet = selectMesh->GetRefMesh();
			VertexSet*  vertexSet = mesh->GetRefMesh();

			std::vector<Vector3>* selectPnts = selectVertexSet->GetPositionArray();
			std::vector<Vector3>* selectNormals = selectVertexSet->GetNormalArray();
			std::vector<M3D_INDEX_TYPE>* selectIndexArray = selectVertexSet->GetIndexArray();
			std::vector<Vector3>* selectTextures = selectVertexSet->GetTextureCoordArray();

			std::vector<Vector3>* Pnts = vertexSet->GetPositionArray();
			std::vector<Vector3>* Normals = vertexSet->GetNormalArray();
			std::vector<M3D_INDEX_TYPE>* IndexArray = vertexSet->GetIndexArray();
			std::vector<Vector3>* Textures = vertexSet->GetTextureCoordArray();

			unsigned int dataOffset = selectMesh->GetDataOffset();
			unsigned int dataLength = selectMesh->GetDataLength();
			unsigned int offset = mesh->GetDataOffset();
			unsigned int length = mesh->GetDataLength();

			if (selectVertexSet->IsUseIndex())
			{
				if (face->GetBody() != this && bFirst)//if (selectPnts != Pnts && bFirst)
				{
					nCount = selectPnts->size();
					selectPnts->insert(selectPnts->end(), Pnts->begin(), Pnts->end());
					selectNormals->insert(selectNormals->end(), Normals->begin(), Normals->end());
					selectTextures->insert(selectTextures->end(), Textures->begin(), Textures->end());
					bFirst = false;
				}

				std::vector<M3D_INDEX_TYPE> tmpIndexArray;
				if (face->GetBody() == this)//if (selectPnts == Pnts)
				{
					for (vector<M3D_INDEX_TYPE>::iterator cIt = selectIndexArray->begin() + offset;
						cIt != selectIndexArray->end(); )
					{
						tmpIndexArray.push_back(*cIt);
						cIt = selectIndexArray->erase(cIt);
						if (tmpIndexArray.size() == length)
						{
							break;
						}
					}
				}
				else
				{
					for (int i = offset; i < offset + length; i++)
					{
						M3D_INDEX_TYPE nIndex = IndexArray->at(i);
						tmpIndexArray.push_back(nIndex + nCount);
					}
				}
				selectIndexArray->insert(selectIndexArray->begin() + dataOffset + dataLength, tmpIndexArray.begin(), tmpIndexArray.end());
				ResizeMeshIndex(&tmpFaceArray, dataOffset, dataLength, length, true);
				selectMesh->SetDataLength(dataLength + length);
			}
			else
			{
				selectIndexArray->clear();
				std::vector<Vector3> insertPnts;
				std::vector<Vector3> insertNormals;
				std::vector<Vector3> insertTextures;
				if (selectPnts != Pnts)
				{
					if (Pnts->size() > 0)
					{
						insertPnts.assign(Pnts->begin() + offset, Pnts->begin() + offset + length);
					}
					if (selectNormals->size() > 0 && Normals->size() > 0)
					{
						insertNormals.assign(Normals->begin() + offset, Normals->begin() + offset + length);
					}
					if (selectTextures->size() < 0 && Textures->size() > 0)
					{
						insertTextures.assign(Textures->begin() + offset, Textures->begin() + offset + length);
					}

				}
				else
				{
					if (selectPnts->size() > 0)
					{
						for (vector<Vector3>::iterator cIt = selectPnts->begin() + offset;
							cIt != selectPnts->end(); )
						{
							insertPnts.push_back(*cIt);
							cIt = selectPnts->erase(cIt);
							if (insertPnts.size() == length)
							{
								break;
							}
						}
					}
					if (selectNormals->size() > 0)
					{
						for (vector<Vector3>::iterator cIt = selectNormals->begin() + offset;
							cIt != selectNormals->end(); )
						{
							insertNormals.push_back(*cIt);
							cIt = selectNormals->erase(cIt);
							if (insertNormals.size() == length)
							{
								break;
							}
						}
					}
					if (selectTextures->size() > 0)
					{
						for (vector<Vector3>::iterator cIt = selectTextures->begin() + offset;
							cIt != selectTextures->end(); )
						{
							insertTextures.push_back(*cIt);
							cIt = selectTextures->erase(cIt);
							if (insertTextures.size() == length)
							{
								break;
							}
						}
					}
				}

				if (insertPnts.size() > 0)
				{
					selectPnts->insert(selectPnts->begin() + dataOffset + dataLength, insertPnts.begin(), insertPnts.end());
				}
				if (insertNormals.size() > 0)
				{
					selectNormals->insert(selectNormals->begin() + dataOffset + dataLength, insertNormals.begin(), insertNormals.end());
				}
				if (insertTextures.size() > 0)
				{
					selectTextures->insert(selectTextures->begin() + dataOffset + dataLength, insertTextures.begin(), insertTextures.end());
				}

				ResizeMeshIndex(&tmpFaceArray, dataOffset, dataLength, length, true);
				selectMesh->SetDataLength(dataLength + length);
			}
		}
		else
		{
			Face * newFace = new Face();
			*newFace = *face;

			if (face->GetBody() != this && tmpFaceArray.size() > 0 && bFirst)//if (tmpFaceArray.size() > 0 && bFirst)
			{
				Mesh* newMesh = newFace->GetData();
				VertexSet*  newVertexSet = newMesh->GetRefMesh();
				
				Face* insertFace = tmpFaceArray.at(0);
				Mesh* insertMesh = insertFace->GetData();
				VertexSet* vertexSet = insertMesh->GetRefMesh();
				
				std::vector<Vector3>* existPnts = vertexSet->GetPositionArray();
				std::vector<Vector3>* existNormals = vertexSet->GetNormalArray();
				std::vector<M3D_INDEX_TYPE>* existIndexArray = vertexSet->GetIndexArray();
				std::vector<Vector3>* existTextures = vertexSet->GetTextureCoordArray();

				std::vector<Vector3>* newPnts = newVertexSet->GetPositionArray();
				std::vector<Vector3>* newNormals = newVertexSet->GetNormalArray();
				std::vector<M3D_INDEX_TYPE>* newIndexArray = newVertexSet->GetIndexArray();
				std::vector<Vector3>* newTextures = newVertexSet->GetTextureCoordArray();

				if (vertexSet->IsUseIndex())
				{
					nCount = existPnts->size();
					existPnts->insert(existPnts->end(), newPnts->begin(), newPnts->end());
					existNormals->insert(existNormals->end(), newNormals->begin(), newNormals->end());
					existTextures->insert(existTextures->end(), newTextures->begin(), newTextures->end());
					bFirst = false;

					unsigned int offset = newMesh->GetDataOffset();
					unsigned int length = newMesh->GetDataLength();

					Mesh* createMesh = new Mesh(vertexSet);
					createMesh->SetDataOffset(existIndexArray->size());
					createMesh->SetDataLength(length);
					newFace->SetData(createMesh);
					for (int i = offset; i < offset + length; i++)
					{
						M3D_INDEX_TYPE nIndex = newIndexArray->at(i);
						existIndexArray->push_back(nIndex + nCount);
					}
					newMesh->Release();
				}
			}

			tmpFaceArray.push_back(newFace);
		}
		face->Release();
		face = NULL;
		ite = m_FaceArray.erase(ite);
	}
	for (int i = 0; i < tmpFaceArray.size(); i++)
	{
		AddFace(tmpFaceArray.at(i));;
	}
	return true;
}

void Body::ResizeMeshIndex(vector<Face*>* faceArray, unsigned int Offset1, unsigned int length1, unsigned int length2, bool value)
{
	for (int i = 0; i < faceArray->size(); i++)
	{
		Face * face = faceArray->at(i);
		if (face)
		{
			Mesh* mesh = face->GetData();
			if (mesh != NULL)
			{
				if (value)
				{
					unsigned int dataOffset = mesh->GetDataOffset();
					if (dataOffset >= Offset1 + length1)
					{
						mesh->SetDataOffset(dataOffset + length2);
					}
				}
				else
				{
					unsigned int dataOffset = mesh->GetDataOffset();
					if (dataOffset >= Offset1 + length1)
					{
						mesh->SetDataOffset(dataOffset + length2);
					}
				}

			}
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
