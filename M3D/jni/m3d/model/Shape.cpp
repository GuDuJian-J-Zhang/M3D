#include "m3d/model/Shape.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/Material.h"
#include "sview/views/Parameters.h"
#include "m3d/action/Action.h"
#include "m3d/action/RenderAction.h"

namespace M3D
{
Shape::Shape():Object()
{
	this->Init();
}
Shape::~Shape()
{
	if(this->m_material)
	{
		this->m_material->Release();

		this->m_material = NULL;
	}
}

void Shape::Optimize()
{
	OnOptimize();
}

void Shape::OnOptimize()
{

}

Shape::Shape(const Shape& orig)
{
	this->Init();
	*this = orig;
}
Shape& Shape::operator =(const Shape& orig)
{
	if (this != &orig)
	{
		Object::operator=(orig);
		//m_HasColor = orig.m_HasColor;
		m_Visible = orig.m_Visible;
		m_IsSelect = orig.m_IsSelect;
		m_Color = orig.m_Color;
		//m_InitColor = orig.m_InitColor;
		node = NULL;
		m_IsFirstGetProperties = orig.m_IsFirstGetProperties;
		//m_Properties = orig.m_Properties;
		this->SetType(orig.m_Type);

		//设置材质
		this->SetMaterial(orig.m_material);

		//this->m_copyId = orig.m_copyId;//拷贝对象，使用同一个copy对象的id
	}
	return *this;
}

SHAPETYPE Shape::GetType(void)
{
	return m_Type;
}

void Shape::SetType(SHAPETYPE type)
{
	this->m_Type = type;
}

BoundingBox& Shape::GetBoundingBox()
{
	if(!m_BoundingBox.Defined())
	{
		ComputeBox();
	}
	return m_BoundingBox;
}

bool Shape::IsVisible()
{
	return this->m_Visible;
}

bool Shape::IsOrigVisible()
{
	//return m_origVisible;
	return true;
}

void Shape::SetOrigVisible(bool visible)
{
	//m_origVisible = visible;

	this->SetVisible(visible);
}

void Shape::SetOrigVisible(bool visible, bool relSub)
{
	//this->m_origVisible = visible;
	this->SetVisible(visible);
}

void Shape::SetVisible(bool visible)
{
	this->m_Visible = visible;
}

void Shape::SetVisible(bool visible, bool relSub)
{

}

void Shape::ComputeBox()
{

}

void Shape::SetBox(const BoundingBox& box)
{
	m_BoundingBox = box;
}

void Shape::SetColor(const Color& color)
{
	this->m_Color = color;
}

Color* Shape::GetColor()
{
	return &m_Color;
}

Color* Shape::GetDrawColor()
{
	if (IsSelected())
	{
		//LOGE("shape is selected id:: %d",(int)this);
		return Color::SelectColor;
	}
	else
		return &m_Color;
}

float Shape::GetAlpha()
{
	return m_Color.m_a;
}

void Shape::SetAlpha(float a)
{
	m_Color.m_a = a; // > m_OldAlpha ? m_OldAlpha : a;
}

void Shape::SetAlpha(float a, bool relSub)
{
	m_Color.m_a = a; // > m_OldAlpha ? m_OldAlpha : a;
}

void Shape::SetSelected(bool select)
{
	this->m_IsSelect = select;
}

bool Shape::IsSelected() const
{
	return m_IsSelect;
}

void Shape::RayPick(RayPickAction* action)
{

}

void Shape::FramePick(RayPickAction* action)
{

}

string Shape::GetProperties()
{
	//if (m_IsFirstGetProperties)
	//{
	//	InitProperties();
	//	m_IsFirstGetProperties = false;
	//}
	//return m_Properties;
	return "";
}

void Shape::InitProperties()
{

}

void Shape::AddProperty(string key, string value)
{
	/*if(m_Properties.find(key+ "::") != string::npos)
	{
		LOGI("find properties");
		return;
	}
	if (!m_Properties.empty())
		m_Properties.append(";;");
	m_Properties.append(key + "::" + value);*/
}

void Shape::ClearProperties()
{
	//m_Properties.clear();
}

void Shape::SetSceneNode(SceneNode* node)
{
	this->node = node;
}

SceneNode* Shape::GetSceneNode()
{
	return this->node;
}

void Shape::Restore()
{
	Shape::ResetColor();
	Shape::SetVisible(this->IsOrigVisible());
	Shape::SetSelected(false);
}

void Shape::FindVisiableObject(RenderAction* renderAction)
{

}

string Shape::GetGeometryInfo()
{
	string geoInfo;

	return geoInfo;
}	

void Shape::Traverse(Action* action)
{

}

void Shape::SetHasChild(bool haschild)
{
	//this->m_hasChild  = haschild;
}

bool Shape::GetHasChild()
{
	//return this->m_hasChild;
	return false;
}

void Shape::SetMaterial(BaseMaterial* material)
{
	this->m_material = material;
	if(this->m_material)
	{
		this->m_material->AddRef();
	}
}

BaseMaterial* Shape::GetMaterial()
{
	return this->m_material;
}

void Shape::SetCADNode(ShapeSet* node)
{
	//this->m_parentNode = node;
}

ShapeSet* Shape::GetCADNode()
{
	//return this->m_parentNode;
	return NULL;
}

void Shape::SetCopyObjId(IDTYPE objId)
{
	//this->m_copyId = objId;
}

IDTYPE Shape::GetCopyObjId()
{
	//return this->m_copyId;
	return 0;
}

void Shape::AddRef(void)
{
	Object::AddRef();
}

void Shape::Release(void)
{
	Object::Release();
}

int Shape::GetSVLId()
{
	return -1;
}

void Shape::SetSVLId(int Id)
{

}

void Shape::SetScene(SceneManager* scene)
{
	//this->m_scene = scene;
}

bool Shape::AllowExculding()
{
	return this->m_allowExcluding;
}

void Shape::SetAlloewExculding(bool allow)
{
	this->m_allowExcluding = allow;
}

bool Shape::RendreVisible()
{
	//return this->m_renderVisible;
	return true;
}

void Shape::SetRenderVisible(bool visible)
{
	//this->m_renderVisible = visible;
}

void Shape::Init()
{
	//m_Type = shapeType::SHAPE;
	//color = Color::Default;
	//ambcolor = Color::Default;
	//difcolor = Color::Default;
	//m_HasColor = false;
	m_Visible = true;
	//m_origVisible = true;
	m_IsSelect = false;
	m_allowExcluding = true;
	m_IsFirstGetProperties = true;
	//m_OldAlpha = 1.0f;
	//m_InitColor =  Color(0.8, 0.8, 0.8, 1.0);
	node = NULL;
	//this->m_Id = IDCreator::GetDefaultID();
	this->m_Id = OBJID++;
	//m_hasChild = false;
	m_isHighlight = false;
	//m_renderVisible = true;
//    this->SetDataChanged(true,false);
	this->SetMaterial(NULL);
	this->SetCADNode(NULL);
	this->SetScene(NULL);
}


float Shape::GetVolumeAndArea(float& volume,float& area)
{
	volume = 0;
	area = 0;
	return 0;
}

}
