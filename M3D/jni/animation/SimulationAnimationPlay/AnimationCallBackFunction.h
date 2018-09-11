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
	static void __stdcall GetModelPlcMtxCallBack(const char* plcIdPath, float fMtxAA[4][4], void* pInstance);
	static void __stdcall SetModelPlcMtxCallBack(const char* plcIdPath, const float fMtxAA[4][4], void* pInstance);
	static void __stdcall GetCameraCallBack(const char* plcIdPath, float fScale[3], float fMtxAA[4][4], void* pInstance);
	static void __stdcall SetCameraCallBack(const char* plcIdPath, const float fScale[3],
			const float fMtxAA[4][4], void* pInstance);
	static void __stdcall GetCameraTargetPntCallBack(const char* plcIdPath, float fTargetPnt[3], void* pInstance);
	static bool __stdcall PlayVisibleCallBack(const char* plcIdPath, const char* name, const bool bOnOff,
			const float fTran, void* pInstance);
	static bool __stdcall PlayColorCallBack(const char* plcIdPath, const char* name,
			const float fColor[3], void* pInstance);
	static bool __stdcall PlayAnimationImageCallBack(const char* plcIdPath, const char* name,
			const float fPos[3], const float fScale[3], const bool bOnOffFlg, void* pInstance);
	static void __stdcall UpdateViewCallBack(void* pBehaviorManager, void* pInstance);
	static void __stdcall PlayBeginCallBack(void* pBehaviorManager, void* pInstance);
	static void __stdcall PlayEndCallBack(void* pBehaviorManager, void* pInstance);
	
	static bool __stdcall CollisionCallBack(const char* plcIdPath, void* pInstance);
	static void __stdcall InitColisionDataCallBack(void* pInstance);
	static void __stdcall ClearColisionDataCallBack(void* pInstance);
	static void __stdcall SetTargetStateCallBack(vector<PTARGETOBJECTINFO>& vecTarget, void* pInstance);

	static void __stdcall GetCameraFocalCallBack(float& fPosTargetDistance, void* pInstance);
	

	void SetCameraState(const PTARGETOBJECTINFO& cameraInfo);
	void SetObjectState(const PTARGETOBJECTINFO& cameraInfo);
	void SetPMIState(const PTARGETOBJECTINFO& cameraInfo);
	static void __stdcall PlayClipPlaneCallBack(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg, void* pInstance);
	static void __stdcall PlayToolAnimationCallBack(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath, void* pInstance);

	static void __stdcall LockViewCallBack(bool bLock, void* pInstance);

private:
	View* m_pView;
};
#endif //_H_ANIMATIONCALLBACKFUNCTION_H
