#include "Reduction.h"
#include <set>
#include <iterator>
#include "m3d/base/MathDefs.h"
#include "m3d/model/ModelShape.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/Curve.h"

namespace M3D {

	typedef std::vector<int> VectorOfFace;
	typedef std::pair<GEdge, double> EdgeError;
	typedef std::vector<EdgeError> VectorOfEdgeError;

	bool cmp(const EdgeError& p1, const EdgeError& p2)
	{
		return p1.second < p2.second;
	}
	void sortEdgeErrors(Errors& errors, VectorOfEdgeError& vecErros)
	{
		copy(errors.begin(), errors.end(), std::back_inserter(vecErros));

		sort(vecErros.begin(), vecErros.end(), cmp);
	}

	Reduction::Reduction()
	{
		m_Step = 0;
	}
	Reduction::~Reduction()
	{

	}

	bool Reduction::MeshReduction(vector<Model*>& models, float percent)
	{
		if (models.size() <=0)
		{
			return false;
		}
		for (size_t i = 0; i < models.size(); i++)
		{
			Model* model = models[i];
			if (!model)
			{
				continue;
			}
			ModelShape * modelShape = model->GetModelShape();
			if (modelShape->IsDrawDataPrepared())
			{
				vector<Body*>* srcBodys = model->GetBodys();
				if (srcBodys)
				{
					for (vector<Body*>::iterator it = srcBodys->begin(); it != srcBodys->end(); it++)
					{
						m_Step++;
						Body* body = *it;
						if (!body)
						{
							continue;
						}

						//VertexReduction(body, 50);
						VertexReduction(body, percent);
					}

				}

				vector<Model *>& childModels = model->GetSubModels();
				MeshReduction(childModels, percent);
			}
		}
		

		return true;
	}

