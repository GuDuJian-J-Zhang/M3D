/**@file
 *@brief
 *@author	XL
 *@date		2016-6-1
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_CROSSSECTION_H_
#define M3D_CROSSSECTION_H_

#include "m3d/M3D.h"
#include "m3d/graphics/SectionLine.h"


namespace M3D
{
class SectionLine;
/**
 * @class
 * @brief 盖面类
 */
class M3D_API CrossSection
{
public:
	CrossSection();
	virtual ~CrossSection();

	SectionLine* m_outer;//!<外层剖切线

	vector<SectionLine*> m_inner;//!<内部剖切线，围成孔

	vector<Vector3> m_Points;//!<围成多边形的点

	vector<unsigned short> m_triangleIndex;//!<三角形索引

	vector<unsigned short> m_segmentIndex;//!<线段索引

	vector<Vector3> m_holes;//!<洞内点

	vector<Vector3> m_trianglesVertex;//!<三角形坐标

};

} /* namespace M3D */
#endif /* M3D_CROSSSECTION_H_ */
