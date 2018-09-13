/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_L_SHAPE_NODE_H_
#define     M3D_L_SHAPE_NODE_H_

#include "m3d/M3D.h"
#include "m3d/model/Model.h"

namespace M3D
{
class Octant;
class Octree;
class Image;
class ImageBoard;

class M3D_API ModelShape
{
public:
	ModelShape(void);
	virtual ~ModelShape(void);

	ModelShape(const ModelShape& orig);
	ModelShape& operator =(const ModelShape& orig);

	bool CopyFrom(const ModelShape& orig);

	virtual SHAPETYPE GetType(void);

	/**
	 * @see ShapeNode
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void FindVisiableObjectFast(RenderAction* renderAction, int littleModelState);

	BoundingBox& GetWorldBoundingBox();

	BoundingBox& GetBoundingBox();

	void SetBoundingBox(BoundingBox& boundingBox);

	Matrix4& GetGLWorldTransform();

	void UpdateWorldTransform();

	void  UpdateOCTree();

	Matrix3x4& GetWorldTransform();

	void Clear();

	void SetModel(Model* model);
	Model* GetModel();
	void AddBody(Body *body);
	virtual void SetSelected(bool select);
	bool IsVisible();
	void SetVisible(bool visible);
	virtual void RayPick(RayPickAction* action);
	virtual void FramePick(RayPickAction* action);
	int GetVertexCount(int level);
	virtual void ComputeBox();
	void SetInitColor(const Color &color);
	void SetInitAlpha(float alpha);
	virtual void SetInitHightlight(bool isHighlight);
	bool IsHightlight();
	void SetAlpha(float a);
	float GetAlpha();
	void ResetColor();
	float GetVolumeAndArea(float& volume, float& area);

	void Restore();
	void ResetAlpha();
	void SetColor(const Color &color);
	Color* GetColor();
	vector<Body*>* GetBodys();
	void SetBodys(vector<Body*>& bodys);

	void MarkDirty();
	bool IsDirty() const;

	//不允许剔除的模型，将加入八叉树显示的顶层
	virtual bool  AllowCuller();

	void SetOctant(Octant* octant);
	Octant* GetOctant() const;
	void SetOctree(Octree* octant);
	Octree* GetOctree() const;

	bool isOCtreeUpdated();
	void SetOCTreeUpdated(bool updated);

	bool IsDrawDataPrepared();
	void SetDrawDataPrepared(bool dataPrepared);
	void SetWorldMatrix(Matrix3x4& worldMatrix);

	/**
	* @brief Move the scene node in the chosen transform space.
	*/
	void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);

	void MarkDrawDataDirty();

	void SetNeedClip(bool val);

	void Selectable(bool selectable);
protected:
	mutable Matrix4 m_glRenderMatrix;//!< @see ShapeNode
	//相对于世界坐标的包围盒
	mutable BoundingBox m_worldBox;//!< @see ShapeNode
	mutable BoundingBox m_BoundingBox;
	vector<Body*> m_bodys; //!<body数据

	mutable bool m_dirty; //!< @see SceneNode
	mutable Matrix3x4 m_worldMatrix;
	Model* m_parentModel;

	Octant* m_octant;
	Octree* m_octree;
	bool m_updateQueued;

	bool m_drawDataPrepared; //显示数据是否准备完毕标志。

	Mutex m_mutex;
};

class M3D_API ImageModelShape :public ModelShape
{
public:
	ImageModelShape(void);

	virtual ~ImageModelShape(void);

	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void RayPick(RayPickAction * action);

	virtual void FramePick(RayPickAction * action);

	void SetImagePath(const string& imagePath);
	string GetImagePath();

	void SetImageData(char* imageData, int dataLength);

	void SetImageGPUID(unsigned int gupID);

	void SetFlipImage(bool flipImage);

	virtual bool  AllowCuller();

	ImageBoard* GetImageBoard();

	void SetImageSize(Vector3& position,Vector2& size);
	Vector2 GetImageSize();

	void SetImagePosition(Vector3& position);
	Vector3 GetImagePosition();

	void SetAllowScall(bool allowScale);

	void SetAllowRotate(bool allowRotate);

	void SetAllowTran(bool allowTran);

	void SetFixShowInScreen(bool fixShow);

	void  SetInTopShow(bool allowInTopShow);
	virtual void ComputeBox();

	virtual void SetSelected(bool select);
	virtual void SetInitHightlight(bool isHighlight);
private:
	void CreateImage();

private:
	ImageBoard* m_imageBoard;
	Image* m_image;
};

}
#endif // L_SCENE_NODE_H
