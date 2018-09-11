

#include "m3d/handler/Command.h"
 
#include <algorithm>
 
namespace M3D {

///////////////////////////////////////////////////////////////////////////////
//
// Motion Command base class.
//

MotionCommand::MotionCommand() : _stage(NONE), Object()
{
	this->SetRefDragger(NULL);
}

M3D::Dragger* MotionCommand::GetRefDragger() const
{
	return _refDragger;
}

MotionCommand::~MotionCommand()
{
}

void MotionCommand::SetRefDragger(M3D::Dragger* val)
{
	_refDragger = val;
}

///////////////////////////////////////////////////////////////////////////////
//
// Translate in line command.
//

TranslateInLineCommand::TranslateInLineCommand():MotionCommand()
{
    //_line = new M3D::LineSegment;  
}

TranslateInLineCommand::TranslateInLineCommand(const M3D::Vector3& s,const M3D::Vector3& e)
{
    //_line = new M3D::LineSegment(s,e);
	_line.m_StartPoint = s;
	_line.m_EndPoint = e;
}

TranslateInLineCommand::~TranslateInLineCommand()
{
}

MotionCommand* TranslateInLineCommand::createCommandInverse()
{
   /* TranslateInLineCommand> inverse = new TranslateInLineCommand();
    *inverse = *this;
    inverse->setTranslation(-_translation);
    return inverse.release();*/
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Translate in plane command.
//

TranslateInPlaneCommand::TranslateInPlaneCommand() :MotionCommand()
{
}

TranslateInPlaneCommand::TranslateInPlaneCommand(const M3D::Plane& plane) : _plane(plane)
{
}

TranslateInPlaneCommand::~TranslateInPlaneCommand()
{
}
MotionCommand* TranslateInPlaneCommand::createCommandInverse()
{
 /*   TranslateInPlaneCommand> inverse = new TranslateInPlaneCommand();
    *inverse = *this;
    inverse->setTranslation(-_translation);
    return inverse.release();*/
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Scale 1D command.
//

Scale1DCommand::Scale1DCommand() : _deltaScale(1.0),MotionCommand()
{
	 _deltaScale =1.0f ;
	 _scaleCenter = 0.0f;
	 _referencePoint =0.0f ;
	 _minScale =1.0f ;
	 _scale = 1.0f;
}

Scale1DCommand::~Scale1DCommand()
{
}

MotionCommand* Scale1DCommand::createCommandInverse()
{
    //Scale1DCommand> inverse = new Scale1DCommand();
    //*inverse = *this;
    //if (_scale) inverse->setScale(1.0/_scale);
    //return inverse.release();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Scale 2D command.
//

Scale2DCommand::Scale2DCommand() : _scale(1.0,1.0) , MotionCommand()
{
}

Scale2DCommand::~Scale2DCommand()
{
}

MotionCommand* Scale2DCommand::createCommandInverse()
{
    //Scale2DCommand> inverse = new Scale2DCommand();
    //*inverse = *this;
    //if (_scale[0] && _scale[1])
    //    inverse->setScale(M3D::Vec2(1.0/_scale[0],1.0/_scale[1]));
    //return inverse.release();

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Scale uniform command.
//

ScaleUniformCommand::ScaleUniformCommand() : _scale(1.0) , MotionCommand()
{
}

ScaleUniformCommand::~ScaleUniformCommand()
{
}

MotionCommand* ScaleUniformCommand::createCommandInverse()
{
    //ScaleUniformCommand> inverse = new ScaleUniformCommand();
    //*inverse = *this;
    //if (_scale) inverse->setScale(1.0/_scale);

    //return inverse.release();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// Rotate 3D command.
//

Rotate3DCommand::Rotate3DCommand() :MotionCommand()
{
}

Rotate3DCommand::~Rotate3DCommand()
{
}

MotionCommand* Rotate3DCommand::createCommandInverse()
{
    //Rotate3DCommand> inverse = new Rotate3DCommand();
    //*inverse = *this;
    //inverse->setRotation(_rotation.inverse());
    //return inverse.release();
	return NULL;
}

}