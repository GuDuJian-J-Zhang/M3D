#include <stdlib.h>
//#include <unistd.h>

#include "Utility.h"

#include "sview/extern/ExplosiveViewOperator.h"
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
#include "m3d/model/Note.h"
#include "m3d/extras/measure/Measure.h"
#include "m3d/extras/measure/MeasureGroup.h"
#include "math.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
using namespace SVIEW;

namespace M3D
{
const int ExplosiveViewOperator::MAX_ASSEMBLY_LEVLE = 32; //默认最大的爆炸装配级别

ExplosiveViewOperator::ExplosiveViewOperator():m_screenDepth(0.001)
{
	this->SetExplosiveLevel(MAX_ASSEMBLY_LEVLE);
	this->Reset();
}

ExplosiveViewOperator::~ExplosiveViewOperator()
{

}

void ExplosiveViewOperator::Reset()
{
	this->m_explosiveStyle = NOEXPLOSIVE;
	m_explosivePercent = 0;
	m_isUseAnimation = false;
	m_isFirstOpen = true;
	this->m_explosivePercent = 0;
	m_allNodeMatrixsCache.clear();
}

void ExplosiveViewOperator::SetView(View* view)
{
	this->m_view = view;
}

View* ExplosiveViewOperator::GetView()
{
	return this->m_view;
}
void ExplosiveViewOperator::startExplosion(vector<Model*> models) {

	if (models.size() == 0)
	{
		this->CacheMatrixState();
	}
	else
	{
		this->CacheSelectMatrixState(models);
	}
	
}

void ExplosiveViewOperator::endExplosion() {
	this->Reset();
}
bool ExplosiveViewOperator::setPercentWithDirection(View* view, vector<Model*> arrayModels, int style, float percent,
	Vector3 direction) {
	bool setExplosiveState = false;
	//if (style != this->m_explosiveStyle || percent != this->m_explosivePercent)
	{
		this->m_explosiveStyle = style;
		this->m_isSelector = true;
		this->m_explosivePercent = percent / 50.0f;
		this->m_view = view;
		this->m_isUseAnimation = true;
		this->explosiveDirection = direction;
		if (arrayModels.size() == 0)
		{
			this->CacheMatrixState();
			ProcressAwayFromCenter();
		}
		else if (arrayModels.size() > 0)
		{
			this->CacheSelectMatrixState(arrayModels);
			ProcressAwayFromCenter(arrayModels);
		}
		
	}
	return setExplosiveState;
}
bool ExplosiveViewOperator::SetPercent(View* view, vector<Model*> arrayModels, int style, float percent,
	bool useAnimation)
{

	bool setExplosiveState = false;
	this->explosiveDirection = Vector3(0,0,0);
	//LOGE("SetPercent m_explosiveStyle: %d style: %d", m_explosiveStyle, style);
	if (this->m_explosiveStyle != style)
	{
		this->m_explosiveStyle = style;
		this->m_explosivePercent = 0;
	}
	if (percent != this->m_explosivePercent)
	{
		this->m_isSelector = true;
		
		this->m_explosivePercent = percent / 50.0f + this->m_explosivePercent;
		this->m_view = view;
		this->m_isUseAnimation = useAnimation;

		this->CacheSelectMatrixState(arrayModels);
		ProcressAwayFromCenter(arrayModels);
	}

	return setExplosiveState;
}
bool ExplosiveViewOperator::SetPercent(View* view, int style, float percent,
		bool useAnimation)
{

	bool setExplosiveState = false;
	this->explosiveDirection = Vector3(0, 0, 0);
	//LOGE("SetPercent m_explosiveStyle: %d style: %d", m_explosiveStyle, style);
	if (this->m_explosiveStyle != style)
	{
		this->m_explosiveStyle = style;
		this->m_explosivePercent = 0;
		this->m_explosiveAllValue = 0;
	}
	if (percent != this->m_explosivePercent)
	{
		if ((this->m_isFirstOpen || this->m_isSelector))
		{
			this->Reset();
			if (view != NULL)
			{
				//            view->RestoreView();
			}

			this->m_isFirstOpen = false;
			this->m_isSelector = false;
		}
	
		this->m_explosivePercent = percent/50.0f;
		this->m_explosiveAllValue = m_explosiveAllValue + this->m_explosivePercent;
		this->m_view = view;
		this->m_isUseAnimation = useAnimation;

		this->CacheMatrixState();
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
		case AWAYFROMCENTER_YZ_FACE:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_XZ_FACE:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_XY_FACE:
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
bool ExplosiveViewOperator::SetPercentWithoutRestore(View * view, int style, float percent, bool useAnimation)
{
	bool setExplosiveState = false;

	//LOGE("SetPercent m_explosiveStyle: %d style: %d", m_explosiveStyle, style);
	if (style != this->m_explosiveStyle || percent != this->m_explosivePercent)
	{

		this->m_explosiveStyle = style;

		if (this->m_isFirstOpen)
		{
			this->Reset();
			this->m_isFirstOpen = false;
			this->CacheMatrixState();
		}

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
		case AWAYFROMCENTER_YZ_FACE:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_XZ_FACE:
		{
			ProcressAwayFromCenter();
		}
		break;
		case AWAYFROMCENTER_XY_FACE:
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
bool ExplosiveViewOperator::Close(View* view)
{
	bool closeExplosiveState = false;
	SceneManager* scene = m_view->GetSceneManager();
	scene->Lock();
	scene->RemoveShape(modelID);
	scene->GetRenderManager()->RequestRedraw();
	scene->UnLock();
	SetPercent(view, this->m_explosiveStyle,0,false);

	this->Reset();

	if(view != NULL)
	{
		//view->RestoreView();
	}
// // 	if (model != NULL)
// // 	{
// // 		m_view->GetSceneManager()->GetModel()->RemoveSubModel(model);
// // 	}
	return closeExplosiveState;
}
bool ExplosiveViewOperator::ProcressAwayFromCenter(vector<Model*> models)
{
	bool explosiveState = false;
	if (this->m_view != NULL)
	{
		View* view = this->m_view;
		for (int i = 0; i < models.size(); i++) 
		{
			AwayFromCenterCallback(this,models.at(i));
		}
		view->GetSceneManager()->RequestUpdateWhenSceneBoxChanged();
	
	}
	return explosiveState;
}
bool ExplosiveViewOperator::ProcressAwayFromCenter()
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

void ExplosiveViewOperator::AwayFromCenterCallback(void* data, Model* node)
{
 	if (node)
	{
		string nodeName = node->GetName();
		bool currentNodeCanExplosive = false;
		ExplosiveViewOperator* explosiveViewOperator =
				(ExplosiveViewOperator*) data;
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
			Matrix3x4 tModelMatrix;

			NodeState nodestate;
			bool hasModelMatrix = explosiveViewOperator->GetMatrix(node,
					nodestate);
			if (hasModelMatrix)
			{
				//获取节点点ModelMatix
				tModelMatrix = nodestate.matrixCache;
				BoundingBox& shapeNodeBox = nodestate.boxCache;
				Vector3 disPos = shapeNodeBox.Center();
				//Vector3 disPos = node->GetWorldBoundingBox().Center();
				Vector3 mov;

				Vector3 tDisPos = disPos;

				if (explosiveViewOperator->m_explosiveStyle == AWAYFROMCENTER)
				{ //场景中心点爆炸
					center = explosiveViewOperator->m_TopNodeCenter;
				}
				else if (explosiveViewOperator->m_explosiveStyle == AWAYFROMCENTER_YZ_FACE)
				{// YZ面方向爆炸
					center = explosiveViewOperator->m_TopNodeCenter;
					center.m_x = tDisPos.m_x;
				}
				else if (explosiveViewOperator->m_explosiveStyle == AWAYFROMCENTER_XZ_FACE)
				{// XZ面方向爆炸
					center = explosiveViewOperator->m_TopNodeCenter;
					center.m_y = tDisPos.m_y;
				}
				else if (explosiveViewOperator->m_explosiveStyle == AWAYFROMCENTER_XY_FACE)
				{// XY面方向爆炸
					center = explosiveViewOperator->m_TopNodeCenter;
					center.m_z = tDisPos.m_z;
				}
				else if (explosiveViewOperator->m_explosiveStyle
					== AWAYFROMCENTER_X)
				{ //X direction
					if (explosiveViewOperator->m_explosiveAllValue >= 0)
					{
						center = explosiveViewOperator->m_TopNodeFront;
					}
					else
					{
						center = explosiveViewOperator->m_TopNodeBehind;
					}
					
					center.m_y = tDisPos.m_y; //修改比较中心点位置
					center.m_z = tDisPos.m_z; //修改比较中心点位置
				}
				else if (explosiveViewOperator->m_explosiveStyle
					== AWAYFROMCENTER_Y) //Y direction
				{
					if (explosiveViewOperator->m_explosiveAllValue >= 0)
					{
						center = explosiveViewOperator->m_TopNodeLeft;
					}
					else
					{
						center = explosiveViewOperator->m_TopNodeRight;
					}
					center.m_x = tDisPos.m_x;
					center.m_z = tDisPos.m_z;
				}
				else if (explosiveViewOperator->m_explosiveStyle
					== AWAYFROMCENTER_Z) //Z direction
				{
					if (explosiveViewOperator->m_explosiveAllValue >= 0)
					{
						center = explosiveViewOperator->m_TopNodeBottom;
					}
					else
					{
						center = explosiveViewOperator->m_TopNodeTop;
					}
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

				//disPos = modelMatrix * disPos;
				}
				else if (explosiveViewOperator->m_explosiveStyle
					== AWAYFROMCENTER_Z_BOTTOM)
				{
					center = explosiveViewOperator->m_TopNodeBottom;

					disPos = GetNewPos(shapeNodeBox, tModelMatrix,
						AWAYFROMCENTER_Z_BOTTOM);

					center.m_x = disPos.m_x;
					center.m_y = disPos.m_y;

				//disPos = modelMatrix * disPos;
				}

				//如果是是线性爆炸有方向性
				if (explosiveViewOperator->explosiveDirection == Vector3(0, 0, 0))
				{
					//将顶级包围盒中心点变化到 节点空间
					disPos = tModelMatrix.Inverse() * disPos;
					center = tModelMatrix.Inverse() * center;
					//未指向爆炸方向，根据标准爆炸轴爆炸的功能
					if (explosiveViewOperator->m_explosiveAllValue >= 0)
					{
						mov = (disPos - center)* explosiveViewOperator->m_explosivePercent;
					}
					else
					{
						mov = (center - disPos)* explosiveViewOperator->m_explosivePercent;
					}
				}
				else
				{
					//指定了标准爆炸轴的方向爆炸
					Vector3 vect = tModelMatrix.Inverse() * Vector3(0, 0, 0);
					Vector3 direction = explosiveViewOperator->explosiveDirection;
					Plane plane;

					if (direction.m_x >= 0 && direction.m_y >= 0 && direction.m_z >= 0)
					{
						plane = Plane(direction/direction.Length(), explosiveViewOperator->p000);
					}
					else if (direction.m_x < 0 && direction.m_y >= 0 && direction.m_z >= 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p100);
					}
					else if (direction.m_x < 0 && direction.m_y < 0 && direction.m_z >= 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p110);
					}
					else if (direction.m_x >= 0 && direction.m_y < 0 && direction.m_z >= 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p010);
					}
					else if (direction.m_x >= 0 && direction.m_y >= 0 && direction.m_z < 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p001);
					}
					else if (direction.m_x < 0 && direction.m_y >= 0 && direction.m_z < 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p101);
					}
					else if (direction.m_x < 0 && direction.m_y < 0 && direction.m_z < 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p111);
					}
					else if (direction.m_x >= 0 && direction.m_y < 0 && direction.m_z < 0)
					{
						plane = Plane(direction / direction.Length(), explosiveViewOperator->p011);
					}

					float modelDistance = M3D::Abs(plane.Distance(disPos));
					float boxDistance = M3D::Abs(plane.Distance(explosiveViewOperator->m_TopNodeCenter));
					float percent = direction.Length() / boxDistance;
					mov = direction / direction.Length() * percent * modelDistance;
					mov = (tModelMatrix.Inverse() * mov- vect);
				}
				
				//TODO 判断浮点数是否溢出
				if (mov.m_x == mov.m_x && mov.m_y == mov.m_y
						&& mov.m_z == mov.m_z)
				{
					//直接设置更新装配矩阵的值
					{
						//Matrix3x4 origMatrix ;
						//(origMatrix).MultiTranslate(mov);
						//shapeNode->SetWorldMatrix(origMatrix);
     					//node->SetWorldTransform(origMatrix);
						//node->SetPlaceMatrix(origMatrix);
						//ModelAssemblyHelper::TranslateAssemblyModel(node, mov);

						//Model对象相对于某一位置移动到的绝对位置
						Model* parent = node->GetParent();
						Matrix3x4 modelWMat;
						modelWMat = tModelMatrix;//node->GetWorldTransform();
						Matrix3x4 pareWMat; //父节点世界转换矩阵
						pareWMat = parent ? parent->GetWorldTransform() : pareWMat;
						Matrix3x4 tempmat;
						tempmat.MultiTranslate(mov);
						Matrix3x4 plcMat = pareWMat.Inverse() * (tempmat * modelWMat);
						node->SetPlaceMatrix(plcMat);

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

void ExplosiveViewOperator::AwayFromCenter(const Vector3& topCenter,
		GroupNode* subNode)
{

}

bool ExplosiveViewOperator::ProcressAwayToBallSurface()
{
	bool explosiveState = false;

	return explosiveState;
}
void ExplosiveViewOperator::CacheSelectMatrixState(vector<Model*> arrayModels)
{
	
	if (this->m_view != NULL && arrayModels.size() > 0)
	{
		View* view = this->m_view;
		BoundingBox box = ((Model*)arrayModels[0])->GetWorldBoundingBox();
		
		for (int i = 1; i < arrayModels.size();i++)
		{
			box.Merge(((Model*)arrayModels[i])->GetWorldBoundingBox());
		}
		
		Vector3 maxPos = (&box)->m_max;
		Vector3 minPos = (&box)->m_min;
		if (this->arrayModels != arrayModels)
		{
			this->arrayModels = arrayModels;
			this->m_TopNodeCenter = (maxPos + minPos)*0.5f;
			this->m_TopNodeTop = (maxPos + Vector3(minPos.m_x, minPos.m_y, maxPos.m_z))*0.5f;
			this->m_TopNodeBottom = (minPos + Vector3(maxPos.m_x, maxPos.m_y, minPos.m_z))*0.5f;
			this->m_TopNodeFront = (minPos + Vector3(minPos.m_x, maxPos.m_y, maxPos.m_z))*0.5f;
			this->m_TopNodeBehind = (maxPos + Vector3(maxPos.m_x, minPos.m_y, minPos.m_z))*0.5f;
			this->m_TopNodeRight = (maxPos + Vector3(minPos.m_x, maxPos.m_y, minPos.m_z)*0.5f);
			this->m_TopNodeLeft = (minPos + Vector3(maxPos.m_x, minPos.m_y, maxPos.m_z))*0.5f;

			this->p000 = Vector3(minPos.m_x,minPos.m_y,minPos.m_z);
			this->p001 = Vector3(minPos.m_x, minPos.m_y, maxPos.m_z);
			this->p010 = Vector3(minPos.m_x, maxPos.m_y, minPos.m_z);
			this->p011 = Vector3(minPos.m_x, maxPos.m_y, maxPos.m_z);
			this->p100 = Vector3(maxPos.m_x, minPos.m_y, minPos.m_z);
			this->p101 = Vector3(maxPos.m_x, minPos.m_y, maxPos.m_z);
			this->p110 = Vector3(maxPos.m_x, maxPos.m_y, minPos.m_z);
			this->p111 = Vector3(maxPos.m_x, maxPos.m_y, maxPos.m_z);
		}
		CallBackAction* cacheMatrixAction = new CallBackAction;

		cacheMatrixAction->SetActionFun(CacheMatrixState);
		cacheMatrixAction->SetActionData(this);
		view->GetSceneManager()->ExecuteAction(cacheMatrixAction);
		delete cacheMatrixAction;

	}
}

void ExplosiveViewOperator::CacheMatrixState()
{
	if (this->m_view != NULL)
	{
		View* view = this->m_view;
		BoundingBox* box = &view->GetSceneManager()->GetSceneBox();

		Vector3 maxPos = box->m_max;
		Vector3 minPos = box->m_min;

		if (this->m_TopNodeCenter.m_x==0&& this->m_TopNodeCenter.m_y == 0 && this->m_TopNodeCenter.m_z == 0)
		{
			this->m_TopNodeCenter = (maxPos + minPos)*0.5f;
			this->m_TopNodeTop = (maxPos + Vector3(minPos.m_x, minPos.m_y, maxPos.m_z))*0.5f;
			this->m_TopNodeBottom = (minPos + Vector3(maxPos.m_x, maxPos.m_y, minPos.m_z))*0.5f;
			this->m_TopNodeFront = (minPos + Vector3(minPos.m_x, maxPos.m_y, maxPos.m_z))*0.5f;
			this->m_TopNodeBehind = (maxPos + Vector3(maxPos.m_x, minPos.m_y, minPos.m_z))*0.5f;
			this->m_TopNodeRight = (maxPos + Vector3(minPos.m_x, maxPos.m_y, minPos.m_z)*0.5f);
			this->m_TopNodeLeft = (minPos + Vector3(maxPos.m_x, minPos.m_y, maxPos.m_z))*0.5f;

		}
		{
			this->p000 = Vector3(minPos.m_x, minPos.m_y, minPos.m_z);
			this->p001 = Vector3(minPos.m_x, minPos.m_y, maxPos.m_z);
			this->p010 = Vector3(minPos.m_x, maxPos.m_y, minPos.m_z);
			this->p011 = Vector3(minPos.m_x, maxPos.m_y, maxPos.m_z);
			this->p100 = Vector3(maxPos.m_x, minPos.m_y, minPos.m_z);
			this->p101 = Vector3(maxPos.m_x, minPos.m_y, maxPos.m_z);
			this->p110 = Vector3(maxPos.m_x, maxPos.m_y, minPos.m_z);
			this->p111 = Vector3(maxPos.m_x, maxPos.m_y, maxPos.m_z);
		}
		CallBackAction* cacheMatrixAction = new CallBackAction;

		cacheMatrixAction->SetActionFun(CacheMatrixState);
		cacheMatrixAction->SetActionData(this);
		view->GetSceneManager()->ExecuteAction(cacheMatrixAction);
		delete cacheMatrixAction;

	}
}


bool ExplosiveViewOperator::AddNoteToScene(Vector3 start, Vector3 end)
{
	bool ret = false;
	SceneManager* scene = m_view->GetSceneManager();

	Line3D* m_line = new Line3D(start, end);
	m_line->SetName("dottedLine");
	m_line->SetLineWidth(2);
	m_line->SetColor(Color::RED);
	Note* baseNote = new Note;
	baseNote->m_LineList.push_back(m_line);

	if (scene && baseNote)
	{
		MeasureGroup* measureGroup = scene->GetMeasureGroup();
		ShapeNode* shapNode = new ShapeNode();
		shapNode->SetShape(baseNote);
		baseNote->SetScene(scene);

		char strID[10];
		sprintf(strID, "%d", shapNode->GetID());
		string anoteName = measureGroup->GetName() + "|" + strID;
		shapNode->SetName(anoteName);
		if (this->GetExplosivePercent() != 0)
		{
			scene->Lock();
			//删除上次绘制的辅助线
			scene->RemoveShape(modelID);
			//增加最新的辅助线
			measureGroup->AddChild(shapNode);
			scene->AddShapeIDToMap(baseNote);
			scene->GetRenderManager()->RequestRedraw();
			modelID = baseNote->GetID();
			scene->UnLock();
		}
		ret = true;

	}
	return ret;
}

void ExplosiveViewOperator::CacheMatrixState(void* data, Model* node)
{
	if(node)
	{
		ExplosiveViewOperator* explosiveViewOperator =
			(ExplosiveViewOperator*) data;
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

						NodeState nodeState(shapeNode, modelMatrix, shapeNode->GetWorldBoundingBox());
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

					NodeState nodeState(shapeNode, modelMatrix, shapeNode->GetWorldBoundingBox());

					explosiveViewOperator->AddMatrix(node, nodeState);
				}
			}
		}

	}
}
    
Vector3 ExplosiveViewOperator::GetNewPos(const BoundingBox& box,const Matrix3x4&  matrix,int newPosStyle)
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

bool ExplosiveViewOperator::GetMatrix(Model* nodeAddr, NodeState& nodeState)
{
	bool hasMatrix = false;
	if (!nodeAddr || !nodeAddr->GetModelShape())
	{
		return hasMatrix;
	}
	map<ModelShape*, NodeState>::iterator it = this->m_allNodeMatrixsCache.find(nodeAddr->GetModelShape());
	if (it != this->m_allNodeMatrixsCache.end())
	{
		nodeState = it->second;
		hasMatrix = true;
	}

	return hasMatrix;
}
void ExplosiveViewOperator::AddMatrix(Model* nodeAddr, const NodeState& nodeState)
{
    this->m_allNodeMatrixsCache.insert(pair<ModelShape*, NodeState>(nodeAddr->GetModelShape(), nodeState));
}

bool ExplosiveViewOperator::FlatModel(View* view,vector<Model *>& shapeList,int row , int column )
{
	LOGI(" BEGIN ExplosiveViewOperator::FlatModel");
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

int ExplosiveViewOperator::GetExplosiveLevel()
{
	return m_explosiveLevel;
}

void ExplosiveViewOperator::SetExplosiveLevel(int level)
{
	m_explosiveLevel = level;
}
float ExplosiveViewOperator::GetExplosivePercent()
{
	return m_explosivePercent;
}

void ExplosiveViewOperator::SetExplosivePercent(int percent)
{
	m_explosivePercent = percent;
}
int ExplosiveViewOperator::GetExplosiveStyle()
{
	return m_explosiveStyle;
}

void ExplosiveViewOperator::SetExplosiveStyle(int style)
{
	m_explosiveStyle = style;
}

void ExplosiveViewOperator::SetExplosiveMinus(int minus)
{
	m_explosiveMinus = minus;
}
}

