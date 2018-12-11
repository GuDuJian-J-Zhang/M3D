#include <string>
#include <stdlib.h>

#include "m3d/ResourceManager.h"
#include "AnimationCallBackFunction.h"
#include "animation/SimulationCommon/MatrixOperation.h"
#include "animation/SimulationCommon/SAUtility.h"
//#include "../../sview/native/native.h"
#include "../../sview/views/View.h"
#include "../../m3d/SceneManager.h"
#include "sview/views/View.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/graphics/CameraNode.h"

#include "animation/SimulationAnimationPlay/AnimationHelper.h"
#include "animation/SimulationAnimation/SimulationAnimationManager.h"
#include "animation/SimulationAnimation/SBehaviorAction.h"
#include "m3d/utils/StringHelper.h"
#include "sview/views/Parameters.h"
#include "animation/SimulationAnimation/AnimationAPI.h"
#include "m3d/scene/ScreenUILayerGroup.h"
#include "m3d/model/Curve.h"
#include "m3d/model/Body.h"
#include "m3d/model/Edge.h"
#include "m3d/utils/Platform.h"
#include "m3d/graphics/SectionPlane.h"


using namespace SVIEW;

/*
 * 安卓系统下，还要添加以下动画播放相关接口，你们有空可以加上。不加这些接口原来安卓有的播放效果也可以正常使用，但新加的效果播放不了。

1、播放图片接口	PlayImage(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[256], const float fPos[3], const float fScale[3], bool bOnOffFlg);
plcIdPath：图片文件路径
name：图片名称
fPos：图片位置
fScale：图片大小（占视口的百分比）
bOnOffFlg：显示隐藏标志
2、播放剖面接口
PlayClipPlane(const char plcIdPath[256], const float fNormal[3], const float fPos[3], bool bOnOff);
plcIdPath：剖面对象的唯一标识
fNormal：剖面的法向
fPos：剖面的位置
3、播放声音
Bool PlaySound(const char* strSoundPath,bool bPlayFlg,float fFrom);
strSoundPath：声音文件路径（相对于动画文件的路径）
bPlayFlg：播放停止标志
fFrom：bPlayFlg为true时有效，表示从第几秒开始播放

4、获取摄像机信息
getCamera(float dScale[3], float fMtxAA[4][4]);
dScale：场景缩放信息，dScale[0]:缩放比例，dScale[1]：缩放比例为1时，视图体的宽度，dScale[2]:缩放比例为1时，视图体的高度
fMtxAA：摄像机矩阵
 */

CAnimationCallBackFunction::CAnimationCallBackFunction(View* pView)
{
	m_pView = pView;
}
CAnimationCallBackFunction::~CAnimationCallBackFunction()
{
	if (m_pView &&
		m_pView->GetSimulationMgr() &&
		m_pView->GetSimulationMgr()->GetAnimationCallBack())
	{
		CAnimationCallBack* pAnimationCallBack = m_pView->GetSimulationMgr()->GetAnimationCallBack();

		pAnimationCallBack->RemoveUpdateViewCB((UpdateViewCB)CAnimationCallBackFunction::UpdateViewCallBack, this);
		pAnimationCallBack->RemovePlayBeginCB((PlayBeginCB)CAnimationCallBackFunction::PlayBeginCallBack, this);
		pAnimationCallBack->RemovePlayEndCB((PlayEndCB)CAnimationCallBackFunction::PlayEndCallBack, this);
		pAnimationCallBack->RemoveLockViewCB((LockViewCB)CAnimationCallBackFunction::LockViewCallBack, this);
		pAnimationCallBack->RemoveSetTargetStateCB((SetTargetStateCB)CAnimationCallBackFunction::SetTargetStateCallBack, this);
		pAnimationCallBack->RemoveGetModelPlcMtxCB((GetModelPlcMtxCB)CAnimationCallBackFunction::GetModelPlcMtxCallBack, this);
		pAnimationCallBack->RemoveSetModelPlcMtxCB((SetModelPlcMtxCB)CAnimationCallBackFunction::SetModelPlcMtxCallBack, this);
		pAnimationCallBack->RemoveGetCameraCB((GetCameraCB)CAnimationCallBackFunction::GetCameraCallBack, this);
		pAnimationCallBack->RemoveSetCameraCB((SetCameraCB)CAnimationCallBackFunction::SetCameraCallBack, this);
		pAnimationCallBack->RemoveGetCameraFocalCB((GetCameraFocalCB)CAnimationCallBackFunction::GetCameraFocalCallBack, this);
		pAnimationCallBack->RemoveGetCameraTargetPntCB((GetCameraTargetPntCB)CAnimationCallBackFunction::GetCameraTargetPntCallBack, this);
		pAnimationCallBack->RemovePlayVisibleCB((PlayVisibleCB)CAnimationCallBackFunction::PlayVisibleCallBack, this);
		pAnimationCallBack->RemovePlayColorCB((PlayColorCB)CAnimationCallBackFunction::PlayColorCallBack, this);
		pAnimationCallBack->RemovePlayImageCB((PlayImageCB)CAnimationCallBackFunction::PlayAnimationImageCallBack, this);
		pAnimationCallBack->RemovePlayClipPlaneCB((PlayClipPlaneCB)CAnimationCallBackFunction::PlayClipPlaneCallBack, this);
		pAnimationCallBack->RemovePlayToolCB((PlayToolCB)CAnimationCallBackFunction::PlayToolAnimationCallBack, this);
	}
	m_pView = NULL;
}

