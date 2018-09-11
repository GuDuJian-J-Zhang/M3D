

#include "m3d/handler/RotateSphereDragger.h""
#include "m3d/handler/Command.h"
 
#include <iostream>

using namespace M3D;

RotateSphereDragger::RotateSphereDragger() :Dragger(), _prevPtOnSphere(true)
{
    _projector = new SpherePlaneProjector();
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
	_projector->AddRef();
}

RotateSphereDragger::~RotateSphereDragger()
{
	ReleaseMe(_projector);
}

bool RotateSphereDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
{
    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this)) return false;

    switch (ea.getEventType())
    {
        // Pick start.
        case ( TouchEvent::PUSH):
            {
                // Get the LocalToWorld matrix for this node and set it for the projector.
                M3D::Matrix3x4 localToWorld = GetWorldTransform();
                _projector->setLocalToWorld(localToWorld);

                _startLocalToWorld = _projector->getLocalToWorld();
                _startWorldToLocal = _projector->getWorldToLocal();

                if (_projector->isPointInFront(pointer, _startLocalToWorld))
                    _projector->setFront(true);
                else
                    _projector->setFront(false);

                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    // Generate the motion command.
                    Rotate3DCommand* cmd = new Rotate3DCommand();
					cmd->AddRef();
                    cmd->setStage(MotionCommand::START);
                    cmd->setLocalToWorldAndWorldToLocal(_startLocalToWorld,_startWorldToLocal);

                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                    // Set color to pick color.
                    /*setMaterialColor(_pickColor,*this);*/

                    _prevRotation = M3D::Quaternion();
                    _prevWorldProjPt = _projector->getLocalToWorld()*projectedPoint;
                    _prevPtOnSphere = _projector->isProjectionOnSphere();

                    /*aa.requestRedraw();*/
                }

                return true; 
            }
            
        // Pick move.
        case ( TouchEvent::DRAG):
            {
                // Get the LocalToWorld matrix for this node and set it for the projector.
			M3D::Matrix3x4 rotationMatrix;
			rotationMatrix.SetRotation(_prevRotation.RotationMatrix());
                M3D::Matrix3x4 localToWorld = _startLocalToWorld*rotationMatrix;
                _projector->setLocalToWorld(localToWorld);

                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    M3D::Vector3 prevProjectedPoint = _projector->getWorldToLocal()*_prevWorldProjPt;
                    M3D::Quaternion  deltaRotation = _projector->getRotation(prevProjectedPoint, _prevPtOnSphere,
                                                                      projectedPoint, _projector->isProjectionOnSphere(),1.0f);
                    M3D::Quaternion rotation = deltaRotation * _prevRotation;

                    // Generate the motion command.
                    Rotate3DCommand* cmd = new Rotate3DCommand();
					cmd->AddRef();
                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_startLocalToWorld,_startWorldToLocal);
                    cmd->setRotation(rotation);

                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                    _prevWorldProjPt = _projector->getLocalToWorld()*projectedPoint;
                    _prevRotation = rotation;
                    _prevPtOnSphere = _projector->isProjectionOnSphere();
                    /*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick finish.
        case ( TouchEvent::RELEASE):
            {
                Rotate3DCommand* cmd = new Rotate3DCommand();
				cmd->AddRef();
                cmd->setStage(MotionCommand::FINISH);
                cmd->setLocalToWorldAndWorldToLocal(_startLocalToWorld,_startWorldToLocal);

                // Dispatch command.
                dispatch(*cmd);
				cmd->Release();
                // Reset color.
                /*setMaterialColor(_color,*this);*/

                /*aa.requestRedraw();*/

                return true;
            }
        default:
            return false;
    }
}

void RotateSphereDragger::setupDefaultGeometry()
{
   /* M3D::Geode* geode = new M3D::Geode;
    geode->addDrawable(new M3D::ShapeDrawable(const_cast<M3D::Sphere*>(_projector->getSphere())));
    addChild(geode);*/
}
