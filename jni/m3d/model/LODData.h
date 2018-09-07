/**@file
 *@brief	LODData类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_LODDATA_H_
#define M3D_LODDATA_H_

#include "m3d/model/Shape.h"

namespace M3D
{
class IVertexSet;

/**
 *LOD 数据管理类
 *提供LOD数据的增加删除，获取指定等级的LOD接口
 */
class M3D_API LOD
{
public:
	static  int LODMAX;//!< 精细度最高等级
	static  int LODMIN;//!< 精细度最小等级
public:
	LOD();
	virtual ~LOD();

	LOD(const LOD& orig);
	LOD& operator =(const LOD& orig);
	/**
	 * 得到level等级的lod数据
	 * @param level
	 * @return
	 */
	virtual IVertexSet* GetData(int level = 0);

	/**
	 * 添加level等级的lod数据
	 * @param
	 * @param level
	 */
	virtual void AddData(IVertexSet*, int level = 0);

	/**
	 * 得到lod数据的总级数
	 * @return
	 */
	virtual int GetCount();

	/**
	 * 得到当前使用的lod等级
	 * @param renderAction
	 * @return
	 */
	virtual int GetUseLevel(RenderAction* renderAction);

protected:
	vector<IVertexSet*> m_data;//!<存储lod数据
};

}

#endif /*LODDATA_H_*/

