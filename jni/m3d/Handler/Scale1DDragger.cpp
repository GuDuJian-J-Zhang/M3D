

#include "m3d/handler/Scale1DDragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/Parameters.h"
#include "../utils/MeshHelper.h"
#include "../scene/ShapeNode.h"

namespace M3D
{

double computeScale(const M3D::Vector3& startProjectedPoint,
                   const M3D::Vector3& projectedPoint, double scaleCenter)
{
    double denom = startProjectedPoint.Data()[0] - scaleCenter;
    double scale = denom ? (projectedPoint.Data()[0] - scaleCenter)/denom : 1.0;
    return scale;
}

}

Scale1DDragger::Scale1DDragger(ScaleMode scaleMode) : Dragger(), _minScale(0.001), _scaleMode(scaleMode)
{
	_drawModel = NULL;
    _projector = new LineProjector(M3D::Vector3(-0.5,0.0,0.0),M3D::Vector3(0.5,0.0,0.0));
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
	this->SetScale(0.0f);
	_projector->AddRef();
}

Scale1DDragger::~Scale1DDragger()
{
	ReleaseMe(_projector);
}

bool Scale1DDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
{
    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this)) return false;

    switch (ea.getEventType())
    {
        // Pick start.
        case ( TouchEvent::PUSH):
            {
				_priProjectedPoint = Vector3::ZERO;
                // Get the LocalToWorld matrix for this node and set it for the projector.
                M3D::Matrix3x4 localToWorld = GetWorldTransform();
                _projector->setLocalToWorld(localToWorld);

                if (_projector->project(pointer, _startProjectedPoint))
				{
					this->SetScale(0.0f);
                    _scaleCenter = 0.0;
                   /* if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
                    {
                        if ( pointer.contains(_leftHandleNode) )
                            _scaleCenter = _projector->getLineEnd().Data()[0];
                        else if ( pointer.contains( _rightHandleNode) ) 
                            _scaleCenter = _projector->getLineStart().Data()[0];
                    }*/

                    // Generate the motion command.
                    Scale1DCommand* cmd = new Scale1DCommand();
					cmd->AddRef();
					cmd->SetRefDragger(this);
                    cmd->setStage(MotionCommand::START);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
					this->_priProjectedPoint = _startProjectedPoint;
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                    // Set color to pick color.
                    /*setMaterialColor(_pickColor,*this);*/

                    /*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick move.
        case ( TouchEvent::DRAG):
		{		M3D::Matrix3x4 localToWorld = this->GetWorldTransform();//::computeLocalToWorld(nodePathToRoot);
				_projector->setLocalToWorld(localToWorld);

                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    // Generate the motion command.
                    Scale1DCommand* cmd = new Scale1DCommand();
					cmd->AddRef();
					cmd->SetRefDragger(this);
					Vector3 translateVec3 = projectedPoint - _priProjectedPoint;
					 
					double priScale = computeScale(_startProjectedPoint, this->_priProjectedPoint, _scaleCenter);
					//if (priScale < getMinScale()) priScale = getMinScale();

					this->_priProjectedPoint = projectedPoint;
                    // Compute scale.
                    double scale = computeScale(_startProjectedPoint, projectedPoint,_scaleCenter);
                    //if (scale < getMinScale()) scale = getMinScale();

                    // Snap the referencePoint to the line start or line end depending on which is closer.
                 //   double referencePoint = _startProjectedPoint.Data()[0];
                 /*   if (fabs(_projector->getLineStart().Data()[0] - referencePoint) <
                        fabs(_projector->getLineEnd().Data()[0]   - referencePoint))
                        referencePoint = _projector->getLineStart().Data()[0];
                    else
                        referencePoint = _projector->getLineEnd().Data()[0];*/
                    
					float deltaSCale = scale - priScale;
					this->SetScale(this->GetScale() + deltaSCale);

                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
                    cmd->setDeltaScale(deltaSCale);
					cmd->SetScale(this->GetScale());
                    cmd->setScaleCenter(_scaleCenter);
                    //cmd->setReferencePoint(referencePoint);
                    cmd->setMinScale(getMinScale());
					cmd->setTranslation(translateVec3);
 
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();

                    /*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick finish.
        case ( TouchEvent::RELEASE):
            {
                Scale1DCommand* cmd = new Scale1DCommand();
				cmd->AddRef();
				cmd->SetRefDragger(this);
                cmd->setStage(MotionCommand::FINISH);
                cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());

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

void Scale1DDragger::setupDefaultGeometry()
{
    // Get the line length and direction.
    //M3D::Vector3 lineDir = _projector->getLineEnd()-_projector->getLineStart();
    //float lineLength = lineDir.length();
    //lineDir.normalize();

    //M3D::Geode* lineGeode = new M3D::Geode;
    //// Create a line.
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();

    //    M3D::Vec3Array* vertices = new M3D::Vec3Array(2);
    //    (*vertices)[0] = _projector->getLineStart();
    //    (*vertices)[1] = _projector->getLineEnd();

    //    geometry->setVertexArray(vertices);
    //    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::LINES,0,2));

    //    lineGeode->addDrawable(geometry);
    //}
    //
    //// Turn of lighting for line and set line width.
    //lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);
    //M3D::LineWidth* linewidth = new M3D::LineWidth();
    //linewidth->setWidth(2.0f);
    //lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, M3D::StateAttribute::ON);

    //// Add line and cones to the scene.
    //addChild(lineGeode);

    //// Create a left box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(_projector->getLineStart(), 0.05 * lineLength)));
    //    addChild(geode);
    //    setLeftHandleNode(*geode);
    //}
    //
    //// Create a right box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(_projector->getLineEnd(), 0.05 * lineLength)));
    //    addChild(geode);
    //    setRightHandleNode(*geode);
	//}	
	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("ScaleHandler.stl");
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
