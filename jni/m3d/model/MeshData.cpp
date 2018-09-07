#include "m3d/action/RayPickAction.h"

#include "SView/views/Parameters.h"

#include "m3d/model/MeshData.h"
#include "m3d/model/GeoAttribute.h"

#include "m3d/SceneManager.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/graphics/HardWareVertexBuffer.h"

#include "m3d/utils/MeshTools.h"

namespace M3D
{
VertexSet::VertexSet(void):IVertexSet()
{
//	this->SetType(SHAPE_TRIMESH);
	m_UseIndex = false;
	this->m_geoAttribute = NULL;

	m_hardWareVertexBuffer = NULL;
	m_hardWareIndexBuffer = NULL;

	this->MarkDirty();
}

VertexSet::~VertexSet(void)
{
	if (this->m_geoAttribute != NULL)
	{
		delete this->m_geoAttribute;
		this->m_geoAttribute = NULL;
	}

	this->Clear();

	this->ClearGPUData();
}

void VertexSet::Clear()
{
	this->m_NormalArray.clear();
	this->m_IndexArray.clear();
	this->m_positionArray.clear();
	this->m_textureCoords.clear();

	this->m_linesModeIndexArray.clear();
	this->m_linesModepositionArray.clear();

	this->m_NormalArray.shrink_to_fit();
	this->m_IndexArray.shrink_to_fit();
	this->m_positionArray.shrink_to_fit();
	this->m_textureCoords.shrink_to_fit();

	this->m_linesModeIndexArray.shrink_to_fit();
	this->m_linesModepositionArray.shrink_to_fit();

}

void VertexSet::ClearGPUData()
{
	//TODO，
	if(m_hardWareVertexBuffer)
	{
		m_hardWareVertexBuffer->Release();
		m_hardWareVertexBuffer = NULL;
	}

	if(m_hardWareIndexBuffer)
	{
		m_hardWareIndexBuffer->Release();
		m_hardWareIndexBuffer = NULL;
	}

	this->MarkDirty();
}


int VertexSet::GetVertexCount()
{
    return this->GetPositionArray()->size();
}

BoundingBox& VertexSet::GetBoundingBox()
{
	if (!m_boudingBox.Defined())
	{
		int count = 0;
//		if (this->IsUseIndex())
//		{ //使用索引
//			int indexCount = m_IndexArray.size();
//			count = indexCount;
//			for (int i = 0; i < indexCount; i++)
//			{
//				tempBox.Merge(m_positionArray[m_IndexArray[i]]);
//			}
//		}
//		else
		{ //不使用索引
			count = m_positionArray.size();

			for (int i = 0; i < count; i += 3)
			{
				m_boudingBox.Merge(m_positionArray[i]);
			}
		}
	}

	return m_boudingBox;
}

void VertexSet::RayPick(RayPickAction* action)
{
	//if (action->Intersect(GetBoundingBox()))
	{
		Vector3* triData = m_positionArray.data();
		Vector3 intersectPos;
		for (int i = 0; i < m_positionArray.size() / 3; i++)
		{
			if (action->IsintersectRayAndTriangle(triData[i*3],triData[i*3+1],triData[i*3+2],
					intersectPos))
			{
				action->AddIntersectPnt(intersectPos);
			}
		}
	}
}

GeometryAttribute* VertexSet::GetGeoAttribute()
{
	return this->m_geoAttribute;
}
void VertexSet::SetGeoAttribute(GeometryAttribute* geoAttribute)
{
	this->m_geoAttribute = geoAttribute;
}

void VertexSet::Optimize()
{
//	LOGE("optimize");
//	Vector3 offset (-16136.9, -26312.8, -11748.4);
//	int size = m_positionArray.size();
//	for(int i = 0;i< size;i++)
//	{

//			LOGE("m_positionArray 111 %s",m_positionArray[i].Tostring().c_str());
//			m_positionArray[i] = m_positionArray[i]/1000000 ;//- offset;
//			LOGE("m_positionArray  222%s",m_positionArray[i].Tostring().c_str());
//	}
//	size = m_NormalArray.size();
//		for(int i = 0;i< size;i++)
//		{
//			LOGE("normal %s",m_NormalArray[i].Tostring().c_str());
//			m_NormalArray[i] = -m_NormalArray[i];
//			m_NormalArray[i].Normalize();
//		}

//	m_positionArray;//!< 存储顶点位置
}

HardWareVertexBuffer* VertexSet::GetHardWareVertexBuffer()
{
	return this->m_hardWareVertexBuffer;
}

HardWareIndexBuffer* VertexSet::GetHardWareIndexBuffer()
{
	return this->m_hardWareIndexBuffer;
}

bool VertexSet::IsDirty()
{
	return this->m_dirty;
}

void VertexSet::MarkDirty()
{
	m_dirty = true;
}	

void VertexSet::UpdateHardWareBuffer(SceneManager* sceneMgr)
{
	if (this->m_dirty)
	{
		this->m_dirty = false;
		if (!m_hardWareVertexBuffer && this->m_positionArray.size() > 0)
		{
			//创建VertexBuffer
			m_hardWareVertexBuffer = new HardWareVertexBuffer();
			m_hardWareVertexBuffer->AddRef();

			m_hardWareVertexBuffer->SetResourceManager(sceneMgr->GetResourceManager());
			//////填充数据
			//计算顶点数据需要申请的硬件空间大小
			unsigned bufferSize = 0, positionOffset, normalOffset,
					textureOffset, positionSize, normalSize, textureSize;

			positionSize = m_positionArray.size() * sizeof(Vector3);
			normalSize = m_NormalArray.size() * sizeof(Vector3);
			textureSize = m_textureCoords.size() * sizeof(Vector3);

			//顶点位置1
			positionOffset = 0;
			//顶点法向量
			normalOffset = positionOffset + positionSize;
			//纹理坐标
			textureOffset = normalOffset + normalSize;

			bufferSize = positionSize + normalSize + textureSize;

			m_hardWareVertexBuffer->SetSize(bufferSize);

			int bufferType = SVIEW::Parameters::Instance()->m_bufferType;

			bool createState = m_hardWareVertexBuffer->Create(bufferType);

			//如果创建不成功，释放该buffer
			if (!createState)
			{
				m_hardWareVertexBuffer->Release();
				m_hardWareVertexBuffer = NULL;
			}
			else
			{
				//将内存中的数据拷贝到显存中
				//设置顶点坐标
				m_hardWareVertexBuffer->SetDataRange(m_positionArray.data(),
						positionOffset, positionSize);
				//设置法线
				m_hardWareVertexBuffer->SetDataRange(m_NormalArray.data(),
						normalOffset, normalSize);
				//设置纹理坐标
				m_hardWareVertexBuffer->SetDataRange(m_textureCoords.data(),
						textureOffset, textureSize);

				m_hardWareVertexBuffer->SetVertexOffset(positionOffset);
				m_hardWareVertexBuffer->SetNormalOffset(normalOffset);
				m_hardWareVertexBuffer->SetTextureCoordsOffset(textureOffset);

				m_hardWareVertexBuffer->WriteBuffer();

				if(SVIEW::Parameters::Instance()->m_bufferType == GPUObject::DISK_CACHE)
				{ 
					//vector<Vector3> temp;
					//vector<M3D_INDEX_TYPE> indexTemp;
					this->m_NormalArray.resize(0);
					//this->m_IndexArray.swap(indexTemp);
					this->m_positionArray.resize(0);
					this->m_textureCoords.resize(0);
					this->m_NormalArray.shrink_to_fit();
					//this->m_IndexArray.swap(indexTemp);
					this->m_positionArray.shrink_to_fit();
					this->m_textureCoords.shrink_to_fit();
					//this->m_linesModeIndexArray.swap(indexTemp);
					//this->m_linesModepositionArray.swap(temp);
					//Clear();
				}
			}
		}

		if (!m_hardWareIndexBuffer && this->IsUseIndex())
		{
			//创建Indexbuffer
			m_hardWareIndexBuffer = new HardWareIndexBuffer();
			m_hardWareIndexBuffer->AddRef();

			m_hardWareIndexBuffer->SetResourceManager(sceneMgr->GetResourceManager());
			//////填充数据
			//计算索引数据需要申请的硬件空间大小
			unsigned bufferSize = 0;
			//索引
			bufferSize += m_IndexArray.size()*sizeof(M3D_INDEX_TYPE);

			m_hardWareIndexBuffer->SetSize(bufferSize);

			int bufferType = SVIEW::Parameters::Instance()->m_bufferType;

			bool createState = m_hardWareIndexBuffer->Create(bufferType);

			if(!createState)
			{
				m_hardWareIndexBuffer->Release();
				m_hardWareIndexBuffer = NULL;
			}
			else
			{
				unsigned indexOffset = 0;
				//将内存中的数据拷贝到显存中
				m_hardWareIndexBuffer->SetDataRange(m_IndexArray.data(),indexOffset,bufferSize);
				m_hardWareIndexBuffer->SetOffset(indexOffset);

				m_hardWareIndexBuffer->WriteBuffer();
				if(SVIEW::Parameters::Instance()->m_bufferType == GPUObject::DISK_CACHE)
				{ 
					//vector<Vector3> temp;
					//vector<M3D_INDEX_TYPE> indexTemp;
 
					this->m_IndexArray.resize(0);
					this->m_IndexArray.shrink_to_fit();
					//this->m_linesModeIndexArray.swap(indexTemp);
					//this->m_linesModepositionArray.swap(temp);
					//Clear();
				}
			}
		}
	}
}

void VertexSet::UpdateHardWareBuffer(SceneManager* sceneMgr, int bufferType)
{
	this->m_dirty = false;
	if (!m_hardWareVertexBuffer && this->m_positionArray.size() > 0)
	{
		//创建VertexBuffer
		m_hardWareVertexBuffer = new HardWareVertexBuffer();
		m_hardWareVertexBuffer->AddRef();

		m_hardWareVertexBuffer->SetResourceManager(sceneMgr->GetResourceManager());
		//////填充数据
		//计算顶点数据需要申请的硬件空间大小
		unsigned bufferSize = 0, positionOffset, normalOffset,
			textureOffset, positionSize, normalSize, textureSize;

		positionSize = m_positionArray.size() * sizeof(Vector3);
		normalSize = m_NormalArray.size() * sizeof(Vector3);
		textureSize = m_textureCoords.size() * sizeof(Vector3);

		//顶点位置1
		positionOffset = 0;
		//顶点法向量
		normalOffset = positionOffset + positionSize;
		//纹理坐标
		textureOffset = normalOffset + normalSize;

		bufferSize = positionSize + normalSize + textureSize;

		m_hardWareVertexBuffer->SetSize(bufferSize);
 
		bool createState = m_hardWareVertexBuffer->Create(bufferType);

		//如果创建不成功，释放该buffer
		if (!createState)
		{
			m_hardWareVertexBuffer->Release();
			m_hardWareVertexBuffer = NULL;
		}
		else
		{
			//将内存中的数据拷贝到显存中
			//设置顶点坐标
			m_hardWareVertexBuffer->SetDataRange(m_positionArray.data(),
				positionOffset, positionSize);
			//设置法线
			m_hardWareVertexBuffer->SetDataRange(m_NormalArray.data(),
				normalOffset, normalSize);
			//设置纹理坐标
			m_hardWareVertexBuffer->SetDataRange(m_textureCoords.data(),
				textureOffset, textureSize);

			m_hardWareVertexBuffer->SetVertexOffset(positionOffset);
			m_hardWareVertexBuffer->SetNormalOffset(normalOffset);
			m_hardWareVertexBuffer->SetTextureCoordsOffset(textureOffset);

			m_hardWareVertexBuffer->WriteBuffer();

			if (bufferType == GPUObject::DISK_CACHE)
			{
				//vector<Vector3> temp;
				//vector<M3D_INDEX_TYPE> indexTemp;
				this->m_NormalArray.resize(0);
				//this->m_IndexArray.swap(indexTemp);
				this->m_positionArray.resize(0);
				this->m_textureCoords.resize(0);
				this->m_NormalArray.shrink_to_fit();
				//this->m_IndexArray.swap(indexTemp);
				this->m_positionArray.shrink_to_fit();
				this->m_textureCoords.shrink_to_fit();
				//this->m_linesModeIndexArray.swap(indexTemp);
				//this->m_linesModepositionArray.swap(temp);
				//Clear();
			}
		}
	}

	if (!m_hardWareIndexBuffer && this->IsUseIndex())
	{
		//创建Indexbuffer
		m_hardWareIndexBuffer = new HardWareIndexBuffer();
		m_hardWareIndexBuffer->AddRef();

		m_hardWareIndexBuffer->SetResourceManager(sceneMgr->GetResourceManager());
		//////填充数据
		//计算索引数据需要申请的硬件空间大小
		unsigned bufferSize = 0;
		//索引
		bufferSize += m_IndexArray.size() * sizeof(M3D_INDEX_TYPE);

		m_hardWareIndexBuffer->SetSize(bufferSize);

		bool createState = m_hardWareIndexBuffer->Create(bufferType);

		if (!createState)
		{
			m_hardWareIndexBuffer->Release();
			m_hardWareIndexBuffer = NULL;
		}
		else
		{
			unsigned indexOffset = 0;
			//将内存中的数据拷贝到显存中
			m_hardWareIndexBuffer->SetDataRange(m_IndexArray.data(), indexOffset, bufferSize);
			m_hardWareIndexBuffer->SetOffset(indexOffset);

			m_hardWareIndexBuffer->WriteBuffer();
			if (bufferType == GPUObject::DISK_CACHE)
			{
				//vector<Vector3> temp;
				//vector<M3D_INDEX_TYPE> indexTemp;

				this->m_IndexArray.resize(0);
				this->m_IndexArray.shrink_to_fit();
				//this->m_linesModeIndexArray.swap(indexTemp);
				//this->m_linesModepositionArray.swap(temp);
				//Clear();
			}
		}
	}
}

void VertexSet::UnCacheFromDisk(SceneManager* sceneMgr)
{
	if (m_hardWareVertexBuffer && this->m_positionArray.size() == 0)
	{
		if (m_hardWareVertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			HardWareVertexBuffer* vertexBuffer = this->GetHardWareVertexBuffer();
			Vector3* allpntArray = (Vector3*)vertexBuffer->Bind();

			//求出所有点的个数
			long ontvertexSize = sizeof(Vector3);
			long allVertexCount = m_hardWareVertexBuffer->GetBufferSize() / ontvertexSize;

			unsigned positionOffset = m_hardWareVertexBuffer->GetVertexOffset();
			unsigned normalOffset = m_hardWareVertexBuffer->GetNormalOffset();
			unsigned textureOffset = m_hardWareVertexBuffer->GetTextureCoordsOffset();

			long  textureSize = allVertexCount - textureOffset/ ontvertexSize;
			long  normalSize = (textureOffset - normalOffset) / ontvertexSize;
			long  positionSize = (normalOffset - positionOffset)/ontvertexSize; 

			vector<Vector3>* pntsArray = this->GetPositionArray();
			for (int i = 0; i < positionSize; i++)
			{
				pntsArray->push_back(*(allpntArray + i));
			}

			vector<Vector3>* normalArray = this->GetNormalArray();
			for (int i = positionSize; i < normalSize + positionSize; i++)
			{
				normalArray->push_back(*(allpntArray + i));
			}

			vector<Vector3>* textArray = this->GetTextureCoordArray();
			for (int i = normalSize + positionSize; i < normalSize + positionSize + textureSize; i++)
			{
				textArray->push_back(*(allpntArray + i));
			}
			//将磁盘缓存对应的buffer释放掉

 
			m_hardWareVertexBuffer->UnBind();
		 
			m_hardWareVertexBuffer->Release();
			m_hardWareVertexBuffer = NULL;

			bool useIndex = this->IsUseIndex();
			if (useIndex)
			{ //使用索引
				 
				if (m_hardWareIndexBuffer && m_hardWareIndexBuffer->BufferType() == GPUObject::DISK_CACHE)
				{
					M3D_INDEX_TYPE* allIndexdata = (M3D_INDEX_TYPE*)m_hardWareIndexBuffer->Bind();
					long indexbufferSize = m_hardWareIndexBuffer->GetBufferSize()/sizeof(M3D_INDEX_TYPE);

					vector<M3D_INDEX_TYPE>* indexArray = this->GetIndexArray();

					for (int i = 0; i < indexbufferSize; i++)
					{
						indexArray->push_back(*(allIndexdata + i));
					}

					m_hardWareIndexBuffer->UnBind();

					m_hardWareIndexBuffer->Release();
					m_hardWareIndexBuffer = NULL;
				}
			}


			//创建VertexBuffer
			//通过正常的流程创建内存缓存中的buffer
			this->UpdateHardWareBuffer(sceneMgr, GPUObject::NO_CACHE);
		}
	}
}


float VertexSet::GetVolumeAndArea(float& volume,float& area)
{
	double air = 0.;
	double vol = 0.;
	double S;
	double V;
	MTools::Vtx pnt0;
	MTools::Vtx pnt1;
	MTools::Vtx pnt2;

	//if (action->Intersect(GetBoundingBox()))

	Vector3 intersectPos;
	bool useIndex = this->IsUseIndex();
	int count = 0;

	if (useIndex)
	{ //使用索引
		int indexCount = this->m_IndexArray.size();
		count = indexCount;

		HardWareVertexBuffer* vertexBuffer = this->GetHardWareVertexBuffer();
		Vector3* pntArray = NULL;
		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			Vector3* vertexData = (Vector3*) vertexBuffer->Bind();
			pntArray = vertexData;
		}
		else
		{
			pntArray = this->GetPositionArray()->data();
		}

		HardWareIndexBuffer* indexBuffer = this->GetHardWareIndexBuffer();
		M3D_INDEX_TYPE* tempIndex = NULL;
		if (indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			M3D_INDEX_TYPE* vertexData = (M3D_INDEX_TYPE*) indexBuffer->Bind();
			tempIndex = vertexData;
		}
		else
		{
			tempIndex = this->GetIndexArray()->data();
		}

		int v0, v1, v2;

		for (int i = 0; i < indexCount / 3; i++)
		{
			v0 = tempIndex[i * 3];
			v1 = tempIndex[i * 3 + 1];
			v2 = tempIndex[i * 3 + 2];

			Vector3& mPnt0 = pntArray[v0];
			Vector3& mPnt1 = pntArray[v1];
			Vector3& mPnt2 = pntArray[v2];

			pnt0.x = mPnt0.m_x;
			pnt0.y = mPnt0.m_y;
			pnt0.z = mPnt0.m_z;
			pnt1.x = mPnt1.m_x;
			pnt1.y = mPnt1.m_y;
			pnt1.z = mPnt1.m_z;
			pnt2.x = mPnt2.m_x;
			pnt2.y = mPnt2.m_y;
			pnt2.z = mPnt2.m_z;

			MTools::MeshTools::CalculateAirVol(pnt0, pnt1, pnt2, S, V);
			if (abs(S) < 1.e-10)
			{
				printf(" **** Element NULL  i = %d   S = %f\n", i, S);
			}

			air += S;
			vol += V;
		}

		if (indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			indexBuffer->UnBind();
		}

		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			vertexBuffer->UnBind();
		}

	}
	else
	{ //不使用索引
		count = this->m_positionArray.size();

		HardWareVertexBuffer* vertexBuffer = this->GetHardWareVertexBuffer();
		Vector3* triData = NULL;
		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			Vector3* vertexData = (Vector3*) vertexBuffer->Bind();
			triData = vertexData;
		}
		else
		{
			triData = this->GetPositionArray()->data();
		}

		for (int i = 0; i < count; i += 3)
		{
			Vector3& mPnt0 = triData[i];
			Vector3& mPnt1 = triData[i + 1];
			Vector3& mPnt2 = triData[i + 2];

			pnt0.x = mPnt0.m_x;
			pnt0.y = mPnt0.m_y;
			pnt0.z = mPnt0.m_z;
			pnt1.x = mPnt1.m_x;
			pnt1.y = mPnt1.m_y;
			pnt1.z = mPnt1.m_z;
			pnt2.x = mPnt2.m_x;
			pnt2.y = mPnt2.m_y;
			pnt2.z = mPnt2.m_z;

			MTools::MeshTools::CalculateAirVol(pnt0, pnt1, pnt2, S, V);
			if (abs(S) < 1.e-10)
			{
				printf(" **** Element NULL  i = %d   S = %f\n", i, S);
			}

			air += S;
			vol += V;
		}

		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			vertexBuffer->UnBind();
		}
	}

	double cap = 0.;
	if (air > 1.e-16)
	{
		cap = vol / air;
	}

	volume = vol;
	area = air;

	return cap;
}