void CAnimationCallBackFunction::InitAinmationPlayCB()
{
	if (!m_pView ||
		!m_pView->GetSimulationMgr())
		return;
	CAnimationCallBack* pAnimationCallBack = m_pView->GetSimulationMgr()->GetAnimationCallBack();
	if (!pAnimationCallBack)
		return;
	
	pAnimationCallBack->AddUpdateViewCB((UpdateViewCB)CAnimationCallBackFunction::UpdateViewCallBack, this);
	pAnimationCallBack->AddPlayBeginCB((PlayBeginCB)CAnimationCallBackFunction::PlayBeginCallBack, this);
	pAnimationCallBack->AddPlayEndCB((PlayEndCB)CAnimationCallBackFunction::PlayEndCallBack, this);
	pAnimationCallBack->AddLockViewCB((LockViewCB)CAnimationCallBackFunction::LockViewCallBack, this);
	pAnimationCallBack->AddSetTargetStateCB((SetTargetStateCB)CAnimationCallBackFunction::SetTargetStateCallBack, this);
	pAnimationCallBack->AddGetModelPlcMtxCB((GetModelPlcMtxCB)CAnimationCallBackFunction::GetModelPlcMtxCallBack, this);
	pAnimationCallBack->AddSetModelPlcMtxCB((SetModelPlcMtxCB)CAnimationCallBackFunction::SetModelPlcMtxCallBack, this);
	pAnimationCallBack->AddGetCameraCB((GetCameraCB)CAnimationCallBackFunction::GetCameraCallBack, this);
	pAnimationCallBack->AddSetCameraCB((SetCameraCB)CAnimationCallBackFunction::SetCameraCallBack, this);
	pAnimationCallBack->AddGetCameraFocalCB((GetCameraFocalCB)CAnimationCallBackFunction::GetCameraFocalCallBack, this);
	pAnimationCallBack->AddGetCameraTargetPntCB((GetCameraTargetPntCB)CAnimationCallBackFunction::GetCameraTargetPntCallBack, this);
	pAnimationCallBack->AddPlayVisibleCB((PlayVisibleCB)CAnimationCallBackFunction::PlayVisibleCallBack, this);
	pAnimationCallBack->AddPlayColorCB((PlayColorCB)CAnimationCallBackFunction::PlayColorCallBack,this);
	pAnimationCallBack->AddPlayImageCB((PlayImageCB)CAnimationCallBackFunction::PlayAnimationImageCallBack, this);
	pAnimationCallBack->AddPlayClipPlaneCB((PlayClipPlaneCB)CAnimationCallBackFunction::PlayClipPlaneCallBack, this);
	pAnimationCallBack->AddPlayToolCB((PlayToolCB)CAnimationCallBackFunction::PlayToolAnimationCallBack, this);
}

void CAnimationCallBackFunction::GetModelPlcMtxCallBack(const char* plcIdPath, float fMtxAA[4][4], void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	string m3dModelPath(plcIdPath);
	Model* model = AnimationHelper::GetModel(m3dModelPath, pAnimationCallBackFunction->m_pView);
	if (model == NULL)
	{
		string strToolKey = "PLCID:" + string(plcIdPath);
		HandlerGroup* handlerGroup = pAnimationCallBackFunction->m_pView->GetSceneManager()->GetHandlerGroup();
		model = handlerGroup->GetSVLTool(strToolKey);
	}
	AnimationHelper::GetModelPlcMatrix(model,fMtxAA);
}

