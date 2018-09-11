

#include "m3d/handler/TrackballDragger.h"
 
namespace M3D
{

//M3D::Geometry* createCircleGeometry(float radius, unsigned int numSegments)
//{
//    const float angleDelta = 2.0f*M3D::PI/(float)numSegments;
//    const float r = radius;
//    float angle = 0.0f;
//    M3D::Vec3Array* vertexArray = new M3D::Vec3Array(numSegments);
//    M3D::Vec3Array* normalArray = new M3D::Vec3Array(numSegments);
//    for(unsigned int i = 0; i < numSegments; ++i,angle+=angleDelta)
//    {
//        float c = cosf(angle);
//        float s = sinf(angle);
//        (*vertexArray)[i].set(c*r,s*r,0.0f);
//        (*normalArray)[i].set(c,s,0.0f);
//    }
//    M3D::Geometry* geometry = new M3D::Geometry();
//    geometry->setVertexArray(vertexArray);
//    geometry->setNormalArray(normalArray);
//    geometry->setNormalBinding(M3D::Geometry::BIND_PER_VERTEX);
//    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::LINE_LOOP,0,vertexArray->size()));
//    return geometry;
//}

TrackballDragger::TrackballDragger(bool useAutoTransform)
{
    if (useAutoTransform)
    {
        /*float pixelSize = 50.0f;
        M3D::MatrixTransform* scaler = new M3D::MatrixTransform;
        scaler->setMatrix(M3D::Matrix3x4::scale(pixelSize, pixelSize, pixelSize));

        M3D::AutoTransform *at = new M3D::AutoTransform;
        at->setAutoScaleToScreen(true);
        at->addChild(scaler);
        
        AntiSquish* as = new AntiSquish;
        as->addChild(at);
        addChild(as);

        _xDragger = new RotateCylinderDragger();
        scaler->addChild(_xDragger.get());
        addDragger(_xDragger.get());
    
        _yDragger = new RotateCylinderDragger();
        scaler->addChild(_yDragger.get());
        addDragger(_yDragger.get());
    
        _zDragger = new RotateCylinderDragger();
        scaler->addChild(_zDragger.get());
        addDragger(_zDragger.get());

        _xyzDragger = new RotateSphereDragger();
        scaler->addChild(_xyzDragger.get());
        addDragger(_xyzDragger.get());*/
    }
    else
    {
        _xDragger = new RotateCylinderDragger();
        //addChild(_xDragger.get());
        addDragger(_xDragger);
    
        _yDragger = new RotateCylinderDragger();
        //addChild(_yDragger.get());
        addDragger(_yDragger);
    
        _zDragger = new RotateCylinderDragger();
        //addChild(_zDragger.get());
        addDragger(_zDragger);

        _xyzDragger = new RotateSphereDragger();
        //addChild(_xyzDragger.get());
        addDragger(_xyzDragger);
    }

    setParentDragger(getParentDragger());
}
       
TrackballDragger::~TrackballDragger()
{
}

void TrackballDragger::setupDefaultGeometry()
{
    //M3D::Geode* geode = new M3D::Geode;
    //{
    //    M3D::TessellationHints* hints = new M3D::TessellationHints;
    //    hints->setCreateTop(false);
    //    hints->setCreateBottom(false);
    //    hints->setCreateBackFace(false);

    //    M3D::Cylinder* cylinder = new M3D::Cylinder;
    //    cylinder->setHeight(0.15f);
    //    M3D::ShapeDrawable* cylinderDrawable = new M3D::ShapeDrawable(cylinder,hints);
    //    geode->addDrawable(cylinderDrawable);
    //    setDrawableToAlwaysCull(*cylinderDrawable);
    //    geode->addDrawable(createCircleGeometry(1.0f, 100));
    //}

    //// Draw in line mode.
    //{
    //    M3D::PolygonMode* polymode = new M3D::PolygonMode;
    //    polymode->setMode(M3D::PolygonMode::FRONT_AND_BACK,M3D::PolygonMode::LINE);
    //    geode->getOrCreateStateSet()->setAttributeAndModes(polymode,M3D::StateAttribute::OVERRIDE|M3D::StateAttribute::ON);
    //    geode->getOrCreateStateSet()->setAttributeAndModes(new M3D::LineWidth(2.0f),M3D::StateAttribute::ON);

    //    #if !defined(OSG_GLES2_AVAILABLE)
    //        geode->getOrCreateStateSet()->setMode(GL_NORMALIZE, M3D::StateAttribute::ON);
    //    #endif
    //
    //}

    //// Add line to all the individual 1D draggers.
    //_xDragger->addChild(geode);
    //_yDragger->addChild(geode);
    //_zDragger->addChild(geode);


    //// Rotate X-axis dragger appropriately.
    //{
    //    M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
    //    _xDragger->setMatrix(M3D::Matrix3x4(rotation));
    //}

    //// Rotate Y-axis dragger appropriately.
    //{
    //    M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
    //    _yDragger->setMatrix(M3D::Matrix3x4(rotation));
    //}

    //// Send different colors for each dragger.
    //_xDragger->setColor(M3D::Color(1.0f,0.0f,0.0f,1.0f));
    //_yDragger->setColor(M3D::Color(0.0f,1.0f,0.0f,1.0f));
    //_zDragger->setColor(M3D::Color(0.0f,0.0f,1.0f,1.0f));

    //// Add invisible sphere for pick the spherical dragger.
    //{
    //    M3D::Drawable* sphereDrawable = new M3D::ShapeDrawable(new M3D::Sphere());
    //    setDrawableToAlwaysCull(*sphereDrawable);
    //    M3D::Geode* sphereGeode = new M3D::Geode;
    //    sphereGeode->addDrawable(sphereDrawable);

    //    _xyzDragger->addChild(sphereGeode);
    //}
}

}
