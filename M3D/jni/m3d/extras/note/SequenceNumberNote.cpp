/*
 * SequenceNumberNote.cpp
 *
 *  Created on: 2015-12-14
 *      Author: Administrator
 */

#include "SequenceNumberNote.h"

namespace M3D
{

SequenceNumberNote::SequenceNumberNote():Note()
{
	// TODO Auto-generated constructor stub
	this->Init();

}

SequenceNumberNote::~SequenceNumberNote()
{
	// TODO Auto-generated destructor stub
}

void
SequenceNumberNote::Init()
{
	this->SetType(SHAPETYPE::SHAPE_SEQUENCE_NUMBER_NOTE);
}

Vector3& SequenceNumberNote::GetNotePos()
{
	return this->m_notePos;
}

void SequenceNumberNote::SetNotePos(const Vector3& pos)
{
	this->m_notePos = pos;
}

Vector3& SequenceNumberNote::GetTextsPos()
{
	return this->m_textPos;
}

void SequenceNumberNote::SetTextsPos(const Vector3& pos)
{
	this->m_textPos = pos;
}

}

 /* namespace M3D */