void CAnimationCallBackFunction::SetModelPlcMtxCallBack(const char* plcIdPath, const float fMtxAA[4][4], void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	string m3dModelPath(plcIdPath);
	SceneManager* pSceneMgr = pAnimationCallBackFunction->m_pView->GetSceneManager();
	Model* pModel = dynamic_cast<Model*>(pSceneMgr->GetShape(AnimationHelper::AniDecPathToM3DHexPath(m3dModelPath)));
	//Model* model = AnimationHelper::GetModel(m3dModelPath, pAnimationCallBackFunction->m_pView);
	if (pModel == NULL)
	{
		string strToolKey = "PLCID:" + string(plcIdPath);
		HandlerGroup* handlerGroup = pSceneMgr->GetHandlerGroup();
		pModel = handlerGroup->GetSVLTool(strToolKey);
	}
	AnimationHelper::SetModelTransform(fMtxAA, pModel);
	pAnimationCallBackFunction->m_pView->RequestDraw();
}

void CAnimationCallBackFunction::GetCameraCallBack(const char* plcIdPath,float fScale[3], float fMtxAA[4][4], void* pInstance) 
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;
	CameraNode* camera = pAnimationCallBackFunction->m_pView->GetSceneManager()->GetCamera();

	float zoom =1;
	AnimationHelper::GetCameraZoom(camera,zoom);

	float width = 1, height = 1;



	AnimationHelper::GetViewSize(width,height, pAnimationCallBackFunction->m_pView);
	AnimationHelper::GetCameraTransform(camera,fMtxAA);


	fScale[0] = zoom;
	fScale[1] = width;
	fScale[2] = height;
}

void CAnimationCallBackFunction::SetCameraCallBack(const char* plcIdPath,const float fScale[3],const float fMtxAA[4][4], void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;

	if (pAnimationCallBackFunction->m_pView->AnimationGetPlayCamera())
	{
		NS_SimulationAnimation::CSimulationAnimationManager* simMgr =
			pAnimationCallBackFunction->m_pView->GetSimulationMgr();
		string animationVersion;

		if (simMgr != NULL)
		{
			animationVersion = simMgr->GetCurrentSA()->GetVersion();
		}
		CameraNode* camera = pAnimationCallBackFunction->m_pView->GetSceneManager()->GetCamera();
		if (!camera)
			return;

		float aniZoom = fScale[0];

		float aniWidth = fScale[1];
		float aniHeight = fScale[2];
		float zoom = 1;

		if (atof(animationVersion.c_str()) >= 1.15f && ((int)fScale[1]) > 0
			&& ((int)fScale[2]) > 0)
		{
			AnimationHelper::AdjustCameraZoom(camera, aniWidth, aniHeight, aniZoom, zoom, pAnimationCallBackFunction->m_pView);
		}
		else
		{
			zoom = 1 / aniZoom;
		}

		//AnimationHelper::SetCameraTransform(fMtxAA, camera);
		Matrix4 inMatrix4((float*)fMtxAA);
		inMatrix4 = inMatrix4.Transpose();

		Matrix3x4 transfromMatrix3x4(inMatrix4);
		
		Vector3 trans = transfromMatrix3x4.Translation();
		Quaternion rot = transfromMatrix3x4.Rotation();
		Vector3 aixs = rot.GetAixsValue();
		camera->SetTransform(trans, rot, zoom);
		
		char chLogBuf[1024];
		sprintf(chLogBuf, "Translation:%f,%f,%f\tRotation Aixs:%f,%f,%f Angle:%f\r\n",
			trans.m_x, trans.m_y, trans.m_z,
			aixs.m_z, aixs.m_y, aixs.m_z, rot.GetAngleValue());
		//AnimationHelper::SetCameraZoom(zoom, camera, pAnimationCallBackFunction->m_pView);
		LOGI(chLogBuf);
		//pAnimationCallBackFunction->m_pView->RequestDraw();
	}
}

void CAnimationCallBackFunction::GetCameraTargetPntCallBack(const char* plcIdPath,float fTargetPnt[3], void* pInstance) 
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;

	AnimationHelper::GetCameraTarget(fTargetPnt, pAnimationCallBackFunction->m_pView);
}

bool CAnimationCallBackFunction::PlayVisibleCallBack(const char* plcIdPath, const char* name, bool bOnOff, float fTran, void* pInstance) 
{
	if (!pInstance)
		return false;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return false;

	if(strstr(plcIdPath,"ZOOM")!=NULL)
	{
		//局部放大框的显示隐藏
	}
	else if (strstr(name, "PMI") != NULL)
	{
		string cEntId(name+4);

		unsigned long decID;
		AnimationHelper::GetPMIID(cEntId,decID);

		IShape* pShape = AnimationHelper::GetMeasure(decID, pAnimationCallBackFunction->m_pView);
		if (pShape)
		{
			pShape->SetVisible(bOnOff);
			pShape->SetAlpha(fTran, true);
		}
		else//批注
		{
			pShape = AnimationHelper::GetAnnotation(decID, pAnimationCallBackFunction->m_pView);
			if (pShape)
			{
				pShape->SetVisible(bOnOff);
				pShape->SetAlpha(fTran, true);
			}
		}
	}
	else if (strlen(name) == 0)
	{
		Model* model = AnimationHelper::GetModel(plcIdPath, pAnimationCallBackFunction->m_pView);
		if (model != NULL)
		{
			AnimationHelper::SetModelVisible(bOnOff,model);
			AnimationHelper::SetModelColorAlpha(1-fTran,model);
		}
	}

	pAnimationCallBackFunction->m_pView->RequestDraw();
	
	return true;
}

