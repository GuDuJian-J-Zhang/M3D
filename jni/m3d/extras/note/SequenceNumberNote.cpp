/*
 * SequenceNumberNote.cpp
 *
 *  Created on: 2015-12-14
 *      Author: Administrator
 */

#include "SequenceNumberNote.h"
#include "m3d/base/json/json.h"
#include "m3d/utils/StringHelper.h"
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
/**
 * @brief 将对象值转成JSON数据
 * @param pos 位置
 */
string SequenceNumberNote:: toJson(){
    Json::FastWriter writer;
    Json::Value noteJson ;
    noteJson["notePos"] = this->GetNotePos().Tostring();
    noteJson["textsPos"] = this->GetTextsPos().Tostring();
    noteJson["textValue"] = GetTextValue();
    string jsonStr = writer.write(noteJson);
    return jsonStr;
}
SequenceNumberNote* SequenceNumberNote:: fromJson(string value){
    SequenceNumberNote *note = NULL;
    Json::Reader reader;
    Json::Value readValue;
    
    if (reader.parse(value, readValue))
    {
        note = new SequenceNumberNote();
        string notePos = readValue["notePos"].asString();
        vector<string> nfloats = StringHelper::Split(notePos, " ");
        if (nfloats.size() == 3) {
            float x = StringHelper::StringToFloat(nfloats[0]);
            float y = StringHelper::StringToFloat(nfloats[1]);
            float z = StringHelper::StringToFloat(nfloats[2]);
            note->SetNotePos(Vector3(x, y, z));
        }
        string textPos = readValue["textsPos"].asString();
        vector<string> tfloats = StringHelper::Split(textPos, " ");
        if (tfloats.size() == 3) {
            float x = StringHelper::StringToFloat(tfloats[0]);
            float y = StringHelper::StringToFloat(tfloats[1]);
            float z = StringHelper::StringToFloat(tfloats[2]);
            note->SetTextsPos(Vector3(x, y, z));
        }
        string text = readValue["textValue"].asString();
        note->SetTextValue(text);
            //        note->SetNotePos(.) ;
    }
    return note;
}
}

 /* namespace M3D */
