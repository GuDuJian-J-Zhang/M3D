#include <stdlib.h>
#include "AnimationPlayApi.h"
#include "animation/SimulationCommon/MatrixOperation.h"
#include "animation/SimulationCommon/SAUtility.h"
//#include "../../sview/native/native.h"
#include "../../sview/views/View.h"
#include "../../m3d/SceneManager.h"
#include "sview/views/View.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/graphics/CameraNode.h"

#include "animation/SimulationAnimationPlay/AnimationHelper.h"

#include <string>
#include "animation/SimulationAnimation/SimulationAnimationManager.h"
#include "animation/SimulationAnimation/SBehaviorAction.h"
#include "m3d/utils/StringHelper.h"
#include "sview/views/Parameters.h"

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

AnimationPlayApi::AnimationPlayApi()
{
}
AnimationPlayApi::~AnimationPlayApi()
{

}

void AnimationPlayApi::getModelPlcMtx(const char* plcIdPath, float fMtxAA[4][4],
		View* view)
{
	string m3dModelPath(plcIdPath);
	Model* model = AnimationHelper::GetModel(m3dModelPath,view);
	AnimationHelper::GetModelPlcMatrix(model,fMtxAA);
}

void AnimationPlayApi::setModelPlcMtx(const char* plcIdPath, const float fMtxAA[4][4],
		View* view)
{
	string m3dModelPath(plcIdPath);
	Model* model = AnimationHelper::GetModel(m3dModelPath,view);
	AnimationHelper::SetModelTransform(fMtxAA,model);
    
	view->RequestDraw();
}

void AnimationPlayApi::getCamera(const char* plcIdPath,float fScale[3], float fMtxAA[4][4],View* view) {
	CameraNode* camera = view->GetSceneManager()->GetCamera();

	float zoom =1;
	AnimationHelper::GetCameraZoom(camera,zoom);

	float width = 1, height = 1;



	AnimationHelper::GetViewSize(width,height,view);
	AnimationHelper::GetCameraTransform(camera,fMtxAA);


	fScale[0] = zoom;
	fScale[1] = width;
	fScale[2] = height;
}

void AnimationPlayApi::setCamera(const char* plcIdPath,const float fScale[3],const float fMtxAA[4][4],
		View* view)
{
	if (view->AnimationGetPlayCamera())
	{
		NS_SimulationAnimation::CSimulationAnimationManager* simMgr =
				view->GetSimulationMgr();
		string animationVersion;

		if (simMgr != NULL)
		{
			animationVersion = simMgr->GetCurrentSA()->GetVersion();
		}

		float aniZoom = fScale[0];

		float aniWidth = fScale[1];
		float aniHeight = fScale[2];
		float zoom = 1 ;

		CameraNode* camera = view->GetSceneManager()->GetCamera();

		if (atof(animationVersion.c_str())>= 1.15f && ((int) fScale[1]) > 0
				&& ((int) fScale[2]) > 0)
		{
			AnimationHelper::AdjustCameraZoom(camera,aniWidth,aniHeight,aniZoom,zoom);
		}
		else
		{
			zoom = 1/ aniZoom;
		}


		AnimationHelper::SetCameraTransform(fMtxAA,camera);
		AnimationHelper::SetCameraZoom(zoom,camera);

		view->RequestDraw();
	}
}

void AnimationPlayApi::getCameraTargetPnt(const char* plcIdPath,float fTargetPnt[3],View* view) {
	AnimationHelper::GetCameraTarget(fTargetPnt,view);
}

bool AnimationPlayApi::PlayVisible(const char* plcIdPath, const char* name, bool bOnOff, float fTran, View* view) {

	if(strstr(plcIdPath,"ZOOM")!=NULL)
	{
		//局部放大框的显示隐藏
	}
	else if (strstr(name, "PMI") != NULL)
	{
		string cEntId(name+4);

		unsigned long decID;
		AnimationHelper::GetPMIID(cEntId,decID);

		PMIData* pmiData = AnimationHelper::GetPMI(decID,view);

		AnimationHelper::SetPMIVisible(bOnOff,pmiData);
	}
	else if (strlen(name) == 0)
	{
		Model* model = AnimationHelper::GetModel(plcIdPath,view);
		if (model != NULL)
		{
			AnimationHelper::SetModelVisible(bOnOff,model);
			AnimationHelper::SetModelColorAlpha(1-fTran,model);
		}
	}

	view->RequestDraw();
	
	return true;
}

