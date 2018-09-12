#ifndef _H_ANIMATIONCALLBACKFUNCTION_H
#define _H_ANIMATIONCALLBACKFUNCTION_H
#include <stdlib.h>
#include <vector>

#include 	"m3d/M3D.h"
#include "../SimulationCommon/Animation_def.h"
using namespace std;
namespace SVIEW
{
class View;
}
using SVIEW::View;

class M3D_API CAnimationCallBackFunction
{
public:
	CAnimationCallBackFunction(View* pView);
	virtual ~CAnimationCallBackFunction();
	void InitAinmationPlayCB();
	static void  GetModelPlcMtxCallBack(const char* plcIdPath, float fMtxAA[4][4], void* pInstance);
	static void  SetModelPlcMtxCallBack(const char* plcIdPath, const float fMtxAA[4][4], void* pInstance);
	static void  GetCameraCallBack(const char* plcIdPath, float fScale[3], float fMtxAA[4][4], void* pInstance);
	static void  SetCameraCallBack(const char* plcIdPath, const float fScale[3],
			const float fMtxAA[4][4], void* pInstance);
	static void  GetCameraTargetPntCallBack(const char* plcIdPath, float fTargetPnt[3], void* pInstance);
	static bool  PlayVisibleCallBack(const char* plcIdPath, const char* name, const bool bOnOff,
			const float fTran, void* pInstance);
	static bool  PlayColorCallBack(const char* plcIdPath, const char* name,
			const float fColor[3], void* pInstance);
	static bool  PlayAnimationImageCallBack(const char* plcIdPath, const char* name,
			const float fPos[3], const float fScale[3], const bool bOnOffFlg, void* pInstance);
	static void  UpdateViewCallBack(void* pBehaviorManager, void* pInstance);
	static void  PlayBeginCallBack(void* pBehaviorManager, void* pInstance);
	static void  PlayEndCallBack(void* pBehaviorManager, void* pInstance);
	
	static bool  CollisionCallBack(const char* plcIdPath, void* pInstance);
	static void  InitColisionDataCallBack(void* pInstance);
	static void  ClearColisionDataCallBack(void* pInstance);
	static void  SetTargetStateCallBack(vector<PTARGETOBJECTINFO>& vecTarget, void* pInstance);

	static void  GetCameraFocalCallBack(float& fPosTargetDistance, void* pInstance);
	

	void SetCameraState(const PTARGETOBJECTINFO& cameraInfo);
	void SetObjectState(const PTARGETOBJECTINFO& cameraInfo);
	void SetPMIState(const PTARGETOBJECTINFO& cameraInfo);
	static void  PlayClipPlaneCallBack(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg, void* pInstance);
	static void  PlayToolAnimationCallBack(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath, void* pInstance);

	static void  LockViewCallBack(bool bLock, void* pInstance);

private:
	View* m_pView;
};
#endif //_H_ANIMATIONCALLBACKFUNCTION_H
