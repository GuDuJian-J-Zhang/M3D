#include "m3d/model/LODData.h"
#include "m3d/model/MeshData.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{

int LOD::LODMAX = 0;

int LOD::LODMIN = 10;

LOD::LOD()
{
	this->m_data.resize(1,NULL);
	this->m_data.shrink_to_fit();
}

LOD::~LOD()
{
	//析构时，解除引用
	for (int i = 0; i < m_data.size(); i++)
	{
		IVertexSet* vertex = m_data.at(i);
		if(vertex)
		vertex->Release();
	}

	this->m_data.clear();
}


LOD::LOD(const LOD& orig)
{
	*this = orig;
}

LOD& LOD::operator =(const LOD& orig)
{
	if (this != &orig)
	{
		m_data = orig.m_data;//!<存储lod数据

			//显示数据添加引用
		for (int i = 0; i < m_data.size(); i++)
		{
			IVertexSet* vertex = m_data.at(i);
			if(vertex)
			vertex->AddRef();
		}
	}
	return *this;
}

IVertexSet* LOD::GetData(int lodLevel)
{
	IVertexSet* data = NULL;
	if (lodLevel < this->GetCount() && lodLevel >= 0)
	{
		data = this->m_data.at(lodLevel);
	}
	return data;
}

void LOD::AddData(IVertexSet* meshData, int lodLevel)
{
	if (lodLevel < this->GetCount() && lodLevel >= 0)
	{
		this->m_data.at(lodLevel) = meshData;
	}
	else
	{
		vector<IVertexSet*> tempList = m_data;

		this->m_data.resize(lodLevel+1,NULL);

		for (int i = 0; i < tempList.size(); i++)
		{
			this->m_data.at(i) = tempList.at(i);
		}

		this->m_data.at(lodLevel) = meshData;
	}

	meshData->AddRef();
}

int LOD::GetCount()
{
	return this->m_data.size();
}

int LOD::GetUseLevel(RenderAction* renderAction)
{
	int lodLevel = renderAction->GetCurretSpecifyLod();

	if(lodLevel>=0){
		return lodLevel;
	}

	CameraNode* camera = renderAction->GetCamera();

	if(camera!= NULL)
	{
		LOGI("LOD　GetUseLevel　TODO !!");
	}

	return lodLevel;
}

}
