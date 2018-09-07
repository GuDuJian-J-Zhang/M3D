

#include "m3d/handler/TabBoxTrackballDragger.h"

using namespace M3D;

TabBoxTrackballDragger::TabBoxTrackballDragger()
{
    _trackballDragger = new TrackballDragger(true);
    addChild(_trackballDragger.get());
    addDragger(_trackballDragger.get());
    
    _tabBoxDragger = new TabBoxDragger();
    addChild(_tabBoxDragger.get());
    addDragger(_tabBoxDragger.get());

    setParentDragger(getParentDragger());
}

TabBoxTrackballDragger::~TabBoxTrackballDragger()
{
}

void TabBoxTrackballDragger::setupDefaultGeometry()
{
    _trackballDragger->setupDefaultGeometry();
    _tabBoxDragger->setupDefaultGeometry();
}
