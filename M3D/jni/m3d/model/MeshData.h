/**@file
 *@brief	顶点数据存储 类头文件
 *@author
 *@date
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_MESHDATA_H_
#define M3D_MESHDATA_H_
#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/base/Object.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"


namespace M3D
{

class SceneManager;
class GeometryAttribute;
class HardWareVertexBuffer;
class HardWareIndexBuffer;

/**
 * 网格数据的接口类
 */
class M3D_API IVertexSet:public Object
{
public:
	IVertexSet():Object()
	{

	}

	virtual ~IVertexSet()
	{

	}

	/**
	 * 情况MeshData中存储的顶点法线和索引数据
	 */
	virtual void Clear()= 0;

	/**
	 * 得到存储的顶点个数
	 * @return
	 */
	virtual int GetVertexCount()= 0;

	/**
	 * 得到关联的几何属性
	 * @return 关联的几何属性 为NULL表示没有关联
	 */
	virtual GeometryAttribute* GetGeoAttribute()= 0;

	/**
	 * 设置关联几何属性
	 * @param geoAttribute 待关联的几何属性
	 */
	virtual void SetGeoAttribute(GeometryAttribute* geoAttribute)= 0;

	/**
	 * TODO 优化显示数据，对显示数据精细缩放
	 */
	virtual void Optimize() =0;

	virtual BoundingBox& GetBoundingBox() = 0;
};


/**
 * 存储实际网格数据 类
 */
class M3D_API VertexSet: public IVertexSet
{
public:
	VertexSet(void);

	virtual ~VertexSet(void);
 
	virtual BoundingBox& GetBoundingBox();

	virtual void RayPick(RayPickAction* action);

	virtual void SetUseIndex(bool useIndex);

	virtual bool IsUseIndex();

	virtual void Clear();

	/**
	 * 清空GPU硬件中的数据，此操作必须在GL线程中进行
	 */
	void ClearGPUData();

	virtual int GetVertexCount();

	/**
	 * 得到顶点数组的地址
	 * @return 顶点数组的地址
	 */
	vector<Vector3>* GetPositionArray();

	/**
	 * 得到法向量地址
	 * @return  法向量的地址
	 */
	vector<Vector3>* GetNormalArray();

	/**
	 * 得到文理坐标
	 * @return 纹理坐标地址
	 */
	vector<Vector3>* GetTextureCoordArray();

	/**
	 * 得到索引的地址
	 * @return 索引的地址
	 */
	vector<M3D_INDEX_TYPE>* GetIndexArray();

	/**
	 * 得到线框方式绘制数据点地址
	 * @return 顶点地址
	 */
	vector<Vector3>* GetLinesModePointArray();

	/**
	 * 得到线框方式绘制索引地址
	 * @return 索引地址
	 */
	vector<M3D_INDEX_TYPE>* GetLinesModeIndexArray();

	virtual GeometryAttribute* GetGeoAttribute();

	virtual void SetGeoAttribute(GeometryAttribute* geoAttribute);

	virtual void Optimize();

	HardWareVertexBuffer* GetHardWareVertexBuffer();

	HardWareIndexBuffer* GetHardWareIndexBuffer();
	bool IsDirty();

	void MarkDirty();
	/**
	 * 创建硬件缓存
	 */
	void UpdateHardWareBuffer(SceneManager* sceneMgr);

	float GetVolumeAndArea(float& volume,float& area);
private:
	/**
	 * 内存不足时，回调函数
	 */
	static void no_memory();
private:

	vector<Vector3> m_positionArray;//!< 存储顶点位置

	vector<Vector3> m_NormalArray;//!<存储顶点法向量

	vector<Vector3> m_textureCoords;//!<存储纹理坐标

	vector<M3D_INDEX_TYPE> m_IndexArray;//!<促存储索引

	bool m_UseIndex;//!<是否使用索引 测试索引方式绘制

	GeometryAttribute* m_geoAttribute;//!<关联几何属性

	vector<Vector3> m_linesModepositionArray;//!<临时变量用于存储以线框模式绘制的点
	vector<M3D_INDEX_TYPE> m_linesModeIndexArray;//!<临时变量用于存储以线框模式绘制的索引

	HardWareVertexBuffer* m_hardWareVertexBuffer;//!< 顶点数据硬件缓存管理类

	HardWareIndexBuffer* m_hardWareIndexBuffer;//!< 顶点索引硬件缓存管理类

	bool m_dirty;//!<脏标记，当顶点数据发生改变时，设置此变量为true

	BoundingBox m_boudingBox;
};

/**
 * 引用MeshData中存储的数据，
 * 仅仅存储数据的开始位置、偏移量和长度
 */
class M3D_API Mesh :public IVertexSet
{
public:
	Mesh(VertexSet* refMesh);
	virtual ~Mesh();

	virtual BoundingBox& GetBoundingBox();

	virtual void RayPick(RayPickAction* action);

	/**
	 * IMeshData virtual fun
	 */
	virtual void Clear();

	virtual int GetVertexCount();

	virtual GeometryAttribute* GetGeoAttribute();
	virtual void SetGeoAttribute(GeometryAttribute* geoAttribute);

	virtual void Optimize();
	/**
	 * 设置引用的MeshData
	 * @param refMesh
	 */
	void SetRefMesh(VertexSet* refMesh);

	/**
	 * 得到引用的MeshData
	 * @return
	 */
	VertexSet* GetRefMesh();

	/**
	 * 设置有效数据长度
	 * @param length
	 */
	void SetDataLength(unsigned int  length);

	/**
	 * 得到有效数据长度
	 * @return
	 */
	unsigned int  GetDataLength();

	/**
	 * 设置有效数据的偏移量
	 * @param offset 有效数据偏移量
	 */
	void SetDataOffset(unsigned int  offset);

	/**
	 * 得到有效数据的偏移量
	 * @return 偏移量
	 */
	unsigned int  GetDataOffset();

	float GetVolumeAndArea(float& volume,float& area);

private:
	GeometryAttribute* m_geoAttribute;//!<关联几何属性

	VertexSet* m_refMesh;//!<引用用来存储实际显示数据的类

	unsigned int m_dataOffset; //!< 数据相对于refMeshData的偏移量
	unsigned int m_dataLength; //!< 数据总长度

	BoundingBox m_boudingBox;
};

}
#endif /*MESHDATA_H_*/