bool CAnimationCallBackFunction::PlayColorCallBack(const char* plcIdPath, const char* name, const float fColor[3], void* pInstance) 
{
	if (!pInstance)
		return false;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return false;

	char newPlcIdPath[SA_BUFFER_SIZE_SMALL];
	char cEntId[SA_BUFFER_SIZE_SMALL];
	strcpy(cEntId,"");
	if(name != NULL && strlen(name) > 4)
	{
		char *endptr;
		strcpy(cEntId,name+4);
		INT iEntId = strtoul(cEntId, &endptr, 10);
		sprintf(cEntId,"PMI:%x",iEntId);
	}
	
	Model* model = AnimationHelper::GetModel(plcIdPath, pAnimationCallBackFunction->m_pView);
	Color color(fColor[0],fColor[1],fColor[2],1-fColor[3]);
	AnimationHelper::SetModelColor(color,model);

	pAnimationCallBackFunction->m_pView->RequestDraw();

	return true;
}

//播放图片
bool CAnimationCallBackFunction::PlayAnimationImageCallBack(const char* plcIdPath, const char* name, const float fPos[3], const float fScale[3], bool bOnOffFlg, void* pInstance) 
{
	if (!pInstance)
		return false;
	CAnimationAPI* pAnimationAPI = CAnimationAPI::GetInstance();
	if (pAnimationAPI == NULL)
		return false;
	CSBehaviorAction * pCurBehaviorAction = pAnimationAPI->GetCurBehaviorAction();
	if (pCurBehaviorAction == NULL)
		return false;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (pAnimationCallBackFunction == NULL)
		return false;
	if (pAnimationCallBackFunction->m_pView == NULL)
		return false;
	SceneManager* pSceneManager = pAnimationCallBackFunction->m_pView->GetSceneManager();
	if (pSceneManager == NULL)
		return false;
	ScreenUILayerGroup* pScreenUILayerGroup = pSceneManager->GetScreenUILayerGroup();
	if (pScreenUILayerGroup == NULL)
		return false;
	ModelManager * pModelManager = pAnimationCallBackFunction->m_pView->GetModelManager();
	if (pModelManager == NULL)
		return false;
	bool bPlayImage = pCurBehaviorAction->IsPlayImage();
	string strName = "IMAGE:";
	strName += name;
	Model* pModel = pScreenUILayerGroup->GetHudModelByName(strName);
	if (!bPlayImage)
	{
		if (pModel)
			pScreenUILayerGroup->RemoveHudModel(pModel);
		return true;
	}
	if (bOnOffFlg)
	{
		if (pModel)
			return true;
		else//创建图片
		{
			//dScale[0]:屏幕高与图片高的比例
			//dScale[1]:屏幕宽高比
			//dScale[2]:图片宽高比
			int nViewWidth = pAnimationCallBackFunction->m_pView->GetViewWidth();
			int nViewHeight = pAnimationCallBackFunction->m_pView->GetViewHeight();
			float fCurViewScale = (float)nViewWidth / (float)nViewHeight;
			float fImgHeight = (float)nViewHeight / fScale[0];
			float fViewScale = (float)fCurViewScale / fScale[1];
			if (fViewScale <= 1)
				fImgHeight *= fViewScale;
			float fImgWidth = fImgHeight * fScale[2];
			//创建图片
			Vector3 position(0, 0, 0);
			position.m_x = fPos[0];
			position.m_y = fPos[1];
			string strFilePath = pAnimationCallBackFunction->m_pView->GetCurFilePath();
			strFilePath = strFilePath.substr(0, strFilePath.rfind('/')+1);
			strFilePath.append(plcIdPath);
			ImageModel* pImageModel = new M3D::ImageModel();
			pImageModel->SetName(strName);
			pImageModel->SetImagePath(strFilePath);
            Vector2 temp(fImgWidth, fImgHeight);
			pImageModel->SetImageSize(position, temp);
			pImageModel->SetAllowScall(true);
			pImageModel->SetAllowRotate(true);
			pImageModel->SetInTopShow(true);
			pImageModel->SetFixShowInScreen(true);
			pScreenUILayerGroup->AddHudModel(pImageModel);
		}
	}
	else if (pModel)
	{
		pScreenUILayerGroup->RemoveHudModel(pModel);
	}
	return true;
}