bool AnimationPlayApi::PlayColor(const char* plcIdPath, const char* name, const float fColor[3], View* view) {
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
	
	Model* model = AnimationHelper::GetModel(plcIdPath,view);
	Color color(fColor[0],fColor[1],fColor[2],1-fColor[3]);
	AnimationHelper::SetModelColor(color,model);

	view->RequestDraw();

	return true;
}

//播放图片
bool AnimationPlayApi::PlayAnimationImage(const char* plcIdPath, const char* name, const float fPos[3], const float fScale[3], bool bOnOffFlg, View* view) {
	
	return true;
}

//剖切
void AnimationPlayApi::PlayClipPlane(int type,const char* plcIdPath, const char* name, const float fNormal[3], const float fPos[3], bool bOnOff, View* view)
{
	switch (type) {
	case 0:
		m_ClipPos[0] = fPos[0];
		m_ClipPos[1] = fPos[1];
		m_ClipPos[2] = fPos[2];
		break;
	case 1:
		m_ClipNormal[0] = fNormal[0];
		m_ClipNormal[1] = fNormal[1];
		m_ClipNormal[2] = fNormal[2];
		break;
	case 2:
//		if (m_AnimationPlayFun->m_PlayClipPlane == NULL)
//			return;
//		m_AnimationPlayFun->m_PlayClipPlane(plcIdPath, name, m_ClipNormal, m_ClipPos, bOnOff);
		break;
	}
}

bool AnimationPlayApi::Play(const INT type, const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4], View* view)
{
	switch (type)
	{
	case 0:
		modelTanslation(plcIdPath, fPivot, fMtxAA, view);
		break;
	case 1:
		modelRotation(plcIdPath, fPivot, fMtxAA, view);
		break;
	}
	return true;
}

bool AnimationPlayApi::PlayCamera(const char* plcIdPath, const INT type,const float fPivot[3], const float fMtxAA[4][4],const INT iCamType, View* view) {
	switch (type) {
	case 0:
		cameraTranslation(plcIdPath, fPivot, fMtxAA, view);
		break;
	case 1:
		cameraRotation(plcIdPath, fPivot, fMtxAA, view);
		break;
	case 2:
		cameraScale(plcIdPath, fPivot, fMtxAA,iCamType, view);
		break;
	}
	return true;
}
//更新视口
void AnimationPlayApi::updateView(void* pBehaviorManager, View* view) {
	
}
//播放开始的处理
void AnimationPlayApi::playBegin(View* view)
{
	
}

//播放结束时的处理
void AnimationPlayApi::playEnd(View* view) {
	
}

bool AnimationPlayApi::Collision(const char* plcIdPath, View* view)
{
	bool bRet = false;
	
	return bRet;
}
//初始化干涉数据
void AnimationPlayApi::InitColisionData(View* view)
{
	m_bCollisionInterruptFlg = false;
	
}

//初始化干涉数据
void AnimationPlayApi::ClearColisionData(View* view)
{
	
}

// 根据平移动画信息中的坐标矩阵及模型的当前坐标矩阵，计算动画播放后的坐标矩阵
void AnimationPlayApi::CalculatelTanslationMtx(const char* plcIdPath,
		const float fPivot[3], float fMtxAA[4][4], View* view)
{
	float PlcMtx[4][4];
	getModelPlcMtx(plcIdPath, PlcMtx, view);

	float modelMtx[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			modelMtx[i][j] = PlcMtx[i][j];
		}
	}

	modelMtx[3][0] = modelMtx[3][1] = modelMtx[3][2] = 0;

	float PivotMtx[4][4] = {
			1.0, 0, 0, 0,
			0, 1.0, 0, 0,
			0, 0, 1.0, 0,
			fPivot[0], fPivot[1], fPivot[2], 1.0 };

	float PivotInvMtx[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PivotInvMtx[i][j] = PivotMtx[i][j];
		}
	}

	MatrixOperation::MatrixInversion((float*)PivotInvMtx,4);

	float ResultMtx1[4][4];
	float ResultMtx2[4][4];
	MatrixOperation::Multiply((float*)PivotInvMtx, (float*)modelMtx,(float*)ResultMtx1);
	MatrixOperation::Multiply((float*)ResultMtx1, (float*)PivotMtx,(float*)ResultMtx2);
	MatrixOperation::Multiply((float*)ResultMtx2, (float*)fMtxAA,(float*)ResultMtx1);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fMtxAA[i][j] = ResultMtx1[i][j];
		}
	}
}

