#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/ResourceManager.h"
#include "m3d/base/FileCacheManager.h"

namespace M3D
{

	HardWareIndexBuffer::HardWareIndexBuffer() :
		GPUObject()
	{
		Init();
	}

	HardWareIndexBuffer::~HardWareIndexBuffer()
	{

		if (m_object)
		{
			if(m_bufferType == GPUObject::GPU_CACHE)
			{
				//glDeleteBuffers(1, &m_object);
				if(this->m_resourceMgr)
				{
					this->m_resourceMgr->AddGLObject(m_object,ResourceManager::VBO);

				}
			}else if(m_bufferType == GPUObject::DISK_CACHE )
			{

			}
		}
	}

	void HardWareIndexBuffer::Init()
	{
		this->SetOffset(0);
		this->pBaseAddress = NULL;
		m_bufferType = GPUObject::NO_CACHE;
	}

	void HardWareIndexBuffer::SetSize(unsigned bufferSize, bool dynamic)
	{
		this->m_bufferSize = bufferSize;
		this->m_dynamic = dynamic;
	}

	void HardWareIndexBuffer::SetData(const void* data)
	{

	}

	bool HardWareIndexBuffer::SetDataRange(const void* data, unsigned start,
		unsigned count, bool discard)
	{
		bool setState = false;
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			if (m_object)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, start, count, data);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				setState = true;
			}
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if(pBaseAddress)
			{
				//将数据拷贝到内存映射区域中
				memcpy(pBaseAddress+start,data,count);
			}
		}
 
		return setState;
	}

	bool HardWareIndexBuffer::Create(int bufferType)
	{
		//调用OpenGL api 在显存上分配空间
		bool createState = true;
		if(bufferType == GPUObject::GPU_CACHE)
		{
			//调用OpenGL api 在显存上分配空间
			glGenBuffers(1, &m_object);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object);

			if(pBaseAddress)
			{
				//预留空间，共每一次设置内存时使用
				pBaseAddress = new unsigned char[m_bufferSize];
			}
			if(this->m_dynamic)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferSize, pBaseAddress, GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bufferSize, pBaseAddress, GL_STATIC_DRAW);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//对显存分配空间结果检测
			if (glGetError() == GL_OUT_OF_MEMORY)
			{
				createState = false;
				LOGE("HardWareIndexBuffer::Create() ERROR! 显存空间不足");
			}
		}else if(bufferType == GPUObject::DISK_CACHE) //使用硬盘缓存
		{
			if(!pBaseAddress)
			{
				//预留空间，共每一次设置内存时使用
				pBaseAddress = new unsigned char[m_bufferSize];
			}
		}
		this->m_bufferType = bufferType;
		return createState;
	}

	void* HardWareIndexBuffer::Bind()
	{
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_object);
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
			pBaseAddress = (unsigned char*)fileCacheMgr->getMapView(0, m_object, m_bufferSize);
			pBaseAddress += m_bufferOffset;

			return pBaseAddress;
		}

		return NULL;
	}

	void HardWareIndexBuffer::UnBind()
	{
		if(m_bufferType == GPUObject::GPU_CACHE)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		}else if(m_bufferType == GPUObject::DISK_CACHE )
		{
			if (pBaseAddress) {
				CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
				fileCacheMgr->unMapVew(pBaseAddress);
				pBaseAddress = NULL;
			}
		}
	}

	void HardWareIndexBuffer::WriteBuffer()
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

	int HardWareIndexBuffer::BufferType()
	{
		return this->m_bufferType;
	}

	bool HardWareIndexBuffer::HasValue()
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
 
	void HardWareIndexBuffer::SetOffset(M3D_OFFSET_TYPE offset)
	{
		this->m_offset = offset;
	}

	M3D_OFFSET_TYPE HardWareIndexBuffer::GetOffset()
	{
		return this->m_offset;
	}

}

