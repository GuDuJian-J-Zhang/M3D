

#include "m3d/handler/Projector.h"

using namespace M3D;

namespace
{

bool computeClosestPoints(const Line3D& l1, const Line3D& l2,
                          M3D::Vector3& p1, M3D::Vector3& p2)
{
    // Computes the closest points (p1 and p2 on line l1 and l2 respectively) between the two lines
    // An explanation of the algorithm can be found at
    // http://www.geometryalgorithms.com/Archive/algorithm_0106/algorithm_0106.htm
    
   M3D::Vector3 u = l1.GetEndPoint() - l1.GetPosition(); u.Normalize();
   M3D::Vector3 v = l2.GetEndPoint() - l2.GetPosition(); v.Normalize();

   M3D::Vector3 w0 = l1.GetPosition() - l2.GetPosition();

    double a = u.DotProduct(u);
    double b = u.DotProduct(v);
    double c = v.DotProduct(v);
    double d = u.DotProduct(w0);
    double e = v.DotProduct(w0);

    double denominator = a*c - b*b;

    // Test if lines are parallel
    if (fabs(denominator) < 0.00001) return false;

    double sc = (b*e - c*d)/denominator;
    double tc = (a*e - b*d)/denominator;

    p1 = l1.GetPosition() + u * sc;
    p2 = l2.GetPosition() + v * tc;

    return true;
}

bool computeClosestPointOnLine(const M3D::Vector3& lineStart, const M3D::Vector3& lineEnd,
                               const M3D::Vector3& fromPoint, M3D::Vector3& closestPoint)
{
    M3D::Vector3 v = lineEnd - lineStart;
    M3D::Vector3 w = fromPoint - lineStart;

	//v.Normalize();w.Normalize();

    double c1 = w.DotProduct(v);
    double c2 = v.DotProduct(v);

    double almostZero = 0.000001;
    if (c2 < almostZero) return false;

    double b = c1 / c2;
    closestPoint = lineStart + v * b;

    return true;
}

bool getPlaneLineIntersection(const M3D::Vector4& plane, 
                              const M3D::Vector3& lineStart, const M3D::Vector3& lineEnd, 
                              M3D::Vector3& isect)
{
    const double deltaX = lineEnd.m_x - lineStart.m_x;
    const double deltaY = lineEnd.m_y - lineStart.m_y;
    const double deltaZ = lineEnd.m_z - lineStart.m_z;

	const float* planeData = plane.Data();

    const double denominator = (planeData[0]*deltaX + planeData[1]*deltaY + planeData[2]*deltaZ);
    if (! denominator) return false;

    const double C = (planeData[0]*lineStart.m_x + planeData[1]*lineStart.m_y + planeData[2]*lineStart.m_z + planeData[3]) / denominator;

    isect.m_x = lineStart.m_x - deltaX * C;
    isect.m_y = lineStart.m_y - deltaY * C;
    isect.m_z = lineStart.m_z - deltaZ * C;

    return true;
}

bool getSphereLineIntersection(const M3D::Sphere& sphere, 
                               const M3D::Vector3& lineStart, const M3D::Vector3& lineEnd, 
                               M3D::Vector3& frontISect, M3D::Vector3& backISect)
{
    M3D::Vector3 lineDirection = lineEnd - lineStart;
    lineDirection.Normalize();

    M3D::Vector3 v = lineStart - sphere.GetCenter();
    double B = 2.0f * (lineDirection.DotProduct(v));
    double C = v.DotProduct(v) - sphere.GetRadius() * sphere.GetRadius();
    
    double discriminant = B * B - 4.0f * C;

    if (discriminant < 0.0f) // Line and sphere don't intersect.
        return false;

    double discriminantSqroot = sqrtf(discriminant);
    double t0 = (-B - discriminantSqroot) * 0.5f;
    frontISect = lineStart + lineDirection * t0;

    double t1 = (-B + discriminantSqroot) * 0.5f;
    backISect = lineStart + lineDirection * t1;

    return true;
}

bool getUnitCylinderLineIntersection(const M3D::Vector3& lineStart, const M3D::Vector3& lineEnd, 
                                     M3D::Vector3& isectFront, M3D::Vector3& isectBack)
{
    M3D::Vector3 dir = lineEnd - lineStart;
    dir.Normalize();

    double a = dir.Data()[0] * dir.Data()[0] + dir.Data()[1] * dir.Data()[1];
    double b = 2.0f * (lineStart.Data()[0] * dir.Data()[0] + lineStart.Data()[1] * dir.Data()[1]);
    double c = lineStart.Data()[0] * lineStart.Data()[0] + lineStart.Data()[1] * lineStart.Data()[1] - 1;

    double d = b*b - 4*a*c;
    if (d < 0.0f) return false;

    double dSqroot = sqrtf(d);
    double t0, t1;
    if (b > 0.0f)
    {
        t0 = -(2.0f * c) / (dSqroot + b);
        t1 = -(dSqroot + b) / (2.0 * a);
    }
    else
    {
        t0 = (2.0f * c) / (dSqroot - b);
        t1 = (dSqroot - b) / (2.0 * a);
    }

    isectFront = lineStart + dir * t0;
    isectBack = lineStart + dir * t1;
    return true;
}

bool getCylinderLineIntersection(const M3D::Cylinder& cylinder,
                                 const M3D::Vector3& lineStart, const M3D::Vector3& lineEnd, 
                                 M3D::Vector3& isectFront, M3D::Vector3& isectBack)
{
    // Compute matrix transformation that takes the cylinder to a unit cylinder with Z-axis as it's axis and
    // (0,0,0) as it's center.
    double oneOverRadius = 1.0f / cylinder.GetRadius();
	M3D::Matrix3x4 toUnitCylInZ;
	toUnitCylInZ.MultiRotatiton(cylinder.GetRotation().Inverse());
	toUnitCylInZ.MultiScale(oneOverRadius); toUnitCylInZ.MultiTranslate(-cylinder.GetCenter());
    // Transform the lineStart and lineEnd into the unit cylinder space.
    M3D::Vector3 unitCylLineStart = toUnitCylInZ *lineStart;
    M3D::Vector3 unitCylLineEnd   = toUnitCylInZ *lineEnd;

    // Intersect line with unit cylinder.
    M3D::Vector3 unitCylIsectFront, unitCylIsectBack;
    if (! getUnitCylinderLineIntersection(unitCylLineStart, unitCylLineEnd, unitCylIsectFront, unitCylIsectBack))
        return false;    

    // Transform back from unit cylinder space.
    M3D::Matrix3x4 invToUnitCylInZ= toUnitCylInZ.Inverse();
    isectFront = invToUnitCylInZ * unitCylIsectFront;
    isectBack = invToUnitCylInZ *unitCylIsectBack;

    return true;
}

M3D::Vector3 getLocalEyeDirection(const M3D::Vector3& eyeDir, const M3D::Matrix3x4& localToWorld)
{
    // To take a normal from world to local you need to transform it by the transpose of the inverse of the 
    // world to local matrix. Pre-multiplying is equivalent to doing a post-multiplication of the transpose.
    M3D::Vector3 localEyeDir = localToWorld * Vector4(eyeDir,0);
    localEyeDir.Normalize();
    return localEyeDir;
}

M3D::Plane computePlaneThruPointAndOrientedToEye(const M3D::Vector3& eyeDir, const M3D::Matrix3x4& localToWorld,
                                                 const M3D::Vector3& point, bool front)
{
    M3D::Vector3 planeNormal = getLocalEyeDirection(eyeDir, localToWorld);
    if (! front) planeNormal = -planeNormal;

    M3D::Plane plane(planeNormal, point);
    return plane;
}

M3D::Plane computePlaneParallelToAxisAndOrientedToEye(const M3D::Vector3& eyeDir, const M3D::Matrix3x4& localToWorld,
                                                      const M3D::Vector3& axisDir, double radius,
                                                      M3D::Vector3& planeLineStart, M3D::Vector3& planeLineEnd,
                                                      bool front)
{
    M3D::Vector3 perpDir = axisDir.CrossProduct(getLocalEyeDirection(eyeDir, localToWorld));
    M3D::Vector3 planeDir = perpDir.CrossProduct(axisDir);
    planeDir.Normalize();
    if (! front)
        planeDir = -planeDir;

    M3D::Vector3 planePoint = planeDir * radius + axisDir;
	M3D::Plane plane(planeDir, planePoint);

    planeLineStart = planePoint;
    planeLineEnd = planePoint + axisDir;
    return plane;
}

}