// 根据平移动画信息中的坐标矩阵及模型的当前坐标矩阵，计算动画播放后的坐标矩阵
void AnimationPlayApi::CalculatelRotationMtx(const char* plcIdPath,
		const float fPivot[3], float fMtxAA[4][4], View* view)
{
	float PlcMtx[4][4];
	getModelPlcMtx(plcIdPath, PlcMtx, view);

	float fPointA[3];
	fPointA[0] = fPivot[0]* PlcMtx[0][0]+fPivot[1]* PlcMtx[1][0]+fPivot[2]* PlcMtx[2][0]+PlcMtx[3][0];
	fPointA[1] = fPivot[0]* PlcMtx[0][1]+fPivot[1]* PlcMtx[1][1]+fPivot[2]* PlcMtx[2][1]+PlcMtx[3][1];
	fPointA[2] = fPivot[0]* PlcMtx[0][2]+fPivot[1]* PlcMtx[1][2]+fPivot[2]* PlcMtx[2][2]+PlcMtx[3][2];
	fPointA[0] = fPointA[0] - fPivot[0];
	fPointA[1] = fPointA[1] - fPivot[1];
	fPointA[2] = fPointA[2] - fPivot[2];
	
	float transMtx[] = {
			1.0, 0, 0, 0,
			0, 1.0, 0, 0,
			0, 0, 1.0, 0,
			fPointA[0], fPointA[1], fPointA[2], 1.0 };

	float PivotMtx[] = {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			fPivot[0], fPivot[1], fPivot[2], 1.0 };
	float PivotInvMtx[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PivotInvMtx[i * 4 + j] = PivotMtx[i * 4 + j];
		}
	}
	MatrixOperation::MatrixInversion(PivotInvMtx);


	float ResultMtx1[4][4];
	float ResultMtx2[4][4];
	
	MatrixOperation::Multiply( (float*)PivotInvMtx, (float*)fMtxAA,(float*)ResultMtx1);
	MatrixOperation::Multiply((float*)ResultMtx1,(float*)PivotMtx,(float*)ResultMtx2);
	MatrixOperation::Multiply((float*)ResultMtx2, (float*)transMtx,(float*)ResultMtx1);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fMtxAA[i][j] = ResultMtx1[i][j];
		}
	}
}

// 平移
void AnimationPlayApi::modelTanslation(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4], View* view)
{
	float fTempMtxAA[4][4];
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			fTempMtxAA[i][j] = fMtxAA[i][j];
		}
	}
	CalculatelTanslationMtx(plcIdPath, fPivot, fTempMtxAA, view);
	//__android_log_print(ANDROID_LOG_INFO,"JNIMsg","modelTanslation");
	setModelPlcMtx(plcIdPath, fTempMtxAA, view);
}

// 旋转
void AnimationPlayApi::modelRotation(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4], View* view)
{
	float fTempMtxAA[4][4];
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			fTempMtxAA[i][j] = fMtxAA[i][j];
		}
	}
	CalculatelRotationMtx(plcIdPath, fPivot, fTempMtxAA, view);
	setModelPlcMtx(plcIdPath, fTempMtxAA, view);

}

void AnimationPlayApi::cameraTranslation(const char* plcIdPath,const float fPivot[3],
		const float fMtxAA[4][4], View* view)
{
	m_CamaraMtx[3][0] = fMtxAA[3][0];
	m_CamaraMtx[3][1] = fMtxAA[3][1];
	m_CamaraMtx[3][2] = fMtxAA[3][2];
}

