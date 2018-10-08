#include "animation/SimulationAnimationPlay/AnimationHelper.h"

#include "m3d/scene/ModelNode.h"

#include "m3d/model/Model.h"
#include "m3d/model/PMIData.h"

#include "m3d/base/Color.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Vector3.h"
#include "m3d/M3Dmacros.h"

#include "m3d/SceneManager.h"
#include "m3d/utils/PathHelper.h"

using namespace M3D;

#include "sview/views/View.h"
#include "m3d/scene/ModelNode.h"
#include <string.h>
using namespace SVIEW;

AnimationHelper::AnimationHelper()
{

}

AnimationHelper::~AnimationHelper()
{

}

//Model----------------------
Model* AnimationHelper::GetModel(const string& modelPath, View* view)
{
	Model* model = NULL;

	string m3dModelPath = AnimationHelper::AniDecPathToM3DHexPath(modelPath);
	SceneManager* sceneMgr = view->GetSceneManager();
	SceneNode* node = sceneMgr->GetNode(m3dModelPath);
	if (node && node->GetType() == MODEL_NODE)
	{
		ModelNode* modelNode = (ModelNode*) node;
		ShapeNode* shapeNode = modelNode->GetShapeNode();
		if (shapeNode)
		{
			Shape* shape = shapeNode->GetShape();
			if (shape && shape->GetType() == SHAPETYPE::SHAPE_MODEL)
			{
				model = (Model*) shape;
			}
		}
	}

	return model;
}

void AnimationHelper::SetModelColor(const Color& color, Model* model)
{
	if (model)
	{
		if (color.m_r >= 0)
		{
			model->SetColor(color);
		}
		else //如果小于0 则说明颜色值无效，按照动画播放规则需要重置模型的颜色
		{
			model->ResetColor();
		}
	}
	return;
}

void AnimationHelper::SetModelColorAlpha(float alpha, Model* model)
{
	if (model != NULL)
	{
		model->SetAlpha(alpha, true);
	}
}

void AnimationHelper::SetModelTransform(const Matrix3x4& transform,
		Model* model)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		modelNode->SetPlcMatrix(transform);
	}
}

void AnimationHelper::SetModelTransform(const float inMatrixAArray[4][4],
		Model* model)
{
	Matrix4 inMatrix((float*) inMatrixAArray);
	inMatrix = inMatrix.Transpose();
	Matrix3x4 inMatrix3x4(inMatrix);

	AnimationHelper::SetModelTransform(inMatrix3x4, model);
}

void AnimationHelper::SetModelRotate(const Quaternion& rotation, Model* model)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		modelNode->SetRotation(rotation);
	}
}

void AnimationHelper::SetModelPosition(const Vector3& movement, Model* model)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		modelNode->SetPosition(movement);
	}
}

void AnimationHelper::SetModelScale(float scale, Model* model)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		modelNode->SetScale(Vector3(scale, scale, scale));
	}
}

void AnimationHelper::SetModelScale(const Vector3& scale, Model* model)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		modelNode->SetScale(scale);
	}
}

void AnimationHelper::GetModelPlcMatrix(Model* model,
		float outMatrixAArray[4][4])
{
	Matrix3x4 outMatrix;
	AnimationHelper::GetModelPlcMatrix(model, outMatrix);
	Matrix4 outMatrix4 = outMatrix.ToMatrix4().Transpose();

	memcpy(outMatrixAArray, outMatrix4.Data(), 16 * sizeof(float));
}

void AnimationHelper::GetModelPlcMatrix(Model* model, Matrix3x4& outMatrix)
{
	ModelNode* modelNode = AnimationHelper::GetModelNode(model);

	if (modelNode)
	{
		outMatrix = modelNode->GetPlcMatrix();
	}
}

void AnimationHelper::SetModelVisible(bool visible, Model* model)
{
	if (model)
	{
		model->SetVisible(visible);
	}
}

