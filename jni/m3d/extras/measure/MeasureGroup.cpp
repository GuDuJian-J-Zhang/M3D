#include "m3d/extras/measure/MeasureGroup.h"
#include "m3d/model/Note.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"

namespace M3D
{

MeasureGroup::MeasureGroup()
{
	this->SetScene(NULL);
}


MeasureGroup::~MeasureGroup()
{

}

void MeasureGroup::Initialize()
{

}

int MeasureGroup::GetType( void )
{
	return MEASURE_GROUP_NODE;
}

void MeasureGroup::SetScene(SceneManager* scene)
{
	this->m_scene = scene ;
}

SceneManager* MeasureGroup::GetScene()
{
	return this->m_scene ;
}

}