void AnimationPlayApi::cameraRotation(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4], View* view)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_CamaraMtx[i][j] = fMtxAA[i][j];
		}
	}
	m_CamaraMtx[0][3] = m_CamaraMtx[1][3] = m_CamaraMtx[2][3] = 0.0f;
	m_CamaraMtx[3][3] = 1.0f;
}

void AnimationPlayApi::cameraScale(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4],INT iCamType, View* view)
{
	float fScale[3] = {1.0};
	fScale[0] = fMtxAA[3][0];
	fScale[1] = fMtxAA[3][1];
	fScale[2] = fMtxAA[3][2];

	MatrixOperation::MatrixInversion((float*)m_CamaraMtx);
	if(iCamType == CameraTarget)
	{
		//float fScale[3] = {1.0};
		//float fMtxAA[4][4];
		float fTarget[3] = {0};
		float fPosition[3] = {0};
		//getCamera(plcIdPath,fScale, fMtxAA);

		fTarget[0] = m_CamaraMtx[3][0];
		fTarget[1] = m_CamaraMtx[3][1];
		fTarget[2] = m_CamaraMtx[3][2];

		CamaraTargetPntToPosition(plcIdPath,fTarget,m_CamaraMtx,fPosition,view);
		m_CamaraMtx[3][0] = fPosition[0];
		m_CamaraMtx[3][1] = fPosition[1];
		m_CamaraMtx[3][2] = fPosition[2];
	}
	
	setCamera(plcIdPath,fScale,m_CamaraMtx,view);
}

void AnimationPlayApi::CamaraTargetPntToPosition(const char* plcIdPath, const float fTarget[3],const float fRotMatrix[4][4],float fPosition[3], View* view)
{
	float fPosTargetDistance;
	AnimationHelper::GetCameraFocal(view,fPosTargetDistance);

	float fTargetVector[3] = {0};
	fTargetVector[0]= -fRotMatrix[2][0];
	fTargetVector[1]= -fRotMatrix[2][1];
	fTargetVector[2]= -fRotMatrix[2][2];

	//根据target向量及摄像机位置与目标点位置的距离计算新的摄像机位置
	fPosition[0] = fTarget[0] - fTargetVector[0]*fPosTargetDistance;
	fPosition[1] = fTarget[1] - fTargetVector[1]*fPosTargetDistance;
	fPosition[2] = fTarget[2] - fTargetVector[2]*fPosTargetDistance;
}

void AnimationPlayApi::CamaraPositionToTargetPnt(const char* plcIdPath, const float fPosition[3],const float fRotMatrix[4][4],float fTarget[3], View* view)
{
	float fPosTargetDistance;
	AnimationHelper::GetCameraFocal(view,fPosTargetDistance);

	float fTargetVector[3] = {0};
	fTargetVector[0]= -fRotMatrix[2][0];
	fTargetVector[1]= -fRotMatrix[2][1];
	fTargetVector[2]= -fRotMatrix[2][2];

	//根据target向量及摄像机位置与目标点位置的距离计算新的摄像机目标点
	fTarget[0] = fPosition[0] + fTargetVector[0]*fPosTargetDistance;
	fTarget[1] = fPosition[1] + fTargetVector[1]*fPosTargetDistance;
	fTarget[2] = fPosition[2] + fTargetVector[2]*fPosTargetDistance;
}

//设置视口中对象的状态
void AnimationPlayApi::SetTargetState(vector<PTARGETOBJECTINFO>& vecTarget, View* view)
{
	
//	for(int i = 0; i < (int)vecTarget.size(); i++)
//	{
//		char newPlcIdPath[SA_BUFFER_SIZE_SMALL];
//		if(vecTarget[i]->m_Type == TARGETOBJECT_TYPE_INS || 
//			vecTarget[i]->m_Type == TARGETOBJECT_TYPE_PMI)
//		{

	vector<PTARGETOBJECTINFO> vecInsTarget;

	for (int i = 0; i < (int)vecTarget.size(); i++)
	{
		PTARGETOBJECTINFO pTargetObjectInfo = vecTarget[i];
		if (pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_INS)
		{
			if (pTargetObjectInfo->m_bVisible == false)
			{
				m_vecHiddenInsPath.push_back(pTargetObjectInfo);
			}
		}
	}

	for(int i = 0 ;i < (int)vecTarget.size(); i++)
	{
		PTARGETOBJECTINFO pTargetObjectInfo = vecTarget[i];
		if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_CAM)
		{
			SetCameraState(pTargetObjectInfo,view);
		}
		else if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_INS)
		{
			bool bParentInsHidden = false;
			string strInsPath = pTargetObjectInfo->m_InsPath;
			for (int j = 0; j < m_vecHiddenInsPath.size(); j++)
			{
				string strHiddenInsPath = m_vecHiddenInsPath[j]->m_InsPath;
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
				SetObjectState(pTargetObjectInfo, view);
			}
		}
		else if(pTargetObjectInfo->m_Type == TARGETOBJECT_TYPE_PMI)
		{
			SetPMIState(pTargetObjectInfo,view);
		}
	}

	m_vecHiddenInsPath.clear();

