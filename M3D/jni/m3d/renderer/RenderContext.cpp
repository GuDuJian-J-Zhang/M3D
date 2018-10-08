#include "m3d/model/Line3D.h"
#include "m3d/base/Vector3.h"
#include "RenderContext.h"
#include "m3d/renderer/gl10/GL10Context.h"
#include "m3d/renderer/GL20/GL20Context.h"
#include "m3d/renderer/GL20/GLDrawer20.h"

namespace M3D
{

RenderContext* RenderContext::GetContext(int GLType)
{
#ifdef WIN32
	GLenum err = glewInit();  // 前面运行了glut*的一系列函数，已经获得了opengl的context，所以这里不会出错，读者可以试试在main的开始就调用这句会怎样  
	if( err != GLEW_OK ) {  
		LOGE("%s\n", glewGetErrorString( err ) );  
	}  
#endif

	if (GLType == GL10)
	{
		return new GL10Context();
	}
	else if (GLType == GL20)
	{
		return new GL20Context();
	}
	else
	{
		LOGE("GLHelper create error");
	}

	return NULL;
}

RenderContext::RenderContext()
{

}

RenderContext::~RenderContext()
{
        
}

void RenderContext::SetViewMatrix(const Matrix4& viewMatrix)
{
	this->m_viewMatrix = viewMatrix;
}

void RenderContext::SetProjectMatrix(const Matrix4& projectMatrix)
{
	this->m_projectMatrix = projectMatrix;
}

Matrix4& RenderContext::GetViewMatrix()
{
	return this->m_viewMatrix;
}

Matrix4& RenderContext::GetProjectMatrix()
{
	return this->m_projectMatrix;
}

Matrix4& RenderContext::GetModelMatrix()
{
	return this->m_modelMatrixCache;
}

}
