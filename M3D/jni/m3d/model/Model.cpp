#include "m3d/M3D.h"
#include "m3d/model/Model.h"
#include "m3d/utils/IDCreator.h"
#include "sview/views/Parameters.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/utils/Trackball.h"

#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Face.h"

#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"

#include "m3d/model/Body.h"
#include "m3d/model/ModelShape.h"
#include "m3d/model/ShapeProperty.h"
#include "m3d/model/ModelView.h"
#include "m3d/model/PMIData.h"

#include "m3d/Handler/Handler.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/graphics/Material.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/action/Action.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/Curve.h"
#include "m3d/model/Edge.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/MaterialTemplateManager.h"
#include "m3d/graphics/ShaderMaterial.h"
namespace M3D
{
	SModelFileInfo::SModelFileInfo(M3D::Model* model) {
		stk_partModelId = -1;
		stk_instanceMaterialId = -1;
	}

	SModelFileInfo::~SModelFileInfo() {

	}

	int SModelFileInfo::GetPartModelId()
	{
		return this->stk_partModelId;
	}

	void SModelFileInfo::SetPartModelId(int stk_nodeId)
	{
		this->stk_partModelId = stk_nodeId;
	}

	void  SModelFileInfo::SetInstanceMaterialId(int materialId)
	{
		stk_instanceMaterialId = materialId;
	}

	int  SModelFileInfo::GetInstanceMaterialId()
	{
		return stk_instanceMaterialId;
	}

Model::Model(void):Object()
{
	//m_strNodeName = "NONAME";
	//m_position.ToZero();
	//m_rotation.ToZero();
	//m_scale.ToOne();
	//m_pParent = NULL;
	m_ParentModel = NULL;
/*	this->SetShape(NULL);*/
	m_modelShape = NULL;

	m_Visible = true;
	this->SetVisible(true,false);
	this->SetRenderVisible(true);
	this->SetSelected(false);
	/*m_cullerState = CameraNode::FRUSTUM_INTER;*/
	this->m_Id = OBJID++;

	m_modelExtInfo = NULL;
	m_ExtInfoMgr = NULL;
	m_InstanceID = 0;
	m_svlId = OBJID++;
	m_ProtoTypeID = -1;
	this->m_selectable = true;
	this->SetFileInfo(NULL);

	this->MarkDirty();
	m_originTrans = 1.0f;
}

Model::~Model(void)
{
	this->Clear();
}
 
void  Model::Optimize()
{
	//优化内存占用
	this->m_SubModelArray.shrink_to_fit();
}

SHAPETYPE Model::GetType(void)
{
	return SHAPE_MODEL;
}

void Model::SetType(SHAPETYPE type)
{
	//this->m_Type = type;
}

//void  Model::AddChild(Model* childModel)
//{
//	if (childModel)
//	{
//		childModel->SetParent(this);
//		childModel->AddRef();
//		this->m_SubModelArray.push_back(childModel);
//	}
//}

BoundingBox& Model:: GetWorldBoundingBox()
{
	if (this->m_modelShape)
	{
		return this->m_modelShape->GetWorldBoundingBox();
	}
	return BoundingBox::NO_BOX;
}

void Model::Traverse(Action* action)
{
	if (action->IsFinish())
	{
		return;
	}
	action->Apply(this);

	for(int i= 0;i<this->m_SubModelArray.size();++i)
	{
		(static_cast<Model*>(m_SubModelArray[i]))->Traverse(action);
	}
}
 
BoundingBox Model::GetTotalWorldBoundingBox()
{
	vector<Model*> allSubModel;
	this->GetAllSubModels(allSubModel);

	BoundingBox totalSubBox;

	for (int i = 0;i<allSubModel.size();i++)
	{
		if (allSubModel.at(i)->GetWorldBoundingBox().Defined())
		{
			totalSubBox.Merge(allSubModel.at(i)->GetWorldBoundingBox());
		}	
	}
	if (this->GetWorldBoundingBox().Defined())
	{
		totalSubBox.Merge(this->GetWorldBoundingBox());
	}
	return totalSubBox;
}

BoundingBox Model::GetTotalBoundingBox()
{
	vector<Model*> allSubModel;
	this->GetAllSubModels(allSubModel);

	BoundingBox totalSubBox;

	for (int i = 0; i < allSubModel.size(); i++)
	{
		if (allSubModel.at(i)->GetBoundingBox().Defined())
		{
			totalSubBox.Merge(allSubModel.at(i)->GetBoundingBox());
		}
	}
	if (this->GetBoundingBox().Defined())
	{
		totalSubBox.Merge(this->GetBoundingBox());
	}
	return totalSubBox;
}

void Model::SetLocalTransform(const Matrix3x4& localMatrix)
{
	this->SetPlaceMatrix(localMatrix);
	this->MarkDirty();
}

/**
* @brief Return direction in world space.
*/
Vector3 Model::GetWorldDirection() const
{
	if (m_dirty)
		UpdateWorldTransform();
	return m_worldMatrix.Rotation() * Vector3::FORWARD;
}

Matrix3x4 Model::GetLocalTransform()const
{
	return  GetTransform();
}

Matrix3x4 Model::GetTransform() const
{
	//通过平移旋转缩放构建，平移旋转和缩放暂时屏蔽了
	Matrix3x4 ret = *GetPlaceMatrix();
	return ret;
}

void Model::SetWorldTransform(const Matrix3x4& worldMatrix)
{
	Matrix3x4 localTransform;
	if (this->m_ParentModel == NULL)
	{
		localTransform = worldMatrix;
	}
	else
	{
		localTransform = (static_cast<Model*>(this->m_ParentModel))->GetWorldTransform().Inverse() * worldMatrix;
	}
 
	this->SetLocalTransform(localTransform);
}

Matrix3x4& Model::GetWorldTransform()const
{
	if (this->IsDirty())
	{
		this->UpdateWorldTransform();
	}

	return this->m_worldMatrix;
}

void Model::UpdataHardWareBuffer()
{

}

void Model::UpdateWorldTransform() const
{
    Matrix3x4 transform = GetLocalTransform();
 
    if (m_ParentModel == NULL)
    {
    	m_worldMatrix = transform;
    }
    else
    {
    	m_worldMatrix = (static_cast<Model*>(this->m_ParentModel))->GetWorldTransform() * transform;
    }

   /* this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();*/

    m_dirty = false;
}

void Model::FindVisiableObject(RenderAction* renderAction)
{
	if (this->m_modelShape)
	{
		this->m_modelShape->FindVisiableObject(renderAction);
	}

	for(int i= 0;i<this->m_SubModelArray.size();++i)
	{
		m_SubModelArray[i]->FindVisiableObject(renderAction);
	}
}
 
void Model::ResetMovement()
{
    if (this->GetOrigPlcMartirx()) {
        this->m_plcMatrix =*(this->GetOrigPlcMartirx());
    }
	this->MarkDirty();
}

void Model::Translate(const Vector3& delta, TransformSpace space /*= TS_LOCAL*/)
{
	if (delta.IsZero())
	{
		return;
	}
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);
	switch (space)
	{
	case TS_LOCAL:
		// Note: local space translation disregards local scale for scale-independent movement speed
		m_position += m_rotation * delta;
		break;

	case TS_PARENT:
		m_position += delta;
		break;

	case TS_WORLD:
		m_position += (this->GetParent() == NULL) ? delta : this->GetLocalTransform().Inverse() * Vector4(delta,0);
		break;
	}
	//SetTransform(m_position, m_rotation, m_scale);

	MarkDirty();
}

void Model::Rotate(const Quaternion& delta, TransformSpace space)
{
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);

	switch (space)
	{
	case TS_LOCAL:
		m_rotation += (m_rotation * delta).Normalized();
		break;

	case TS_PARENT:
		m_rotation += (delta * m_rotation).Normalized();
		break;

	case TS_WORLD:
		if (this->GetParent() == NULL)
			m_rotation = (delta * m_rotation).Normalized();
		else
		{
			Quaternion worldRotation = GetWorldRotation();
			m_rotation = m_rotation * worldRotation.Inverse() * delta * worldRotation;
		}
		break;
	}
	SetTransform(m_position, m_rotation, m_scale);
	//MarkDirty();
}

void Model::Scale(const Vector3&  delta, TransformSpace space /*= TS_LOCAL*/)
{
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);

	switch (space)
	{
	case TS_LOCAL:
		m_scale *= delta;
		break;
	}
	SetTransform(m_position, m_rotation, m_scale);
	//MarkDirty();
}