Projector::Projector() : _worldToLocalDirty(false)
{
}

Projector::~Projector()
{
}

LineProjector::LineProjector()
{
    _line = new M3D::Line3D(M3D::Vector3(0.0,0.0,0.0),M3D::Vector3(1.0,0.0,0.0));
	AddRefMe(_line);
}

LineProjector::LineProjector(const M3D::Vector3& s,const M3D::Vector3& e)
{
    _line = new M3D::Line3D(s,e);
	AddRefMe(_line);
}

LineProjector::~LineProjector()
{
	ReleaseMe(_line);
}

bool LineProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    // Transform the line to world/object coordinate space.
	Vector3 lineStart = getLocalToWorld()*_line->GetPosition();
	Vector3 lineEnd = getLocalToWorld()*_line->GetEndPoint();
    M3D::Line3D* objectLine = new M3D::Line3D(lineStart,lineEnd);
	AddRefMe(objectLine);
    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);
    M3D::Line3D* pointerLine = new M3D::Line3D(nearPoint,farPoint);
	AddRefMe(pointerLine);
    M3D::Vector3 closestPtLine, closestPtProjWorkingLine;
    if (! computeClosestPoints(*objectLine, *pointerLine, closestPtLine, closestPtProjWorkingLine))
        return false;

	ReleaseMe(objectLine);
	ReleaseMe(pointerLine);

    M3D::Vector3 localClosestPtLine = getWorldToLocal() * closestPtLine;

    projectedPoint = localClosestPtLine;

    return true;
}

