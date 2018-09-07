#include "m3d/M3D.h"
#include "m3d/utils/IDCreator.h"

#include "sview/views/Parameters.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/utils/Trackball.h"

#include "m3d/model/ModelShape.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Body.h"

#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/action/RenderAction.h"
#include "m3d/scene/Octree.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/ResourceManager.h"
#include "m3d/SceneManager.h"
#include "Edge.h"

namespace M3D
{
	ModelShape::ModelShape(void)
	{
		this->m_bodys.resize(0);
		this->SetModel(NULL);

		this->SetOctant(NULL);
		this->SetOctree(NULL);
		SetOCTreeUpdated(false);
		this->m_drawDataPrepared = true;
		this->MarkDirty();
	}

	ModelShape::~ModelShape(void)
	{
		for (int i = 0; i < m_bodys.size(); i++)
		{
			m_bodys[i]->Release();
		}

		this->m_bodys.resize(0);

		if (this->m_octant)
		{
			this->m_octant->RemoveDrawable(this, false);
		}
	}

	ModelShape::ModelShape(const ModelShape& orig)
	{
		*this = orig;
	}

	bool ModelShape::CopyFrom(const ModelShape& orig)
	{
		this->m_bodys.resize(0);
		//this->SetModel(NULL);
		//this->SetOctant(NULL);
		//this->SetOctree(NULL);
		SetOCTreeUpdated(false);
		//this->MarkDirty();

		//拷贝面数据
		for (int i = 0; i < orig.m_bodys.size(); i++)
		{
			Body* body = new Body(*(Body*)orig.m_bodys[i]);
			this->AddBody(body);
		}

		return true;
	}

	ModelShape& ModelShape::operator =(const ModelShape& orig)
	{
		if (this != &orig)
		{
			this->m_bodys.resize(0);
			this->SetModel(NULL);

			this->SetOctant(NULL);
			this->SetOctree(NULL);
			SetOCTreeUpdated(false);
			this->MarkDirty();

			//拷贝面数据
			for (int i = 0; i < orig.m_bodys.size(); i++)
			{
				Body* body = new Body(*(Body*)orig.m_bodys[i]);
				this->AddBody(body);
			}
		}
		return *this;
	}

	void ModelShape::SetModel(Model* model)
	{
		this->m_parentModel = model;
	}

	Model* ModelShape::GetModel()
	{
		return this->m_parentModel;
	}

	SHAPETYPE ModelShape::GetType(void)
	{
		return MODEL_SHAPE;
	}

	void ModelShape::AddBody(Body *body)
	{
		if (body && m_parentModel)
		{
			body->AddRef();
			body->SetModel(m_parentModel);
			m_bodys.push_back(body);
		}
	}

	void ModelShape::SetAlpha(float a)
	{
		if (this->m_drawDataPrepared)
		{
			for (int r = 0; r < this->m_bodys.size(); r++)
			{
				this->m_bodys[r]->SetAlpha(a);
			}
		}
	}

	float ModelShape::GetAlpha()
	{
		if (this->m_drawDataPrepared)
		{
			for (int r = 0; r < this->m_bodys.size(); r++)
			{
				return this->m_bodys[r]->GetAlpha();
			}
		}
		return 1.0f;
	}

	void ModelShape::SetSelected(bool select)
	{
		if (this->m_drawDataPrepared)
		{
			for (int r = 0; r < this->m_bodys.size(); r++)
			{
				Body* tmpBody = this->m_bodys[r];
				if (NULL != tmpBody)
				{
					tmpBody->SetSelected(select);
				}
			}
		}
	}

	bool ModelShape::IsVisible()
	{
		if (this->m_parentModel)
		{
			return this->m_parentModel->IsVisible();
		}
		return true;
	}

	void ModelShape::SetVisible(bool visible)
	{
		if (this->m_drawDataPrepared)
		{
			for (int r = 0; r < this->m_bodys.size(); r++)
			{
				this->m_bodys[r]->SetVisible(visible);
			}
		}
	}

