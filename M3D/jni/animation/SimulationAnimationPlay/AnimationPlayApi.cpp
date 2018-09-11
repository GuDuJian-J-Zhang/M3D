#include "AnimationPlayApi.h"
#include "animation/SimulationCommon/MatrixOperation.h"
#include "animation/SimulationCommon/SAUtility.h"

#include "animation/SimulationAnimationPlay/AnimationHelper.h"
#include "animation/SimulationAnimation/SimulationAnimationManager.h"
#include "animation/SimulationAnimation/SBehaviorAction.h"

#include <string>
#include <stdlib.h>

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
	m_pAnimationCB = new CAnimationCallBack();
}
AnimationPlayApi::~AnimationPlayApi()
{
	if (m_pAnimationCB)
	{
		delete m_pAnimationCB;
		m_pAnimationCB = NULL;
	}
}

void AnimationPlayApi::getModelPlcMtx(const char* plcIdPath, float fMtxAA[4][4])
{
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeGetModelPlcMtxCB(plcIdPath, fMtxAA);
	}
}

void AnimationPlayApi::setModelPlcMtx(const char* plcIdPath, const float fMtxAA[4][4])
{
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeSetModelPlcMtxCB(plcIdPath, fMtxAA);
	}
}

void AnimationPlayApi::getCamera(const char* plcIdPath,float fScale[3], float fMtxAA[4][4]) {
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeGetCameraCB(plcIdPath, fScale, fMtxAA);
	}
}

void AnimationPlayApi::setCamera(const char* plcIdPath,const float fScale[3],const float fMtxAA[4][4])
{
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeSetCameraCB(plcIdPath, fScale, fMtxAA);
	}
}

void AnimationPlayApi::getCameraFocal(float& fFocal) {
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeGetCameraFocalCB(fFocal);
	}
}

void AnimationPlayApi::getCameraTargetPnt(const char* plcIdPath,float fTargetPnt[3]) {
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokeGetCameraTargetPntCB(plcIdPath, fTargetPnt);
	}
}

bool AnimationPlayApi::PlayVisible(const char* plcIdPath, const char* name, bool bOnOff, float fTran) {

	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokePlayVisibleCB(plcIdPath, name,bOnOff,fTran);
	}
	
	return true;
}

bool AnimationPlayApi::PlayColor(const char* plcIdPath, const char* name, const float fColor[3]) {
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokePlayColorCB(plcIdPath, name, fColor);
	}

	return true;
}

//播放图片
bool AnimationPlayApi::PlayAnimationImage(const char* plcIdPath, const char* name, const float fPos[3], const float fScale[3], bool bOnOffFlg) {
	
	if (m_pAnimationCB)
	{
		m_pAnimationCB->InvokePlayImageCB(plcIdPath, name, fPos, fScale,bOnOffFlg);
	}
	return true;
}

//剖切
void AnimationPlayApi::PlayClipPlane(int type,const char* plcIdPath, const char* name, const float fNormal[3], const float fPos[3], bool bOnOff)
{
	//动画剖切效果 暂未实现
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
		const float fMtxAA[4][4])
{
	switch (type)
	{
	case 0:
		modelTanslation(plcIdPath, fPivot, fMtxAA);
		break;
	case 1:
		modelRotation(plcIdPath, fPivot, fMtxAA);
		break;
	}
	return true;
}

bool AnimationPlayApi::PlayCamera(const char* plcIdPath, const INT type,const float fPivot[3], const float fMtxAA[4][4],const INT iCamType) {
	switch (type) {
	case 0:
		cameraTranslation(plcIdPath, fPivot, fMtxAA);
		break;
	case 1:
		cameraRotation(plcIdPath, fPivot, fMtxAA);
		break;
	case 2:
		cameraScale(plcIdPath, fPivot, fMtxAA,iCamType);
		break;
	}
	return true;
}
//更新视口
void AnimationPlayApi::updateView(void* pBehaviorManager) {
	if (m_pAnimationCB)
		m_pAnimationCB->InvokeUpdateViewCB(pBehaviorManager);
}
//播放开始的处理
void AnimationPlayApi::playBegin(void* pBehaviorManager)
{
	if (m_pAnimationCB)
		m_pAnimationCB->InvokePlayBeginCB(pBehaviorManager);
}

//播放结束时的处理
void AnimationPlayApi::playEnd(void* pBehaviorManager) {
	if (m_pAnimationCB)
		m_pAnimationCB->InvokePlayEndCB(pBehaviorManager);
}

