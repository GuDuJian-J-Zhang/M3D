#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/utils/Trackball.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/ShapeProperty.h"

#include "m3d/Handler/Handler.h"
#include "m3d/model/ModelView.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/model/PMIData.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/ShapeSet.h"

#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"


namespace M3D
{
Model::Model(void) :
		Shape()
{
	m_HasColor = false;
	m_ProtoTypeID = OBJID++;;
	m_IsFirstGetProperties = true;
	m_ParentModel = NULL;
	m_PlaceID = 0;
	m_PlacePath = "";
	m_InstanceID = -1;
	m_PMICount = 0;
	this->SetType(SHAPE_MODEL);

	m_RootCSN = NULL;

	m_userPlcId = -1;

	this->m_shapeProperty = NULL;
}

Model::~Model(void)
{
	Clear();
}

/**
 * 复位颜色，位置，可见性
 */
void  Model::Restore()
{
	if(m_RootCSN)
	{
		m_RootCSN->Restore();
	}else
	{
		for (int r = 0; r < this->m_BodyArray.size(); r++)
		{
			this->m_BodyArray[r]->Restore();
		}
	}
	Shape::Restore();
}

void Model::Restore(bool resub)
{
	if(resub)
	{
		for(int i=0;i<this->m_SubModelArray.size();i++)
		{
			this->m_SubModelArray.at(i)->Restore(resub);
		}
	}
	Restore();
}


void Model::Clear()
{
	for (map<int, PMIData*>::iterator itPMI = m_AllPMIMap.begin();
			itPMI != m_AllPMIMap.end(); itPMI++)
	{
		delete (*itPMI).second;
	}
	m_AllPMIMap.clear();

	for (int i = 0; i < m_ModleViewList.size(); i++)
	{
		delete m_ModleViewList[i];
	}
	m_ModleViewList.clear();

	for (int i = 0; i < m_SectionPlaneList.size(); i++)
	{
		delete m_SectionPlaneList[i];
	}
	m_SectionPlaneList.clear();

	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->Release();
	}
	this->m_BodyArray.resize(0);

	//如果存在节点数据，删除
	if(this->m_RootCSN)
	{
		this->m_RootCSN->Release();
		this->m_RootCSN = NULL;
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->Release();
	}

	m_SubModelArray.clear();

	if(this->m_shapeProperty){
	this->m_shapeProperty->Release();
	this->m_shapeProperty = NULL;
	}
}

void Model::AddSubModel(Model * model)
{
	model->AddRef();
	model->SetParent(this);
	m_SubModelArray.push_back(model);
}

vector<Model*>& Model::GetSubModels()
{
	return this->m_SubModelArray;
}

void Model::RemoveSubModel(Model* model)
{
//	LOGI("Model::RemoveSubModel");
//	LOGI("Model::RemoveSubModel m_SubModelArray.size() ==%d",m_SubModelArray.size());
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_SubModelArray[i] == model)
		{
//			LOGI("Model::RemoveSubModel Remove model");
			delete m_SubModelArray[i];
//			LOGI("Model::RemoveSubModel Remove END");
			m_SubModelArray[i] = NULL;
			m_SubModelArray.erase(m_SubModelArray.begin() + i);
			break;
		}
	}
}

void Model::DetachSubModel(Model* model)
{
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_SubModelArray[i] == model)
		{
			model->SetParent((Model*)NULL);
			m_SubModelArray.erase(m_SubModelArray.begin() + i);
//			model->Release();
			break;
		}
	}

}

int Model::GetSubModelCount()
{
	if (m_SubModelArray.empty())
		return 0;
	return m_SubModelArray.size();
}

void Model::AddBody(Body* body)
{
	body->AddRef();
	body->SetModel(this);
	m_BodyArray.push_back(body);
}

void Model::SetPlaceMatrix(const Matrix4& plcMatrix)
{
	m_plcMatrix = plcMatrix;
}
const Matrix4& Model::GetPlaceMatrix()
{
	return this->m_plcMatrix;
}
void Model::SetAlpha(float a)
{
	SetAlpha(a, true);
}

void Model::SetAlpha(float a, bool relSub)
{
	Shape::SetAlpha(a);
	for (int r = 0; r < this->m_BodyArray.size(); r++)
	{
		this->m_BodyArray[r]->SetAlpha(a);
	}
	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetAlpha(a, relSub);
		}
	}
}