void VertexSet::RecalculateNormal()
{
	if (IsUseIndex())
	{				
		m_NormalArray.clear();
		int positionSize = m_positionArray.size();
		m_NormalArray.resize(positionSize);
		int indexSize = this->m_IndexArray.size();
		for (int i = 0;i<indexSize-2;i=i+3)
		{
			Vector3 &p0 = m_positionArray[m_IndexArray[i+0]];
			Vector3 &p1 = m_positionArray[m_IndexArray[i+1]];
			Vector3 &p2 = m_positionArray[m_IndexArray[i+2]];
			Vector3 p0p1 = p1 - p0;
			Vector3 p0p2 = p2 - p0;
			Vector3 faceNormal = (p0p1.CrossProduct(p0p2)).Normalized();
			m_NormalArray[m_IndexArray[i + 0]] += faceNormal;
			m_NormalArray[m_IndexArray[i + 1]] += faceNormal;
			m_NormalArray[m_IndexArray[i + 2]] += faceNormal;
		}

		//单位化
		for (int j = 0;j<positionSize;j++)
		{
			m_NormalArray[j].Normalize();
		}
	}
	else
	{
		m_NormalArray.clear();
		int positionSize = m_positionArray.size();
		m_NormalArray.resize(positionSize);
		for (int i = 0; i < positionSize-2; i=i+3)
		{
			Vector3 &p0 = m_positionArray[i];
			Vector3 &p1 = m_positionArray[i + 1];
			Vector3 &p2 = m_positionArray[i + 2];
			Vector3 p0p1 = p1 - p0;
			Vector3 p0p2 = p2 - p0;
			Vector3 faceNormal = (p0p1.CrossProduct(p0p2)).Normalized();
			m_NormalArray[i + 0] = faceNormal;
			m_NormalArray[i + 1] = faceNormal;
			m_NormalArray[i + 2] = faceNormal;
		}

		//单位化
		for (int j = 0; j<positionSize; j++)
		{
			m_NormalArray[j].Normalize();
		}
		
	}
}

