

#include "m3d/handler/Translate1DDragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/scene/ShapeNode.h"

namespace M3D {

Translate1DDragger::Translate1DDragger() : Dragger(), _checkForNodeInNodePath(true)
{
    _projector = new LineProjector;
	_projector->AddRef();
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
	
}
       
Translate1DDragger::Translate1DDragger(const M3D::Vector3& s, const M3D::Vector3& e) : Dragger(), _checkForNodeInNodePath(true)
{
    _projector = new LineProjector(s,e);
	_projector->AddRef();
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
}

Translate1DDragger::~Translate1DDragger()
{
	ReleaseMe(_projector);
}

bool Translate1DDragger::handle(const PointerInfo& pointer, const TouchEvent& ea)
{
    // Check if the dragger node is in the nodepath.
    if (_checkForNodeInNodePath)
    {
      //  if (!pointer.contains(this)) return false;
    }

    switch (ea.getEventType())
    {
        // Pick start.
        case (TouchEvent::PUSH):
            {
                // Get the LocalToWorld matrix for this node and set it for the projector.
                //M3D::NodePath nodePathToRoot;
                //computeNodePathToRoot(*this,nodePathToRoot);
				M3D::Matrix3x4 localToWorld = this->GetWorldTransform();//::computeLocalToWorld(nodePathToRoot);
                _projector->setLocalToWorld(localToWorld);

                if (_projector->project(pointer, _startProjectedPoint))
                {
                    // Generate the motion command.
                    TranslateInLineCommand* cmd = new TranslateInLineCommand(_projector->getLineStart(),
                                                                                       _projector->getLineEnd());
					cmd->AddRef();
                    cmd->setStage(MotionCommand::START);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
					cmd->SetRefDragger(this);
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                    // Set color to pick color.
                    ///*setMaterialColor(_pickColor,*this);*/

                    ///*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick move.
        case (TouchEvent::DRAG):
            {
				M3D::Matrix3x4 localToWorld = this->GetWorldTransform();//::computeLocalToWorld(nodePathToRoot);
				_projector->setLocalToWorld(localToWorld);
                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    // Generate the motion command.
					TranslateInLineCommand* cmd = new TranslateInLineCommand(_projector->getLineStart(),
                                                         _projector->getLineEnd());
					cmd->AddRef();
                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
                    cmd->setTranslation(projectedPoint - _startProjectedPoint);
					cmd->SetRefDragger(this);
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                }
                return true; 
            }
            
        // Pick finish.
        case (TouchEvent::RELEASE):
            {
                M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    TranslateInLineCommand* cmd = new TranslateInLineCommand(_projector->getLineStart(),
                            _projector->getLineEnd());
					cmd->AddRef();
                    cmd->setStage(MotionCommand::FINISH);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());

					cmd->SetRefDragger(this);
                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();
                    // Reset color.
                    //setMaterialColor(_color,*this);
                }

                return true;
            }
        default:
            return false;
    }
}

void Translate1DDragger::setupDefaultGeometry()
{
    // Get the line length and direction.
    M3D::Vector3 lineDir = _projector->getLineEnd()-_projector->getLineStart();
    //float lineLength = lineDir.length();
    //lineDir.normalize();

    //M3D::Geode* geode = new M3D::Geode;
    //// Create a left cone.
    //{
    //    M3D::Cone* cone = new M3D::Cone (_projector->getLineStart(), 0.025f * lineLength, 0.10f * lineLength);
    //    M3D::Quaternion rotation;
    //    rotation.makeRotate(lineDir, M3D::Vector3(0.0f, 0.0f, 1.0f));
    //    cone->setRotation(rotation);

    //    geode->addDrawable(new M3D::ShapeDrawable(cone));
    //}

    //// Create a right cone.
    //{
    //    M3D::Cone* cone = new M3D::Cone (_projector->getLineEnd(), 0.025f * lineLength, 0.10f * lineLength);
    //    M3D::Quaternion rotation;
    //    rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), lineDir);
    //    cone->setRotation(rotation);

    //    geode->addDrawable(new M3D::ShapeDrawable(cone));
    //}

    //// Create an invisible cylinder for picking the line.
    //{
    //    M3D::Cylinder* cylinder = new M3D::Cylinder ((_projector->getLineStart()+_projector->getLineEnd())/2, 0.015f * lineLength, lineLength);
    //    M3D::Quaternion rotation;
    //    rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), lineDir);
    //    cylinder->setRotation(rotation);
    //    M3D::Drawable* cylinderGeom = new M3D::ShapeDrawable(cylinder);

    //    setDrawableToAlwaysCull(*cylinderGeom);
    //
    //    geode->addDrawable(cylinderGeom);
    //}

    //M3D::Geode* lineGeode = new M3D::Geode;
    //// Create a line.
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();
    //    
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
    //addChild(geode);
 
	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("AxisHandler.stl");
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

void Translate1DDragger::setupDefaultPointGeometry()
{
	// Get the line length and direction.
	M3D::Vector3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("AxisHandler.stl");
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

}
