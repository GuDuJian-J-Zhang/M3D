// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_ResourceManager.h"

namespace HoteamSoft {
	namespace SVLLib {

STK_IMPLEMENT_CLASS(Stk_ResourceManager, Stk_Object)
Stk_ResourceManager::Stk_ResourceManager(void) :Stk_Object()
{
    m_workingDir="";
}
        
        Stk_ResourceManager::~Stk_ResourceManager(void)
{
	//DELETE_STK_CLASS_IMPL
}

Stk_FilePtr Stk_ResourceManager::GetFile(const string& fileUri)
{
	map<string,Stk_FilePtr>::iterator stk_fileKey= m_files.find(fileUri);
	if (stk_fileKey == m_files.end())
	{
		return this->AddFile(fileUri);
	}

	return stk_fileKey->second;
}


Stk_FilePtr Stk_ResourceManager::AddFile(const string& fileUri)
{
	if (fileUri.length() == 0)
	{
		return NULL;
	}
	string fileUriHash = string(fileUri);
	map<string, Stk_FilePtr>::iterator stk_file = m_files.find(fileUriHash);
	if (stk_file == m_files.end())
	{
		Stk_FilePtr newFile = Stk_FilePtr::CreateObject();
		newFile->SetName(this->GetWorkingDir()+fileUri);
		//newFile->OpenAsBin();

		this->m_files.insert(map<string,Stk_FilePtr>::value_type(fileUriHash,newFile));

		return newFile;
	}

	return NULL;
}

void Stk_ResourceManager::RemoveFile(const string& fileUri)
{
	map<string, Stk_FilePtr>::iterator stk_file = m_files.find(fileUri);
	if (stk_file == m_files.end())
	{

	}
}

bool Stk_ResourceManager::Clear()
{

	return true;
}

bool Stk_ResourceManager::CloseAllFile()
{

	return true;
}

void Stk_ResourceManager::SetWorkingDir(const string& workingDir)
{
	this->m_workingDir = workingDir;
}
const string& Stk_ResourceManager::GetWorkingDir()
{
	return m_workingDir;
}

}//svllib
}//hoteamsoft