//更新视口
void CAnimationCallBackFunction::UpdateViewCallBack(void* pBehaviorManager, void* pInstance) 
{
	if (!pInstance)
		return;
}
//播放开始的处理
void CAnimationCallBackFunction::PlayBeginCallBack(void* pBehaviorManager, void* pInstance)
{
	if (!pInstance || !pBehaviorManager)
		return;

	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;

	if (((CSBehaviorAction*)pBehaviorManager)->IsShowTrochoid()/* &&
		((CSBehaviorAction*)pBehaviorManager)->IsPlaying()*/)
	{
		ShowAniTrochoid(pAnimationCallBackFunction);
	}
	
}

/************************************************************************/
/*    显示动画轨迹线                                                                  */
/************************************************************************/
void CAnimationCallBackFunction::ShowAniTrochoid(CAnimationCallBackFunction* pAnimationCallBackFunction)
{
	if (!pAnimationCallBackFunction->m_pView)
		return;
	CAnimationAPI* pAnimationAPI = CAnimationAPI::GetInstance();
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = pAnimationAPI->GetCurBehaviorAction();
	if (NULL == pBehaviorAction)
		return;


	SceneManager* pSceneManager = pAnimationCallBackFunction->m_pView->GetSceneManager();
	if (pSceneManager == NULL)
		return;
	HandlerGroup* pScreenHandlerGroup = pSceneManager->GetHandlerGroup();
	if (pScreenHandlerGroup == NULL)
		return;
	string strTrochoidName = "AnimationTrochoid";
	Model* pTrochoidModel = pScreenHandlerGroup->GetSVLTool(strTrochoidName);
	if (pTrochoidModel)
	{
		pScreenHandlerGroup->RemoveSVLTool(strTrochoidName);
		pTrochoidModel = NULL;
	}
	pTrochoidModel = new M3D::Model();
	pTrochoidModel->SetName(strTrochoidName);
	pTrochoidModel->SetMaterial(pSceneManager->GetResourceManager()->GetOrCreateMaterial("m3d_default_material"));
	pSceneManager->GetHandlerGroup()->AddSVLTool(pTrochoidModel, strTrochoidName);

	//int curTick = pBehaviorAction->GetCurrentTick();
	vlist_s* animationList = pBehaviorAction->GetAnimationList();
	START_LIST_ITERATION(CSAnimation, animationList)
		wstring wstrAnimationName = Platform::StringToWString(temp->GetName(), "auto");
		wstring wstrPosition = Platform::StringToWString("位置");
		if (wstrAnimationName.compare(wstrPosition)==0)
		{
			//CSTimeline* pTimeline = temp->GetTimeline();
			//int tlArrayLength = pTimeline->GetTimelineArrayLength();
			//int* tlArray = 0;
			//int beginTick = -1;
			//int endTick = -1;
			//if (tlArrayLength > 0)
			//{
			//	tlArray = pTimeline->GetTimelineArray();
			//	beginTick = tlArray[0];
			//	endTick = tlArray[tlArrayLength - 1];
			//	if (curTick >= beginTick && curTick <= endTick)
			//	{
					vector<Vector3> vecLineVertex;
					vector<M3D_INDEX_TYPE> vecLineIndex;
					GetLineVertexAndIndex(pAnimationCallBackFunction, temp, vecLineVertex, vecLineIndex);
					if((int)vecLineVertex.size()>0)
					{
						pTrochoidModel->AddLineData(vecLineVertex/*, vecLineIndex*/);
					}

					vecLineVertex.clear();
					vecLineIndex.clear();
			//	}
			//}
		}
	END_LIST_ITERATION(animationList)
	
}


