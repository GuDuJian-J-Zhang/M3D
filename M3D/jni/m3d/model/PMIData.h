/**@file
 *@brief	PMIData类头文件
 *@author	bing
 *@date		2013-9-29
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *PMIData类头文件
 *
 */
#ifndef M3D_PMIDATA_H_
#define M3D_PMIDATA_H_
#include "m3d/model/Shape.h"
#include "ComText.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class PolyLine;

//TODO:改为继承自Note bing

class ImageBoard;

/**@class PMIData
 * @brief PMIData类
 *
 * 表示从svl文件中读取到的PMI信息
 */
class M3D_API PMIData: public Shape, public Renderable
{
public:
	static int FixScreen; //!<文字区域固定屏幕类型

public:
	PMIData();
	virtual ~PMIData();
	void PrepareTextData();

	/**
	 * 得到外边框的显示时的校正Matrix
	 * @return
	 */
	Matrix4* GetOutFrameMatrix();

	virtual void RayPick(RayPickAction* action);
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 设置定义平面
	 * @param defPlane
	 */
	void SetDefPlane(const Matrix3x4& defPlane);
	/**
	 * 获取定义平面
	 * @return
	 */
	Matrix3x4& GetDefPlane();

	/**
	 * 获取定义平面OpenGL 显示矩阵
	 * @return
	 */
	Matrix4& GetGLDefPlane();

	/**
	 * 得到符合文本的矩阵，在现实过程中进行显示位置矫正
	 * @return
	 */
	BoundingBox GetComTextsBox();

private:
	void Init();

	Matrix3x4 GetWorldMatrix(const Matrix3x4& mvMatrix,
			RenderAction* renderAction, const Vector3& center, bool allowTran,
			bool allowRotate, bool allowScale);

public:
	int m_ProtoTypeID; //!< 所属零件ID
	int m_Type; //!<PMI类型

	vector<PolyLine*> m_Lines; //!<折线集合
	vector<ComText*> m_ComTexts; //!<复合文本

	bool m_IsParallelScreen; //!<是否平行于屏幕

	Vector3 m_outramLocation; //!<外框的位置。TODO 如果没有外框的位置，平齐屏幕的方式无法确定文字的旋转点。

	string m_showText; //!< PMI文本

	float fontSize; //!< 字体大小
private:
	bool m_DefPlaneDirty; //!<定义平面是否需要更新标识

	Matrix3x4 m_WorldMatrix; //!<定义平面

	Matrix3x4 m_DefPlane; //!<定义平面

	Matrix4 m_GLDefPlaneMatrix; //!<定义平面GL显示矩阵

	Matrix4 m_outFrameMatrix; //!<外框显示时的方向和位置矫正矩阵，为了实现朝向屏幕和固定屏幕效果
};

}
#endif /* PMIDATA_H_ */
