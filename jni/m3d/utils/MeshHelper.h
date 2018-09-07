/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_MESH_HELPER_H
#define M3D_MESH_HELPER_H

#include "m3d/M3D.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/model/Model.h"
#include "FileHelper.h"
#include "sview/io/Reader.h"

class FTFont;

namespace M3D{

class Mesh;
class VertexSet;

/**
 * @class TODO
 * @brief mesh帮助类。提供字符串分元工具函数
 */
class M3D_API MeshHelper
{
public:
	//static float axisPntX[];
	static VertexSet* handlerVertexs;
	static Mesh* axisXMesh;

	static float axisPntO[];

	static Mesh* pointMesh;

	static int PntONum;
public:
	static bool Init();
	/**
	 * 得到传入字符串分元后的网格
	 * @param texts
	 * @param vertexSet
	 * @param meshData
	 * @param ftglFont
	 * @return
	 */
	static bool GetTextsMesh(const string& texts,VertexSet& vertexSet,Mesh& meshData,FTFont& ftglFont);

	/**
	 * 得到箭头数据
	 * @return
	 */
	static Mesh* GetArrowMesh();

	static string createTextImage(SceneManager * scene, string & noteStr, float fontSize);

	/**
	 * 得到圆环数据
	 * @return
	 */
	static Mesh* GetCylinerMesh();

	/**
	 * 得到缩放箭头数据
	 * @return
	 */
	static Mesh* GetScaleArrowMesh();

	/**
	 * 得到面数据
	 * @return
	 */
	static Mesh* GetPlaneMesh();

	/**
	 * 加载obj数据
	 * @param filePath
	 * @return
	 */
	static Mesh* GetVertexSetFromObj(const string& filePath);

	static Vector3 TransformPosition(Vector3& position,Matrix3x4& transform);

	static Vector3 TransformNormal(Vector3& normal, Matrix3x4& transform);

	static bool ReadSingleModel(const string& path, Model* & singleModel);

	/**
	 * 获取点的显示Mesh，来实现点的不同显示效果
	 * @return
	 */
	static Mesh* GetPointMesh(float scale = 1.0f);

	/**
	* 生成Mesh对应的纹理坐标数据
	* @return
	*/
	static bool GenTextureCoords(VertexSet*vertexSet,bool force = true);

};
}
#endif //M3D_MESH_HELPER_H