vector<Vector3>* VertexSet::GetPositionArray()
{
	return &this->m_positionArray;
}

vector<Vector3>* VertexSet::GetNormalArray()
{
	return &this->m_NormalArray;
}

vector<Vector3>* VertexSet::GetTextureCoordArray()
{
	return &this->m_textureCoords;
}

vector<M3D_INDEX_TYPE>* VertexSet::GetIndexArray()
{
	return &this->m_IndexArray;
}

vector<Vector3>* VertexSet::GetLinesModePointArray()
{
	if(this->m_linesModepositionArray.empty() && !this->m_UseIndex)
	{
	//std::set_new_handler(VertexSet::no_memory);

		int triSize = m_positionArray.size()/3;
		this->m_linesModepositionArray.reserve(triSize*6); 

		//每次添加三角形的三条边
		for(int i =0;i < triSize;i++)
		{
			//第一条边
			m_linesModepositionArray.push_back(m_positionArray[i*3]);
			m_linesModepositionArray.push_back(m_positionArray[i*3 + 1]);

			//第二条边
			m_linesModepositionArray.push_back(m_positionArray[i*3 + 1]);
			m_linesModepositionArray.push_back(m_positionArray[i*3 + 2]);

			//第三条边
			m_linesModepositionArray.push_back(m_positionArray[i*3 + 2]);
			m_linesModepositionArray.push_back(m_positionArray[i*3]);
		}
	}

	return &(this->m_linesModepositionArray);
}

