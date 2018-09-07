// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_CurveEntity.h
 *
 *	@brief	Stk_CurveEntity类、Stk_PolyLine类、Stk_Ellipse类和Stk_Line类的声明
 *
 *	@par	履历:
 *		2013/08/14	创建。WangY@HOTEAMSOFT
 *
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 ****************************************************************************/
#ifndef M3D_CURE_H_
#define M3D_CURE_H_

#include "m3d/M3D.h"
#include "m3d/M3DStates.h"
#include "m3d/model/Shape.h"
namespace M3D
{

class GeometryAttribute;
/**************************************************************************************
 *	@class	Stk_Text
 *	@brief	线条基类
 *
 *	PMI各线条类型的基础类。<br>
 *	继承自Stk_Entity类。
 ***************************************************************************************/
class M3D_API Curve: public Shape
{
public:
	Curve(void);
	virtual ~Curve(void);

	/**
	 *
	 * @return
	 */
	virtual GeometryAttribute* GetGeoAttribute();
	/**
	 *
	 * @param geoAttribute
	 */
	virtual void SetGeoAttribute(GeometryAttribute* geoAttribute);
private:
	GeometryAttribute* m_geoAttribute;
};

class M3D_API IPolyLine:public Curve
{
public:
	IPolyLine():Curve()
	{

	}
	virtual ~IPolyLine()
	{

	}
};

class M3D_API SPolyLine: public IPolyLine
{
private:
	float m_fUMin; //!< 起始范围
	float m_fUMax; //!< 终止范围
	vector<Vector3> m_pntPoints; //!< 点列
	vector<M3D_INDEX_TYPE> m_PntIndex; //!<纪录索引点，共保存的时候使用
public:
	SPolyLine(void);
	virtual ~SPolyLine(void);
	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_POLYLINE;
	}

	virtual void RayPick(RayPickAction* action);

	virtual vector<Vector3>& GetPoints();

	void AddPoints(const Vector3& point)
	{
		this->m_pntPoints.push_back(point);
	}

	M3D_STATUS SetPoints(const vector<Vector3>& i_pntPoints);
	void GetDomain(float& o_fUMin, float& o_fUMax);
	M3D_STATUS SetDomain(float i_fUMin, float i_fUMax);

	void AddPointsIndex(M3D_INDEX_TYPE pointIndex);
	vector<M3D_INDEX_TYPE>& GetPntIndex();
	void SetPntIndex(vector<M3D_INDEX_TYPE>& val);
};

class M3D_API RefPolyLine: public IPolyLine
{
public:
	RefPolyLine(SPolyLine* refLine);
	virtual ~RefPolyLine(void);

	SPolyLine* GetRefLine();

	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_REF_POLYLINE;
	}
	virtual void RayPick(RayPickAction* action);

	virtual void ComputeBox();

	void SetDataLength(unsigned int  length);
	unsigned int  GetDataLength();

	void SetDataOffset(unsigned int  offset);

	unsigned int  GetDataOffset();

	/**
	 * 得到折线开始点
	 * @return
	 */
	Vector3 GetStartPnt();

	/**
	 * 得到折线结束点
	 * @return
	 */
	Vector3 GetEndPnt();

private:
	SPolyLine* m_refLine;

	unsigned int m_dataOffset; //!< 数据相对于refMeshData的偏移量
	unsigned int m_dataLength; //!< 数据总长度
};


/**************************************************************************************
 *	@class	Stk_Text
 *	@brief	弧线类
 *
 *	记录PMI中类型为弧线的数据信息。<br>
 *	继承自Stk_CurveEntity类。
 ***************************************************************************************/

class M3D_API Ellipse: public Curve
{
private:
	float m_fUMin; //!< 起始范围
	float m_fUMax; //!< 终止范围
	Vector3 m_pntCenter; //!< 中心点
	Vector3 m_dirX; //!< X方向向量
	Vector3 m_dirY; //!< Y方向向量
	Vector3 m_dirZ; //!< Z方向向量
	float m_fMajorRadius; //!< 长半径
	float m_fMinorRadius; //!< 短半径
	Vector3 m_pntStart; //!< 起点坐标
	Vector3 m_pntEnd; //!< 终点坐标
	float m_fStartPar; //!< 起点参数位置
	float m_fEndPar; //!< 终点参数位置
	Vector3 m_dirNormal; //!< 圆弧方向
	Vector3 m_dirOrigin; //!< 原点方向

public:
	Ellipse(void);
	~Ellipse(void);
	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_ELLIPSE;
	}

	Vector3 GetCenterPoint();
	M3D_STATUS SetCenterPoint(const Vector3& i_pntCenter);

	void GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ);
	M3D_STATUS SetXYZDir(const Vector3& i_dirX, const Vector3& i_dirY,
			const Vector3& i_dirZ);

	void GetRadius(float& o_fMajorR, float& o_fMinorR);
	M3D_STATUS SetRadius(float i_fMajorR, float i_fMinorR);

	void SetNormal(const Vector3& i_dirNormal);
	Vector3 GetNormal();

	void SetOriginDir(const Vector3& i_dirOrigin);
	Vector3 GetOriginDir();

	void GetDomain(float& o_fUMin, float& o_fUMax);
	M3D_STATUS SetDomain(float i_fUMin, float i_fUMax);

	void GetCoordinatePnt(Vector3& o_pntStart, Vector3& o_pntEnd);
	M3D_STATUS SetCoordiantePnt(const Vector3& i_pntStart,
			const Vector3& i_pntEnd);

	void GetParameter(float& o_fStartPar, float& o_fEndPar);
	M3D_STATUS SetParameter(float i_fStartPar, float i_fEndPar);

};

