#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/ResourceManager.h"
#include "m3d/base/FileCacheManager.h"

namespace M3D
{

	HardWareVertexBuffer::HardWareVertexBuffer() :
		GPUObject()
	{
		Init();
	}

	HardWareVertexBuffer::~HardWareVertexBuffer()
	{
		if (m_object)
		{
			if(m_bufferType == GPUObject::GPU_CACHE)
			{
				if(this->m_resourceMgr)
				{
					this->m_resourceMgr->AddGLObject(m_object,ResourceManager::VBO);
				}
			}else if(m_bufferType == GPUObject::DISK_CACHE )
			{

			}
		}
	}

	void HardWareVertexBuffer::Init()
	{
		m_normalOffset = 0;
		m_vertexOffset = 0;
		m_textureCoordsOffset = 0;

		this->m_bufferSize = 0;
		this->pBaseAddress = NULL;
		m_bufferType = GPUObject::NO_CACHE;
	}

	void HardWareVertexBuffer::SetSize(unsigned bufferSize, bool dynamic)
	{
		this->m_dynamic = dynamic;
		this->m_bufferSize = bufferSize;
	}

	void HardWareVertexBuffer::SetData(const void* data)
	{
		
	}

	void HardWareVertexBuffer::WriteBuffer()
	{
		bool ret =false;
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			//这里修改内存文件数据
			delete[] pBaseAddress;
			pBaseAddress = NULL;

		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
			if(fileCacheMgr)
			{
				unsigned long tObject = 0;
				unsigned long bufferOffset = 0;

				m_bufferSize = fileCacheMgr->writeCache((void*)(pBaseAddress), m_bufferSize,
					tObject, bufferOffset);

				m_bufferOffset  = bufferOffset;
				m_object = tObject;
			}
			//这里修改内存文件数据
			delete[] pBaseAddress;
			pBaseAddress = NULL;
		}
	}

	bool HardWareVertexBuffer::SetDataRange(const void* data, unsigned start,
		unsigned count, bool discard)
	{
		bool ret =false;
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			if (m_object)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_object);
				glBufferSubData(GL_ARRAY_BUFFER, start, count, data);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				ret = true;
			}
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if(pBaseAddress)
			{
				//将数据拷贝到内存映射区域中
				memcpy(pBaseAddress+start,data,count);

				ret = true;
			}
		}

		return ret;
	}

	bool HardWareVertexBuffer::Create(int cacheType)
	{
		bool createState = true;
		m_bufferType = cacheType;

		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			//调用OpenGL api 在显存上分配空间
			glGenBuffers(1, &m_object);
			glBindBuffer(GL_ARRAY_BUFFER, m_object);

			if(!pBaseAddress)
			{
				//预留空间，共每一次设置内存时使用
				pBaseAddress = new unsigned char[m_bufferSize];
			}

			if(this->m_dynamic)
			{
				glBufferData(GL_ARRAY_BUFFER, m_bufferSize, pBaseAddress, GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, m_bufferSize, pBaseAddress, GL_STATIC_DRAW);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//对显存分配空间结果检测
			if (glGetError() == GL_OUT_OF_MEMORY)
			{
				createState = false;
				LOGE("HardWareVertexBuffer::Create() ERROR! 显存空间不足");
			}
		}
		else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if(!pBaseAddress)
			{
				//预留空间，共每一次设置内存时使用
				pBaseAddress = new unsigned char[m_bufferSize];
			}
		}

		return createState;
	}

	bool HardWareVertexBuffer::HasValue()
	{
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			return (m_object>0);
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			return (m_bufferSize > 0);
		}
		return false;
	}

	int HardWareVertexBuffer::BufferType()
	{
		return this->m_bufferType;
	}

	void* HardWareVertexBuffer::Bind()
	{
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_object);
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if(!pBaseAddress)
			{
				CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
				pBaseAddress = (unsigned char*)fileCacheMgr->getMapView(0, m_object, m_bufferSize);
				pBaseAddress += m_bufferOffset;
			}
			return pBaseAddress;
		}
		return NULL;
	}

	void HardWareVertexBuffer::UnBind()
	{
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if (pBaseAddress) {
				CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
				fileCacheMgr->unMapVew(pBaseAddress);
				pBaseAddress = NULL;
			}
		}
	}

	void HardWareVertexBuffer::SetNormalOffset(M3D_OFFSET_TYPE offset)
	{
		this->m_normalOffset = offset;
	}

	M3D_OFFSET_TYPE HardWareVertexBuffer::GetNormalOffset()
	{
		return this->m_normalOffset;
	}

	void HardWareVertexBuffer::SetVertexOffset(M3D_OFFSET_TYPE offset)
	{
		this->m_vertexOffset = offset;
	}

	M3D_OFFSET_TYPE HardWareVertexBuffer::GetVertexOffset()
	{
		return this->m_vertexOffset;
	}

	void HardWareVertexBuffer::SetTextureCoordsOffset(M3D_OFFSET_TYPE offset)
	{
		this->m_textureCoordsOffset = offset;
	}

	M3D_OFFSET_TYPE HardWareVertexBuffer::GetTextureCoordsOffset()
	{
		return this->m_textureCoordsOffset;
	}

}

