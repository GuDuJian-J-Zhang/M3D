#include "m3d/extras/note/NoteGroup.h"
#include "m3d/model/Note.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
//#include "m3d/extras/note/VoiceAnnovation.h"
namespace M3D
{
NoteGroup::NoteGroup()
{
//	this->SetScene(NULL);
}


NoteGroup::~NoteGroup()
{

}

void NoteGroup::Initialize()
{

}

int NoteGroup::GetType( void )
{
	return NOTE_GROUP_NODE;
}

void NoteGroup::Clear()
{
	GroupNode::DeleteAllChildren();
}
}