PlaneProjector::PlaneProjector()
{
}

PlaneProjector::PlaneProjector(const M3D::Plane& plane)
{
    _plane = plane;
}


PlaneProjector::~PlaneProjector()
{
}

bool PlaneProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);

    // Transform these points into local coordinates.
    M3D::Vector3 objectNearPoint, objectFarPoint;
    objectNearPoint = getWorldToLocal() * nearPoint;
    objectFarPoint  = getWorldToLocal()*farPoint;

    // Find the intersection of the plane with the line (formed by the near and far points in local coordinates).
    return getPlaneLineIntersection(_plane.ToVector4(), objectNearPoint, objectFarPoint, projectedPoint);
}


SphereProjector::SphereProjector() : _sphere(new M3D::Sphere), _front(true)
{
	AddRefMe(_sphere);
}

SphereProjector::SphereProjector(M3D::Sphere* sphere) : _sphere(sphere), _front(true)
{
	AddRefMe(_sphere);
}


SphereProjector::~SphereProjector()
{
	ReleaseMe(_sphere);
}

bool SphereProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    if (!_sphere)
    {
        return false;
    }

    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);

    // Transform these points into local coordinates.
    M3D::Vector3 objectNearPoint, objectFarPoint;
    objectNearPoint = getWorldToLocal() *nearPoint;
    objectFarPoint  = getWorldToLocal()*farPoint;

    // Find the intersection of the sphere with the line.
    M3D::Vector3 dontCare;
    if (_front)
        return getSphereLineIntersection(*_sphere, objectNearPoint, objectFarPoint, projectedPoint, dontCare);
    return getSphereLineIntersection(*_sphere, objectNearPoint, objectFarPoint, dontCare, projectedPoint);
}

bool SphereProjector::isPointInFront(const PointerInfo& pi, const M3D::Matrix3x4& localToWorld) const
{
    M3D::Vector3 centerToPoint = getSphere()->GetCenter() - pi.getLocalIntersectPoint();
    if (centerToPoint.DotProduct(getLocalEyeDirection(pi.getEyeDir(), localToWorld)) < 0.0)
        return false;
    return true;
}


SpherePlaneProjector::SpherePlaneProjector()
{
}

SpherePlaneProjector::SpherePlaneProjector(M3D::Sphere* sphere) : SphereProjector(sphere)
{
}