//Camera----------------------
CameraNode* AnimationHelper::AnimationHelper::GetCamera(View* view)
{
	CameraNode* camera = view->GetSceneManager()->GetCamera();
	return camera;
}

//ViewPort
void AnimationHelper::GetViewSize(float& outWidth, float& outHeight, View* view)
{
	CameraNode* camera = view->GetSceneManager()->GetCamera();
	if (camera != NULL)
	{
		camera->GetOrthoSize(&outWidth, &outHeight);
	}
}

void AnimationHelper::SetCameraTransform(const Matrix3x4& transform,
		CameraNode* camera)
{
	if (camera != NULL)
	{
		camera->SetTransform(transform.Translation(), transform.Rotation());
	}
}

void AnimationHelper::SetCameraTransform(const float inMatrixArray[4][4],
		CameraNode* camera)
{
	Matrix4 inMatrix4((float*) inMatrixArray);
	inMatrix4 = inMatrix4.Transpose();

	Matrix3x4 transfromMatrix3x4(inMatrix4);

	AnimationHelper::SetCameraTransform(transfromMatrix3x4, camera);
}

void AnimationHelper::GetCameraTransform(CameraNode* camera,
		Matrix3x4& outMatrix3x4)
{
	Matrix3x4 viewMatrix = camera->GetView();
	outMatrix3x4 = viewMatrix.Inverse();
}

void AnimationHelper::GetCameraTransform(CameraNode* camera,
		float outMatrixArray[4][4])
{
	Matrix3x4 outMatrix3x4;
	AnimationHelper::GetCameraTransform(camera, outMatrix3x4);

	Matrix4 retviewMatrix4 = outMatrix3x4.ToMatrix4();
	retviewMatrix4 = retviewMatrix4.Transpose();

	memcpy(outMatrixArray, retviewMatrix4.Data(), 4 * 4 * sizeof(float));
}

void AnimationHelper::GetCameraZoom(CameraNode* camera, float& zoom)
{
	if (camera)
	{
		zoom = camera->GetZoom();
	}
}

void AnimationHelper::AdjustCameraZoom(CameraNode* camera, float aniWidth,
		float aniHeight, float aniZoom, float& zoom)
{
	float width = 1, height = 1;

	if (camera != NULL)
	{
		camera->GetOrthoSize(&width, &height);
	}

	float dx = aniWidth / width;
	float dy = aniHeight / height;

	if (dx > dy)
	{
		aniZoom = aniZoom / dx;
	}
	else
	{
		aniZoom = aniZoom / dy;
	}

	if (aniZoom <= 0)
	{
		aniZoom = 1.0f;
	}

	zoom = aniZoom;
}

void AnimationHelper::SetCameraRotate(const Quaternion& rotation,
		CameraNode* camera)
{
	if (camera != NULL)
	{
		camera->SetRotation(rotation);
	}
}

void AnimationHelper::SetCameraPosition(const Vector3& position,
		CameraNode* camera)
{
	if (camera != NULL)
	{
		camera->SetPosition(position);
	}
}

void AnimationHelper::SetCameraZoom(float zoom, CameraNode* camera)
{
	if (camera != NULL)
	{
		camera->SetZoom(zoom);
	}
}

//PMI----------------------
PMIData* AnimationHelper::GetPMI(unsigned long pmiId, View* view)
{
	PMIData* pmi = NULL;
	map<int, PMIData*>& pmis = view->GetModel()->GetPMIs();
    if (pmis.size() > 0) {
        map<int, PMIData*>::iterator it = pmis.find(pmiId);
        if (it != pmis.end())
        {
            pmi = (*it).second;
            
        }
    }
	return pmi;
}

void AnimationHelper::SetPMITransform(const Matrix3x4& transform,
		PMIData* pmiData)
{

}

void AnimationHelper::SetPMIRotate(const Quaternion& rotation, PMIData* pmiData)
{

}

void AnimationHelper::SetPMIPosition(const Vector3& movement, PMIData* pmiData)
{

}

void AnimationHelper::SetPMIScale(float scale, PMIData* pmiData)
{

}

