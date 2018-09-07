#include "m3d/graphics/Renderable.h"

#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/MeshData.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/graphics/Material.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/graphics/HardWareVertexBuffer.h"

#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Note.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/PMIData.h"

#include "m3d/RenderManager.h"

namespace M3D
{
void Renderable::SetRenderWorldMatrix(Matrix4* worldMatrix)
{
	this->m_renderWorldMatrix = worldMatrix;
}

Matrix4* Renderable::GetRenderWorldMatrix()
{
	return this->m_renderWorldMatrix;
}

Renderable::Renderable()
{
	this->Init();
}
Renderable::~Renderable()
{

}

const Color& Renderable::GetRenderColor()
{
	return *Color::Default;
}

/**
 * 获取材质
 * @return
 */
BaseMaterial*  Renderable::GetRenderMaterial()
{
	return NULL;
}

void Renderable::SetRenderMesh(Mesh* mesh)
{
	//当前mesh在RefMesh中的偏移量
	unsigned int dataOffset = mesh->GetDataOffset();
	VertexSet* meshData = mesh->GetRefMesh();
	this->m_useIndex = meshData->IsUseIndex();

	//判断是否在GPU中存储有缓存数据，如果存在顶点缓存则认为创建vbo成功
	m_hardWareVertexBuffer = meshData->GetHardWareVertexBuffer();
	m_hardWareIndexBuffer = meshData->GetHardWareIndexBuffer();

	if (m_hardWareVertexBuffer && m_hardWareVertexBuffer->HasValue())
	{
		if (m_useIndex)
		{
			//如果使用索引，获取索引的缓存
			if(m_hardWareIndexBuffer && m_hardWareIndexBuffer->HasValue())
			{
				//更新硬件位置的偏移量
				m_indexOffset = m_hardWareIndexBuffer->GetOffset()+ dataOffset*sizeof(M3D_INDEX_TYPE);

				m_normalOffset =m_hardWareVertexBuffer->GetNormalOffset();
				m_vertexOffset = m_hardWareVertexBuffer->GetVertexOffset();
				m_textureCoordsOffset = m_hardWareVertexBuffer->GetTextureCoordsOffset();

				//以线框模式显示，仍然使用内存中的数据，和未使用VBO的情况保持一致
				//TODO
				if (RenderManager::isShowTriLine)
				{
					///以线框模式显示的数据
					vector<M3D_INDEX_TYPE>* linesModelIndexArray =
							meshData->GetLinesModeIndexArray();
					this->m_linesIndex = linesModelIndexArray->data()
							+ dataOffset * 2;
					this->m_linesVertexPos = meshData->GetPositionArray()->data();
				}
			}
		}
		else
		{
			//此处数据偏移需要乘上单个数据大小，才能转换到GPU中按字节方式存储的数据
			dataOffset = dataOffset*sizeof(Vector3);
			//更新硬件位置的偏移量
			m_normalOffset = m_hardWareVertexBuffer->GetNormalOffset()+ dataOffset;
			m_vertexOffset = m_hardWareVertexBuffer->GetVertexOffset()+ dataOffset;
			m_textureCoordsOffset = m_hardWareVertexBuffer->GetTextureCoordsOffset()+ dataOffset;

			//TODO
			if (RenderManager::isShowTriLine)
			{
				///以线框模式显示的数据
				vector<Vector3>* linesModelPointArray =
						meshData->GetLinesModePointArray();
				this->m_linesVertexPos =
						meshData->GetLinesModePointArray()->data()
								+ dataOffset * 2;
			}
		}
	}
	else
	{
		if (m_useIndex)
		{
			///使用索引，仅索引数据进行偏移，引用的数据不需要进行偏移
			this->m_indexOffset =(M3D_OFFSET_TYPE)(meshData->GetIndexArray()->data() + dataOffset);
			this->m_vertexOffset =(M3D_OFFSET_TYPE)( meshData->GetPositionArray()->data());
			this->m_normalOffset = (M3D_OFFSET_TYPE)(meshData->GetNormalArray()->data());

			/*m_useIndexVertexNumber = meshData->GetNormalArray()->size();*/

			this->m_textureCoordsOffset = (M3D_OFFSET_TYPE)(meshData->GetTextureCoordArray()->data());
			//TODO
			if (RenderManager::isShowTriLine)
			{
				///以线框模式显示的数据
				vector<M3D_INDEX_TYPE>* linesModelIndexArray =
						meshData->GetLinesModeIndexArray();
				this->m_linesIndex = linesModelIndexArray->data()
						+ dataOffset * 2;
				this->m_linesVertexPos = meshData->GetPositionArray()->data();
			}
		}
		else
		{
			this->m_vertexOffset =(M3D_OFFSET_TYPE)( meshData->GetPositionArray()->data()
					+ dataOffset);
			this->m_normalOffset = (M3D_OFFSET_TYPE)(meshData->GetNormalArray()->data()
					+ dataOffset);
			this->m_textureCoordsOffset =(M3D_OFFSET_TYPE)( meshData->GetTextureCoordArray()->data()
					+ dataOffset);

			//TODO
			if (RenderManager::isShowTriLine)
			{
				///以线框模式显示的数据
				vector<Vector3>* linesModelPointArray =
						meshData->GetLinesModePointArray();
				this->m_linesVertexPos =
						meshData->GetLinesModePointArray()->data()
								+ dataOffset * 2;
			}
		}
	}

	this->m_length = mesh->GetDataLength();
}

void Renderable::SetRenderPolyLine(Edge* edgeRenderData)
{
	RefPolyLine* refEdgeData = edgeRenderData->GetLineData();
	SPolyLine* edgeData = refEdgeData->GetRefLine();

	unsigned int dataOffset = refEdgeData->GetDataOffset();

	this->m_vertexPos = edgeData->GetPoints().data() + dataOffset;
//	this->m_normalBegin = meshData->GetNormalArray()->data()  +dataOffset;
//	this->m_indexBegin =meshData->GetIndexArray()->data() + dataOffset;
//	this->m_useIndex = edgeData-();
	this->m_length = refEdgeData->GetDataLength();
}

void Renderable::MergeRenderMesh(Mesh* mesh)
{
//	Mesh* faceMesh = (Mesh*) (faceRenderData->GetRenderMeshData());
	this->m_length += mesh->GetDataLength();
}

void Renderable::MergeRenderPolyLine(Edge* edgeRenderData)
{
	RefPolyLine* refEdgeData = edgeRenderData->GetLineData();
	this->m_length += refEdgeData->GetDataLength();
}

Vector3* Renderable::GetVertexPos()
{
	return this->m_vertexPos;
}

Vector3* Renderable::GetVertexNormal()
{
	return this->m_vertexNormal;
}

Vector3* Renderable::GetTextureCoords()
{
	return this->m_textureCoords;
}

M3D_INDEX_TYPE* Renderable::GetIndex()
{
	return this->m_index;
}

Vector3* Renderable::GetLinesModeVertexPos()
{
	return this->m_linesVertexPos;
}

M3D_INDEX_TYPE* Renderable::GetLineModeIndex()
{
	return this->m_linesIndex;
}

int Renderable::GetDataLength()
{
	return this->m_length;
}
bool Renderable::IsUseIndex()
{
	return this->m_useIndex;
}

bool Renderable::IsFrontShow()
{
	return this->m_frontShow;
}

void Renderable::SetFrontShow(bool frontShow)
{
	this->m_frontShow = frontShow;
}

void  Renderable::Init()
{
	m_renderWorldMatrix = NULL;
	m_vertexPos = NULL;
	m_vertexNormal = NULL;
	m_textureCoords = NULL;
	m_index = NULL;
	m_linesVertexPos = NULL;

	m_useIndex = false;
	m_length = 0;
	m_frontShow = false;

	m_normalOffset = 0;
	m_vertexOffset = 0;
	m_textureCoordsOffset = 0;
	this->m_indexOffset = 0;

	m_hardWareVertexBuffer = NULL;
	m_hardWareIndexBuffer = NULL;
	m_recesiveShadow = true;
}

bool Renderable::HardwarebufferValid(Mesh* mesh)
{



	return false;
}


Vector3 *  Renderable::GetNewTextureCooords(int size)
{
	Vector3 * ret = NULL;
// 	if (size > m_newTextureCoords.size())
// 	{
// 		//m_newTextureCoords.clear();
// 		m_newTextureCoords.resize(size);
// 	}
// 	if (m_newTextureCoords.size() > 0)
// 		ret = &m_newTextureCoords[0];

	return ret;

}

int Renderable::GetUseIndexVertexNumber()
{
	/*return m_useIndexVertexNumber;*/
	return 0;
}

void Renderable::SetNormalOffset(M3D_OFFSET_TYPE offset)
{
	this->m_normalOffset = offset;
}

M3D_OFFSET_TYPE Renderable::GetNormalOffset()
{
	return this->m_normalOffset;
}

void Renderable::SetVertexOffset(M3D_OFFSET_TYPE offset)
{
	this->m_vertexOffset = offset;
}

M3D_OFFSET_TYPE Renderable::GetVertexOffset()
{
	return this->m_vertexOffset;
}

void Renderable::SetTextureCoordsOffset(M3D_OFFSET_TYPE offset)
{
	this->m_textureCoordsOffset = offset;
}

M3D_OFFSET_TYPE Renderable::GetTextureCoordsOffset()
{
	return this->m_textureCoordsOffset;
}

void Renderable::SetIndexOffset(M3D_OFFSET_TYPE offset)
{
	this->m_indexOffset = offset;
}

M3D_OFFSET_TYPE Renderable::GetIndexOffset()
{
	return this->m_indexOffset;
}

HardWareVertexBuffer* Renderable::GetHardWareVertexBuffer()
{
	return this->m_hardWareVertexBuffer;
}

HardWareIndexBuffer* Renderable::GetHardWareIndexBuffer()
{
	return this->m_hardWareIndexBuffer;
}

}