void Model::SetSelected(bool select)
{
	Shape::SetSelected(select);

	for (int r = 0; r < this->m_BodyArray.size(); r++)
	{
		Body* tmpBody = this->m_BodyArray[r];
		if (NULL != tmpBody)
		{
			tmpBody->SetSelected(select);
		}
	}

	for (int j = 0; j < m_SubModelArray.size(); j++)
	{
		Model *tmpModel = m_SubModelArray[j];
		if (NULL != tmpModel)
		{
			tmpModel->SetSelected(select);
		}
	}

}

void Model::SetVisible(bool visible)
{
	SetVisible(visible, true);
}

void Model::SetVisible(bool visible, bool relSub)
{
	Shape::SetVisible(visible);

	for (int r = 0; r < this->m_BodyArray.size(); r++)
	{
		this->m_BodyArray[r]->SetVisible(visible);
	}

	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetVisible(visible, relSub);
		}
	}
}

void Model::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !this->RendreVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}
	//首先通过包围盒进行判断是否有交点
	if (action->Intersect(GetBoundingBox()))
	{
		for (int i = 0; i < this->m_BodyArray.size(); i++)
		{
			this->m_BodyArray[i]->RayPick(action);
		}
	}
}

int Model::GetInstanceCount()
{
	int count = m_SubModelArray.size();

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		count += m_SubModelArray[i]->GetInstanceCount();
	}
	return count;
}

int Model::GetVertexCount(int level)
{
	int count = 0;
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		count += this->m_BodyArray[i]->GetVertexCount(level);
	}
	for (int i = 0; i < this->m_SubModelArray.size(); i++)
	{
		count += this->m_SubModelArray[i]->GetVertexCount(level);
	}

	return count;
}

void Model::UpdataInfo()
{
	ResortByPlcID();
}

void Model::ResortByPlcID()
{
	if(this->m_SubModelArray.size()>0)
	{
		sort(this->m_SubModelArray.begin(), this->m_SubModelArray.end(),
				CompareByPlcID);

		for (int i = 0; i < this->m_SubModelArray.size(); i++)
		{
			this->m_SubModelArray[i]->ResortByPlcID();
		}
	}
}

bool Model::CompareByPlcID(const Model* m1, const Model* m2)
{
	return m1->m_PlaceID < m2->m_PlaceID;
}

void Model::ComputeBox()
{
	m_BoundingBox.Clear();
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		if (this->m_BodyArray[i]->GetBoundingBox().Defined())
		{
			m_BoundingBox.Merge(this->m_BodyArray[i]->GetBoundingBox());
		}
	}

	//如果有PMI 设置成不允许剔除
	if(this->m_AllPMIMap.size()>0)
	{
		this->SetAlloewExculding(false);
	}
}

void Model::SetInitColor(const Color &color)
{
	Shape::SetInitColor(color);
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->SetInitColor(color);
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->SetInitColor(color);
	}
}

void Model::ResetColor()
{
	Shape::ResetColor();
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->ResetColor();
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->ResetColor();
	}
}

void Model::ResetAlpha()
{
	Shape::ResetAlpha();
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->ResetAlpha();
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->ResetAlpha();
	}
}

void Model::SetColor(const Color& color, bool reSub)
{
	Shape::SetColor(color);
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->SetColor(color);
	}

	if (reSub)
	{
		for (int i = 0; i < m_SubModelArray.size(); i++)
		{
			m_SubModelArray[i]->SetColor(color);
		}
	}
}

void Model::SetColor(const Color &color)
{
	this->SetColor(color, true);
}

void Model::SetMaterial(Material* material)
{
	Shape::SetMaterial(material);
	for (int i = 0; i < this->m_BodyArray.size(); i++)
	{
		m_BodyArray[i]->SetMaterial(material);
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->SetMaterial(material);
	}


}

//用于输出视图信息到java端 likun
void Model::GetModleViews(vector<int> *idArray, vector<string> *nameArray)
{
	if (!m_ModleViewList.empty())
	{
		for (int i = 0; i < m_ModleViewList.size(); i++)
		{
			idArray->push_back(m_ModleViewList[i]->GetID());
			nameArray->push_back(m_ModleViewList[i]->GetName());
		}
	}
}

