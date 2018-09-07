

#ifndef M3DMANIPULATOR_PROJECTOR
#define M3DMANIPULATOR_PROJECTOR 1

 
 
#include "m3d/handler/Dragger.h"
#include "m3d/model/geometry/Cylinder.h"
#include "m3d/base/Object.h"

namespace M3D {

/**
 * Base class for Projectors. Projectors maps 2D cursor motions to 3D motions.
 */
class M3D_API Projector :public Object
{
    public:
        Projector();

        /**
         * Calculates the object/world coordinates (projectedPoint) of a window
         * coordinate (pointToProject) when projected onto some shape or 
         * geometry (implemented in derived classes). SceneView in used for i
         * projecting window coordinates into object coordinates and vice versa.
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const = 0;

        /**
         * Sets the matrix for transforming the projector's local coordinate
         * system to the world/object coordinate system.
         */
        void setLocalToWorld(const M3D::Matrix3x4& localToWorld)
        {
            _localToWorld = localToWorld;
            _worldToLocalDirty = true;
        }

        /**
         * Gets the matrix for transforming the projector's local coordinate
         * system to the world/object coordinate system.
         */
        inline const M3D::Matrix3x4& getLocalToWorld() const { return _localToWorld; }

        /**
         * Gets the matrix for transforming the world/object coordinate
         * system to the command's local coordinate system.
         */
        inline const M3D::Matrix3x4& getWorldToLocal() const 
        {
            if (_worldToLocalDirty)
            {
                _worldToLocal = _localToWorld.Inverse();
                _worldToLocalDirty = false;
            }
            return _worldToLocal;
        }

    protected:

        virtual ~Projector();

        M3D::Matrix3x4         _localToWorld;
        mutable M3D::Matrix3x4 _worldToLocal;

        mutable bool        _worldToLocalDirty;
};


/**
 * LineProjector projects points onto the closest point on the given line.
 */
class M3D_API LineProjector : public Projector
{
    public:

        LineProjector();

        LineProjector(const M3D::Vector3& s,const M3D::Vector3& e);

        inline void setLine(const M3D::Vector3& s,const M3D::Vector3& e) { _line->m_StartPoint = s; _line->m_EndPoint = e; }

        inline const M3D::Vector3& getLineStart() const { return _line->m_StartPoint; }
        inline M3D::Vector3& getLineStart() { return _line->m_StartPoint; }

        inline const M3D::Vector3& getLineEnd() const { return _line->m_EndPoint; }
        inline M3D::Vector3& getLineEnd() { return _line->m_EndPoint; }

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given line. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

    protected:

        virtual ~LineProjector();

        M3D::Line3D* _line;
};

/**
 * PlaneProjector projects points onto the given line.
 */
class M3D_API PlaneProjector : public Projector
{
    public:

        PlaneProjector();

        PlaneProjector(const M3D::Plane& plane);

        inline void setPlane(const M3D::Plane& plane) { _plane = plane; }
        inline const M3D::Plane& getPlane() const { return _plane; }

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given plane. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

    protected:

        virtual ~PlaneProjector();

        M3D::Plane _plane;
};

/**
 * SphereProjector projects points onto the given sphere.
 */
class M3D_API SphereProjector : public Projector
{
    public:

        SphereProjector();

        SphereProjector(M3D::Sphere* sphere);

        inline void setSphere(M3D::Sphere* sphere) { _sphere = sphere; }
        inline const M3D::Sphere* getSphere() const { return _sphere; }

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given sphere. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

        /**
         * Returns true is the point is in front of the cylinder given the eye
         * direction.
         */
        bool isPointInFront(const PointerInfo& pi, const M3D::Matrix3x4& localToWorld) const;

        void setFront(bool front) { _front = front; }

    protected:

        virtual ~SphereProjector();

        M3D::Sphere* _sphere;
        bool                      _front;
};

/**
 * SpherePlaneProjector projects points onto a sphere, failing which it project
 * onto a plane oriented to the viewing direction.
 */
class M3D_API SpherePlaneProjector : public SphereProjector
{
    public:

        SpherePlaneProjector();

        SpherePlaneProjector(M3D::Sphere* sphere);

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given sphere. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

        /**
         * Returns true if the previous projection was on the sphere and false 
         * if the projection was on the plane.
         */
        bool isProjectionOnSphere() const { return _onSphere; }

        M3D::Quaternion getRotation(const M3D::Vector3& p1, bool p1OnSphere, 
                              const M3D::Vector3& p2, bool p2OnSphere,
                              float radialFactor = 0.0f) const;

    protected:

        virtual ~SpherePlaneProjector();

        mutable M3D::Plane _plane;
        mutable bool       _onSphere;
};

/**
 * CylinderProjector projects points onto the given cylinder.
 */
class M3D_API CylinderProjector : public Projector
{
    public:

        CylinderProjector();

        CylinderProjector(M3D::Cylinder* cylinder);

		~CylinderProjector() {
			ReleaseMe(_cylinder);
		}

        inline void setCylinder(M3D::Cylinder* cylinder)
        {
			ReleaseMe(_cylinder);
            _cylinder = cylinder;
			_cylinder->AddRef();
            _cylinderAxis =  cylinder->GetRotation()* M3D::Vector3(0.0,0.0,1.0);
            _cylinderAxis.Normalize();
        }
        inline const M3D::Cylinder* getCylinder() const { return _cylinder; }

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given plane. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

        
        /**
         * Returns true is the point is in front of the cylinder given the eye 
         * direction.
         */
        bool isPointInFront(const PointerInfo& pi, const M3D::Matrix3x4& localToWorld) const;

        void setFront(bool front) { _front = front; }

    protected:

        M3D::Cylinder* _cylinder;
        M3D::Vector3                  _cylinderAxis;
        bool                        _front;
};

/**
 * CylinderPlaneProjector projects points onto the given cylinder.
 */
class M3D_API CylinderPlaneProjector : public CylinderProjector
{
    public:

        CylinderPlaneProjector();

        CylinderPlaneProjector(M3D::Cylinder* cylinder);

        /**
         * Calculates the object coordinates (projectedPoint) of a window 
         * coordinate (pointToProject) when projected onto the given plane. 
         * Returns true on successful projection.
         */
        virtual bool project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const;

        /**
         * Returns true if the previous projection was on the cylinder and 
         * false if the projection was on the plane.
         */
        bool isProjectionOnCylinder() const { return _onCylinder; }

        M3D::Quaternion getRotation(const M3D::Vector3& p1, bool p1OnCyl, const M3D::Vector3& p2, bool p2OnCyl) const;

    protected:

        virtual ~CylinderPlaneProjector();

        mutable M3D::Plane _plane;
        mutable bool       _onCylinder;
        mutable M3D::Vector3 _planeLineStart, _planeLineEnd;
};

}

#endif
