#include "Merge.h"
#include "m3d/model/ModelShape.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/Curve.h"
#include "m3d/model/Edge.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/model/ExtendInfoManager.h"

namespace M3D {

	int Merge::m_Step = 0;
	Merge::Merge()
	{

	}
	Merge::~Merge()
	{

	}

	bool Merge::MergeModels(Model* srcModel, Model* destModel, int MergeFlg, int iEliminateFlg, bool NeedSetMatrix)
	{
		bool ret = false;
		if (!srcModel || !destModel)
		{
			return ret;
		}

		bool bMergeBody = false;
		if ((MergeFlg&PARTMERGELEVEL_BODY) != 0)
		{
			bMergeBody = true;
		}
		Matrix3x4 worldMatrix = srcModel->GetWorldTransform();
		ModelShape * modelShape = destModel->GetModelShape();
		if (modelShape->IsDrawDataPrepared())
		{
			bool bSrcSameDest = false;
			vector<Body*>* destBodys = destModel->GetBodys();
			vector<Body*>* srcBodys = srcModel->GetBodys();
			vector<Body*> tmpBodys;
			if (destBodys == srcBodys)
			{
				bSrcSameDest = true;
			}
			if (srcBodys && destBodys)
			{
				if (srcBodys->size() > 0)
				{
					m_Step++;
				}
				for (vector<Body*>::iterator it = srcBodys->begin(); it != srcBodys->end();it++)
				{
					Body* srcBody = *it;
					if (!srcBody)
					{
						continue;
					}

					bool bSameColor = false;
					Body* pDestBody = NULL;
					
					if (bSrcSameDest)
					{
						for (int m = 0; m < tmpBodys.size(); m++)
						{
							Body* body = tmpBodys[m];
							if (CompareBodyColor(srcBody, body))
							{
								pDestBody = body;
								break;
							}
						}
					}
					else
					{
						for (int j = 0; j < destBodys->size(); j++)
						{
							Body* destBody = destBodys->at(j);
							if (CompareBodyColor(srcBody, destBody))
							{
								bSameColor = true;
								pDestBody = destBody;
								break;
							}
						}
					}

					if (!pDestBody || !bMergeBody)
					{
						pDestBody = new Body();
						pDestBody->SetType(SHAPE_BODY);
						if (bSrcSameDest)
						{
							tmpBodys.push_back(pDestBody);
						}
						else
						{
							destModel->AddBody(pDestBody);
						}
					}
					MergeBodys(srcBody, pDestBody, MergeFlg, iEliminateFlg, NeedSetMatrix, worldMatrix);
				}

				if (tmpBodys.size() > 0)
				{
					destBodys->resize(0);
					for (int i = 0; i < tmpBodys.size(); i++)
					{
						destModel->AddBody(tmpBodys[i]);
					}
				}
			}
			
			vector<Model *> childModels = srcModel->GetSubModels();
			for (int i = 0; i < childModels.size(); i++)
			{
				Model* child = childModels[i];
				MergeModels(child, destModel, MergeFlg, iEliminateFlg, true);
			}
			if (!NeedSetMatrix)
			{
				if (srcModel->GetParent()&& childModels.size() > 0)
				{
					//srcModel->SetPlaceMatrix(srcModel->GetParent()->GetWorldTransform().Inverse());
					srcModel->SetPlaceMatrix(Matrix3x4::IDENTITY);
				}
			}
		}
		
		return true;
	}