//		}
//	}
}

void AnimationPlayApi::SetCameraState(const PTARGETOBJECTINFO& cameraInfo,View* view)
{
	//获取摄像机节点
	CameraNode* camera = AnimationHelper::GetCamera(view);

	//设置摄像机状态
	if (camera)
	{
		NS_SimulationAnimation::CSimulationAnimationManager* simMgr =
				view->GetSimulationMgr();
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
			AnimationHelper::AdjustCameraZoom(camera,aniWidth,aniHeight,aniZoom,zoom);
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
		AnimationHelper::SetCameraZoom(zoom,camera);

		view->RequestDraw();
	}

}

void AnimationPlayApi::SetObjectState(const PTARGETOBJECTINFO& ObjectInfo,View* view)
{
	//获取模型对象
	string m3dModelPath(ObjectInfo->m_InsPath);
	Model* model = AnimationHelper::GetModel(m3dModelPath,view);
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
		view->RequestDraw();
	}
}

void AnimationPlayApi::SetPMIState(const PTARGETOBJECTINFO& PMIInfo,View* view)
{
	//获取PMI对象
	char cEntId[SA_BUFFER_SIZE_SMALL] = {0};
	sprintf(cEntId,"%d",PMIInfo->m_EntId);

	unsigned long decID;
	AnimationHelper::GetPMIID(cEntId,decID);

	PMIData* pmiData = AnimationHelper::GetPMI(decID,view);
	if(pmiData )
	{
		//设置PMI对象状态
		AnimationHelper::SetPMIVisible(PMIInfo->m_bVisible,pmiData);
		view->RequestDraw();
	}
}
//插入工具库动画的回调方法
//nType 0显示 1隐藏 显隐状态  plcIdPath 实例ID  strToolPath 工具路径 :  存放路径 +“/Screwdrives/......”   strParentPath  (暂时无用)
void AnimationPlayApi::PlayToolAnimation(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath, View* view)
{
	//获取模型
	if (plcIdPath)
	{
        string toolsFilePath = Parameters::Instance()->m_appWorkPath + "\\data\\Standard Tool\\" + string(strToolPath);
		string toolsKey =Parameters::Instance()->m_appWorkPath + "\\data\\Standard Tool\\" + string(strToolPath) + string(plcIdPath);
        toolsKey = M3D::FileHelper::GetUnionStylePath(toolsKey);

		HandlerGroup* handlerGroup = view->GetSceneManager()->GetHandlerGroup();
		Model* toolsModel = handlerGroup->GetSVLTool(toolsKey);
		if (!toolsModel)
		{
			toolsModel = NULL;
			Model* tempModel = NULL;
			if (view->ReadSingleModel(toolsFilePath, tempModel))
			{
				//int nPlcPath = (int) strtol(plcIdPath, NULL, 16);
				string newIdString = plcIdPath;
				newIdString = M3D::StringHelper::subString(newIdString, 6, newIdString.length());
				int plcId = M3D::StringHelper::StringToInt(newIdString);
                if (tempModel) {
                    tempModel->SetPlcId(plcId);
                    handlerGroup->AddSVLTool(dynamic_cast<Model*>(tempModel), toolsKey);
                }
			}
		}
		else
		{
            if (nType==1) {
               handlerGroup->HideSVLTool(toolsKey);
            }else{
                handlerGroup->ShowSVLTool(toolsKey);
            }
            
		}
	}
	//没有获取到
}




