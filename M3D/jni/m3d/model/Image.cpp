#include "m3d/model/Image.h"

#include "m3d/renderer/RenderContext.h"

namespace M3D
{

void Image::Read(const string& fileName)
{
	if (fileName.length() == 0)
	{
		return;
	}
	FILE* file = fopen(fileName.c_str(), "r");
	if (file == NULL)
	{
		LOGI("image file not exist\n");
		return;
	}
	int w, h;
	char s[25];
	int a;
	if (file != NULL)
	{
		fscanf(file, "%d %d", &w, &h);
		LOGI("w=%d h=%d\n", w, h);
		this->m_iWidth = w;
		this->m_iHeight = h;
		fscanf(file, "%s", s);
		LOGI("format is %s",s);
		if (s == "RGB")
		{
			m_bAlpha = false;
			this->m_pPixelData = new unsigned char[w * h * 3];
			for (int i = 0; i < w * h * 3; ++i)
			{
				fscanf(file, "%d", &a);
				this->m_pPixelData[i] = (unsigned char) a;
			}
		}
		else
		{
			m_bAlpha = true;
			this->m_pPixelData = new unsigned char[w * h * 4];
			for (int i = 0; i < w * h * 4; ++i)
			{
				fscanf(file, "%d", &a);
				//LOGE("RGBA %d",a);
				this->m_pPixelData[i] = (unsigned char) a;

				if ((i + 1) % 4 == 0)
				{
					//a = 255-a;
					//LOGE("the data is %d  %d  %d", this->pixelData[i-3], this->pixelData[i-2], this->pixelData[i-1]);
					if (this->m_pPixelData[i - 3] == this->m_pPixelData[i - 2]
							&& this->m_pPixelData[i - 2]
									== this->m_pPixelData[i - 1]
							&& this->m_pPixelData[i - 1] > 200)
					{
						this->m_pPixelData[i - 3] = 0;
						this->m_pPixelData[i - 2] = 0;
						this->m_pPixelData[i - 1] = 0;
						this->m_pPixelData[i] = 0;
					}
				}
			}

		}
	}
	fclose(file);
}

void Image::SetData(const void* data)
{
	char* image = (char*) data;
	int w, h;
	char s[25];
	int a;
	if (image != NULL)
	{
		sscanf(image, "%d %d", &w, &h);
		image += sizeof(int) * 2;

		LOGI("w=%d h=%d\n", w, h);
		this->m_iWidth = w;
		this->m_iHeight = h;
		sscanf(image, "%s", s);
		image += sizeof(char) * strlen(s);
		if (s == "RGB")
		{
			m_bAlpha = false;
			this->m_pPixelData = new unsigned char[w * h * 3];
			for (int i = 0; i < w * h * 3; ++i)
			{
				sscanf(image, "%d", &a);
				this->m_pPixelData[i] = (unsigned char) a;
				image += sizeof(int);
			}
		}
		else
		{
			m_bAlpha = true;
			this->m_pPixelData = new unsigned char[w * h * 4];
			for (int i = 0; i < w * h * 4; ++i)
			{
				sscanf(image, "%d", &a);
				this->m_pPixelData[i] = (unsigned char) a;
				image += sizeof(int);
			}

		}
	}

}

void Image::SetData(unsigned char* data,int dataLength)
{
	this->m_dataLength = dataLength;
	this->m_pPixelData = data;
}

Image::~Image()
{
	if (this->m_pPixelData != NULL)
	{
		delete[] this->m_pPixelData;
	}
	this->m_pPixelData = NULL;
}

Image::Image()
{
	m_bAlpha = false;
	m_iWidth = 0;
	m_iHeight = 0;

	m_pPixelData = NULL;
	m_dataLength = 0;
}
}
