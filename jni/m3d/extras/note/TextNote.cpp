#include "m3d/extras/note/TextNote.h"
#include "m3d/base/json/json.h"
#include "m3d/utils/StringHelper.h"
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
/**
 * @brief 将对象值转成JSON数据
 * @param pos 位置
 */
string TextNote:: toJson(){
    Json::FastWriter writer;
    Json::Value noteJson ;
    noteJson["notePos"] = this->GetNotePos().Tostring();
    noteJson["textsPos"] = this->GetTextsPos().Tostring();
    noteJson["textValue"] = GetTextValue();
    string jsonStr = writer.write(noteJson);
    return jsonStr;
}
TextNote *TextNote:: fromJson(string value){
    TextNote *note = NULL;
    Json::Reader reader;
    Json::Value readValue;
    
    if (reader.parse(value, readValue))
    {
        note = new TextNote();
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
