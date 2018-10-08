/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef  M3D_GL_
#define  M3D_GL_

#ifdef __ANDROID__
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define M3D_GL_INDEX_TYPE GL_UNSIGNED_SHORT 
#define DEPTH_COMPONENT GL_DEPTH_COMPONENT16
#endif

#ifdef __IOS__
#include <OpenGLES/es1/gl.h>
#include <OpenGLES/es1/glext.h>
#include <OpenGLES/es2/gl.h>
#include <OpenGLES/es2/glext.h>
#define GLenum unsigned int

#define M3D_GL_INDEX_TYPE GL_UNSIGNED_SHORT 
#define DEPTH_COMPONENT GL_DEPTH_COMPONENT16
#endif

#ifdef WIN32
#define M3D_GL_INDEX_TYPE GL_UNSIGNED_INT 
#include "m3d\renderer\GL\glew.h"
#define DEPTH_COMPONENT GL_DEPTH_COMPONENT24
#ifdef GLEW_MX
extern GLEWContext _glewctx;
#define glewGetContext() (&_glewctx)
#endif
#endif

#endif // #ifndef _GL_INC_
