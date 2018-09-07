#include "m3d/M3D.h"
#include "m3d/model/Curve.h"
#include "m3d/model/GeoAttribute.h"

#include "m3d/action/RayPickAction.h"
namespace M3D
{
/****************************************************************************
 *	@brief		Curve构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Curve::Curve(void):Shape()
{
	this->m_geoAttribute = NULL;
	this->SetType(CURVE_UNKNOWN);
}

/****************************************************************************
 *	@brief		Curve析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Curve::~Curve(void)
{
	if (this->m_geoAttribute != NULL)
	{
		delete this->m_geoAttribute;
		this->m_geoAttribute = NULL;
	}
}

GeometryAttribute* Curve::GetGeoAttribute()
{
	return this->m_geoAttribute;
}
void Curve::SetGeoAttribute(GeometryAttribute* geoAttribute)
{
	this->m_geoAttribute = geoAttribute;
}

/****************************************************************************
 *	@brief		XPolyLine构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

SPolyLine::SPolyLine(void):IPolyLine()
{
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
}

/****************************************************************************
 *	@brief		XPolyLine析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

SPolyLine::~SPolyLine(void)
{

}

/****************************************************************************
 *	@brief		获取折线点列信息
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntPoints			折线点列
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Vector3>& SPolyLine::GetPoints()
{
	return m_pntPoints;
}


void SPolyLine::RayPick(RayPickAction* action)
{
	Vector3 intersectPos;
	vector<Vector3>& pnts = this->GetPoints();
	for (int i = 0; i < pnts.size() - 1; i++)
	{
		if (action->Intersect(pnts[i], pnts[i + 1], intersectPos))
		{
			action->AddIntersectPnt(intersectPos);
		}
	}
}



/****************************************************************************
 *	@brief		设置折线点列信息
 *	@param[in]	i_pntPoints			折线点列
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS SPolyLine::SetPoints(const vector<Vector3>& i_pntPoints)
{
	m_pntPoints = i_pntPoints;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取折线起始终止范围
 *	@param[out]	o_fUMin				起始范围
 *	@param[out]	o_fUMax				终止范围
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void SPolyLine::GetDomain(float& o_fUMin, float& o_fUMax)
{
	//o_fUMin = m_fUMin;
	//o_fUMax = m_fUMax;
}

/****************************************************************************
 *	@brief		设置折线起始终止范围
 *	@param[in]	i_fUMin				起始范围
 *	@param[in]	i_fUMax				终止范围
 *	@retval		SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS SPolyLine::SetDomain(float i_fUMin, float i_fUMax)
{
	//m_fUMin = i_fUMin;
	//m_fUMax = i_fUMax;
	return M_SUCCESS;
}


void SPolyLine::AddPointsIndex(M3D_INDEX_TYPE pointIndex)
{
	this->m_PntIndex.push_back(pointIndex);
}

vector<M3D_INDEX_TYPE>& SPolyLine::GetPntIndex()
{
	return m_PntIndex;
}

void SPolyLine::SetPntIndex(vector<M3D_INDEX_TYPE>& val)
{
	m_PntIndex = val;
}

RefPolyLine::RefPolyLine(SPolyLine* refLine):IPolyLine()
{
	this->m_refLine = refLine;
	this->m_refLine->AddRef();

	m_dataOffset =0 ; ///数据相对于refMeshData的偏移量
	m_dataLength =0; ///数据总长度
}

RefPolyLine::~RefPolyLine(void)
{
	if(m_refLine)
	this->m_refLine->Release();

	this->m_refLine = NULL;
}

SPolyLine* RefPolyLine::GetRefLine()
{
	return this->m_refLine;
}

void RefPolyLine::RayPick(RayPickAction* action)
{
	Vector3* pnts = this->GetRefLine()->GetPoints().data() + this->m_dataOffset;

	Vector3 intersectPos;
	//由于点在读入的过程中做了特殊处理，这里求真正点的个数
	int realPntNum = this->m_dataLength/2 + 1;
	for (int i = 0; i < realPntNum-1; i++)
	{
		//2n -- 2n-1 构成一条实际的线
		if (action->Intersect(pnts[i*2], pnts[i*2 + 1], intersectPos))
		{
			action->AddIntersectPnt(intersectPos);
		}
	}
}

void RefPolyLine::ComputeBox()
{
	this->m_BoundingBox.Clear();
	Vector3* pnts = this->GetRefLine()->GetPoints().data() + this->m_dataOffset;

	Vector3 intersectPos;
	//由于点在读入的过程中做了特殊处理，这里求真正点的个数
	int realPntNum = this->m_dataLength/2 + 1;
	for (int i = 0; i < realPntNum-1; i++)
	{
		//2n -- 2n-1 构成一条实际的线
		this->m_BoundingBox.Merge(pnts[i*2]);
		this->m_BoundingBox.Merge(pnts[i*2 + 1]);
	}
}
void RefPolyLine::SetDataLength(unsigned int  length)
{
	this->m_dataLength = length;
}

unsigned int  RefPolyLine::GetDataLength()
{
	return this->m_dataLength;
}

void RefPolyLine::SetDataOffset(unsigned int  offset)
{
	this->m_dataOffset = offset;
}

unsigned int  RefPolyLine::GetDataOffset()
{
	return this->m_dataOffset;
}

Vector3 RefPolyLine::GetStartPnt()
{
	Vector3 pnt;
	if (m_refLine)
	{
		vector<Vector3>& pnts = m_refLine->GetPoints();
		if (pnts.size() > m_dataOffset)
		{
			pnt = pnts.at(m_dataOffset);
		}
	}
	return pnt;
}

Vector3 RefPolyLine::GetEndPnt()
{
	Vector3 pnt;
	if (m_refLine)
	{
		vector<Vector3>& pnts = m_refLine->GetPoints();
		if (pnts.size() > m_dataOffset + m_dataLength -1)
		{
			pnt = pnts.at(m_dataOffset + m_dataLength-1);
		}
	}
	return pnt;
}

/****************************************************************************
 *	@brief		Ellipse构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Ellipse::Ellipse(void)
{
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
}

/****************************************************************************
 *	@brief		Ellipse析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Ellipse::~Ellipse(void)
{

}

/****************************************************************************
 *	@brief		获取弧线中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter			中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Ellipse::GetCenterPoint()
{
	return m_pntCenter;
}

/****************************************************************************
 *	@brief		设置弧线中心点
 *	@param[in]	i_pntCenter			中心点
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetCenterPoint(const Vector3& i_pntCenter)
{
	m_pntCenter = i_pntCenter;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取弧线向量
 *	@param[out]	o_dirX				X方向向量
 *	@param[out]	o_dirY				Y方向向量
 *	@param[out]	o_dirZ				Z方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;
}

/****************************************************************************
 *	@brief		设置弧线向量
 *	@param[in]	i_dirX				X方向向量
 *	@param[in]	i_dirY				Y方向向量
 *	@param[in]	i_dirZ				Z方向向量
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetXYZDir(const Vector3& i_dirX, const Vector3& i_dirY,
		const Vector3& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取弧线半径
 *	@param[out]	o_fMajorR			长半径
 *	@param[out]	o_fMinorR			短半径
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::GetRadius(float& o_fMajorR, float& o_fMinorR)
{
	o_fMajorR = m_fMajorRadius;
	o_fMinorR = m_fMinorRadius;
}

/****************************************************************************
 *	@brief		设置弧线半径
 *	@param[in]	o_fMajorR			长半径
 *	@param[in]	o_fMinorR			短半径
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetRadius(float i_fMajorR, float i_fMinorR)
{
	m_fMajorRadius = i_fMajorR;
	m_fMinorRadius = i_fMinorR;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		设置圆弧方向
 *	@param[in]	i_dirNormal	圆弧方向
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Ellipse::SetNormal(const Vector3& i_dirNormal)
{
	m_dirNormal = i_dirNormal;
}

/****************************************************************************
 *	@brief		获取圆弧方向
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirNormal	圆弧方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Ellipse::GetNormal()
{
	return m_dirNormal;
}

/****************************************************************************
 *	@brief		设置原点方向
 *	@param[in]	i_dirOrigin	原点方向
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::SetOriginDir(const Vector3& i_dirOrigin)
{
	m_dirOrigin = i_dirOrigin;
}

/****************************************************************************
 *	@brief		获取原点方向
 *	@param[in]	无	
 *	@param[out]	无
 *	@retval		m_dirOrigin	原点方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Ellipse::GetOriginDir()
{
	return m_dirOrigin;
}

/****************************************************************************
 *	@brief		获取弧线起始终止范围
 *	@param[out]	o_fUMin				起始范围
 *	@param[out]	o_fUMax				终止范围
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::GetDomain(float& o_fUMin, float& o_fUMax)
{
	o_fUMin = m_fUMin;
	o_fUMax = m_fUMax;
}

/****************************************************************************
 *	@brief		设置弧线起始终止范围
 *	@param[in]	i_fUMin				起始范围
 *	@param[in]	i_fUMax				终止范围
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetDomain(float i_fUMin, float i_fUMax)
{
	m_fUMin = i_fUMin;
	m_fUMax = i_fUMax;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取弧线点坐标
 *	@param[out]	o_pntStart			起点坐标
 *	@param[out]	o_pntEnd			终点坐标
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::GetCoordinatePnt(Vector3& o_pntStart, Vector3& o_pntEnd)
{
	o_pntStart = m_pntStart;
	o_pntEnd = m_pntEnd;
}

/****************************************************************************
 *	@brief		设置弧线点坐标
 *	@param[in]	i_pntStart			起点坐标
 *	@param[in]	i_pntEnd			终点坐标
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetCoordiantePnt(const Vector3& i_pntStart,
		const Vector3& i_pntEnd)
{
	m_pntStart = i_pntStart;
	m_pntEnd = i_pntEnd;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取弧线点参数位置
 *	@param[out]	o_fStartPar			起点参数位置
 *	@param[out]	o_fEndPar			终点参数位置
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Ellipse::GetParameter(float& o_fStartPar, float& o_fEndPar)
{
	o_fStartPar = m_fStartPar;
	o_fEndPar = m_fEndPar;
}

/****************************************************************************
 *	@brief		设置弧线点参数位置
 *	@param[in]	o_fStartPar			起点参数位置
 *	@param[in]	o_fEndPar			终点参数位置
 *	@retval		M_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS Ellipse::SetParameter(float i_fStartPar, float i_fEndPar)
{
	m_fStartPar = i_fStartPar;
	m_fEndPar = i_fEndPar;
	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		NurbsCurve构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
NurbsCurve::NurbsCurve(void)
{
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_nuDegree = 0;
}

/*************************************************************************//**
 *	@brief		NurbsCurve析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
NurbsCurve::~NurbsCurve(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		M_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::GetDomain(float& o_fMin, float& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::SetDomain(float i_fMin, float i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Degree
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuDegree				Degree值
 *	@note		无
 *	@attention	无
 ****************************************************************************/
unsigned int NurbsCurve::GetDegree()
{
	return m_nuDegree;
}

/*************************************************************************//**
 *	@brief		设置Degree
 *	@param[in]	i_nuDegree				Degree参数
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::SetDegree(unsigned int i_nuDegree)
{
	m_nuDegree = i_nuDegree;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Knot的多样性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecKnotPlicity		Knot的多样性
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<unsigned int> NurbsCurve::GetKnotMultiplicity()
{
	return m_vcKnotPlicity;
}

/*************************************************************************//**
 *	@brief		添加Knot的多样性
 *	@param[in]	i_nuKnotPlicity			Knot的多样性
 *	@param[out]	无
 *	@retval		M_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::AddKnotMultiplicity(unsigned int i_nuKnotPlicity)
{
	m_vcKnotPlicity.push_back(i_nuKnotPlicity);

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Knot节点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecKnots				Knots节点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<float> NurbsCurve::GetKnots()
{
	return m_vcKnots;
}

/*************************************************************************//**
 *	@brief		添加Knot节点
 *	@param[in]	i_fKnot					Knot节点
 *	@param[out]	无
 *	@retval		M_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::AddKnot(float i_fKnot)
{
	m_vcKnots.push_back(i_fKnot);

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		添加Nurbs曲线控制点
 *	@param[in]	i_pntCtrl				Nurbs曲线控制点
 *	@param[out]	无
 *	@retval		M_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::AddPoint(Vector3 i_pntCtrl)
{
	m_vcPoints.push_back(i_pntCtrl);

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Nurbs曲线控制点的权重
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecWeights				Nurbs曲线控制点的权重
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<float> NurbsCurve::GetWeights()
{
	return m_vcWeights;
}

/*************************************************************************//**
 *	@brief		添加Nurbs曲线控制点的权重
 *	@param[in]	i_fWeight					Nurbs曲线控制点的权重
 *	@param[out]	无
 *	@retval		M_SUCCESS					添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS NurbsCurve::AddWeight(float i_fWeight)
{
	m_vcWeights.push_back(i_fWeight);

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Hyperbola构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Hyperbola::Hyperbola(void)
{
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_fSemiAxis = 0.0f;
	m_fSemiImgAxis = 0.0f;
}

/*************************************************************************//**
 *	@brief		Hyperbola析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Hyperbola::~Hyperbola(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		M_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::GetDomain(float& o_fMin, float& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetDomain(float i_fMin, float i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter				双曲线中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Hyperbola::GetCenterPoint()
{
	return m_pntCenter;
}

/*************************************************************************//**
 *	@brief		设置双曲线的中心点
 *	@param[in]	i_pntCenter				双曲线的中心点
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetCenterPoint(Vector3& i_pntCenter)
{
	m_pntCenter = i_pntCenter;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的XYZ方向向量
 *	@param[in]	无
 *	@param[out]	o_dirX					X方向向量
 *	@param[out]	o_dirY					Y方向向量
 *	@param[out]	o_dirZ					Z方向向量
 *	@retval		M_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置双曲线的XYZ方向向量
 *	@param[in]	i_dirX					X方向向量
 *	@param[in]	i_dirY					Y方向向量
 *	@param[in]	i_dirZ					Z方向向量
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetXYZDir(Vector3& i_dirX, Vector3& i_dirY, Vector3& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的半轴
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fSemiAxis				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
float Hyperbola::GetSemiAxis()
{
	return m_fSemiAxis;
}

/*************************************************************************//**
 *	@brief		设置双曲线的半轴
 *	@param[in]	i_fSemiAxis				双曲线的半轴
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetSemiAxis(float i_fSemiAxis)
{
	m_fSemiAxis = i_fSemiAxis;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的SemiImgAxis
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fSemiAxis				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
float Hyperbola::GetSemiImgAxis()
{
	return m_fSemiImgAxis;
}

/*************************************************************************//**
 *	@brief		设置双曲线的SemiImgAxis
 *	@param[in]	i_fSemiImgAxis			双曲线的SemiImgAxis
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetSemiImgAxis(float i_fSemiImgAxis)
{
	m_fSemiImgAxis = i_fSemiImgAxis;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取起点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStart				起点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Hyperbola::GetStartPoint()
{
	return m_pntStart;
}

/*************************************************************************//**
 *	@brief		设置起点坐标
 *	@param[in]	i_pntStart				起点坐标
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetStartPoint(Vector3 i_pntStart)
{
	m_pntStart = i_pntStart;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取终点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEnd				终点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Hyperbola::GetEndPoint()
{
	return m_pntEnd;
}

/*************************************************************************//**
 *	@brief		设置终点坐标
 *	@param[in]	i_pntStart				终点坐标
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Hyperbola::SetEndPoint(Vector3 i_pntEnd)
{
	m_pntEnd = i_pntEnd;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Parabola构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Parabola::Parabola(void)
{
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_fFocalDis = 0.0f;
}

/*************************************************************************//**
 *	@brief		Parabola析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Parabola::~Parabola(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		M_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::GetDomain(float& o_fMin, float& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetDomain(float i_fMin, float i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取起点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStart				起点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Parabola::GetStartPoint()
{
	return m_pntStart;
}

/*************************************************************************//**
 *	@brief		设置起点坐标
 *	@param[in]	i_pntStart				起点坐标
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetStartPoint(Vector3 i_pntStart)
{
	m_pntStart = i_pntStart;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取终点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEnd				终点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Parabola::GetEndPoint()
{
	return m_pntEnd;
}

/*************************************************************************//**
 *	@brief		设置终点坐标
 *	@param[in]	i_pntStart				终点坐标
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetEndPoint(Vector3 i_pntEnd)
{
	m_pntEnd = i_pntEnd;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter				中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 Parabola::GetCenterPoint()
{
	return m_pntCenter;
}

/*************************************************************************//**
 *	@brief		设置中心点
 *	@param[in]	i_pntCenter				中心点
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetCenterPoint(Vector3& i_pntCenter)
{
	m_pntCenter = i_pntCenter;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取抛物线的XYZ方向向量
 *	@param[in]	无
 *	@param[out]	o_dirX					X方向向量
 *	@param[out]	o_dirY					Y方向向量
 *	@param[out]	o_dirZ					Z方向向量
 *	@retval		M_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置抛物线的XYZ方向向量
 *	@param[in]	i_dirX					X方向向量
 *	@param[in]	i_dirY					Y方向向量
 *	@param[in]	i_dirZ					Z方向向量
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetXYZDir(Vector3& i_dirX, Vector3& i_dirY, Vector3& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;

	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Focal距离
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fFocalDis				Focal距离
 *	@note		无
 *	@attention	无
 ****************************************************************************/
float Parabola::GetFocalDistance()
{
	return m_fFocalDis;
}

/*************************************************************************//**
 *	@brief		设置Focal距离
 *	@param[in]	i_fDistance				Focal距离
 *	@param[out]	无
 *	@retval		M_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS Parabola::SetFocalDistance(float i_fDistance)
{
	m_fFocalDis = i_fDistance;

	return M_SUCCESS;
}

}
