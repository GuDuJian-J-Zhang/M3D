#ifndef M3D_OCTREE_QUERY_H_
#define M3D_OCTREE_QUERY_H_

#include "m3d/M3D.h"
#include "m3d/base/Frustum.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Sphere.h"

namespace M3D
{
class Drawable;
class Node;
class ModelShape;
class RenderAction;
class CameraNode;

/// Graphics raycast detail level.
enum RayQueryLevel
{
	RAY_AABB = 0,
	RAY_OBB,
	RAY_TRIANGLE,
	RAY_TRIANGLE_UV
};

/// Base class for octree queries.
class M3D_API OctreeQuery
{
public:
    /// Construct with query parameters.
    OctreeQuery(vector<ModelShape*>& result, unsigned char drawableFlags, unsigned viewMask) :
        m_result(result),
        m_drawableFlags(drawableFlags),
        m_viewMask(viewMask)
    {
		this->SetRenderAction(NULL);
		this->SetCamera(NULL);
    }

    /// Destruct.
    virtual ~OctreeQuery()
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside) = 0;
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside) = 0;

	void SetRenderAction(RenderAction* action);
	RenderAction* GetRenderAction();

	void SetCamera(CameraNode* camera);
	CameraNode* GetCamera();

    /// Result vector reference.
	vector<ModelShape*>& m_result;
    /// Drawable flags to include.
    unsigned char m_drawableFlags;
    /// Drawable layers to include.
    unsigned m_viewMask;

protected:
	RenderAction* m_renderAction;
	CameraNode* m_camera;
private:
    /// Prevent copy construction.
    OctreeQuery(const OctreeQuery& rhs);
    /// Prevent assignment.
    OctreeQuery& operator =(const OctreeQuery& rhs);
};

/// Point octree query.
class M3D_API PointOctreeQuery : public OctreeQuery
{
public:
    /// Construct with point and query parameters.
    PointOctreeQuery(vector<ModelShape*>& result, const Vector3& point, unsigned char drawableFlags,
        unsigned viewMask) :
        OctreeQuery(result, drawableFlags, viewMask),
        m_point(point)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside);

    /// Point.
    Vector3 m_point;
};

/// %Sphere octree query.
class M3D_API SphereOctreeQuery : public OctreeQuery
{
public:
    /// Construct with sphere and query parameters.
    SphereOctreeQuery(vector<ModelShape*>& result, const Sphere& sphere, unsigned char drawableFlags,
        unsigned viewMask) :
        OctreeQuery(result, drawableFlags, viewMask),
        m_sphere(sphere)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside);

    /// Sphere.
    Sphere m_sphere;
};

/// Bounding box octree query.
class M3D_API BoxOctreeQuery : public OctreeQuery
{
public:
    /// Construct with bounding box and query parameters.
    BoxOctreeQuery(vector<ModelShape*>& result, const BoundingBox& box, unsigned char drawableFlags,
        unsigned viewMask) :
        OctreeQuery(result, drawableFlags, viewMask),
        m_box(box)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside);

    /// Bounding box.
    BoundingBox m_box;
};

/// %Frustum octree query.
class M3D_API FrustumOctreeQuery : public OctreeQuery
{
public:
    /// Construct with frustum and query parameters.
    FrustumOctreeQuery(vector<ModelShape*>& result, const Frustum& frustum, unsigned char drawableFlags,
        unsigned viewMask) :
        OctreeQuery(result, drawableFlags, viewMask),
        m_frustum(frustum)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside);

    /// Frustum.
    Frustum m_frustum;
};

/// General octree query result. Used for Lua bindings only.
struct M3D_API OctreeQueryResult
{
    /// Construct with defaults.
    OctreeQueryResult() :
        m_drawable(0),
        m_node(0)
    {
    }

    /// Test for inequality, added to prevent GCC from complaining.
    bool operator !=(const OctreeQueryResult& rhs) const { return m_drawable != rhs.m_drawable || m_node != rhs.m_node; }

    /// LModelShape.
	ModelShape* m_drawable;
    /// Scene node.
    Node* m_node;
};

/// Raycast result.
struct M3D_API RayQueryResult
{
    /// Construct with defaults.
    RayQueryResult() :
        m_drawable(0),
        m_node(0)
    {
    }

    /// Test for inequality, added to prevent GCC from complaining.
    bool operator !=(const RayQueryResult& rhs) const
    {
        return m_position != rhs.m_position ||
               m_normal != rhs.m_normal ||
               m_textureUV != rhs.m_textureUV ||
               m_distance != rhs.m_distance ||
               m_drawable != rhs.m_drawable ||
               m_node != rhs.m_node ||
               m_subObject != rhs.m_subObject;
    }

    /// Hit position in world space.
    Vector3 m_position;
    /// Hit normal in world space. Negation of ray direction if per-triangle data not available.
    Vector3 m_normal;
    /// Hit texture position
    Vector2 m_textureUV;
    /// Distance from ray origin.
    float m_distance;
    /// LModelShape.
	ModelShape* m_drawable;
    /// Scene node.
    Node* m_node;
    /// LModelShape specific subobject if applicable.
    unsigned m_subObject;
};

/// Raycast octree query.
class M3D_API RayOctreeQuery
{
public:
    /// Construct with ray and query parameters.
    RayOctreeQuery(vector<RayQueryResult>& result, const Ray& ray, RayQueryLevel level,
        float maxDistance, unsigned char drawableFlags, unsigned viewMask) :
        m_result(&result),
        m_ray(ray),
        m_drawableFlags(drawableFlags),
        m_viewMask(viewMask),
        m_maxDistance(maxDistance),
        m_level(level),
		m_fastResult(NULL)
    {
	}

	RayOctreeQuery(vector<ModelShape*>&fastResult, const Ray& ray, RayQueryLevel level,
		float maxDistance) :
		m_fastResult(&fastResult),
		m_ray(ray),
		m_maxDistance(maxDistance),
		m_level(level),
		m_result(NULL)
	{

	}

    /// Result vector reference.
	vector<RayQueryResult>* m_result;

	vector<ModelShape*>* m_fastResult;
	/// Ray.
    Ray m_ray;
    /// Drawable flags to include.
    unsigned char m_drawableFlags;
    /// Drawable layers to include.
    unsigned m_viewMask;
    /// Maximum ray distance.
    float m_maxDistance;
    /// Raycast detail level.
    RayQueryLevel m_level;

private:
    /// Prevent copy construction.
    RayOctreeQuery(const RayOctreeQuery& rhs);
    /// Prevent assignment.
    RayOctreeQuery& operator =(const RayOctreeQuery& rhs);
};

class M3D_API AllContentOctreeQuery : public OctreeQuery
{
public:
    /// Construct.
    AllContentOctreeQuery(vector<ModelShape*>& result, unsigned char drawableFlags, unsigned viewMask) :
        OctreeQuery(result, drawableFlags, viewMask)
    {
    }

    /// Intersection test for an octant.
    virtual Intersection TestOctant(const BoundingBox& box, bool inside);
    /// Intersection test for drawables.
    virtual void TestDrawables(ModelShape** start, ModelShape** end, bool inside);
};

}

#endif