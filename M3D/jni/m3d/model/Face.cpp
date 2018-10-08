#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/MeshData.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"

namespace M3D
{
LFace::LFace():Object()
{
	this->m_drawCache = NULL;
	m_IsSelect = false;
	m_visible = true;
	this->SetColor(&Color::GRAY);
	this->SetParent(NULL);
}

LFace::~LFace()
{
	if(this->m_drawCache)
	{
	     this->m_drawCache->Release();
	}
}

SHAPETYPE  LFace::GetType(void)
{
	return SHAPETYPE::SHAPE_LITTLE_FACE;
}

BoundingBox LFace::GetBoundingBox()
{
	BoundingBox tempBox;
 
	IVertexSet* meshData = m_drawCache;

	if (meshData != NULL)
	{
		 tempBox = meshData->GetBoundingBox();
	}

	return tempBox;
}

void LFace::FindVisiableObject(RenderAction* renderAction)
{
	RenderEffect* renderType = renderAction->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());

	if (dataType.Contain(RenderableType::RGT_SOLID) && m_drawCache)
	{
		VertexSet* meshData = m_drawCache->GetRefMesh();
		if(meshData->IsDirty())
		{
			SceneManager* scene = renderAction->GetScene();
			//显示更新硬件缓存
			meshData->UpdateHardWareBuffer(scene);
		}

		this->SetRenderMesh(this->GetRenderMeshData());
 
		if(!renderAction->IsDelayDraw())
		{
			if (this->GetRenderColor().IsTransparent())
			{
				renderAction->PushRenderable(this, RenderableType::RGT_TRANSPARENT);
			}
			else
			{
				renderAction->PushRenderable(this, RenderableType::RGT_SOLID);
			}
		}else
		{
			LOGE("Add to delay draw");
			renderAction->AddToDelayDraw(this);
		}
	}
}

void LFace::RayPick(RayPickAction* action)
{
	if (!m_visible)
	{
		return;
	}

	if (action->CanPickShape(SHAPE_TRIMESH))
	{
		Mesh* meshData =(Mesh*) this->GetRenderMeshData();
		meshData->RayPick(action);
		action->AddShape(this);
	}
}
 
Mesh* LFace::GetRenderMeshData()
{
	return m_drawCache;
}
 
void LFace::SetRenderMeshData(Mesh* meshCache)
{
	this->m_drawCache = meshCache;
	if(meshCache)
	{
		this->m_drawCache->AddRef();
	}
}

const  Color& LFace::GetRenderColor()
{
	if(this->m_IsSelect)
	{
		return Color::RED;
	}
	else
	{
		return m_Color;
	}
}

void LFace::SetColor(const Color* color)
{
	this->m_Color =*color;
}

void LFace::SetSelected(bool isSelected)
{
	this->m_IsSelect = isSelected;
}

IShape* LFace::GetParent()
{
	return this->m_parent;
}

void LFace::SetParent(IShape* parent)
{
	this->m_parent = parent;
}

Face::Face() :
		Shape(),LOD()
{
	this->SetBody(NULL);
	this->SetType(SHAPE_FACE);

	m_drawCache = NULL;
}

Face::Face(const Face& orig)
{
	*this = orig;
}

Face& Face::operator=(const Face& orig)
{
	if (this != &orig)
	{
		Shape::operator =(orig);
		LOD::operator =(orig);

		for (int i = 0; i < orig.m_edges.size(); i++)
		{
			this->AddEdge(new Edge(*orig.m_edges.at(i)), 0);
		}
	}
	return *this;
}

Face::~Face()
{
	//在这里不做释放操作，占用的内存在 Reader中进行了统一的管理，当reader进行析构时候进行释放
	for (int i = 0; i < this->m_edges.size(); i++)
	{
		delete m_edges.at(i);
	}
	this->m_edges.clear();
}

void Face::ComputeBox()
{
	m_BoundingBox.Clear();

	if (!m_BoundingBox.Defined())
	{
		BoundingBox tempBox;
		m_BoundingBox = tempBox;

		IVertexSet* meshData = this->GetData();

		if (meshData != NULL)
		{
			if (meshData->GetBoundingBox().Defined())
			{
				tempBox = meshData->GetBoundingBox();
				m_BoundingBox = tempBox;
			}
		}
	}
}

