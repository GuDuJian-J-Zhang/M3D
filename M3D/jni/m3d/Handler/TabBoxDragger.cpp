

#include "m3d/handler/TabBoxDragger.h"


using namespace M3D;

TabBoxDragger::TabBoxDragger()
{
    for (int i=0; i<6; ++i)
    {
        _planeDraggers.push_back(new TabPlaneDragger());
        addChild(_planeDraggers[i].get());
        addDragger(_planeDraggers[i].get());
    }

    {
        _planeDraggers[0]->setMatrix(M3D::Matrix3x4::translate(M3D::Vector3(0.0,0.5,0.0)));
    }
    {
        M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, -1.0f, 0.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
        _planeDraggers[1]->setMatrix(M3D::Matrix3x4(rotation)*M3D::Matrix3x4::translate(M3D::Vector3(0.0,-0.5,0.0)));
    }
    {
        M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
        _planeDraggers[2]->setMatrix(M3D::Matrix3x4(rotation)*M3D::Matrix3x4::translate(M3D::Vector3(0.0,0.0,-0.5)));
    }

    {
        M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 1.0f, 0.0f), M3D::Vector3(0.0f, 0.0f, 1.0f));
        _planeDraggers[3]->setMatrix(M3D::Matrix3x4(rotation)*M3D::Matrix3x4::translate(M3D::Vector3(0.0,0.0,0.5)));
    }

    {
        M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(1.0f, 0.0f, 0.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
        _planeDraggers[4]->setMatrix(M3D::Matrix3x4(rotation)*M3D::Matrix3x4::translate(M3D::Vector3(-0.5,0.0,0.0)));
    }

    {
        M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 1.0f, 0.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
        _planeDraggers[5]->setMatrix(M3D::Matrix3x4(rotation)*M3D::Matrix3x4::translate(M3D::Vector3(0.5,0.0,0.0)));
    }

    setParentDragger(getParentDragger());
}
       
TabBoxDragger::~TabBoxDragger()
{
}

void TabBoxDragger::setupDefaultGeometry()
{
    for (unsigned int i=0; i<_planeDraggers.size(); ++i)
        _planeDraggers[i]->setupDefaultGeometry(false);
}

void TabBoxDragger::setPlaneColor(const M3D::Color& color)
{
    for (unsigned int i=0; i<_planeDraggers.size(); ++i)
        _planeDraggers[i]->setPlaneColor(color);
}
