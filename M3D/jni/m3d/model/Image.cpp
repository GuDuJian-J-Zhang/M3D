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

void Image::LoadImage(string name)
{
	m_iWidth = m_iHeight = 0;
	ifstream ffin(name, std::ios::binary);
	if (!ffin)
	{
		std::cout << "Can not open this file." << std::endl;
		return;
	}
	int result = Get_extension(name);
	char s1[2] = { 0 }, s2[2] = { 0 };
	switch (result)
	{
	case 1://gif
		ffin.seekg(6);
		ffin.read(s1, 2);
		ffin.read(s2, 2);
		m_iWidth = (unsigned char)(s1[1]) << 8 | (unsigned char)(s1[0]);
		m_iHeight = (unsigned char)(s2[1]) << 8 | (unsigned char)(s2[0]);
		break;
	case 2://jpg
		ffin.seekg(255);
		ffin.read(s1, 2);
		ffin.read(s2, 2);
		m_iWidth = (unsigned char)(s2[0]) << 8 | (unsigned char)(s2[1]);
		m_iHeight = (unsigned char)(s1[0]) << 8 | (unsigned char)(s1[1]);
		break;
	case 3://png
		ffin.seekg(18);
		ffin.read(s1, 2);
		ffin.seekg(2, std::ios::cur);
		ffin.read(s2, 2);
		m_iWidth = (unsigned char)(s1[0]) << 8 | (unsigned char)(s1[1]);
		m_iHeight = (unsigned char)(s2[0]) << 8 | (unsigned char)(s2[1]);
		break;
	case 4://bmp
		ffin.seekg(18);
		ffin.read(s1, 2);
		ffin.seekg(2, std::ios::cur);
		ffin.read(s2, 2);
		m_iWidth = (unsigned char)(s1[1]) << 8 | (unsigned char)(s1[0]);
		m_iHeight = (unsigned char)(s2[1]) << 8 | (unsigned char)(s2[0]);
		break;
	default:
		std::cout << "NO" << std::endl;
		break;
	}
	ffin.close();
}

int Image::Get_extension(string name)
{
	char c = name.at(name.length() - 1);
	char c2 = name.at(name.length() - 3);
	if ((c == 'f') && (c2 == 'g'))
		return 1;//gif
	else if ((c == 'g') && (c2 == 'j'))
		return 2;//jpg
	else if ((c == 'g') && (c2 == 'p'))
		return 3;//png
	else if ((c == 'p') && (c2 == 'b'))
		return 4;//bmp
	return 0;
}

}