ModelView* Model::GetModleView(int viewId)
{
	ModelView *pView = NULL;
	if (!m_ModleViewList.empty())
	{
		for (int i = 0; i < m_ModleViewList.size(); i++)
		{
			if (m_ModleViewList[i]->GetID() == viewId)
			{
				pView = m_ModleViewList[i];
				break;
			}
		}
	}
	//if(!pView)
	//{
//	LOGI("Begin Model::GetModleView:");
//			LOGI("Model::GetModleView: id:%d type:%d address:%d"
//					,pView->GetID(),pView->GetViewType(),pView);
//			LOGI("End Model::GetModleView:");
	//}
//	LOGI("Model::GetModleView: id:%d type:%d address:%d"
//			,pView->GetID(),pView->GetViewType(),pView);
	return pView;
}

void Model::AddModelView(ModelView* view)
{
	bool exist = false;
	vector<ModelView*>::iterator it;
	if (!m_ModleViewList.empty())
	{
		for (it = m_ModleViewList.begin(); it != m_ModleViewList.end(); it++)
		{

			if ((*it)->GetID() == view->GetID())
			{
				LOGI("Exist View Id is ===== %d",(*it)->GetID());
				exist = true;
				break;
			}
		}
	}
	if (exist)
	{
		LOGI("Model::addModelView: error：Id已经存在！将替换。");
		m_ModleViewList.erase(it);
	}

	m_ModleViewList.push_back(view);
}

void Model::RemoveModelView(int viewId)
{
	ModelView *pView = NULL;
	if (!m_ModleViewList.empty())
	{
		for (int i = 0; i < m_ModleViewList.size(); i++)
		{
			if (m_ModleViewList[i]->GetID() == viewId)
			{
				pView = m_ModleViewList[i];
				delete pView;
				m_ModleViewList.erase(m_ModleViewList.begin() + i);

				break;
			}
		}
	}
}

void Model::AddSectionPlane(SectionPlane* plane)
{
	bool exist = false;

	//判断id是否存在
	for (int i = 0; i < m_SectionPlaneList.size(); i++)
	{
		if (m_SectionPlaneList.at(i)->GetID() == plane->GetID())
		{
			exist = true;
			break;
		}
	}

	if (exist)
	{
		LOGE("Model::addSectionPlane: error：Id已经存在！");
	}
	else
	{
		m_SectionPlaneList.push_back(plane);
	}
}

SectionPlane* Model::GetSectionPlane(int id)
{
	SectionPlane* ret = NULL;
	for (int i = 0; i < m_SectionPlaneList.size(); i++)
	{
		if (m_SectionPlaneList.at(i)->GetID() == id)
		{
			ret = m_SectionPlaneList.at(i);
			break;
		}
	}
	return ret;
}

void Model::RemoveSectionPlane(int id)
{
	for (int i = 0; i < m_SectionPlaneList.size(); i++)
	{
		if (m_SectionPlaneList.at(i)->GetID() == id)
		{
			m_SectionPlaneList.erase(m_SectionPlaneList.begin() + i);
			break;
		}
	}
}

