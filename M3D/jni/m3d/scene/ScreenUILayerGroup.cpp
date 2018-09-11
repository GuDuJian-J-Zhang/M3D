#include "m3d/scene/ScreenUILayerGroup.h"

#include "m3d/graphics/CameraNode.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/model/Model.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/Handler/dragger.h"
#include "m3d/Handler/Translate1DDragger.h"
#include "m3d/Handler/TranslateAxisDragger.h"
#include "m3d/Handler/RotateCylinderAxisDragger.h"
#include "m3d/Handler/ScaleAxisDragger.h"
#include "m3d/action/RenderAction.h"
#include "m3d/action/RayPickAction.h"
#include "../Handler/TranslateBoxDragger.h"
#include "../graphics/ImageBoard.h"

namespace M3D
{
	ImageModelDraggerCallback::ImageModelDraggerCallback()
	{

	}

	ImageModelDraggerCallback::~ImageModelDraggerCallback()
	{
		this->ClearModel();
	}

	bool ImageModelDraggerCallback::receive(const MotionCommand& command)
	{
		if (command.getStage() == MotionCommand::MOVE)
		{
			//this->receive(command);

			if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATELINE)
			{
				this->receive((TranslateInLineCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATEINPLANE)
			{
				this->receive((TranslateInPlaneCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_ROTATE3D)
			{
				this->receive((Rotate3DCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALE1D)
			{
				this->receive((Scale1DCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALE2D)
			{
				this->receive((Scale2DCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_SCALEUNIFROM)
			{
				this->receive((ScaleUniformCommand&)command);
			}
		}

		return false;
	}

	bool ImageModelDraggerCallback::receive(const TranslateInLineCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				//重新设置图片的大小
				if (model->GetType() == SHAPE_IMAGE_MODEL)
				{
					ImageModel* imageModel = (ImageModel*)(model);
 
					if (m_boxDragger)
					{
						Vector3 leftBottom;
						Vector3 rightTop;
						m_boxDragger->GetBoxSize(leftBottom, rightTop);

						Vector3 center = (leftBottom + rightTop) / 2;
						float width = rightTop.m_x - leftBottom.m_x;
						float height = rightTop.m_y - leftBottom.m_y;
                        Vector2 temp = Vector2(width,height);
						imageModel->SetImageSize(center,temp);
					}
				}
			}
		}

		return true;
	}

	bool ImageModelDraggerCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_draggerModels.size())
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			for (int i = 0; i < this->m_draggerModels.size(); i++)
			{
				Model* model = m_draggerModels.at(i);
				//重新设置图片的大小
				if (model->GetType() == SHAPE_IMAGE_MODEL)
				{
					ImageModel* imageModel = (ImageModel*)(model);

					if (m_boxDragger)
					{
						Vector3 leftBottom;
						Vector3 rightTop;
						m_boxDragger->GetBoxSize(leftBottom, rightTop);

						Vector3 center = (leftBottom + rightTop) / 2;
						float width = rightTop.m_x - leftBottom.m_x;
						float height = rightTop.m_y - leftBottom.m_y;
                        Vector2 temp = Vector2(width,height);
						imageModel->SetImageSize(center, temp);
					}
				}
			}
		}
		return true;
	}

	bool ImageModelDraggerCallback::AddModel(Model* model)
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			if (m_draggerModels.at(i) == model)
			{
				return false;
			}
		}
		model->AddRef();
		m_draggerModels.push_back(model);
		return true;
	}

	void ImageModelDraggerCallback::AddModels(vector<Model*> models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			this->AddModel(models.at(i));
		}
	}

	void ImageModelDraggerCallback::ClearModel()
	{
		for (int i = 0; i < m_draggerModels.size(); i++)
		{
			m_draggerModels.at(i)->Release();
		}
		m_draggerModels.clear();
	}


ScreenUILayerGroup::ScreenUILayerGroup(SceneManager* scene)
{
	this->m_scene = scene;
	m_boxDragger = NULL;
}

ScreenUILayerGroup::~ScreenUILayerGroup()
{
//不需要释放，已经挂载到父级节点中的对象，在场景节点释放时，会自动释放掉
//	if(m_rotateCenterPoint){
//		delete m_rotateCenterPoint;
//		m_rotateCenterPoint = NULL;
//	}
//
//	if(m_TransformHandlerNode){
//		delete m_TransformHandlerNode;
//		m_TransformHandlerNode = NULL;
//	}
	//ReleaseMe(m_TransformHandlerNode);
	//ReleaseMe(m_RotateCylinderAxisDragger);
	//ReleaseMe(m_ScaleAxisDragger);

	this->DeleteAllChildren();
}

int ScreenUILayerGroup::GetType( void )
{
	return SCREENLAYER_GROUP_NODE;
}

void ScreenUILayerGroup::RayPick(RayPickAction * action)
{
	GroupNode::RayPick(action);

	//构造二次拾取
	RayPickAction* hudPickAction = new RayPickAction(action->GetScene());

	hudPickAction->SetCamera(action->GetScene()->GetHudCamera());
	hudPickAction->SetPickShapeType(action->GetPickShapeType());
	hudPickAction->SetPickGeoType(action->GetPickGeoType());
	hudPickAction->SetRay(action->GetScreentPoint());

	//遍历工具的显示
	for (int i = 0; i < this->m_svlTools.size(); i++)
	{
		Model* model = this->m_svlTools.at(i);
		if (model->GetType() == SHAPE_IMAGE_MODEL)
		{
			ImageModel* imageModel = static_cast<ImageModel*>(model);
			imageModel->RayPick(hudPickAction);
		}
	}

	IShape* shape = hudPickAction->GetNearPickShape();
	Vector3 hudPickPoint = hudPickAction->GetNearestPickPoint();

	if (shape)
	{
		action->AddIntersectPnt(hudPickPoint);
		action->AddShape(shape,2);
	}

	delete hudPickAction;
}

void ScreenUILayerGroup::Traverse(Action* action)
{
	GroupNode::Traverse(action);
}

void ScreenUILayerGroup::FindVisiableObject(RenderAction* renderAction)
{
	int priRenderImageQueueIndex = renderAction->GetCurrentRenderImageQueueIndex();

	//放到HudIamges组里面去显示
	renderAction->SetCurrentRenderImageQueueIndex(2);
	for (int i = 0; i < this->m_svlTools.size(); i++)
	{
		Model* model = this->m_svlTools.at(i);
		if (model->GetType() ==  SHAPE_IMAGE_MODEL)
		{
			ImageModel* imageModel = static_cast<ImageModel*>(model);
			imageModel->FindVisiableObject(renderAction);
		}		
	}
	renderAction->SetCurrentRenderImageQueueIndex(priRenderImageQueueIndex);

	//map<int, M3D::RenderQueue>* currentRenderQueue = renderAction->GetWorkingRenderQueueGroup();
	//map<int, M3D::RenderQueue>* draggerRenderQueue = &renderAction->GetDraggerRenderQueueGroup();
	//renderAction->SetWorkingRenderQueueGroup(draggerRenderQueue);

	////GroupNode::FindVisiableObject(renderAction);

	////控制拖拽器的显示
	//if (this->m_boxDragger && this->m_boxDragger->IsVisible())
	//{
	//	m_boxDragger->FindVisiableObject(renderAction);
	//}
	//renderAction->SetWorkingRenderQueueGroup(currentRenderQueue);
}


bool  ScreenUILayerGroup::AddHudModel(Model* model)
{
	if (model == nullptr)
	{
		return false;
	}

	bool addState = false;

	bool modelAlreadyExist = false;
	for (int i =0;i<this->m_svlTools.size();i++)
	{
		if (this->m_svlTools.at(i) == model)
		{
			modelAlreadyExist = true;
		}
	}

	if (!modelAlreadyExist)
	{
		this->m_svlTools.push_back(model);
		AddRefMe(model);
		addState = true;
	}

	return addState;
}

void ScreenUILayerGroup::RemoveHudModel(Model* model)
{
	if (model == nullptr)
	{
		return;
	}

	for (int i = 0; i < this->m_svlTools.size(); i++)
	{
		if (this->m_svlTools.at(i) == model)
		{
			this->m_svlTools.erase(this->m_svlTools.begin() + i);
			break;
		}
	}
}
void ScreenUILayerGroup::RemoveHudModel(std::string strName)
{
	for (int i = 0; i < m_svlTools.size(); i++)
	{
		Model* pTmpModel = m_svlTools.at(i);
		if (pTmpModel == NULL)
			continue;
		if (pTmpModel->GetName().compare(strName) == 0)
		{
			m_svlTools.erase(m_svlTools.begin()+i);
			break;
		}
	}
}
Model * ScreenUILayerGroup::GetHudModelByName(std::string strName)
{
	Model* pModel = NULL;
	for (int i = 0; i < m_svlTools.size(); i++)
	{
		Model* pTmpModel = m_svlTools.at(i);
		if (pTmpModel == NULL)
			continue;
		if (pTmpModel->GetName().compare(strName) == 0)
		{
			pModel = pTmpModel;
			break;
		}
	}
	return pModel;
}

void ScreenUILayerGroup::BindBoxDragger(Model* model, int boxDraggerType)
{
	m_boxDragger = this->m_scene->GetHandlerGroup()->GetTranslateBoxDragger();
	if (boxDraggerType == 0)
	{
		if (model->GetType() == SHAPE_IMAGE_MODEL)
		{
			ImageModel* imageModel =(ImageModel*)(model);

			Vector3 position = model->GetWorldBoundingBox().Center();
			m_boxDragger->SetName("TranslateBoxDragger");
			m_boxDragger->SetVisible(true);
			m_boxDragger->SetWorldPosition(position);
			ImageModelDraggerCallback* draggerCallback = new ImageModelDraggerCallback();

			Vector3 imagePos = imageModel->GetImagePosition();
			Vector3 imageSize = imageModel->GetImageSize();

			Vector3 leftBottom(imagePos.m_x - imageSize.m_x/2,imagePos.m_y-imageSize.m_y/2);

			Vector3 rightTop(imagePos.m_x + imageSize.m_x / 2, imagePos.m_y + imageSize.m_y / 2);
 
			m_boxDragger->SetBoxSize(leftBottom, rightTop);

			draggerCallback->SetBoxDragger(m_boxDragger);

			draggerCallback->AddModel(model);

			draggerCallback->AddRef();
			m_boxDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
		}
	}
	else if (boxDraggerType == 1)
	{
		Vector3 position = model->GetWorldBoundingBox().Center();
		m_boxDragger->SetName("TranslateBoxDragger");
		m_boxDragger->SetVisible(true);
		m_boxDragger->SetWorldPosition(position);
		ImageModelDraggerCallback* draggerCallback = new ImageModelDraggerCallback();

		draggerCallback->SetBoxDragger(m_boxDragger);

		draggerCallback->AddModel(model);

		draggerCallback->AddRef();
		m_boxDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
}

void ScreenUILayerGroup::UnBindBoxDragger()
{
	if (this->m_boxDragger)
	{
		this->m_boxDragger->ClearDraggerCallbacks();
		this->m_boxDragger->SetVisible(false);
	}
}

}