void Model::SetPosition(const Vector3& delta)
{
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);
	SetTransform(delta, m_rotation, m_scale);
	//MarkDirty();
}

Vector3 Model::GetPosition(){
//    Vector3 m_position;
//    Quaternion m_rotation;
//    Vector3 m_scale;
//    this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);
	//TODO 测试看是否对其他程序有影响
    Vector3 position(0,0,0);
    ModelShape* shapeNode = GetModelShape();
    if(shapeNode)
    {
        Matrix3x4 parentTran = GetWorldTransform();
        Matrix3x4& woldMatrix = shapeNode->GetWorldTransform();
        position = (parentTran.Inverse()*woldMatrix).Translation();
    }
    return position;
}

void Model::SetRotation(const Quaternion& delta)
{
	Vector3 m_position;
	Quaternion m_rotation = delta;
	Vector3 m_scale;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);

	SetTransform(m_position, delta, m_scale);

	//MarkDirty();
}

void Model::SetScale(const Vector3&  delta)
{
	Vector3 m_position ;
	Quaternion m_rotation;
	Vector3 m_scale = delta;
	this->GetPlaceMatrix()->Decompose(m_position, m_rotation, m_scale);

	SetTransform(m_position, m_rotation, delta);

	//MarkDirty();
}

void Model::SetWorldPosition(const Vector3& position)
{
	SetPosition((m_ParentModel == NULL) ? position : m_ParentModel->GetWorldTransform().Inverse() * position);
}

void Model::SetWorldRotation(const Quaternion& rotation)
{
	SetRotation((m_ParentModel == NULL) ? rotation : m_ParentModel->GetWorldRotation().Inverse() * rotation);
}

void Model::SetWorldDirection(const Vector3& direction)
{
	Vector3 localDirection = (m_ParentModel == NULL) ? direction : m_ParentModel->GetWorldRotation().Inverse() * direction;
	SetRotation(Quaternion(Vector3::FORWARD, localDirection));
}

void Model::SetWorldScale(float scale)
{
	SetWorldScale(Vector3(scale, scale, scale));
}

void Model::SetWorldScale(const Vector3& scale)
{
	SetScale((m_ParentModel == NULL) ? scale : scale / m_ParentModel->GetWorldScale());
}

M3D::Quaternion Model::GetWorldRotation() const
{
	if (m_dirty)
		UpdateWorldTransform();

	return m_worldMatrix.Rotation();
}

void Model::SetTransform(const Vector3& position, const Quaternion& rotation)
{
	SetTransform(position, rotation, Vector3(1.0f, 1.0f, 1.0f));
    MarkDirty();
}

void Model::SetTransform(const Vector3& position, const Quaternion& rotation, float scale)
{
    SetTransform(position, rotation, Vector3(scale, scale, scale));
}

void Model::SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	Matrix3x4 plcMatrix(position,rotation,scale);
	this->SetPlaceMatrix(plcMatrix);
    //MarkDirty();
}
 
 
void Model::MarkDirty()
{
    m_dirty = true;
    this->OnMarkedDirty();

	if (this->m_modelShape)
	{
		this->m_modelShape->MarkDirty();
	}	

	for (int i = 0; i<this->m_SubModelArray.size(); ++i)
	{
		static_cast<Model*>(this->m_SubModelArray[i])->MarkDirty();
	}
}

bool Model::IsDirty() const
{
	return m_dirty;
}

bool Model::HasDrawData() const
{
	return this->m_modelShape != NULL;
}

void Model::OnMarkedDirty()
{

}
 
vector<Model*>& Model::GetChildren()
{
	return m_SubModelArray;
}

void Model::Reset()
{
	//if (m_ExtInfoMgr)
	//{
	//	ModelExtInfo* modelExtInfo = m_ExtInfoMgr->GetModelExtInfo(m_Id);
	//	if (modelExtInfo)
	//	{
	//		m_plcMatrix = modelExtInfo->m_origPlcMatrix;
	//	}
	//}
	if (m_modelExtInfo)
	{
		m_plcMatrix = m_modelExtInfo->m_origPlcMatrix;
	}
	m_dirty = true;
	m_worldMatrix = Matrix4::IDENTITY;
	if (this->m_modelShape)
	{
		this->m_modelShape->MarkDirty();
	}
	this->OnMarkedDirty();
}
  
bool Model::GenTextureCoords(bool force /*= true*/)
{
	bool genState = false;
	if (this->m_modelShape)
	{
		vector<Body*>* bodies = this->m_modelShape->GetBodys();
		if (bodies)
		{
			for (int i =0;i<bodies->size();++i)
			{
				Body* subBody = bodies->at(i);
				if (subBody->GetData() != NULL)
				{
					genState = MeshHelper::GenTextureCoords(subBody->GetData(), force);
				}
			}
		}
	}
	return genState;
}

void  Model::Restore()
{
	SetVisible(IsOrigVisible(),false);
	//if (m_ExtInfoMgr)
	//{
	//	ModelExtInfo* modelExtInfo =  m_ExtInfoMgr->GetModelExtInfo(m_Id);
	//	if (modelExtInfo)
	//	{
	//		m_plcMatrix = modelExtInfo->m_origPlcMatrix;
	//	}
	//}
	if (m_modelExtInfo)
	{
		m_plcMatrix = m_modelExtInfo->m_origPlcMatrix;
	}
	MarkDirty();
	
	if (this->m_modelShape)
	{
		this->m_modelShape->Restore();
	}
}

void Model::Restore(bool resub)
{
	if (resub)
	{
		for (int i = 0; i<this->m_SubModelArray.size(); i++)
		{
			this->m_SubModelArray.at(i)->Restore(resub);
		}
	}

	Restore();
}

void Model::Clear()
{
	if (m_ExtInfoMgr)
	{
		m_ExtInfoMgr->Clear(m_Id);
	}
	
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_ExtInfoMgr)
		{
			m_ExtInfoMgr->Clear(m_SubModelArray[i]->GetID());
		}
		m_SubModelArray[i]->Release();
	}

	m_SubModelArray.clear();

	if (this->m_modelShape)
	{
		delete this->m_modelShape;
		this->m_modelShape = NULL;
	}

	if (this->m_modelExtInfo)
	{
		delete m_modelExtInfo;
		m_modelExtInfo = NULL;
	}

}

int Model::GetInstatnceID()
{
	/*if (this->m_modelExtInfo)
	{
		this->m_modelExtInfo->m_InstanceID;
	}*/
	return m_InstanceID;
}

void Model::SetInstanceID(int insID)
{
	//if (this->m_modelExtInfo)
	//{
	//	this->m_modelExtInfo->m_InstanceID = insID;
	//}
	m_InstanceID = insID;
}

void Model::AddSubModel(Model * model)
{
	if (model)
	{
		model->AddRef();
		model->SetParent(this);
		m_SubModelArray.push_back(model);
	}
}

vector<Model*>& Model::GetSubModels()
{
	return this->m_SubModelArray;
}

void Model::RemoveSubModel(Model* model)
{
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_SubModelArray[i] == model)
		{
			m_SubModelArray[i]->Release();
			m_SubModelArray[i] = NULL;
			m_SubModelArray.erase(m_SubModelArray.begin() + i);
			break;
		}
	}
}

Model* Model::GetModelByName(const string& name)
{
	Model* retModel = NULL;
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_SubModelArray[i]->GetName() == name)
		{
			retModel = m_SubModelArray[i];
			break;
		}
	}
	return retModel;
}

void Model::DetachSubModel(Model* model)
{
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		if (m_SubModelArray[i] == model)
		{
			model->SetParent((Model*)NULL);
			m_SubModelArray.erase(m_SubModelArray.begin() + i);
			model->Release();
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
	if (!this->m_modelShape)
	{
		this->m_modelShape = new ModelShape();
		SetModelShape(m_modelShape);
	}
	if (this->m_modelShape)
	{
		this->m_modelShape->AddBody(body);
	}
}

void Model::SetPlaceMatrix(const Matrix3x4& plcMatrix)
{
	m_plcMatrix = plcMatrix;
	MarkDirty();
	// plcMatrix.Decompose(this->m_position, this->m_rotation, this->m_scale);
}
Matrix3x4* Model::GetPlaceMatrix() const
{
	return &this->m_plcMatrix;
}
void Model::SetAlpha(float a)
{
	SetAlpha(a, true);
}

void Model::SetAlpha(float a, bool relSub)
{
	//m_Color.m_a = a;
	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetAlpha(a, relSub);
		}
	}

	if (this->m_modelShape)
	{
		this->m_modelShape->SetAlpha(a);
	}
}

