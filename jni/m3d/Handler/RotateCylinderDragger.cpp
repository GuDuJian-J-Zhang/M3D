

#include "m3d/handler/RotateCylinderDragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/Parameters.h"
#include "../utils/MeshHelper.h"
#include "../scene/ShapeNode.h"

using namespace M3D;

RotateCylinderDragger::RotateCylinderDragger()
{
	_drawModel = NULL;
    _projector = new CylinderPlaneProjector();
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
	_projector->AddRef();
}

RotateCylinderDragger::~RotateCylinderDragger()
{
	ReleaseMe(_projector);
}

bool RotateCylinderDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
{
    // Check if the dragger node is in the nodepath.
   // if (!pointer.contains(this)) return false;

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

                    _prevWorldProjPt = /*_projector->getLocalToWorld()**/projectedPoint;
                    _prevRotation = M3D::Quaternion();
                    _prevPtOnCylinder = _projector->isProjectionOnCylinder();

                    /*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick move.
        case ( TouchEvent::DRAG):
            {
			M3D::Matrix3x4 localToWorld = GetWorldTransform();
			//_projector->setLocalToWorld(localToWorld);

               //_pointer.setMousePosition(ea.getX(), ea.getY());
                // Get the LocalToWorld matrix for this node and set it for the projector.
		/*	Matrix3x4 rotationMatrix;
			rotationMatrix.SetRotation(_prevRotation.RotationMatrix());
			Matrix3x4 localToWorld =_startLocalToWorld*rotationMatrix;
            _projector->setLocalToWorld(localToWorld);*/
		
                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    M3D::Vector3 prevProjectedPoint = /*_projector->getWorldToLocal()**/ _prevWorldProjPt;
                    M3D::Quaternion  deltaRotation = _projector->getRotation(prevProjectedPoint, _prevPtOnCylinder,
                                                                      projectedPoint, _projector->isProjectionOnCylinder());
                    M3D::Quaternion rotation = _prevRotation*deltaRotation;

					//printf("_prevRotation %s", _prevRotation.Tostring().c_str());
                    // Generate the motion command.

					Vector3 deltaVec = prevProjectedPoint - projectedPoint;
					float deltaVecLength = deltaVec.Length();

                    Rotate3DCommand* cmd = new Rotate3DCommand();
					cmd->AddRef();
                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_startLocalToWorld,_startWorldToLocal);
                    cmd->setRotation(rotation);
					cmd->setDeltaRotation(deltaRotation);
			 
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();

                    _prevWorldProjPt =/* _projector->getLocalToWorld() **/ projectedPoint;
                    _prevRotation = rotation;
                    _prevPtOnCylinder = _projector->isProjectionOnCylinder();
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

void RotateCylinderDragger::setupDefaultGeometry()
{
  /*  M3D::Geode* geode = new M3D::Geode;
    geode->addDrawable(new M3D::ShapeDrawable(const_cast<M3D::Cylinder*>(_projector->getCylinder())));
    addChild(geode);*/

	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("CyicleXYHandler.stl");
	_drawModel = NULL;
	if (MeshHelper::ReadSingleModel(toolsFilePath, _drawModel))
	{
		if (_drawModel) {
			//int nPlcPath = (int) strtol(plcIdPath, NULL, 16);
			ShapeNode* shapeNode = new ShapeNode();
			this->AddChild(shapeNode);
			shapeNode->SetShape(_drawModel);
			_drawModel->SetInitHightlight(true);
			_drawModel->SetUserData(this);
			_drawModel->Release();
		}
	}
}
