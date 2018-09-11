#include "m3d/extras/note/TextNote.h"

namespace M3D
{
TextNote::TextNote() :
		Note()
{
	this->Init();
}

TextNote::~TextNote()
{

}

void TextNote::Init()
{
	this->SetType(SHAPETYPE::SHAPE_TEXT_NOTE);
}

Vector3& TextNote::GetNotePos()
{
	return this->m_notePos;
}

void TextNote::SetNotePos(const Vector3& pos)
{
	this->m_notePos = pos;
}

Vector3& TextNote::GetTextsPos()
{
	return this->m_textPos;
}

void TextNote::SetTextsPos(const Vector3& pos)
{
	this->m_textPos = pos;
}
}