void Face::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}

	if (action->Intersect(GetBoundingBox()))
	{
		if (action->CanPickShape(SHAPE_EDGE))
		{
			for (int i = 0; i < this->m_edges.size(); i++)
			{
				m_edges[i]->RayPick(action);
			}
		}

		if (action->CanPickShape(SHAPE_TRIMESH))
		{
			Mesh* meshData =(Mesh*) this->GetData();
			meshData->RayPick(action);
			action->AddShape(this);
		}
	}

}

void Face::SetSelected(bool select)
{
	Shape::SetSelected(select);

	for (int i = 0; i < this->m_edges.size(); i++)
	{
		m_edges[i]->SetSelected(select);
	}
}

void Face::SetAlpha(float a)
{
	Shape::SetAlpha(a);
}

void Face::SetVisible(bool visible)
{
	Shape::SetVisible(visible);
}

int Face::GetVertexCount(int level)
{
	int count = 0;

	IVertexSet* meshData = this->GetData(level);

	if (meshData != NULL)
	{
		count += meshData->GetVertexCount();
	}
	return count;
}

void Face::AddEdge(Edge* edge, int level)
{
	if(level  == 0)
	{
		edge->SetFace(this);
		this->m_edges.push_back(edge);
	}
	else
	{
		LOGE("Face::AddEdge(Edge* edge, int level) level %d TODO ",level);
	}
}

vector<Edge*>& Face::GetEdges()
{
	return this->m_edges;
}

void Face::SetBody(Body* body)
{
	this->body = body;
}

Body* Face::GetBody()
{
	return this->body;
}

string Face::GetGeoInfo()
{
	string geoInfo = "Face GetoInfo TODO";

	return geoInfo;
}

void Face::InitProperties()
{
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string nameStr =language.GetCurrentTypeValue("FacePropertiesName");
	string ID = language.GetCurrentTypeValue("FacePropertiesID");
	string Color =language.GetCurrentTypeValue("FacePropertiesColor");
	string LOD0PatchNumber = language.GetCurrentTypeValue("FacePropertiesLOD0PatchNumber");
	string LOD1PatchNumber =language.GetCurrentTypeValue("FacePropertiesLOD1PatchNumber");
	AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));

	string colorStr = M3DTOOLS::floatToString(m_Color.m_r)+","+M3DTOOLS::floatToString(m_Color.m_g)+","
			+M3DTOOLS::floatToString(m_Color.m_b)+","+M3DTOOLS::floatToString(m_Color.m_a);
	AddProperty(Color,colorStr);

	AddProperty(LOD0PatchNumber, IntToString(this->GetVertexCount(0)/3));
	AddProperty(LOD1PatchNumber, IntToString(this->GetVertexCount(1)/3));

	string vomlumeKey =language.GetCurrentTypeValue("FacePropertiesVolume");
	string areaKey =language.GetCurrentTypeValue("FacePropertiesArea");
	float volume = 0;
	float area = 0;

	this->GetVolumeAndArea(volume,area);

	//AddProperty(vomlumeKey, M3DTOOLS::floatToString(volume));
	AddProperty(areaKey, M3DTOOLS::floatToString(area));
}

void Face::FindVisiableObject(RenderAction* renderAction)
{
	renderAction->PrepareRenderFace(this);
}

Mesh* Face::GetRenderMeshData()
{
	return this->m_drawCache;
}

void Face::SetRenderMeshData(Mesh* meshCache)
{
	this->m_drawCache = meshCache;
}

const Color&  Face::GetRenderColor()
{
	return this->GetDrawColor();
}

Material*  Face::GetRenderMaterial()
{
	return this->GetMaterial();
}

float Face::GetVolumeAndArea(float& volume,float& area)
{
	volume = 0;
	area = 0;

	Mesh* mesh = (Mesh*)this->GetData(0);
	if(mesh)
	{
		mesh->GetVolumeAndArea(volume,area);
	}

	return 0;
}

}
