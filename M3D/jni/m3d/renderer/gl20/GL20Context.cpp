#include "GL20Context.h"

namespace M3D
{
GL20Context::GL20Context()
{

}

GL20Context::~GL20Context()
{

}
int GL20Context::GetContextType()
{
	return 2;
}


void GL20Context::GetDoublev(GLenum pname, double *params)
{

}

void GL20Context::GetFloatv(GLenum pname, float *params)
{

}
//
void GL20Context::GetDoublev(GLenum pname, Matrix4& m)
{

}
void GL20Context::GetFloatv(GLenum pname, Matrix4& m)
{

}
}