/************************************************************************/
/* 获取动画的所有顶点及索引值                                                                     */
/************************************************************************/
void CAnimationCallBackFunction::GetLineVertexAndIndex(CAnimationCallBackFunction* pAnimationCallBackFunction,CSAnimation* pAnimation, vector<Vector3> &oVecLineVertex, vector<M3D_INDEX_TYPE> &oVecLineIndex)
{
	CSTimeline* pTimeline = pAnimation->GetTimeline();
	int* tlArray = 0;
	int tlArrayLength = pTimeline->GetTimelineArrayLength();
	if (tlArrayLength > 0)
		tlArray = pTimeline->GetTimelineArray();
	CAnimationAPI* pAnimationAPI = CAnimationAPI::GetInstance();
	if (!pAnimationAPI)
	{
		return;
	}

	STargetObject* pTargetObject = pAnimation->GetTarget();
	if (!pTargetObject || strcmp(pTargetObject->GetType(), TARGETOBJECTTYPE_INS))
		return;
	const char* saPlcPath = pTargetObject->GetResolvedPath();
	string m3dModelPath(saPlcPath);
	Vector3 centerPnt(0.0f,0.0f,0.0f);
	Model* model = AnimationHelper::GetModel(m3dModelPath, pAnimationCallBackFunction->m_pView);
	if (model != NULL)
	{
		Matrix3x4 insMatrix = model->GetWorldTransform();
		centerPnt = model->GetBoundingBox().Center();
		//centerPnt = insMatrix.Inverse().Multiply(centerPnt);
	}

	vlist_s* pInterpolatorList = pAnimation->GetInterpolatorList();
	for (int j = 0; j < tlArrayLength; j++)
	{
		AniPoint pivotPoint(0.0, 0.0, 0.0);
		AniPoint point(0.0, 0.0, 0.0);
		AniQuat quat(0.0, 0.0, 0.0, 1.0);
		START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
			int keyFrameCount = temp->GetArrayLength();
		if (keyFrameCount > 0 && j < keyFrameCount)
		{
			if (strcmp(temp->GetType(), INTERPOLATOR_PIVOT) == 0)
			{
				pivotPoint = ((CKeyframeChannel*)temp->GetAt(j))->m_cp;
			}
			else if (strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0)
			{
				quat = ((CKeyframeQuatSquad*)temp->GetAt(j))->m_quat;
			}
			else if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
			{
				point = ((CKeyframeChannel*)temp->GetAt(j))->m_cp;
			}
		}
		END_LIST_ITERATION(pInterpolatorList)

			
		point = CSACommonAPI::UniTanslationToMtxTanslation(pivotPoint, quat, point);

		Vector3 tmpVtx(point.x, point.y, point.z);

		//取当前状态模型包围盒中心点的位置
		Quaternion tmpQuat(quat.w, quat.x, quat.y, quat.z);
		Matrix3x4* tmpModelMatrix = new Matrix3x4(tmpVtx, tmpQuat,1.0f);
		tmpVtx = tmpModelMatrix->Multiply(centerPnt);
		
		string m3dParentModelPath = m3dModelPath;
		size_t indexCh = m3dParentModelPath.find_last_of('\\');
		if (indexCh != std::string::npos)
		{
			m3dParentModelPath = m3dParentModelPath.substr(0, indexCh);
			Model* parentModel = AnimationHelper::GetModel(m3dParentModelPath, pAnimationCallBackFunction->m_pView);
			if (parentModel != NULL)
			{
				Matrix3x4 parentInsMatrix = parentModel->GetWorldTransform();
				tmpVtx = parentInsMatrix.Multiply(tmpVtx);
			}
		}
		
		oVecLineVertex.push_back(tmpVtx);
	}

	for (int m = 0; m < (int)oVecLineVertex.size() - 1; m++)
	{
		oVecLineIndex.push_back(m);
		oVecLineIndex.push_back(m + 1);
	}
}

//播放结束时的处理
void CAnimationCallBackFunction::PlayEndCallBack(void* pBehaviorManager, void* pInstance) 
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;
	SceneManager* pSceneManager = pAnimationCallBackFunction->m_pView->GetSceneManager();
	if (pSceneManager == NULL)
		return;
	HandlerGroup* pScreenHandlerGroup = pSceneManager->GetHandlerGroup();
	if (pScreenHandlerGroup == NULL)
		return;
	string strTrochoidName = "AnimationTrochoid";
	Model* pTrochoidModel = pScreenHandlerGroup->GetSVLTool(strTrochoidName);
	if (pTrochoidModel)
	{
		pSceneManager->Lock();
		pScreenHandlerGroup->RemoveSVLTool(strTrochoidName);
		pSceneManager->UnLock();
	}
}

bool CAnimationCallBackFunction::CollisionCallBack(const char* plcIdPath, void* pInstance)
{
	bool bRet = false;
	if (!pInstance)
		return bRet;
	return bRet;
}
//初始化干涉数据
void CAnimationCallBackFunction::InitColisionDataCallBack( void* pInstance)
{
	if (!pInstance)
		return;
	
}

//初始化干涉数据
void CAnimationCallBackFunction::ClearColisionDataCallBack( void* pInstance)
{
	if (!pInstance)
		return;
}

void CAnimationCallBackFunction::GetCameraFocalCallBack(float& fPosTargetDistance, void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;
	AnimationHelper::GetCameraFocal(pAnimationCallBackFunction->m_pView,fPosTargetDistance);
}


