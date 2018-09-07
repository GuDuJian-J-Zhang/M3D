

#include "m3d/handler/Translate2DDragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/Parameters.h"
#include "../utils/MeshHelper.h"
#include "../scene/ShapeNode.h"

namespace M3D {

	Translate2DDragger::Translate2DDragger() :Dragger()
{
    _projector = new PlaneProjector(M3D::Plane(Vector3(.0,1.0,0.0),0.0));
	_projector->AddRef();
	_drawModel = NULL;
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
}
       
Translate2DDragger::Translate2DDragger(const M3D::Plane& plane)
{
    _projector = new PlaneProjector(plane);
	_projector->AddRef();

	_drawModel = NULL;
    setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
    setPickColor(M3D::Color(1.0f, 1.0f, 0.0f, 1.0f));
}

Translate2DDragger::~Translate2DDragger()
{
	ReleaseMe(_projector);
}

bool Translate2DDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
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
                    // Generate the motion command.
                    TranslateInPlaneCommand* cmd = new TranslateInPlaneCommand(_projector->getPlane());
					cmd->AddRef();

                    cmd->setStage(MotionCommand::START);
                    cmd->setReferencePoint(_startProjectedPoint);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());

                    // Dispatch command.
                    dispatch(*cmd);
					cmd->Release();

                    // Set color to pick color.
                    /*setMaterialColor(_pickColor,*this);*/
					// getOrCreateStateSet()->setAttributeAndModes(_polygonOffset.get(), M3D::StateAttribute::ON);

                    /*aa.requestRedraw();*/
                }
                return true; 
            }
            
        // Pick move.
        case ( TouchEvent::DRAG):
		{	M3D::Matrix3x4 localToWorld = this->GetWorldTransform();//::computeLocalToWorld(nodePathToRoot);
			_projector->setLocalToWorld(localToWorld);
			M3D::Vector3 projectedPoint;
                if (_projector->project(pointer, projectedPoint))
                {
                    // Generate the motion command.
                    TranslateInPlaneCommand* cmd = new TranslateInPlaneCommand(_projector->getPlane());
					cmd->AddRef();

                    cmd->setStage(MotionCommand::MOVE);
                    cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());
                    cmd->setTranslation(projectedPoint - _startProjectedPoint);
                    cmd->setReferencePoint(_startProjectedPoint);

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
                TranslateInPlaneCommand* cmd = new TranslateInPlaneCommand(_projector->getPlane());
				cmd->AddRef();
                    cmd->setStage(MotionCommand::FINISH);
                cmd->setReferencePoint(_startProjectedPoint);
                cmd->setLocalToWorldAndWorldToLocal(_projector->getLocalToWorld(),_projector->getWorldToLocal());

                // Dispatch command.
                dispatch(*cmd);
				cmd->Release();

                // Reset color.
                /*setMaterialColor(_color,*this);*/
                //getOrCreateStateSet()->removeAttribute(_polygonOffset.get());

                /*aa.requestRedraw();*/

                return true;
            }
        default:
            return false;
    }
}

void Translate2DDragger::setupDefaultGeometry()
{
    // Create a line.
    //M3D::Geode* lineGeode = new M3D::Geode;
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();
    //    
    //    M3D::Vec3Array* vertices = new M3D::Vec3Array(2);
    //    (*vertices)[0] = M3D::Vector3(0.0f,0.0f,-0.5f);
    //    (*vertices)[1] = M3D::Vector3(0.0f,0.0f,0.5f);

    //    geometry->setVertexArray(vertices);
    //    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::LINES,0,2));

    //    lineGeode->addDrawable(geometry);
    //}

    //// Turn of lighting for line and set line width.
    //M3D::LineWidth* linewidth = new M3D::LineWidth();
    //linewidth->setWidth(2.0f);
    //lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, M3D::StateAttribute::ON);
    //lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);

    //M3D::Geode* geode = new M3D::Geode;

    //// Create left cone.
    //{
    //    M3D::Cone* cone = new M3D::Cone (M3D::Vector3(0.0f, 0.0f, -0.5f), 0.025f, 0.10f);
    //    M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f,0.0f,-1.0f), M3D::Vector3(0.0f, 0.0f, 1.0f));
    //    cone->setRotation(rotation);
    //    geode->addDrawable(new M3D::ShapeDrawable(cone));
    //}
    //
    //// Create right cone.
    //{
    //    M3D::Cone* cone = new M3D::Cone (M3D::Vector3(0.0f, 0.0f, 0.5f), 0.025f, 0.10f);
    //    geode->addDrawable(new M3D::ShapeDrawable(cone));
    //}

    //// Create an invisible cylinder for picking the line.
    //{
    //    M3D::Cylinder* cylinder = new M3D::Cylinder (M3D::Vector3(0.0f,0.0f,0.0f), 0.015f, 1.0f);
    //    M3D::Drawable* drawable = new M3D::ShapeDrawable(cylinder);
    //    setDrawableToAlwaysCull(*drawable);
    //    geode->addDrawable(drawable);
    //}

    //// MatrixTransform to rotate the geometry according to the normal of the plane.
    //M3D::MatrixTransform* xform = new M3D::MatrixTransform;

    //// Create an arrow in the X axis.
    //{
    //    M3D::MatrixTransform* arrow = new M3D::MatrixTransform;
    //    arrow->addChild(lineGeode);
    //    arrow->addChild(geode);

    //    // Rotate X-axis arrow appropriately.
    //    M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(1.0f, 0.0f, 0.0f), M3D::Vector3(0.0f, 0.0f, 1.0f));
    //    arrow->setMatrix(M3D::Matrix3x4(rotation));

    //    xform->addChild(arrow);
    //}

    //// Create an arrow in the Z axis.
    //{
    //    M3D::Group* arrow = new M3D::Group;
    //    arrow->addChild(lineGeode);
    //    arrow->addChild(geode);

    //    xform->addChild(arrow);
    //}
    
    // Rotate the xform so that the geometry lies on the plane.
    {
        //M3D::Vector3 normal = _projector->getPlane().m_normal;
        //M3D::Quaternion rotation; rotation=Quaternion(M3D::Vector3(0.0f, 1.0f, 0.0f), normal);
        //xform->setMatrix(M3D::Matrix3x4(rotation));
    }

	string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("PlanXYHandler.stl");
	_drawModel = NULL;
	if (MeshHelper::ReadSingleModel(toolsFilePath, _drawModel))
	{
		if (_drawModel) {
			//int nPlcPath = (int) strtol(plcIdPath, NULL, 16);
			ShapeNode* shapeNode = new ShapeNode();
			this->AddChild(shapeNode);
			_drawModel->SetInitHightlight(true);
			shapeNode->SetShape(_drawModel);
			_drawModel->SetUserData(this);
			_drawModel->Release();
		}
	}
    
    //addChild(xform);
}

}
