#include "m3d/model/Image.h"

#include "m3d/renderer/RenderContext.h"
#include "m3d/action/RenderAction.h"
#include "m3d/utils/StringHelper.h"
#include "../utils/FileHelper.h"
namespace M3D
{

void Image::SetPath(const string& fileName)
{
	if (fileName.length() == 0)
	{
		return;
	}
	m_imagePath = fileName;
}

string& Image::GetPath()
{
	return this->m_imagePath;
}

void Image::SetData(unsigned char* data,int dataLength)
{
	this->m_dataLength = dataLength;
	this->m_pPixelData = data;
}

string Image::GetHashCode()
{
	if (this->m_pPixelData)
	{
#ifdef WIN32
		return StringHelper::IntToString((int)this->m_pPixelData);
#else
		return StringHelper::IntToString((long int)this->m_pPixelData);
#endif
	}
	else
	{
		return this->m_imagePath;
	}
	return "";
}

void Image::ReadFile()
{
	if (this->m_imagePath.length()>0 && this->m_pPixelData == NULL)
	{
		char* readBuffer = NULL;
		FileHelper::ReadBinFile(m_imagePath, readBuffer, this->m_dataLength);
		this->m_pPixelData =(unsigned char*)readBuffer;
	}
}

Image::~Image()
{
	if (this->m_pPixelData != NULL)
	{
		
		delete[] this->m_pPixelData;
	}
	this->m_pPixelData = NULL;
}

Image::Image():Object()
{
	m_bAlpha = false;
	m_iWidth = 0;
	m_iHeight = 0;

	m_pPixelData = NULL;
	m_dataLength = 0; 
	this->m_Id = OBJID++;
}
}