void Model::InitProperties()
{
	//LOGE("Model:InitProperties step 1");

	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string nameStr =language.GetCurrentTypeValue("ModelPropertiesName");
	string ID = language.GetCurrentTypeValue("ModelPropertiesID");
	string PlaceID =language.GetCurrentTypeValue("ModelPropertiesPlaceID");
	string ProtoTypeID = language.GetCurrentTypeValue("ModelPropertiesProtoTypeID");
	string Color =language.GetCurrentTypeValue("ModelPropertiesColor");
	string SubModelNumber = language.GetCurrentTypeValue("ModelPropertiesSubModelNumber");
	string InstanceCount =language.GetCurrentTypeValue("ModelPropertiesInstanceCount");
	string ModleViewCount = language.GetCurrentTypeValue("ModelPropertiesModleViewCount");
	string PMICount =language.GetCurrentTypeValue("ModelPropertiesPMICount");
	string LOD0PatchNumber = language.GetCurrentTypeValue("ModelPropertiesLOD0PatchNumber");
	string LOD1PatchNumber =language.GetCurrentTypeValue("ModelPropertiesLOD1PatchNumber");

	AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));
	AddProperty(PlaceID, IntToString(m_PlaceID));
	AddProperty(ProtoTypeID, IntToString(m_ProtoTypeID));
	string colorStr = M3DTOOLS::floatToString(m_Color.m_r)+","+M3DTOOLS::floatToString(m_Color.m_g)+","
			+M3DTOOLS::floatToString(m_Color.m_b)+","+M3DTOOLS::floatToString(m_Color.m_a);
	AddProperty(Color,colorStr);
	AddProperty(SubModelNumber, IntToString(m_SubModelArray.size()));
	AddProperty(InstanceCount, IntToString(GetInstanceCount()));
	AddProperty(ModleViewCount, IntToString(m_ModleViewList.size()));
	AddProperty(PMICount, IntToString(m_PMICount));
	AddProperty(LOD0PatchNumber, IntToString(GetVertexCount(0)/3));
	AddProperty(LOD1PatchNumber, IntToString(GetVertexCount(1)/3));


    string result = SVIEW::Parameters::Instance()->GetParameter(SVIEW::Parameters::ITEM_USE_COMPUTERPROPERTY);
    if (result == "true") {
      	string vomlumeKey =language.GetCurrentTypeValue("FacePropertiesVolume");
        string areaKey =language.GetCurrentTypeValue("FacePropertiesArea");
        float volume = 0;
        float area = 0;

        this->GetVolumeAndArea(volume,area);

        AddProperty(vomlumeKey, M3DTOOLS::floatToString(volume));
        AddProperty(areaKey, M3DTOOLS::floatToString(area));
    }
    
	if(this->m_shapeProperty)
	{
		vector<ShapePropertyItem>& propertyItems = this->m_shapeProperty->GetProperties();
		for(int i = 0;i<propertyItems.size();i++)
		{
			ShapePropertyItem& item = propertyItems.at(i);
			AddProperty(item.m_strTitle,item.m_strValue + " " + item.m_strUnits);
		}
	}
}

vector<SectionPlane*> Model::GetSectionPlaneList()
{
	return m_SectionPlaneList;
}

const vector<ModelView*>& Model::GetModelViewList()
{
	return m_ModleViewList;
}

void Model::SetParent(Model* model)
{
	this->m_ParentModel = model;
}

Model* Model::GetParent()
{
	return this->m_ParentModel;
}

void Model::FindVisiableObject(RenderAction* renderAction)
{
	if (!this->IsVisible())
	{
		return;
	}

	if (renderAction != NULL)
	{
		RenderEffect* renderEffect = renderAction->GetRenderEffect();
		RenderableTypeGroup& dataType = renderEffect->GetRenderableTypeFilter();

		if (dataType.Contain(RenderableType::RGT_PMI))
		{
			//将PMI加入显示队列
			renderAction->PrepareRenderPMIS(this);
		}

		if (dataType.Contain(RenderableType::RGT_BOX))
		{
			//将Box加入显示队列
			renderAction->PrepareRenderBox(this);
		}

		vector<Body*>& bodys = this->GetBodys();
		for (int i = 0; i < bodys.size(); i++)
		{
			bodys[i]->FindVisiableObject(renderAction);
		}
	}

}

BoundingBox& Model::GetWorldBoundingBox()
{
	if (this->GetSceneNode() != NULL)
	{
		return this->GetSceneNode()->GetWorldBoundingBox();
	}

	return BoundingBox::NO_BOX;
}
vector<Body*>& Model::GetBodys()
{
	return m_BodyArray;
}

void Model::SetBodys(vector<Body*>& bodys)
{
	m_BodyArray = bodys;
}

void Model::SetPlcId(int plcId)
{
	this->m_PlaceID = plcId;
}

int Model::GetPlcId()
{
	return this->m_PlaceID;
}

void Model::SetPMIs(const map<int, PMIData*>& pmis)
{
	this->m_AllPMIMap = pmis;
}

map<int, PMIData*>& Model::GetPMIs()
{
	return this->m_AllPMIMap;
}

void Model::SetProtoTypeId(int protoTypeId)
{
	this->m_ProtoTypeID = protoTypeId;
}

int Model::GetProtoTypeId()
{
	return this->m_ProtoTypeID;
}

void Model::SetPlcPath(const string& plcPath)
{
	this->m_PlacePath = plcPath;
}

string& Model::GetPlcPath()
{
	return this->m_PlacePath;
}

void Model::SetShapeSet(ShapeSet *newRootCSN)
{
	m_RootCSN = newRootCSN;
}