	bool Merge::MergeBodys(Body* srcBody, Body* destBody,
		int MergeFlg, int iEliminateFlg, bool NeedSetMatrix, Matrix3x4& worldMatrix)
	{
		if (!srcBody || !destBody)
		{
			return false;
		}

		bool bMergeFace = false;
		bool bMergeLineSet = false;
		bool bRmvEdge = false;//
		if ((MergeFlg&PARTMERGELEVEL_FACE) != 0)
		{
			bMergeFace = true;
		}
		if ((MergeFlg&PARTMERGELEVEL_LINESET) != 0)
		{
			bMergeLineSet = true;
		}
		if ((iEliminateFlg&ELIMINATEDATA_EDGE) != 0)
		{
			bRmvEdge = true;
		}

		VertexSet* srcVertexSet = srcBody->GetData();
		VertexSet* destVertexSet = destBody->GetData();
		if (!destVertexSet)
		{
			destVertexSet = new VertexSet();
		}
		std::vector<Vector3>* mergePnts = destVertexSet->GetPositionArray();
		int mergePntCount = mergePnts ? mergePnts->size() : 0;

		SPolyLine* destPolyLine = destBody->GetPolyLine();
		SPolyLine* srcPolyLine = srcBody->GetPolyLine();
		if (destPolyLine == NULL)
		{
			destPolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
			destBody->SetPolyLine(destPolyLine);
		}

		if (!bRmvEdge)
		{
			vector<Edge*>* srcVec = srcBody->GetEdges();
			vector<Edge*>* destVec = destBody->GetEdges();
			for (int i = 0; i < srcVec->size(); i++)
			{
				Edge * edge = srcVec->at(i);
				if (edge)
				{
					Edge* pDestEdge = NULL;
					Color* srcColor = edge->GetColor();
					for (int j = 0; j < destVec->size(); j++)
					{
						Edge* destEdge = destVec->at(j);
						if (destEdge)
						{
							Color* destColor = destEdge->GetColor();
							if (CompareColor(srcColor, destColor))
							{
								pDestEdge = destEdge;
								break;
							}
						}
					}
					if (!pDestEdge || !bMergeLineSet)
					{
						pDestEdge = new Edge();
						pDestEdge->SetColor(*(edge->GetColor()));
						pDestEdge->SetSelected(edge->IsSelected());
						pDestEdge->SetVisible(edge->IsVisible());
						pDestEdge->SetInitHightlight(false);
						RefPolyLine * edgeLine = new RefPolyLine(destPolyLine);
						edgeLine->SetDataOffset(0);
						edgeLine->SetDataLength(0);
						pDestEdge->AddData(edgeLine);
						destBody->AddEdge(pDestEdge);
					}
					MergeEdges(edge, pDestEdge, mergePntCount, NeedSetMatrix, worldMatrix);
					ResizeLineIndex(destVec, pDestEdge, edge);
				}
			}
		}
		

		vector<Face*>& srcFaces = srcBody->GetFaces();
		vector<Face*>& destFaces = destBody->GetFaces();
		bool bAddMesh = true;
		for (int i = 0; i < srcFaces.size(); i++)
		{
			Face* srcFace = srcFaces.at(i);
			if (srcFace)
			{
				Face* pDestFace = NULL;
				Color* srcColor = srcFace->GetColor();
				for (int j = 0; j < destFaces.size(); j++)
				{
					Face* destFace = destFaces.at(j);
					if (destFace)
					{
						Color* destColor = destFace->GetColor();
						if (CompareColor(srcColor, destColor))
						{
							pDestFace = destFace;
							break;
						}
					}
				}

				if (!pDestFace || !bMergeFace)
				{
					pDestFace = new Face();
					pDestFace->SetSelected(srcFace->IsSelected());
					pDestFace->SetVisible(srcFace->IsVisible());
					pDestFace->SetInitHightlight(false);
					pDestFace->SetColor(*(srcFace->GetColor()));
					pDestFace->SetMaterial(srcFace->GetMaterial());

					//VertexSet* bodyDrawData = new VertexSet();
					Mesh* facemeshData = new Mesh(destVertexSet);
					facemeshData->SetDataOffset(0);
					facemeshData->SetDataLength(0);
					pDestFace->SetData(facemeshData);
					destBody->AddFace(pDestFace);
				}

				MergeFaces(srcFace, pDestFace, bAddMesh, NeedSetMatrix, worldMatrix);
				ResizeMeshIndex(destFaces, pDestFace, srcFace);
				if (bAddMesh)
				{
					bAddMesh = false;
				}
			}
		}
		
		return true;
	}

