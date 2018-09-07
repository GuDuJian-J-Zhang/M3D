#include "m3d/model/ShapeSet.h"
#include "m3d/model/Shape.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{
ShapeSet::ShapeSet():Object()
{
	m_origVisible = true;
	this->SetParent(NULL);
	this->m_shapes.clear();
	this->SetType(ShapeSetType::Undefine);
}

ShapeSet::ShapeSet(const ShapeSet& orig)
{
	*this = orig;
}

ShapeSet& ShapeSet::operator=(const ShapeSet& orig)
{
	if (this != &orig)
	{
		//拷贝面数据
		for (int i = 0; i < orig.m_Children.size(); i++)
		{
			ShapeSet* node = new ShapeSet(*orig.m_Children[i]);
			this->AddChild(node);
		}
	}
	return *this;
}

void ShapeSet::SetID(long id)
{
	this->m_ID = id;
}

long ShapeSet::GetID()
{
	return this->m_ID;
}

ShapeSet::~ShapeSet()
{
	//拷贝面数据
	for (int i = 0; i < m_Children.size(); i++)
	{
		delete m_Children[i];
	}
}

void ShapeSet::AddShape(Shape* shape)
{
	if(shape)
	{
		((Shape*)shape)->SetCADNode(this);
		this->m_shapes.push_back(shape);
	}
}

vector<Shape*>& ShapeSet::GetShapes()
{
	return this->m_shapes;
}

ShapeSet::ShapeSetType ShapeSet::GetType()
{
	return m_Type;
}

void ShapeSet::SetType(ShapeSet::ShapeSetType type)
{
	m_Type =type;
}

string ShapeSet::GetName()
{
	return m_Name;
};

void ShapeSet::SetName(string &name)
{
	m_Name = name;
};

vector<ShapeSet*>& ShapeSet::GetChildren()
{
	return m_Children;
}

void ShapeSet::AddChild(ShapeSet *child)
{
	if(child)
	{
		child->SetParent(this);
		m_Children.push_back(child);
	}
}

void ShapeSet::SetParent(ShapeSet* node)
{
	this->m_parent = node;
}

ShapeSet* ShapeSet::GetParent()
{
	return this->m_parent;
}

void ShapeSet::UpdataRelevateShape(Model* model)
{
	/*if(this->m_shapes.size()>0)
	{
		vector<Shape*> priShapes = this->GetShapes();
		this->m_shapes.clear();
		for(int i=0;i<priShapes.size();i++)
		{
			Shape* shape = model->GetContainShape(((Shape*)priShapes[i])->GetCopyObjId());
			if(shape)
			{
				this->AddShape(shape);
			}
		}
	}

	for(int i =0;i< m_Children.size();i++)
	{
		ShapeSet* node = m_Children.at(i);
		if(node)
		{
			node->UpdataRelevateShape(model);
		}
	}*/
}


void ShapeSet::SetVisible(bool visiable)
{
	this->m_visible = visiable;
}

bool ShapeSet::Getvisible()
{
	return this->m_visible;
}

bool ShapeSet::IsOrigVisible()
{
	return m_origVisible;
}

void ShapeSet::SetOrigVisible(bool visible)
{
	m_origVisible = visible;
	this->SetVisible(visible);
}

void ShapeSet::Restore()
{
	for(int i=0;i<m_Children.size();i++)
	{
		m_Children.at(i)->Restore();
	}

	Shape* shape = NULL;
	for(int i=0;i<m_shapes.size();i++)
	{
		shape = m_shapes.at(i);
		shape->SetVisible(this->m_origVisible);
	}
}


}