	int Reduction::VertexReduction(Body* body, const double angle)
	{
		VertexSet* vertexSet = body->GetData();
		SPolyLine* bodyPolyLine = body->GetPolyLine();
		if (!vertexSet)
		{
			return -2;
		}
		std::vector<Vector3>* vecVtx = vertexSet->GetPositionArray();
		std::vector<Vector3>* vecNor = vertexSet->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* vecTri = vertexSet->GetIndexArray();

		if (static_cast<int>(vecVtx->size()) < 3 || vecVtx->size() != vecNor->size()) {
			return -2;
		}

		double angVal = cos(angle*3.1415926 / 180.);

		map<int, int> mapOldNew;

		double epsilon = 1.e-10;


		Vector3 maxPnt, minPnt;

		for (vector<Vector3>::iterator it = vecVtx->begin(); it != vecVtx->end(); it++)
		{
			Vector3 position = *it;
			maxPnt.m_x = Max(position.m_x, maxPnt.m_x);
			minPnt.m_x = Min(position.m_x, minPnt.m_x);
			maxPnt.m_y = Max(position.m_y, maxPnt.m_y);
			minPnt.m_y = Min(position.m_y, minPnt.m_y);
			maxPnt.m_z = Max(position.m_z, maxPnt.m_z);
			minPnt.m_z = Min(position.m_z, minPnt.m_z);
		}

		double dx = maxPnt.m_x - minPnt.m_x;
		double dy = maxPnt.m_y - minPnt.m_y;
		double dz = maxPnt.m_z - minPnt.m_z;

		int NN = sqrt(double(vecVtx->size())) / 3;
		int NN1 = NN;
		int NN2 = NN;

		double D1, D2;
		int N1, N2;
		if (dx <= dy && dx <= dz) {
			N1 = 1;
			N2 = 2;
			D1 = (maxPnt.m_y - minPnt.m_y) / NN1;
			D2 = (maxPnt.m_z - minPnt.m_z) / NN2;
		}
		else if (dy <= dz && dy <= dx) {
			N1 = 2;
			N2 = 0;
			D1 = (maxPnt.m_z - minPnt.m_z) / NN1;
			D2 = (maxPnt.m_x - minPnt.m_x) / NN2;
		}
		else if (dz <= dx && dz <= dy) {
			N1 = 0;
			N2 = 1;
			D1 = (maxPnt.m_x - minPnt.m_x) / NN1;
			D2 = (maxPnt.m_y - minPnt.m_y) / NN2;
		}

		if (D1 < 1.e-05) {
			D1 = 1.;
		}
		if (D2 < 1.e-05) {
			D2 = 1.;
		}

		BiVecOfVectorOfVertex BiVecVertices;
		for (int i = 0; i <= NN1; i++) {
			VecOfVectorOfVertex vecvecV;
			for (int j = 0; j <= NN2; j++) {
				VectorOfVertex vecV;
				vecvecV.push_back(vecV);
			}
			BiVecVertices.push_back(vecvecV);
		}

		double xyz[3];
		int index = 0;
		for (vector<Vector3>::iterator it = vecVtx->begin(); it != vecVtx->end(); it++)
		{
			Vector3 v = *it;
			int i1, i2;
			switch (N1)
			{
			case 0:
				i1 = (v.m_x - minPnt.m_x) / D1;
				break;
			case 1:
				i1 = (v.m_y - minPnt.m_y) / D1;
				break;
			case 2:
				i1 = (v.m_z - minPnt.m_z) / D1;
				break;
			}
			switch (N2)
			{
			case 0:
				i2 = (v.m_x - minPnt.m_x) / D2;
				break;
			case 1:
				i2 = (v.m_y - minPnt.m_y) / D2;
				break;
			case 2:
				i2 = (v.m_z - minPnt.m_z) / D2;
				break;
			}

			VectorOfVertex& VecV = BiVecVertices[i1][i2];
			VecV.push_back(index++);
		}
		
		for (int i1 = 0; i1 <= NN1; i1++) {
			for (int i2 = 0; i2 <= NN2; i2++) {
				VectorOfVertex& VecV = BiVecVertices[i1][i2];

				int LL = VecV.size();

				for (int i = 0; i < LL - 1; i++) {
					int n1 = VecV[i];
					if (mapOldNew.find(n1) == mapOldNew.end()) {
						Vector3 vi = *(vecVtx->begin() + n1);//vecVtx[n1];
						Vector3 vni = *(vecNor->begin() + n1); //vecNor[n1];

						for (int j = i + 1; j < LL; j++) {
							int n2 = VecV[j];
							if (mapOldNew.find(n2) == mapOldNew.end()) {
								Vector3 vj = *(vecVtx->begin() + n2);//vecVtx[n2];
								if (squareDistance(vi, vj) < epsilon) {
									Vector3 vnj = *(vecNor->begin() + n2);//vecNor[n2];
									if (isParallel(vni, vnj, angVal)) {
										mapOldNew.insert(map<int, int>::value_type(n2, n1));
									}
								}
							}
						}
					}
				}
			}
		}

		BiVecVertices.clear();

		map<int, int> mapOldNode;
		int ii = 0;
		
		for (size_t i = 0; i < vecVtx->size(); i++)
		{
			int nn;
			if (mapOldNew.find(i) == mapOldNew.end()) {
				vector<Vector3>::iterator itV = vecVtx->begin();
				vector<Vector3>::iterator itN = vecNor->begin();
				*(itV + ii) = *(itV + i);
				*(itN + ii) = *(itN + i);
				mapOldNode.insert(map<int, int>::value_type(i, ii));
				nn = ii;
				ii++;
			}
			else {
				nn = mapOldNew[i]; 
				if (mapOldNode.find(nn) == mapOldNode.end()) {
					return -1;
				}
				nn = mapOldNode[nn];
				mapOldNode.insert(map<int, int>::value_type(i, nn));
			}
		}

		// reduction
		vecVtx->resize(ii);
		vecNor->resize(ii);

		// change index
		for (size_t i = 0; i < vecTri->size(); i++) 
		{
			vector<M3D_INDEX_TYPE>::iterator it = vecTri->begin();
			int n1 = *(it + i);
			n1 = mapOldNode[n1];
			*(it + i) = n1;
		}
		
		if (bodyPolyLine)
		{
			vector<Vector3>& edgePoints = bodyPolyLine->GetPoints();
			vector<M3D_INDEX_TYPE>& edgePntIndex = bodyPolyLine->GetPntIndex();
			if (edgePoints.size() == edgePntIndex.size())
			{
				for (size_t i = 0; i < edgePoints.size(); i++)
				{
					Vector3 edgePnt = edgePoints[i];
					M3D_INDEX_TYPE pntIndex = edgePntIndex[i];

					if (mapOldNode.find(pntIndex) != mapOldNode.end())
					{
						int value = mapOldNode[pntIndex];
						edgePntIndex[i] = value;
						edgePoints[i] = *(vecVtx->begin() + value);
					}
				}
			}
		}
		mapOldNode.clear();
		mapOldNew.clear();
		return 0;
	}

	double Reduction::squareDistance(const Vector3& v1, const Vector3& v2)
	{
		return (v1 - v2).LengthSquared();
	}
	bool Reduction::isParallel(const Vector3& vn1, const Vector3& vn2, const double angVal)
	{
		return (vn1.DotProduct(vn2) > angVal);
	}

	int Reduction::GetCurrentStep()
	{
		return m_Step;
	}

	bool Reduction::Simplification(Body* body, const float unitLength)
	{
		VertexSet* vertexSet = body->GetData();
		SPolyLine* bodyPolyLine = body->GetPolyLine();
		if (!vertexSet)
		{
			return false;
		}
		std::vector<Vector3>* vecVtx = vertexSet->GetPositionArray();
		std::vector<Vector3>* vecNor = vertexSet->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* vecTri = vertexSet->GetIndexArray();

		if (static_cast<int>(vecVtx->size()) < 3 || vecVtx->size() != vecNor->size()) {
			return false;
		}

		BoundingBox& box = body->GetBoundingBox();
		Vector3 minPnt = box.m_min;
		for (vector<Vector3>::iterator it = vecVtx->begin(); it != vecVtx->end(); it++)
		{
			Vector3 pnt = *it;
			int xindex = (int)((pnt.m_x - minPnt.m_x) / unitLength);
			int yindex = (int)((pnt.m_y - minPnt.m_y) / unitLength);
			int zindex = (int)((pnt.m_z - minPnt.m_z) / unitLength);
		}
		return true;
	}

