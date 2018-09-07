

#include "m3d/handler/Scale2DDragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/Parameters.h"
#include "../utils/MeshHelper.h"
#include "../scene/ShapeNode.h"
 
namespace M3D
{

M3D::Vector2 computeScale(const M3D::Vector3& startProjectedPoint,
                        const M3D::Vector3& projectedPoint,
                        const M3D::Vector2& scaleCenter)
{
    M3D::Vector2 scale(1.0,1.0);
    if ((startProjectedPoint.Data()[0] - scaleCenter.Data()[0]) != 0.0)
        scale.m_x = (projectedPoint.Data()[0] - scaleCenter.Data()[0])/(startProjectedPoint.Data()[0] - scaleCenter.Data()[0]);
    if ((startProjectedPoint.Data()[2] - scaleCenter.Data()[1]) != 0.0)
        scale.m_y = (projectedPoint.Data()[2] - scaleCenter.Data()[1])/(startProjectedPoint.Data()[2] - scaleCenter.Data()[1]);
    return scale;
}

}

Scale2DDragger::Scale2DDragger(ScaleMode scaleMode) : Dragger(), _minScale(0.001,0.001), _scaleMode(scaleMode)
{
    _projector = new PlaneProjector(M3D::Plane(Vector3(.0,1.0,0.0),0.0));
    setColor(M3D::Color(0.0, 1.0, 0.0, 1.0));
    setPickColor(M3D::Color(1.0, 1.0, 0.0, 1.0));

    _topLeftHandlePosition = Vector2(-0.5,0.5);
    _bottomLeftHandlePosition = Vector2(-0.5,-0.5);
    _bottomRightHandlePosition = Vector2(0.5,-0.5);
    _topRightHandlePosition = Vector2(0.5,0.5);

	_projector->AddRef();
}

Scale2DDragger::~Scale2DDragger()
{
	ReleaseMe(_projector);
}

bool Scale2DDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
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

                if (_projector->project(pointer, _startProjectedPoint))
                {
                    _scaleCenter = Vector2(0.0,0.0);

                  /*  if (pointer.contains(_topLeftHandleNode.get())) 
                    {
                        _referencePoint = _topLeftHandlePosition;
                        if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
                            _scaleCenter = _bottomRightHandlePosition;
                    }
                    else if (pointer.contains(_bottomLeftHandleNode.get())) 
                    {
                        _referencePoint = _bottomLeftHandlePosition;
                        if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
                            _scaleCenter = _topRightHandlePosition;
                    }
                    else if (pointer.contains(_bottomRightHandleNode.get())) 
                    {
                        _referencePoint = _bottomRightHandlePosition;
                        if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
                            _scaleCenter = _topLeftHandlePosition;
                    }
                    else if (pointer.contains(_topRightHandleNode.get())) 
                    {
                        _referencePoint = _topRightHandlePosition;
                        if (_scaleMode == SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT)
                            _scaleCenter = _bottomLeftHandlePosition;

                    }*/

                    // Generate the motion command.
                    Scale2DCommand* cmd = new Scale2DCommand();
					cmd->AddRef();
                    cmd->setStage(MotionCommand::START);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
                    cmd->setReferencePoint(_referencePoint);

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
		{
			M3D::Matrix3x4 localToWorld = GetWorldTransform();
			_projector->setLocalToWorld(localToWorld);
                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    // Compute scale.
                    M3D::Vector2 scale = computeScale(_startProjectedPoint,projectedPoint,_scaleCenter);

                    if (scale.Data()[0] < getMinScale().Data()[0]) scale.m_x = getMinScale().Data()[0];
                    if (scale.Data()[1] < getMinScale().Data()[1]) scale.m_y = getMinScale().Data()[1];

                    // Generate the motion command.
                    Scale2DCommand* cmd = new Scale2DCommand();
					cmd->AddRef();
                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
                    cmd->setDeltaScale(scale);
                    cmd->setScaleCenter(_scaleCenter);
                    cmd->setReferencePoint(_referencePoint);
                    cmd->setMinScale(getMinScale());

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
                Scale2DCommand* cmd = new Scale2DCommand();
				cmd->AddRef();
                cmd->setStage(MotionCommand::FINISH);
                cmd->setReferencePoint(_referencePoint);
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

void Scale2DDragger::setupDefaultGeometry()
{
    //M3D::Geode* lineGeode = new M3D::Geode;
    //// Create a line.
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();

    //    M3D::Vec3Array* vertices = new M3D::Vec3Array(4);
    //    (*vertices)[0].set(_topLeftHandlePosition[0],0.0,_topLeftHandlePosition[1]);
    //    (*vertices)[1].set(_bottomLeftHandlePosition[0],0.0,_bottomLeftHandlePosition[1]);
    //    (*vertices)[2].set(_bottomRightHandlePosition[0],0.0,_bottomRightHandlePosition[1]);
    //    (*vertices)[3].set(_topRightHandlePosition[0],0.0,_topRightHandlePosition[1]);

    //    geometry->setVertexArray(vertices);
    //    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::LINE_LOOP,0,vertices->size()));

    //    lineGeode->addDrawable(geometry);
    //}
    //
    //// Turn of lighting for line and set line width.
    //lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);
    //M3D::LineWidth* linewidth = new M3D::LineWidth();
    //linewidth->setWidth(2.0);
    //lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, M3D::StateAttribute::ON);

    //// Add line and cones to the scene.
    //addChild(lineGeode);

    //// Create a top left box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(M3D::Vector3(_topLeftHandlePosition[0],
    //                                                                     0.0,_topLeftHandlePosition[1]), 0.05)));
    //    addChild(geode);
    //    setTopLeftHandleNode(*geode);
    //}
    //
    //// Create a bottom left box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(M3D::Vector3(_bottomLeftHandlePosition[0],
    //                                                                     0.0,_bottomLeftHandlePosition[1]), 0.05)));
    //    addChild(geode);
    //    setBottomLeftHandleNode(*geode);
    //}

    //// Create a bottom right box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(M3D::Vector3(_bottomRightHandlePosition[0],
    //                                                                     0.0,_bottomRightHandlePosition[1]), 0.05)));
    //    addChild(geode);
    //    setBottomRightHandleNode(*geode);
    //}

    //// Create a top right box.
    //{
    //    M3D::Geode* geode = new M3D::Geode;
    //    geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(M3D::Vector3(_topRightHandlePosition[0],
    //                                                                     0.0,_topRightHandlePosition[1]), 0.05)));
    //    addChild(geode);
    //    setTopRightHandleNode(*geode);
    //}
	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("PlanXYHandler.stl");
	Model* tempModel = NULL;
	if (MeshHelper::ReadSingleModel(toolsFilePath, tempModel))
	{
		if (tempModel) {
			//int nPlcPath = (int) strtol(plcIdPath, NULL, 16);
			ShapeNode* shapeNode = new ShapeNode();
			this->AddChild(shapeNode);
			shapeNode->SetShape(tempModel);
			tempModel->SetUserData(this);
			tempModel->Release();
		}
	}
}