vector<M3D_INDEX_TYPE>* VertexSet::GetLinesModeIndexArray()
{
	if(this->m_linesModeIndexArray.empty() && this->m_UseIndex)
	{
		int triSize = m_IndexArray.size()/3;

//		std::set_new_handler(VertexSet::no_memory);

		this->m_linesModeIndexArray.reserve(triSize*6);
		//每次添加三角形的三条边
		for(int i =0;i < triSize;i++)
		{
			//第一条边
			m_linesModeIndexArray.push_back(m_IndexArray[i*3]);
			m_linesModeIndexArray.push_back(m_IndexArray[i*3 + 1]);

			//第二条边
			m_linesModeIndexArray.push_back(m_IndexArray[i*3 + 1]);
			m_linesModeIndexArray.push_back(m_IndexArray[i*3 + 2]);

			//第三条边
			m_linesModeIndexArray.push_back(m_IndexArray[i*3 + 2]);
			m_linesModeIndexArray.push_back(m_IndexArray[i*3]);
		}
	}
	return &(this->m_linesModeIndexArray);
}

void VertexSet::SetUseIndex(bool useIndex)
{
	this->m_UseIndex = useIndex;
}

bool VertexSet::IsUseIndex()
{
	return this->m_UseIndex;
}

void VertexSet::no_memory()
{
	LOGE("MeshData::no_memory(): Error Memory Low !!!");
}