void Model::SetSelected(bool select)
{
	this->m_IsSelect = select;

	for (int j = 0; j < m_SubModelArray.size(); j++)
	{
		Model *tmpModel = m_SubModelArray[j];
		if (NULL != tmpModel)
		{
			tmpModel->SetSelected(select);
		}
	}

	if (this->m_modelShape)
	{
		this->m_modelShape->SetSelected(select);
	}
}

void Model::SetVisible(bool visible)
{
	SetVisible(visible, true);
}

void Model::SetVisible(bool visible, bool relSub)
{
	this->m_Visible = visible;
	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetVisible(visible, relSub);
		}
	}

	if (this->m_modelShape)
	{
		this->m_modelShape->SetVisible(visible);
	}
}
 
void Model::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !this->RendreVisible() || !action->CanPickShape(this->GetType())|| !this->m_selectable)
	{
		return;
	}

	if (this->m_modelShape)
	{
		this->m_modelShape->RayPick(action);
	}

	for (int j = 0; j < m_SubModelArray.size(); j++)
	{
		m_SubModelArray[j]->RayPick(action);
	}
}

void Model::FramePick(RayPickAction* action)
{
	if (!this->IsVisible() || !this->RendreVisible() || !this->m_selectable)
	{
		return;
	}

	if (this->m_modelShape)
	{
		this->m_modelShape->FramePick(action);
	}

	for (int j = 0; j < m_SubModelArray.size(); j++)
	{
		m_SubModelArray[j]->RayPick(action);
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

int Model::GetFaceCount()
{
	int count = 0;
	if (GetBodys())
	{
		for (int i = 0; i < GetBodys()->size(); i++)
		{
			Body * body = GetBodys()->at(i);
			if (body)
			{
				count += body->GetFaces().size();
			}
		}
	}
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		count += m_SubModelArray[i]->GetFaceCount();
	}
	return count;
}

int Model::GetVertexCount(int level)
{
	int count = 0;
	for (int i = 0; i < this->m_SubModelArray.size(); i++)
	{
		count += this->m_SubModelArray[i]->GetVertexCount(level);
	}

	if (this->m_modelShape)
	{
		count += this->m_modelShape->GetVertexCount(level);
	}

	return count;
}

void Model::UpdataInfo()
{
	//ResortByPlcID();
}

void Model::ComputeBox()
{
	//m_BoundingBox.Clear();
	if (this->m_modelShape)
	{
		//m_BoundingBox.Merge(this->m_shapeNode->GetBoundingBox());
		this->m_modelShape->GetBoundingBox();
	}
	//如果有PMI 设置成不允许剔除
	/*if (this->m_AllPMIMap.size()>0)
	{
		this->SetAlloewExculding(false);
	}*/
}

void Model::SetInitAlpha(float alpha)
{
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->SetInitAlpha(alpha);
	}
}

void Model::SetInitHightlight(bool isHighlight)
{
	//for (int j = 0; j < m_SubModelArray.size(); j++)
	//{
	//	Model *tmpModel = m_SubModelArray[j];
	//	if (NULL != tmpModel)
	//	{
	//		tmpModel->SetInitHightlight(isHighlight);
	//	}
	//}

	if (this->m_modelShape)
	{
		this->m_modelShape->SetInitHightlight(isHighlight);
	}
}

bool Model::IsHightlight()
{
	return false;
}

void Model::SetInitColor(const Color &color)
{
	if (this->m_modelShape)
	{
		this->m_modelShape->SetInitColor(color);
	}
}

void Model::ResetColor()
{
	if (this->m_modelShape)
	{
		this->m_modelShape->ResetColor();
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->ResetColor();
	}
}

void Model::ResetAlpha()
{
	if (this->m_modelShape)
	{
		this->m_modelShape->ResetAlpha();
	}

	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		m_SubModelArray[i]->ResetAlpha();
	}
}

void Model::SetColor(const Color& color, bool reSub)
{
	//this->m_Color = color;
	if (this->m_modelShape)
	{
		this->m_modelShape->SetColor(color);
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

Color* Model::GetDrawColor()
{
	return NULL;
}

Color* Model::GetColor()
{
	Color * tmpColor = NULL;
	if (this->m_modelShape)
	{
		tmpColor = this->m_modelShape->GetColor();
	}
	if (!tmpColor && m_SubModelArray.size() > 0)
	{
		tmpColor = m_SubModelArray[0]->GetColor();
	}
	return tmpColor;
}
 
float Model::GetAlpha()
{
	if (this->m_modelShape)
	{
		return this->m_modelShape->GetAlpha();
	}
	else if (m_SubModelArray.size() > 0)
	{
		return m_SubModelArray[0]->GetAlpha();
	}
	return -1.0f;
}

bool Model::IsSelected() const
{
	return m_IsSelect;
}

void  Model::Selectable(bool selectable, bool reSub)
{
	m_selectable = selectable;

	if (this->m_modelShape)
	{
		this->m_modelShape->Selectable(selectable);
	}

	if (reSub)
	{
		for (int i = 0; i < m_SubModelArray.size(); i++)
		{
			m_SubModelArray[i]->Selectable(selectable, reSub);
		}
	}
}
 

bool Model::Selectable()
{
	return m_selectable;
}

void Model::SetBox(const BoundingBox& box)
{
	//m_BoundingBox = box;
}

bool Model::IsVisible()
{
	return this->m_Visible;
}


bool Model::IsOrigVisible()
{
	if (this->m_modelExtInfo)
	{
		return m_modelExtInfo->m_origVisible;
	}
	return true;
}


void Model::SetOrigVisible(bool visible)
{
	this->SetOrigVisible(visible,true);
}

void Model::SetOrigVisible(bool visible, bool relSub)
{
	if (this->m_modelExtInfo)
	{
		m_modelExtInfo->m_origVisible = visible;
	}

	this->SetVisible(visible, false);

	if (relSub)
	{
		for (int i = 0; i < m_SubModelArray.size(); i++)
		{
			m_SubModelArray[i]->SetOrigVisible(visible);
		}
	}
}

//void Model::SetInitVisble(bool visible)
//{
//    if (this->m_modelShape)
//    {
//        this->m_modelShape->SetVisible(visible);
//    }
//}
string Model::GetStatistics()
{
	string statisticsInfo = "";
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string InstanceCount = language.GetCurrentTypeValue("ModelPropertiesInstanceCount");
	string FaceCount = language.GetCurrentTypeValue("ModelPropertiesModleFaceCount");//
	string PMICount = language.GetCurrentTypeValue("ModelPropertiesPMICount");
	string LOD0PatchNumber = language.GetCurrentTypeValue("ModelPropertiesLOD0PatchNumber");
	string LOD1PatchNumber = language.GetCurrentTypeValue("ModelPropertiesLOD1PatchNumber");


	statisticsInfo.append(InstanceCount + "::" + IntToString(GetInstanceCount() + 1));
	statisticsInfo.append(";;");
	statisticsInfo.append(FaceCount + "::" + IntToString(GetFaceCount()));
	statisticsInfo.append(";;");
	statisticsInfo.append(LOD0PatchNumber + "::" + IntToString(GetVertexCount(0) / 3));
	statisticsInfo.append(";;");
	statisticsInfo.append(LOD1PatchNumber + "::" + IntToString(GetVertexCount(1) / 3));
	statisticsInfo.append(";;");
	statisticsInfo.append(PMICount + "::" + IntToString(m_ExtInfoMgr->GetModelPMIsCount(m_Id)));
	return statisticsInfo;
}

string Model::GetProperties()
{
	if (this->m_modelExtInfo)
	{
		if (this->m_modelExtInfo->m_IsFirstGetProperties)
		{
			InitProperties();
			this->m_modelExtInfo->m_IsFirstGetProperties = false;
		}
		return this->m_modelExtInfo->m_Properties;
	}
	return "";
}

void Model::ClearProperties()
{
	if (this->m_modelExtInfo)
	{
		this->m_modelExtInfo->m_Properties.clear();
		this->m_modelExtInfo->m_shapeProperty->GetProperties().clear();
	}
}
 
IDTYPE Model::GetID()
{
	return this->m_Id;
}

void Model::SetID(IDTYPE  value)
{
	this->m_Id = value;
}

std::string Model::GetName()
{
	//string name;
	//if (this->m_ExtInfoMgr)
	//{
	//	ModelExtInfo* extInfo = m_ExtInfoMgr->GetModelExtInfo(m_Id);
	//	if (extInfo)
	//	{
	//		name = extInfo->m_Name;
	//	}
	//}
	//return name;
	return m_Name;
}

void Model::SetName(const std::string& name)
{
	//if (this->m_ExtInfoMgr)
	//{
	//	ModelExtInfo* extInfo = m_ExtInfoMgr->GetModelExtInfo(m_Id);
	//	if (extInfo)
	//	{
	//		extInfo->m_Name = name;
	//	}
	//}
	this->m_Name = name;
}

/**
* 获取初始颜色
* @return
*/
Color* Model::GetInitColor()
{
	if (this->m_modelExtInfo)
	{

	}
	return NULL;
}


void Model::SetSceneNode(SceneNode* node)
{
	//this->node = node;
}

SceneNode* Model::GetSceneNode()
{
	return NULL;
}

void Model::SetMaterial(BaseMaterial* material)
{
	if (this->m_modelExtInfo)
	{

	}
	if (this->GetBodys())
	{
		for (int i = 0; i < this->GetBodys()->size(); i++) {
			Body *body = GetBodys()->at(i);
			if (body) {
				for (int j = 0; j < body->GetFaces().size(); j++) {
					Face *face = body->GetFaces().at(j);
					if (face) {
						face->SetMaterial(material);
					}
				}
			}
		}
	}

    for (int i = 0; i < m_SubModelArray.size(); i++) {
        m_SubModelArray[i]->SetMaterial(material);
    }
	//this->m_material = material;
	//if (this->m_material)
	//{
	//	this->m_material->AddRef();
	//}
}

void Model::SetMaterialForLastBody(BaseMaterial* material)
{
	if (this->m_modelExtInfo)
	{
	}

	if (this->GetBodys())
	{
		int nCount = this->GetBodys()->size();
		if (nCount > 0)
		{
			Body *body = GetBodys()->at(nCount - 1);
			if (body) 
			{
				for (int j = 0; j < body->GetFaces().size(); j++)
				{
					Face *face = body->GetFaces().at(j);
					if (face)
					{
						face->SetMaterial(material);
					}
				}
			}
		}
	}
}

bool	Model::HasMaterial()
{
	bool bHasMaterial = true;
	BaseMaterial* baseMaterial = NULL;
	BaseMaterial* firstMaterial = NULL;
	int iCount = 0;
	if (this->GetBodys())
	{
		for (int i = 0; i < this->GetBodys()->size(); i++) {
			Body *body = GetBodys()->at(i);
			if (body) {
				for (int j = 0; j < body->GetFaces().size(); j++)
				{
					Face *face = body->GetFaces()[j];
					if (face) {
						iCount++;
						baseMaterial = face->GetMaterial();
						if (1 == iCount)
						{
							firstMaterial = baseMaterial;
						} 
						else
						{
							if (firstMaterial != baseMaterial)
							{
								return false;
							}
						}
					}
				}
			}
		}
	}
	return bHasMaterial;
}

BaseMaterial* Model::GetMaterial()
{
	if (this->m_modelExtInfo)
	{

	}
	BaseMaterial* ret = NULL;
	if (this->GetBodys())
	{
		for (int i = 0; i < this->GetBodys()->size(); i++) {
			Body *body = GetBodys()->at(i);
			if (body) {
				if (body->GetFaces().size()>0)
				{
					Face *face = body->GetFaces()[0];
					if (face) {
						ret = face->GetMaterial();
					}
				}				
			}
		}
	}
	return ret;
}

bool Model::AllowExculding()
{
	if (this->m_modelExtInfo)
	{

	}
	return false;
}

void Model::SetAlloewExculding(bool allow)
{
	if (this->m_modelExtInfo)
	{

	}
}

bool Model::RendreVisible()
{
	if (this->m_modelExtInfo)
	{

	}
	return true;
}

void Model::SetRenderVisible(bool visible)
{
	if (this->m_modelExtInfo)
	{

	}
}

string Model::GetGeometryInfo()
{
	string geoInfo;

	return geoInfo;
}

void Model::AddRef(void)
{
	Object::AddRef();
}

void Model::Release(void)
{
	Object::Release();
}

int Model::GetSVLId()
{
	return this->m_svlId;
}

void Model::SetSVLId(int Id)
{
	this->m_svlId = Id;
}

void Model::MarkDrawDataDirty(bool relSub)
{
	if (this->m_modelShape)
	{
		this->m_modelShape->MarkDrawDataDirty();
	}

	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->MarkDrawDataDirty(relSub);
		}
	}
}