bool AnimationPlayApi::Collision(const char* plcIdPath)
{
	bool bRet = false;
	
	return bRet;
}
//初始化干涉数据
void AnimationPlayApi::InitColisionData()
{
	m_bCollisionInterruptFlg = false;
	
}

//初始化干涉数据
void AnimationPlayApi::ClearColisionData()
{
	
}

// 根据平移动画信息中的坐标矩阵及模型的当前坐标矩阵，计算动画播放后的坐标矩阵
void AnimationPlayApi::CalculatelTanslationMtx(const char* plcIdPath,
		const float fPivot[3], float fMtxAA[4][4])
{
	float PlcMtx[4][4];
	getModelPlcMtx(plcIdPath, PlcMtx);

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
		const float fPivot[3], float fMtxAA[4][4])
{
	float PlcMtx[4][4];
	getModelPlcMtx(plcIdPath, PlcMtx);

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
		const float fMtxAA[4][4])
{
	float fTempMtxAA[4][4];
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			fTempMtxAA[i][j] = fMtxAA[i][j];
		}
	}
	CalculatelTanslationMtx(plcIdPath, fPivot, fTempMtxAA);
	//__android_log_print(ANDROID_LOG_INFO,"JNIMsg","modelTanslation");
	setModelPlcMtx(plcIdPath, fTempMtxAA);
}

// 旋转
void AnimationPlayApi::modelRotation(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4])
{
	float fTempMtxAA[4][4];
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			fTempMtxAA[i][j] = fMtxAA[i][j];
		}
	}
	CalculatelRotationMtx(plcIdPath, fPivot, fTempMtxAA);
	setModelPlcMtx(plcIdPath, fTempMtxAA);

}

void AnimationPlayApi::cameraTranslation(const char* plcIdPath,const float fPivot[3],
		const float fMtxAA[4][4])
{
	m_CamaraMtx[3][0] = fMtxAA[3][0];
	m_CamaraMtx[3][1] = fMtxAA[3][1];
	m_CamaraMtx[3][2] = fMtxAA[3][2];
}

void AnimationPlayApi::cameraRotation(const char* plcIdPath, const float fPivot[3],
		const float fMtxAA[4][4])
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
		const float fMtxAA[4][4],INT iCamType)
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

		CamaraTargetPntToPosition(plcIdPath,fTarget,m_CamaraMtx,fPosition);
		m_CamaraMtx[3][0] = fPosition[0];
		m_CamaraMtx[3][1] = fPosition[1];
		m_CamaraMtx[3][2] = fPosition[2];
	}
	
	setCamera(plcIdPath,fScale,m_CamaraMtx);
}

void AnimationPlayApi::CamaraTargetPntToPosition(const char* plcIdPath, const float fTarget[3],const float fRotMatrix[4][4],float fPosition[3])
{
	float fPosTargetDistance;
	getCameraFocal(fPosTargetDistance);

	float fTargetVector[3] = {0};
	fTargetVector[0]= -fRotMatrix[2][0];
	fTargetVector[1]= -fRotMatrix[2][1];
	fTargetVector[2]= -fRotMatrix[2][2];

	//根据target向量及摄像机位置与目标点位置的距离计算新的摄像机位置
	fPosition[0] = fTarget[0] - fTargetVector[0]*fPosTargetDistance;
	fPosition[1] = fTarget[1] - fTargetVector[1]*fPosTargetDistance;
	fPosition[2] = fTarget[2] - fTargetVector[2]*fPosTargetDistance;
}

void AnimationPlayApi::CamaraPositionToTargetPnt(const char* plcIdPath, const float fPosition[3],const float fRotMatrix[4][4],float fTarget[3])
{
	float fPosTargetDistance;
	getCameraFocal(fPosTargetDistance);

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
void AnimationPlayApi::SetTargetState(vector<PTARGETOBJECTINFO>& vecTarget)
{
	if (m_pAnimationCB)
		m_pAnimationCB->InvokeSetTargetStateCB((void*)&vecTarget);
}

//插入工具库动画的回调方法
//nType 0显示 1隐藏 显隐状态  plcIdPath 实例ID  strToolPath 工具路径 :  存放路径 +“/Screwdrives/......”   strParentPath  (暂时无用)
void AnimationPlayApi::PlayToolAnimation(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath)
{
	if (m_pAnimationCB)
		m_pAnimationCB->InvokePlayToolCB(nType,plcIdPath,strToolPath,strParentPath);
}

void AnimationPlayApi::LockView(bool bLock)
{
	if (m_pAnimationCB)
		m_pAnimationCB->InvokeLockViewCB(bLock);
}



