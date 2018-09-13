#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/MeshData.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/graphics/Material.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/body.h"
#include "m3d/model/Model.h"
#include "../graphics/PbrMaterial.h"
namespace M3D
{
BoundingBox Face::m_globalFaceBox;

Face::Face():Object()
{
	this->m_drawCache = NULL;
	m_IsSelect = false;
	m_visible = true;
	m_IsHighlight = false;
	m_material = NULL;
	this->SetColor(Color::GRAY);
	this->SetBody(NULL);
	this->SetMaterial(NULL);
	this->m_Id = OBJID++;
	m_FaceExtInfo = NULL;
	m_drawMesh = NULL;	
	this->m_svlId = OBJID++;
	m_bNeedClip = true;
	this->Selectable(true);
}

Face::~Face()
{
	if(this->m_drawMesh)
	{
	     this->m_drawMesh->Release();
		 m_drawMesh = NULL;
	}
	if (m_FaceExtInfo)
	{
		delete m_FaceExtInfo;
		m_FaceExtInfo  =NULL;
	}
	if (m_material)
	{
		m_material->Release();
	}
}

//bool Face::UseExtInfo()
//{
//	if (this->m_faceExtInfo == NULL)
//	{
//		this->m_faceExtInfo = new FaceExtInfo();
//
//		return true;
//	}
//	return false;
//}
//
//bool Face::UnUseExtInfo()
//{
//	if (this->m_faceExtInfo)
//	{
//		delete this->m_faceExtInfo;
//		this->m_faceExtInfo = NULL;
//
//
//		return true;
//	}
//	return false;
//}

Face::Face(const Face& orig)
{
	this->m_drawCache = NULL;
	m_IsSelect = false;
	m_visible = true;
	m_IsHighlight = false;
	m_material = NULL;	
	m_FaceExtInfo = NULL;
	m_drawMesh = NULL;
	this->m_svlId = OBJID++;
	this->m_selectable = true;
	*this = orig;
}

Face& Face::operator =(const Face& orig)
{
	if (this != &orig)
	{
		Object::operator =(orig);
		this->m_drawMesh = orig.m_drawMesh;
		m_IsSelect = orig.m_IsSelect;
		m_visible = orig.m_visible;
		m_IsHighlight = false;
		this->m_Color = orig.m_Color;
		this->SetBody(NULL);	
		this->m_Id = OBJID++;
		this->SetMaterial(orig.m_material);
		this->SetData(orig.m_drawMesh);	
		this->SetSVLId(orig.m_svlId);

		this->m_selectable = orig.m_selectable;
		if(orig.m_FaceExtInfo)
		{
			this->m_FaceExtInfo = new FaceExtInfo();
			*(this->m_FaceExtInfo) = *(orig.m_FaceExtInfo);
		}
		else
		{
			this->m_FaceExtInfo = NULL;
		}
	}
	return *this;
}

SHAPETYPE Face::GetType(void)
{
	return SHAPETYPE::SHAPE_FACE;
}

BoundingBox& Face::GetBoundingBox()
{
	IVertexSet* meshData = m_drawMesh;

	if (meshData != NULL)
	{
		return meshData->GetBoundingBox();
	}

	return BoundingBox::NO_BOX;
}

void Face::FindVisiableObject(RenderAction* renderAction)
{
	if (m_drawMesh)
	{
		renderAction->PrepareRenderFace(this);
	}
	//RenderEffect* renderType = renderAction->GetRenderEffect();
	//RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	//SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());

	//if (dataType.Contain(RenderableType::RGT_SOLID) && m_drawCache)
	//{
	//	VertexSet* meshData = m_drawCache->GetRefMesh();
	//	if(meshData->IsDirty())
	//	{
	//		SceneManager* scene = renderAction->GetScene();
	//		//显示更新硬件缓存
	//		meshData->UpdateHardWareBuffer(scene);
	//	}

	//	this->SetRenderMesh(this->GetRenderMeshData());
 //
	//	if(!renderAction->IsDelayDraw())
	//	{
	//		if (this->GetRenderColor().IsTransparent())
	//		{
	//			renderAction->PushRenderable(this, RenderableType::RGT_TRANSPARENT);
	//		}
	//		else
	//		{
	//			renderAction->PushRenderable(this, RenderableType::RGT_SOLID);
	//		}

	//		if (this->IsHightlight())
	//		{
	//			renderAction->PushRenderable(this, RenderableType::RGT_INTOP);
	//		}
	//	}else
	//	{
	//		LOGE("Add to delay draw");
	//		renderAction->AddToDelayDraw(this);
	//	}
	//}
}

void Face::Traverse(Action* action)
{

}

void Face::RayPick(RayPickAction* action)
{
	if (!m_visible || !this->m_selectable)
	{
		return;
	}

	if (action->CanPickShape(SHAPE_TRIMESH))
	{
		Mesh* meshData =(Mesh*) this->GetData();
		if (meshData)
		{
			meshData->RayPick(action);
			action->AddShape(this);
		}
	}
}

void Face::FramePick(RayPickAction* action)
{

}
 
Mesh* Face::GetRenderMeshData()
{
	return m_drawCache;
}

void Face::SetData(Mesh* vertexData)
{
	this->m_drawMesh = vertexData;	
	if (m_drawMesh)
	{
		this->m_drawMesh->AddRef();
	}
}

Mesh* Face::GetData()
{
	return this->m_drawMesh;
}
 
void Face::SetRenderMeshData(Mesh* meshCache)
{
	this->m_drawCache = meshCache;
}
 
bool Face::Visible()
{
	return this->m_visible;
}

void Face::SetBody(Body* body)
{
	this->m_body = body;
}

Body* Face::GetBody()
{
	return m_body;
}

void Face::AddEdge(Edge* edge, int level)
{

}

vector<Edge*>* Face::GetEdges()
{
	return NULL;
}

int Face::GetVertexCount(int level)
{
	int count = 0;

	IVertexSet* meshData = this->GetData();

	if (meshData != NULL)
	{
		count += meshData->GetVertexCount();
	}
	return count;
}

void Face::Restore()
{
	ResetColor();
	SetVisible(this->IsVisible());
	SetSelected(false);
}

void Face::Restore(bool resub)
{
	Restore();
}

void Face::SetAlpha(float a)
{
	m_Color.m_a = a;
	if (m_material)
	{
		if (m_material->GetMaterialType() == MaterialType_Base ||
			m_material->GetMaterialType() == MaterialType_Phong||
			m_material->GetMaterialType() == MaterialType_MatCap)
		{
			Material* temp = static_cast<Material*>(m_material);
			temp->Opacity(a);
		}
		else if (m_material->GetMaterialType() == MaterialType_Pbr)
		{
			PbrMaterial* temp = static_cast<PbrMaterial*>(m_material);
			temp->Opacity(a);
		}
	}
}

void Face::SetAlpha(float a, bool relSub)
{
	SetAlpha(a);
}

/**
* 设置颜色为初始颜色
*/
void Face::ResetColor()
{	//m_Color = m_InitColor;
	if (m_FaceExtInfo)
	{
		m_Color = m_FaceExtInfo->m_InitColor;
	}	
}

void Face::SetInitAlpha(float alpha)
{
	m_Color.m_a = alpha;
//	ResetColor();
}

void Face::SetInitHightlight(bool isHighlight)
{
	this->m_IsHighlight = isHighlight;
	if (!m_FaceExtInfo)
	{
		m_FaceExtInfo = new FaceExtInfo();
	}
	m_FaceExtInfo->m_origHighlight = isHighlight;
}

bool Face::IsHightlight()
{
	return this->m_IsHighlight || this->m_IsSelect;
}

void Face::SetInitColor(const Color &color)
{
	//m_InitColor = color;
	m_Color = color;
	if (m_material)
	{
		if (m_material->GetMaterialType() == MaterialType_Base || 
			m_material->GetMaterialType() == MaterialType_Phong||
			m_material->GetMaterialType() == MaterialType_MatCap)
		{
			Material* temp = static_cast<Material*>(m_material);
			temp->SetDiffuse(m_Color);
//			temp->Opacity(m_Color.m_a);
		}
		else if (m_material->GetMaterialType() == MaterialType_Pbr)
		{
			PbrMaterial* temp = static_cast<PbrMaterial*>(m_material);
			temp->AlbedoColor(m_Color);
//			temp->Opacity(m_Color.m_a);
		}		
	}
	if (!m_FaceExtInfo)
	{
		m_FaceExtInfo = new FaceExtInfo();
	}
	m_FaceExtInfo->m_InitColor = m_Color;
//	ResetColor();
}

void Face::ResetAlpha()
{
	if (m_FaceExtInfo)
	{
		m_Color.m_a = (m_FaceExtInfo->m_InitColor).m_a;
		SetAlpha(m_Color.m_a);
	}
}

void Face::SetVisible(bool visible, bool relSub)
{
	SetVisible(visible);
}

void Face::ComputeBox()
{

}

void Face::SetColor(const Color& color, bool reSub)
{
	SetColor(color);
}

void Face::InitProperties()
{
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string nameStr = language.GetCurrentTypeValue("FacePropertiesName");
	string ID = language.GetCurrentTypeValue("FacePropertiesID");
	string Color = language.GetCurrentTypeValue("FacePropertiesColor");
	string LOD0PatchNumber = language.GetCurrentTypeValue("FacePropertiesLOD0PatchNumber");
	string LOD1PatchNumber = language.GetCurrentTypeValue("FacePropertiesLOD1PatchNumber");
	//AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));

	string colorStr = M3DTOOLS::floatToString(m_Color.m_r) + "," + M3DTOOLS::floatToString(m_Color.m_g) + ","
		+ M3DTOOLS::floatToString(m_Color.m_b) + "," + M3DTOOLS::floatToString(m_Color.m_a);
	AddProperty(Color, colorStr);

	AddProperty(LOD0PatchNumber, IntToString(this->GetVertexCount(0) / 3));
	//AddProperty(LOD1PatchNumber, IntToString(this->GetVertexCount(1) / 3));

	string vomlumeKey = language.GetCurrentTypeValue("FacePropertiesVolume");
	string areaKey = language.GetCurrentTypeValue("FacePropertiesArea");
	float volume = 0;
	float area = 0;

	this->GetVolumeAndArea(volume, area);

	//AddProperty(vomlumeKey, M3DTOOLS::floatToString(volume));
	AddProperty(areaKey, M3DTOOLS::floatToString(area));
}

string Face::GetGeometryInfo()
{
	return "";
}

GeometryAttribute* Face::GetGeoAttribute()
{
	Mesh* meshData = (Mesh*) this->GetData();
	if (meshData)
	{
		GeometryAttribute* geo = meshData->GetGeoAttribute();
		if (geo == NULL && this->GetSVLId())
		{
			if (this->GetBody())
			{
				if (this->GetBody()->GetModel())
				{
					geo = this->GetBody()->GetModel()->GetExtendInfoManager()->GetFaceGeoAttribute(
						this->GetBody()->GetModel()->GetProtoTypeId(), this->GetSVLId());
				}
			}
			meshData->SetGeoAttribute(geo);
		}
		return geo;
	}
	return NULL;
}

void Face::SetColor(const Color &color)
{
	this->m_Color = color;
}

void Face::SetVisible(bool visible)
{
	this->m_visible = visible;
}
 
void Face::SetType(SHAPETYPE type)
{
	//this->m_Type = type;
}

void Face::SetSelected(bool select)
{
	this->m_IsSelect = select;

	if (this->GetBody())
	{
		this->GetBody()->MarkDrawDataDirty();
	}
}

Color* Face::GetDrawColor()
{
	if (IsSelected() && (SVIEW::Parameters::Instance()->m_selectedStyle ==2|| SVIEW::Parameters::Instance()->m_selectedStyle == 3))
	{
		//LOGE("shape is selected id:: %d",(int)this);
		return Color::SelectColor;
	}
	else
	{
		//if (m_material)
		//{
		//	if (m_material->GetMaterialType() == MaterialType_Base || m_material->GetMaterialType() == MaterialType_Phong)
		//	{
		//		Material* temp = static_cast<Material*>(m_material);
		//		m_Color = temp->GetDiffuse();
		//	}
		//	else if (m_material->GetMaterialType() == MaterialType_Pbr)
		//	{
		//		PbrMaterial* temp = static_cast<PbrMaterial*>(m_material);
		//		m_Color = temp->AlbedoColor();
		//	}
		//}
		return &m_Color;
	}
		
	
}

Color* Face::GetColor()
{
	
	if (m_material)
	{
		if (m_material->GetMaterialType() == MaterialType_Base ||
			m_material->GetMaterialType() == MaterialType_Phong||
			m_material->GetMaterialType() == MaterialType_MatCap)
		{
			Material* temp = static_cast<Material*>(m_material);
			return &temp->GetDiffuse();
		}
		else if (m_material->GetMaterialType() == MaterialType_Pbr)
		{
			PbrMaterial* temp = static_cast<PbrMaterial*>(m_material);
			return &temp->AlbedoColor();
		}
		else
		{
			return &this->m_Color;
		}
	}

	return &this->m_Color;
}


float Face::GetAlpha()
{
	float ret = 0;
	if (m_material)
	{
		if (m_material->GetMaterialType() == MaterialType_Base ||
			m_material->GetMaterialType() == MaterialType_Phong||
			m_material->GetMaterialType() == MaterialType_MatCap)
		{
			Material* temp = static_cast<Material*>(m_material);
			ret = temp->Opacity();
		}
		else if (m_material->GetMaterialType() == MaterialType_Pbr)
		{
			PbrMaterial* temp = static_cast<PbrMaterial*>(m_material);
			ret = temp->Opacity();
		}
	}
	else
	{
		ret = m_Color.m_a;
	}

	return ret;
}
bool Face::IsSelected() const
{
	return m_IsSelect;
}

void Face::SetBox(const BoundingBox& box)
{
	//m_BoundingBox = box;
}

bool Face::IsVisible()
{
	return this->m_visible;
}


bool Face::IsOrigVisible()
{
	return true;
}


void Face::SetOrigVisible(bool visible)
{
	//m_origVisible = visible;

	this->SetVisible(visible);
}

void Face::SetOrigVisible(bool visible, bool relSub)
{
	this->SetVisible(visible);
}

string Face::GetProperties()
{
	if (!m_FaceExtInfo)
	{
		return "";
	}
	if (m_FaceExtInfo->m_IsFirstGetProperties)
	{
		InitProperties();
		m_FaceExtInfo->m_IsFirstGetProperties = false;
	}
	return m_FaceExtInfo->m_Properties;
}

void Face::ClearProperties()
{
	if (this->m_FaceExtInfo)
	{
		this->m_FaceExtInfo->m_Properties.clear();
	}
}

/**
* 获取id
* @return
*/
IDTYPE Face::GetID()
{
	return this->m_Id;
}

/**
* 设置Id
* @param value
*/
void Face::SetID(IDTYPE  value)
{
	this->m_Id = value;
}

std::string Face::GetName()
{
	return "";
}

void Face::SetName(const std::string& value)
{
	//this->m_Name = value;
}

/**
* 获取初始颜色
* @return
*/
Color* Face::GetInitColor()
{
	if (m_FaceExtInfo)
	{
		return &(m_FaceExtInfo->m_InitColor);
	}
	return NULL;
}

void Face::Selectable(bool selectable)
{
	m_selectable = selectable;
}


bool Face::Selectable()
{
	return this->m_selectable;
}
 
void Face::SetSceneNode(SceneNode* node)
{
	//this->node = node;
}

SceneNode* Face::GetSceneNode()
{
	return NULL;
}

void Face::SetMaterial(BaseMaterial* material)
{
	//防止自身被设置多次
	if (material == this->m_material)
	{
		return;
	}

	//先释放之前的
	if (this->m_material )
	{
		m_material->Release();
	}
	this->m_material = material;
	if (this->m_material)
	{
		this->m_material->AddRef();
	}
}

BaseMaterial* Face::GetMaterial()
{
	return this->m_material;
}

bool Face::AllowExculding()
{
	return true;
}

void Face::SetAlloewExculding(bool allow)
{
	//this->m_allowExcluding = allow;
}

bool Face::RendreVisible()
{
	return true;
}

void Face::SetRenderVisible(bool visible)
{
	//this->m_renderVisible = visible;
}

void Face::AddProperty(string key, string value)
{
	if (!m_FaceExtInfo)
	{
		return;
	}
	if (m_FaceExtInfo->m_Properties.find(key + "::") != string::npos)
	{
		LOGI("find properties");
		return;
	}
	if (!m_FaceExtInfo->m_Properties.empty())
		m_FaceExtInfo->m_Properties.append(";;");
	m_FaceExtInfo->m_Properties.append(key + "::" + value);
}

void Face::AddRef(void)
{
	Object::AddRef();
}

void Face::Release(void)
{
	Object::Release();
}
 
int Face::GetSVLId()
{
	return this->m_svlId;
}

void Face::SetSVLId(int Id)
{
	this->m_svlId = Id;
}

const Color& Face::GetRenderColor()
{
	return *GetDrawColor();
}

IDTYPE Face::GetCopyObjId()
{
	return 0;
}

BaseMaterial* Face::GetRenderMaterial()
{
	return this->GetMaterial();
}

float Face::GetVolumeAndArea(float& volume, float& area)
{
	volume = 0;
	area = 0;
	Mesh* mesh = this->GetData();
	if (mesh)
	{
		mesh->GetVolumeAndArea(volume, area);
	}

	return 0;
}

void Face::SetFaceExtInfo()
{
	if (!m_FaceExtInfo)
	{
		m_FaceExtInfo = new FaceExtInfo();
	}
	m_FaceExtInfo->m_InitColor = m_Color;
	m_FaceExtInfo->m_origHighlight = m_IsHighlight;
}

void Face::SetNeedClip(bool val)
{
	m_bNeedClip = val;
}

bool Face::GetNeedClip()
{
	return m_bNeedClip;
}

FaceExtInfo::FaceExtInfo()
{
	m_IsFirstGetProperties = true;
}

FaceExtInfo::~FaceExtInfo()
{

}

}
