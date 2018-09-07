#include "Envelope.h"
#include "m3d/model/Body.h"
#include "m3d/model/MeshData.h"
#include "m3d/base/Ray.h"
#include "m3d/SceneManager.h"
#include "m3d/model/GeoAttribute.h"
#include "Merge.h"

namespace M3D {

	Envelope::Envelope()
	{
		m_Step = 0;
		for (size_t jj = 0; jj < 3; jj++)
		{
			for (size_t kk = 0; kk < 3; kk++)
			{
				for (size_t ll = 0; ll < 3; ll++)
				{
					m_StandardDirection.push_back(Vector3((float)jj - 1, (float)kk - 1, (float)ll - 1));
				}
			}
		}
	}
	Envelope::~Envelope()
	{

	}

	bool Envelope::CreateEnvelope(Model* model, bool rmvInner, bool rmvLittle, float rmvPercent, SceneManager* scene)
	{
		if (!model)
		{
			return false;
		}
		//m_ModelVec.resize(0);
		BoundingBox box = model->GetTotalBoundingBox();
		float value = box.Length() * rmvPercent * 0.01;
	
		Enveloping(model, model, rmvInner, rmvLittle, value, scene);
		return true;
	}
	void Envelope::Enveloping(Model* model, Model* mergeModel, bool rmvInner, bool rmvLittle, float rmvData, SceneManager* scene)
	{
		if (!model)
		{
			return;
		}

		BoundingBox box = model->GetTotalWorldBoundingBox();
		if (rmvLittle)
		{
			float length = box.Length();
			if (length < rmvData)
			{
				return;
			}
		}

		if (model->GetSubModelCount() > 0)
		{
			vector<Model*>& children = model->GetSubModels();
			for (size_t i = 0; i < model->GetSubModelCount(); i++)
			{
				Enveloping(children[i], mergeModel, rmvInner, rmvLittle, rmvData, scene);
			}
		}
		else
		{
			m_Step++;
			Vector3 boxCenter = box.Center();
			bool bOutSide = false;
			for (size_t i = 0; i < m_StandardDirection.size(); i++)
			{
				if (i == 13)
				{
					continue;
				}
				Vector3 standardPnt = m_StandardDirection[i];
				Ray ray(boxCenter, standardPnt);

				IShape* farShape = scene->GetFarPickShape(ray, ShapeType::SHAPE_MODEL, GeoAttrTypeEnum::M3D_GEOATTR_TYPE_UNKNOWN);
				if (!farShape || (farShape && farShape == model))
				{
					bOutSide = true;
					break;
				}
			}

			if (!bOutSide)
			{
				vector<Body*>* bodys = model->GetBodys();
				if (bodys)
				{
					for (vector<Body*>::iterator it = bodys->begin(); it != bodys->end(); it++)
					{
						Body* body = *it;
						if (body && body->GetData())
						{
							VertexSet* vertexSet = body->GetData();
							Vector3 *Pnts = vertexSet->GetPositionArray()->data();
							
							M3D_INDEX_TYPE* Indexs = vertexSet->GetIndexArray()->data();
							int count = vertexSet->GetIndexArray()->size();
							bOutSide = CheckOutSide(0, count - 1, Indexs, Pnts, boxCenter, model, scene);
							//std::vector<M3D_INDEX_TYPE>* Indexs = vertexSet->GetIndexArray();
							//for (std::vector<M3D_INDEX_TYPE>::iterator indexIte = Indexs->begin(); indexIte != Indexs->end();
							//	indexIte = indexIte + 6)
							//{
							//	Vector3 pnt = Pnts[*indexIte];
							//	Vector3 direction = pnt - boxCenter;
							//	Ray ray(boxCenter, direction);

							//	IShape* farShape = scene->GetFarPickShape(ray, ShapeType::SHAPE_MODEL, GeoAttrTypeEnum::M3D_GEOATTR_TYPE_UNKNOWN);
							//	if (farShape && farShape == model)
							//	{
							//		bOutSide = true;
							//		break;
							//	}
							//}
							if (bOutSide)
							{
								break;
							}
						}
					}
				}
			}

			if (bOutSide)
			{
				Merge::MergeModels(model, mergeModel, 0, 0, true);
			}
		}
	}

	bool Envelope::CheckOutSide(int startIndex, int endIndex, M3D_INDEX_TYPE* Indexs, Vector3 *Pnts, Vector3& boxCenter, 
		Model* model, SceneManager* scene)
	{
		bool ret = false;
		int x = (startIndex + endIndex) / 2;

		Vector3 pnt = Pnts[x];
		Vector3 direction = pnt - boxCenter;
		Ray ray(boxCenter, direction);

		IShape* farShape = scene->GetFarPickShape(ray, ShapeType::SHAPE_MODEL, GeoAttrTypeEnum::M3D_GEOATTR_TYPE_UNKNOWN);
		if (!farShape || (farShape && farShape == model))
		{
			ret = true;
			return ret;
		}

		if (x - startIndex >= 3)
		{
			ret = CheckOutSide(startIndex, x - 1, Indexs, Pnts, boxCenter, model, scene);
		}
		if (endIndex - x >= 3 && !ret)
		{
			ret = CheckOutSide(x + 1, endIndex, Indexs, Pnts, boxCenter, model, scene);
		}
		return ret;
	}
}