	int Reduction::VertexReduction(Body* body, const float percent)
	{
		VertexSet* vertexSet = body->GetData();
		SPolyLine* bodyPolyLine = body->GetPolyLine();
		if (!vertexSet)
		{
			return -2;
		}
		std::vector<Vector3>* vecPnt = vertexSet->GetPositionArray();
		std::vector<Vector3>* vecNor = vertexSet->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* vecTri = vertexSet->GetIndexArray();

		vector<Face*>& faces = body->GetFaces();
		if (faces.size() <= 0)
		{
			return -2;
		}

		std::vector<Vertex> vecVtx;
		for (vector<Vector3>::iterator it = vecPnt->begin(); it != vecPnt->end(); it++)
		{
			Vector3 pnt = *it;
			Vertex vertex;
			vertex.x = (double)pnt.m_x;
			vertex.y = (double)pnt.m_y;
			vertex.z = (double)pnt.m_z;
			vecVtx.push_back(vertex);
		}

		VecOfVectorOfTri vecPatchTri;
		
		for (size_t i = 0; i < faces.size(); i++)
		{
			Face* face = faces[i];
			if (face)
			{

				Mesh* mesh = face->GetData();
				unsigned int offset = mesh->GetDataOffset();
				unsigned int length = mesh->GetDataLength();
				if (length % 3 != 0)
				{
					continue;
				}
				VectorOfTri vec;
				for (size_t j = 0; j < length; j = j+3)
				{
					Tri TR;
					TR.n1 = *(vecTri->begin() + offset + j);
					TR.n2 = *(vecTri->begin() + offset + j+1);
					TR.n3 = *(vecTri->begin() + offset + j+2);

					vec.push_back(TR);
				}
				vecPatchTri.push_back(vec);
			}
		}

		std::vector<double> RATIOS;
		RATIOS.push_back(percent / 100);

		std::vector<VecOfVectorOfTri> LODvecPatchTriNew;
		Perform(vecVtx, vecPatchTri, RATIOS, LODvecPatchTriNew);
		if (LODvecPatchTriNew.size() > 0)
		{
			vecTri->clear();
			VecOfVectorOfTri& vecRetTri = LODvecPatchTriNew[0];
			for (size_t i = 0; i < vecRetTri.size(); i++)
			{
				if (i >= faces.size())
				{
					continue;
				}
				Face* face = faces[i];
				if (face)
				{
					Mesh* mesh = face->GetData();
					mesh->SetDataOffset(vecTri->size());
					VectorOfTri retTri = vecRetTri[i];
					for (size_t j = 0; j < retTri.size(); j++)
					{
						Tri tri = retTri[j];
						vecTri->push_back(tri.n1);
						vecTri->push_back(tri.n2);
						vecTri->push_back(tri.n3);
					}
					mesh->SetDataLength(retTri.size() * 3);
				}	
			}
		}

		//该简化算法不能处理对边界线的点转换
		//if (bodyPolyLine)
		//{
		//	vector<Vector3>& edgePoints = bodyPolyLine->GetPoints();
		//	vector<M3D_INDEX_TYPE>& edgePntIndex = bodyPolyLine->GetPntIndex();
		//	if (edgePoints.size() == edgePntIndex.size())
		//	{
		//		for (size_t i = 0; i < edgePoints.size(); i++)
		//		{
		//			Vector3 edgePnt = edgePoints[i];
		//			M3D_INDEX_TYPE pntIndex = edgePntIndex[i];

		//			for (map<int, VectorOfVertex>::iterator it = mapNewOld.begin(); it != mapNewOld.end(); it++)
		//			{
		//				bool bFind = false;
		//				VectorOfVertex vecVertex = it->second;
		//				for (size_t j = 0; j < vecVertex.size(); j++)
		//				{
		//					if (vecVertex[j] == pntIndex)
		//					{
		//						bFind = true;
		//						break;
		//					}
		//				}
		//				
		//				if (bFind)
		//				{
		//					edgePntIndex[i] = it->first;
		//					edgePoints[i] = *(vecPnt->begin() + edgePntIndex[i]);
		//				}
		//			}
		//		}
		//	}
		//}
		return 0;
	}

	
	int Reduction::Perform(std::vector<Vertex>& vecVtx, VecOfVectorOfTri& vecPatchTri, std::vector<double>& ratios,
		std::vector<VecOfVectorOfTri>& LODvecPatchTriNew)
	{
		if (ratios.size() < 1) {
			return -1;
		}

		// ==================================
		//     set initial mesh 
		// ==================================

		Vertex v;
		GFace f;
		int local_num_vertices = -1; /* vertex index starts from 1 -> changed to 0*/
		double x0, y0, z0;        /* ax + by + cz = 0 */
		double x1, y1, z1;
		double x2, y2, z2;
		double a, b, c, M;

		faces.clear();
		vertices.clear();
		quadrics.clear();

		Vertices verticesIni;
		Faces facesIni;

		int ii = 0;
		for (int i = 0; i < vecVtx.size(); i++) {
			local_num_vertices++;							/* vertex index starts from 1 */
			v = vecVtx[i];
			verticesIni.insert(Vertices::value_type(local_num_vertices, v));
		}

		std::map<int, int> mapFacePatch;
		std::map<int, int> mapNodePatch;

		int jj = 0;
		for (int i = 0; i < vecPatchTri.size(); i++) {
			VectorOfTri& vecTri = vecPatchTri[i];

			for (int j = 0; j < vecTri.size(); j++) {
				f.id_vertex[0] = vecTri[j].n1; //  Vertex starts from 0
				f.id_vertex[1] = vecTri[j].n2; //
				f.id_vertex[2] = vecTri[j].n3; // 
				facesIni.insert(Faces::value_type(jj, f));  /* faces index starts from 1 */
				mapFacePatch.insert(std::map<int, int>::value_type(jj, i));

				mapNodePatch.insert(std::map<int, int>::value_type(f.id_vertex[0], i));
				mapNodePatch.insert(std::map<int, int>::value_type(f.id_vertex[1], i));
				mapNodePatch.insert(std::map<int, int>::value_type(f.id_vertex[2], i));

				jj++;
			}
		}

		// open solid processing
		std::map<int, int> mapOldNew;

		fuseNodes(verticesIni, facesIni, mapOldNew);
		if (mapOldNew.size() != verticesIni.size()) { // no fusion
			for (int i = 0; i < static_cast<int>(verticesIni.size()); i++) {
				mapOldNew.insert(std::map<int, int>::value_type(i, i));
			}
		}

		// init
		for (int i = 0; i < static_cast<int>(verticesIni.size()); i++) {
			if (mapOldNew[i] >= 0) {
				int local_num_vertices = mapOldNew[i];							/* vertex index starts from 1 -> changed to 0*/
				vertices.insert(Vertices::value_type(local_num_vertices, verticesIni[i]));
			}
		}

		for (int i = 0; i < static_cast<int>(facesIni.size()); i++) {
			int nv1 = facesIni[i].id_vertex[0];
			int nv2 = facesIni[i].id_vertex[1];
			int nv3 = facesIni[i].id_vertex[2];

			nv1 = mapOldNew[nv1];
			nv2 = mapOldNew[nv2];
			nv3 = mapOldNew[nv3];
			if (nv1 < 0) nv1 = -nv1 - 1;
			if (nv2 < 0) nv2 = -nv2 - 1;
			if (nv3 < 0) nv3 = -nv3 - 1;

			f.id_vertex[0] = nv1;
			f.id_vertex[1] = nv2;
			f.id_vertex[2] = nv3;

			x0 = vertices[f.id_vertex[0]].x;
			y0 = vertices[f.id_vertex[0]].y;
			z0 = vertices[f.id_vertex[0]].z;
			x1 = vertices[f.id_vertex[1]].x;
			y1 = vertices[f.id_vertex[1]].y;
			z1 = vertices[f.id_vertex[1]].z;
			x2 = vertices[f.id_vertex[2]].x;
			y2 = vertices[f.id_vertex[2]].y;
			z2 = vertices[f.id_vertex[2]].z;
			a = (y1 - y0)*(z2 - z0) - (z1 - z0)*(y2 - y0);   /* a1*b2 - a2*b1;        */
			b = (z1 - z0)*(x2 - x0) - (x1 - x0)*(z2 - z0);   /* a2*b0 - a0*b2;        */
			c = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);   /* a0*b1 - a1*b0;        */
			M = sqrt(a*a + b*b + c*c);
			a = a / M;
			b = b / M;
			c = c / M;
			f.plane[0] = a;
			f.plane[1] = b;
			f.plane[2] = c;
			f.plane[3] = -1 * (a*x0 + b*y0 + c*z0);				/* -1*(a*x + b*y + c*z); */
			faces.insert(Faces::value_type(i, f));
		}

