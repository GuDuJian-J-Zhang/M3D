#include "AnnotationGroup.h"
#include "m3d/extras/annotation/Annotation.h"

namespace M3D
{
	AnnotationGroup::AnnotationGroup()
	{
		m_strNodeName = "AnnotationGroup";
		m_scene = NULL;
	}
	AnnotationGroup::~AnnotationGroup()
	{

	}
	void AnnotationGroup::Initialize()
	{

	}

	void AnnotationGroup::Clear()
	{
		GroupNode::DeleteAllChildren();
	}


	void AnnotationGroup::SetScene(SceneManager* scene)
	{
		this->m_scene = scene;
	}

	SceneManager* AnnotationGroup::GetScene()
	{
		return this->m_scene;
	}

}