//设置视口中对象的状态
void CAnimationCallBackFunction::SetTargetStateCallBack(vector<PTARGETOBJECTINFO>& vecTarget, void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;

	vector<PTARGETOBJECTINFO> vecHiddenIns;

	for (int i = 0; i < (int)vecTarget.size(); i++)
	{
		PTARGETOBJECTINFO pTargetObjectInfo = vecTarget[i];
		if (pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_INS)
		{
			if (pTargetObjectInfo->m_bVisible == false)
			{
				vecHiddenIns.push_back(pTargetObjectInfo);
			}
		}
	}

	for(int i = 0 ;i < (int)vecTarget.size(); i++)
	{
		PTARGETOBJECTINFO pTargetObjectInfo = vecTarget[i];
		if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_CAM)
		{
			pAnimationCallBackFunction->SetCameraState(pTargetObjectInfo);
		}
		else if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_INS)
		{
			bool bParentInsHidden = false;
			string strInsPath = pTargetObjectInfo->m_InsPath;
			for (int j = 0; j < vecHiddenIns.size(); j++)
			{
				string strHiddenInsPath = vecHiddenIns[j]->m_InsPath;
				if (strHiddenInsPath != strInsPath &&
					strHiddenInsPath == strInsPath.substr(0, strHiddenInsPath.length()) &&
					*(pTargetObjectInfo->m_InsPath + strHiddenInsPath.length()) == '\\')
				{
					bParentInsHidden = true;
					break;
				}
			}
			if(!bParentInsHidden)
			{
				pAnimationCallBackFunction->SetObjectState(pTargetObjectInfo);
			}
		}
		else if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_PMI)
		{
			pAnimationCallBackFunction->SetPMIState(pTargetObjectInfo);
		}
	}

	vecHiddenIns.clear();

//		}
//	}
}

void CAnimationCallBackFunction::SetCameraState(const PTARGETOBJECTINFO& cameraInfo)
{
	//获取摄像机节点
	CameraNode* camera = AnimationHelper::GetCamera(m_pView);

	//设置摄像机状态
	if (camera)
	{
		NS_SimulationAnimation::CSimulationAnimationManager* simMgr =
				m_pView->GetSimulationMgr();
		string animationVersion = "1.0";

		if (simMgr != NULL)
		{
			animationVersion = simMgr->GetCurrentSA()->GetVersion();
		}

		float aniZoom = cameraInfo->m_Scale[0];

		float aniWidth = cameraInfo->m_Scale[1];
		float aniHeight = cameraInfo->m_Scale[2];
		float zoom = 1 ;

		if (atof(animationVersion.c_str())>= 1.15f && ((int) cameraInfo->m_Scale[1]) > 0
				&& ((int) cameraInfo->m_Scale[2]) > 0)
		{
			AnimationHelper::AdjustCameraZoom(camera,aniWidth,aniHeight,aniZoom,zoom,m_pView);
		}
		else
		{
			zoom = aniZoom;
		}

		float fMtxAA[4][4]={0};
		MatrixOperation::FromQuat(cameraInfo->m_Quat,(float*)fMtxAA);

		fMtxAA[3][0] = cameraInfo->m_Pos[0];
		fMtxAA[3][1] = cameraInfo->m_Pos[1];
		fMtxAA[3][2] = cameraInfo->m_Pos[2];
		fMtxAA[0][3] = fMtxAA[1][3] = fMtxAA[2][3] = 0.0f;
		fMtxAA[3][3] = 1.0f;

		AnimationHelper::SetCameraTransform(fMtxAA,camera);
		AnimationHelper::SetCameraZoom(zoom,camera,m_pView);

		m_pView->RequestDraw();
	}

}

void CAnimationCallBackFunction::SetObjectState(const PTARGETOBJECTINFO& ObjectInfo)
{
	//获取模型对象
	string m3dModelPath(ObjectInfo->m_InsPath);
	Model* model = AnimationHelper::GetModel(m3dModelPath,m_pView);
	if(model)
	{
		//设置模型的状态
		float fMtxAA[4][4]={0};
		//MatrixOperation::FromQuat(ObjectInfo->m_Quat,(float*)fMtxAA);
        Quaternion rotation(ObjectInfo->m_Quat[3],ObjectInfo->m_Quat[0],
                            ObjectInfo->m_Quat[1],ObjectInfo->m_Quat[2]);
        Vector3 translation(ObjectInfo->m_Pos[0],ObjectInfo->m_Pos[1],ObjectInfo->m_Pos[2]);
        Vector3 scale(ObjectInfo->m_Scale[0],ObjectInfo->m_Scale[1],
                      ObjectInfo->m_Scale[2]);
        
        Matrix3x4 modelTransform(translation,rotation,scale);
 
		AnimationHelper::SetModelTransform(modelTransform,model);
		AnimationHelper::SetModelVisible(ObjectInfo->m_bVisible, model);
		

		model->SetVisible(ObjectInfo->m_bVisible);
		if(ObjectInfo->m_Color[0]>=0.0)
		{
		Color modelColor(ObjectInfo->m_Color[0],ObjectInfo->m_Color[1],ObjectInfo->m_Color[2],1 - ObjectInfo->m_Trans);

		model->SetColor(modelColor);
		}
		m_pView->RequestDraw();
	}
}