	bool Merge::MergeEdges(Edge* srcEdge, Edge* destEdge, int offset, bool NeedSetMatrix, Matrix3x4& worldMatrix)
	{
		if (!srcEdge || !destEdge)
		{
			return false;
		}
		RefPolyLine* srcRefLine = srcEdge->GetLineData();
		RefPolyLine* destRefLine = destEdge->GetLineData();
		if (!srcRefLine || !destRefLine)
		{
			return false;
		}
		SPolyLine* srcPolyLine = srcRefLine->GetRefLine();
		SPolyLine* destPolyLine = destRefLine->GetRefLine();

		unsigned int srcOffset = srcRefLine->GetDataOffset();
		unsigned int srcLength = srcRefLine->GetDataLength();
		unsigned int destOffset = destRefLine->GetDataOffset();
		unsigned int destLength = destRefLine->GetDataLength();

		vector<Vector3> points = srcPolyLine ? srcPolyLine->GetPoints() : points;
		vector<M3D_INDEX_TYPE> indexs = srcPolyLine ? srcPolyLine->GetPntIndex() : indexs;

		vector<Vector3>& destPoints = destPolyLine->GetPoints();
		vector<M3D_INDEX_TYPE>& destIndex = destPolyLine->GetPntIndex();
		int count = destPoints.size();
		
		vector<Vector3> tmpPoints;
		vector<M3D_INDEX_TYPE> tmpIndex;
		for (int i = srcOffset; i < srcOffset+srcLength; i++)
		{
			Vector3 pnt = NeedSetMatrix ? worldMatrix * Vector4(points[i], 1.0) : points[i];
			tmpPoints.push_back(pnt);
			tmpIndex.push_back(indexs[i] + offset);
		}

		if (destOffset == destLength && destOffset == 0)
		{
			destPoints.insert(destPoints.end(), tmpPoints.begin(), tmpPoints.end());
			destIndex.insert(destIndex.end(), tmpIndex.begin(), tmpIndex.end());
			destRefLine->SetDataOffset(count);
		}
		else
		{
			destPoints.insert(destPoints.begin() + destOffset + destLength, tmpPoints.begin(), tmpPoints.end());
			destIndex.insert(destIndex.begin() + destOffset + destLength, tmpIndex.begin(), tmpIndex.end());
		}
		destRefLine->SetDataLength(destLength + srcLength);
	}

	bool Merge::MergeFaces(Face* srcFace, Face* destFace, bool AddMeshData, bool NeedSetMatrix, Matrix3x4& worldMatrix)
	{
		if (!srcFace || !destFace)
		{
			return false;
		}

		int nCount = 0; 
		Mesh* srcMesh = srcFace->GetData();
		Mesh* destMesh = destFace->GetData();

		VertexSet*  srcVertexSet = srcMesh->GetRefMesh();
		VertexSet* destVertexSet = destMesh->GetRefMesh();

		std::vector<Vector3>* destPnts = destVertexSet->GetPositionArray();
		std::vector<Vector3>* destNormals = destVertexSet->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* destIndexArray = destVertexSet->GetIndexArray();
		std::vector<Vector3>* destTextures = destVertexSet->GetTextureCoordArray();

		std::vector<Vector3>* srcPnts = srcVertexSet->GetPositionArray();
		std::vector<Vector3>* srcNormals = srcVertexSet->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* srcIndexArray = srcVertexSet->GetIndexArray();
		std::vector<Vector3>* srcTextures = srcVertexSet->GetTextureCoordArray();

		unsigned int srcOffset = srcMesh->GetDataOffset();
		unsigned int srcLength = srcMesh->GetDataLength();
		unsigned int destOffset = destMesh->GetDataOffset();
		unsigned int destLength = destMesh->GetDataLength();

		if (AddMeshData)
		{
			if (srcVertexSet->IsUseIndex())
			{
				destVertexSet->SetUseIndex(true);
				nCount = destPnts->size();
				for (int i = 0; i < srcPnts->size(); i++)
				{
					Vector3 pnt = NeedSetMatrix ? worldMatrix * Vector4(srcPnts->at(i), 1.0) : srcPnts->at(i);
					destPnts->push_back(pnt);
				}
				for (int i = 0; i < srcNormals->size(); i++)
				{
					Vector3 normal = NeedSetMatrix ? worldMatrix * Vector4(srcNormals->at(i), 1.0) : srcNormals->at(i);
					destNormals->push_back(normal);
				}
				for (int i = 0; i < srcTextures->size(); i++)
				{
					Vector3 texture = NeedSetMatrix ? worldMatrix * Vector4(srcTextures->at(i), 1.0) : srcTextures->at(i);
					destTextures->push_back(texture);
				}
			}
		}
		else
		{
			if (srcVertexSet->IsUseIndex())
			{
				nCount = destPnts->size() - srcPnts->size();
			}
		}
		if (srcVertexSet->IsUseIndex())
		{
			std::vector<M3D_INDEX_TYPE> tmpIndexArray;
			for (int i = srcOffset; i < srcOffset + srcLength; i++)
			{
				M3D_INDEX_TYPE nIndex = srcIndexArray->at(i);
				tmpIndexArray.push_back(nIndex + nCount);
			}
			
			if (destOffset == destLength && destOffset == 0)
			{
				int count = destIndexArray->size();
				destIndexArray->insert(destIndexArray->end(), tmpIndexArray.begin(), tmpIndexArray.end());
				destMesh->SetDataOffset(count);
			}
			else
			{
				destIndexArray->insert(destIndexArray->begin() + destOffset + destLength, tmpIndexArray.begin(), tmpIndexArray.end());
			}

			destMesh->SetDataLength(destLength + srcLength);
		}
	}

