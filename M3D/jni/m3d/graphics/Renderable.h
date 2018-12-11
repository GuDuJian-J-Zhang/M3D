/**@file
 *@brief
 *@author	XL
 *@date		
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef  M3D_RENDERABLE_H_
#define  M3D_RENDERABLE_H_
#include "m3d/M3D.h"
#include <vector>
namespace M3D
{
class Matrix4;
class Vector3;
class Edge;
class Mesh;
class Color;
class BaseMaterial;
class HardWareVertexBuffer;
class HardWareIndexBuffer;

/**
 * @class 可绘制对象
 * @brief Renderable
 */

/**
 * @class 可绘制对象
 * @brief Renderable
 */
class M3D_API Renderable
{
public:
	/**
	 *构造函数
	 */
	Renderable();
	/**
	 *析构函数
	 */
	virtual ~Renderable();

	/**
	 *设置绘制时所需的世界矩阵
	 * @param worldMatrix
	 */
	virtual void SetRenderWorldMatrix(Matrix4* worldMatrix);
	/**
	 * 获取绘制时所需的世界矩阵
	 * @return
	 */
	virtual Matrix4* GetRenderWorldMatrix();

	/**
	 * 获取绘制时的颜色
	 * @return
	 */
	virtual const Color& GetRenderColor();

	/**
	 * 获取材质
	 * @return
	 */
	virtual BaseMaterial* GetRenderMaterial();

	/**
	 * 设置绘制时所需的面数据
	 * @param faceRenderData
	 */
	void SetRenderMesh(Mesh* mesh);

	/**
	 * 设置绘制时所需的边界线数据
	 * @param edgeRenderData
	 */
    void SetRenderPolyLine(Edge* edgeRenderData);

    /**
	 * 合并显示面到已有的显示数据中
	 * @param faceRenderData
	 */
	void MergeRenderMesh(Mesh* mesh);

	/**
	 * 合并显示边界线到已有的边界线数据中
	 * @param edgeRenderData
	 */
	void MergeRenderPolyLine(Edge* edgeRenderData);

	/**
	 * 得到顶点位置的开始地址
	 * @return
	 */
	Vector3* GetVertexPos();

	/**
	 * 得到顶点法向量开始地址
	 * @return
	 */
	Vector3* GetVertexNormal();

	/**
	 * 得到纹理坐标地址
	 * @return
	 */
	Vector3* GetTextureCoords();

	/**
	 * 得到索引的起始地址
	 */
	M3D_INDEX_TYPE* GetIndex();

	/**
	 * 得到线框模式渲染顶点位置的开始地址
	 * @return
	 */
	Vector3* GetLinesModeVertexPos();

	/**
	 * 得到线框模式渲染索引的起始地址
	 */
	M3D_INDEX_TYPE* GetLineModeIndex();

	/**
	 * 得到有效数据的总长度，
	 * 使用索引时，此值表示索引长度。非索引时，此值表示顶点个数
	 * @return
	 */
	int GetDataLength();

	/**
	 * 当前显示数据，是否需要按索引解析
	 * @return true 按索引解析 false 按数组解析
	 */
	bool IsUseIndex();

	/**
	 * 是否最前端显示
	 * @return
	 */
	bool IsFrontShow();

	/**
	 * 设置最前端显示
	 * @param isFrontShow
	 */
	virtual void SetFrontShow(bool frontShow);

	/**
	* 是否使用背面剔除
	*  @return
	*/
	bool IsCullBackFace() { return m_bCullBackFace; };
	/**
	* 设置使用双面光照
	* @param bFlg
	*/
	virtual void SetCullBackFace(bool bFlg) { m_bCullBackFace = bFlg; };
	/**
	 *
	 * @param size
	 * @return
	 */
	Vector3 * GetNewTextureCooords(int size); //TODO //CDJ

	/**
	 *
	 * @return
	 */
	int GetUseIndexVertexNumber(); //TODO //CDJ

	/**
	 * 设置索引的相对于数据起点的偏移量
	 * @param offset
	 */
	void SetIndexOffset(M3D_OFFSET_TYPE offset);

	/**
	 * 获取索引相对于数据起点的偏移量
	 */
	M3D_OFFSET_TYPE GetIndexOffset();

	/**
	 * 设置法向量数据相对于数据起点的偏移量
	 * @param offset
	 */
	void SetNormalOffset(M3D_OFFSET_TYPE offset);

	/**
	 * 获取法向量数据相对应数据起点的偏移量
	 */
	M3D_OFFSET_TYPE GetNormalOffset();

	/**
	 * 设置顶点相对数据起点的偏移量
	 * @param offset
	 */
	void SetVertexOffset(M3D_OFFSET_TYPE offset);

	/**
	 * 获取顶点坐标相对数据起点的偏移量
	 */
	M3D_OFFSET_TYPE GetVertexOffset();

	/**
	 * 设置纹理坐标相对数据起点的偏移量
	 * @param offset
	 */
	void SetTextureCoordsOffset(M3D_OFFSET_TYPE offset);

	/**
	 * 获取纹理坐标相对数据起点的偏移量
	 */
	M3D_OFFSET_TYPE GetTextureCoordsOffset();

	/**
	 * 获取此Vertexset顶点数据在GPU端的缓存
	 * @return NULL 说明没有硬件缓存
	 */
	HardWareVertexBuffer* GetHardWareVertexBuffer();

	/**
	 *
	 * 获取索引数据在GPU端的缓存
	 * @return NULL 说明没有硬件缓存
	 */
	HardWareIndexBuffer* GetHardWareIndexBuffer();
	bool RecesiveShadow() const { return m_recesiveShadow; }
	void RecesiveShadow(bool val) { m_recesiveShadow = val; }
private:
	void Init();

	bool HardwarebufferValid(Mesh* mesh);

protected:
	bool m_frontShow;//!< 最前端显示
	bool m_useIndex;//!<是否使用索引解析数据
	bool m_bCullBackFace;//Add by zyp 是否使用双面光照

	Matrix4* m_renderWorldMatrix;//!< 世界矩阵

	//TODO 后面通过偏移量 将这些值给优化掉
	Vector3* m_vertexPos;//!< 顶点坐标起始地址

	Vector3* m_vertexNormal;//!<法向量起始地址

	Vector3* m_textureCoords;//!<文理坐标起始地址

	M3D_INDEX_TYPE*  m_index;//!<索引起始地址

	Vector3* m_linesVertexPos;//!< 以线框模式渲染顶点坐标起始地址

	M3D_INDEX_TYPE* m_linesIndex;//!<以线框模式渲染索引起始地址

	HardWareIndexBuffer* m_hardWareIndexBuffer;   //!< 索引数据在GPU端的缓存
	HardWareVertexBuffer* m_hardWareVertexBuffer;  //!< 顶点数据在GPU端的缓存，可能包括顶点坐标、法向量、纹理等

	int m_length;//!<数据的总长度
	//int m_primitive;//!<表达图元的类型，0 点 1 线 2三角形

	//VBO 数据偏移量纪录
	M3D_OFFSET_TYPE m_indexOffset; //!< 索引偏移量
	M3D_OFFSET_TYPE  m_normalOffset; //!< 法向量偏移量
	M3D_OFFSET_TYPE m_vertexOffset; //!< 顶点坐标偏移量
	M3D_OFFSET_TYPE m_textureCoordsOffset; //!<纹理坐标数据偏移量
	bool m_recesiveShadow;

	//std::vector<Vector3> m_newTextureCoords;//TODO cdj
	//int m_useIndexVertexNumber;//TODO cdj
};

}
#endif // M3D_RENDERABLE_H_
