#include "Selector.h"
#include "m3d/model/IShape.h"
#include "Utility.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"

using M3D::SceneManager;

namespace SVIEW {
 
Selector::Selector(SceneManager* scene) {
    this->m_scene = scene;
}

Selector::~Selector() {
}

void Selector::Set(IShape* shape) {
	//LOGI("Selector::Set()");
	Clear();
	Add(shape);
}

void Selector::Add(IShape* shape) {
	if (shape)
	{
		//	LOGI("Selector::Add() %d",selectList.size());
		shape->SetSelected(true);
		m_selectList.push_back(shape);
		//	LOGI("Selector::Add() is ok");
		m_scene->GetRenderManager()->RequestRedraw();
	}
}

void Selector::Remove(IShape* shape) {
	if (shape)
	{
		//LOGI("BEGIN remove================");
		shape->SetSelected(false);
		vector<IShape*>::iterator iter = m_selectList.begin();
		while (iter != m_selectList.end()) {
			if (shape->GetID() == (*iter)->GetID()) {
				m_selectList.erase(iter);
				break;
			}
			iter++;
		}
		m_scene->GetRenderManager()->RequestRedraw();
	}
}

bool Selector::Exist(IShape* shape) {
    vector<IShape*>::iterator iter = m_selectList.begin();
    while (iter != m_selectList.end()) {
        if (shape->GetID() == (*iter)->GetID()) {
            return true;
        }
        iter++;
    }
    return false;
}
void Selector::Clear() {
	for (int i=0;i<m_selectList.size();i++){
		IShape* shape = m_selectList[i];
		shape->SetSelected(false);
	}
	m_selectList.clear();
	m_scene->GetRenderManager()->RequestRedraw();
}

IShape* Selector::Get() {
	return m_selectList.empty() ? NULL : m_selectList[0];
}

vector<IShape*>& Selector::GetAll() {
	return m_selectList;
}

int Selector::Count() {
	return m_selectList.size();
}

}