Mesh::Mesh(VertexSet* refMesh):IVertexSet()
{
	m_geoAttribute = NULL;

	this->m_refMesh = refMesh;
    if(m_refMesh){
        this->m_refMesh->AddRef();
    }
}
Mesh::~Mesh()
{
	if(m_refMesh)
	m_refMesh->Release();
    m_refMesh = NULL;

	this->Clear();
}

///Shape virtual fun
BoundingBox& Mesh::GetBoundingBox()
{
	if (!m_boudingBox.Defined() && this->m_refMesh)
	{
		int count = 0;
		bool useIndex = this->m_refMesh->IsUseIndex();
		if (useIndex&&this->m_refMesh->GetIndexArray()->size()>0)
		{ //使用索引
			int indexCount = m_dataLength;
			count = indexCount;
			Vector3* pntArray = this->m_refMesh->GetPositionArray()->data();
			M3D_INDEX_TYPE* tempIndex = this->m_refMesh->GetIndexArray()->data()
					+ this->m_dataOffset;
			for (int i = 0; i < indexCount; i++)
			{
				m_boudingBox.Merge(pntArray[tempIndex[i]]);
			}
		}
		else
		{ //不使用索引
			if (this->m_refMesh->GetPositionArray()->size()>0)
			{
				count = m_dataLength;
				Vector3* tempPoss = this->m_refMesh->GetPositionArray()->data()
					+ this->m_dataOffset;
				for (int i = 0; i < count; i++)
				{
					m_boudingBox.Merge(tempPoss[i]);
				}
			}
		}
	}

	return m_boudingBox;
}
void Mesh::RayPick(RayPickAction* action)
{
	//if (action->Intersect(GetBoundingBox()))
	{
		Vector3 intersectPos;
		bool useIndex = this->m_refMesh->IsUseIndex();
		int count = 0;
 
		if (useIndex)
		{ //使用索引
			int indexCount = this->m_dataLength;
			count = indexCount;

			HardWareVertexBuffer* vertexBuffer = this->m_refMesh->GetHardWareVertexBuffer();
			Vector3* pntArray = NULL;
			if(vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				Vector3* vertexData = (Vector3*)vertexBuffer->Bind();
				pntArray = vertexData;
			}else
			{
				pntArray = this->m_refMesh->GetPositionArray()->data();
			}

			HardWareIndexBuffer* indexBuffer = this->m_refMesh->GetHardWareIndexBuffer();
			M3D_INDEX_TYPE* tempIndex = NULL;
			if(indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				M3D_INDEX_TYPE* vertexData = (M3D_INDEX_TYPE*)indexBuffer->Bind();
				tempIndex = vertexData + this->m_dataOffset;
			}else
			{
				tempIndex = this->m_refMesh->GetIndexArray()->data()
					+ this->m_dataOffset;
			}
 
			int v0,v1,v2;

			for (int i = 0; i < indexCount/3; i++)
			{
				v0 = tempIndex[i*3];
			    v1 = tempIndex[i*3 +1];
			    v2 = tempIndex[i*3 +2];
				if (action->IsintersectRayAndTriangle(pntArray[v0],pntArray[v1],pntArray[v2],intersectPos))
				{
					action->AddIntersectPnt(intersectPos);
				}
			}

			if(indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				indexBuffer->UnBind();
			}	

			if(vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				vertexBuffer->UnBind();
			}

		}
		else
		{ //不使用索引
			count = this->m_dataLength;

			HardWareVertexBuffer* vertexBuffer = this->m_refMesh->GetHardWareVertexBuffer();
			Vector3* triData = NULL;
			if(vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				Vector3* vertexData = (Vector3*)vertexBuffer->Bind();
				triData = vertexData + this->m_dataOffset;
			}else
			{
				triData = this->m_refMesh->GetPositionArray()->data()
					+ this->m_dataOffset;
			}

			for (int i = 0; i < count / 3; i++)
			{
				if (action->IsintersectRayAndTriangle(triData[i*3],triData[i*3+1],triData[i*3+2],
						intersectPos))
				{
					action->AddIntersectPnt(intersectPos);
				}
			}

			if(vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
			{
				vertexBuffer->UnBind();
			}
		}
	}
}
///IMeshData virtual fun
void Mesh::Clear()
{
	if(m_geoAttribute)
	{
		delete m_geoAttribute;
		m_geoAttribute = NULL;
	}
    m_dataOffset = 0; ///数据相对于refMeshData的偏移量
    m_dataLength = 0; ///数据总长度
 
}
int Mesh::GetVertexCount()
{
	return this->m_dataLength;
}
GeometryAttribute* Mesh::GetGeoAttribute()
{
	return this->m_geoAttribute;
}
void Mesh::SetGeoAttribute(GeometryAttribute* geoAttribute)
{
	this->m_geoAttribute = geoAttribute;
}

void Mesh::Optimize()
{

}

//void Mesh::SetRefMesh(VertexSet* refMesh)
//{
//	this->m_refMesh = refMesh;
//}

VertexSet* Mesh::GetRefMesh()
{
	return this->m_refMesh;
}

void Mesh::SetDataLength(unsigned int  length)
{
	this->m_dataLength = length;
}
unsigned int  Mesh::GetDataLength()
{
	return this->m_dataLength;
}

void Mesh::SetDataOffset(unsigned int  offset)
{
	this->m_dataOffset = offset;
}
unsigned int  Mesh::GetDataOffset()
{
	return this->m_dataOffset;
}
       
float Mesh::GetVolumeAndArea(float& volume,float& area)
{
	double air = 0.;
	double vol = 0.;
	double S;
	double V;
	MTools::Vtx pnt0;
	MTools::Vtx pnt1;
	MTools::Vtx pnt2;

	//if (action->Intersect(GetBoundingBox()))

	Vector3 intersectPos;
	bool useIndex = this->m_refMesh->IsUseIndex();
	int count = 0;

	if (useIndex)
	{ //使用索引
		int indexCount = this->m_dataLength;
		count = indexCount;

		HardWareVertexBuffer* vertexBuffer =
				this->m_refMesh->GetHardWareVertexBuffer();
		Vector3* pntArray = NULL;
		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			Vector3* vertexData = (Vector3*) vertexBuffer->Bind();
			pntArray = vertexData;
		}
		else
		{
			pntArray = this->m_refMesh->GetPositionArray()->data();
		}

		HardWareIndexBuffer* indexBuffer =
				this->m_refMesh->GetHardWareIndexBuffer();
		M3D_INDEX_TYPE* tempIndex = NULL;
		if (indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			M3D_INDEX_TYPE* vertexData = (M3D_INDEX_TYPE*) indexBuffer->Bind();
			tempIndex = vertexData + this->m_dataOffset;
		}
		else
		{
			tempIndex = this->m_refMesh->GetIndexArray()->data()
					+ this->m_dataOffset;
		}

		int v0, v1, v2;

		for (int i = 0; i < indexCount / 3; i++)
		{
			v0 = tempIndex[i * 3];
			v1 = tempIndex[i * 3 + 1];
			v2 = tempIndex[i * 3 + 2];

			Vector3& mPnt0 = pntArray[v0];
			Vector3& mPnt1 = pntArray[v1];
			Vector3& mPnt2 = pntArray[v2];

			pnt0.x = mPnt0.m_x;
			pnt0.y = mPnt0.m_y;
			pnt0.z = mPnt0.m_z;
			pnt1.x = mPnt1.m_x;
			pnt1.y = mPnt1.m_y;
			pnt1.z = mPnt1.m_z;
			pnt2.x = mPnt2.m_x;
			pnt2.y = mPnt2.m_y;
			pnt2.z = mPnt2.m_z;

			MTools::MeshTools::CalculateAirVol(pnt0, pnt1, pnt2, S, V);
			if (abs(S) < 1.e-10)
			{
				printf(" **** Element NULL  i = %d   S = %f\n", i, S);
			}

			air += S;
			vol += V;
		}

		if (indexBuffer && indexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			indexBuffer->UnBind();
		}

		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			vertexBuffer->UnBind();
		}

	}
	else
	{ //不使用索引
		count = this->m_dataLength;

		HardWareVertexBuffer* vertexBuffer =
				this->m_refMesh->GetHardWareVertexBuffer();
		Vector3* triData = NULL;
		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			Vector3* vertexData = (Vector3*) vertexBuffer->Bind();
			triData = vertexData + this->m_dataOffset;
		}
		else
		{
			triData = this->m_refMesh->GetPositionArray()->data()
					+ this->m_dataOffset;
		}

		for (int i = 0; i < count; i += 3)
		{
			Vector3& mPnt0 = triData[i];
			Vector3& mPnt1 = triData[i + 1];
			Vector3& mPnt2 = triData[i + 2];

			pnt0.x = mPnt0.m_x;
			pnt0.y = mPnt0.m_y;
			pnt0.z = mPnt0.m_z;
			pnt1.x = mPnt1.m_x;
			pnt1.y = mPnt1.m_y;
			pnt1.z = mPnt1.m_z;
			pnt2.x = mPnt2.m_x;
			pnt2.y = mPnt2.m_y;
			pnt2.z = mPnt2.m_z;

			MTools::MeshTools::CalculateAirVol(pnt0, pnt1, pnt2, S, V);
			if (abs(S) < 1.e-10)
			{
				printf(" **** Element NULL  i = %d   S = %f\n", i, S);
			}

			air += S;
			vol += V;
		}

		if (vertexBuffer && vertexBuffer->BufferType() == GPUObject::DISK_CACHE)
		{
			vertexBuffer->UnBind();
		}
	}

	double cap = 0.;
	if (air > 1.e-16)
	{
		cap = vol / air;
	}

	volume = vol;
	area = air;

	return cap;
}
}