//用于输出视图信息到java端 likun
void Model::GetModleViews(vector<int> *idArray, vector<string> *nameArray)
{
	vector<ModelView*>* pVecViews = GetModelViewList();
	if (pVecViews)
	{
		for (int i = 0; i < pVecViews->size(); i++)
		{
			idArray->push_back((*pVecViews)[i]->GetID());
			nameArray->push_back((*pVecViews)[i]->GetName());
		}
	}
}

ModelView* Model::GetModleView(int viewId)
{
	ModelView *pView = NULL;
	if (m_ExtInfoMgr)
	{
		vector<ModelView*>* pVecViews = m_ExtInfoMgr->GetModelView(m_Id);
		if (pVecViews)
		{
			for (int i = 0; i < pVecViews->size(); i++)
			{
				if ((*pVecViews)[i]->GetID() == viewId)
				{
					pView = (*pVecViews)[i];
					break;
				}
			}
		}
	}
	return pView;
}

void Model::InsertModelView(int iIndex, ModelView* view)
{
	bool exist = false;
	if (this->m_ExtInfoMgr)
	{
		vector<ModelView*>* pVecViews = m_ExtInfoMgr->GetModelView(m_Id);
		vector<ModelView*>::iterator ite;
		if (pVecViews)
		{
			for (ite = pVecViews->begin(); ite != pVecViews->end(); ite++)
			{
				if ((*ite)->GetID() == view->GetID())
				{
					exist = true;
					break;
				}
			}
		}
		else
		{
			m_ExtInfoMgr->AddModelView(m_Id, vector<ModelView*>());
			pVecViews = m_ExtInfoMgr->GetModelView(m_Id);
		}
		if (!exist ||
			*ite != view)
		{
			if (exist)
			{
				(*ite)->Release();
				pVecViews->erase(ite);
			}
			view->AddRef();
			if (iIndex >= 0 && iIndex <= (int)pVecViews->size() - 1)
			{
				ite = pVecViews->begin() + iIndex;
				pVecViews->insert(ite, view);
			}
			else
			{
				pVecViews->push_back(view);
			}
		}
	}
}

void Model::AddModelView(ModelView* view)
{
	bool exist = false;
	if (this->m_ExtInfoMgr)
	{
		vector<ModelView*>* pVecViews = m_ExtInfoMgr->GetModelView(m_Id);
		vector<ModelView*>::iterator ite;
		if (pVecViews)
		{
			for (ite = pVecViews->begin(); ite != pVecViews->end(); ite++)
			{
				if ((*ite)->GetID() == view->GetID())
				{
					exist = true;
					break;
				}
			}
		}
		else
		{
			m_ExtInfoMgr->AddModelView(m_Id, vector<ModelView*>());
			pVecViews = m_ExtInfoMgr->GetModelView(m_Id);
		}
		if(!exist ||
			*ite != view)
		{
			if (exist)
			{
				(*ite)->Release();
				pVecViews->erase(ite);
			}
			view->AddRef();
			pVecViews->push_back(view);
		}
	}
}

void Model::RemoveModelView(int viewId)
{
	vector<ModelView*>* pVecViews = GetModelViewList();

	ModelView *pView = NULL;
	if (pVecViews)
	{
		for (int i = 0; i < pVecViews->size(); i++)
		{
			if ((*pVecViews)[i]->GetID() == viewId)
			{
				pView = (*pVecViews)[i];
				pView->Release();
				pVecViews->erase(pVecViews->begin() + i);
				break;
			}
		}
	}
}

