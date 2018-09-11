

#include "m3d/handler/TabPlaneTrackballDragger.h"


using namespace M3D;

TabPlaneTrackballDragger::TabPlaneTrackballDragger()
{
    _trackballDragger = new TrackballDragger(true);
    addChild(_trackballDragger.get());
    addDragger(_trackballDragger.get());
    
    _tabPlaneDragger = new TabPlaneDragger();
    addChild(_tabPlaneDragger.get());
    addDragger(_tabPlaneDragger.get());

    setParentDragger(getParentDragger());
}
       
TabPlaneTrackballDragger::~TabPlaneTrackballDragger()
{
}

void TabPlaneTrackballDragger::setupDefaultGeometry()
{
    _trackballDragger->setupDefaultGeometry();
    _tabPlaneDragger->setupDefaultGeometry();
}
