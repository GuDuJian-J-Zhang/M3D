/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     ANIMATION_PLYAER_HELPOER_H
#define     ANIMATION_PLYAER_HELPOER_H
#include 	"m3d/M3D.h"

namespace M3D
{
	class Model;
	class PMIData;
	class Color;
	class Matrix3x4;
	class Quaternion;
	class Vector3;
	class CameraNode;
	class ModelNode;
}
using namespace M3D;


namespace SVIEW
{
	class View;
}
using namespace SVIEW;

/**
 * @class M3D 动画播放工具类
 */
class M3D_API AnimationHelper
{
public:
	AnimationHelper();
	virtual ~AnimationHelper();

	//Model----------------------
	static Model* GetModel(const string& modelPath,View* view);
	static void SetModelColor(const Color& color,Model* model);

	static void SetModelColorAlpha(float alpha,Model* model);

	static void SetModelTransform(const float inMatrixAArray[4][4],Model* model);
	static void SetModelTransform(const Matrix3x4& transform,Model* model);
	static void SetModelRotate(const Quaternion& rotation,Model* model);
	static void SetModelPosition(const Vector3& movement,Model* model);
	static void SetModelScale(float scale,Model* model);
	static void SetModelScale(const Vector3& scale,Model* model);

	static void SetModelVisible(bool visible,Model* model);

	static void GetModelPlcMatrix(Model* model,float outMatrixAArray[4][4]);
	static void GetModelPlcMatrix(Model* model,Matrix3x4& outMatrix);

	//Camera----------------------
	static CameraNode* GetCamera(View* view);
	static void SetCameraTransform(const Matrix3x4& transform,CameraNode* camera);

	static void SetCameraTransform(const float inMatrixArray[4][4],CameraNode* camera);

	static void GetCameraTransform(CameraNode* camera,float outMatrixArray[4][4]);

	static void GetCameraTransform(CameraNode* camera,Matrix3x4& outMatrix3x4);

	static void GetCameraZoom(CameraNode* camera,float& zoom);

	//根据当前view的宽高比调整缩放比例
	static void AdjustCameraZoom(CameraNode* camera,float aniWidth,float aniHeight,
			float aniZoom, float& zoom, View* view);

	//ViewPort
	static void GetViewSize(float& outWidth,float& outHeight,View* view);

	static void SetCameraRotate(const Quaternion& rotation,CameraNode* camera);
	static void SetCameraPosition(const Vector3& movement,CameraNode* camera);
	static void SetCameraZoom(float zoom,CameraNode* camera, View* view);

	//PMI----------------------
	static PMIData* GetPMI(unsigned long pmiId,View* view);
	static void SetPMITransform(const Matrix3x4& transform,PMIData* pmiData);

	static void SetPMIRotate(const Quaternion& rotation,PMIData* pmiData);
	static void SetPMIPosition(const Vector3& movement,PMIData* pmiData);
	static void SetPMIScale(float scale,PMIData* pmiData);
	static void SetPMIScale(const Vector3& scale,PMIData* pmiData);

	static void SetPMIVisible(bool visible,PMIData* pmiData);

	static void GetPMIID(const string pmiID,unsigned long& decID);

	/**
	 * 获取摄像机目标点
	 * @param fTargetPnt
	 * @param view
	 */
	static void GetCameraTarget(float fTargetPnt[3],View* view);

	/**
	 * 获取摄像机的焦距，动画播放计算摄像机位置使用
	 * @param view
	 * @param focal
	 */
	static void GetCameraFocal(View* view,float& focal);

	static string AniDecPathToM3DHexPath(const string& AniPath);

	static string M3DDecPathToAniDecPath(const string& m3dDecPath);


private:
	static const Matrix3x4& GetPlaceMatrix(Model* model);
};
#endif //ANIMATION_PLYAER_HELPOER_H