ShapeSet* Model::GetShapeCollection()
{
	return m_RootCSN;
}

void  Model::UpdateContainsShapesId()
{
	for (int r = 0; r < this->m_BodyArray.size(); r++)
	{
		this->m_BodyArray[r]->UpdateContainsShapesId(this);
	}
}

void  Model::UpdateContainsShapes(long id,Shape* shape)
{
	this->m_containShapes.insert(map<long,Shape*>::value_type(id,shape));
}

Shape*  Model::GetContainShape(long id)
{
	Shape* shape = NULL;

	map<long,Shape*>::iterator it = this->m_containShapes.find(id);

	if(it != m_containShapes.end())
	{
		shape = it->second;
	}

	return shape;
}


bool  Model::IsAssembly()
{
	 return this->m_SubModelArray.size()>0;
}

void Model::GetAllSubModels(vector<Model*>& subModelArray)
{
	for(int i=0;i<this->m_SubModelArray.size();i++)
	{
		Model* subModel = m_SubModelArray[i];
		subModelArray.push_back(subModel);
		subModel->GetAllSubModels(subModelArray);
	}
}

void Model::CopyData(Model* orig)
{
	if(orig->GetSubModelCount()>0)
	{
		vector<Model *>& subModels = orig->GetSubModels();
		for(int i = 0; i<subModels.size();i++)
		{
			Model * tempMdl = new Model();
			tempMdl->CopyData(subModels[i]);
			this->AddSubModel(tempMdl);
		}
	}

	vector<Body *>::iterator ite1;
	for (ite1 = orig->GetBodys().begin(); ite1 != orig->GetBodys().end();
			ite1++)
	{
		Body* body = new Body(*(*ite1));
		this->AddBody(body);
	}

	this->SetName(orig->GetName());
	this->SetPlaceMatrix(orig->GetPlaceMatrix());
	this->SetPlcId(orig->GetPlcId());
	this->SetProtoTypeId(orig->GetProtoTypeId());
	//如果使用catia 则进行node节点信息的copy

	if (orig->GetShapeCollection())
	{
		ShapeSet* node = new ShapeSet();
		*node = *orig->GetShapeCollection();
		this->SetShapeSet(node);
		this->UpdateContainsShapesId();
		this->GetShapeCollection()->UpdataRelevateShape(this);
	}

	this->SetShapeProperty(orig->GetShapeProperty());
}

int Model::GetUseablePlcId()
{

	for (int ii = 0; ii < m_SubModelArray.size(); ii ++)
	{
		int idChildPlc = m_SubModelArray.at(ii)->GetPlcId();
		if (m_userPlcId < idChildPlc)
			m_userPlcId = idChildPlc;
	}
	m_userPlcId++;

	return m_userPlcId;
}


void Model::SetShapeProperty(ShapeProperty* shapeProperty)
{
	if(this->m_shapeProperty){
	this->m_shapeProperty->Release();
	this->m_shapeProperty = NULL;
	}

	this->m_shapeProperty = shapeProperty;
    if (this->m_shapeProperty) {
        this->m_shapeProperty->AddRef();
    }
}

ShapeProperty* Model::GetShapeProperty()
{
	return this->m_shapeProperty;
}

string Model::GetPropertyFast(const string& key)
{
	if (this->m_shapeProperty)
	{
		vector<ShapePropertyItem>& propertyItems =
				this->m_shapeProperty->GetProperties();
		for (int i = 0; i < propertyItems.size(); i++)
		{
			ShapePropertyItem& item = propertyItems.at(i);

			if (key == item.m_strTitle)
			{
				return item.m_strValue + " " + item.m_strUnits;
			}
		}
	}
	return "";
}


float Model::GetVolumeAndArea(float& volume,float& area)
{
	vector<Body*>& bodys = this->GetBodys();
	for (int i = 0; i < bodys.size(); i++)
	{
		float subVolum = 0;
		float subArea = 0;
		bodys[i]->GetVolumeAndArea(subVolum,subArea);

		volume += subVolum;
		area += subArea;
	}

	for (int i = 0; i < this->m_SubModelArray.size(); i++)
	{
		float subVolum = 0;
				float subArea = 0;
		this->m_SubModelArray[i]->GetVolumeAndArea(subVolum,subArea);
		volume += subVolum;
		area += subArea;
	}

	return 0;
}


}




