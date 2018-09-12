#ifndef _H_ANIMATIONPLAYAPI_H
#define _H_ANIMATIONPLAYAPI_H
#include <stdlib.h>
#include <vector>
#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/TypeDef.h"
#include "../SimulationCommon/Animation_def.h"
#include "AnimationCallBack.h"
using namespace std;


class SA_API AnimationPlayApi
{
public:
	AnimationPlayApi();
	virtual ~AnimationPlayApi();
	static void plcPathConvert1(const char *oldPlcPath, char *newPlcPath,
			bool bPre = true);

	void getModelPlcMtx(const char* plcIdPath, float fMtxAA[4][4]);
	void setModelPlcMtx(const char* plcIdPath, const float fMtxAA[4][4]);
	void getCamera(const char* plcIdPath, float fScale[3], float fMtxAA[4][4]);
	void setCamera(const char* plcIdPath, const float fScale[3],
			const float fMtxAA[4][4]);
	void getCameraFocal(float& fFocal);
	void getCameraTargetPnt(const char* plcIdPath, float fTargetPnt[3]);
	bool Play(const INT type, const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4]);
	bool PlayCamera(const char* plcIdPath, const INT type,
			const float fPivot[3], const float fMtxAA[4][4], const INT iCamType);
	bool PlayVisible(const char* plcIdPath, const char* name, const bool bOnOff,
			const float fTran);
	bool PlayColor(const char* plcIdPath, const char* name,
			const float fColor[3]);
	bool PlayAnimationImage(const char* plcIdPath, const char* name,
			const float fPos[3], const float fScale[3], const bool bOnOffFlg);
	void updateView(void* pBehaviorManager);
	void playBegin(void* pBehaviorManager);
	void playEnd(void* pBehaviorManager);
	void PlayClipPlane(const int type, const char* plcIdPath, const char* name,
			const float fNormal[3], const float fPos[3], const bool bOnOff);
	bool Collision(const char* plcIdPath);
	void InitColisionData();
	void ClearColisionData();
	void SetTargetState(vector<PTARGETOBJECTINFO>& vecTarget);

	void CalculatelTanslationMtx(const char* plcIdPath, const float fPivot[3],
			float fMtxAA[4][4]);
	void CalculatelRotationMtx(const char* plcIdPath, const float fPivot[3],
			float fMtxAA[4][4]);
	void modelTanslation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4]);
	void modelRotation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4]);
	void cameraTranslation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4]);
	void cameraRotation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4]);
	void cameraScale(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], const INT iCamType);
//	float GetFocalDistance1(const char* plcIdPath);
	void CamaraTargetPntToPosition(const char* plcIdPath,
			const float dTarget[3], const float fRotMatrix[4][4],
			float dPosition[3]);
	void CamaraPositionToTargetPnt(const char* plcIdPath,
			const float fPosition[3], const float fRotMatrix[4][4],
			float fTarget[3]);

	void PlayToolAnimation(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath);

	void LockView(bool bLock);

private:
	float m_CamaraMtx[4][4];

	float m_ClipPos[3];
	float m_ClipNormal[3];
	bool m_ClipVisible;

	bool m_bCollisionInterruptFlg;

	//记录隐藏的零件列表，切换步骤时，隐藏零件的子节点不进行处理
	vector<PTARGETOBJECTINFO> m_vecHiddenInsPath;

public:
	CAnimationCallBack* m_pAnimationCB;
};
#endif