	void Merge::ResizeMeshIndex(vector<Face*>& faceArray, Face* destFace, Face* srcFace)
	{
		if (!srcFace || !destFace)
		{
			return;
		}
		Mesh* srcMesh = srcFace->GetData();
		Mesh* destMesh = destFace->GetData();

		unsigned int srcLength = srcMesh->GetDataLength();
		unsigned int destOffset = destMesh->GetDataOffset();
		unsigned int destLength = destMesh->GetDataLength();

		for (int i = 0; i < faceArray.size(); i++)
		{
			Face * face = faceArray.at(i);
			if (face)
			{
				Mesh* mesh = face->GetData();
				if (mesh != NULL)
				{
					unsigned int dataOffset = mesh->GetDataOffset();
					if (srcLength != destLength && dataOffset >= destOffset + destLength - srcLength )
					{
						mesh->SetDataOffset(dataOffset + srcLength);
					}
				}
			}
		}
	}

	void Merge::ResizeLineIndex(vector<Edge*>* edgeArray, Edge* destEdge, Edge* srcEdge)
	{
		if (!edgeArray || !destEdge || !srcEdge)
		{
			return;
		}

		RefPolyLine* srcRefLine = srcEdge->GetLineData();
		RefPolyLine* destRefLine = destEdge->GetLineData();
		if (!srcRefLine || !destRefLine)
		{
			return;
		}
		unsigned int srcLength = srcRefLine->GetDataLength();
		unsigned int destOffset = destRefLine->GetDataOffset();
		unsigned int destLength = destRefLine->GetDataLength();

		for (int i = 0; i < edgeArray->size(); i++)
		{
			Edge* edge = edgeArray->at(i);
			if (edge)
			{
				RefPolyLine* refLine = edge->GetLineData();
				if (refLine)
				{
					unsigned int offset = refLine->GetDataOffset();
					if (srcLength != destLength && offset >= destOffset + destLength - srcLength)
					{
						refLine->SetDataOffset(offset + srcLength);
					}
				}
			}
		}
	}