		// ==================================
		//     perform 
		// ==================================

		initial_quadrics();

		std::vector<int> targetFacesNb;
		for (int i = 0; i < ratios.size(); i++) {
			int face_target = get_face_num() * ratios[i];
			targetFacesNb.push_back(face_target);
		}

		std::vector<Faces> LODFaces;
		construct_n_contract(targetFacesNb, LODFaces);

		// ==================================
		//     get results 
		// ==================================

		std::map<int, VectorOfVertex> mapNewOld;
		for (std::map<int, int>::iterator it = mapOldNew.begin(); it != mapOldNew.end(); it++) {
			int nOld = it->first;
			int nNew = it->second;
			if (nNew < 0) nNew = -nNew - 1; // to avoid 0

			if (mapNewOld.find(nNew) != mapNewOld.end()) {
				VectorOfVertex& vec = mapNewOld[nNew];
				vec.push_back(nOld);
			}
			else {
				VectorOfVertex vec;
				vec.push_back(nOld);
				mapNewOld.insert(std::map<int, VectorOfVertex>::value_type(nNew, vec));
			}
		}

		for (int NLOD = 0; NLOD < LODFaces.size(); NLOD++) {
			Faces& oneLODFaces = LODFaces[NLOD];

			VecOfVectorOfTri vecPatchTriNew;

			for (int i = 0; i < vecPatchTri.size(); i++) { // same as old
				VectorOfTri vec;
				vecPatchTriNew.push_back(vec);
			}

			for (Faces::iterator it = oneLODFaces.begin(); it != oneLODFaces.end(); it++) {
				int kf = it->first;
				int patch = mapFacePatch[kf];

				GFace f = it->second;

				int kn[3];
				bool bFind = true;
				for (int k = 0; k < 3; k++) {
					int n = f.id_vertex[k];
					if (mapNewOld.find(n) == mapNewOld.end())
					{
						bFind = false;
						break;
					}
					VectorOfVertex& vec = mapNewOld[n];

					// set a node on the GFace
					bool found = false;
					for (int kk = 0; kk < vec.size(); kk++) {
						int nold = vec[kk];
						if (mapNodePatch[nold] == patch) {
							kn[k] = vec[kk];
							found = true;
							break;
						}
					}
					if (!found) {
						kn[k] = vec[0];
					}
				}
				if (!bFind)
				{
					continue;
				}

				Tri TR;
				TR.n1 = kn[0];
				TR.n2 = kn[1];
				TR.n3 = kn[2];

				VectorOfTri& vec = vecPatchTriNew[patch];
				vec.push_back(TR);
			}
			LODvecPatchTriNew.push_back(vecPatchTriNew);
		}
		return 0;
	}

	void Reduction::fuseNodes(Vertices& vertices, Faces& faces, std::map<int, int>& mapOldNewGlobal)
	{
		// find free boudary nodes
		std::set<GEdge> freeEdges;
		for (int i = 0; i < faces.size(); i++) {
			int n1 = faces[i].id_vertex[0];
			int n2 = faces[i].id_vertex[1];
			int n3 = faces[i].id_vertex[2];

			GEdge p;
			std::set<GEdge>::iterator it;

			if (n1 < n2) {
				p = GEdge(n1, n2);
			}
			else {
				p = GEdge(n2, n1);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}

			if (n2 < n3) {
				p = GEdge(n2, n3);
			}
			else {
				p = GEdge(n3, n2);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}

			if (n3 < n1) {
				p = GEdge(n3, n1);
			}
			else {
				p = GEdge(n1, n3);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}
		}

		std::set<int> freeNodes;
		for (std::set<GEdge>::iterator it = freeEdges.begin(); it != freeEdges.end(); ++it) {
			//GEdge& p = *it;
			freeNodes.insert((*it).first);
			freeNodes.insert((*it).second);
		}

		if (freeNodes.size() < 2) {
			return;
		}

		if (0) {
			// old method
			std::vector<Vertex> vecN;
			for (std::set<int>::iterator it = freeNodes.begin(); it != freeNodes.end(); ++it) {
				int n = *it;
				Vertex v = vertices[n];
				vecN.push_back(v);
			}

			std::map<int, int> mapOldNew;
			for (int i = 0; i < vecN.size() - 1; i++) {
				if (mapOldNew.find(i) != mapOldNew.end()) continue;

				Vertex vi = vertices[i];
				for (int j = i; j < vecN.size(); j++) {
					if (mapOldNew.find(j) != mapOldNew.end()) continue;

					Vertex vj = vertices[j];
					if (distance(vi, vj) < 1.e-05) {
						mapOldNew.insert(std::map<int, int>::value_type(j, i));;
					}
				}
			}
		}

		// repartion of points

		double dmin[3] = { 1.e10 };
		double dmax[3] = { -1.e10 };

		for (std::set<int>::iterator it = freeNodes.begin(); it != freeNodes.end(); ++it) {
			int n = *it;
			Vertex v = vertices[n];
			dmin[0] = std::min(dmin[0], v.x);
			dmax[0] = std::max(dmax[0], v.x);
			dmin[1] = std::min(dmin[1], v.y);
			dmax[1] = std::max(dmax[1], v.y);
			dmin[2] = std::min(dmin[2], v.z);
			dmax[2] = std::max(dmax[2], v.z);
		}

		double dx = dmax[0] - dmin[0];
		double dy = dmax[1] - dmin[1];
		double dz = dmax[2] - dmin[2];

		int N1, N2;
		if (dx <= dy && dx <= dz) {
			N1 = 1;
			N2 = 2;
		}
		else if (dy <= dz && dy <= dx) {
			N1 = 2;
			N2 = 0;
		}
		else if (dz <= dx && dz <= dy) {
			N1 = 0;
			N2 = 1;
		}

		// repartion of points
		int NN = sqrt(double(freeNodes.size())) / 5;
		int NN1 = NN;
		int NN2 = NN;

		double D1 = (dmax[N1] - dmin[N1]) / NN1;
		if (D1 < 1.e-05) {
			D1 = 1.;
		}
		double D2 = (dmax[N2] - dmin[N2]) / NN2;
		if (D2 < 1.e-05) {
			D2 = 1.;
		}

		BiVecOfVectorOfVertex BiVecVertices;
		for (int i = 0; i <= NN1; i++) {
			VecOfVectorOfVertex vecvecV;
			for (int j = 0; j <= NN2; j++) {
				VectorOfVertex vecV;
				vecvecV.push_back(vecV);
			}
			BiVecVertices.push_back(vecvecV);
		}

		double xyz[3];
		for (std::set<int>::iterator it = freeNodes.begin(); it != freeNodes.end(); ++it) {
			int n = *it;
			Vertex v = vertices[n];

			xyz[0] = v.x;
			xyz[1] = v.y;
			xyz[2] = v.z;

			int i1 = (xyz[N1] - dmin[N1]) / D1;
			int i2 = (xyz[N2] - dmin[N2]) / D2;

			VectorOfVertex& VecV = BiVecVertices[i1][i2];
			VecV.push_back(n);
		}

		std::map<int, int> mapOldNew;
		for (int i1 = 0; i1 <= NN1; i1++) {
			for (int i2 = 0; i2 <= NN2; i2++) {
				VectorOfVertex& VecV = BiVecVertices[i1][i2];

				int LL = VecV.size();

				for (int i = 0; i < LL - 1; i++) {
					int n1 = VecV[i];
					if (mapOldNew.find(n1) == mapOldNew.end()) {
						Vertex vi = vertices[n1];

						for (int j = i + 1; j < LL; j++) {
							int n2 = VecV[j];
							if (mapOldNew.find(n2) == mapOldNew.end()) {
								Vertex vj = vertices[n2];
								if (distance(vi, vj) < 1.e-05) {
									mapOldNew.insert(std::map<int, int>::value_type(n2, n1));
								}
							}
						}
					}
				}
			}
		}

		// mapOldNew respects the order

		int ii = -1;
		for (int i = 0; i < vertices.size(); i++) {

			if (mapOldNew.find(i) == mapOldNew.end()) {
				ii++; // staring from 0
				mapOldNewGlobal.insert(std::map<int, int>::value_type(i, ii));
			}
			else {
				int n = mapOldNew[i];
				if (mapOldNewGlobal.find(n) == mapOldNewGlobal.end()) {
					printf(" ERROR \n");
#ifdef _WIN32
					system("PAUSE");
#endif
                    exit(-2);
				}
				mapOldNewGlobal.insert(std::map<int, int>::value_type(i, -mapOldNewGlobal[n] - 1)); // to avoid "0"
			}
		}

		//printf(" Removed Nodes = %d\n", mapOldNew.size());

		return;
	}

	double Reduction::distance(Vertex v1, Vertex v2)
	{
		return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
	}

	int Reduction::get_face_num() 
	{ 
		return faces.size(); 
	}

	void Reduction::initial_quadrics()
	{
		for (int i = 1; i <= static_cast<int>(vertices.size()); i++)
		{
			quadrics.insert(Matrices::value_type(i, Matrix(0.0)));
		}

		/* compute initial quadric */
		for (int i = 0; i < static_cast<int>(faces.size()); i++)
		{
			/* faces are triangles */
			for (int j = 0; j < 3; j++)
			{
				quadrics[faces[i].id_vertex[j]] += Matrix(faces[i].plane);
			}
		}
	}

	void Reduction::construct_n_contract(std::vector<int>& targetFacesNb, std::vector<Faces>& LODFaces)
	{
		// find free boudary nodes
		GEdge p;

		std::set<GEdge> freeEdges;
		for (int i = 0; i < static_cast<int>(faces.size()); i++) {
			int n1 = faces[i].id_vertex[0];
			int n2 = faces[i].id_vertex[1];
			int n3 = faces[i].id_vertex[2];

			std::set<GEdge>::iterator it;

			if (n1 < n2) {
				p = GEdge(n1, n2);
			}
			else {
				p = GEdge(n2, n1);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}

			if (n2 < n3) {
				p = GEdge(n2, n3);
			}
			else {
				p = GEdge(n3, n2);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}

			if (n3 < n1) {
				p = GEdge(n3, n1);
			}
			else {
				p = GEdge(n1, n3);
			}
			it = freeEdges.find(p);
			if (it != freeEdges.end()) {
				freeEdges.erase(it);
			}
			else {
				freeEdges.insert(p);
			}
		}

		std::set<int> freeNodes;
		for (std::set<GEdge>::iterator it = freeEdges.begin(); it != freeEdges.end(); ++it) {
			//GEdge p = *it;
			freeNodes.insert((*it).first);
			freeNodes.insert((*it).second);
		}

		// construct Vertex -> Faces
		std::map<int, VectorOfFace> mapVertexFaces;
		for (Faces::iterator it = faces.begin(); it != faces.end(); it++) {
			int kf = it->first;
			GFace f = it->second;
			for (int j = 0; j < 3; j++) {
				int nn = f.id_vertex[j];
				if (mapVertexFaces.find(nn) != mapVertexFaces.end()) {
					VectorOfFace& vec = mapVertexFaces[nn];
					vec.push_back(kf);
				}
				else {
					VectorOfFace vec;
					vec.push_back(kf);
					mapVertexFaces.insert(std::map<int, VectorOfFace>::value_type(nn, vec));
				}
			}
		}

		// =========================================================================

		/* calculate initial error for each valid GEdge*/
		select_edge();

		int id_v1, id_v2;
		double vx, vy, vz;

		int target_num_faces = targetFacesNb[targetFacesNb.size() - 1]; // the last value
																		/* contract vertices*/

		int nbDiffEdge = faces.size() - targetFacesNb[0];
		int nbTimes = 20;
		int LIMITNBEDGE = nbDiffEdge / nbTimes;
		if (LIMITNBEDGE < 5) {
			LIMITNBEDGE = 5;
		}

		VectorOfEdgeError vecErrors;
		int nbEdge = LIMITNBEDGE; // starting
		int numVecErr = 0;

		int sortTimes = 0;

		Errors::iterator iter_min_error;

		while (faces.size() > target_num_faces)
		{
			// store the LOD results
			if (faces.size() <= targetFacesNb[LODFaces.size()]) {
				// copy the result
				Faces oneLODFaces = faces;

				printf("NLOD = %d   NBFaces = %d    NBTarget = %d   SortTimes = %d\n", LODFaces.size(), oneLODFaces.size(), targetFacesNb[LODFaces.size()], sortTimes);

				LODFaces.push_back(oneLODFaces);

				// ------------------
				//   initialisation
				// ------------------
				if (LODFaces.size() >= targetFacesNb.size()) {
					// impossible
					break; // while
				}
				nbDiffEdge = faces.size() - targetFacesNb[LODFaces.size()];
				LIMITNBEDGE = nbDiffEdge / nbTimes;
				if (LIMITNBEDGE < 5) {
					LIMITNBEDGE = 5;
				}

				nbEdge = LIMITNBEDGE; // starting
				numVecErr = 0;
				sortTimes = 0;
			}

			if (nbEdge == LIMITNBEDGE) {
				vecErrors.clear();
				sortEdgeErrors(errors, vecErrors);
				nbEdge = 0;
				numVecErr = 0;
				sortTimes++;
			}

			/* find least-error GEdge */
			bool foundMinEdge = false;

			while (nbEdge < LIMITNBEDGE && numVecErr < vecErrors.size()) {
				GEdge edgeErr = vecErrors[numVecErr].first;
				numVecErr++;

				iter_min_error = errors.find(edgeErr);
				if (iter_min_error == errors.end()) {
					continue;
				}
				id_v1 = edgeErr.first;
				id_v2 = edgeErr.second;

				// avoid FreeNodes
				if (freeNodes.find(id_v1) != freeNodes.end() || freeNodes.find(id_v2) != freeNodes.end()) {
					continue;
				}
				nbEdge++;
				foundMinEdge = true;
				break; // inner while
			}

			if (foundMinEdge == false) {
				break; // break while
			}


			/* add to vsplits; */
			int id_NN;
			calculate_error(id_v1, id_v2, &vx, &vy, &vz, &id_NN);	/* get coordinate of vf */

																	/* update coordinate of v1*/ // canceled: can't be changed
																								 //vertices[id_NN].x = vx;
																								 //vertices[id_NN].y = vy;
																								 //vertices[id_NN].z = vz;

																								 /* update quadric of v1*/
			quadrics[id_NN] = quadrics[id_v1] + quadrics[id_v2];

			// before update, construct two lists

			int id_N1 = id_v1;
			int id_N2 = id_v2;
			if (id_NN == id_v2) {
				id_N1 = id_v2;
				id_N2 = id_v1;
			}

			VectorOfFace& vecfaces1 = mapVertexFaces[id_N1];
			VectorOfFace& vecfaces2 = mapVertexFaces[id_N2];
			// =============== Important

			std::set<int> setOfVertexTotal;
			for (int k = 0; k < vecfaces1.size(); k++) {
				int kf = vecfaces1[k];
				for (int kk = 0; kk < 3; kk++) {

					int nn = faces[kf].id_vertex[kk];
					if (nn != id_v1 && nn != id_v2) {
						setOfVertexTotal.insert(nn);
					}
				}
			}

			std::set<int> setOfVertex2;
			for (int k = 0; k < vecfaces2.size(); k++) {
				int kf = vecfaces2[k];
				for (int kk = 0; kk < 3; kk++) {

					int nn = faces[kf].id_vertex[kk];
					if (nn != id_v1 && nn != id_v2) {
						setOfVertex2.insert(nn);
						setOfVertexTotal.insert(nn);
					}
				}
			}

			/* replace v2 with v1 in faces */
			/* remove faces that has an GEdge of (v1, v2) */

			for (int k = 0; k < vecfaces2.size(); k++) {
				int kf = vecfaces2[k];

				if (faces[kf].id_vertex[0] == id_N1 || faces[kf].id_vertex[1] == id_N1 || faces[kf].id_vertex[2] == id_N1) {
					for (std::vector<int>::iterator it = vecfaces1.begin(); it != vecfaces1.end(); it++) {
						if (*it == kf) {
							vecfaces1.erase(it);
							break;
						}
					}

					int NN3 = faces[kf].id_vertex[0];
					if (NN3 == id_v1 || NN3 == id_v2) {
						NN3 = faces[kf].id_vertex[1];
						if (NN3 == id_v1 || NN3 == id_v2) {
							NN3 = faces[kf].id_vertex[2];
						}
					}

					VectorOfFace& vecfaces3 = mapVertexFaces[NN3];
					for (std::vector<int>::iterator it = vecfaces3.begin(); it != vecfaces3.end(); it++) {
						if (*it == kf) {
							vecfaces3.erase(it);
							break;
						}
					}

					faces.erase(kf);
				}
				else {
					for (int j = 0; j < 3; j++) {

						if (faces[kf].id_vertex[j] == id_N2) {
							faces[kf].id_vertex[j] = id_N1;

							vecfaces1.push_back(kf);

							break;
						}
					}
				}
			}

			// update mapVertexFaces
			mapVertexFaces.erase(id_N2);

			/* remove v2 in vertices */
			vertices.erase(id_N2);

			// find Edges connected to 

			/* merge Edges of v2 to v1 */
			GEdge p;
			for (std::set<int>::iterator it = setOfVertex2.begin(); it != setOfVertex2.end(); it++) {
				int N2 = *it;
				if (id_N2 < N2) {
					p = GEdge(id_N2, N2);
				}
				else {
					p = GEdge(N2, id_N2);
				}

				//erase
				errors.erase(p);

				//insert
				//duplicate is not possible in map
				std::pair<Errors::iterator, bool> pr = errors.insert(Errors::value_type(GEdge(std::min(id_N1, N2), std::max(id_N1, N2)), 0.0));
				//#ifdef _DEBUG
				//			if (pr.second == true) {
				//				printf("QUADRIC: insert  Edge(%d, %d)\n", std::min(id_v1, N2), std::max(id_v1, N2));
				//			}
				//			else {
				//				printf("QUADRIC: insert  Edge(%d, %d) FAIL..duplicate\n", std::min(id_v1, N2), std::max(id_v1, N2));
				//			}
				//#endif
			}

			/* remove Edge (v1, v2) */
			errors.erase(iter_min_error);

			/* update error of Edges involving v1 */
			for (std::set<int>::iterator it = setOfVertexTotal.begin(); it != setOfVertexTotal.end(); it++) {
				int N2 = *it;
				if (id_N1 < N2) {
					p = GEdge(id_N1, N2);
				}
				else {
					p = GEdge(N2, id_N1);
				}

				double err = calculate_error(id_N1, N2);
				errors[p] = err;
			}
		}

		// store the LOD results for the last one
		if (faces.size() > target_num_faces) {
			printf(" Gaol not reached\n");
		}

		if (LODFaces.size() < targetFacesNb.size()) {
			// fill LOD
			for (int i = LODFaces.size(); i < targetFacesNb.size(); i++) {
				Faces oneLODFaces = faces;

				printf("the last one NBFaces = %d    NBTarget = %d     SortTimes = %d\n", oneLODFaces.size(), target_num_faces, sortTimes);

				LODFaces.push_back(oneLODFaces);
			}
		}
	}

	void Reduction::select_edge()
	{
		int max_vid;
		int min_vid;

		/* (v1, v2) is an edge */
		/* id_v1 < id_v2*/
		for (int i = 0; i < static_cast<int>(faces.size()); i++)
		{
			int n1 = faces[i].id_vertex[0];
			int n2 = faces[i].id_vertex[1];
			int n3 = faces[i].id_vertex[2];

			for (int k = 0; k < 3; k++) {
				if (k == 0) {
					min_vid = std::min(n1, n2);
					max_vid = std::max(n1, n2);
				}
				else if (k == 1) {
					min_vid = std::min(n1, n3);
					max_vid = std::max(n1, n3);
				}
				else if (k == 2) {
					min_vid = std::min(n2, n3);
					max_vid = std::max(n2, n3);
				}

				GEdge p = GEdge(min_vid, max_vid);

				if (errors.find(p) == errors.end()) {
					double edgeErr = calculate_error(min_vid, max_vid);
					errors.insert(Errors::value_type(p, edgeErr));
				}
			}
		}
	}

	double Reduction::calculate_error(int id_v1, int id_v2, double* vx, double* vy, double* vz, int* id_NN)
	{
		double min_error;
		Matrix q_bar;
		Matrix q_delta;
		bool isReturnVertex = true;
		if (vx == NULL) { vx = new double; isReturnVertex = false; }
		if (vy == NULL) { vy = new double; }
		if (vz == NULL) { vz = new double; }
		if (id_NN == NULL) { id_NN = new int; }

		/* computer quadric of virtual vertex vf */
		q_bar = quadrics[id_v1] + quadrics[id_v2];

		// select vertex from v1, v2 
		double vx1 = vertices[id_v1].x;
		double vy1 = vertices[id_v1].y;
		double vz1 = vertices[id_v1].z;

		double vx2 = vertices[id_v2].x;
		double vy2 = vertices[id_v2].y;
		double vz2 = vertices[id_v2].z;

		double error1 = vertex_error(q_bar, vx1, vy1, vz1);
		double error2 = vertex_error(q_bar, vx2, vy2, vz2);

		min_error = std::min(error1, error2);
		if (error1 == min_error) {
			*vx = vx1; *vy = vy1, *vz = vz1;
			*id_NN = id_v1;
		}
		else {
			*vx = vx2; *vy = vy2, *vz = vz2;
			*id_NN = id_v2;
		}

		if (isReturnVertex == false) { delete vx; delete vy; delete vz; delete id_NN; }
		return min_error;
	}

	inline double Reduction::vertex_error(Matrix q, double x, double y, double z)
	{
		return q[0] * x*x + 2 * q[1] * x*y + 2 * q[2] * x*z + 2 * q[3] * x + q[5] * y*y
			+ 2 * q[6] * y*z + 2 * q[7] * y + q[10] * z*z + 2 * q[11] * z + q[15];
	}

	
}