SpherePlaneProjector::~SpherePlaneProjector()
{
}

M3D::Quaternion SpherePlaneProjector::getRotation(const M3D::Vector3& p1, bool p1OnSphere, const M3D::Vector3& p2, bool p2OnSphere,
                                            float radialFactor) const
{
    if (p1OnSphere && p2OnSphere)
    {
        M3D::Quaternion rotation;
        if (_front)
            rotation = Quaternion(p1 - getSphere()->GetCenter(), p2 - getSphere()->GetCenter());
        else
            rotation = Quaternion(p2 - getSphere()->GetCenter(), p1 - getSphere()->GetCenter());
        return rotation;
    }
    else if (!p1OnSphere && !p2OnSphere)
    {
        M3D::Quaternion rotation;
        rotation = Quaternion(p1 - getSphere()->GetCenter(), p2 - getSphere()->GetCenter());

        M3D::Vector3 axis; double angle;
        rotation = Quaternion(angle, axis);

        M3D::Vector3 realAxis;
        if (axis.DotProduct( _plane.m_normal) > 0.0f)
            realAxis = _plane.m_normal;
        else
            realAxis = - _plane.m_normal;

        M3D::Quaternion rollRotation(angle, realAxis);

        M3D::Vector3 diff1 = p1 - getSphere()->GetCenter();
        M3D::Vector3 diff2 = p2 - getSphere()->GetCenter();
        double d = diff2.Length() - diff1.Length();

        double theta = d / getSphere()->GetRadius();
        if (fabs(theta) < 0.000001 || fabs(theta) > 1.0)
            return rollRotation;

        diff1.Normalize();
        M3D::Vector3 pullAxis = diff1.CrossProduct(_plane.m_normal);
        pullAxis.Normalize();
        M3D::Quaternion pullRotation(radialFactor * theta, pullAxis);

        M3D::Quaternion totalRotation = pullRotation * rollRotation;
        return totalRotation;
    }
    else
    {
        const M3D::Vector3& planePoint = getSphere()->GetCenter();

        M3D::Vector3 intersection, dontCare;
        if (p1OnSphere)
            getSphereLineIntersection(*getSphere(), p2, planePoint, intersection, dontCare);
        else
            getSphereLineIntersection(*getSphere(), p1, planePoint, intersection, dontCare);

        M3D::Quaternion rotation;
        if (p1OnSphere)
            rotation = Quaternion(p1 - getSphere()->GetCenter(), intersection - getSphere()->GetCenter());
        else
            rotation = Quaternion(intersection - getSphere()->GetCenter(), p2 - getSphere()->GetCenter());
        return rotation;
    }
}

bool SpherePlaneProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    if (!_sphere)
    {
       // OSG_WARN << "Warning: Invalid sphere. SpherePlaneProjector::project() failed." << std::endl;
        return false;
    }

    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);

    // Transform these points into local coordinates.
    M3D::Vector3 objectNearPoint, objectFarPoint;
    objectNearPoint = getWorldToLocal()*nearPoint;
    objectFarPoint  = getWorldToLocal()*farPoint;

    // Find the intersection of the sphere with the line.
    M3D::Vector3 sphereIntersection, dontCare;
    bool hitSphere = false;
    if (_front)
        hitSphere = getSphereLineIntersection(*_sphere, objectNearPoint, objectFarPoint, sphereIntersection, dontCare);
    else
        hitSphere = getSphereLineIntersection(*_sphere, objectNearPoint, objectFarPoint, dontCare, sphereIntersection);

    // Compute plane oriented to the eye.
    _plane = computePlaneThruPointAndOrientedToEye(pi.getEyeDir(), getLocalToWorld(), getSphere()->GetCenter(), _front);

    // Find the intersection on the plane.
    M3D::Vector3 planeIntersection;
    if (hitSphere)
    {
        if (! getPlaneLineIntersection(_plane.ToVector4(), sphereIntersection, sphereIntersection + _plane.m_normal, planeIntersection))
            return false;
    }
    else
    {
        if (! getPlaneLineIntersection(_plane.ToVector4(), objectNearPoint, objectFarPoint, planeIntersection))
            return false;
    }

    // Distance from the plane intersection point to the center of the sphere.
    double dist = (planeIntersection - getSphere()->GetCenter()).Length();

    // If the distance is less that the sphere radius choose the sphere intersection else choose
    // the plane intersection.
    if (dist < getSphere()->GetRadius())
    {
        if (! hitSphere) return false;
        projectedPoint = sphereIntersection;
        _onSphere = true;
    }
    else
    {
        projectedPoint = planeIntersection;
        _onSphere = false;
    }
    return true;
}

