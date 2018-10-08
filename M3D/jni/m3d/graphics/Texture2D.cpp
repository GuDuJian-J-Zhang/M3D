#include "m3d/graphics/Texture2D.h"

#include "m3d/ResourceManager.h"
#include "prebuilt/soil/include/SOIL.h"

namespace M3D
{

unsigned int Texture2D::LoadOGLTexture(const string& filename,
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags)
{
	unsigned int OGLObj = SOIL_load_OGL_texture(filename.c_str(),
			force_channels,
			reuse_texture_ID,
			flags);

	return OGLObj;
}

unsigned int Texture2D::LoadOGLTexture(const unsigned char *const buffer,
		int buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags)
{
	unsigned int OGLObj = SOIL_load_OGL_texture_from_memory(buffer,
			buffer_length,
			force_channels,
			reuse_texture_ID,
			flags);

	return OGLObj;
}

bool Texture2D::FreeOGLTexture(unsigned int OGLObj)
{
	bool freeState = false;


	return freeState;
}

Texture2D::Texture2D():Texture()
{
	Init();
}

Texture2D::Texture2D(const string& imagePath):Texture()
{
	Init();
	this->SetImagePath(imagePath);
}

Texture2D::Texture2D(const string& imagePath,int forceChannels,unsigned int flags):Texture()
{
	Init();

	this->SetImagePath(imagePath);

	this->SetImageParameter(forceChannels,flags);
}

Texture2D::~Texture2D()
{

}


void Texture2D::Init()
{
	m_imagePath = "";
	m_image = NULL;
	this->m_textures[0] = 0;

	this->SetImageParameter(TEXTURE_LOAD_RGBA,TEXTURE_FLAG_MIPMAPS);
	this->MarkDirty();
}

void Texture2D::UpdataOGLObj()
{
//	LOGI(" Texture2D:: UpdataOGLObj() 111 %s",m_imagePath.c_str());
	ResourceManager* resourceMgr = this->m_resourceMgr;
	//路径有值，根据路径去生成纹理
	if (this->m_image)
	{


		string imagePath = this->GetName();
		unsigned int OGLObj = this->m_textures[0];

		//根据路径在资源管理器中没有找到资源，则创建OGL对象加入资源管理器中
		if (OGLObj == 0)
		{
			const unsigned char *const buffer = m_image->m_pPixelData;
			int buffer_length = m_image->m_dataLength;
			OGLObj = LoadOGLTexture(buffer,buffer_length, m_forceChannels,
					0, m_flags);

			if (OGLObj != 0)
			{
				resourceMgr->AddOGLTexture(imagePath, OGLObj);
			}
			else
			{
				LOGE("create Texture2D error!!");
			}
		}

		this->m_textures[0] = OGLObj;

		//LOGI(" Texture2D:: UpdataOGLObj() id %d", OGLObj);
	}else if(m_imagePath.length() > 0)
	{
		unsigned int OGLObj = this->m_textures[0];
//		resourceMgr->GetOGLTexture(this->m_imagePath);
		//根据路径在资源管理器中没有找到资源，则创建OGL对象加入资源管理器中
		if(OGLObj == 0)
		{
			OGLObj = LoadOGLTexture(m_imagePath,m_forceChannels,0,m_flags);

			if(OGLObj != 0)
			{
				resourceMgr->AddOGLTexture(m_imagePath,OGLObj);
			}else
			{
				LOGE("create Texture2D error!!");
			}

		}

		this->m_textures[0] = OGLObj;

		LOGI(" Texture2D:: UpdataOGLObj() id %d",OGLObj);
	}
}


}

