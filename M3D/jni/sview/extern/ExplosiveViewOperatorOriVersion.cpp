#include <stdlib.h>
//#include <unistd.h>

#include "Utility.h"

#include "sview/extern/ExplosiveViewOperatorOriVersion.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/SceneManager.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/views/View.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/utils/Trackball.h"
#include "m3d/base/Vector2.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/model/Model.h"
#include "m3d/model/ModelShape.h"

using namespace SVIEW;

namespace M3D
{
const int ExplosiveViewOperatorOriVersion::MAX_ASSEMBLY_LEVLE = 32; //默认最大的爆炸装配级别

ExplosiveViewOperatorOriVersion::ExplosiveViewOperatorOriVersion():m_screenDepth(0.001)
{
	this->SetExplosiveLevel(MAX_ASSEMBLY_LEVLE);
	this->Reset();
}

ExplosiveViewOperatorOriVersion::~ExplosiveViewOperatorOriVersion()
{

}

void ExplosiveViewOperatorOriVersion::Reset()
{
	this->m_explosiveStyle = NOEXPLOSIVE;
	m_explosivePercent = 0;
	m_isUseAnimation = false;
	m_isFirstOpen = true;

	m_allNodeMatrixsCache.clear();
}

void ExplosiveViewOperatorOriVersion::SetView(View* view)
{
	this->m_view = view;
}

View* ExplosiveViewOperatorOriVersion::GetView()
{
	return this->m_view;
}

bool ExplosiveViewOperatorOriVersion::SetPercent(View* view, int style, float percent,
		bool useAnimation)
{
	if(this->m_isFirstOpen)
	{
		this->Reset();
		if(view!= NULL)
		{
//            view->RestoreView();
		}

		this->m_isFirstOpen = false;
		this->CacheMatrixState();
	}

	bool setExplosiveState = false;

	//LOGE("SetPercent m_explosiveStyle: %d style: %d", m_explosiveStyle, style);
	if (style != this->m_explosiveStyle || percent != this->m_explosivePercent)
	{
		this->m_explosiveStyle = style;

		this->m_explosivePercent = percent/50.0f;
		this->m_view = view;
		this->m_isUseAnimation = useAnimation;
		switch (m_explosiveStyle)
		{
		case AWAYFROMCENTER:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_X:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Y:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Z:
		{
			ProcressAwayFromCenter();
		}
		break;
        case AWAYFROMCENTER_X_LEFT:
        {
                ProcressAwayFromCenter();
        }
        break;
        case AWAYFROMCENTER_Y_FRONT:
        {
        ProcressAwayFromCenter();
        }
        break;
        case AWAYFROMCENTER_Z_BOTTOM:
        {
        ProcressAwayFromCenter();
        }
        break;
		case TOBALLSURFACE:
		{
			ProcressAwayToBallSurface();
		}
			break;
		default:
			//ProcressAwayFromCenter();
			break;
		}
	}

	return setExplosiveState;
}
bool ExplosiveViewOperatorOriVersion::SetPercentWithoutRestore(View * view, int style, float percent, bool useAnimation)
{

	if (this->m_isFirstOpen)
	{
		this->Reset();
		this->m_isFirstOpen = false;
		this->CacheMatrixState();
	}

	bool setExplosiveState = false;

	//LOGE("SetPercent m_explosiveStyle: %d style: %d", m_explosiveStyle, style);
	if (style != this->m_explosiveStyle || percent != this->m_explosivePercent)
	{
		this->m_explosiveStyle = style;

		this->m_explosivePercent = percent / 50.0f;
		this->m_view = view;
		this->m_isUseAnimation = useAnimation;
		switch (m_explosiveStyle)
		{
		case AWAYFROMCENTER:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_X:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Y:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Z:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_X_LEFT:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Y_FRONT:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_Z_BOTTOM:
		{
			ProcressAwayFromCenter();
		}
		break;
		case TOBALLSURFACE:
		{
			ProcressAwayToBallSurface();
		}
		break;
		default:
			//ProcressAwayFromCenter();
			break;
		}
	}

	return setExplosiveState;
}
bool ExplosiveViewOperatorOriVersion::Close(View* view)
{
	bool closeExplosiveState = false;

	SetPercent(view, this->m_explosiveStyle,0,false);

	this->Reset();

	if(view != NULL)
	{
		//view->RestoreView();
	}

	return closeExplosiveState;
}

bool ExplosiveViewOperatorOriVersion::ProcressAwayFromCenter()
{
	bool explosiveState = false;
	if (this->m_view != NULL)
	{
		View* view = this->m_view;
		//LOGE("ProcressAwayFromCenter step1");
		//this->m_TopNodeCenter = view->GetSceneManager()->GetSceneBox().Center();;

		//LOGE("ProcressAwayFromCenter step2");
		CallBackAction* explosiveAction = new CallBackAction;
		explosiveAction->SetApplyType(Action::KEEPING);
		explosiveAction->SetActionFun(AwayFromCenterCallback);
		explosiveAction->SetActionData(this);

		view->GetSceneManager()->ExecuteAction(explosiveAction);

		delete explosiveAction;

		view->GetSceneManager()->RequestUpdateWhenSceneBoxChanged();
		//LOGE("ProcressAwayFromCenter step3");
	}
	return explosiveState;
}

void ExplosiveViewOperatorOriVersion::AwayFromCenterCallback(void* data, Model* node)
{
	if (node)
	{
		bool currentNodeCanExplosive = false;
		ExplosiveViewOperatorOriVersion* explosiveViewOperator =
				(ExplosiveViewOperatorOriVersion*) data;
		int exploveLevel = explosiveViewOperator->GetExplosiveLevel();
		//爆炸到所有级别
		if (exploveLevel < MAX_ASSEMBLY_LEVLE)
		{
			int level = PathHelper::GetPathLevel(node->GetName());

			int nodeType = node->GetType();

			if (exploveLevel == level && nodeType == SHAPE_MODEL)
			{
				currentNodeCanExplosive = true;
			}
		}else
		{
			currentNodeCanExplosive = true;
		}
		//由于所有的显示内容存储在shapeNode中， 仅对shapeNode进行移动处理，不对上级GroupNode进行处理
		if (currentNodeCanExplosive)
		{
			Vector3 center;
			ModelShape* shapeNode =node->GetModelShape();
			Matrix3x4 modelMatrix1;
			Matrix3x4 tModelMatrix;

			NodeStateCache nodestate;
			bool hasModelMatrix = explosiveViewOperator->GetMatrix(node,
					nodestate);
			if (hasModelMatrix)
			{
				//获取节点点ModelMatix
				tModelMatrix = nodestate.matrixCache;
				BoundingBox& shapeNodeBox = nodestate.boxCache;
				Vector3 disPos = shapeNodeBox.Center();

				Vector3 mov;

				Vector3 tDisPos = disPos;

				if (explosiveViewOperator->m_explosiveStyle == AWAYFROMCENTER)
				{
					center = explosiveViewOperator->m_TopNodeCenter;
				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_X)
				{ //X direction
					center = explosiveViewOperator->m_TopNodeFront;
					center.m_y = tDisPos.m_y; //修改比较中心点位置
					center.m_z = tDisPos.m_z; //修改比较中心点位置
				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_Y) //Y direction
				{
					center = explosiveViewOperator->m_TopNodeLeft;
					center.m_x = tDisPos.m_x;
					center.m_z = tDisPos.m_z;
				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_Z) //Z direction
				{
					center = explosiveViewOperator->m_TopNodeBottom;
					center.m_x = tDisPos.m_x;
					center.m_y = tDisPos.m_y;

				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_X_LEFT)
				{
					center = explosiveViewOperator->m_TopNodeFront;

					disPos = GetNewPos(shapeNodeBox, tModelMatrix,
							AWAYFROMCENTER_X_LEFT);

					center.m_y = disPos.m_y; //修改比较中心点位置
					center.m_z = disPos.m_z; //修改比较中心点位置

//					disPos = modelMatrix * disPos;
				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_Y_FRONT)
				{
					center = explosiveViewOperator->m_TopNodeLeft;

					disPos = GetNewPos(shapeNodeBox, tModelMatrix,
							AWAYFROMCENTER_Y_FRONT);

					center.m_x = disPos.m_x;
					center.m_z = disPos.m_z;

//					disPos = modelMatrix * disPos;
				}
				else if (explosiveViewOperator->m_explosiveStyle
						== AWAYFROMCENTER_Z_BOTTOM)
				{
					center = explosiveViewOperator->m_TopNodeBottom;

					disPos = GetNewPos(shapeNodeBox, tModelMatrix,
							AWAYFROMCENTER_Z_BOTTOM);

					center.m_x = disPos.m_x;
					center.m_y = disPos.m_y;

//					disPos = modelMatrix * disPos;
				}
				//将顶级包围盒中心点变化到 节点空间

				disPos = tModelMatrix.Inverse() * disPos;
				center = tModelMatrix.Inverse() * center;

				mov = (disPos - center)* explosiveViewOperator->m_explosivePercent;
				//TODO 判断浮点数是否溢出
				if (mov.m_x == mov.m_x && mov.m_y == mov.m_y
						&& mov.m_z == mov.m_z)
				{
					//直接设置更新装配矩阵的值
					{
						Matrix3x4 origMatrix ;
						(origMatrix).MultiTranslate(mov);
						shapeNode->SetWorldMatrix(origMatrix);
					}

				}
				else
				{
					LOGE("explosive move error : %s", mov.Tostring().c_str());
				}
			}
		}
	}
}

void ExplosiveViewOperatorOriVersion::AwayFromCenter(const Vector3& topCenter,
		GroupNode* subNode)
{

}

bool ExplosiveViewOperatorOriVersion::ProcressAwayToBallSurface()
{
	bool explosiveState = false;

	return explosiveState;
}

void ExplosiveViewOperatorOriVersion::CacheMatrixState()
{
	if (this->m_view != NULL)
	{
		View* view = this->m_view;

		BoundingBox* box = &view->GetSceneManager()->GetSceneBox();

		Vector3 maxPos = box->m_max;
		Vector3 minPos = box->m_min;

		this->m_TopNodeCenter  = (maxPos + minPos)*0.5f;
		this->m_TopNodeBottom =  (minPos + Vector3(maxPos.m_x,maxPos.m_y,minPos.m_z))*0.5f;
		this->m_TopNodeFront =  (minPos + Vector3(minPos.m_x,maxPos.m_y,maxPos.m_z))*0.5f;
		this->m_TopNodeLeft =  (minPos + Vector3(maxPos.m_x,minPos.m_y,maxPos.m_z))*0.5f;

		CallBackAction* cacheMatrixAction = new CallBackAction;

		cacheMatrixAction->SetActionFun(CacheMatrixState);
		cacheMatrixAction->SetActionData(this);
		view->GetSceneManager()->ExecuteAction(cacheMatrixAction);
		delete cacheMatrixAction;
	}
}

void ExplosiveViewOperatorOriVersion::CacheMatrixState(void* data, Model* node)
{
	if(node)
	{
		ExplosiveViewOperatorOriVersion* explosiveViewOperator =
			(ExplosiveViewOperatorOriVersion*) data;
		int exploveLevel = explosiveViewOperator->GetExplosiveLevel();
		//爆炸到所有级别
		if(exploveLevel >= MAX_ASSEMBLY_LEVLE)
		{
				if (node != NULL&& node->GetType() == SHAPE_MODEL)
				{
					ModelShape* shapeNode = node->GetModelShape();
					if (shapeNode&&shapeNode->GetBodys()&&shapeNode->GetBodys()->size()>0)
					{
						Matrix3x4& modelMatrix = shapeNode->GetWorldTransform();

						NodeStateCache nodeState(shapeNode, modelMatrix, shapeNode->GetWorldBoundingBox());
						explosiveViewOperator->AddMatrix(node, nodeState);
					}

				}
		}
		else//爆炸到指定级别
		{
			int level = PathHelper::GetPathLevel(node->GetName());
			int nodeType = node->GetType();
			if(exploveLevel == level && nodeType == MODEL_NODE)
			{
				//Model* shapeNode = (Model*) node;
				ModelShape* shapeNode = node->GetModelShape();
				if (shapeNode&&shapeNode->GetBodys()&&shapeNode->GetBodys()->size()>0)
				{
					Matrix3x4& modelMatrix = shapeNode->GetWorldTransform();

					NodeStateCache nodeState(shapeNode, modelMatrix, shapeNode->GetWorldBoundingBox());

					explosiveViewOperator->AddMatrix(node, nodeState);
				}
			}
		}

	}
}
    
Vector3 ExplosiveViewOperatorOriVersion::GetNewPos(const BoundingBox& box,const Matrix3x4&  matrix,int newPosStyle)
{
    Vector3 newPos;
    
    BoundingBox tBox = box;

    Vector3 maxPos = tBox.m_max;
    Vector3 minPos = tBox.m_min;
    
    if(newPosStyle == AWAYFROMCENTER_Z_BOTTOM){
        newPos = (minPos + Vector3(maxPos.m_x,maxPos.m_y,minPos.m_z))*0.5f;
    }else if(newPosStyle == AWAYFROMCENTER_Y_FRONT){
        newPos =  (minPos + Vector3(minPos.m_x,maxPos.m_y,maxPos.m_z))*0.5f;
    }else if(newPosStyle == AWAYFROMCENTER_X_LEFT){
        newPos = (minPos + Vector3(maxPos.m_x,minPos.m_y,maxPos.m_z))*0.5f;
    }
 
    return newPos;
    
}

bool ExplosiveViewOperatorOriVersion::GetMatrix(Model* nodeAddr, NodeStateCache& nodeState)
{
	bool hasMatrix = false;
	if (!nodeAddr || !nodeAddr->GetModelShape())
	{
		return hasMatrix;
	}
	map<ModelShape*, NodeStateCache>::iterator it = this->m_allNodeMatrixsCache.find(nodeAddr->GetModelShape());
	if (it != this->m_allNodeMatrixsCache.end())
	{
		nodeState = it->second;
		hasMatrix = true;
	}

	return hasMatrix;
}
void ExplosiveViewOperatorOriVersion::AddMatrix(Model* nodeAddr, const NodeStateCache& nodeState)
{
	this->m_allNodeMatrixsCache.insert(pair<ModelShape*, NodeStateCache>(nodeAddr->GetModelShape(), nodeState));
}

bool ExplosiveViewOperatorOriVersion::FlatModel(View* view,vector<Model *>& shapeList,int row , int column )
{
	LOGI(" BEGIN ExplosiveViewOperatorOriVersion::FlatModel");
	bool ret = false;
	if (shapeList.size() > row * column || !view)
	{
		return ret;
	}

	CameraNode * camera = view->GetSceneManager()->GetCamera();

	if (!camera)
	{
		LOGI("CameraNode get error");
		return ret;
	}

	SceneManager * scene = view->GetSceneManager();
	scene->Lock();
	Vector3 sceneCenter = camera->GetWorldBoundingBox().Center();
	LOGI("sceneCenter = %s", sceneCenter.Tostring().c_str());

	float depth = 0.2f;
	int width = view->GetViewWidth();
	int height = view->GetViewHeight();
	int aveWidth = width / column; //每个grid的宽高
	int aveHeight = height / row;

	Ray cameraRay = camera->GetViewPort().GetScreenRay(width / 2, height / 2);
	LOGI(
			"camera ray direction = %s", cameraRay.GetDirection().Tostring().c_str());

	//构造投影平面
	Plane projPlane(cameraRay.GetDirection(), sceneCenter);

	Vector3 start = camera->GetViewPort().ScreenToWorldPoint(0, 0, depth);
	Vector3 left = camera->GetViewPort().ScreenToWorldPoint(width, 0, depth);
	Vector3 down = camera->GetViewPort().ScreenToWorldPoint(0, height, depth);

	float gridWidth = (left - start).Length() / column;
	float gridHeight = (down - start).Length() / row;

	float minLength = gridWidth < gridHeight ? gridWidth : gridHeight;
	for (int i = 0; i < shapeList.size(); i++)
	{
		Model * tempNode = shapeList.at(i);
		int tempRow = i / column; //所在行
		int tempColumn = i % column; //所在列
		int temX = tempColumn * aveWidth + aveWidth / 2;
		int temY = tempRow * aveHeight + aveHeight / 2;
		LOGI("X==%d,Y==%d", temX, temY);
		Vector3 worldPos = camera->GetViewPort().ScreenToWorldPoint(temX, temY,
				depth);
		worldPos = projPlane.Project(worldPos);

		ModelShape* modelShape = tempNode->GetModelShape();
		if(modelShape)
		{
			Matrix3x4 transformMatrix;
			BoundingBox & boundingBox = modelShape->GetWorldBoundingBox();

			float rat = minLength / boundingBox.Length();

			Vector3 scale(rat, rat, rat);

			transformMatrix.SetScale(scale); //先进性缩放
		    //transformMatrix.MultiTranslate(newPos);
		    modelShape->SetWorldMatrix(transformMatrix);
			//缩放后包围盒中心点改变，重新进算
			boundingBox = modelShape->GetWorldBoundingBox();

			Vector3 translation = worldPos - boundingBox.Center();
			Vector4 toVec4(translation, 0.0f); //需要构造成Vector4
			translation = worldPos - boundingBox.Center();

			//最后进行移动
			//TODO
			Vector3 newPos = tempNode->GetWorldTransform().Inverse()* toVec4;
		    //tempNode->Translate(newPos, TS_PARENT);
			transformMatrix = Matrix3x4::IDENTITY;
			transformMatrix.MultiTranslate(newPos);
			transformMatrix.MultiScale(scale);
		    modelShape->SetWorldMatrix(transformMatrix);
		}
	}

	ret = true;
	scene->UnLock();
	return ret;

}

int ExplosiveViewOperatorOriVersion::GetExplosiveLevel()
{
	return m_explosiveLevel;
}

void ExplosiveViewOperatorOriVersion::SetExplosiveLevel(int level)
{
	m_explosiveLevel = level;
}
float ExplosiveViewOperatorOriVersion::GetExplosivePercent()
{
	return m_explosivePercent;
}

void ExplosiveViewOperatorOriVersion::SetExplosivePercent(int percent)
{
	m_explosivePercent = percent;
}
int ExplosiveViewOperatorOriVersion::GetExplosiveStyle()
{
	return m_explosiveStyle;
}

void ExplosiveViewOperatorOriVersion::SetExplosiveStyle(int style)
{
	m_explosiveStyle = style;
}

}