CylinderProjector::CylinderProjector() : _cylinder(new Cylinder()), _cylinderAxis(0.0, 0.0, 1.0), _front(true)
{
	AddRefMe(_cylinder);
}

CylinderProjector::CylinderProjector(M3D::Cylinder* cylinder) : _front(true)
{
	_cylinder = NULL;
    setCylinder(cylinder);
}

bool CylinderProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    if (!_cylinder)
    {
        return false;
    }

    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);

    // Transform these points into local coordinates.
    M3D::Vector3 objectNearPoint, objectFarPoint;
    objectNearPoint = getWorldToLocal() *nearPoint;
    objectFarPoint  = getWorldToLocal()*farPoint;

    // Find the intersection of the sphere with the line.
    M3D::Vector3 dontCare;
    return getCylinderLineIntersection(*_cylinder, objectNearPoint, objectFarPoint, projectedPoint, dontCare);
}

bool CylinderProjector::isPointInFront(const PointerInfo& pi, const M3D::Matrix3x4& localToWorld) const
{
    M3D::Vector3 closestPointOnAxis;
    computeClosestPointOnLine(getCylinder()->GetCenter(), getCylinder()->GetCenter() + _cylinderAxis,
                              pi.getLocalIntersectPoint(), closestPointOnAxis);

    M3D::Vector3 perpPoint = pi.getLocalIntersectPoint() - closestPointOnAxis;
    if (perpPoint.DotProduct(getLocalEyeDirection(pi.getEyeDir(), localToWorld)) < 0.0)
        return false;
    return true;
}

CylinderPlaneProjector::CylinderPlaneProjector()
{
}

CylinderPlaneProjector::CylinderPlaneProjector(M3D::Cylinder* cylinder) : CylinderProjector(cylinder)
{
}

CylinderPlaneProjector::~CylinderPlaneProjector()
{
}

bool CylinderPlaneProjector::project(const PointerInfo& pi, M3D::Vector3& projectedPoint) const
{
    if (!_cylinder)
    {
        return false;
    }

    // Get the near and far points for the mouse point.
    M3D::Vector3 nearPoint, farPoint;
    pi.getNearFarPoints(nearPoint,farPoint);

    // Transform these points into local coordinates.
    M3D::Vector3 objectNearPoint, objectFarPoint;
    objectNearPoint = getWorldToLocal()*nearPoint;
    objectFarPoint  = getWorldToLocal()*farPoint;

    // Find the intersection of the sphere with the line.
    M3D::Vector3 cylIntersection;
    bool hitCylinder = false;
    if (_front)
    {
        M3D::Vector3 dontCare;
        hitCylinder = getCylinderLineIntersection(*_cylinder, objectNearPoint, objectFarPoint, cylIntersection, dontCare);
    }
    else
    {
        M3D::Vector3 dontCare;
        hitCylinder = getCylinderLineIntersection(*_cylinder, objectNearPoint, objectFarPoint, dontCare, cylIntersection);
    }

    // Compute plane oriented to the eye.
    _plane = computePlaneParallelToAxisAndOrientedToEye(pi.getEyeDir(), getLocalToWorld(), _cylinderAxis,
                                                        getCylinder()->GetRadius(), _planeLineStart, _planeLineEnd,
                                   _front);

    // Find the intersection on the plane.
    M3D::Vector3 planeIntersection;
    getPlaneLineIntersection(_plane.ToVector4(), objectNearPoint, objectFarPoint, planeIntersection);

    if (hitCylinder)
    {
        M3D::Vector3 projectIntersection;
        getPlaneLineIntersection(_plane.ToVector4(), cylIntersection, cylIntersection + _plane.m_normal, projectIntersection);

        M3D::Vector3 closestPointToCylAxis;
        computeClosestPointOnLine(getCylinder()->GetCenter(), getCylinder()->GetCenter() + _cylinderAxis,
                                  projectIntersection, closestPointToCylAxis);

        // Distance from the plane intersection point to the closest point on the cylinder axis.
        double dist = (projectIntersection - closestPointToCylAxis).Length();

        if (dist < getCylinder()->GetRadius())
        {
            if (!hitCylinder) return false;
            projectedPoint = cylIntersection;
            _onCylinder = true;
        }
        else
        {
            projectedPoint = planeIntersection;
            _onCylinder = false;
        }
    }
    else
    {
        projectedPoint = planeIntersection;
        _onCylinder = false;
    }

    return true;
}

