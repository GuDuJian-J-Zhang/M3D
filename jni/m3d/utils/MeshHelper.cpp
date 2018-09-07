
#include "m3d/utils/MeshHelper.h"
#include "m3d/model/MeshData.h"
#include <vector>
using namespace std;

#ifdef __MOBILE__
#include "FTGL/ftgles.h"
#elif _WIN32
#include "ftgl/ftgl.h"
#endif
#include "../extras/measure/tools/MeasureDisplayHelper.h"
#include "../base/glue/GlueObj.h"
#include "../SceneManager.h"

namespace M3D{

VertexSet* MeshHelper::handlerVertexs = NULL;
Mesh* MeshHelper::axisXMesh = NULL;
 
Mesh* MeshHelper::pointMesh = NULL;

int MeshHelper::PntONum = 3240;

bool MeshHelper::Init()
{


	return true;
}

bool MeshHelper::GetTextsMesh(const string& texts,VertexSet& vertexSet,
		Mesh& meshData,FTFont& ftglFont)
{
//	LOGI("MeshHelper::GetTextsMesh begin");
	vector<FTPoint> vecTextPnt;
	vector<int> vecNums;
	vector<int> vecTypes;
	FTPoint position;
	int faceSize = ftglFont.FaceSize();
	FTPoint Space (0,0,0);
//	LOGI("MeshHelper::GetTextsMesh text:%s length:%d",texts.c_str(),texts.length());
//	ftglFont.Render(texts.c_str(), vecTextPnt, vecNums, vecTypes, -1,
//			position,Space);
	//因字库中貌似没有utf8的符号，改成wstring试下
	wstring wstr = StringToWString(texts,"utf8");
	ftglFont.Render(wstr.c_str(),vecTextPnt, vecNums, vecTypes, -1,position,Space);
//	LOGI("MeshHelper::GetTextsMesh ftglFont.Render ok.");

	/************************************************************************/
	/* buildTriangle                                                                                    */
	/************************************************************************/
	vector<int> faceindexlist;
	vector<int> edgeindexlist;
	int triangle_indices[3];
	int triangle_fan_root_index = 0;
	int triangle_index_counter = 0;
	bool triangle_strip_flipflop = false;

	unsigned int triangle_mode;

	//LOGI("MeshHelper::GetTextsMesh vecNums:%d",vecNums.size());
	//copy from pc
	int i, j, tIndex = 0, wIndex, lastEdgeIndex = -1;
	for (i = 0; i < vecNums.size(); i++)
	{
		triangle_mode = vecTypes[i];
		if (triangle_mode == 6 /*GL_TRIANGLE_FAN*/)
			triangle_fan_root_index = -1;
		else
			triangle_fan_root_index = 0;
		triangle_index_counter = 0;
		triangle_strip_flipflop = false;

		//LOGI("MeshHelper::GetTextsMesh vecNums[i]:%d",vecNums[i]);
		lastEdgeIndex = -1;
		for (j = 0; j < vecNums[i]; j++, tIndex++)
		{
			if (-1 == lastEdgeIndex)
				lastEdgeIndex = tIndex;
			else
			{
				edgeindexlist.push_back(lastEdgeIndex);
				edgeindexlist.push_back(tIndex);
				lastEdgeIndex = tIndex;
			}
			wIndex = tIndex;
			if ((triangle_fan_root_index == -1)
					&& (triangle_index_counter == 0))
			{
				triangle_fan_root_index = wIndex;
			}

			if (triangle_mode == 6 /*GL_TRIANGLE_FAN*/)
			{
				if (triangle_index_counter == 0)
				{
					triangle_indices[0] = triangle_fan_root_index;
					triangle_indices[1] = wIndex;
					++triangle_index_counter;
				}
				else
					triangle_indices[triangle_index_counter++] = wIndex;
			}
			else
			{
				triangle_indices[triangle_index_counter++] = wIndex;
			}

			assert(triangle_index_counter < 4);

			if (triangle_index_counter == 3)
			{
				if (triangle_mode == 5/*GL_TRIANGLE_STRIP*/)
				{
					if (triangle_strip_flipflop)
					{
						wIndex = triangle_indices[1];
						triangle_indices[1] = triangle_indices[2];
						triangle_indices[2] = wIndex;
					}
				}

				faceindexlist.push_back(triangle_indices[0]);
				faceindexlist.push_back(triangle_indices[1]);
				faceindexlist.push_back(triangle_indices[2]);

				if (triangle_mode == 6/*GL_TRIANGLE_FAN*/)
				{
					triangle_indices[1] = triangle_indices[2];
					triangle_index_counter = 2;
				}
				else if (triangle_mode == 5/*GL_TRIANGLE_STRIP*/)
				{
					if (triangle_strip_flipflop)
					{
						wIndex = triangle_indices[1];
						triangle_indices[1] = triangle_indices[2];
						triangle_indices[2] = wIndex;
					}

					triangle_indices[0] = triangle_indices[1];
					triangle_indices[1] = triangle_indices[2];
					triangle_index_counter = 2;
					triangle_strip_flipflop = !triangle_strip_flipflop;

				}
				else
					triangle_index_counter = 0;
			}
		}
	}
	vector<Vector3>* pnts = vertexSet.GetPositionArray();

	int faceIndex = 0;
	//解索引数据，按照数组的方式进行存储
	for (int i = 0; i < faceindexlist.size(); ++i)
	{
		faceIndex = faceindexlist[i];
		const FTPoint& ftPnt = vecTextPnt[faceIndex];
		Vector3 pnt(ftPnt.X(), ftPnt.Y(), ftPnt.Z());
		pnts->push_back(pnt);
	}

	meshData.SetDataOffset(0);
	meshData.SetDataLength(pnts->size());
//	LOGI("MeshHelper::GetTextsMesh end.");
	return true;

//		/************************************************************************/
//
//			struct cc_font_vector_glyph * vector_glyph = (struct cc_font_vector_glyph *) malloc(sizeof(struct cc_font_vector_glyph));
//
//			/************************************************************************/
//			/* buildVertexList                                                                                */
//			/************************************************************************/
//			{
//				int numcoords,i;
//				float * coord;
//
//				numcoords = vecTextPnt.size();
//				vector_glyph->vertices = (float *) malloc(sizeof(float)*numcoords*2);
//
//				for (i=0;i<numcoords;++i)
//				{
//					vector_glyph->vertices[i*2 + 0] = vecTextPnt[i].Xf() / 64.0 / 40;
//					vector_glyph->vertices[i*2 + 1] = vecTextPnt[i].Yf() / 64.0 / 40;
//				}
//			}
//
//			/************************************************************************/
//
//
//			/************************************************************************/
//			/* buildFaceIndexList                                                                           */
//			/************************************************************************/
//			{
//				int len, i;
//				len = faceindexlist.size()+1;
//				vector_glyph->faceindices = (int *) malloc(sizeof(int)*len);
//
//				for (i=0;i<len-1;++i)
//					vector_glyph->faceindices[i] =faceindexlist[i];
//
//				vector_glyph->faceindices[i] = -1;
//			}
//			/************************************************************************/
//
//			/************************************************************************/
//			/* buildEdgeIndexList                                                                           */
//			/************************************************************************/
//			{
//				int i,len;
//				len = edgeindexlist.size()+1;
//				vector_glyph->edgeindices = (int *) malloc(sizeof(int)*len);
//
//				for (i=0;i<len-1;++i)
//					vector_glyph->edgeindices[i] = edgeindexlist[i];
//
//				vector_glyph->edgeindices[i] = -1;
//			}
//
//			/************************************************************************/
//			/* calcboundingbox                                                                             */
//			/************************************************************************/
//			{
//		#if 0
//				float minX, minY, maxX, maxY;
//				minX = minY = maxX = maxY = 0;
//				float vecX, vecY;
//				/* 计算文字包围盒大小*/
//				for (int i=0; i<vecTextPnt.size(); i++)
//				{
//					vecX = vector_glyph->vertices[i*2 + 0];
//					vecY = vector_glyph->vertices[i*2 + 1];
//
//					maxX = maxX < vecX ? vecX : maxX;
//					maxY = maxY < vecY ? vecY : maxY;
//					minX =  minX > vecX ? vecX : minX;
//					minY =  minX > vecY ? vecY : minY;
//				}
//
//				glyph->bbox[0] = minX;
//				glyph->bbox[1] = minY;
//				glyph->bbox[2] = maxX;
//				glyph->bbox[3] = maxY;
//				glyph->width = glyph->bbox[2] - glyph->bbox[0];
//		#endif
//			}
//			/************************************************************************/

}


/**
 * 获取到箭头mesh数据
 * 整个应用程序唯一，在应用结束时，才会删除
 * @return
 */
Mesh*  MeshHelper::GetArrowMesh()
{
	if(!MeshHelper::axisXMesh)
	{
		if(!MeshHelper::handlerVertexs)
		{
			MeshHelper::handlerVertexs = new VertexSet();
		}

		MeshHelper::axisXMesh = new Mesh(MeshHelper::handlerVertexs);

		/*float* pnts = MeshHelper::axisPntX;
		vector<Vector3>* vertexsArray = handlerVertexs->GetPositionArray();

		axisXMesh->SetDataOffset(vertexsArray->size());
		int axisPntsCount = 1062/3;

		for(int i = 0;i<axisPntsCount;i++)
		{
			Vector3 axisPnt(pnts[i*3],pnts[i*3 + 1],pnts[i*3 + 2]);
			vertexsArray->push_back(axisPnt);
		}
		axisXMesh->SetDataLength(axisPntsCount);*/
	}
	return MeshHelper::axisXMesh;
}

string  MeshHelper::createTextImage(SceneManager* scene,
	string&noteStr, float fontSize)
{
	//SVIEW::Parameters parameter;
	Shape2DSet * shape2DSet = new Shape2DSet();

	if (fontSize < 1.0)
		fontSize = 15.0f;
	fontSize = fontSize * 40 / 15.0f;
	GlueObj* gluObj = scene->GetGlueObj();

	int notZh = 0;
	int charNumber = MeasureDisplayHelper::countChar(noteStr, notZh);
#ifdef WIN32
	float strLength = notZh * 0.6f + (charNumber - notZh) * 2;
#else
	float strLength = notZh * 0.6f + (charNumber - notZh);
#endif
	float RectWight = (strLength + 1) * fontSize;
#ifdef WIN32
	float ScaleFactor = 1.0f;
	Vector2 tempStart(0.5, 0.5); //2.5 为调节因子
	Vector2 tempEnd(RectWight, fontSize * 3.5 + 1);
	Vector2 tempStartNoUse(RectWight, fontSize * 3.5 + 1);
	gluObj->GetMeasureTextFun()(noteStr.c_str(), fontSize, "楷体", (float*)tempStartNoUse.Data(), (float*)tempEnd.Data());
	tempEnd.m_y = tempEnd.m_y + 1.0f;//留出结束空白字符
	tempEnd.m_x = tempEnd.m_x*0.7;
	tempEnd.m_x = tempEnd.m_x + 10.0f;//留出结束空白字符
	tempStart = tempStart*ScaleFactor;
	tempEnd = tempEnd*ScaleFactor;
	RectWight = tempEnd.m_x;
	fontSize = fontSize* ScaleFactor;
#else	
	Vector2 tempStart(1, 1); //2.5 为调节因子
	Vector2 tempEnd(RectWight, fontSize * 2.5 + 1);
#endif // WIN32	
	Color blk = Color::BLACK;
	Color gr = Color::GRAY;
	Color wt = Color::WHITE;
	Color textColor(0.15, 0.15, 0.15);
	Color lineColor(0.03, 0.38, 0.73);
	float lineWidth = 4.0f;
	MeasureDisplayHelper::CreateNoteRectangleImage(shape2DSet, tempStart, tempEnd, lineColor,
		lineWidth, wt, textColor, fontSize, noteStr);

#ifdef WIN32
	float wideFactor = RectWight / 90.0f;
	float heightFactor = (fontSize * 1.7) / 90.0f;
#else
	float wideFactor = RectWight / 100.0f;
	float heightFactor = (fontSize * 2.5) / 100.0f;
#endif
	string createImagePath = "";
	if (gluObj)
	{
		createImagePath = gluObj->createImage(shape2DSet, scene);
	}
	delete shape2DSet;
	shape2DSet = NULL;

	return createImagePath;
}

Mesh* MeshHelper::GetCylinerMesh()
{
    return NULL;
}


Mesh* MeshHelper::GetScaleArrowMesh()
{
    return NULL;
}


Mesh* MeshHelper::GetPlaneMesh()
{
    return NULL;
}

Mesh* MeshHelper::GetVertexSetFromObj(const string& filePath)
{
    return NULL;
}

Vector3 MeshHelper::TransformPosition(Vector3& position, Matrix3x4& transform)
{
	return transform*position;
}

Vector3 MeshHelper::TransformNormal(Vector3& normal, Matrix3x4& transform)
{
	return transform*Vector4(normal.m_x,normal.m_y,normal.m_z,1.0f);
}


bool MeshHelper::ReadSingleModel(const string& path, Model* & singleModel)
{
	bool bRet = false;
	Model* model = NULL;
	string realPath = M3D::FileHelper::GetUnionStylePath(path);

	SVIEW::Reader* reader = SVIEW::Reader::GetReader(realPath);
	model = reader->GetModel();

	if (model)
	{
		singleModel = model;
		model->AddRef();
		bRet = true;
	}

	if (reader)
	{
		delete reader;
	}

	return bRet;
}

Mesh* MeshHelper::GetPointMesh(float scale)
{
	if(pointMesh == NULL)
	{
		/*VertexSet* refMesh = new VertexSet();
		vector<Vector3>* posArray = refMesh->GetPositionArray();
		Vector3 pos;

		for(int i=0;i< PntONum;i+=3)
		{
			pos.m_x = axisPntO[i]*scale;
			pos.m_y = axisPntO[i+1]*scale;
			pos.m_z = axisPntO[i+2]*scale;

			posArray->push_back(pos);
		}

		pointMesh = new Mesh(refMesh);

		pointMesh->SetDataOffset(0);
		pointMesh->SetDataLength(posArray->size());*/
	}

	return pointMesh;
}

bool MeshHelper::GenTextureCoords(VertexSet*vertexSet, bool force /*= true*/)
{
	bool  genState = true;
	if (vertexSet)
	{
#ifdef _WIN32
		//如果没有纹理坐标数据
		if (vertexSet->GetTextureCoordArray()->size() == 0 || force)
		{
			//仅支持使用索引方式解析的顶点
			if (vertexSet->IsUseIndex())
			{
				//获取顶点
				vector<Vector3>& pntsArray = *(vertexSet->GetPositionArray());
				////获取索引
				//vector<M3D_INDEX_TYPE>& indexArray = *(vertexSet->GetIndexArray());
				////构造UVGentor对象
				//vector<VEC3>vertex;

				//for (int i =0;i<pntsArray.size();++i)
				//{
				//	Vector3& pnt = pntsArray.at(i);
				//	VEC3 vNewVertex;
				//	vNewVertex.x = pnt.m_x;
				//	vNewVertex.y = pnt.m_y;
				//	vNewVertex.z = pnt.m_z;
				//	vertex.push_back(vNewVertex);
				//}

				//vector<CFace>face;
				//for (int i= 0;i<indexArray.size()/3;++i)
				//{
				//	CFace triFace;
				//	triFace.v[0] = indexArray.at(i*3);
				//	triFace.v[1]= indexArray.at(i * 3 + 1);
				//	triFace.v[2] = indexArray.at(i * 3 + 2);
				//	face.push_back(triFace);
				//}

				//vector<VEC2> pUVs;
				//vector<CFace> UVIndex;
				////UVfun::AutoMapping(vertex, face, pUVs, UVIndex, false, 60.0);
				//UVfun::BoxMapping(vertex, face, pUVs, UVIndex,false);

				vector<Vector3>& textureCoordinateArray = *(vertexSet->GetTextureCoordArray());
				textureCoordinateArray.clear();

				////由于UV坐标个数和顶点的个数对应不起来，处理一下，遍历遍历顶点根据顶点去找纹理坐标，让顶点和纹理坐标一一对应。
				//textureCoordinateArray.resize(vertex.size());
				//for (int i= 0;i<face.size();i++)
				//{
				//	CFace& subFace = face.at(i);
				//	CFace& subFaceUV = UVIndex.at(i);

				//	int genUVIndex0 = subFaceUV.v[0];
				//	int genUVIndex1 = subFaceUV.v[1];
				//	int genUVIndex2 = subFaceUV.v[2];

				//	VEC2& genUV0 = pUVs[genUVIndex0];
				//	VEC2& genUV1 = pUVs[genUVIndex1];
				//	VEC2& genUV2 = pUVs[genUVIndex2];

				//	int faceIndex0 = subFace.v[0];
				//	int faceIndex1 = subFace.v[1];
				//	int faceIndex2 = subFace.v[2];

				//	Vector3& vUV0 = textureCoordinateArray[faceIndex0];
				//	Vector3& vUV1 = textureCoordinateArray[faceIndex1];
				//	Vector3& vUV2 = textureCoordinateArray[faceIndex2];

				//	vUV0.m_x = genUV0.u;
				//	vUV0.m_y = genUV0.v;
				//	vUV0.m_z = 0;

				//	vUV1.m_x = genUV1.u;
				//	vUV1.m_y = genUV1.v;
				//	vUV1.m_z = 0;

				//	vUV2.m_x = genUV2.u;
				//	vUV2.m_y = genUV2.v;
				//	vUV2.m_z = 0;
				//}

				Vector4 plane_S(1,0,0,0);
				Vector4 plane_T(0,1,0,0);

				BoundingBox& box = vertexSet->GetBoundingBox();
				textureCoordinateArray.resize(pntsArray.size());
				//测试一下最简单的线性投影的方式
				for (int i = 0; i < pntsArray.size(); i++)
				{
					Vector3& vUV0 = textureCoordinateArray[i];
					Vector3 pntPos = pntsArray[i];
					Vector4 pntPos4 = Vector4(pntPos,1.0f);

					vUV0.m_x = pntPos4.DotProduct(plane_S)/box.GetXLength();
					vUV0.m_y = pntPos4.DotProduct(plane_T)/box.GetYLength();
					vUV0.m_z = 0;
				}

				//暂时认为UV输出的索引和顶点默认的索引移植，后面再测试准确性
				//清空一下对应的GPU数据，
				vertexSet->ClearGPUData();
				//让绘制时在GPU线程进行更新
				//vertexSet->UpdateHardWareBuffer(scene);
			}
		}
#endif
	}

	return genState;
}

}
