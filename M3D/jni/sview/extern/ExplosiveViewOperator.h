/**@file
 *@brief	爆炸图实现类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef EXPLOSIVEVIEWOPERA_H_
#define EXPLOSIVEVIEWOPERA_H_

#define NOEXPLOSIVE -1
#define AWAYFROMCENTER 0

#define AWAYFROMCENTER_X 1
#define AWAYFROMCENTER_Y 2
#define AWAYFROMCENTER_Z 3

#define AWAYFROMCENTER_X_LEFT 4
#define AWAYFROMCENTER_Y_FRONT 5
#define AWAYFROMCENTER_Z_BOTTOM 6

#define AWAYFROMCENTER_YZ_FACE 7
#define AWAYFROMCENTER_XZ_FACE 8
#define AWAYFROMCENTER_XY_FACE 9

#define TOBALLSURFACE 10


#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/model/Note.h"

namespace SVIEW
{
class View;
}

using namespace SVIEW;

namespace M3D
{
class Model;
class GroupNode;
class BoundingBox;
class ModelShape;
class M3D_API NodeState
{
public:
	NodeState()
	{
	}

	NodeState(ModelShape* node, const Matrix3x4& matrix, const BoundingBox& box)
	{
		this->sceneNode = node;
		this->matrixCache = matrix;
		this->boxCache = box;
	}

	ModelShape* sceneNode;
	Matrix3x4 matrixCache;
	BoundingBox boxCache;
};

/**
 * 爆炸图操作类，首先将所有节点的初始状态进行缓存，
 * 然后根据初始状态和最终状态进行插值，获取爆炸中间状态
 *
 * 当前支持爆炸到指定级别，支持按照中心点向四周爆炸操作
 */
class M3D_API ExplosiveViewOperator
{
public:
	static const int MAX_ASSEMBLY_LEVLE; //默认最大的爆炸装配级别

public:
	ExplosiveViewOperator();
	virtual ~ExplosiveViewOperator();

	void SetView(View* view);
	View* GetView();

	bool SetPercent(View* view, int style, float percent = 100.0f,
			bool useAnimation = true);

	bool SetPercent(View* view,vector<Model*> arrayModels, int style, float percent = 100.0f,
		bool useAnimation = true);
	bool setPercentWithDirection(View* view, vector<Model*> arrayModels, int style, float percent = 100.0f, Vector3 director =Vector3(0,0,0));

	//不带复位的
	bool SetPercentWithoutRestore(View* view, int style, float percent = 100.0f,
		bool useAnimation = true);
	bool Close(View* view);

	/**
	 * 将模型拆解，并排列
	 * @param shapeList 模型列表
	 * @param row		行数
	 * @param column	列数
	 * @anchor cdj
	 * @return 成功为true
	 */
	static bool FlatModel(View* view, vector<Model *>& shapeList, int row,
			int column);

	/**
	 * 获取当前爆炸图装配级别
	 * @return
	 */
	int GetExplosiveLevel();

	/**
	 * 设置爆炸到第几级装配
	 * @param level
	 */
	void SetExplosiveLevel(int level);
	/**
	 * 获取当前爆炸图装配级别
	 * @return
	 */
	float GetExplosivePercent();

	/**
	 * 设置爆炸到第几级装配
	 * @param level
	 */
	void SetExplosivePercent(int percent);
	/**
	 * 获取当前爆炸图装配级别
	 * @return
	 */
	int GetExplosiveStyle();

	/**
	 * 设置爆炸到第几级装配
	 * @param level
	 */
	void SetExplosiveStyle(int style);
	void SetExplosiveMinus(int minus);
	void Reset();
	bool AddNoteToScene(Vector3 start, Vector3 end);
private:
	

	/**
	 * 实现远离中心的爆炸效果
	 * @return
	 */
	bool ProcressAwayFromCenter();
	bool ProcressAwayFromCenter(vector < Model*> models);
	/**
	 * 实现所有零件到球面的爆炸效果
	 * @return
	 */
	bool ProcressAwayToBallSurface();

	static void AwayFromCenterCallback(void* data, Model* node);

	void AwayFromCenter(const Vector3& topCenter, GroupNode* subNode);

	void CacheMatrixState();
	void CacheSelectMatrixState(vector<Model*> arrayModels);

	static void CacheMatrixState(void* data, Model* node);

	static Vector3 GetNewPos(const BoundingBox& box, const Matrix3x4& matrix,
			int newPosStyle);

	/**
	 * 获取缓存的矩阵
	 * @param nodeAddr
	 * @param matrix
	 * @return
	 */
	bool GetMatrix(Model* nodeAddr, NodeState& nodeState);
	/**
	 * 将矩阵加入缓存中
	 * @param nodeAddr
	 * @param matrix
	 */
	void AddMatrix(Model* nodeAddr, const NodeState& nodeState);
	float m_explosiveAllValue;
private:
	View* m_view;
	int m_explosiveStyle; //当前作用的爆炸图类型
	int m_explosiveMinus;
	float m_explosivePercent; //爆炸力度
	
	bool m_isUseAnimation;

	bool m_isFirstOpen;
	bool m_isSelector;
	long modelID;
	 
	Vector3 explosiveDirection;//指定爆炸轴为选定方向

	vector<Model*> arrayModels;
	
	map<ModelShape*, NodeState> m_allNodeMatrixsCache;

	Matrix4 m_viewMatrix; //viewMatrix

	Vector3 m_TopNodeCenter; //顶层节点包围盒中心

	Vector3 m_TopNodeTop; //顶层节点包围盒最上边

	Vector3 m_TopNodeBottom; //顶层节点包围盒最下边

	Vector3 m_TopNodeLeft; //顶层节点包围盒最左边
	
	Vector3 m_TopNodeRight; //顶层节点包围盒最右边

	Vector3 m_TopNodeFront; //顶层节点包围盒最前边

	Vector3 m_TopNodeBehind; //顶层节点包围盒最后边
	//如果是自由指定的方向，则需要以法向量及包围盒合适的点确定的面为参考系
	Vector3 p000;
	Vector3 p001;
	Vector3 p010;
	Vector3 p011;
	Vector3 p100;
	Vector3 p101;
	Vector3 p110;
	Vector3 p111;

	float m_screenDepth; //屏幕点到空间点所需的深度

	int m_explosiveLevel; //爆炸到几级装配


};
}

#endif //EXPLOSIVEVIEWOPERA_H_
