/*
 * display.cpp
 *
 *  Created on: 2013-5-31
 *      Author: billy
 */

#include "m3d/M3D.h"
#include "m3d/model/Model.h"
#include "m3d/model/MeshData.h"

#include "m3d/utils/IDCreator.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/utils/FileHelper.h"

#include "Reader.h"
#include "Svlreader.h"

#include "AssimpReader.h"
#include "m3d/utils/FileHelper.h"
#include "sview/views/Parameters.h"

using M3D::IDCreator;
using M3D::StringHelper;
using M3D::FileHelper;

namespace SVIEW
{

ReaderListener::ReaderListener() :
		M3D::Object()
{

}

ReaderListener::~ReaderListener()
{

}

void ReaderListener::OnProcessPercent(Reader* reader, float percent)
{

}

void ReaderListener::OnBegin(Reader* reader)
{

}

void ReaderListener::OnEnd(Reader* reader)
{

}

string Reader::defaultAnimationData = "";
Reader::Reader(void)
{
	m_File = "";
	m_isCancel = false;
	m_IDCreator = NULL;
//	m_FontPath = "";
	m_readPercent = -1;
	InitializeID();
	this->m_readerListener = NULL;

	this->SetView(NULL);
}

Reader::~Reader(void)
{
	Clear();
}

int Reader::GetNewID()
{
	if (this->m_IDCreator != NULL)
	{
		return this->m_IDCreator->GetID(IDCreator::MODEL);
	}
	return 0;
}

int Reader::InitializeID()
{
	if (this->m_IDCreator != NULL)
	{
		this->m_IDCreator->Initialize(IDCreator::MODEL, -1);
		return this->m_IDCreator->GetID(IDCreator::MODEL);

		M3D::Object::OBJID =0;
	}
	return 0;
}

string Reader::GetFile()
{
	return this->m_File;
}

void Reader::SetFile(string file)
{
	this->m_File = file;
}

void Reader::SetView(View* view)
{
	this->m_view = view;
}

Model*
Reader::GetModel()
{
	return GetModel(-1);
}

set<string>& Reader::GetSourceFiles()
{
	return m_srcFiles;
}

bool Reader::AddSourceFile(const string& filePath)
{
	bool ret =false;
	if(m_srcFiles.find(filePath)== m_srcFiles.end())
	{
		m_srcFiles.insert(filePath);
		ret = true;
	}
	return ret;
}

Model*
Reader::GetModel(int id)
{
	return new Model;
}

IVertexSet*
Reader::GetMeshData(int id)
{
	return NULL;
}

void Reader::Cancel()
{
	this->m_isCancel = true;
}

bool Reader::IsCancel()
{
	return this->m_isCancel;
}

void Reader::Clear()
{
	if (this->m_readerListener)
	{
		this->m_readerListener->Release();
		this->m_readerListener = NULL;
	}
}

Reader*
Reader::GetReader(const string &file)
{
	Reader* reader = NULL;
	string ext = StringHelper::ToLower(FileHelper::GetExt(file));
	if(ext == "")
	{
		return reader;
}
	 if(!FileHelper::FileExist(file))
	 {
	 LOGE("File not exist %s",file.c_str());
	 }
    string defaultSuffix = Parameters::Instance()->GetParameter(Parameters::ITEM_DEFAULT_SUFFIX);
    if ("svl" == ext || defaultSuffix == ext)
	{
		reader = new SvlReader();
	}
//	else if ("obj" == ext || "OBJ" == ext)
//	{
//		reader = new ObjReader();
//	}
//	else	if ("stl" == ext || "STL" == ext)
//	{
//		reader = new StlReader();
//	}
	else
	{
		reader = new AssimpReader();
	}

	if (reader != NULL)
	{
		reader->SetFile(file);
	}

	return reader;
}

const string& Reader::GetXMLAnimationData()
{
	return defaultAnimationData;
}

void Reader::SetXMLAnimationData(const string& xmlData)
{

}

void Reader::SetPercent(float percent)
{
	this->m_readPercent = percent;
	if(this->m_readerListener)
	{
		this->m_readerListener->OnProcessPercent(this,percent);
	}
}

float Reader::GetPercent()
{
	return this->m_readPercent;
}

void Reader::SetListener(ReaderListener* listener)
{
	if(this->m_readerListener)
	{
		this->m_readerListener->Release();
		this->m_readerListener = NULL;
	}

	this->m_readerListener = listener;
	if(this->m_readerListener)
	{
		this->m_readerListener->AddRef();
	}
}

ReaderListener* Reader::GetListener()
{
	return this->m_readerListener;
}

//void Reader::SetFontPath(const string &fontPath)
//{
//	if (!fontPath.empty())
//	{
//		m_FontPath = fontPath;
//	}
//}

//string& Reader::GetReadLogs()
//{
//	return this->m_readLogs;
//}

}