void AnimationHelper::SetPMIScale(const Vector3& scale, PMIData* pmiData)
{

}

void AnimationHelper::SetPMIVisible(bool visible, PMIData* pmiData)
{
	if (pmiData)
	{
		pmiData->SetVisible(visible);
	}
}

void AnimationHelper::GetPMIID(const string pmiID, unsigned long& decID)
{
	decID = strtoul(pmiID.data(), NULL, 10);
}

void AnimationHelper::GetCameraTarget(float fTargetPnt[3], View* view)
{
	CameraNode* camera = view->GetSceneManager()->GetCamera();

	if (camera != NULL)
	{
		float zoom = camera->GetZoom();
		float width = 1, height = 1;
		//首先获取整个场景包围盒中心，根据场景包围盒的中心构造一张平面，求摄像机当前位置和此屏幕的交点作为摄像机的目标点
		BoundingBox& worldBox = view->GetSceneManager()->GetSceneBox();
		Vector3 center = worldBox.Center();

		Plane worldCenterProjectPlane(camera->GetWorldDirection(), center);

		worldCenterProjectPlane.Project(camera->GetWorldPosition());

		Vector3 targetPoint = worldCenterProjectPlane.Project(
				camera->GetWorldPosition());

		fTargetPnt[0] = targetPoint.m_x;
		fTargetPnt[1] = targetPoint.m_y;
		fTargetPnt[2] = targetPoint.m_z;
	}
}

void AnimationHelper::GetCameraFocal(View* view, float& focal)
{
	float fPosTargetDistance = 1.0f;

	if (view)
	{
		//此代码只在平行视图下可以正确播放，将来添加透视投影视图模式后，在透视投影模式下需要获取实际的焦距
		BoundingBox box = view->GetSceneManager()->GetSceneBox();
		fPosTargetDistance = view->GetSceneManager()->GetCamera()->GetHalfClip()
				* 0.5;
	}
	focal = fPosTargetDistance;
}

ModelNode * AnimationHelper::GetModelNode(Model * model)
{
	ModelNode* ret = NULL;
	if (model)
	{
		SceneNode* tempRet = model->GetSceneNode()->GetParent();
		if (tempRet && tempRet->GetType() == MODEL_NODE)
		{
			ret = (ModelNode *) tempRet;
		}
	}
	return ret;
}

const Matrix3x4& AnimationHelper::GetPlaceMatrix(Model* model)
{
	if (model)
	{
		ModelNode * tempNode = GetModelNode(model);
		if (tempNode)
		{
			return tempNode->GetPlcMatrix();
		}

	}
	return Matrix3x4::IDENTITY;
}

string AnimationHelper::AniDecPathToM3DHexPath(const string& AniPath)
{
	char newPlcPath[256] =
	{ 0 };
	char *cPlcIdP = NULL;
	char srcPath[256] =
	{ 0 };
	char targetPath[256] =
	{ 0 };
	int wiPlcId = 0;

	strcpy(srcPath, AniPath.c_str());
	cPlcIdP = strtok(srcPath, "\\");
	while (cPlcIdP != NULL)
	{
		wiPlcId = strtoul(cPlcIdP, NULL, 10);
		wiPlcId &= 0x00FFFFFF;
		if (strcmp(newPlcPath, "") == 0)
		{
			sprintf(targetPath, "%x", wiPlcId);
		}
		else
		{
			sprintf(targetPath, "%s|%x", newPlcPath, wiPlcId);
		}

		strcpy(newPlcPath, targetPath);
		cPlcIdP = strtok(NULL, "\\");
	}

	if (strlen(newPlcPath) > 0 && newPlcPath[0] != '0')
	{
		sprintf(targetPath, "0|%s", newPlcPath);
	}

	string ret(targetPath);

	return PathHelper::M3D_PATH_PRE + "PATH|" + ret;
}

string AnimationHelper::M3DDecPathToAniDecPath(const string& m3dDecPath)
{
	return "";
}