void Model::MoveModelView(int viewId, int preViewId)
{
	vector<ModelView*>* pVecViews = GetModelViewList();
	if (pVecViews == NULL)
		return;
	int viewCount = (int)pVecViews->size();
	ModelView* pModelView = NULL;
	int nModelViewIndex = -1;
	int nPreModelViewIndex = viewCount-1;
	for (int i = 0; i < viewCount; i++)
	{
		if ((*pVecViews)[i]->GetID() == viewId)
		{
			pModelView = (*pVecViews)[i];
			nModelViewIndex = i;
			break;
		}
	}

	if (preViewId >=0)
	{
		for (int i = 0; i < viewCount; i++)
		{
			if ((*pVecViews)[i]->GetID() == preViewId)
			{
				nPreModelViewIndex = i;
				break;
			}
		}
	}
	

	if (pModelView)
	{
		if(nModelViewIndex > nPreModelViewIndex)
		{
			int nModelViewNewIndex = nPreModelViewIndex;
			for (int i = nModelViewIndex; i > nModelViewNewIndex; i--)
			{
				(*pVecViews)[i] = (*pVecViews)[i - 1];
			}
			(*pVecViews)[nModelViewNewIndex] = pModelView;
		}
		else
		{
			int nModelViewNewIndex = nPreModelViewIndex-1;
			for (int i = nModelViewIndex; i < nModelViewNewIndex; i++)
			{
				(*pVecViews)[i] = (*pVecViews)[i + 1];
			}
			(*pVecViews)[nModelViewNewIndex] = pModelView;
		}
	}
}

void Model::AddSectionPlane(SectionPlane* plane)
{
	bool exist = false;

	vector<SectionPlane*>* pVecSections = m_ExtInfoMgr->GetModelSectionPlane(m_Id);
	vector<SectionPlane*>::iterator ite;
	if (pVecSections)
	{
		for (ite = pVecSections->begin(); ite != pVecSections->end(); ite++)
		{
			if ((*ite)->GetID() == plane->GetID())
			{
				exist = true;
				break;
			}
		}
	}
	else
	{
		m_ExtInfoMgr->AddModelSectionPlane(m_Id, vector<SectionPlane*>());
		pVecSections = m_ExtInfoMgr->GetModelSectionPlane(m_Id);
	}
	if (exist)
	{
		(*ite)->Release();
		pVecSections->erase(ite);
	}
	plane->AddRef();
	pVecSections->push_back(plane);
}

SectionPlane* Model::GetSectionPlane(int id)
{
	SectionPlane* ret = NULL;
	vector<SectionPlane*>* pVecSections = GetSectionPlaneList();
	if (pVecSections)
	{
		for (int i = 0; i < pVecSections->size(); i++)
		{
			if (pVecSections->at(i)->GetID() == id)
			{
				ret = pVecSections->at(i);
				break;
			}
		}
	}
	return ret;
}

void Model::RemoveSectionPlane(int id)
{
	vector<SectionPlane*>* pVecSections = GetSectionPlaneList();

	SectionPlane *pSection = NULL;
	if (pVecSections)
	{
		for (int i = 0; i < pVecSections->size(); i++)
		{
			if ((*pVecSections)[i]->GetID() == id)
			{
				pSection = (*pVecSections)[i];
				pSection->Release();
				pVecSections->erase(pVecSections->begin() + i);
				break;
			}
		}
	}
}

void Model::InitProperties()
{
	//LOGE("Model:InitProperties step 1");
	this->m_modelExtInfo->m_Properties = "";
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string nameStr = language.GetCurrentTypeValue("ModelPropertiesName");
	string ID = language.GetCurrentTypeValue("ModelPropertiesID");
	string PlaceID = language.GetCurrentTypeValue("ModelPropertiesPlaceID");
	string ProtoTypeID = language.GetCurrentTypeValue("ModelPropertiesProtoTypeID");
	string Color = language.GetCurrentTypeValue("ModelPropertiesColor");
	string SubModelNumber = language.GetCurrentTypeValue("ModelPropertiesSubModelNumber");
	string InstanceCount = language.GetCurrentTypeValue("ModelPropertiesInstanceCount");
	string ModleViewCount = language.GetCurrentTypeValue("ModelPropertiesModleViewCount");
	string PMICount = language.GetCurrentTypeValue("ModelPropertiesPMICount");
	string LOD0PatchNumber = language.GetCurrentTypeValue("ModelPropertiesLOD0PatchNumber");
	string LOD1PatchNumber = language.GetCurrentTypeValue("ModelPropertiesLOD1PatchNumber");

	AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));
	AddProperty(PlaceID, IntToString(m_PlaceID));
	AddProperty(ProtoTypeID, IntToString(m_ProtoTypeID));
	if (this->m_modelShape)
	{
		M3D::Color* pColor =  m_modelShape->GetColor();
		if (pColor)
		{
			string colorStr = M3DTOOLS::floatToString(pColor->m_r) + "," + M3DTOOLS::floatToString(pColor->m_g) + ","
				+ M3DTOOLS::floatToString(pColor->m_b) + "," + M3DTOOLS::floatToString(pColor->m_a);
			AddProperty(Color, colorStr);
		}
	}
	
	AddProperty(SubModelNumber, IntToString(m_SubModelArray.size()));
	AddProperty(InstanceCount, IntToString(GetInstanceCount()));

	if (m_ExtInfoMgr)
	{
		AddProperty(ModleViewCount, IntToString(m_ExtInfoMgr->GetModelViewCount(m_Id)));
		AddProperty(PMICount, IntToString(m_ExtInfoMgr->GetModelPMIsCount(m_Id)));
	}
	else
	{
		AddProperty(ModleViewCount, IntToString(0));
		AddProperty(PMICount,IntToString(0));
	}

	AddProperty(LOD0PatchNumber, IntToString(GetVertexCount(0) / 3));
	//AddProperty(LOD1PatchNumber, IntToString(GetVertexCount(1) / 3));


	string result = SVIEW::Parameters::Instance()->GetParameter(SVIEW::Parameters::ITEM_USE_COMPUTERPROPERTY);
	if (result == "true") {
		string vomlumeKey = language.GetCurrentTypeValue("FacePropertiesVolume");
		string areaKey = language.GetCurrentTypeValue("FacePropertiesArea");
		float volume = 0;
		float area = 0;

		this->GetVolumeAndArea(volume, area);

		AddProperty(vomlumeKey, M3DTOOLS::floatToString(volume));
		AddProperty(areaKey, M3DTOOLS::floatToString(area));
	}

	if (this->m_modelExtInfo && this->m_modelExtInfo->m_shapeProperty)
	{
		vector<ShapePropertyItem>& propertyItems = this->m_modelExtInfo->m_shapeProperty->GetProperties();
		for (int i = 0; i < propertyItems.size(); i++)
		{
			ShapePropertyItem& item = propertyItems.at(i);
			AddProperty(item.m_strTitle, item.m_strValue + " " + item.m_strUnits);
		}
	}
}

void Model::AddProperty(string key, string value)
{
	if (this->m_modelExtInfo)
	{
		if (this->m_modelExtInfo->m_Properties.find(key + "::") != string::npos)
		{
			//LOGI("find properties");
			return;
		}
		if (!this->m_modelExtInfo->m_Properties.empty())
			this->m_modelExtInfo->m_Properties.append(";;");
		this->m_modelExtInfo->m_Properties.append(key + "::" + value);
	}

}

vector<SectionPlane*>* Model::GetSectionPlaneList()
{
	if (this->m_ExtInfoMgr)
	{
		return m_ExtInfoMgr->GetModelSectionPlane(m_Id);
	}
	return NULL;
}

vector<ModelView*>* Model::GetModelViewList()
{
	if (this->m_ExtInfoMgr)
	{
		return m_ExtInfoMgr->GetModelView(m_Id);
	}
	return NULL;
}

void Model::SetParent(Model* model)
{
	this->m_ParentModel = model;
}

Model* Model::GetParent()
{
	return this->m_ParentModel;
}

vector<Body*>* Model::GetBodys()
{
	if (this->m_modelShape)
	{
		return this->m_modelShape->GetBodys();
	}
	return NULL;
}

void Model::SetBodys(vector<Body*>& bodys)
{
	if (this->m_modelShape)
	{
		this->m_modelShape->SetBodys(bodys);
	}
}

void Model::ClearMeshData()
{
	if (this->m_modelShape)
	{
		delete this->m_modelShape;
		this->m_modelShape = NULL;
	}
}

void Model::AddMeshData(vector<Vector3>& points, vector<Vector3>& normals, vector<Vector3>& uvs, vector<M3D_INDEX_TYPE>& indexs)
{
	//顶点个数大于0，并且是3的整数倍
	Body* body = new Body();
	VertexSet* bodyDrawData = new VertexSet();
	bodyDrawData->SetUseIndex(true);
	std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
	std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
	std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
	std::vector<Vector3>* textureCoords = bodyDrawData->GetTextureCoordArray();

	*bodyPnts = points;
	*bodyNormals = normals;
	*indexArray = indexs;
	*textureCoords = uvs;

	//构造一个合并面显示
	Face* pFace = new Face();

	Mesh* facemeshData = new Mesh(bodyDrawData);
	facemeshData->SetDataOffset(0);
	facemeshData->SetDataLength(bodyDrawData->GetIndexArray()->size());
	pFace->SetData(facemeshData);
	body->AddFace(pFace);

	this->AddBody(body);
}