void CAnimationCallBackFunction::SetPMIState(const PTARGETOBJECTINFO& PMIInfo)
{
	//获取PMI对象
	char cEntId[SA_BUFFER_SIZE_SMALL] = {0};
	sprintf(cEntId,"%d",PMIInfo->m_EntId);

	unsigned long decID;
	AnimationHelper::GetPMIID(cEntId,decID);

	PMIData* pmiData = AnimationHelper::GetPMI(decID,m_pView);
	if(pmiData )
	{
		//设置PMI对象状态
		AnimationHelper::SetPMIVisible(PMIInfo->m_bVisible,pmiData);
		m_pView->RequestDraw();
	}
}

void CAnimationCallBackFunction::PlayClipPlaneCallBack(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fNormal[3], const float fPos[3], bool bOnOffFlg, void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (!pAnimationCallBackFunction->m_pView)
		return;
	SectionPlane* pSectionPlane = AnimationHelper::GetSectionPlane(plcIdPath, pAnimationCallBackFunction->m_pView);
	if (pSectionPlane == NULL)
		return;
	Vector3 vecPos(fPos[0], fPos[1], fPos[2]);
	Vector3 vecNormal(fNormal[0], fNormal[1], fNormal[2]);
	pSectionPlane->SetEnable(bOnOffFlg);
	float D = -(vecNormal.DotProduct(vecPos));
	pSectionPlane->SetPlaneParam(vecNormal.m_x, vecNormal.m_y, vecNormal.m_z, D);
}

//插入工具库动画的回调方法
//nType 0显示 1隐藏 显隐状态  plcIdPath 实例ID  strToolPath 工具路径 :  存放路径 +“/Screwdrives/......”   strParentPath  (暂时无用)
void CAnimationCallBackFunction::PlayToolAnimationCallBack(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath, void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	//获取模型
	if (plcIdPath && pAnimationCallBackFunction->m_pView)
	{
        string toolsFilePath = Parameters::Instance()->m_appWorkPath + "\\data\\Tools\\" + string(strToolPath);
		string toolsKey =/*Parameters::Instance()->m_appWorkPath + "data\\Tools\\" + string(strToolPath) + */string(plcIdPath);
        //toolsKey = M3D::FileHelper::GetUnionStylePath(toolsKey);
		HandlerGroup* handlerGroup = pAnimationCallBackFunction->m_pView->GetSceneManager()->GetHandlerGroup();
		Model* toolsModel = handlerGroup->GetSVLTool(toolsKey);
		if (!toolsModel)
		{
			pAnimationCallBackFunction->m_pView->Lock();
			toolsModel = NULL;
			Model* tempModel = NULL;
			if (pAnimationCallBackFunction->m_pView->ReadSingleModel(toolsFilePath, tempModel))
			{
				if (tempModel) {		
					//int nPlcPath = (int) strtol(plcIdPath, NULL, 16);
					string newIdString = plcIdPath;
					newIdString = M3D::StringHelper::subString(newIdString, 6, newIdString.length());
					int plcId = M3D::StringHelper::StringToInt(newIdString);
                    tempModel->SetPlcId(plcId);
                    handlerGroup->AddSVLTool(dynamic_cast<Model*>(tempModel), toolsKey);
					tempModel->Release();
                }
			}
			pAnimationCallBackFunction->m_pView->UnLock();
		}
		else
		{
			pAnimationCallBackFunction->m_pView->Lock();
            if (nType==1) {
               handlerGroup->HideSVLTool(toolsKey);
            }else{
                handlerGroup->ShowSVLTool(toolsKey);
            }
			pAnimationCallBackFunction->m_pView->UnLock();
		}
	}
	//没有获取到
}

void CAnimationCallBackFunction::LockViewCallBack(bool bLock, void* pInstance)
{
	if (!pInstance)
		return;
	CAnimationCallBackFunction* pAnimationCallBackFunction = (CAnimationCallBackFunction*)pInstance;
	if (pAnimationCallBackFunction->m_pView)
	{
		if (bLock)
		{
			pAnimationCallBackFunction->m_pView->Lock();
		}
		else
		{
			pAnimationCallBackFunction->m_pView->UnLock();
		}
	}
}



