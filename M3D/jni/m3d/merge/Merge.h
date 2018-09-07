#ifndef M3D_MODEL_MERGE_H_
#define M3D_MODEL_MERGE_H_
#include "m3d/model/Model.h"

namespace M3D {

	enum PartMergeLevelEnum
	{
		PARTMERGELEVEL_BODY = 1,
		PARTMERGELEVEL_FACE = 2,
		PARTMERGELEVEL_LINESET = 4,
	};
	enum EliminateDataEnum
	{
		ELIMINATEDATA_NONE = 0,			//²»ÌÞ³ý
		ELIMINATEDATA_SUBLINE = 1,		//ÌÞ³ý²ÎÕÕÏß
		ELIMINATEDATA_LINESET = 2,		//ÌÞ³ýÏß¼¯
		ELIMINATEDATA_EDGE = 4,			//ÌÞ³ý±ß¿òÏß
	};
	class SPolyLine;
	class M3D_API Merge
	{
	public:
		Merge();
		~Merge();

	public:
		static bool MergeModels(Model* srcModel, Model* destModel, int MergeFlg, int iEliminateFlg, bool NeedSetMatrix = false);

		static bool MergeBodys(Body* srcBody, Body* destBody, 
			int MergeFlg, int iEliminateFlg, bool NeedSetMatrix, Matrix3x4& worldMatrix);

		static bool MergeFaces(Face* srcFace, Face* destFace, bool AddMeshData, bool NeedSetMatrix, Matrix3x4& worldMatrix);
		static bool MergeEdges(Edge* srcEdge, Edge* destEdge, int offset, bool NeedSetMatrix, Matrix3x4& worldMatrix);

		static string Merge::CalculateParentPlcPath(string strPlcPath1, string strPlcPath2);
		static void WorldTransform(Model* model);

		static void SetInitStep();
		static int GetStep();
	private:
		static void ResizeMeshIndex(vector<Face*>& faceArray, Face* destFace, Face* srcFace);
		static void ResizeLineIndex(vector<Edge*>* edgeArray, Edge* destEdge, Edge* srcEdge);
		static bool CompareBodyColor(Body* srcBody, Body* destBody);
		static bool CompareColor(Color* color1, Color* color2);

	private:
		static int m_Step;
	};

}
#endif