void Model::AddBodyByFace(Face* face)
{
	Mesh* pMesh = face->GetData();
	VertexSet* bodyDrawData = pMesh->GetRefMesh();
	unsigned int uiDataOffset = pMesh->GetDataOffset();
	unsigned int uiDataLength =  pMesh->GetDataLength();
	std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
	std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
	std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
	std::vector<Vector3>* bodyTextures = bodyDrawData->GetTextureCoordArray();
	vector<Vector3> curFacePnts;
	vector<Vector3> curFaceNmls;
	vector<Vector3> curFaceTxts;
	vector<M3D_INDEX_TYPE> curFaceIndex;
	for (int i = 0; i < uiDataLength; i ++)
	{
		int realIndex = indexArray->at(uiDataOffset++);
		curFaceIndex.push_back(realIndex);

		if (bodyPnts->size() > 0)
		{
			Vector3 curPnt = bodyPnts->at(realIndex);
			curFacePnts.push_back(curPnt);
		}
		if (bodyNormals->size() > 0)
		{
			Vector3 curNml = bodyNormals->at(realIndex);
			curFaceNmls.push_back(curNml);
		}		
		if (bodyTextures->size() > 0)
		{
			Vector3 curTxt = bodyTextures->at(realIndex);
			curFaceTxts.push_back(curTxt);
		}
	}

	//顶点个数大于0，并且是3的整数倍
	Body* body = new Body();
	
	VertexSet* newBodyDrawData = new VertexSet();
	newBodyDrawData->SetUseIndex(true);
	std::vector<Vector3>* newBodyPnts = newBodyDrawData->GetPositionArray();
	std::vector<Vector3>* newBodyNormals = newBodyDrawData->GetNormalArray();
	std::vector<M3D_INDEX_TYPE>* newIndexArray = newBodyDrawData->GetIndexArray();
	std::vector<Vector3>* newTextureCoords = newBodyDrawData->GetTextureCoordArray();

	*newBodyPnts = curFacePnts;
	*newBodyNormals = curFaceNmls;
	*newIndexArray = curFaceIndex;
	if (curFaceTxts.size() > 0)
	{
		*newTextureCoords = curFaceTxts;
	}
	else
	{
		//newTextureCoords->push_back(Vector3(0, 0, 0));
		//newTextureCoords->push_back(Vector3(0, 1, 0));
		//newTextureCoords->push_back(Vector3(1, 1, 0));
		//newTextureCoords->push_back(Vector3(1, 0, 0));
		//newTextureCoords->push_back(Vector3(0, 0, 0));
		//newTextureCoords->push_back(Vector3(1, 1, 0));
	}	

	//构造一个合并面显示
	Face* pFace = new Face();

	Mesh* facemeshData = new Mesh(bodyDrawData);
	facemeshData->SetDataOffset(0);
	facemeshData->SetDataLength(bodyDrawData->GetPositionArray()->size());
	pFace->SetData(facemeshData);

	//构造一个合并面显示
	//Face* pFace1 = new Face(*face);

	body->AddFace(pFace);

	this->AddBody(body);
}

void Model::AddMeshData(vector<Vector3>& points, vector<Vector3>& normals, vector<Vector3>& uvs)
{
	//顶点个数大于0，并且是3的整数倍
	Body* body = new Body();
	VertexSet* bodyDrawData = new VertexSet();
	bodyDrawData->SetUseIndex(false);
	std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
	std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
	std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
	std::vector<Vector3>* textureCoords = bodyDrawData->GetTextureCoordArray();

	*bodyPnts = points;
	*bodyNormals = normals;
	*textureCoords = uvs;

	//构造一个合并面显示
	Face* pFace = new Face();

	Mesh* facemeshData = new Mesh(bodyDrawData);
	facemeshData->SetDataOffset(0);
	facemeshData->SetDataLength(bodyDrawData->GetPositionArray()->size());
	pFace->SetData(facemeshData);
	body->AddFace(pFace);

	this->AddBody(body);
}

void Model::ClearLineData()
{
	if (this->m_modelShape)
	{
		delete this->m_modelShape;
		this->m_modelShape = NULL;
	}
}

void Model::AddLineData(vector<Vector3>& points, vector<M3D_INDEX_TYPE>& indexs)
{
	Body* body = new Body();
	M3D::SPolyLine* bodyPolyLine = body->GetPolyLine();

	if (bodyPolyLine == NULL)
	{
		bodyPolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
		body->SetPolyLine(bodyPolyLine);

		RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);

		edgeLine->SetDataOffset(bodyPolyLine->GetPoints().size());	
		edgeLine->SetDataLength(2 * (indexs.size() - 1));

		Edge* edge = new Edge();
		edge->AddData(edgeLine, 0);
		body->AddEdge(edge);

		unsigned int dataLength = indexs.size();

		if (dataLength >= 2) {
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			bodyPolyLine->AddPoints(points[0]);
			//加入中间部分点
			for (int j = 1; j < dataLength - 1; j++) {				 
				//中间的点，每个点要添加两次
				bodyPolyLine->AddPoints(points[indexs[j]]);
				bodyPolyLine->AddPoints(points[indexs[j]]);
			}
			//在加入最后一个点
			bodyPolyLine->AddPoints(points[dataLength-1]);
		}
	}
	this->AddBody(body);
}

void Model::AddLineData(vector<Vector3>& points, Color color, bool dotted)
{
	Body* body = new Body();
	M3D::SPolyLine* bodyPolyLine = body->GetPolyLine();

	if (bodyPolyLine == NULL)
	{
		bodyPolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
		body->SetPolyLine(bodyPolyLine);

		RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);

		edgeLine->SetDataOffset(bodyPolyLine->GetPoints().size());
		edgeLine->SetDataLength(2 * (points.size() - 1));

		Edge* edge = new Edge();
		edge->AddData(edgeLine, 0);
		edge->SetColor(color);
		edge->SetDotted(dotted);
		body->AddEdge(edge);

		unsigned int dataLength = points.size();

		if (dataLength >= 2) {
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			bodyPolyLine->AddPoints(points[0]);
			//加入中间部分点
			for (int j = 1; j < dataLength - 1; j++) {
				//中间的点，每个点要添加两次
				bodyPolyLine->AddPoints(points[j]);
				bodyPolyLine->AddPoints(points[j]);
			}
			//在加入最后一个点
			bodyPolyLine->AddPoints(points[dataLength - 1]);
		}
	}
	this->AddBody(body);
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
	if (this->m_ExtInfoMgr)
	{
		m_ExtInfoMgr->AddModelPMIs(m_Id, pmis);

		for (map<int, PMIData*>::const_iterator PMIDataIt = pmis.begin(); PMIDataIt != pmis.end(); PMIDataIt++)
		{
			PMIData* value = PMIDataIt->second;
			value->SetParentModel(this);
		}
	}
	//this->m_AllPMIMap = pmis;
}

map<int, PMIData*>* Model::GetPMIs()
{
	if (this->m_ExtInfoMgr)
	{
		return m_ExtInfoMgr->GetModelPMIs(m_Id);
	}
	return NULL;
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
	if (!m_modelExtInfo)
	{
		m_modelExtInfo = new ModelExtInfo();
	}
	m_modelExtInfo->m_placePath = plcPath;
}

const string& Model::GetPlcPath()
{
	if (!SVIEW::Parameters::m_useSimplePath)
	{
		if (!m_modelExtInfo)
		{
			m_modelExtInfo = new ModelExtInfo();
		}

		if (this->m_modelExtInfo->m_placePath.size())
		{
			return this->m_modelExtInfo->m_placePath;
		}
		else
		{
			//通过上级进行路径更新
			if (this->GetParent())
			{
				//获取配置ID路径
				char tmpCurPlcIDStr[10];
				memset(tmpCurPlcIDStr, '\0', sizeof(tmpCurPlcIDStr));
				sprintf(tmpCurPlcIDStr, "%x", this->GetPlcId()); //配置路径按照16进制解析
				this->SetPlcPath(this->GetParent()->GetPlcPath() + "|" + tmpCurPlcIDStr);
			}
			else
			{
				this->SetPlcPath(MAINMODELROOT + "|0");
			}
		}	
		return this->GetPlcPath();
	}
	return M3D::MAINMODELROOT;
}