M3D::Quaternion CylinderPlaneProjector::getRotation(const M3D::Vector3& p1, bool p1OnCyl, const M3D::Vector3& p2, bool p2OnCyl) const
{
    if (p1OnCyl && p2OnCyl)
    {
        M3D::Vector3 closestPointToCylAxis1, closestPointToCylAxis2;
        computeClosestPointOnLine(getCylinder()->GetCenter(), getCylinder()->GetCenter() + _cylinderAxis * getCylinder()->GetHeight(),
                                  p1, closestPointToCylAxis1);
        computeClosestPointOnLine(getCylinder()->GetCenter(), getCylinder()->GetCenter() + _cylinderAxis * getCylinder()->GetHeight(),
                                  p2, closestPointToCylAxis2);

        M3D::Vector3 v1 = p1 - closestPointToCylAxis1;
        M3D::Vector3 v2 = p2 - closestPointToCylAxis2;

        double cosAngle = v1.DotProduct(v2) / (v1.Length() * v2.Length());

        if (cosAngle > 1.0 || cosAngle < -1.0)
            return M3D::Quaternion();

        double angle = acosf(cosAngle);
        M3D::Vector3 rotAxis = v1.CrossProduct(v2);

        return M3D::Quaternion(angle/(M_DEGTORAD), rotAxis);
    }
    else if (!p1OnCyl && !p2OnCyl)
    {
        M3D::Vector3 closestPointToPlaneLine1, closestPointToPlaneLine2;
        computeClosestPointOnLine(_planeLineStart, _planeLineEnd,
                                  p1, closestPointToPlaneLine1);
        computeClosestPointOnLine(_planeLineStart, _planeLineEnd,
                                  p2, closestPointToPlaneLine2);

        M3D::Vector3 v1 = p1 - closestPointToPlaneLine1;
        M3D::Vector3 v2 = p2 - closestPointToPlaneLine2;

        M3D::Vector3 diff = v2 - v1;
        double d = diff.Length();

        double angle = (getCylinder()->GetRadius() == 0.0) ? 0.0 : (d / getCylinder()->GetRadius());
        M3D::Vector3 rotAxis = _plane.m_normal.CrossProduct(v1);
		//angle = 0.001;
		if (v2.Length() > v1.Length())
			return M3D::Quaternion(angle / (M_DEGTORAD), rotAxis);
		else
            return M3D::Quaternion(-angle/ (M_DEGTORAD), rotAxis);

    }
    else
    {
        M3D::Vector3 offCylinderPt = (p1OnCyl) ? p2 : p1;

        M3D::Vector3 linePtNearest;
        computeClosestPointOnLine(_planeLineStart, _planeLineEnd,
                                  offCylinderPt, linePtNearest);
        M3D::Vector3 dirToOffCylinderPt = offCylinderPt - linePtNearest;
        dirToOffCylinderPt.Normalize();

        M3D::Vector3 ptOnCylinder = linePtNearest + dirToOffCylinderPt * getCylinder()->GetRadius();

        if (p1OnCyl)
            return (getRotation(ptOnCylinder, false, p2, false)*getRotation(p1, true, ptOnCylinder, true)
                    );
        else
            return (getRotation(ptOnCylinder, true, p2, true)*getRotation(p1, false, ptOnCylinder, false)
                    );
    }
}
