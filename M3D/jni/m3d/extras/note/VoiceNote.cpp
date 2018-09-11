#include "m3d/extras/note/VoiceNote.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/graphics/Billboard.h"
#include "m3d/model/Image.h"
#include "m3d/graphics/Texture.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/graphics/Texture2D.h"
#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/action/RenderAction.h"
using namespace SVIEW;

namespace M3D
{
//const string VoiceNote::VoiceAnnotationImage = "/data/pic/voicenote.png";
//Texture* VoiceNote::defaultImage = NULL;
//
//Texture* VoiceNote::GetDefaultVoiceImage()
//{
//	if(!defaultImage)
//	{
//		defaultImage = new Texture2D;
//
//		if(defaultImage && defaultImage->GetType() == Texture::TEXTURE_2D)
//		{
//			Texture2D* texture2d = (Texture2D*)defaultImage;
//			texture2d->SetImagePath(Parameters::Instance()->m_appWorkPath + VoiceAnnotationImage);
//		}
////		string path = Parameters::Instance()->GetParameter("resPath");
////		defaultImage->ReadZip(path, VoiceAnnovation::VoiceAnnotationImage);
//	}
//	return defaultImage;
//}

VoiceNote::VoiceNote(SceneManager* scene) :
		Note(),m_uri(""),m_format(""),m_voiceData("")
{
	Init(scene);
}

VoiceNote::VoiceNote(const Vector3& position ,SceneManager* scene) :
				Note()
{
	this->SetType(SHAPE_VOICE_NOTE);
	float ratio = 5.0f;
	m_imageboard = new ImageBoard(position,
			Vector2(ratio, ratio));
	m_imageboard->SetTexture(scene->GetResourceManager()->GetDefaultVoiceImage());
    this->SetPosition(position);
}

void VoiceNote::Init(SceneManager* scene)
{
	float ratio = 5.0f;
	m_imageboard = new ImageBoard(Vector3::ZERO,
			Vector2(ratio, ratio));
	m_imageboard->SetTexture(scene->GetResourceManager()->GetDefaultVoiceImage());
	this->SetType(SHAPE_VOICE_NOTE);

}

VoiceNote::VoiceNote(const Vector3& pos, const Vector2& size ,SceneManager* scene):
						Note()
{
	this->SetType(SHAPE_VOICE_NOTE);
	m_imageboard = new ImageBoard(pos,size);
	m_imageboard->SetTexture(scene->GetResourceManager()->GetDefaultVoiceImage());
	this->SetHasChild(true);
	Set(pos,size);
	this->SetPosition(pos);
}

void VoiceNote::Set(const Vector3& pos, const Vector2& size)
{
	if(m_imageboard)
	{
		m_imageboard->Set(pos,size);
	}
}

VoiceNote::~VoiceNote()
{
	if(m_imageboard)
	{
		delete m_imageboard;
		m_imageboard = NULL;
	}
}

void VoiceNote::SetSelected(bool select)
{
	Note::SetSelected(select);

	if(this->m_imageboard)
	{
		this->m_imageboard->SetSelected(select);
	}
}

void VoiceNote::ComputeBox()
{
//	LOGE("VoiceAnnovation::ComputeBox()");
//	m_BoundingBox.Clear();
	m_BoundingBox = BoundingBox(Vector3::MINIMUM,Vector3::MAXMUN);
//	LOGE("max box %s",BoundingBox::MAX_BOX.Tostring().c_str());
//	LOGE("voice box %s",m_BoundingBox.Tostring().c_str());
//	if(m_imageboard)
//	{
//		this->m_BoundingBox.Merge(m_imageboard->GetBoundingBox());
//	}
}

void VoiceNote::FindVisiableObject(RenderAction* renderAction)
{
	if(this->IsVisible())
	{
		this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());
		//如果使用billboard效果，则进行矩阵校正
		if(m_imageboard)
		{
			m_imageboard->UpdateRenderData(renderAction);
		}

		renderAction->PrepareRenderNote(this);
	}
}

void VoiceNote::SetPosition(const Vector3& position)
{
	this->m_position = position;

	if(m_imageboard)
	{
		m_imageboard->SetPosition(position);
	}
}

Vector3& VoiceNote::GetPosition()
{
	return this->m_position;
}

ImageBoard* VoiceNote::GetImageboard()
{
	return m_imageboard;
}

void VoiceNote::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}

	vector<Vector3> intersects =  m_imageboard->GetIntersects(action);

	if(intersects.size() > 0)
	{
		for(int i=0;i< intersects.size();i++)
		{
			action->AddIntersectPnt(intersects[i]);
		}
		action->AddShape(this);
	}
}

}