void Model::SetShapeSet(ShapeSet *newRootCSN)
{
	if (this->m_modelExtInfo)
	{

	}
	//m_RootCSN = newRootCSN;
}

ShapeSet* Model::GetShapeCollection()
{
	if (this->m_modelExtInfo)
	{

	}
	return NULL;
}

void  Model::UpdateContainsShapesId()
{
	if (this->m_modelExtInfo)
	{

	}
}

void  Model::UpdateContainsShapes(long id, IShape* shape)
{
	if (this->m_modelExtInfo)
	{

	}
	//this->m_containShapes.insert(map<long, IShape*>::value_type(id, shape));
}

IShape* Model::GetContainShape(long id)
{
	if (this->m_modelExtInfo)
	{

	}
	IShape* shape = NULL;

	/*map<long, IShape*>::iterator it = this->m_containShapes.find(id);

	if (it != m_containShapes.end())
	{
		shape = it->second;
	}*/

	return shape;
}


bool  Model::IsAssembly()
{
	return this->m_SubModelArray.size()>0;
}

void Model::GetAllSubModels(vector<Model*>& subModelArray)
{
	for (int i = 0; i<this->m_SubModelArray.size(); i++)
	{
		Model* subModel = m_SubModelArray[i];
		subModelArray.push_back(subModel);
		subModel->GetAllSubModels(subModelArray);
	}
}

BoundingBox& Model::GetBoundingBox()
{
	if (this->m_modelShape)
	{
		return this->m_modelShape->GetBoundingBox();
	}
	return BoundingBox::NO_BOX;
}

void Model::CopyData(Model* orig)
{
    ModelShape* tShapeNode = new ModelShape();
    this->SetModelShape(tShapeNode);
    if (orig->m_ExtInfoMgr) {
       SetModelExtInfo(orig->m_ExtInfoMgr);
    }
    if (orig->GetSubModelCount()>0)
    {
        vector<Model *>& subModels = orig->GetSubModels();
        for (int i = 0; i<subModels.size(); i++)
        {
            Model * tempMdl = new Model();
            tempMdl->CopyData(subModels[i]);
            this->AddSubModel(tempMdl);
        }
    }
	if(orig->GetBodys())
	{
	 vector<Body *>::iterator ite1;
    for (ite1 = orig->GetBodys()->begin(); ite1 != orig->GetBodys()->end();
        ite1++)
    {
        Body* body = new Body(*(Body*)(*ite1));
        this->AddBody(body);
    }
	}
   
    this->SetName(orig->GetName());
    this->SetPlaceMatrix(*orig->GetPlaceMatrix());
    this->SetPlcId(orig->GetPlcId());
    this->SetProtoTypeId(orig->GetProtoTypeId());
    if(orig->GetOrigPlcMartirx()){
      this->SetOrigPlcMatrix(*orig->GetOrigPlcMartirx());
    }
    //如果使用catia 则进行node节点信息的copy

//    if (orig->GetShapeCollection())
//    {
//        ShapeSet* node = new ShapeSet();
//        *node = *orig->GetShapeCollection();
//        this->SetShapeSet(node);
//        this->UpdateContainsShapesId();
//        this->GetShapeCollection()->UpdataRelevateShape(this);
//    }
    
    this->SetShapeProperty(orig->GetShapeProperty());
}

int Model::GetUseablePlcId()
{
	int m_userPlcId = 1;
	for (int ii = 0; ii < m_SubModelArray.size(); ii++)
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
	if (this->m_modelExtInfo)
	{
		if (this->m_modelExtInfo->m_shapeProperty) {
			this->m_modelExtInfo->m_shapeProperty->Release();
			this->m_modelExtInfo->m_shapeProperty = NULL;
		}

		this->m_modelExtInfo->m_shapeProperty = shapeProperty;
		if (this->m_modelExtInfo->m_shapeProperty)
		{
			this->m_modelExtInfo->m_shapeProperty->AddRef();
		}
	}
}

ShapeProperty* Model::GetShapeProperty()
{
	if (this->m_modelExtInfo)
	{
		return this->m_modelExtInfo->m_shapeProperty;
	}
	return NULL;
}

string Model::GetPropertyFast(const string& key)
{
	if (this->m_modelExtInfo)
	{
		if (this->m_modelExtInfo->m_shapeProperty)
		{
			vector<ShapePropertyItem>& propertyItems =
				this->m_modelExtInfo->m_shapeProperty->GetProperties();
			for (int i = 0; i < propertyItems.size(); i++)
			{
				ShapePropertyItem& item = propertyItems.at(i);

				if (key == item.m_strTitle)
				{
					return item.m_strValue + " " + item.m_strUnits;
				}
			}
		}
	}
	return "";
}

ModelShape* Model::GetModelShape()
{
	return m_modelShape;
}

void Model::SetModelShape(ModelShape* modelShape)
{
	m_modelShape = modelShape;
	if (modelShape)
	{
		modelShape->SetModel(this);
	}
}

SModelFileInfo* Model::GetFileInfo()
{
	return this->m_fileInfo;
}

void Model::SetFileInfo(SModelFileInfo* fileInfo)
{
	this->m_fileInfo = fileInfo;
}

void Model::ClearFileInfo()
{
	if (this->m_fileInfo)
	{
		delete this->m_fileInfo;
		this->m_fileInfo = NULL;
	}
}


float Model::GetVolumeAndArea(float& volume, float& area)
{
	if (this->m_modelShape)
	{
		float subVolum = 0;
		float subArea = 0;
		this->m_modelShape->GetVolumeAndArea(subVolum, subArea);
		volume += subVolum;
		area += subArea;
	}

	for (int i = 0; i < this->m_SubModelArray.size(); i++)
	{
		float subVolum = 0;
		float subArea = 0;
		this->m_SubModelArray[i]->GetVolumeAndArea(subVolum, subArea);
		volume += subVolum;
		area += subArea;
	}

	return 0;
}

void Model::SetDrawDataPrepared(bool dataPrepared)
{
	if (dataPrepared)
	{
		SetModelExtInfo();
	}

	if (m_modelShape)
	{
		m_modelShape->SetDrawDataPrepared(dataPrepared);
	}
}	

void Model::SetDrawDataPrepared(bool dataPrepared, bool resub)
{
	SetDrawDataPrepared(dataPrepared);
	if (resub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetDrawDataPrepared(dataPrepared, resub);
		}
	}
}

void Model::SetModelExtInfo()
{
	vector<Body*>* bodys = this->GetBodys();
	if (bodys)
	{
		for (int i = 0; i < bodys->size(); i++)
		{
			Body* body = bodys->at(i);
			if (body)
			{
				body->SetBodyExtInfo();
			}
		}
	}
}

void Model::SetModelExtInfo(ExtendInfoManager* infoMgr)
{
	if (!m_ExtInfoMgr || m_ExtInfoMgr != infoMgr)
	{
		m_ExtInfoMgr = infoMgr;
		if (!m_modelExtInfo)
		{
			m_modelExtInfo = new ModelExtInfo();
		}

		m_modelExtInfo->m_origVisible = this->IsVisible();
		m_modelExtInfo->m_origPlcMatrix = this->m_plcMatrix;
		SetModelExtInfo();
	}
}

ExtendInfoManager* Model::GetExtendInfoManager()
{
	return this->m_ExtInfoMgr;
}
void Model::SetModelExtInfo(ExtendInfoManager* infoMgr, bool relSub)
{
	SetModelExtInfo(infoMgr);
	if (relSub)
	{
		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->SetModelExtInfo(infoMgr, relSub);
		}
	}
}


Matrix3x4* Model::GetOrigPlcMartirx()
{
	if (m_modelExtInfo)
	{
		return &m_modelExtInfo->m_origPlcMatrix;
	}
	return NULL;
}

void Model::SetOrigPlcMatrix(const Matrix3x4& plcMatrix)
{
	if (!m_modelExtInfo)
	{
		m_modelExtInfo = new ModelExtInfo();
	}

	if (m_modelExtInfo)
	{
		m_modelExtInfo->m_origPlcMatrix = plcMatrix;
		//SetModelExtInfo();
		this->SetPlaceMatrix(plcMatrix);
	}
}

M3D::Vector3 Model::GetWorldScale() const
{
	if (m_dirty)
		UpdateWorldTransform();

	return m_worldMatrix.Scale();
}

