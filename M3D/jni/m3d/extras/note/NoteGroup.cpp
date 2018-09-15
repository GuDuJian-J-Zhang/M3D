#include "m3d/extras/note/NoteGroup.h"
#include "m3d/model/Note.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
//#include "m3d/extras/note/VoiceAnnovation.h"
namespace M3D
{
    const int NoteGroup::ANNOTATION_TYPE_BASE = 0; //!<文本批注
    const int NoteGroup::ANNOTATION_TYPE_COMPONENTNAME = 1; //!<零组件批注
    const int NoteGroup::ANNOTATION_TYPE_VOICE = 1001; //!<语音批注
    const int NoteGroup::ANNOTATION_TYPE_SEQUENCE = 2;//!<序号批注
    const int NoteGroup::ANNOTATION_TYPE_GESTURE = 1002;//3D手势批注
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