	bool Merge::CompareBodyColor(Body* srcBody, Body* destBody)
	{
		if (!srcBody || !destBody)
		{
			return false;
		}
		vector<Face*> srcFaces = srcBody->GetFaces();
		vector<Face*> destFaces = destBody->GetFaces();

		for (int i = 0; i < srcFaces.size(); i++)
		{
			Face* srcFace = srcFaces.at(i);
			if (!srcFace)
			{
				continue;
			}
			Color* srcColor = srcFace->GetColor();
			for (int j = 0; j < destFaces.size(); j++)
			{
				Face* destFace = destFaces.at(j);
				if (destFace)
				{
					Color* destColor = destFace->GetColor();
					if (Merge::CompareColor(srcColor, destColor))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool Merge::CompareColor(Color* color1, Color* color2)
	{
		bool bRet = false;
		if (!color1 || !color2)
		{
			return bRet;
		}
		if (fabs(color1->m_a - color2->m_a) < 0.001 &&
			fabs(color1->m_b - color2->m_b) < 0.001 &&
			fabs(color1->m_g - color2->m_g) < 0.001 &&
			fabs(color1->m_r - color2->m_r) < 0.001)
		{
			bRet = true;
		}
		return bRet;
	}

	string Merge::CalculateParentPlcPath(string strPlcPath1, string strPlcPath2)
	{
		string strRet = "";
		string strTmpPlcPath1 = strPlcPath1;
		string strTmpPlcPath2 = strPlcPath2;
		size_t iPlcPath = strTmpPlcPath1.find("PATH|");
		if (iPlcPath != -1)
			strTmpPlcPath1 = strTmpPlcPath1.substr(iPlcPath + 5, strTmpPlcPath1.size() - iPlcPath);
		iPlcPath = strTmpPlcPath2.find("PATH|");
		if (iPlcPath != -1)
			strTmpPlcPath2 = strTmpPlcPath2.substr(iPlcPath + 5, strTmpPlcPath2.size() - iPlcPath);

		std::vector<string> vecPlcID = StringHelper::Split(strTmpPlcPath1, "|");
		int iPlcPath1 = vecPlcID.size();
		vecPlcID.clear();
		vecPlcID = StringHelper::Split(strTmpPlcPath2, "|");
		int iPlcPath2 = vecPlcID.size();
		vecPlcID.clear();

		string strShortPlcPath, strLongPlcPath;
		if (iPlcPath1 >= iPlcPath2)
		{
			strShortPlcPath = strTmpPlcPath2;
			strLongPlcPath = strTmpPlcPath1;
		}
		else
		{
			strShortPlcPath = strTmpPlcPath1;
			strLongPlcPath = strTmpPlcPath2;
		}
		
		while (strShortPlcPath.size() > 0)
		{
			string strShortID = "", strLongID = "";
			size_t iShortPos = strShortPlcPath.find_first_of('|', 0);
			size_t iLongPos = -1;
			if (iShortPos == std::string::npos)
			{
				strShortID = strShortPlcPath;
				iLongPos = strLongPlcPath.find_first_of('|', 0);
				if (iLongPos == std::string::npos)
					strLongID = strLongPlcPath;
				else
					strLongID = strLongPlcPath.substr(0, iLongPos);
				if (strShortID == strLongID)
				{
					vecPlcID.push_back(strShortID);
				}
				else
					break;
			}
			else
			{
				strShortID = strShortPlcPath.substr(0, iShortPos);
				iLongPos = strLongPlcPath.find_first_of('|', 0);
				if (iLongPos == std::string::npos)
				{
					//正常情况下 这句不应该执行
					strLongID = strLongPlcPath;
				}
				else
					strLongID = strLongPlcPath.substr(0, iLongPos);
				if (strShortID == strLongID)
					vecPlcID.push_back(strShortID);
				else
					break;
			}
			strShortPlcPath = strShortPlcPath.substr(iShortPos + 1, strShortPlcPath.size() - iShortPos - 1);
			strLongPlcPath = strLongPlcPath.substr(iLongPos + 1, strLongPlcPath.size() - iLongPos - 1);
		}
		strRet = MAINMODELROOT;
		std::vector<string>::iterator itorPlcID = vecPlcID.begin();
		while (itorPlcID != vecPlcID.end())
		{
			strRet += "|";
			strRet += *itorPlcID;
			itorPlcID++;
		}
		return strRet;
	}

	void Merge::WorldTransform(Model* model)
	{
		if (!model)
		{
			return;
		}

		Matrix3x4 worldMatrix = model->GetWorldTransform();

		vector<Body*>* bodys = model->GetBodys();
		if (bodys)
		{
			for (int i = 0; i < bodys->size(); i++)
			{
				Body* body = bodys->at(i);
				if (body && body->GetPolyLine())
				{
					M3D::SPolyLine* bodyPolyLine = body->GetPolyLine();
					vector<Vector3>& linePnts = bodyPolyLine->GetPoints();
					for (int j = 0; j < linePnts.size(); j++)
					{
						linePnts[j] = worldMatrix * Vector4(linePnts[j],1.0);
					}
				}
				if (body)
				{
					VertexSet* vertexSet = body->GetData();
					std::vector<Vector3>* Pnts = vertexSet->GetPositionArray();
					std::vector<Vector3>* Normals = vertexSet->GetNormalArray();
					std::vector<Vector3>* Textures = vertexSet->GetTextureCoordArray();
					if (Pnts)
					{
						for (int j = 0; j < Pnts->size(); j++)
						{
							Pnts->at(j) = worldMatrix * Vector4(Pnts->at(j), 1.0);
						}
					}
					if (Normals)
					{
						for (int j = 0; j < Normals->size(); j++)
						{
							Normals->at(j) = worldMatrix * Vector4(Normals->at(j), 1.0);
						}
					}
					if (Textures)
					{
						for (int j = 0; j < Textures->size(); j++)
						{
							Textures->at(j) = worldMatrix * Vector4(Textures->at(j), 1.0);
						}
					}
				}
			}
		}
		
	}

	void Merge::SetInitStep()
	{
		m_Step = 0;
	}
	int Merge::GetStep()
	{
		return m_Step;
	}
}