M3D::Vector3 Model::GetWorldPosition() const
{
	if (m_dirty)
		UpdateWorldTransform();

	return m_worldMatrix.Translation();
}


bool Model::LookAt(const Vector3& target, const Vector3& up, TransformSpace space)
{
	Vector3 worldSpaceTarget;

	switch (space)
	{
	case TS_LOCAL:
		worldSpaceTarget = GetWorldTransform() * target;
		break;

	case TS_PARENT:
		worldSpaceTarget = (this->GetParent() == NULL) ? target : this->GetParent()->GetWorldTransform() * target;
		break;

	case TS_WORLD:
		worldSpaceTarget = target;
		break;
	}

	Vector3 lookDir = worldSpaceTarget - GetWorldPosition();
	// Check if target is very close, in that case can not reliably calculate lookat direction
	if (lookDir.Equals(Vector3::ZERO))
		return false;
	Quaternion newRotation;
	// Do nothing if setting look rotation failed
	if (!newRotation.FromLookRotation(lookDir, up))
		return false;

	SetWorldRotation(newRotation);
	return true;
}

ModelExtInfo::ModelExtInfo()
{
	m_shapeProperty = NULL;
	m_IsFirstGetProperties = true; 
}

ModelExtInfo::~ModelExtInfo()
{

}

ImageModel::ImageModel(void) :Model()
{
	
}

ImageModel::~ImageModel(void)
{

}

M3D::SHAPETYPE ImageModel::GetType(void)
{
	return SHAPETYPE::SHAPE_IMAGE_MODEL;
}

void ImageModel::SetImagePath(const string& imagePath)
{
	this->CreateModelShape();
	//this->SetImageData(NULL, 0);
	ImageModelShape* imageModelshape = this->GetImageModelShape();
	if (imageModelshape)
	{
		imageModelshape->SetImagePath(imagePath);
	}
}

string ImageModel::GetImagePath()
{
	string strImagePath = "";
	ImageModelShape* imageModelshape = this->GetImageModelShape();
	if (imageModelshape)
		strImagePath = imageModelshape->GetImagePath();
	return strImagePath;
}

void ImageModel::SetImageData(char* imageData, int dataLength)
{
	this->CreateModelShape();
	//this->SetImagePath("");
	ImageModelShape* imageModelshape = this->GetImageModelShape();
	if (imageModelshape)
	{
		imageModelshape->SetImageData(imageData, dataLength);
	}
}

void ImageModel::SetImageSize(Vector3& position, Vector2& size)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetImageSize(position, size);
	}
}

M3D::Vector2 ImageModel::GetImageSize()
{
	Vector2 size;
	if (m_modelShape)
	{
		ImageModelShape* pImgModel = (ImageModelShape*)m_modelShape;
		if (pImgModel)
			size = pImgModel->GetImageSize();
	}
	return size;
}

void ImageModel::SetImagePosition(Vector3& position)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetImagePosition(position);
	}
}

M3D::Vector3 ImageModel::GetImagePosition()
{
	Vector3 position;
	if (m_modelShape)
	{
		ImageModelShape* pImgModel = (ImageModelShape*)m_modelShape;
		if (pImgModel)
			position = pImgModel->GetImagePosition();
	}
	return position;
}

void ImageModel::SetImageGPUID(unsigned int gupID)
{
	this->CreateModelShape();
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetImageGPUID(gupID);
	}
}

void ImageModel::SetFlipImage(bool flipImage)
{
	this->CreateModelShape();
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetFlipImage(flipImage);
	}
}

void ImageModel::SetAllowScall(bool allowScale)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetAllowScall(allowScale);
	}
}

void ImageModel::SetAllowRotate(bool allowRotate)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetAllowRotate(allowRotate);
	}
}

void ImageModel::SetInTopShow(bool allowInTopShow)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetInTopShow(allowInTopShow);
	}
}

void ImageModel::SetAllowTran(bool allowTran)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetAllowTran(allowTran);
	}
}


void ImageModel::SetFixShowInScreen(bool fixShow)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		imageModel->SetFixShowInScreen(fixShow);
	}
}

void ImageModel::SetAllowClip(bool allowClip)
{
	if (this->m_modelShape)
	{
		ImageModelShape* imageModel = (ImageModelShape*)this->m_modelShape;
		ImageBoard* imageBoard = imageModel->GetImageBoard();
		if (imageBoard)
		{
			imageBoard->SetAllowClip(allowClip);
		}
	}
}

void ImageModel::CreateModelShape()
{
	if (this->m_modelShape == NULL)
	{
		this->m_modelShape = new ImageModelShape();
		this->m_modelShape->SetModel(this);
	}
}

M3D::ImageModelShape* ImageModel::GetImageModelShape()
{
	return static_cast<ImageModelShape*> (this->m_modelShape);
}

SignModel::SignModel(void)
{
	SetSimpleSignModel(NULL);
	SetAllSignModel(NULL);

	SetNeedUpdataSign(false);//需要更新显示状态对应的数据
	m_showSimpleSign = false; //显示简单标记
	m_ShowAllSign = false; //显示完整标记
}

SignModel::~SignModel(void)
{
	ReleaseMe(this->m_allSignModel);
	ReleaseMe(this->m_simpleSignModel);
}	

void SignModel::SetShowSimpleSign(bool val)
{
	m_showSimpleSign = val;

	if (this->m_allSignModel)
	{
		this->m_allSignModel->SetVisible(val);
	}
}

void SignModel::OnMarkedDirty()
{
	this->SetNeedUpdataSign(true);
}

void SignModel::SetShowAllSign(bool val)
{
	m_ShowAllSign = val;
	if (this->m_allSignModel)
	{
		this->m_allSignModel->SetVisible(val);
	}
}

void Model::ClearPlcPath(bool bSub)
{
	if (!SVIEW::Parameters::m_useSimplePath && this->m_modelExtInfo)
	{
		this->m_modelExtInfo->m_placePath = "";
		if (bSub)
		{
			for (int i = 0; i < this->m_SubModelArray.size(); i++)
			{
				m_SubModelArray.at(i)->ClearPlcPath(bSub);
			}
		}
	}
}

void Model::SaveProperties(string& key, string& value)
{
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	vector<string> vec;
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesName"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesID"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesPlaceID"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesProtoTypeID"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesColor"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesSubModelNumber"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesInstanceCount"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesModleViewCount"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesPMICount"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesLOD0PatchNumber"));
	vec.push_back(language.GetCurrentTypeValue("ModelPropertiesLOD1PatchNumber"));

	vec.push_back(language.GetCurrentTypeValue("FacePropertiesVolume"));
	vec.push_back(language.GetCurrentTypeValue("FacePropertiesArea"));

	
	if (find(vec.begin(), vec.end(), key) != vec.end())
	{
		return;
	}
	
	if (!m_modelExtInfo)
	{
		m_modelExtInfo = new ModelExtInfo();
	}
	
	if (!m_modelExtInfo->m_shapeProperty)
	{
		m_modelExtInfo->m_shapeProperty = new ShapeProperty();
	}

	if (this->m_modelExtInfo && this->m_modelExtInfo->m_shapeProperty)
	{
		vector<ShapePropertyItem>& propertyItems = this->m_modelExtInfo->m_shapeProperty->GetProperties();

		bool bExist = false;
		for (int i = 0; i < propertyItems.size(); i++)
		{
			ShapePropertyItem& item = propertyItems.at(i);
			if (item.m_strTitle.compare(key) == 0)
			{
				item.m_strValue = value;
				bExist = true;
			}
		}
		if (!bExist)
		{
			ShapePropertyItem child;
			child.m_eValueType = SHAPE_PROPERTY_TypeValueUnknown;
			child.m_strTitle = key;
			child.m_strValue = value;

			propertyItems.push_back(child);
		}
	}
	
	InitProperties();
}

int Model::GetBodyCount()
{
	int count = 0;
	if (GetBodys())
	{
		count += GetBodys()->size();
	}
	for (int i = 0; i < m_SubModelArray.size(); i++)
	{
		count += m_SubModelArray[i]->GetBodyCount();
	}
	return count;
}


void Model::SetNeedClip(bool val)
{
	if (this->m_modelShape)
	{
		m_modelShape->SetNeedClip(val);
	}
}

void Model::SetOriginTransparency(float fTrans)
{
	m_originTrans = fTrans;
}

float Model::GetOriginTransparency()
{
	return m_originTrans;
}

}