	int ModelShape::GetVertexCount(int level)
	{
		if (this->m_drawDataPrepared)
		{
			int count = 0;
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				count += this->m_bodys[i]->GetVertexCount(level);
			}
			return count;
		}
		return 0;
	}

	void ModelShape::ComputeBox()
	{
		if (!this->IsDrawDataPrepared())
		{
			return;
		}

		m_BoundingBox.Clear();

		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				vector<Face*>& faces = m_bodys[i]->GetFaces();
				for (int j = 0; j < faces.size(); j++)
				{
					m_BoundingBox.Merge(faces.at(j)->GetBoundingBox());
				}

				vector<Edge*>* edges = m_bodys[i]->GetEdges();
				if (edges)
				{
					for (int j = 0; j < edges->size();j++)
					{
						m_BoundingBox.Merge(edges->at(j)->GetBoundingBox());
					}
				}
				
			}
		}
	}

	void ModelShape::SetInitAlpha(float alpha)
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->SetInitAlpha(alpha);
			}
		}
	}

	void ModelShape::SetInitHightlight(bool isHighlight)
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->SetInitHightlight(isHighlight);
			}
		}
	}

	bool ModelShape::IsHightlight()
	{
		return false;
	}

	void ModelShape::SetInitColor(const Color &color)
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->SetInitColor(color);
			}
		}
	}

	void ModelShape::ResetColor()
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->ResetColor();
			}
		}
	}

	float ModelShape::GetVolumeAndArea(float& volume, float& area)
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < m_bodys.size(); i++)
			{
				float subVolum = 0;
				float subArea = 0;
				m_bodys.at(i)->GetVolumeAndArea(subVolum, subArea);

				volume += subVolum;
				area += subArea;
			}
		}
		return 0;
	}

	void  ModelShape::Restore()
	{
		if (this->m_drawDataPrepared)
		{
			this->MarkDirty();
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->Restore();
			}
		}
	}

	void ModelShape::ResetAlpha()
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->ResetAlpha();
			}
		}
	}

	void ModelShape::SetColor(const Color &color)
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->SetColor(color);
			}
		}
	}

	Color* ModelShape::GetColor()
	{
		if (this->m_drawDataPrepared)
		{
			if (this->m_bodys.size() > 0)
			{
				return m_bodys[0]->GetColor();
			}
		}
		return NULL;
	}

	vector<Body*>* ModelShape::GetBodys()
	{
		return &m_bodys;
	}

	void ModelShape::SetBodys(vector<Body*>& bodys)
	{
		m_bodys = bodys;
	}

	void ModelShape::MarkDirty()
	{
		this->m_dirty = true;
		m_worldBox.Clear();
	}

	bool ModelShape::IsDirty() const
	{
		return m_dirty;
	}

	bool ModelShape::AllowCuller()
	{
		return true;
	}

	void ModelShape::SetOctant(Octant* octant)
	{
		m_octant = octant;
	}

	Octant* ModelShape::GetOctant() const
	{
		return m_octant;
	}

	void ModelShape::SetOctree(Octree* octree)
	{
		m_octree = octree;
	}

	Octree* ModelShape::GetOctree() const
	{
		return m_octree;
	}

	bool ModelShape::isOCtreeUpdated()
	{
		return this->m_updateQueued;
	}

	void ModelShape::SetOCTreeUpdated(bool updated)
	{
		this->m_updateQueued = updated;
	}

	bool ModelShape::IsDrawDataPrepared()
	{
		//暂时不加锁 x86应该是线程安全的
		return this->m_drawDataPrepared;
	}

	void ModelShape::SetDrawDataPrepared(bool dataPrepared)
	{
		if (dataPrepared)
		{
			this->MarkDirty();
		}

		this->m_drawDataPrepared = dataPrepared;
	}

	void ModelShape::SetWorldMatrix(Matrix3x4 & worldMatrix)
	{
		this->m_worldMatrix = this->m_parentModel->GetWorldTransform()*worldMatrix;
		this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();

		this->m_worldBox.Clear();

		UpdateOCTree();
	}

	void ModelShape::Translate(const Vector3& delta, TransformSpace space /*= TS_LOCAL*/)
	{
		if (delta.IsZero())
		{
			return;
		}

		Matrix3x4 origMatrix;
		(origMatrix).MultiTranslate(delta);

		this->m_worldMatrix = this->m_worldMatrix*origMatrix;
		this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();

		this->m_worldBox.Clear();

		UpdateOCTree();
	}
	void ModelShape::MarkDrawDataDirty()
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->MarkDrawDataDirty();
			}
		}
	}

	void ModelShape::Clear()
	{
		if (this->m_drawDataPrepared)
		{
			for (int i = 0; i < this->m_bodys.size(); i++)
			{
				m_bodys[i]->Release();
			}
			this->m_bodys.resize(0);
		}
	}

	Matrix3x4& ModelShape::GetWorldTransform()
	{
		if (this->IsDirty())
		{
			this->UpdateWorldTransform();
		}

		return this->m_worldMatrix;
	}

	BoundingBox& ModelShape::GetWorldBoundingBox()
	{
		if (!m_worldBox.Defined())
		{
			m_worldBox = GetBoundingBox();
			if (m_worldBox.Defined())
			{
				m_worldBox.Transform(this->GetWorldTransform());
			}
		}

		return m_worldBox;
	}

	BoundingBox& ModelShape::GetBoundingBox()
	{
		ComputeBox();
		return m_BoundingBox;
	}

	void  ModelShape::SetBoundingBox(BoundingBox& boundingBox)
	{
		this->m_BoundingBox = boundingBox;
	}

	Matrix4& ModelShape::GetGLWorldTransform()
	{
		this->GetWorldTransform();

		return this->m_glRenderMatrix;
	}

	void ModelShape::UpdateWorldTransform()
	{
		m_dirty = false;

		Matrix3x4 transform;
		if (this->m_parentModel == NULL)
		{
			m_worldMatrix = transform;
		}
		else
		{
			m_worldMatrix = this->m_parentModel->GetWorldTransform() * transform;
		}

		this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();

		UpdateOCTree();
	}

	void  ModelShape::UpdateOCTree()
	{
		//if (this->m_octant)
		{
			//this->m_octant->RemoveDrawable(this,true);
			SetOCTreeUpdated(false);
			//this->m_octant = NULL;
		}

		if (this->m_octree)
		{
			m_octree->QueueUpdate(this);
		}
	}

	void ModelShape::FindVisiableObjectFast(RenderAction* renderAction, int littleModelState)
	{
		if (this->m_drawDataPrepared)
		{
			//视景体剔除
			Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(), renderAction->GetCamera());
			if (cullerState != OUTSIDE)
			{
				renderAction->SetWorldMatrix(&GetWorldTransform());
				renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
			 if (littleModelState == 1)
			{
				renderAction->SetDelayDraw(true);
				for (int i = 0; i < m_bodys.size(); i++)
				{
					m_bodys[i]->FindVisiableObject(renderAction);
				}
				renderAction->SetDelayDraw(false);
			}
			 else	if (littleModelState == 0)
				{
					if (false)
					{
						for (int i = 0; i < m_bodys.size(); i++)
						{
							int littleModelState = renderAction->GetCullerHelper().IsLittleModel(m_bodys[i]->GetBoundingBox(), renderAction->GetCamera());
							if (littleModelState == 0)
							{
								m_bodys[i]->FindVisiableObject(renderAction);
							}
							else if (littleModelState == 1)
							{
								renderAction->SetDelayDraw(true);
								m_bodys[i]->FindVisiableObject(renderAction);
								renderAction->SetDelayDraw(false);
							}
						}
					}
					else
					{
						for (int i = 0; i < m_bodys.size(); i++)
						{
							m_bodys[i]->FindVisiableObject(renderAction);
						}
					}
				}
				
			}
		}
	}
	void ModelShape::FindVisiableObject(RenderAction* renderAction)
	{
		if (this->m_drawDataPrepared)
		{
			//微小模型剔除
			//int littleModelState = renderAction->GetCullerHelper().IsLittleModel(this->GetWorldBoundingBox(), renderAction->GetCamera());

			////非常小的模型
			//if (littleModelState == 2)
			//{
			//	return;
			//} //小件剔除的模型
			//else //其他
			{
				//视景体剔除
				Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(), renderAction->GetCamera());
				//if (cullerState != OUTSIDE)
				//{
					renderAction->SetWorldMatrix(&GetWorldTransform());
					renderAction->SetGLWorldMatrix(&GetGLWorldTransform());

					//if (littleModelState == 0)
					{
						for (int i = 0; i < m_bodys.size(); i++)
						{
							m_bodys[i]->FindVisiableObject(renderAction);
						}
					}
					/*else if (littleModelState == 1)
					{
						renderAction->SetDelayDraw(true);
						for (int i = 0; i < m_bodys.size(); i++)
						{
							m_bodys[i]->FindVisiableObject(renderAction);
						}
						renderAction->SetDelayDraw(false);
					}*/
				//}
			}
		}
	}

	void ModelShape::RayPick(RayPickAction * action)
	{
		if (this->m_drawDataPrepared)
		{
			if (this->GetModel() &&
				!action->CanPickShape(this->GetModel()->GetType()))
			{
				return;
			}

			if (m_bodys.size() > 0)
			{
				if (action->GetData()->GetCameraRay().HitDistance(this->GetWorldBoundingBox()) != M_INFINITY)
				{
					//通过shape对象显示时的ModelMatrix，更新Model射线
					action->UpdataModelRay(this->GetWorldTransform());
					for (int i = 0; i < m_bodys.size(); i++)
					{
						m_bodys[i]->RayPick(action);
					}
				}
			}
		}
	}

	void ModelShape::FramePick(RayPickAction* action)
	{
		if (this->m_drawDataPrepared)
		{
			if (action->FrustumIntersetWithWorldBox(GetWorldBoundingBox()))
			{
				action->AddToFramePickCollection(this->GetModel());
			}
		}
	}

	bool ModelShape::MergeFace()
	{
		if (this->m_drawDataPrepared)
		{
			vector<Body*> tmpBodys;

			for (vector<Body*>::iterator ite = m_bodys.begin(); ite != m_bodys.end(); ite++)
			{
				Body* body = *ite;
				if (body)
				{
					Body* mergeBody = NULL;
					vector<Face*> faces = body->GetFaces();
					for (int i = 0; i < faces.size(); i++)
					{
						Face* face = faces.at(i);
						if (face)
						{
							Color faceColor = *(face->GetColor());
							mergeBody = GetBody(tmpBodys, faceColor);
							if (mergeBody)
							{
								break;
							}
						}
					}

					if (mergeBody)
					{
						VertexSet* mergeVertexSet = mergeBody->GetData();
						VertexSet* vertexSet = body->GetData();
						std::vector<Vector3>* mergePnts = mergeVertexSet->GetPositionArray();
						int mergePntCount = mergePnts ? mergePnts->size() : 0;
						SPolyLine* mergePolyLine = mergeBody->GetPolyLine();
						SPolyLine* polyLine = body->GetPolyLine();
						vector<Vector3> points = polyLine ? polyLine->GetPoints() : points;
						vector<M3D_INDEX_TYPE> indexs = polyLine ? polyLine->GetPntIndex() : indexs;
						if (mergePolyLine == NULL)
						{
							mergePolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
							mergeBody->SetPolyLine(mergePolyLine);
						}
						int mergelineCount = mergePolyLine->GetPoints().size();
						for (int i = 0; i < points.size(); i++)
						{
							mergePolyLine->AddPoints(points[i]);
							mergePolyLine->AddPointsIndex(indexs[i] + mergePntCount);
						}

						vector<Edge*>* edgeVec = body->GetEdges();
						for (int i = 0; i < edgeVec->size(); i++)
						{
							Edge * edge = edgeVec->at(i);
							if (edge)
							{
								Edge * newEdge = new Edge();
								RefPolyLine* oldLine = edge->GetLineData();
								if (oldLine)
								{
									RefPolyLine * edgeLine = new RefPolyLine(mergePolyLine);
									edgeLine->SetDataOffset(oldLine->GetDataOffset() + mergelineCount);
									edgeLine->SetDataLength(oldLine->GetDataLength());
									//oldLine->Release();
									newEdge->AddData(edgeLine);
								}
								mergeBody->AddEdge(newEdge);
							}
						}
						for (int i = 0; i < faces.size(); i++)
						{
							Face* face = faces.at(i);
							if (face)
							{
								//mergeBody->AddFace(face);
								face->AddRef();
								mergeBody->GetFaces().push_back(face);
							}
						}
						mergeBody->MergeFace();
					}
					else
					{
						Body* newBody = new Body();
						*newBody = *body;
						newBody->MergeFace();
						tmpBodys.push_back(newBody);
					}
				}

				body->Release();
				body = NULL;
			}
			m_bodys.resize(0);
			for (int i = 0; i < tmpBodys.size(); i++)
			{
				AddBody(tmpBodys.at(i));;
			}
			//m_bodys.assign(tmpBodys.begin(), tmpBodys.end());
		}
		return true;
	}

	Body* ModelShape::GetBody(vector<Body*>& bodys, Color& color)
	{
		for (int i = 0; i < bodys.size(); i++)
		{
			Body* body = bodys[i];
			if (body)
			{
				vector<Face*> faces = body->GetFaces();
				for (int j = 0; j < faces.size(); j++)
				{
					Face * face = faces[j];
					if (face)
					{
						Color* faceColor = face->GetColor();
						if (*faceColor == color)
						{
							return body;
						}
					}
				}
			}
		}
		return NULL;
	}

	ImageModelShape::ImageModelShape(void) :ModelShape()
	{
		this->m_image = NULL;
		m_imageBoard = NULL;
	}

	ImageModelShape::~ImageModelShape(void)
	{
		ReleaseMe(this->m_imageBoard);
		ReleaseMe(this->m_image);
	}

	void ImageModelShape::FindVisiableObject(RenderAction* renderAction)
	{
		if (this->IsVisible())
		{
			renderAction->SetWorldMatrix(&GetWorldTransform());
			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());

			//如果使用billboard效果，则进行矩阵校正
			if (this->m_image)
			{
				if (!m_imageBoard->GetTexture() && this->m_image)
				{
					Texture2D* texture2d = (Texture2D*)renderAction->GetScene()->GetResourceManager()->GetOrCreateTexture(this->m_image->GetHashCode(), Texture::TEXTURE_2D);
					//this->m_imageBoard = new ImageBoard();
					//AddRefMe(m_image);
					texture2d->SetImage(m_image);
					//ReleaseMe(m_image);
					this->m_imageBoard->SetTexture(texture2d);
				}

				m_imageBoard->UpdateRenderData(renderAction);
				renderAction->PrepareRenderImage(m_imageBoard);
			}
		}
	}

	void ImageModelShape::RayPick(RayPickAction * action)
	{
		if (this->m_drawDataPrepared)
		{
			//判断线是否有交点
			Vector3 intersectPos;
			//判断文字是否有交点
			MutexLock lock(m_mutex);
 
			if (this->m_imageBoard)
			{	
				//通过shape对象显示时的ModelMatrix，更新Model射线
				action->UpdataModelRay(Matrix3x4::IDENTITY);
				vector<Vector3> intersects = m_imageBoard->GetIntersects(action);
				if (intersects.size() > 0)
				{
					for (int j = 0; j < intersects.size(); j++)
					{
						action->AddIntersectPnt(intersects[j]);
					}

					action->AddShape(this->GetModel(), 1);
				}
			}
		}
	}

	void ImageModelShape::FramePick(RayPickAction* action)
	{
		if (this->m_drawDataPrepared)
		{
			if (action->FrustumIntersetWithWorldBox(GetWorldBoundingBox()))
			{
				action->AddToFramePickCollection(this->GetModel());
			}
		}
	}

	void ImageModelShape::SetImagePath(const string& imagePath)
	{
		this->CreateImage();
		m_image->SetPath(imagePath);
	}

	void ImageModelShape::SetImageData(char* imageData, int dataLength)
	{
		this->CreateImage();
		m_image->SetData((unsigned char*)imageData, dataLength);
	}

	void ImageModelShape::SetImageGPUID(unsigned int gupID)
	{
		this->CreateImage();

		if (!m_imageBoard->GetTexture())
		{
			Texture2DPackaging* texture2dPackaging = new Texture2DPackaging();
			texture2dPackaging->AddRef();
			m_imageBoard->SetTexture(texture2dPackaging);
			texture2dPackaging->Release();
		}

		if (m_imageBoard->GetTexture())
		{
			dynamic_cast<Texture2DPackaging*>(m_imageBoard->GetTexture())->SetTexture2DID(gupID);
		}
	}

	void ImageModelShape::SetFlipImage(bool flipImage)
	{
		this->CreateImage();

		this->m_imageBoard->SetFlipV(flipImage);
	}

	bool ImageModelShape::AllowCuller()
	{
		return false;
	}

	M3D::ImageBoard* ImageModelShape::GetImageBoard()
	{
		return this->m_imageBoard;
	}

	void ImageModelShape::SetImageSize(Vector3& position, Vector2& size)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->SetOrigSize(position, size);
		}
	}

	void ImageModelShape::SetImagePosition(Vector3& position)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->SetPosition(position);
		}
	}

	M3D::Vector3 ImageModelShape::GetImagePosition()
	{
		M3D::Vector3 position;
		if (m_imageBoard)
			position = m_imageBoard->GetPosition();
		return position;
	}

	void ImageModelShape::CreateImage()
	{
		if (this->m_image == NULL)
		{
			this->m_image = new Image();
			this->m_imageBoard = new ImageBoard();
			m_imageBoard->AddRef();
			this->m_image->AddRef();
		}
	}

	void ImageModelShape::SetAllowScall(bool allowScale)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->AllowScale(allowScale);
		}
	}

	void ImageModelShape::SetAllowRotate(bool allowRotate)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->AllowRotate(allowRotate);
		}
	}

	void ImageModelShape::SetAllowTran(bool allowTran)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->AllowTran(allowTran);
		}
	}

	void ImageModelShape::SetInTopShow(bool allowInTopShow)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->SetInTop(allowInTopShow);
		}
	}

	void ImageModelShape::ComputeBox()
	{
		ModelShape::ComputeBox();
		if (this->m_imageBoard)
		{
			Vector3* vertexs = m_imageBoard->GetVertexs();
			this->m_BoundingBox.Merge(m_imageBoard->GetBoundingBox()
				);
		}
	}

	void ImageModelShape::SetSelected(bool select)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->SetSelected(select);
		}
	}

	void ImageModelShape::SetInitHightlight(bool isHighlight)
	{
		if (this->m_imageBoard)
		{
			m_imageBoard->SetInitHightlight(isHighlight);
		}
	}

}