/************************************************************************************//**
 *	@class	Stk_NurbsCurve
 *	@brief	NURBS曲线类
 *
 *	记录NURBS曲线的数据信息。<br>
 *	继承自Stk_CurveEntity类。
 ***************************************************************************************/
class M3D_API NurbsCurve: public Curve
{
private:
	float m_fUMin; //!< 起始范围
	float m_fUMax; //!< 终止范围
	unsigned int m_nuDegree; //!< degree值
	vector<unsigned int> m_vcKnotPlicity; //!< 多样性
	vector<float> m_vcKnots; //!< Knots节点
	vector<Vector3> m_vcPoints; //!< 控制点
	vector<float> m_vcWeights; //!< 权重
public:
	NurbsCurve(void);
	~NurbsCurve(void);

	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_NURBSCURVE;
	}

	M3D_STATUS GetDomain(float& o_fMin, float& o_fMax);
	M3D_STATUS SetDomain(float i_fMin, float i_fMax);

	unsigned int GetDegree();
	M3D_STATUS SetDegree(unsigned int i_nuDegree);

	vector<unsigned int> GetKnotMultiplicity();
	M3D_STATUS AddKnotMultiplicity(unsigned int i_nuKnotPlicity);

	vector<float> GetKnots();
	M3D_STATUS AddKnot(float i_fKnot);

	M3D_STATUS AddPoint(Vector3 i_pntCtrl);

	vector<float> GetWeights();
	M3D_STATUS AddWeight(float i_fWeight);
};

/************************************************************************************//**
 *	@class	Stk_Hyperbola
 *	@brief	双曲线类
 *
 *	记录双曲线的数据信息。<br>
 *	继承自Stk_CurveEntity类。
 ***************************************************************************************/
class M3D_API Hyperbola: public Curve
{
private:
	float m_fUMin; //!< 起始范围
	float m_fUMax; //!< 终止范围
	Vector3 m_pntCenter; //!< 中心点
	Vector3 m_dirX; //!< X方向向量
	Vector3 m_dirY; //!< Y方向向量
	Vector3 m_dirZ; //!< Z方向向量
	float m_fSemiAxis; //!< 半轴
	float m_fSemiImgAxis;
	Vector3 m_pntStart; //!< 起点坐标
	Vector3 m_pntEnd; //!< 终点坐标
public:
	Hyperbola(void);
	~Hyperbola(void);
	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_HYPERBOLA;
	}

	M3D_STATUS GetDomain(float& o_fMin, float& o_fMax);
	M3D_STATUS SetDomain(float i_fMin, float i_fMax);

	Vector3 GetCenterPoint();
	M3D_STATUS SetCenterPoint(Vector3& i_pntCenter);

	M3D_STATUS GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ);
	M3D_STATUS SetXYZDir(Vector3& i_dirX, Vector3& i_dirY, Vector3& i_dirZ);

	float GetSemiAxis();
	M3D_STATUS SetSemiAxis(float i_fSemiAxis);

	float GetSemiImgAxis();
	M3D_STATUS SetSemiImgAxis(float i_fSemiImgAxis);

	Vector3 GetStartPoint();
	M3D_STATUS SetStartPoint(Vector3 i_pntStart);

	Vector3 GetEndPoint();
	M3D_STATUS SetEndPoint(Vector3 i_pntEnd);
};

/************************************************************************************//**
 *	@class	Stk_Parabola
 *	@brief	抛物线类
 *
 *	记录抛物线的数据信息。<br>
 *	继承自Stk_CurveEntity类。
 ***************************************************************************************/
class M3D_API Parabola: public Curve
{
private:
	float m_fUMin; //!< 起始范围
	float m_fUMax; //!< 终止范围
	Vector3 m_pntStart; //!< 起点坐标
	Vector3 m_pntEnd; //!< 终点坐标
	Vector3 m_pntCenter; //!< 中心点
	Vector3 m_dirX; //!< X方向向量
	Vector3 m_dirY; //!< Y方向向量
	Vector3 m_dirZ; //!< Z方向向量
	float m_fFocalDis; //!< 焦点距离
public:
	Parabola(void);
	~Parabola(void);

	virtual SHAPETYPE GetType(void)
	{
		return  CURVE_PARABOLA;
	}

	M3D_STATUS GetDomain(float& o_fMin, float& o_fMax);
	M3D_STATUS SetDomain(float i_fMin, float i_fMax);

	Vector3 GetStartPoint();
	M3D_STATUS SetStartPoint(Vector3 i_pntStart);

	Vector3 GetEndPoint();
	M3D_STATUS SetEndPoint(Vector3 i_pntEnd);

	Vector3 GetCenterPoint();
	M3D_STATUS SetCenterPoint(Vector3& i_pntCenter);

	M3D_STATUS GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ);
	M3D_STATUS SetXYZDir(Vector3& i_dirX, Vector3& i_dirY, Vector3& i_dirZ);

	float GetFocalDistance();
	M3D_STATUS SetFocalDistance(float i_fDistance);

};

}
#endif //
