#ifndef _H_ANIMATIONPLAYAPI_H
#define _H_ANIMATIONPLAYAPI_H
#include <stdlib.h>
#include <vector>
#include "../SimulationCommon/TypeDef.h"
#include "../SimulationCommon/Animation_def.h"
using namespace std;
namespace SVIEW
{
class View;
}
using SVIEW::View;

class AnimationPlayApi
{
public:
	AnimationPlayApi();
	virtual ~AnimationPlayApi();
	static void plcPathConvert1(const char *oldPlcPath, char *newPlcPath,
			bool bPre = true);

	void getModelPlcMtx(const char* plcIdPath, float fMtxAA[4][4], View* view);
	void setModelPlcMtx(const char* plcIdPath, const float fMtxAA[4][4],
			View* view);
	void getCamera(const char* plcIdPath, float fScale[3], float fMtxAA[4][4],
			View* view);
	void setCamera(const char* plcIdPath, const float fScale[3],
			const float fMtxAA[4][4], View* view);
	void getCameraTargetPnt(const char* plcIdPath, float fTargetPnt[3],
			View* view);
	bool Play(const INT type, const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], View* view);
	bool PlayCamera(const char* plcIdPath, const INT type,
			const float fPivot[3], const float fMtxAA[4][4], const INT iCamType,
			View* view);
	bool PlayVisible(const char* plcIdPath, const char* name, const bool bOnOff,
			const float fTran, View* view);
	bool PlayColor(const char* plcIdPath, const char* name,
			const float fColor[3], View* view);
	bool PlayAnimationImage(const char* plcIdPath, const char* name,
			const float fPos[3], const float fScale[3], const bool bOnOffFlg,
			View* view);
	void updateView(void* pBehaviorManager, View* view);
	void playBegin(View* view);
	void playEnd(View* view);
	void PlayClipPlane(const int type, const char* plcIdPath, const char* name,
			const float fNormal[3], const float fPos[3], const bool bOnOff,
			View* view);
	bool Collision(const char* plcIdPath, View* view);
	void InitColisionData(View* view);
	void ClearColisionData(View* view);
	void SetTargetState(vector<PTARGETOBJECTINFO>& vecTarget, View* view);

	void CalculatelTanslationMtx(const char* plcIdPath, const float fPivot[3],
			float fMtxAA[4][4], View* view);
	void CalculatelRotationMtx(const char* plcIdPath, const float fPivot[3],
			float fMtxAA[4][4], View* view);
	void modelTanslation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], View* view);
	void modelRotation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], View* view);
	void cameraTranslation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], View* view);
	void cameraRotation(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], View* view);
	void cameraScale(const char* plcIdPath, const float fPivot[3],
			const float fMtxAA[4][4], const INT iCamType, View* view);
//	float GetFocalDistance1(const char* plcIdPath, View* view);
	void CamaraTargetPntToPosition(const char* plcIdPath,
			const float dTarget[3], const float fRotMatrix[4][4],
			float dPosition[3], View* view);
	void CamaraPositionToTargetPnt(const char* plcIdPath,
			const float fPosition[3], const float fRotMatrix[4][4],
			float fTarget[3], View* view);

	void SetCameraState(const PTARGETOBJECTINFO& cameraInfo, View* view);
	void SetObjectState(const PTARGETOBJECTINFO& cameraInfo, View* view);
	void SetPMIState(const PTARGETOBJECTINFO& cameraInfo, View* view);
	void PlayToolAnimation(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath,View* view);

private:
	float m_CamaraMtx[4][4];

	float m_ClipPos[3];
	float m_ClipNormal[3];
	bool m_ClipVisible;

	bool m_bCollisionInterruptFlg;

	//记录隐藏的零件列表，切换步骤时，隐藏零件的子节点不进行处理
	vector<PTARGETOBJECTINFO> m_vecHiddenInsPath;

public:

};
#endif
